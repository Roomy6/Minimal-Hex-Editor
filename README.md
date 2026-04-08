# Minimal Binary / Hex editor (MBE)

This is a super minimalist binary / hex editor and viewer written in pure C code.

---
## Examples

![Terminal GIF Example](https://raw.githubusercontent.com/Roomy6/Minimal-Hex-Editor/refs/heads/main/img/MBE-Example-1.gif)

---
## Compile from source

Instructions on how to compile the project from source.
### Linux

Requirements:
- GCC
- CMake
- Make

Clone the repository
```bash
git clone https://github.com/Roomy6/Minimal-Hex-Editor.git
```

Create a build directory
```
cd Minimal-Hex-Editor && mkdir build && cd build
```

Setup CMake and build
```bash
cmake .. && make
```

**One line clone and compile**
```bash
git clone https://github.com/Roomy6/Minimal-Hex-Editor.git && cd Minimal-Hex-Editor && mkdir build && cd build && cmake .. && make
```
The compiles program should be in the `build` directory named `mbe`

Or you *could* directly compile the program using GCC or CC
```bash
cc binEdit.c -o binEdit
```
### Windows

Requirements:
- [CMake](https://cmake.org/download/)
- [git](https://git-scm.com/install/)

Clone the repository
```bash
git clone https://github.com/Roomy6/Minimal-Hex-Editor.git
```

Create a build directory
```bash
cd Minimal-Hex-Editor && mkdir build
```

Build using CMake
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release
```

The built program *should* be in `build/Release` named `mbe.exe`.

---
## TODO

A list of things I'm working on implementing to the program.

- [X] Add text representation of Hex code.
- [ ] Implement Address jumping
- [ ] Line editing
