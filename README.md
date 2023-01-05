# librw CMake starter

This starter project shows how to use CMake to create a project with librw and GLFW.

This project is based on librw's [camera](https://github.com/aap/librw/tree/master/tools/camera) example.

As such it should run on all platforms supported by librw and GLFW.

librw on GitHub: [github.com/aap/librw](https://github.com/aap/librw)

GLFW homepage: [glfw.org](https://www.glfw.org/)  
GLFW on GitHub: [github.com/glfw/glfw](https://www.github.com/glfw/glfw)

## Getting the code

The easiest way to get hold of the starter code is to run the following command using a shell you can run git from:

```
git clone https://github.com/kaalkuul/librw-cmake-starter.git
```

This will create the directory _librw-cmake-starter_.

You may also download the code from GitHub via the "Download ZIP" approach.

Dependencies (librw and GLFW) will be automatically downloaded at configure time by CMake.

## Using CMake to create and build the project

From a command prompt:

```
cd path/to/librw-cmake-starter
cmake -S . -B build
cmake --build build
```
