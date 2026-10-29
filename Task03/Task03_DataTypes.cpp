/**
 * ============================================================================
 * Assignment 1 - Task 03: Data Types, Type Compatibility & Precision Analysis
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Demonstrating:
 * 1. sizeof and range audit across target architecture types
 * 2. Precision catastrophe: Floating-point double rupees vs integer long long paise
 * 3. Daily network revenue overflow: Why 32-bit int overflows at 3,000,000 trips
 * 4. Mixed-type expression evaluation and usual arithmetic conversions
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cstdint>

int main() {
    std::cout << "====================================================================\n";
    std::cout << " TASK 03: DATA TYPES, SIZEOF, OVERFLOW & PRECISION EVALUATION       \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    // 1. Data Type Audit Table with sizeof
    std::cout << "[1] SYSTEM DATA TYPE AUDIT (x86_64 GCC Windows):\n";
    std::cout << "    -----------------------------------------------------------------\n";
    std::cout << "    Data Entity        Chosen Type        Size (bytes)  Valid Domain / Range\n";
    std::cout << "    -----------------------------------------------------------------\n";
    std::cout << "    isPeakHour         bool               " << sizeof(bool)
              << "             true / false\n";
    std::cout << "    concessionCode     char               " << sizeof(char)
              << "             'R', 'S', 'C'\n";
    std::cout << "    zoneIndex          unsigned char      " << sizeof(unsigned char)
              << "             0 .. 255\n";
    std::cout << "    hour24 / minute    int                " << sizeof(int)
              << "             0..23 / 0..59\n";
    std::cout << "    farePaise          long long          " << sizeof(long long)
              << "             ~[-9e18, +9e18]\n";
    std::cout << "    cardBalancePaise   long long          " << sizeof(long long)
              << "             Exact paise\n";
    std::cout << "    networkJourneys    unsigned int       " << sizeof(unsigned int)
              << "             0 .. 4,294,967,295\n";
    std::cout << "    networkRevenue     long long          " << sizeof(long long)
              << "             Multi-crore paise\n";
    std::cout << "    float (Rupees)     float              " << sizeof(float)
              << "             24-bit mantissa (UNSAFE)\n";
    std::cout << "    double (Rupees)    double             " << sizeof(double)
              << "             53-bit mantissa (UNSAFE)\n";
    std::cout << "    -----------------------------------------------------------------\n\n";

    // 2. Demonstration of Precision Catastrophe: Double vs Integer Paise
    std::cout << "[2] FLOATING-POINT ROUNDING ERROR MEASUREMENT:\n";
    std::cout << "    Scenario: Adding Rs 0.10 (10 paise surcharge component) 1,000,000 times.\n";

    double doubleRupeesTotal = 0.0;
    long long paiseTotal = 0;
    const int iterations = 1000000;

    for (int i = 0; i < iterations; ++i) {
        doubleRupeesTotal += 0.10; // Incurring IEEE 754 binary representation error
        paiseTotal += 10;          // Exact integer arithmetic
    }

    double expectedRupees = iterations * 0.10; // Exactly 100,000.00
    double actualFromPaise = paiseTotal / 100.0;
    double drift = doubleRupeesTotal - expectedRupees;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "    Expected Total:       Rs " << expectedRupees << "\n";
    std::cout << "    Double Accumulator:   Rs " << doubleRupeesTotal << "\n";
    std::cout << "    Paise Accumulator:    Rs " << actualFromPaise << "\n";
    std::cout << "    Accumulated Drift:    Rs " << drift << " (" << (drift * 100.0) << " paise)\n";
    std::cout << "    EVALUATION: Double introduces cumulative financial leakage!\n\n";

    // 3. Network Scale Revenue Overflow Analysis
    std::cout << "[3] DAILY NETWORK SCALE OVERFLOW PROOF (3,000,000 JOURNEYS):\n";
    long long journeys = 3000000LL;
    long long avgFarePaise = 2500LL; // Rs 25.00 average fare
    long long totalRevenuePaise = journeys * avgFarePaise; // 7,500,000,000 paise = Rs 7.5 Crore

    std::cout << std::resetiosflags(std::ios::floatfield);
    std::cout << "    Total Journeys / Day:  " << journeys << "\n";
    std::cout << "    Average Fare:          " << avgFarePaise << " paise (Rs 25.00)\n";
    std::cout << "    Exact Total Revenue:   " << totalRevenuePaise << " paise (Rs 75,000,000.00)\n";
    std::cout << "    32-bit INT_MAX:        " << INT_MAX << " paise (Rs " << INT_MAX / 100 << ")\n";

    // Simulating 32-bit signed int overflow
    int overflowedIntTotal = 0;
    for (int i = 0; i < 3000000; ++i) {
        overflowedIntTotal += static_cast<int>(avgFarePaise);
    }
    std::cout << "    Accumulated in int:    " << overflowedIntTotal << " paise\n";
    std::cout << "    OVERFLOW VERDICT:      int overflows into NEGATIVE revenue! ("
              << overflowedIntTotal << ")\n";
    std::cout << "                           long long is mandatory for metro network accounting.\n\n";

    // 4. Type Compatibility & Mixed-Type Arithmetic Promotion
    std::cout << "[4] MIXED-TYPE ARITHMETIC CONVERSIONS IN C++:\n";
    int a = 10;
    double b = 2.5;
    bool flag = true;
    auto mixedResult = a * b + flag; // int promoted to double, bool promoted to int then double
    std::cout << "    Expression: int(10) * double(2.5) + bool(true)\n";
    std::cout << "    Evaluated Result: " << mixedResult << "\n";
    std::cout << "    Result Type Size: " << sizeof(mixedResult) << " bytes (promoted to double)\n";
    std::cout << "====================================================================\n";

    return 0;
}
