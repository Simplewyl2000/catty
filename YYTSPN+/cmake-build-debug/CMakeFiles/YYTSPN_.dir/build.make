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
CMAKE_SOURCE_DIR = E:\code3\YYTSPN+

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\code3\YYTSPN+\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/YYTSPN_.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/YYTSPN_.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/YYTSPN_.dir/flags.make

CMakeFiles/YYTSPN_.dir/main.c.obj: CMakeFiles/YYTSPN_.dir/flags.make
CMakeFiles/YYTSPN_.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\code3\YYTSPN+\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/YYTSPN_.dir/main.c.obj"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\YYTSPN_.dir\main.c.obj   -c E:\code3\YYTSPN+\main.c

CMakeFiles/YYTSPN_.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/YYTSPN_.dir/main.c.i"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\code3\YYTSPN+\main.c > CMakeFiles\YYTSPN_.dir\main.c.i

CMakeFiles/YYTSPN_.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/YYTSPN_.dir/main.c.s"
	D:\Mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\code3\YYTSPN+\main.c -o CMakeFiles\YYTSPN_.dir\main.c.s

# Object files for target YYTSPN_
YYTSPN__OBJECTS = \
"CMakeFiles/YYTSPN_.dir/main.c.obj"

# External object files for target YYTSPN_
YYTSPN__EXTERNAL_OBJECTS =

YYTSPN_.exe: CMakeFiles/YYTSPN_.dir/main.c.obj
YYTSPN_.exe: CMakeFiles/YYTSPN_.dir/build.make
YYTSPN_.exe: CMakeFiles/YYTSPN_.dir/linklibs.rsp
YYTSPN_.exe: CMakeFiles/YYTSPN_.dir/objects1.rsp
YYTSPN_.exe: CMakeFiles/YYTSPN_.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\code3\YYTSPN+\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable YYTSPN_.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\YYTSPN_.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/YYTSPN_.dir/build: YYTSPN_.exe

.PHONY : CMakeFiles/YYTSPN_.dir/build

CMakeFiles/YYTSPN_.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\YYTSPN_.dir\cmake_clean.cmake
.PHONY : CMakeFiles/YYTSPN_.dir/clean

CMakeFiles/YYTSPN_.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code3\YYTSPN+ E:\code3\YYTSPN+ E:\code3\YYTSPN+\cmake-build-debug E:\code3\YYTSPN+\cmake-build-debug E:\code3\YYTSPN+\cmake-build-debug\CMakeFiles\YYTSPN_.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/YYTSPN_.dir/depend
