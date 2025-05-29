# Root

> Warning: STILL IN PROGRESS AND CODE CAN CHANGE AT ANY TIME!

**Forget CMake. Forget Bazel. Forget the bloated corporate trash that enslaves your soul.**

Root is the **nuclear option** in build systems. Designed to burn down the empire of complexity and spit in the face of those massive, convoluted beasts masquerading as tools.

If you want a build system that doesn’t choke on dependencies, that compiles your C and C++ projects with the blazing thought of a thousand suns — and looks you dead in the eye while doing it — then Root is your salvation.

## What is Root?

- A single-header, zero-dependency, no-bullshit build system.
- Written in pure C (and usable in C++) because why complicate what doesn’t need it?
- **Parallel compilation**? Not yet.
- **Incremental builds**? Naturally.
- Precompiled headers? If you want ‘em.
- Designed to be **ultra minimal**, and **utterly unstoppable**.

## Why Root?

**Root says NO.**

Root is the ***TempleOS*** of build systems. It’s raw, it’s brutal, it’s pure **code mysticism**.

## Quickstart

Drop `root.h` in your project. Include it. Define your compiler and flags. Point it to your source directory. Call `root_build("my_executable")`. Done.

```c
#define ROOT_COMPILER_C "gcc"
#define ROOT_COMPILER_CXX "g++"
#define ROOT_C_FLAGS "-Wall -O2 -g -DDEBUG"
#define ROOT_CXX_FLAGS "-Wall -O2 -g -DDEBUG"
#define ROOT_SRC_DIR "."
#define ROOT_INC_DIR "."

#include "root.h"

int main() {
    root_build("my_executable");
    return 0;
}
