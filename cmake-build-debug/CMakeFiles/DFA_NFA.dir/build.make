# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\FMI\DFA-NFA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\FMI\DFA-NFA\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DFA_NFA.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DFA_NFA.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DFA_NFA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DFA_NFA.dir/flags.make

CMakeFiles/DFA_NFA.dir/main.cpp.obj: CMakeFiles/DFA_NFA.dir/flags.make
CMakeFiles/DFA_NFA.dir/main.cpp.obj: D:/FMI/DFA-NFA/main.cpp
CMakeFiles/DFA_NFA.dir/main.cpp.obj: CMakeFiles/DFA_NFA.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\FMI\DFA-NFA\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DFA_NFA.dir/main.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/DFA_NFA.dir/main.cpp.obj -MF CMakeFiles\DFA_NFA.dir\main.cpp.obj.d -o CMakeFiles\DFA_NFA.dir\main.cpp.obj -c D:\FMI\DFA-NFA\main.cpp

CMakeFiles/DFA_NFA.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/DFA_NFA.dir/main.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\FMI\DFA-NFA\main.cpp > CMakeFiles\DFA_NFA.dir\main.cpp.i

CMakeFiles/DFA_NFA.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/DFA_NFA.dir/main.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\FMI\DFA-NFA\main.cpp -o CMakeFiles\DFA_NFA.dir\main.cpp.s

# Object files for target DFA_NFA
DFA_NFA_OBJECTS = \
"CMakeFiles/DFA_NFA.dir/main.cpp.obj"

# External object files for target DFA_NFA
DFA_NFA_EXTERNAL_OBJECTS =

DFA_NFA.exe: CMakeFiles/DFA_NFA.dir/main.cpp.obj
DFA_NFA.exe: CMakeFiles/DFA_NFA.dir/build.make
DFA_NFA.exe: CMakeFiles/DFA_NFA.dir/linkLibs.rsp
DFA_NFA.exe: CMakeFiles/DFA_NFA.dir/objects1.rsp
DFA_NFA.exe: CMakeFiles/DFA_NFA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\FMI\DFA-NFA\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable DFA_NFA.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\DFA_NFA.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DFA_NFA.dir/build: DFA_NFA.exe
.PHONY : CMakeFiles/DFA_NFA.dir/build

CMakeFiles/DFA_NFA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\DFA_NFA.dir\cmake_clean.cmake
.PHONY : CMakeFiles/DFA_NFA.dir/clean

CMakeFiles/DFA_NFA.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\FMI\DFA-NFA D:\FMI\DFA-NFA D:\FMI\DFA-NFA\cmake-build-debug D:\FMI\DFA-NFA\cmake-build-debug D:\FMI\DFA-NFA\cmake-build-debug\CMakeFiles\DFA_NFA.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/DFA_NFA.dir/depend

