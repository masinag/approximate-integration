# Approximate Integration
Command line tool that computes the approximate integral of arbitrary functions over N-dimensional convex polytopes, using the [VolEsti](https://github.com/GeomScale/volesti) C++ library.

## Building
```
mkdir build
cd build
cmake ..
make
```
This will create an executable file named `bin/volesti_integrate`. 

## Usage
```
❯ bin/volesti_integrate --help
Usage: ./volesti_integrate [options] polytope_file integrand_file error 

Approximate integration of polynomial over polytopes

Positional arguments:
polytope_file   The file containing the polytope to integrate over in .hrep format
integrand_file  The file containing the function to integrate in exprtk format
error           The relative error tolerated

Optional arguments:
-h --help       shows help message and exits [default: false]
-v --version    prints version information and exits [default: false]
--volume        The method used to compute the volume. The available methods are:
        CB      Cooling Balls
        CG      Cooling Gaussians
        SOB     Sequence Of Balls [default: "CB"]
--walk          The type of random walk to sample points. The available types are:
        Ba      Ball Walk
        RDHR    Random Direction Hit and Run
        CDHR    Coordinate Direction Hit and Run
        Bi      Billiard Walk
        ABi     Accelerated Billiard Walk [default: "CDHR"]
--N             The number of points used to estimate the integral [default: 1000]
--wlength       The length of the random walk to sample random points. If 0, a default value is set to: 
        ABi     min(10, d^2)
        Bi      min(10, d^2)
        Ba      min(10, d^2.5)
        CDHR    min(10, d^3)
        RDHR    min(10, d^3)
where d is the number of dimensions of the polytope 
```

## Running Examples
Examples can be runned with:
```
bin/volesti_integrate examples/example-gaussian/polytope.hrep examples/example-gaussian/gaussian.expr 0.001 
```
