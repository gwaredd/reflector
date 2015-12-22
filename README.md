# reflector
> A clang based tool for generating RTTI data from C/C++ code

This is the prototype code for the accompanying blog post (TBD).

## Getting Setup

### Reflector (Clang)

Reflector is a clang based tool, it requires the LLVM and Clang source tree. You can get more information on the [official site](http://clang.llvm.org/get_started.html).

**NB:** For convenience, I'm using the github mirror of the llvm/clang sources below if you're wondering why there's a difference with the official site.

Requires [git](https://git-scm.com/) and [cmake](https://cmake.org/).


**Get LLVM**

```
git clone https://github.com/llvm-mirror/llvm.git
```

**Get Clang**

```bash
cd llvm/tools
git clone https://github.com/llvm-mirror/clang.git
cd ../..
```

**Get Reflector**
```
git clone https://github.com/gwaredd/reflector.git
```
Now move the reflector code into the clang tools source tree (`llvm/tools/clang/tools/reflector`). It will be build as part of the llvm project.
```
mv reflector/llvm/tools/clang/tools/reflector/ llvm/tools/clang/tools/
```
**NB:** use `move` on windows.

**Make the things!**

If you are on a unix like environment (e.g. osx) you can use cmake to generate make files like this ...

```bash
mkdir build
cd build
cmake -G "Unix Makefiles" ../llvm
make
```

On windows you may prefer to generate a Visual Studio project instead (`cmake -G "Visual Studio 12" ..\llvm`).

***(building may take some time)***

If this all worked then the reflector binary should be here ... `build/bin/reflector`

To test reflector on some code ...

```bash
    ./bin/reflector ../reflector/tests/test.cpp
```

This should output JSON data to the stdout.

### Envision

Envision is a coffeescript script that takes the JSON data created by reflector and compiles it into C++ source targetted at the prototype gwRTTI library (`src/gwRTTI/`).

This requires [node.js](https://nodejs.org/) and [coffeescript](http://coffeescript.org/).

**Install node.js**
* Download and install from http://nodejs.org

This installs the `npm` package manager required for the next two steps.

**Install cofeescript**

```bash
npm -g install coffee-script
```

**Install envision**

To install the modules required by envision ...
```bash
cd src/envision
npm install
```
To run envision on some test data
```bash
coffee envision.coffee ../../tests/test.json test_rtti.cpp
```

***Warnings are expected with this test file.***

This should create a `test_rtti.cpp` in the envision folder.

Have fun, big love

Gw
