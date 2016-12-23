
# These are the source files
options_c = C_files/data.c C_files/memory.c C_files/main.c C_files/project_1.c

#These are the header files
options_h = Headers/data.h Headers/memory.h Headers/project_1.h

# These are the object files
options_o = data.o memory.o project_1.o main.o


# Setting the path
vpath %.c C_files
vpath %.h Headers