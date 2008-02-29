===========================================================
Project name : vIRtuel machin
Version : 1.0
Authors : Tom Miette & Sebastien Mouret
-----------------------------------------------------------
README.txt
Created February 28, 2008 by Tom Miette & Sebastien Mouret
===========================================================

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
	5. "doc/html" which contains the source file documentation
	(doxygen format).
	6. "src" which contains source files and makefiles.

Building jar archive
--------------------

The following commands will build the executable jar archive in the
"bin" directory : 
	
	$ ant
	OR
	$ ant jar

Quick start
-----------

To run the program, you can use the build file with the "run" task as
following :

	$ ant run

Or you can simply use the java command to launch the jar archive :

	$ java - jar ${user_directory}/bin/IRPhoto.jar

Others ant tasks
----------------

The build file has others ant tasks 

	1. To compile java source files in the "classes" directory :
	
		$ ant compile
		
	2. To generate java documentation in the "docs/api" directory :
	
		$ ant javadoc

	3. To clean the project directory. This task removes class files, 
	documentation files and the jar archive :
	
		$ ant clean

Libraries
---------

The application used external libraries to run. These libraries are in the
"lib" directory and enable to perform graphical effects not included in
the default swing api.

	1. swingx to display the world map and to manage photo localisations.
	2. metadata-extractor to extract exif informations from photos files.

Known bugs
----------

The following section lists all known bugs of vIRtuel machin :


