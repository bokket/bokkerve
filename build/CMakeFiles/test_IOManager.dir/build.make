# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/cmake-3.17/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.17/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bokket/bokkerve

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bokket/bokkerve/build

# Include any dependencies generated for this target.
include CMakeFiles/test_IOManager.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_IOManager.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_IOManager.dir/flags.make

CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o: CMakeFiles/test_IOManager.dir/flags.make
CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o: ../test/test_iomanager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o -c /home/bokket/bokkerve/test/test_iomanager.cpp

CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bokket/bokkerve/test/test_iomanager.cpp > CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.i

CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bokket/bokkerve/test/test_iomanager.cpp -o CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.s

# Object files for target test_IOManager
test_IOManager_OBJECTS = \
"CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o"

# External object files for target test_IOManager
test_IOManager_EXTERNAL_OBJECTS =

../bin/test_IOManager: CMakeFiles/test_IOManager.dir/test/test_iomanager.cpp.o
../bin/test_IOManager: CMakeFiles/test_IOManager.dir/build.make
../bin/test_IOManager: ../lib/libbokket.so
../bin/test_IOManager: CMakeFiles/test_IOManager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_IOManager"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_IOManager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_IOManager.dir/build: ../bin/test_IOManager

.PHONY : CMakeFiles/test_IOManager.dir/build

CMakeFiles/test_IOManager.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_IOManager.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_IOManager.dir/clean

CMakeFiles/test_IOManager.dir/depend:
	cd /home/bokket/bokkerve/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bokket/bokkerve /home/bokket/bokkerve /home/bokket/bokkerve/build /home/bokket/bokkerve/build /home/bokket/bokkerve/build/CMakeFiles/test_IOManager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_IOManager.dir/depend

