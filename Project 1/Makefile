include sources.mk

# Compiler variables
CC = gcc
BBB = arm-linux-gnueabihf-gcc -DBBB 
FRDM = arm-none-eabi-gcc -DFRDM --specs=nosys.specs 


# Setting the compiler flags
CFLAGS = -c -Wall -g -std=c99 -O0

# Setting the linker
LINK = -Wl,-Map=project.map


# Setting phony targets 
.PHONY : preprocess data.i memory.i project_1.i main.i asm-file data.S memory.S project_1.S main.S \
    compile-all compile-all-BBB compile-all-FRDM %.o build build_BBB build_FRDM upload build-lib


all: project

project : $(options_o)
	$(CC) $(options_o) -o project


# These are the preprocessed output .i files
preprocess : $(options_c)
	$(CC) $(options_c) -E > file.i

data.i : data.c
	$(CC) C_files/data.c -E > data.i

memory.i : memory.c
	$(CC) C_files/memory.c -E > memory.i

project_1.i : project_1.c
	$(CC) C_files/project_1.c -E > project_1.i

main.i : main.c
	$(CC) C_files/main.c -E > main.i


# These are the assembly output .S files
asm-file : data.c memory.c main.c project_1.c
	$(CC) $(options_c) -S 

data.S : data.c data.h
	$(CC) C_files/data.c -S 

memory.S : memory.c memory.h
	$(CC) C_files/memory.c -S

project_1.S : project_1.c project_1.h data.h memory.h
	$(CC) C_files/project_1.c -S

main.S : main.c data.h memory.h project_1.h
	$(CC) C_files/main.c -S


# Compiling (but not linking) the source code files
compile-all : $(options_c) $(options_h)
	$(CC) $(options_c) $(CFLAGS) $^
	

compile-all-BBB : $(options_c) $(options_h)
	$(BBB) $(options_c) $(CFLAGS)
	

compile-all-FRDM : $(options_c) $(options_h)
	$(FRDM) $(options_c) $(CFLAGS)


# Individually compiles the .c file and converts it to a .o file 
%.o : %.c
	$(CC) $(CFLAGS) -c $^


# Build - compiling and linking
build : $(options_c) $(options_h)
	$(CC) C_files/data.c $(CFLAGS)
	$(CC) C_files/memory.c $(CFLAGS)
	$(CC) C_files/project_1.c $(CFLAGS)
	$(CC) C_files/main.c $(CFLAGS)
	$(CC) -o project $(options_o) $(LINK)
	$(CC) -MM $(options_c) $(options_h) > depfiles    # Generating dependencies for the object files
	size project                                      # Determining the size of the program
	objdump -f project                                # Performing object dump  
	
	
build_BBB : $(options_c) $(options_h)
	$(BBB) C_files/data.c $(CFLAGS)
	$(BBB) C_files/memory.c $(CFLAGS)
	$(BBB) C_files/project_1.c $(CFLAGS)
	$(BBB) C_files/main.c $(CFLAGS)
	$(BBB) -o project $(options_o)
	$(CC) -MM $(options_c) $(options_h) > depfiles_BBB # Generating dependencies for the object files for Beaglebone Black
	size project                                       # Size of project



build_FRDM : $(options_c) $(options_h)
	$(FRDM) C_files/data.c $(CFLAGS)
	$(FRDM) C_files/memory.c $(CFLAGS)
	$(FRDM) C_files/project_1.c $(CFLAGS)
	$(FRDM) C_files/main.c $(CFLAGS)
	$(FRDM) -o project $(options_o)
	$(CC) -MM $(options_c) $(options_h) > depfiles_FRDM # Generating dependencies for the object files for Freedom KL25z
	size project                                        # Size of project

	
# Directly invoking Linker - without gcc
lnk : $(options_c) $(options_h)
	$(CC) $(CFLAGS) -c C_files/data.c
	$(CC) $(CFLAGS) -c C_files/memory.c
	$(CC) $(CFLAGS) -c C_files/project_1.c
	$(CC) $(CFLAGS) -c C_files/main.c
	ld -o project data.o memory.o project_1.o main.o



# Connection to beaglebone and uploading the executable 'project'
upload: 
	scp project root@192.168.7.2:/home/


# Creating an archive of my .c files and naming it libproject1.a
build-lib:
	ar -cvq libproject1.a *.c


# Phony targets
.PHONY : clean
clean:
	rm -rf *.o *.map *.i *.s project
