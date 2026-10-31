/**
 * Dedicated Benchmark to measure 10,000,000 calls With and Without inline
 * across both -O0 (Unoptimized) and -O2 (Optimized) compiler flags.
 */

#include <iostream>
#include <iomanip>
#include <chrono>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}

    friend int diffWithInline(const Time& a, const Time& b);
    friend int diffWithoutInline(const Time& a, const Time& b);
};

// With inline hint
inline int diffWithInline(const Time& a, const Time& b) {
    int diff = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return diff < 0 ? -diff : diff;
}

// Explicitly preventing inline
#if defined(__GNUC__) || defined(__clang__)
__attribute__((noinline))
#endif
int diffWithoutInline(const Time& a, const Time& b) {
    int diff = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return diff < 0 ? -diff : diff;
}

int main(int argc, char* argv[]) {
    std::string optLevel = (argc > 1) ? argv[1] : "Default";
    constexpr int CALLS = 10'000'000;
    Time t1(9, 15), t2(18, 45);

    volatile int sink = 0;

    // 1. Without Inline
    auto startNoInline = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < CALLS; ++i) {
        sink = diffWithoutInline(t1, t2);
    }
    auto endNoInline = std::chrono::high_resolution_clock::now();
    double timeNoInline = std::chrono::duration<double, std::milli>(endNoInline - startNoInline).count();

    // 2. With Inline
    auto startInline = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < CALLS; ++i) {
        sink = diffWithInline(t1, t2);
    }
    auto endInline = std::chrono::high_resolution_clock::now();
    double timeInline = std::chrono::duration<double, std::milli>(endInline - startInline).count();

    (void)sink;

    std::cout << "Optimization Level: [" << optLevel << "]\n";
    std::cout << "  Calls: 10,000,000\n";
    std::cout << "  Without Inline : " << std::fixed << std::setprecision(3) << timeNoInline << " ms ("
              << (timeNoInline * 1e6 / CALLS) << " ns/call)\n";
    std::cout << "  With Inline    : " << std::fixed << std::setprecision(3) << timeInline << " ms ("
              << (timeInline * 1e6 / CALLS) << " ns/call)\n";
    std::cout << "  Speedup Factor : " << std::setprecision(2) << (timeNoInline / timeInline) << "x\n";

    return 0;
}
