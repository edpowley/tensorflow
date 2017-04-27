This is a fork of [TensorFlow](README_original.md) which includes a DLL wrapping a minimal subset of functionality for running models.

## Build instructions

We basically follow TensorFlow's [CMake](tensorflow/contrib/cmake/) build process. However we don't care about building the Python bindings, so no need to install Python 3.5 or SWIG if you don't already have them.

* Clone this repository using your preferred git client.
* Ensure you have [Visual Studio 2017](https://www.visualstudio.com/) with the C++ compiler and libraries.
* Install [CMake](https://cmake.org/), making sure to select the option to add it to your path.
* Open the Start menu and run "Visual Studio 2017 -> x64 Native Tools Command Prompt".
* `cd` to the `tensorflow\tensorflow\contrib\cmake` directory.
* Type the following commands:
```
mkdir build
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
