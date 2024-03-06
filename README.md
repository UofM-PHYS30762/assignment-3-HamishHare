[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/HsF5FN7D)
This is the template for the third assignment. 

# Notes

The template is prepared to have everything in a single file, since splitting in interface and implementation gives challenge marks and the students should be using elements from the pre-lecture to make that compile (tip: remember also include guards like #IFNDEF to avoid including the headers too many times). 

# Tips on checking compilation 

Note: only command-line compilation works on Windows, unless you install MinGW's makefile.exe. For mac, you have to make sure that when you write "which g++" you don't get "clang" in your output - if you do, you should edit the Makefile so that the g++-11 is the one you installed with brew. 

If you want to compile your code in the same way the demonstrators will compile it to check it, you can just write "make" in the console. 
This will use a the combination of the `make` command with a `Makefile` file that you will see in the same folder. In this case, doing this which in this case will run the following command:

`g++-11 assignment-3.cpp -o assignment-3.o -std=gnu++17`

The std=gnu++17 makes sure that the compiler you're using locally (which you should have installed following the instructions on the Quickstart guide on Blackboard/Git) picks up the same version of the C++ standards as the computers in the lab. 

This command will produce the following file in the same directory you're running on:

`g++-11 assignment-3.cpp -o assignment-3.o -std=gnu++17`

Which you can then execute in a terminal using: 

`./assignment-3.o`

You can find out more about Makefiles [here](https://www.gnu.org/software/make/manual/html_node/Introduction.html) or [in this simple starter guide](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/) but we won't cover these in the course. 
