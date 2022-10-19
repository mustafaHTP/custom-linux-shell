
# Custom Linux Shell
 2022 Spring Semester System Programming Lecture Project

## Overview

<p align="center">
	<img src="/img/myShell.gif" />
</p>

This shell simply takes the command from user and runs it. But, some commands like ``cd`` don't work.
In this program, generally, the following system calls were used
* fork system call &rarr; creates a new processes
* exec family functions &rarr; to replace the current running process with the new process
* pipe &rarr; to provide communication between processes
* wait &rarr; to synchronize parent and child processes
* dup, dup2 &rarr; to direct std_out stream to pipes

## To Build and Run
You need linux environment and the packages.
> In Debian/Ubuntu, To install the packages type  ``sudo apt install build-essential`` 

Then change directory to the folder where you download this project and type ``make`` then ``./shell`` and  you're ready.

## How Custom Shell Works

1. Print ">>"
2. Fetch the command from user
3. Try to find the command in the system
4. To find the command, create pipe which is going to be used for communication between processes by using ``pipe`` system call  and create child process by ``fork`` system call
5. In the child process, direct **std_out** to **pipe** with the help of ``dup2`` system call and then pass the command entered by the user to ``execl`` function with **whereis** argument. Thus, output is written to pipe. 
6. In the parent process, read pipe 
	- If the command is not found, then print ***"commandName : not found"***
	- If the command is found, to run the command
		- create a new process and then pass the command and its arguments to **execv** system call. After run the command, turn **step 1**. 
