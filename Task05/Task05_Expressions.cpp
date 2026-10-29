/**
 * ============================================================================
 * Assignment 1 - Task 05: Operator Precedence, Expressions & Peak Check Logic
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Part 1: Exact Given Assignment Program (Predict the Output)
 * Part 2: Operator-by-Operator Precedence & Associativity Trace
 * Part 3: Peak Check Logic Evaluation (Boolean Expression vs 24-hr Lookup Table)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>

void runGivenProgram() {
    std::cout << "--- [PART 1: EXECUTION OF GIVEN ASSIGNMENT CODE] ---\n";
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';
}

void evaluatePeakExpressions() {
    std::cout << "\n--- [PART 2: PEAK HOUR EXPRESSION EVALUATION] ---\n";
    // Expression A: Parenthesized
    // (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)
    // Expression B: Unparenthesized
    // hour >= 8 && hour < 10 || hour >= 17 && hour < 20
    //
    // Precedence: Relational (>=, <) > Logical AND (&&) > Logical OR (||)
    // Both evaluate identically, but Expression A prevents cognitive fatigue & audit ambiguity.

    // 24-hour Lookup Table Alternative (24 bytes)
    constexpr bool PEAK_TABLE[24] = {
        false, false, false, false, false, false, false, false, // 00-07
        true,  true,                                            // 08-09 (Peak)
        false, false, false, false, false, false, false,        // 10-16
        true,  true,  true,                                     // 17-19 (Peak)
        false, false, false, false                              // 20-23
    };

    std::cout << "Hour | Parenthesized | Unparenthesized | 24-hr Table | Status\n";
    std::cout << "-----+---------------+-----------------+-------------+----------\n";
    int testHours[] = {7, 8, 9, 10, 16, 17, 19, 20, 21};
    for (int h : testHours) {
        bool exprA = (h >= 8 && h < 10) || (h >= 17 && h < 20);
        bool exprB = h >= 8 && h < 10 || h >= 17 && h < 20;
        bool tableVal = PEAK_TABLE[h];

        std::cout << std::setw(4) << h << " | "
                  << std::setw(13) << std::boolalpha << exprA << " | "
                  << std::setw(15) << exprB << " | "
                  << std::setw(11) << tableVal << " | "
                  << (exprA ? "PEAK SURCHARGE (+20%)" : "OFF-PEAK (Standard)") << "\n";
    }
}

int main() {
    std::cout << "====================================================================\n";
    std::cout << " TASK 05: OPERATOR PRECEDENCE, EXPRESSIONS & PEAK LOGIC EVALUATION  \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    runGivenProgram();
    evaluatePeakExpressions();

    std::cout << "\n====================================================================\n";
    return 0;
}
