# taskkill_exec

This program simulates the actions of the Windows Executable taskkill.exe. It is based on the Windows32 API function TerminateProcess(). On a high level, it’s main functionality processes the user input into the correct format for this API call.

How the Program Operates:
- The tool finds the PID of the desired process
- The User may input the PID directly.
- The User may input an Executable name. In order to generate the PID from an executable name, the tool takes a snapshot of the system with CreateToolhelp32Snapshot() and iterates through the list of processes until it finds the matching executable (when /T is selected, CreateToolhelp32Snapshot() is called, and the iteration looks for the PID of the parent process directly kills the process from there)
- The tool fetches the handle of the process
- The tool calls TerminateProcess() to terminate the function


Optional Flags:

**/PID**: Takes the process PID as the input

**/IM**: Takes the process executable name as the input

**/F**: Forces the process to exit, provides an exit code 1

**/T**: Kills any child processes as well

**/S computer-name**: displays the processes running on the remote computer specified in computer-name

**/U username**: runs the process with the permissions of the user specified in username. This can only be run if /S is also specified
The user will also be required to input a password for the specified user when prompted (if the user does not have a password, it can be left blank)
