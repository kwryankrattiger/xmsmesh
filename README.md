XMSMesh
========
Meshing library for XMS products

Prerequisites
--------------
In order to build this library, you must first have a few things installed.
1. [Conan](https://conan.io)
2. [CMake](https://cmake.org)
3. [Conda](https://miniconda.org)

Building
--------
Once you have cloned the repository, navigate to the `build` directory. All
of the following commands will be run from that directory unless otherwise 
noted.

1. Get External Libraries
```
    conan install .. 
``` 

2. Configure CMake
```
    cmake ..
```

3. Build 
```
    make
```

Documentation
-------------

See: [xmsmesh documentation](https://aquaveo.github.io/xmsmesh/)
