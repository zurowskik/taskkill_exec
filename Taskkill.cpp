

#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <TlHelp32.h>

int ExitCode = 0;
HANDLE proc_handle;
int pid = 0;
LPWSTR lpImage;
TCHAR* ProcessName;
TCHAR temp[256];
PROCESSENTRY32 peInfo;




//Finds the PID from the Process Name
int findProcess(char* input) {


    swprintf(temp, 256, L"%hs", input);

    HANDLE  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    peInfo.dwSize = sizeof(peInfo);
    BOOL nextProcess = Process32First(hProcessSnap, &peInfo);

    do {

        if (!wcscmp(peInfo.szExeFile, temp)) {
            _tprintf(TEXT("\nProcess found.  Process ID = %d"), peInfo.th32ProcessID);
            return peInfo.th32ProcessID;

        }

    } while (Process32Next(hProcessSnap, &peInfo));

    printf("No Process found with that name");
    exit(1);

}

//CCreate a Snapshot of the sysetm and searches for 
//   Processes whose PPID match the specified process
void findChildren(int parent) {

    HANDLE  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    peInfo.dwSize = sizeof(peInfo);
    BOOL nextProcess = Process32First(hProcessSnap, &peInfo);

    do {

        if (peInfo.th32ParentProcessID == parent) {
            _tprintf(TEXT("\nProcess found. Process ID = %d"), peInfo.th32ProcessID);

            proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, peInfo.th32ProcessID);
            TerminateProcess(proc_handle, ExitCode);
            break;
        }

    } while (Process32Next(hProcessSnap, &peInfo));

}

int main(int argc, char* argv[]) {

    //Makes sure at least one parameter is provided
    if (argc == 1) {
        printf("Not enough arguments- must specify process to terminate");
        exit(1);
    }


    for (int i = 1; i < argc; i++) {
        //Checks for the PID Flag
        if (!strcmp(argv[i], "/PID")) {
            //advances to the PID flag
            i++;
            pid = atoi(argv[i]);
        }
        //Checks for the verbose Flag
        if (!strcmp(argv[i], "/IM")) {
            i++;
            //Advances to the Image name
            pid = findProcess(argv[i]);
        }
        //Checks for the Force Flag
        if (!strcmp(argv[i], "/F")) {
            ExitCode = 1;
        }
        //Checks for the Child Thread
        if (!strcmp(argv[i], "/T")) {
            findChildren(pid);
        }
        //Checks for the Remote Computer Flag
        //Not currently implemented
        if (!strcmp(argv[i], "/S")) {
            return 0;
        }
        //Checks for the User Flag
        //Not Currently Implemented
        if (!strcmp(argv[i], "/U")) {
            return 0;
        }
    }

    //Makes sure that a process was provided
    if (pid == 0) {
        printf("No process specified");
        exit(1);
    }

    //Performs the API call to terminate the process
    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    TerminateProcess(proc_handle, ExitCode);


    return 0;
}
