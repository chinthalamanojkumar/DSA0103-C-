/**
 * ============================================================================
 * Assignment 1 - Task 04: Control Implicit Conversions & Predict Output
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Part 1: Exact Given Assignment Program (Predict the Output)
 * Part 2: Compiler Warning & Conversion Analysis (-Wsign-compare, brace initialization)
 * Part 3: Corrected / Modern C++ Version using static_cast and brace init
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

void runGivenProgram() {
    std::cout << "--- [PART 1: EXECUTION OF GIVEN ASSIGNMENT CODE] ---\n";
    int a = 7, b = 2;
    double c = a / b;
    double d = static_cast<double>(a) / b;
    std::cout << c << ' ' << d << '\n';

    char ch = 'A' + 2;
    std::cout << ch << ' ' << ch + 1 << '\n';

    bool flag = 5;
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u = 1;
    int s = -1;
    // Compiling this triggers: warning: comparison of integer expressions of different signedness: 'int' and 'unsigned int' [-Wsign-compare]
    std::cout << (s < u ? "s < u" : "s >= u") << '\n';
}

void runCorrectedProgram() {
    std::cout << "\n--- [PART 2: REFACTORED SAFE CONVERSIONS & BRACE INIT] ---\n";
    // 1. Preventing unintended integer truncation using static_cast
    int a{7};
    int b{2};
    double exactDivision{static_cast<double>(a) / static_cast<double>(b)};
    std::cout << "Exact Division with static_cast: " << exactDivision << '\n';

    // 2. Safe character arithmetic
    char ch{static_cast<char>('A' + 2)};
    char nextCh{static_cast<char>(ch + 1)};
    std::cout << "Character preserved: " << ch << " -> " << nextCh << '\n';

    // 3. Strict boolean initialization
    bool safeFlag{5 != 0};
    std::cout << "Explicit boolean condition: " << std::boolalpha << safeFlag << '\n';

    // 4. Safe signed-unsigned comparison
    unsigned int u{1u};
    int s{-1};
    // Correct comparison: check sign before casting or compare safely
    bool isLessThan = (s < 0) || (static_cast<unsigned int>(s) < u);
    std::cout << "Safely evaluated (s < u) taking sign into account: "
              << (isLessThan ? "true (s < u)" : "false (s >= u)") << '\n';
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << " TASK 04: IMPLICIT CONVERSIONS, WARNING AUDIT & PREDICT OUTPUT      \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    runGivenProgram();
    runCorrectedProgram();

    std::cout << "\n====================================================================\n";
    return 0;
}
