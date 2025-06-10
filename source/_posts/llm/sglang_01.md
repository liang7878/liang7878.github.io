---
title: mac本地编译sglang踩坑贴
date: 2025-06-02 23:49:54
tags:
  - LLM
  - SGLang
---
```bash
CMake Error at /opt/homebrew/Cellar/cmake/3.28.3/share/cmake/Modules/Internal/CMakeCUDAFindToolkit.cmake:104 (message):
  Failed to find nvcc.

  Compiler requires the CUDA toolkit.  Please set the CUDAToolkit_ROOT
  variable.
Call Stack (most recent call first):
  /opt/homebrew/Cellar/cmake/3.28.3/share/cmake/Modules/CMakeDetermineCUDACompiler.cmake:89 (cmake_cuda_find_toolkit)
  CMakeLists.txt:2 (project)
```