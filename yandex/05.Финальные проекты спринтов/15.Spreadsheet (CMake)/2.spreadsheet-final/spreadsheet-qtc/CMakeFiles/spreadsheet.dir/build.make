# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc

# Include any dependencies generated for this target.
include CMakeFiles/spreadsheet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/spreadsheet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/spreadsheet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/spreadsheet.dir/flags.make

antlr4cpp_generated_src/Formula/FormulaLexer.interp: Formula.g4
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building FormulaParser with ANTLR 4.13.1"
	/Library/Java/JavaVirtualMachines/jdk-22.jdk/Contents/Home/bin/java -jar /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr-4.13.1-complete.jar Formula.g4 -o /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula -no-listener -Dlanguage=Cpp -listener

antlr4cpp_generated_src/Formula/FormulaLexer.tokens: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaLexer.tokens

antlr4cpp_generated_src/Formula/FormulaLexer.h: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaLexer.h

antlr4cpp_generated_src/Formula/FormulaLexer.cpp: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaLexer.cpp

antlr4cpp_generated_src/Formula/FormulaParser.h: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaParser.h

antlr4cpp_generated_src/Formula/FormulaParser.cpp: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaParser.cpp

antlr4cpp_generated_src/Formula/FormulaBaseListener.h: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaBaseListener.h

antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp

antlr4cpp_generated_src/Formula/FormulaListener.h: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaListener.h

antlr4cpp_generated_src/Formula/FormulaListener.cpp: antlr4cpp_generated_src/Formula/FormulaLexer.interp
	@$(CMAKE_COMMAND) -E touch_nocreate antlr4cpp_generated_src/Formula/FormulaListener.cpp

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o: antlr4cpp_generated_src/Formula/FormulaLexer.cpp
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o -MF CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o.d -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaLexer.cpp

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaLexer.cpp > CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.i

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaLexer.cpp -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.s

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o: antlr4cpp_generated_src/Formula/FormulaParser.cpp
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o -MF CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o.d -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaParser.cpp

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaParser.cpp > CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.i

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaParser.cpp -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.s

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o: antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o -MF CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o.d -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp > CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.i

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.s

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o: antlr4cpp_generated_src/Formula/FormulaListener.cpp
CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o -MF CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o.d -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaListener.cpp

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaListener.cpp > CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.i

CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/antlr4cpp_generated_src/Formula/FormulaListener.cpp -o CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.s

CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o: FormulaAST.cpp
CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o -MF CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o.d -o CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/FormulaAST.cpp

CMakeFiles/spreadsheet.dir/FormulaAST.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/FormulaAST.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/FormulaAST.cpp > CMakeFiles/spreadsheet.dir/FormulaAST.cpp.i

CMakeFiles/spreadsheet.dir/FormulaAST.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/FormulaAST.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/FormulaAST.cpp -o CMakeFiles/spreadsheet.dir/FormulaAST.cpp.s

CMakeFiles/spreadsheet.dir/cell.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/cell.cpp.o: cell.cpp
CMakeFiles/spreadsheet.dir/cell.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/spreadsheet.dir/cell.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/cell.cpp.o -MF CMakeFiles/spreadsheet.dir/cell.cpp.o.d -o CMakeFiles/spreadsheet.dir/cell.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/cell.cpp

CMakeFiles/spreadsheet.dir/cell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/cell.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/cell.cpp > CMakeFiles/spreadsheet.dir/cell.cpp.i

CMakeFiles/spreadsheet.dir/cell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/cell.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/cell.cpp -o CMakeFiles/spreadsheet.dir/cell.cpp.s

CMakeFiles/spreadsheet.dir/formula.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/formula.cpp.o: formula.cpp
CMakeFiles/spreadsheet.dir/formula.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/spreadsheet.dir/formula.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/formula.cpp.o -MF CMakeFiles/spreadsheet.dir/formula.cpp.o.d -o CMakeFiles/spreadsheet.dir/formula.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/formula.cpp

CMakeFiles/spreadsheet.dir/formula.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/formula.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/formula.cpp > CMakeFiles/spreadsheet.dir/formula.cpp.i

CMakeFiles/spreadsheet.dir/formula.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/formula.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/formula.cpp -o CMakeFiles/spreadsheet.dir/formula.cpp.s

CMakeFiles/spreadsheet.dir/main.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/main.cpp.o: main.cpp
CMakeFiles/spreadsheet.dir/main.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/spreadsheet.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/main.cpp.o -MF CMakeFiles/spreadsheet.dir/main.cpp.o.d -o CMakeFiles/spreadsheet.dir/main.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/main.cpp

CMakeFiles/spreadsheet.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/main.cpp > CMakeFiles/spreadsheet.dir/main.cpp.i

CMakeFiles/spreadsheet.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/main.cpp -o CMakeFiles/spreadsheet.dir/main.cpp.s

CMakeFiles/spreadsheet.dir/sheet.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/sheet.cpp.o: sheet.cpp
CMakeFiles/spreadsheet.dir/sheet.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/spreadsheet.dir/sheet.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/sheet.cpp.o -MF CMakeFiles/spreadsheet.dir/sheet.cpp.o.d -o CMakeFiles/spreadsheet.dir/sheet.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/sheet.cpp

CMakeFiles/spreadsheet.dir/sheet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/sheet.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/sheet.cpp > CMakeFiles/spreadsheet.dir/sheet.cpp.i

CMakeFiles/spreadsheet.dir/sheet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/sheet.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/sheet.cpp -o CMakeFiles/spreadsheet.dir/sheet.cpp.s

CMakeFiles/spreadsheet.dir/structures.cpp.o: CMakeFiles/spreadsheet.dir/flags.make
CMakeFiles/spreadsheet.dir/structures.cpp.o: structures.cpp
CMakeFiles/spreadsheet.dir/structures.cpp.o: CMakeFiles/spreadsheet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/spreadsheet.dir/structures.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/spreadsheet.dir/structures.cpp.o -MF CMakeFiles/spreadsheet.dir/structures.cpp.o.d -o CMakeFiles/spreadsheet.dir/structures.cpp.o -c /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/structures.cpp

CMakeFiles/spreadsheet.dir/structures.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/spreadsheet.dir/structures.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/structures.cpp > CMakeFiles/spreadsheet.dir/structures.cpp.i

CMakeFiles/spreadsheet.dir/structures.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/spreadsheet.dir/structures.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/structures.cpp -o CMakeFiles/spreadsheet.dir/structures.cpp.s

# Object files for target spreadsheet
spreadsheet_OBJECTS = \
"CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o" \
"CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o" \
"CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o" \
"CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o" \
"CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o" \
"CMakeFiles/spreadsheet.dir/cell.cpp.o" \
"CMakeFiles/spreadsheet.dir/formula.cpp.o" \
"CMakeFiles/spreadsheet.dir/main.cpp.o" \
"CMakeFiles/spreadsheet.dir/sheet.cpp.o" \
"CMakeFiles/spreadsheet.dir/structures.cpp.o"

# External object files for target spreadsheet
spreadsheet_EXTERNAL_OBJECTS =

spreadsheet: CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaLexer.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaParser.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/antlr4cpp_generated_src/Formula/FormulaListener.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/FormulaAST.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/cell.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/formula.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/main.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/sheet.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/structures.cpp.o
spreadsheet: CMakeFiles/spreadsheet.dir/build.make
spreadsheet: antlr4_runtime/runtime/libantlr4-runtime.a
spreadsheet: CMakeFiles/spreadsheet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable spreadsheet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spreadsheet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/spreadsheet.dir/build: spreadsheet
.PHONY : CMakeFiles/spreadsheet.dir/build

CMakeFiles/spreadsheet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/spreadsheet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/spreadsheet.dir/clean

CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaBaseListener.cpp
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaBaseListener.h
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaLexer.cpp
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaLexer.h
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaLexer.interp
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaLexer.tokens
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaListener.cpp
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaListener.h
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaParser.cpp
CMakeFiles/spreadsheet.dir/depend: antlr4cpp_generated_src/Formula/FormulaParser.h
	cd /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc /Users/ps/Documents/cpp-course/cpp-spreadsheet/spreadsheet-qtc/CMakeFiles/spreadsheet.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/spreadsheet.dir/depend

