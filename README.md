# SO

## Description

The main purpose of this application is to interpret files *. nb*. These files have comments, shell commands that will be executed and the results of each command.

This program will run each shell command will put the result beneath the command. If there were previous results, those will be replaced by the new results.

The process of interpreting consists in these steps:
* Read each line of the *. nb* file and filter it:
    * Verify if it is a command or its description (comment)    
    * Storage it in the proper dynamic array
* Execute every command in the commands dynamic array.

A command can use all the shell syntax. If a command starts with the pipe symbol ( *|* ), then it will use the results of the previous command as arguments.

To execute the commands in the proper order, it was created a binary tree that stores strings (*char \**).

## Compiling and Executing

To compile, run `make`.

To execute, run `./notebook <file>`, where `<file>` is the path to the *.nb* file.

## Developers

* [Luís Macedo](https://github.com/FallenFoil)  
* [Carlos Castro](https://github.com/carlosc20)  
* [Daniel Costa](https://github.com/Dans24)