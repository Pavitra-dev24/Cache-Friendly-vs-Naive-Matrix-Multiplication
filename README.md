# Cache-Friendly vs Naive Matrix Multiplication (C++)

## Overview
This project benchmarks two different single-threaded matrix multiplication implementations in C++:

1. **Naive implementation** — loop order `(i, j, k)`
2. **Cache-friendly implementation** — loop order `(i, k, j)` to maximize memory locality

Both implementations perform exactly the same arithmetic but differ in how they access memory. This difference leads to significant performance variation on modern hardware due to CPU cache behavior.

---

## Key Features
- Single-file, single-threaded C++ program
- Uses `<chrono>` for high-resolution timing
- Generates reproducible random matrices
- Computes checksums to validate correctness
- Prints execution time, speedup, and improvement percentage
- Configurable matrix size via command-line argument (default: 1024)

---

## Performance Summary

Typical results on modern CPUs:

| Matrix Size | Naive Time | Cache-Friendly Time | Speedup | Improvement |
|-------------|------------|----------------------|---------|-------------|
| 1024×1024   | 8–15 s     | 2–4 s                | 3×–6×   | **70–85%**  |
| 1536×1536   | 30–60 s    | 4–10 s               | 5×–12×  | **80–92%**  |
| 2048×2048   | 70–150 s   | 8–20 s               | 6×–15×  | **85–93%**  |

Actual results vary across systems, but the cache-friendly implementation consistently delivers large gains.

---

## Why This Works

### Naive Version (i, j, k)
- Accesses matrix **B** column-by-column  
- Poor spatial locality  
- High cache miss rate  
- Slow on large matrices  

### Cache-Friendly Version (i, k, j)
- Accesses **B** row-by-row (sequential in memory)
- Better spatial and temporal locality
- Fewer cache misses
- Faster execution without changing algorithmic complexity

---

## Build Instructions

### Compile
```bash```
g++ -O3 -std=c++17 Matrix_Multiplication.cpp -o matmul_test


## Files

.
├── Matrix_Multiplication.cpp   # Source code
└── README.md                   # Project documentation

