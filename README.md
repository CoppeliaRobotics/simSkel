# Plugin Skeleton (new generation) for CoppeliaSim

This is an example of developing CoppeliaSim plugins using the [libPlugin](http://github.com/CoppeliaRobotics/libPlugin.git) framework.

With libPlugin, callbacks signature and documentation are specified using a single XML file. The build tool will generate additional .cpp and .h files to be compiled along the project, and .html file for the documentation.

### Compiling

1. Install required packages for [libPlugin](https://github.com/CoppeliaRobotics/libPlugin): see libPlugin's README
2. Checkout and compile
```
$ git clone --recursive https://github.com/CoppeliaRobotics/simExtPluginSkeletonNG.git
$ cmake .
$ cmake --build .
```
