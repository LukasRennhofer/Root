/*                                                                                                        
                   The kernel knows. The compiler obeys. The bloat dies.                                                                                                    
*/

// The no-bloat single-header Build System for C/C++
// Root is still under construction and the source code can change at any time!
// Use at your own risk!

// Written by Lukas Rennhofer, 2025, Github: LukasRennhofer

/*
MIT License

Copyright (c) 2025 Lukas Rennhofer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
    ====== @ Root Preprocessors @ ======

    ROOT_H >> Roots Header Guard

    ROOT_VERSION  >> The version of the Root Build System.
    
    ROOT_COMPILER_C >> The C compiler to use (default: `gcc`).
    ROOT_COMPILER_CXX >> The C++ compiler to use (default: `g++`).
    
    ROOT_C_FLAGS >> Compiler flags for C files (default: `-Wall -Wextra -std=c99`).
    ROOT_CXX_FLAGS >> Compiler flags for C++ files (default: `-Wall -Wextra -std=c++17`).

    ROOT_USER_C_FLAGS >> User-defined flags to append to the C compiler (default: `""`).
    ROOT_USER_CXX_FLAGS >> User-defined flags to append to the C++ compiler (default: `""`).

    ROOT_SRC_DIR >> Directory where the source files are located (default: `"./src"`).
    ROOT_INC_DIR >> Directory where the header files are located (default: `"./include"`).

    ROOT_OUTPUT >> Enables or disables debug output for various processes in the build (1 = enabled, 0 = disabled; default: `1`).

    ROOT_OUTPUT_DEBUG >> Enables debug output specifically for linking process (1 = enabled, 0 = disabled; default: `0`).

    ROOT_FILE_EXISTS(file) >> A macro to check if a file exists at the given path (returns true or false).

    ROOT_FILE_IS_C(file) >> A macro to check if the file is a C source file (i.e., `.c` or `.h`).
    ROOT_FILE_IS_CPP(file) >> A macro to check if the file is a C++ source file (i.e., `.cpp` or `.cxx`).

    ROOT_CONCAT(a, b) >> A macro to concatenate two tokens, `a` and `b`.

    ROOT_STRINGIFY(x) >> A macro to convert a macro parameter `x` to a string (useful for file paths, etc.).

    ROOT_BUILD_DEBUG_FLAGS >> Defines additional flags for debugging builds (i.e., `-g`, `-DDEBUG`, etc.) depending on user requirements.

    ROOT_COMPILER_FLAGS >> Includes both default flags and user-defined flags for the selected compiler.

    ROOT_LINKER_FLAGS >> Additional linker flags that might be necessary, such as linking with math libraries (`-lm`), SSL libraries, etc.

    ROOT_CUSTOM_MACROS >> Any additional custom macros the user wants to define or customize during the build process.

    @ End of Roots Preprocessors @
*/

#ifndef ROOT_H
#define ROOT_H

// Root version
#define ROOT_VERSION "0.1"

// Root Output in Terminal
#ifndef ROOT_OUTPUT
    #define ROOT_OUTPUT 1
#endif

// Root Debug Output (to show executed commands)
#ifndef ROOT_OUTPUT_DEBUG
    #define ROOT_OUTPUT_DEBUG 0
#endif

// If no compiler is defined, default to `gcc` for C and `g++` for C++
#ifndef ROOT_COMPILER_C
    #define ROOT_COMPILER_C "gcc"
#endif
#ifndef ROOT_COMPILER_CXX
    #define ROOT_COMPILER_CXX "g++"
#endif

// If no C and C++ flags are defined
#ifndef ROOT_C_FLAGS
    #define ROOT_C_FLAGS "-Wall -Wextra -std=c99"
#endif
#ifndef ROOT_CXX_FLAGS
    #define ROOT_CXX_FLAGS "-Wall -Wextra -std=c++17"
#endif

// User-defined compiler flags
#ifndef ROOT_USER_C_FLAGS
    #define ROOT_USER_C_FLAGS ""
#endif
#ifndef ROOT_USER_CXX_FLAGS
    #define ROOT_USER_CXX_FLAGS ""
#endif

// Define paths for source and header directories
#ifndef ROOT_SRC_DIR
    #define ROOT_SRC_DIR "./src"
#endif
#ifndef ROOT_INC_DIR
    #define ROOT_INC_DIR "./include"
#endif

// Macros to build the file list from a directory
#define ROOT_CONCAT(a, b) a ## b
#define ROOT_STRINGIFY(x) #x

// Helper to check if a file exists (for Linux and macOS)
#define ROOT_FILE_EXISTS(file) (access(file, F_OK) != -1)

// Helper to detect file extension
#define ROOT_FILE_IS_CPP(file) (strstr(file, ".cpp") != NULL || strstr(file, ".cxx") != NULL)
#define ROOT_FILE_IS_C(file) (strstr(file, ".c") != NULL)

// File handling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>


// Function to handle each source file
void root_handle_source_file(const char *file) {
    if (ROOT_FILE_IS_C(file)) {
        if (ROOT_OUTPUT) {
            printf("Found C source: %s\n", file);
        }
        char command[512];
        snprintf(command, sizeof(command), "%s %s %s -c %s/%s -o %s.o", ROOT_COMPILER_C, ROOT_C_FLAGS, ROOT_USER_C_FLAGS, ROOT_SRC_DIR, file, file);
        system(command);
    } else if (ROOT_FILE_IS_CPP(file)) {
        if (ROOT_OUTPUT) {
            printf("Found C++ source: %s\n", file);
        }
        char command[512];
        snprintf(command, sizeof(command), "%s %s %s -c %s/%s -o %s.o", ROOT_COMPILER_CXX, ROOT_CXX_FLAGS, ROOT_USER_CXX_FLAGS, ROOT_SRC_DIR, file, file);
        system(command);
    }
}

// Get a list of all files from the source directory
static inline void root_scan_directory(const char* dir, void (*file_handler)(const char*)) {
    struct dirent *entry;
    DIR *dp = opendir(dir);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_REG) {
            file_handler(entry->d_name);
        }
    }

    closedir(dp);
}

static inline void root_gather_sources(void) {
    root_scan_directory(ROOT_SRC_DIR, root_handle_source_file);
}

// Link object files into a final executable
static inline void root_link_executable(const char* output_name) {
    char command[1024] = {0};
    snprintf(command, sizeof(command), "%s *.o -o %s", ROOT_COMPILER_CXX, output_name);

    snprintf(command + strlen(command), sizeof(command) - strlen(command), " -lm");

    if (ROOT_OUTPUT_DEBUG) {
         printf("Linking with command %s\n", command);
    }

    system(command);
}

// Root Build
#define root_build(output_name) \
    do { \
        root_gather_sources(); \
        root_link_executable(output_name); \
    } while (0)

#endif // ROOT_H