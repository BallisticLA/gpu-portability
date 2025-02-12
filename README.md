# demo project using hip in the way RandLAPACK requires

## correspondences with RandLAPACK
 * The ``pr_`` prefix in this project's source files is analogous to the ``rl_`` prefix in RandLAPACK source files.
 * Differences in our analogs to files in ``RandLAPACK/gpu_functions/``:
     * ``pr_cuda_kernels.cuh`` is currently empty.
     * ``pr_cusolver.hh`` includes ``cuda.h``.
 * ``pr_algorithm.hh`` is like our version of a driver file, like ``rl_cqrrpt_gpu.hh``.
 * Notable aspects of our ``pr_lapackpp.hh``:
     * It merges merges RandLAPACK's ``rl_blaspp.hh`` and ``rl_lapackpp.hh``.
     * It includes ``lapack/device.hh`` if ``ENABLE_CUDA`` is defined.

## (Trying to) build this project

Riley installed HIP by using two scripts from AMD.
The first of these actually installs lmod, which is the magic that lets you 
run ``module load <library_here>``. The second of these gets HIP. 

1. https://github.com/amd/HPCTrainingDock/blob/main/rocm/scripts/lmod_setup.sh
2. https://github.com/amd/HPCTrainingDock/blob/main/rocm/scripts/rocm_setup.sh

After running both scripts you'll need to restart your shell.

To build this project on your system, create an empty folder somewhere of your choosing. Then, run the following commands. Note that the current repo is cloned into a folder called "demo".
```
git clone https://github.com/icl-utk-edu/blaspp
git clone https://github.com/icl-utk-edu/lapackpp
git clone https://github.com/BallisticLA/gpu-portability demo
mkdir build
mkdir install
```
Install blaspp using something like this (__inside the build folder__!)
```
module load ROCm/rocm/6.3.0
# ^ you'll need to adjust for your system
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install -Dblas_int=int64 -Dgpu_backend=hip ../blaspp
make -j4 install
```
Clear the build folder and install lapackpp with something like this (again, __inside the build folder__!)
```
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install -Dblaspp_DIR=`pwd`/../install/lib/cmake/blaspp -Dgpu_backend=hip ../lapackpp
make -j4 install
```
Here's what your directory structure should look like if you followed the steps above.
```
├── blaspp
│   ├── blasppConfig.cmake.in
│   ├── CHANGELOG.md
│   ├── ... omitted ...
├── build
├── ... omitted ...
├── demo
│   ├── CMake
│   ├── CMakeLists.txt
│   ├── main.cc
│   ├── proj
│   └── README.md
├── install
│   ├── include
│   │   ├── blas
│   │   ├── blas.hh
│   │   ├── lapack
│   │   └── lapack.hh
│   └── lib
│       ├── cmake
│       ├── libblaspp.so -> libblaspp.so.1
│       ├── libblaspp.so.1 -> libblaspp.so.1.0.0
│       ├── libblaspp.so.1.0.0
│       ├── liblapackpp.so -> liblapackpp.so.1
│       ├── liblapackpp.so.1 -> liblapackpp.so.1.0.0
│       └── liblapackpp.so.1.0.0
├── lapackpp
│   ├── CHANGELOG.md
│   ├── ... omitted ...
├── lmod_setup.sh
└── rocm_setup.sh
```

### Last steps!
Once those steps were complete, I ran the following in the build folder, in an attempt to build this project.
```
rm -rf *
cmake -Dlapackpp_DIR=`pwd`/../install/lib/cmake/lapackpp -Dblaspp_DIR=`pwd`/../install/lib/cmake/blaspp -DHIP_PLATFORM=nvidia -DENABLE_CUDA=ON -Dhipplat=__HIP_PLATFORM_NVIDIA__ ../demo
```
The ``-Dhipplat=__HIP_PLATFORM_NVIDIA__`` line is there for CMake to populate a line in pr_config.hh.in.
For some reason, I needed to *also* define pr_config.hh in the "proj" folder. Right now it's hard-coded to the following:
```
#pragma once

#define Proj_HAS_OpenMP
#define HIP_PLATFORM __HIP_PLATFORM_NVIDIA__
#define ENABLE_CUDA
```
You should change the ``HIP_PLATFORM`` value as needed for what you're trying to accomplish. My understanding is that the only other legitimate value is `` __HIP_PLATFORM_AMD__``.
