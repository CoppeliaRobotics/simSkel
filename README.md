# Plugin Skeleton (new generation) for CoppeliaSim

This is an example of developing CoppeliaSim plugins using simStubsGen and simPlusPlus.

Callbacks signature and documentation are specified using a single XML file (callbacks.xml). The build tool will generate additional .cpp and .h files to be compiled along the project, and .html file for the documentation.

### Compiling

1. Install required packages for simStubsGen: see the README file of simStubsGen.
2. Checkout, compile and install into CoppeliaSim:
```sh
$ git clone https://github.com/CoppeliaRobotics/simExtSkel.git
$ cd simExtSkel
$ git checkout coppeliasim-v4.5.0-rev0
$ mkdir -p build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ cmake --build .
$ cmake --install .
```

NOTE: replace `coppeliasim-v4.5.0-rev0` with the actual CoppeliaSim version you have.
