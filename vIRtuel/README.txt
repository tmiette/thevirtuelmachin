===============================================================================
Project name : vIRtuel machin
Version : 1.0
Authors : Tom Miette & Sebastien Mouret
-------------------------------------------------------------------------------
README.txt
Created February 28, 2008 by Tom Miette & Sebastien Mouret
===============================================================================

What is vIRtuel machin ?
------------------------

vIRtuel machin is small shell which enables to execute tasks 
like object-oriented programming without blocking calls. The
program is divided into three main parts. One part called
"shell" to receive user's commands. Another part "launch" to
create tasks and to execute them from external libraries.
And a third part called "oreader" which displays all messages 
printed by the different objects.

Project directories
-------------------

The project root directory is divided into six main directories.

	1. "bin" which contains the three executable files.
	2. "objects" which contains external libraries used to perform
	tasks.
	3. "objects_src" which contains source files of libraries.
	4. "doc" which contains the project documentation.
	5. "doc/api" which contains the source file documentation
	(doxygen format).
	6. "src" which contains source files and makefiles.

Compiling sources and building executables
------------------------------------------

The compile all source files and to build executables files needed to
run the program, you have to use the makefile in the src directory.
Use the default target to generate all these files :

	$ make

The executables are put in the bin directory.

Quick start
-----------

To run the program, you have to executable files from the bin
directory. You have to launch the vIRm file in a terminal and
the oreader file in another terminal.

	$ cd ./bin
	$ ./vIRm
	$ ./oreader

Others make targets
-------------------

The src directory's makefile enables to perform some others tasks : 

	1. To remove all temporary files
	
		$ make clean
		
	2. To all temporary files and executables
	
		$ make mrproper

Libraries
---------

The program uses external libraries to execute some tasks. When you
create a new object, you have to specify the name of one of these
libraries which will be used to perform the object's work.
All of these libraries source files are in the objects_src directory.
To recompile them and to rebuild libraries, you have to use the
makefile in this directory :

	1. default target builds libraries in the objects directory
	
		$ make
		
	2. clean target removes temporary files and libraries
	
		$ make clean

Known bugs
----------

Refer to the documentation file doc.pdf in the doc directory
to know the list of known bugs.


