# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Clion\CLion 2019.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Clion\CLion 2019.2.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\code3\mmx\Xana

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\code3\mmx\Xana\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Xana.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Xana.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Xana.dir/flags.make

CMakeFiles/Xana.dir/main.c.obj: CMakeFiles/Xana.dir/flags.make
CMakeFiles/Xana.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\code3\mmx\Xana\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Xana.dir/main.c.obj"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Xana.dir\main.c.obj   -c E:\code3\mmx\Xana\main.c

CMakeFiles/Xana.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Xana.dir/main.c.i"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\code3\mmx\Xana\main.c > CMakeFiles\Xana.dir\main.c.i

CMakeFiles/Xana.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Xana.dir/main.c.s"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\code3\mmx\Xana\main.c -o CMakeFiles\Xana.dir\main.c.s

# Object files for target Xana
Xana_OBJECTS = \
"CMakeFiles/Xana.dir/main.c.obj"

# External object files for target Xana
Xana_EXTERNAL_OBJECTS =

Xana.exe: CMakeFiles/Xana.dir/main.c.obj
Xana.exe: CMakeFiles/Xana.dir/build.make
Xana.exe: CMakeFiles/Xana.dir/linklibs.rsp
Xana.exe: CMakeFiles/Xana.dir/objects1.rsp
Xana.exe: CMakeFiles/Xana.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\code3\mmx\Xana\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Xana.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Xana.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Xana.dir/build: Xana.exe

.PHONY : CMakeFiles/Xana.dir/build

CMakeFiles/Xana.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Xana.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Xana.dir/clean

CMakeFiles/Xana.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code3\mmx\Xana E:\code3\mmx\Xana E:\code3\mmx\Xana\cmake-build-debug E:\code3\mmx\Xana\cmake-build-debug E:\code3\mmx\Xana\cmake-build-debug\CMakeFiles\Xana.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Xana.dir/depend

