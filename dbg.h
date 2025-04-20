#ifndef __dbg_H__
#define __dbg_H__ 1
#ifdef  __cplusplus
extern "C" {
#endif

#include <Windows.h>
#include <stdio.h>

//http://www.vbforums.com/showthread.php?874127-Persistent-Debug-Print-Window

extern int ENABLE_REMOTE_DEBUG;

int Warned = 0;
HWND hServer=0;

HWND regFindWindow(void){

	 char* baseKey = "Software\\VB and VBA Program Settings\\dbgWindow\\settings";
	 char tmp[20] = {0};
     unsigned long l = sizeof(tmp);
	 HWND ret=0;
	 HKEY h;
	 
	 printf("regFindWindow triggered\n");

	 RegOpenKeyExA(HKEY_CURRENT_USER, baseKey, 0, KEY_READ, &h);
	 RegQueryValueExA(h, "hwnd", 0,0, (unsigned char*)tmp, &l);
	 RegCloseKey(h);
	
	 ret = (HWND)atoi(tmp);
	 if(!IsWindow(ret)) ret = 0;
	 return ret;
}

void FindVBWindow(){
	char *vbIDEClassName = "ThunderFormDC" ;
	char *vbEXEClassName = "ThunderRT6FormDC" ;
	char *vbEXEClassName2 = "ThunderRT6Form" ;
	char *vbWindowCaption = "Persistent Debug Print Window" ;

	hServer = FindWindowA( vbIDEClassName, vbWindowCaption );
	if(hServer==0) hServer = FindWindowA( vbEXEClassName, vbWindowCaption );
	if(hServer==0) hServer = FindWindowA( vbEXEClassName2, vbWindowCaption );
	if(hServer==0) hServer = regFindWindow(); //if ide is running as admin

	if(hServer==0){
		if(!Warned){
			//MessageBox(0,"Could not find msg window","",0);
			Warned=1;
		}
	}
	else{
		if(!Warned){
			//first time we are being called we could do stuff here...
			//printf("hServer = %x\n", hServer);
			Warned=1;

		}
	}	

} 

int dbgmsg(char *Buffer){
  
  int ret = 0;
  COPYDATASTRUCT cpStructData;

  if(!ENABLE_REMOTE_DEBUG) return;

  if(!IsWindow(hServer)) hServer=0;
  if(hServer==0) FindVBWindow();
  
  memset(&cpStructData,0, sizeof(struct tagCOPYDATASTRUCT )) ;
  
  //_snprintf(msgbuf, 0x1000, "%x,%x,%s", myPID, GetCurrentThreadId(), Buffer);

  cpStructData.dwData = 3;
  cpStructData.cbData = strlen(Buffer) ;
  cpStructData.lpData = (void*)Buffer;
  ret = SendMessage(hServer, WM_COPYDATA, 0,(LPARAM)&cpStructData);
  return ret; //log ui can send us a response msg to trigger special reaction in ret

} 

void dbgmsgf(const char *format, ...)
{
	char buf[1024]; 
	va_list args; 
	DWORD dwErr = GetLastError();
		
	if(!ENABLE_REMOTE_DEBUG) return;

	if(format){
		
		va_start(args,format); 
		//try{
			 _vsnprintf(buf,1024,format,args);
			 dbgmsg(buf);
		//}
		//catch(...){}
	}

	SetLastError(dwErr);
}

#ifdef __cplusplus
}
#endif
#endif