# librw wxWidgets starter

This starter project shows how to use librw within a wxWidgets based application.

This project is based on librw's [camera](https://github.com/aap/librw/tree/master/tools/camera) example.

It should run on all platforms having OpenGL 3.3 and supported by librw and wxWidgets.

It has been developped on Windows 10 using Visual Studio 2019.

## Getting the code

The easiest way to get hold of the starter code is to run the following command using a shell you can run git from:

```
git clone https://github.com/kaalkuul/librw-wxWidgets-starter.git
```

This will create the directory _librw-wxWidgets-starter_.

You may also download the code from GitHub via the "Download ZIP" approach.

## Build external binary dependencies

wxWidgets and its dependencies are updated seldomly, thus they are compiled to provide binary dependencies to librw-wxWidgets-starter.

You need a shell configured with a development toolchain. For example, on Windows you could use "Visual Studio 2019 / x64 Native Tools Command Prompt".

Now run the following:

```
cd path/to/librw-wxWidgets-starter/extdeps
cmake -S . -B build
cmake --build build
```

Compiled dependencies are installed in path/to/librw-wxWidgets-starter/extdeps/usr/local.

## Configure and build the project

From a Linux command prompt:

```
cd path/to/librw-wxWidgets-starter
cmake -S . -B build -DCMAKE_PREFIX_PATH="$PWD/extdeps/usr/local"
cmake --build build
```

From a Windows command prompt:

```
cd path\to\librw-wxWidgets-starter
cmake -S . -B build -DCMAKE_PREFIX_PATH="%CD%\extdeps\usr\local"
cmake --build build
```

The librw dependency is a source dependency.
It will be automatically downloaded and added to the project at configure time by CMake.

This project currently uses a forked version of librw to make librw support externally provided OpenGL3. In this case, OpenGL3 is provided to librw by wxWidgets.

## Run the application

In order to find the 3D graphics files, the application needs to be executed from the directory where the executable was compiled.

### On Windows

```
cd path\to\librw-wxWidgets-starter\build\src\Debug
rwstarter.exe
```

You can also run the application from Visual Studio :

```
cd path\to\librw-wxWidgets-starter
start build\rwstarter.sln
```

Select the rwstarter project as the Startup Project and run (press F5).

### On other platforms

TODO
