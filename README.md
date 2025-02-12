# demo project using hip in the way RandLAPACK requires

## correspondences
 * The ``pr_`` prefix in this project's source files is analogous to the ``rl_`` prefix in RandLAPACK source files.
 * Differences in our analogs to files in ``RandLAPACK/gpu_functions/``:
     * ``pr_cuda_kernels.cuh`` is currently empty.
     * ``pr_cusolver.hh`` includes ``cuda.h``.
 * ``pr_algorithm.hh`` is like our version of a driver file, like ``rl_cqrrpt_gpu.hh``.
 * Notable aspects of our ``pr_lapackpp.hh``:
     * It merges merges RandLAPACK's ``rl_blaspp.hh`` and ``rl_lapackpp.hh``.
     * It includes ``lapack/device.hh`` if ``ENABLE_CUDA`` is defined.

## other notes


