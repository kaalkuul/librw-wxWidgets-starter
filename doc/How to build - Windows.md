# Step by Step Visual Studio 2019 Instructions

### Install the tools

Install Visual Studio Community 2019 from [visualstudio.microsoft.com/vs/](https://visualstudio.microsoft.com/vs/).
Select all workload options for C++ and make sure to launch Visual Studio after install (to ensure that the full setup completes).

Install git for Windows from [gitforwindows.org](https://gitforwindows.org/)
Download and run the exe accepting all defaults

### Download sources

Use Git to clone the respository.

```
git clone <repo url>
```

# Manual Build Instructions

_Follow the steps below if you want to understand how to perform a manual build, or if you're troubleshooting issues with the automatic build script._

### Compile the dependencies.

Dependencies are updated seldomly, thus they are compiled out of the librw-wxWidgets-starter source tree.
Go to the Windows Start Menu and Click on "Visual Studio 2019" folder, then select the ->"x64 Native Tools Command Prompt" to open a command window and run the following:
```
cd path\to\librw-wxWidgets-starter\extdeps
cmake -S . -B build
cmake --build build
```

### Generate Visual Studio project file for librw-wxWidgets-starter, referencing the precompiled dependencies.

Go to the Windows Start Menu and Click on "Visual Studio 2019" folder, then select the ->"x64 Native Tools Command Prompt" to open a command window and run the following:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH="%CD%\extdeps\usr\local"
cmake --build build --config <Config>
```

Note that `CMAKE_PREFIX_PATH` must be absolute path.
A relative path like "..\extdeps\usr\local" does not work.

### Compile the project. 

```
start build\librw-wxWidgets-starter.sln
```

Select rwstarter as your startup project (right-click->Set as Startup Project).

Run Build->Rebuild Solution once to populate all required dependency modules.  This is NOT done automatically when you build/run.  If you run both Debug and Release variants, you will need to do this once for each.

Debug->Start Debugging or press F5

rwstarter should start. You're up and running!
