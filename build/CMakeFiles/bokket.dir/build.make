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
include CMakeFiles/bokket.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bokket.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bokket.dir/flags.make

CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o: CMakeFiles/bokket.dir/flags.make
CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o: ../bokket/Log/Log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o -c /home/bokket/bokkerve/bokket/Log/Log.cpp

CMakeFiles/bokket.dir/bokket/Log/Log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bokket.dir/bokket/Log/Log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bokket/bokkerve/bokket/Log/Log.cpp > CMakeFiles/bokket.dir/bokket/Log/Log.cpp.i

CMakeFiles/bokket.dir/bokket/Log/Log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bokket.dir/bokket/Log/Log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bokket/bokkerve/bokket/Log/Log.cpp -o CMakeFiles/bokket.dir/bokket/Log/Log.cpp.s

CMakeFiles/bokket.dir/bokket/thread/util.cpp.o: CMakeFiles/bokket.dir/flags.make
CMakeFiles/bokket.dir/bokket/thread/util.cpp.o: ../bokket/thread/util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bokket.dir/bokket/thread/util.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bokket.dir/bokket/thread/util.cpp.o -c /home/bokket/bokkerve/bokket/thread/util.cpp

CMakeFiles/bokket.dir/bokket/thread/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bokket.dir/bokket/thread/util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bokket/bokkerve/bokket/thread/util.cpp > CMakeFiles/bokket.dir/bokket/thread/util.cpp.i

CMakeFiles/bokket.dir/bokket/thread/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bokket.dir/bokket/thread/util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bokket/bokkerve/bokket/thread/util.cpp -o CMakeFiles/bokket.dir/bokket/thread/util.cpp.s

CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o: CMakeFiles/bokket.dir/flags.make
CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o: ../bokket/Log/FileWriterType.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o -c /home/bokket/bokkerve/bokket/Log/FileWriterType.cpp

CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bokket/bokkerve/bokket/Log/FileWriterType.cpp > CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.i

CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bokket/bokkerve/bokket/Log/FileWriterType.cpp -o CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.s

CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o: CMakeFiles/bokket.dir/flags.make
CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o: ../bokket/Log/LogStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o -c /home/bokket/bokkerve/bokket/Log/LogStream.cpp

CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bokket/bokkerve/bokket/Log/LogStream.cpp > CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.i

CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bokket/bokkerve/bokket/Log/LogStream.cpp -o CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.s

# Object files for target bokket
bokket_OBJECTS = \
"CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o" \
"CMakeFiles/bokket.dir/bokket/thread/util.cpp.o" \
"CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o" \
"CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o"

# External object files for target bokket
bokket_EXTERNAL_OBJECTS =

../lib/libbokket.so: CMakeFiles/bokket.dir/bokket/Log/Log.cpp.o
../lib/libbokket.so: CMakeFiles/bokket.dir/bokket/thread/util.cpp.o
../lib/libbokket.so: CMakeFiles/bokket.dir/bokket/Log/FileWriterType.cpp.o
../lib/libbokket.so: CMakeFiles/bokket.dir/bokket/Log/LogStream.cpp.o
../lib/libbokket.so: CMakeFiles/bokket.dir/build.make
../lib/libbokket.so: CMakeFiles/bokket.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bokket/bokkerve/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ../lib/libbokket.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bokket.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bokket.dir/build: ../lib/libbokket.so

.PHONY : CMakeFiles/bokket.dir/build

CMakeFiles/bokket.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bokket.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bokket.dir/clean

CMakeFiles/bokket.dir/depend:
	cd /home/bokket/bokkerve/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bokket/bokkerve /home/bokket/bokkerve /home/bokket/bokkerve/build /home/bokket/bokkerve/build /home/bokket/bokkerve/build/CMakeFiles/bokket.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bokket.dir/depend

