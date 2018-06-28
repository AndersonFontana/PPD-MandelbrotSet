
# CUDA

Um exemplo de compilação e execução dos códigos de MandelBrot sequencial e paralelo em CUDA

```

g++ mandelcuda.cpp -std=c++11 -o mandelOrg
time ./mandelOrg 1024 768 18000 > mandelOrg.out

nvcc mandelbrotCUDA.cu -std=c++11 -o mandelCuda
time ./mandelCuda 1024 768 18000 > mandelCuda.out

diff mandelCuda.out mandelOrg.out

```

