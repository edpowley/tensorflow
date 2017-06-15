This is a fork of [TensorFlow](README_original.md) which includes a DLL wrapping a minimal subset of functionality for running models.

## Build instructions

We basically follow TensorFlow's [CMake](tensorflow/contrib/cmake/) build process. However we don't care about building the Python bindings, so no need to install Python 3.5 or SWIG if you don't already have them.

* Clone this repository using your preferred git client.
* Ensure you have a recent version of [Visual Studio](https://www.visualstudio.com/) (tested with 2015 and 2017), with the C++ compiler and libraries.
* Install [CMake](https://cmake.org/), making sure to select the option to add it to your path.
* Open the Start menu and run "Visual Studio 2017 -> x64 Native Tools Command Prompt".
* `cd` to the `tensorflow\tensorflow\contrib\cmake` directory.
* Type the following commands. Note that `run_cmake.bat` will delete all contents of the `build` directory, if present.
```
run_cmake.bat
run_msbuild_use_model_dll.bat
```
* Once that's done, you should find `tf_use_model_dll.dll` and `tf_use_model_dll.lib` in the `tensorflow\contrib\cmake\build\Release` directory.

Having done this once, you should be able to open `tensorflow\tensorflow\contrib\cmake\build\tensorflow.sln` in Visual C++ and build the `tf_use_model_dll` project from there. Just be sure to always build the "Release x64" configuration.

## Using the DLL in your Visual C++ project

* Switch your build configuration to x64 (x86 is not supported).
* Copy the following files to your project:
```
tensorflow\contrib\cmake\build\Release\tf_use_model_dll.dll
tensorflow\contrib\cmake\build\Release\tf_use_model_dll.lib
tensorflow\contrib\use_model_dll\use_model_dll.h
```
* In Visual C++, find your project in the Solution Explorer. Right click it and select "Properties".
* Under "Linker -> Input -> Additional Dependencies", add `tf_use_model_dll.lib`.
* Refer to `use_model_dll.h` for details of available functions.

## Warning messages

Note that your program will print warnings similar to the following:
```
2017-06-15 12:59:32.434009: W x:\tensorflow\tensorflow\core\platform\cpu_feature_guard.cc:45] The TensorFlow library wasn't compiled to use SSE instructions, but these are available on your machine and could speed up CPU computations.
```
These can safely be ignored:
* SSE and SSE2 instructions are used, however Visual C++ does not set the `__SSE__` and `__SSE2__` preprocessor symbols so the warning messages are triggered erroneously.
* AVX and AVX2 can be added if desired, by editing `tensorflow\contrib\cmake\run_cmake.bat` and adding `-Dtensorflow_WIN_CPU_SIMD_OPTIONS=/arch:AVX` (or `AVX2`) to the end of the `cmake` command line; however note that this will cause a fatal error on CPUs not supporting these instructions.
* Other instruction sets (SSE3, SSE4.x, FMA) are not supported by the Visual C++ compiler at present so cannot be enabled.
