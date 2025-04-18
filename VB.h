#ifndef __VB6_H__
#define __VB6_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

typedef enum{ 
	cb_output=0, 
	cb_Refresh = 1,
    cb_Fatal = 2,
	cb_engine = 3,
	cb_error = 4,
	cb_ReleaseObj = 5,
	cb_StringReturn = 6,
	cb_debugger = 7,
	cb_Alert = 8,
	cb_timeoutCheck = 9
} cb_type;

//Public Sub vb_stdout(ByVal t As cb_type, ByVal lpMsg As Long)
typedef int (__stdcall *vbCallback)(cb_type, char*);

//Public Function GetDebuggerCommand(ByVal buf As Long, ByVal sz As Long) As Long
typedef int (__stdcall *vbDbgCallback)(char*, int);

typedef int (__stdcall *vbHostResolverCallback)(char*, int, int, int); //*string, dukCtx, arg_cnt, hInst

extern vbCallback vbStdOut;
extern vbDbgCallback vbDbgReadHandler;
extern vbDbgCallback vbDbgWriteHandler;
extern vbHostResolverCallback vbHostResolver;
extern vbDbgCallback vbLineInput;  


//we override these to force all outputs to the vb gui. not sure the internal hook thing
//catches all
/*
	#define	printf my_printf 
	#define	fprintf my_fprintf

	extern int my_printf(char* format, ...);
	extern int my_fprintf(FILE* fp, char* format, ...);
*/

#ifdef __cplusplus
}
#endif
#endif
