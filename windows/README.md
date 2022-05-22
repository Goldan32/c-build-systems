# Makefile Project: Windows
**This is the makefile intended for usage with windows command line (not powershell). This document explains the structure of the project, and how they relate to the makefile.**

## Building this project
### Requirements
1. **gcc**: It's a little tricky on windows, but this guide should be of help: https://code.visualstudio.com/docs/cpp/config-mingw
2. **make**: After installing, you will alreade have mingw32. Look for the mingw32 way in this guide, and you can skip the installation and proceed to step 6. https://www.technewstoday.com/install-and-use-make-in-windows/

### Build flow
1. Clone this repository from windows cmd: `> git clone https://github.com/Goldan32/c-build-systems.git`
2. Go to the windows folder: `> cd c-build-systems\windows`
3. Build the project: `> make all`
4. Test the project: `> .\build\helloworld.exe`

Output should be:
```
Hello World!
Hello from another file!
Static library says 3*3 is 9
```
5. Test clean target: `> make all`

## Project structure
### Defining folders
```
SRC_DIR  := sources         # .c source files
OBJ_DIR  := build\obj       # object files created during compilation
BIN_DIR  := build           # folder containing the binary (.exe) output
INC_DIR  := include         # .h source files
LIB_DIR  := libraries       # .a (static library) source files
EXE_NAME := helloworld.exe  # name of the executable
```
These variables define where the makefile will search for the source files. The names of these folders and files can be changed freely. In the current setup the obj directory is in the BIN_DIR directory, however it could be in the project root folder separately just by changing its value here.

### Defining targets

```
EXE := $(BIN_DIR)/$(EXE_NAME)
```
This is the final target, the executable. We define it so it will be located in the BIN_DIR folder.
```
SRC := $(wildcard $(SRC_DIR)/*.c)
```
This is where the .c sources are collected. The `wildcard` function lists all the files mathing the pattern in its argument. In this case it matches all the files ending with ".c" in the SRC_DIR directory.
```
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
```
This is where the object files are collected. These files match the names of the .c source files, the difference is the extension. We can list the object files with the already defined .c source files.

### Various other defines
```
RMDIR := rd /s /q
```
The standard makefile defines some variables. RMDIR and RM is such variables, but we can explicitly redifine it. This definition is equal to the linux command `rm -rf` in windows cmd form.

### Compile flags
```
CC       := gcc
```
CC stands for C compiler. Its default value would be gcc here, but we can define it explicitly for clarity

```
CPPFLAGS := -I$(INC_DIR) -MMD -MP
```
CPPFLAGS stands for C PreProcessor Flags. Here we have the option to define our include directory, and instruct the preprocessor, to look for the header files there. We can also pass other flags like -MMD and -MP. These options are necessary in a makefile. The explaination is out of the scope of this document, but can be found here: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html

```
CFLAGS   := -Wall -Werror
```
Here we can pass flags to the compiler, not the preprocessor.

```
LDFLAGS  := -L$(LIB_DIR)
```
The -L flag is used to point to the folder where static libraries are.

```
LDLIBS   := -lm -static $(patsubst $(LIB_DIR)\lib*.a, -l$(LIB_DIR)\*, $(wildcard $(LIB_DIR)/lib*.a))
```
This variable will list all the static library, .a files located in the folder defined above. The `patsubst` function substitutes the second argument in the place of the first, wherever the pattern matches the third argument. It is used to trim the extension from the end and the "lib" from the name of the .a files, and write "-l" to the beginning. All static libraries start with "lib", but the linker knows this, and writes it for us automatically, thus we need to trim it before passing it as argument.

The -l flag is used to point to one static library, so we have to use it multiple times here

The `wildcard` function is once again used to list files with a given extension.

### Phony targets
```
.PHONY: all clean
```
Normally a target is a name of a file. Phony targets are not interpreted as a file name, even a file exists with such name. Phony targets are used to collect multiple other targets most of the time.

## Target recipes
```
all: $(EXE)
```
The all target is a convention, it is the final target of the project.

```
$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
```
This part says that to build the executable file, we will need the object files and the BIN_DIR folder. If any of those are not ready or up to date, their recipes will execute.

$^ refers to all the prerequisites of the current target

$@ refers to the target

```
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
```
This is the recipe for the object files. Their prerequisites are the corresponding .c files and the existence of the OBJ_DIR

$< refers to the firs prerequisite of the current target

```
$(BIN_DIR) $(OBJ_DIR):
	if not exist "$@" mkdir $@
```
This is the recipe for the object and binary folders. If they don't exist, the makefile creates them here.

```
clean:
	@if exist $(BIN_DIR) $(RMDIR) $(BIN_DIR)
	@if exist $(OBJ_DIR) $(RMDIR) $(OBJ_DIR)
```
The clean target deletes everithing that is created during the compile process

### Include other makefile sources
```
-include $(OBJ:.o=.d)
```
gcc creates a .d file for every .o file, which contain makefile rules, so they are included here.