#pragma once

#include "pr_config.hh"

#include <blas.hh>

using Layout = blas::Layout;
using Op = blas::Op;
using Side = blas::Side;
using Diag = blas::Diag;
using Uplo = blas::Uplo;

#define LAPACK_COMPLEX_CPP 1
#include <lapack.hh>

using Job = lapack::Job;
using MatrixType = lapack::MatrixType;
using Norm = lapack::Norm;

#if defined(ENABLE_CUDA)
#include "lapack/device.hh"
#endif
