#include "./duk/duktape.h"
#include <conio.h>
#include "vb.h"
#include "dbg.h"
#include <windows.h>

//hacked into duk_js_executor.c duk_js_execute_bytecode()
int DZZIE_FORCE_QUIT = 0;
int ENABLE_REMOTE_DEBUG = 0;

//#include "./duk/duk_hthread.h"

/*
api tidbits:
	DUK_EXTERNAL_DECL duk_int_t duk_get_type(duk_context *ctx, duk_idx_t index);
	duk_is_none(), which would indicate whether index it outside of stack,
	is not needed; duk_is_valid_index() gives the same information.


modifications to duktape from origial (base 1.2.1)
--------------------------------------------------------------
	duk__debug_process_message       DUK_INTERNAL /*DUK_LOCAL* / void    - changed access so can call from main.c
	duk_debug_write_strbuf           #ifdef DUK_DBG_USE_PACKED_PROTOCOL  - to simplify protocol 
	duk_debug_write_int              #ifdef DUK_DBG_USE_PACKED_PROTOCOL  - to simplify protocol

*/

#pragma comment(lib, "ws2_32.lib")
#define EXPORT comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)

#define real_fwrite fwrite

int ScriptTimeoutCheck(const void*udata); 

#define DUK_OPT_EXEC_TIMEOUT_CHECK  ScriptTimeoutCheck
#define DUK_OPT_INTERRUPT_COUNTER 1


vbCallback vbStdOut = 0;
vbHostResolverCallback vbHostResolver = 0;
vbDbgCallback vbLineInput = 0;
vbDbgCallback vbDbgWriteHandler = 0;
vbDbgCallback vbDbgReadHandler;

char* mLastString = 0;

enum opDuk{
	opd_PushUndef = 0,
	opd_PushNum =1,
	opd_PushStr =2,
	opd_GetInt=3,
	opd_IsNullUndef=4,
	opd_GetString=5,
	opd_Destroy=6,
	opd_LastString=7,
	opd_ScriptTimeout=8, //no longer used
	opd_debugAttach=9,
	opd_dbgCoOp = 10,
	opd_dbgTriggerRead = 12,
	opd_PushBool = 13,
	opd_forceQuit = 14,
	opd_remoteDebug = 15
};

int inCreate = 0 ;
unsigned int startTime = 0;
unsigned int lastRefresh = 0;

duk_size_t DebugRead(void *udata, char *buffer, duk_size_t length);
duk_size_t DebugWrite(void *udata, const char *buffer, duk_size_t length);
void DebugDetached(void *udata);


//returns 0 for success, -1 for error
int __stdcall DukOp(int operation, duk_context *ctx, int arg1, char* arg2){
#pragma EXPORT
	
	//these do not require a context..
	switch(operation){

		case opd_remoteDebug:
				ENABLE_REMOTE_DEBUG = arg1;
				return 0;

		case opd_forceQuit: 
				DZZIE_FORCE_QUIT = 1;
				return 0;

		case opd_LastString: return mLastString;

		case opd_ScriptTimeout: return 0; //no longer used done in vb side now..(less layers)
				//watchdogTimeout = arg1; 
				
	}

	if(ctx == 0) return -1;

	switch(operation){
		case opd_PushUndef: duk_push_undefined(ctx); return 0;
		case opd_PushNum: duk_push_number(ctx,arg1); return 0;
		case opd_PushStr: duk_push_string(ctx, arg2); return 0;
		case opd_GetInt: return duk_to_number(ctx, arg1);
		case opd_IsNullUndef: return (int)duk_is_null_or_undefined(ctx, arg1);
		case opd_GetString: return (int)duk_safe_to_string(ctx, arg1);
		case opd_Destroy: duk_destroy_heap(ctx); ctx = 0; return 0;
		case opd_dbgCoOp: duk_debugger_cooperate(ctx); return 0;
		case opd_PushBool: duk_push_boolean(ctx, (arg1==0 ? 0 : 1) ); return 0; //(0==false, 1==true, vbtrue = -1 ok)
		case opd_debugAttach:
			if(arg1==1){
				if(vbDbgReadHandler==0 || vbStdOut==0 || vbDbgWriteHandler==0) return -1;
				duk_debugger_attach(ctx, DebugRead, DebugWrite, 0, 0,0, DebugDetached,0);
			}
  		    else duk_debugger_detach(ctx);
			return 0;

		/*case opd_dbgCurLine: return duk_debug_curr_line(ctx); */
		case opd_dbgTriggerRead: duk__debug_process_message(ctx); return 0;
	}

	return -1;

}


void __stdcall SetCallBacks(void* lpfnMsgHandler, void* lpfnDbgHandler, void* lpfnHostResolver, void* lpfnLineInput, void* lpfnDbgWriteHandler){
#pragma EXPORT
	vbStdOut     = (vbCallback)lpfnMsgHandler;
	vbDbgReadHandler = (vbDbgCallback)lpfnDbgHandler;
	vbHostResolver = (vbHostResolverCallback)lpfnHostResolver;
	vbLineInput = (vbDbgCallback)lpfnLineInput;
	vbDbgWriteHandler = (vbDbgCallback)lpfnDbgWriteHandler;
}

int __stdcall setLastString(const char* s){ //accepts null string to just free last buffer
#pragma EXPORT
	if(mLastString != 0){ free(mLastString); mLastString = 0 ;}
	if(s==0) return 0;
	mLastString = strdup(s);
	return strlen(s);
}


//return 1 to stop, 0 to continue..had problems with 1 not always stopping through!
//only called every so often - exported strictly for debugging purposes not for use with vb!
//https://github.com/svaarala/duktape/blob/master/doc/sandboxing.rst#bytecode-execution-timeout-details
int ScriptTimeoutCheck(const void *udata)
{ 
#pragma EXPORT 

	if(inCreate){
		dbgmsg("dukdll: timeout check in create ignored");	
		return 0; //fatal exception if this returns 1 during heap creation..
	}

	if(vbStdOut){
		int r = vbStdOut(cb_timeoutCheck,0);
		dbgmsgf("dukdll: timeout check vb returned: %d",r);	
		return r;
	}else{
		dbgmsg("dukdll: no vbstdout!");
		return 0;
	}
 
}

static void sandbox_fatal(duk_context *ctx, duk_errcode_t code, const char *msg) {

	char buf[255]={0};
	char *def = "no message";

	if(msg==0) msg = def;

	if(strlen(msg) < 200){
		sprintf(buf, "%ld: %s", (long)code, msg);
		if(vbStdOut){
			vbStdOut(cb_Fatal, buf);
		}else{
			MessageBox(0,buf,"Fatal Error In DukTape",0);
		}
	}else{
		if(vbStdOut){
			vbStdOut(cb_Fatal, msg);
		}else{
			MessageBox(0,msg,"Fatal Error In DukTape",0);
		}
	}
	
	exit(1);  /* must not return */
}


duk_ret_t js_dtor(duk_context *ctx)
{
	//int hInst = 0; hInst = duk_to_number(ctx, -1); 
	const char* hInst = 0;

    // The object to delete is passed as first argument
	//retrieve this.hInst numeric value
    duk_get_prop_string(ctx, 0, "hInst");
	hInst = duk_to_string(ctx, -1);
	if(vbStdOut!=0 && hInst!=0) vbStdOut(cb_ReleaseObj,hInst); 
	duk_pop(ctx); 
    return 0;

}

int __stdcall DukPushNewJSClass(duk_context *ctx, char* className, int hInst){
#pragma EXPORT
	if(ctx == 0) return -1;
	duk_get_global_string(ctx, className);
	duk_new(ctx, 0);

	if(hInst!=0){
		//its a live COM object instance we creating a js obj for..so set the hInst
		//and register a destructor function for it so we can cleanup when js no longer needs it..
		//set this.hInst = 12345
		duk_push_number(ctx, hInst); 
		duk_put_prop_string(ctx, -2, "hInst"); 

		//register a C function to run when js obj released
		duk_push_c_function(ctx, js_dtor, 1);
		duk_set_finalizer(ctx, -2);	 
	}

	return 0;

}


int comResolver(duk_context *ctx) {
	int i, hasRetVal ;
	const char* meth = 0;
	int realArgCount = 0;
	int hInst = 0;

	int n = duk_get_top(ctx);  //padded number of args..not usable for us here..
	
	if(n < 3) return 0; //we require at least 3 args for this function..
	if(vbHostResolver==NULL) return 0; 
	
	meth = duk_safe_to_string(ctx, 0);   //arg0 is obj.method string
	realArgCount = duk_to_number(ctx,1); //arg1 is arguments.length
	hInst = duk_to_number(ctx,2);       //arg2 is this com objects hinst variable if not a top level obj (0 if not)
	hasRetVal = vbHostResolver(meth, ctx, realArgCount, hInst);

	if(hasRetVal != 0 && hasRetVal != 1){
			MessageBox(0,"comresolver","vbdev the hasRetVal must be 0 or 1",0);
			hasRetVal = 1;
	}

	return hasRetVal;  
}

int prompt(duk_context *ctx){
	//prompt(text,defaultText)
	const char* meth = 0;
	int i, hasRetVal ;
	int n = duk_get_top(ctx);  /* #args */

	if(n < 0) return 0;
	if(vbLineInput==NULL) return 0; 
	
	meth = duk_safe_to_string(ctx, 0); 
	hasRetVal = vbLineInput(meth, ctx);
	return 1;
}

static duk_ret_t my_alert(duk_context *ctx) {
	//https://github.com/svaarala/duktape/blob/master/api-testcases/test-print-replacement.c
	const char* msg = 0;
	int n = duk_get_top(ctx);  /* #args */
	if(n < 0) return 0;
	msg  = duk_safe_to_string(ctx, 0); 
	if(vbStdOut==NULL) return 0; 
	vbStdOut(cb_Alert,msg);
	return 0;
}

void RegisterNativeHandlers(duk_context *ctx){
	
	duk_push_global_object(ctx);
	duk_push_c_function(ctx, comResolver, DUK_VARARGS);
	duk_put_prop_string(ctx, -2, "resolver");
	duk_pop(ctx);  /* pop global */

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, prompt, 2);
	duk_put_prop_string(ctx, -2, "prompt");
	duk_pop(ctx);  /* pop global */

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, my_alert, 2);
	duk_put_prop_string(ctx, -2, "alert");
	duk_pop(ctx);  /* pop global */

	//duk_eval_string(ctx, "Duktape.Logger.clog.l = 0;");duk_pop(ctx); //change log level
	duk_eval_string(ctx, "Duktape.Logger.prototype.raw = print");
	duk_pop(ctx);

	/* ReferenceError: does not include a line number
	   SyntaxError: always include line number.
	   err type is not part of err.message, (line is not part of err.message either not sure how to detect type, but 
	   easy answer is to just always add it for standardization... but sometimes we get a 2x display bug with this code.. 
	   this is easy enough..we will just detect it in the ocx and strip it.
	*/
	duk_eval_string(ctx,"Duktape.errCreate = function (err) "
		              " {\n"  
					  "  try { \n"
					  "   if (typeof err === 'object' &&\n"
					  "       typeof err.message !== 'undefined' &&\n"
					  "        typeof err.lineNumber === 'number')"
	                  "      { \n"
					  "        //alert(err.message.indexOf('ReferenceError:')+' '+err.message);\n"
					  "		   err.message = err.message + ' (line ' + err.lineNumber + ')';\n"
					  "      } \n"
					  "  } catch (e) { \n"
					  "    // ignore; for cases such as where message is not writable etc\n"
					  "  } \n"
					  "  return err;\n"
					  " }");
	duk_pop(ctx);   

}

int __stdcall DukCreate(){
#pragma EXPORT
	//duk_context *ctx = duk_create_heap_default();
	duk_context *ctx = 0;
	inCreate = 1;
	DZZIE_FORCE_QUIT = 0;
	dbgmsg("dukdll: DukCreate");
	ctx = duk_create_heap(0, 0, 0, ScriptTimeoutCheck, sandbox_fatal);
	inCreate = 0;
	RegisterNativeHandlers(ctx);
	return (int)ctx;
}


int __stdcall AddFile(duk_context *ctx, char* pth){
#pragma EXPORT
	int rv;
	char* l;
	if(ctx == 0) return -1;
	startTime = GetTickCount();
	rv = duk_peval_file(ctx, pth); //0 = success
	if(rv != 0){
		setLastString(duk_safe_to_string(ctx, -1)); //error message..
	}
	duk_pop(ctx);  /* ignore result */
	return rv;
}

int __stdcall Eval(duk_context *ctx, char* js ){
#pragma EXPORT

	int rv = 0;

	if(ctx == 0) return -1;

	//safe to call eval to avoid fatal panic handler on syntax error
	duk_push_string(ctx, js);
	startTime = GetTickCount();
	if (duk_peval(ctx) != 0) {
		setLastString(duk_safe_to_string(ctx, -1));
		rv = -1;
	} else {
		setLastString(duk_safe_to_string(ctx, -1));
	}

	duk_pop(ctx);
	return rv;

}

static void DebugDetached(void *udata) {
	if(vbStdOut==0) return;
	vbStdOut(cb_debugger, "Debugger-Detached");
}

//debugger is requesting a command to operate on..vb blocks until user enters command..
duk_size_t DebugRead(void *udata, char *buffer, duk_size_t length){	 
	return vbDbgReadHandler(buffer, length);
}

//debugger is sending our interface data 
duk_size_t DebugWrite(void *udata, const char *buffer, duk_size_t length){	
	return vbDbgWriteHandler(buffer, length);
}


int my_fwrite( const void *buf, size_t size, size_t count, FILE* fp){
	
	int sz = size * count;

	if(vbStdOut != NULL && (fp == stdout || fp == stderr) ){
		vbStdOut( (fp == stdout ? cb_output : cb_error) , (char*)buf);
	}
	else real_fwrite(buf, size, count, fp);
	
	return 0;
}
 



