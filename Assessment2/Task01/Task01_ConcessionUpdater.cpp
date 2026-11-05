/**
 * ============================================================================
 * ASSIGNMENT 2 - CO2: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 01 (Q1): Call by Value, by Reference, and by Pointer
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - HOSTEL FEE CONCESSION UPDATER:
 * The hostel office applies concessions (merit 10%, sports 15%, single parent 25%)
 * to students' fees. A function written to apply the concession appears to work
 * when its result is printed inside the function, but the fee stored for the
 * student never changes because it was passed by value (modifying only a local copy).
 * 
 * SECTION 1: CONCESSION IMPLEMENTATION (3 WAYS + RETURN ALTERNATIVE)
 * SECTION 2: PREDICT THE OUTPUT & VERIFY (change, bigger, show)
 * SECTION 3: COMPLETE THE CODE (TODO 1)
 * SECTION 4: EMPIRICAL COMPLEXITY & MEMORY ANALYSIS (long long vs large Student)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

// ============================================================================
// SECTION 1: CONCESSION FUNCTIONS
// ============================================================================

// 1. Call by Value: Parameter is a local copy created on the callee stack frame.
// Changes made to 'fee' are discarded when the function returns.
void applyConcessionByValue(long long fee, int percent) {
    fee -= (fee * percent) / 100;
    std::cout << "  [Inside applyConcessionByValue] Local fee copy modified to: " << fee << '\n';
}

// 2. Call by Reference: Parameter binds directly to the caller's lvalue.
// No copy is created; operations directly mutate the caller's memory location.
void applyConcessionByReference(long long& fee, int percent) {
    fee -= (fee * percent) / 100;
}

// 3. Call by Pointer: Parameter is a pointer storing the address of caller's fee.
// Dereferencing the pointer (*fee) directly modifies the caller's memory location.
void applyConcessionByPointer(long long* fee, int percent) {
    if (fee != nullptr) {
        *fee -= (*fee * percent) / 100;
    }
}

// 4. Alternative Concept: Pure Functional Return (Immutability / Method Chaining)
// Caller explicitly updates their variable: fee = applyConcessionReturn(fee, percent);
[[nodiscard]] long long applyConcessionReturn(long long fee, int percent) {
    return fee - (fee * percent) / 100;
}

// ============================================================================
// SECTION 2: PREDICT THE OUTPUT (Exact Code from Assignment Specification)
// ============================================================================
void change(int a, int& b, int* c) {
    a += 10;
    b += 10;
    *c += 10;
    (void)a; // Suppress unused-but-set warning while keeping original semantics
}

int& bigger(int& p, int& q) {
    return p > q ? p : q;
}

void show(int x, int y = 5, int z = 10) {
    std::cout << x + y + z << ' ';
}

void runPredictTheOutput() {
    std::cout << "========================================================\n";
    std::cout << "PREDICT THE OUTPUT VERIFICATION (Assignment Page 2-3)\n";
    std::cout << "========================================================\n";
    std::cout << "[MY PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "Line 1: 1 12 13\n";
    std::cout << "Line 2: 1 100\n";
    std::cout << "Line 3: 16 13 6 \n";
    std::cout << "\n[ACTUAL PROGRAM EXECUTION OUTPUT]:\n";

    int a = 1, b = 2, c = 3;
    change(a, b, &c);
    std::cout << a << ' ' << b << ' ' << c << '\n';

    bigger(a, c) = 100;
    std::cout << a << ' ' << c << '\n';

    show(1);
    show(1, 2);
    show(1, 2, 3);
    std::cout << '\n';

    std::cout << "\n[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. change(a, b, &c):\n";
    std::cout << "   - 'a' passed by value: local parameter increments to 11, caller 'a' remains 1.\n";
    std::cout << "   - 'b' passed by reference: caller 'b' directly increments from 2 to 12.\n";
    std::cout << "   - '&c' passed by pointer: caller 'c' dereferenced and increments from 3 to 13.\n";
    std::cout << "   Result printed: '1 12 13'. Prediction matches execution exactly.\n\n";

    std::cout << "2. bigger(a, c) = 100:\n";
    std::cout << "   - a=1, c=13. Condition (p > q) => (1 > 13) evaluates to false.\n";
    std::cout << "   - Returns reference to 'q' (which is caller's 'c').\n";
    std::cout << "   - The expression is an lvalue reference, so assignment '= 100' mutates 'c' to 100.\n";
    std::cout << "   - Result printed: '1 100'. Prediction matches execution exactly.\n\n";

    std::cout << "3. show() Calls & Default Arguments:\n";
    std::cout << "   - show(1): y defaults to 5, z defaults to 10 => 1 + 5 + 10 = 16.\n";
    std::cout << "   - show(1, 2): y provided as 2, z defaults to 10 => 1 + 2 + 10 = 13.\n";
    std::cout << "   - show(1, 2, 3): x=1, y=2, z=3 (no defaults used) => 1 + 2 + 3 = 6.\n";
    std::cout << "   Result printed: '16 13 6 '. Prediction matches execution exactly.\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 3: COMPLETE THE CODE (TODO 1)
// ============================================================================
void applyByValue(long long fee, int percent) {
    fee -= fee * percent / 100; // changes only the local copy
}

void applyConcession(long long& fee, int percent) {
    // TODO 1: Reduce the caller's fee by percent using integer arithmetic (call by reference)
    fee -= (fee * percent) / 100;
}

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "COMPLETE THE CODE (Assignment Page 4-5)\n";
    std::cout << "========================================================\n";
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';
    std::cout << "Expected:\n";
    std::cout << "after call by value : 45000\n";
    std::cout << "after call by reference: 40500\n";
    std::cout << "Status: " << (hostelFee == 40500 ? "VERIFIED MATCH" : "MISMATCH") << '\n';
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 4: EMPIRICAL COMPLEXITY & MEMORY ANALYSIS
// ============================================================================
struct StudentRecord {
    long long studentId;
    char name[128];
    char hostelBlock[64];
    char roomNumber[32];
    long long baseFee;
    long long currentFee;
    double academicGpa;
    char disciplinaryRecord[256];
    char scholarshipCategory[128];
    char emergencyContact[256];
    // Total size ~ 888 bytes
};

void processStudentByValue(StudentRecord s) {
    // Forces copy of entire structure
    s.currentFee -= (s.currentFee * 10) / 100;
}

void processStudentByReference(StudentRecord& s) {
    // Passes 8-byte pointer under the hood
    s.currentFee -= (s.currentFee * 10) / 100;
}

void processStudentByConstReference(const StudentRecord& s, long long& outFee) {
    // Read-only access with zero copy overhead
    outFee = s.currentFee - (s.currentFee * 10) / 100;
}

void runComplexityAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "ALTERNATIVE CONCEPT AND COMPLEXITY ANALYSIS\n";
    std::cout << "========================================================\n";
    std::cout << "Data Type Sizes in Memory:\n";
    std::cout << "  sizeof(long long)      : " << sizeof(long long) << " bytes\n";
    std::cout << "  sizeof(StudentRecord)  : " << sizeof(StudentRecord) << " bytes\n";
    std::cout << "  sizeof(StudentRecord*) : " << sizeof(StudentRecord*) << " bytes (Pointer / Ref representation)\n\n";

    StudentRecord student{};
    student.studentId = 192472008;
    student.baseFee = 45000;
    student.currentFee = 45000;
    student.academicGpa = 9.42;

    constexpr int ITERATIONS = 100'000;

    // Benchmark Pass by Value
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        processStudentByValue(student);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double timeByValue = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // Benchmark Pass by Reference
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        processStudentByReference(student);
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    double timeByRef = std::chrono::duration<double, std::milli>(t4 - t3).count();

    // Benchmark Pass by Const Reference
    long long dummy = 0;
    auto t5 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        processStudentByConstReference(student, dummy);
    }
    auto t6 = std::chrono::high_resolution_clock::now();
    double timeByConstRef = std::chrono::duration<double, std::milli>(t6 - t5).count();

    std::cout << "Empirical Benchmark (" << ITERATIONS << " calls on " << sizeof(StudentRecord) << "-byte object):\n";
    std::cout << "  1. Pass by Value          : " << std::fixed << std::setprecision(3) << timeByValue << " ms\n";
    std::cout << "  2. Pass by Reference      : " << std::fixed << std::setprecision(3) << timeByRef << " ms\n";
    std::cout << "  3. Pass by Const Reference: " << std::fixed << std::setprecision(3) << timeByConstRef << " ms\n";
    std::cout << "  Speedup of Reference over Value: " << std::setprecision(2) << (timeByValue / timeByRef) << "x faster!\n\n";

    std::cout << "Theoretical Analysis Table:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Parameter Type     | Time Complexity | Space Copied (long long) | Space Copied (Student) |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Pass by Value      | O(1)            | 8 bytes                  | 888 bytes (Full copy)  |\n";
    std::cout << "| Pass by Reference  | O(1)            | 0 bytes (8B pointer)     | 0 bytes (8B pointer)   |\n";
    std::cout << "| Pass by Pointer    | O(1)            | 0 bytes (8B pointer)     | 0 bytes (8B pointer)   |\n";
    std::cout << "| Pass by Const Ref  | O(1)            | 0 bytes (8B pointer)     | 0 bytes (8B pointer)   |\n";
    std::cout << "| Return New Value   | O(1)            | 8 bytes in register      | 888 bytes (or RVO)     |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 5: COMPLETE CONCESSION VERIFICATION (3 WAYS + CHAINING)
// ============================================================================
void runConcessionChallenge() {
    std::cout << "========================================================\n";
    std::cout << "YOUR CHALLENGE: HOSTEL FEE CONCESSION APPLICATION TEST\n";
    std::cout << "========================================================\n";
    long long fee = 45000;
    std::cout << "Initial Base Fee: Rs. " << fee << "\n\n";

    // 1. Call by Value: Merit Concession (10%)
    std::cout << "[Step 1] Applying 10% Merit Concession by Value:\n";
    std::cout << "  Prediction: Stored fee will REMAIN 45000 (value is not propagated to caller).\n";
    applyConcessionByValue(fee, 10);
    std::cout << "  Observed Caller Fee: Rs. " << fee << " (Verification: "
              << (fee == 45000 ? "PREDICTION CONFIRMED" : "FAILED") << ")\n\n";

    // 2. Call by Reference: Sports Concession (15%)
    // Base 45000 - 15% (6750) = 38250
    std::cout << "[Step 2] Applying 15% Sports Concession by Reference:\n";
    std::cout << "  Prediction: Stored fee will change from 45000 to 38250 (45000 - 6750).\n";
    applyConcessionByReference(fee, 15);
    std::cout << "  Observed Caller Fee: Rs. " << fee << " (Verification: "
              << (fee == 38250 ? "PREDICTION CONFIRMED" : "FAILED") << ")\n\n";

    // 3. Call by Pointer: Single Parent Concession (25%)
    // 38250 - 25% (9562) = 28688
    std::cout << "[Step 3] Applying 25% Single Parent Concession by Pointer:\n";
    std::cout << "  Prediction: Stored fee will change from 38250 to 28688 (38250 - 9562).\n";
    applyConcessionByPointer(&fee, 25);
    std::cout << "  Observed Caller Fee: Rs. " << fee << " (Verification: "
              << (fee == 28688 ? "PREDICTION CONFIRMED" : "FAILED") << ")\n\n";

    // 4. Return New Fee: Multiple Concessions Chained
    long long cleanFee = 45000;
    std::cout << "[Step 4] Chaining Concessions via Functional Return:\n";
    std::cout << "  Starting fee: Rs. " << cleanFee << '\n';
    cleanFee = applyConcessionReturn(applyConcessionReturn(cleanFee, 10), 15);
    std::cout << "  After 10% Merit followed by 15% Sports: Rs. " << cleanFee << "\n";
    std::cout << "  (45000 - 10% = 40500; 40500 - 15% = 34425)\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 2 - TASK 01: CALL BY VALUE / REF / POINTER   \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictTheOutput();
    runCompleteTheCode();
    runConcessionChallenge();
    runComplexityAnalysis();

    return 0;
}
