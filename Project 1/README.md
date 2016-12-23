This project demostrates the use of makefile to process, compile, link, archive and generate executable files for architecture independent 
targets from source files.

The header files used are memory.h and data.h.
Memory.h includes memory based tasks like moving, resetting to 0 and reversing.
Data.h includes data manipulation like Integer to ASCII and vice-versa, memory dump and changing endianness.

The project_1 file invokes these functions and tests their working.

The Makefile has been built for the Linux host platform, Beaglebone Black development board and the Freedom KL25z board.
The code has been largely designed to be architecture-independent.

A number of targets have been defined to replicate  the functions of the IDE. 
This Makefile helps us to only compile those files that have been changed, so that we do not have to compile all the source files everytime (which can take up a lot of time!).


The following targets have been implemented - 

- preprocess
  Running '$make preprocess' generates a .i or preprocessed file (goes by the name test.i) from all the source files.
  
  Running data.i\memory.i\main.i\project_1.i should generate the corresponding .i files from the .c files.
  
  
- asm-file
  Running '$make asm-file' generates a .s or assembly file from the source files.
  
  Running data.S\memory.S\main.S\project_1.S should generate the corresponding .s files from the .c files.
  
  
- %.o 
  Running '$make memory.o' generates a .o or object file from the source file.
  The % signifies that we can put any filename in its place, and the corresponding object file should be created.
  

- compile-all
  Running '$make compile-all' generates the object files for all the source files, however, it does not link them.
  
  
- build
  Running '$make build' generates the object files plus the executable 'project'. Thus, this stage combines the compile and linking stages.
  Apart from that, the .MAP file is created at this stage and also the size of the program is displayed in terms of text, data and bss size.
  
  
- clean
  Running '$make clean' cleans up all the .o, .s, .map, .i and also the executable from the file directory.
  
  
- upload
  Running '$make upload' uploads the executable that has been created after the build stage to the Beaglebone Board. After this stage, one needs to SSH onto the Beaglebone Board, go into their /home/ directory and type in './project' to execute the file on the board.
  
   
- build-lib
  Running '$make build-lib' generates a static library called libproject1.a, which is a library of our .c files.
  
  
   
A few extra comments :-

- To compile and build the program for the Beaglebone Black, use '$make build_BBB' instead of '$make build', which is for the host platform.   
  
- To compile (and not build) for the Beaglebone Black, use '$make compile-all-BBB' instead of '$make compile-all' to compile it for the board.

- After cross-compiling, the executable can be uploaded to the Beaglebone by doing '$make upload'.

- Similarly, for the Freedom KL25z, use '$make build_FRDM' instead of '$make build.
