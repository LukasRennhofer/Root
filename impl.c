#define ROOT_COMPILER_C "gcc"
#define ROOT_COMPILER_CXX "g++"
#define ROOT_C_FLAGS "-Wall -O2 -g -DDEBUG"
#define ROOT_CXX_FLAGS "-Wall -O2 -g -DDEBUG"
#define ROOT_SRC_DIR "."
#define ROOT_INC_DIR "."

#include "root.h"

int main() {
    root_build("build");
    return 0;
}
