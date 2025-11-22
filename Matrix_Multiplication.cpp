#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <algorithm>

using namespace std;
using chrono_clock = std::chrono::steady_clock;

static void fill_random(vector<double>& M, int N, uint64_t seed = 0) {
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    size_t total = size_t(N) * size_t(N);
    for (size_t i = 0; i < total; ++i) M[i] = dist(rng);
}

static void matmul_naive(const vector<double>& A, const vector<double>& B, vector<double>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[size_t(i) * size_t(N) + size_t(k)] * B[size_t(k) * size_t(N) + size_t(j)];
            }
            C[size_t(i) * size_t(N) + size_t(j)] = sum;
        }
    }
}

static void matmul_cache_friendly(const vector<double>& A, const vector<double>& B, vector<double>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            double aik = A[size_t(i) * size_t(N) + size_t(k)];
            const double* b_row = &B[size_t(k) * size_t(N)];
            double* c_row = &C[size_t(i) * size_t(N)];
            for (int j = 0; j < N; ++j) {
                c_row[j] += aik * b_row[j];
            }
        }
    }
}

static double checksum(const vector<double>& M) {
    double s = 0.0;
    size_t total = M.size();
    for (size_t i = 0; i < total; ++i) s += M[i];
    return s;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N = 1024;
    if (argc >= 2) {
        long long tmp = std::atoll(argv[1]);
        if (tmp > 0) N = int(tmp);
    }

    cout << "Matrix multiply benchmark (naive vs cache-friendly)\n";
    cout << "Matrix size: " << N << " x " << N
         << "  (" << (size_t(N)*size_t(N)*sizeof(double)) / (1024*1024) << " MiB per matrix)\n";
    cout << "Single-threaded. Fixed RNG seeds for reproducibility.\n\n";

    vector<double> A(size_t(N) * size_t(N));
    vector<double> B(size_t(N) * size_t(N));
    vector<double> C(size_t(N) * size_t(N));

    fill_random(A, N, 12345u);
    fill_random(B, N, 67890u);

    std::fill(C.begin(), C.end(), 0.0);
    auto t0 = chrono_clock::now();
    matmul_naive(A, B, C, N);
    auto t1 = chrono_clock::now();
    double t_naive = std::chrono::duration<double>(t1 - t0).count();
    double chk_naive = checksum(C);

    cout << "Naive (i,j,k) time:          " << t_naive << " s\n";
    cout << "Naive checksum:             " << std::setprecision(12) << chk_naive << "\n\n";

    std::fill(C.begin(), C.end(), 0.0);
    t0 = chrono_clock::now();
    matmul_cache_friendly(A, B, C, N);
    t1 = chrono_clock::now();
    double t_cache = std::chrono::duration<double>(t1 - t0).count();
    double chk_cache = checksum(C);

    cout << "Cache-friendly (i,k,j) time: " << t_cache << " s\n";
    cout << "Cache-friendly checksum:    " << std::setprecision(12) << chk_cache << "\n\n";

    if (std::fabs(chk_naive - chk_cache) > 1e-9 * std::fabs(chk_naive)) {
        cout << "Warning: results differ! (checksums differ by "
             << std::fabs(chk_naive - chk_cache) << ")\n";
    }

    if (t_cache > 0.0) {
        cout << "Speedup (naive / cache-friendly): " << (t_naive / t_cache) << "x\n";
    } else {
        cout << "Cache-friendly time too small to compute speedup reliably.\n";
    }
    
    return 0;
}
