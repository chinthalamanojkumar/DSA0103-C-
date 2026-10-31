/**
 * ============================================================================
 * ASSIGNMENT 2 - CO2: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 03 (Q3): Default Arguments and Function Overloading
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - COURIER CHARGE CALCULATOR:
 * A courier counter charges by weight; express delivery and insurance are optional.
 * The original developer wrote:
 *   charge(double weight)
 *   charge(double weight, bool express)
 *   charge(double weight, bool express = false, bool insured = false)
 * The program failed to compile due to ambiguous call errors.
 * 
 * SECTIONS:
 * 1. Cause of Ambiguity & Formal Compiler Error Analysis
 * 2. Unambiguous Redesign with Default Arguments and Overloading
 * 3. Predict-Before-Execution Verification of 6 Calls (including charge(2) and charge(2.0f))
 * 4. Compiler Overload Resolution Mechanics (Exact Match, Promotion, Conversion)
 * 5. Alternative Concept: Single Parcel Structure with Optional Fields vs Overloads
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>

// ============================================================================
// DATA STRUCTURES
// ============================================================================
struct Parcel {
    double weightKg;
    bool express;
    bool insured;
    std::string trackingId;
};

// Alternative Concept: Structure with default/optional fields
struct ParcelOptions {
    double weightKg = 0.0;
    bool express = false;
    bool insured = false;
    bool fragileHandling = false;   // Easily adds new options without breaking call sites
    bool saturdayDelivery = false;  // Zero signature explosion
};

// ============================================================================
// REDESIGNED UNAMBIGUOUS OVERLOAD SET
// ============================================================================

// Rate constants (in INR)
constexpr double RATE_PER_KG = 50.0;
constexpr double RATE_PER_GRAM = 0.06; // Rs 60 per kg for small parcels
constexpr double EXPRESS_SURCHARGE = 120.0;
constexpr double INSURANCE_SURCHARGE = 80.0;

// Overload A: Weight in kilograms (double) with default arguments from right
double charge(double weightKg, bool express = false, bool insured = false) {
    double fee = weightKg * RATE_PER_KG;
    if (express) fee += EXPRESS_SURCHARGE;
    if (insured) fee += INSURANCE_SURCHARGE;
    return fee;
}

// Overload B: Weight in grams (int) with default arguments from right
double charge(int grams, bool express = false, bool insured = false) {
    double fee = grams * RATE_PER_GRAM;
    if (express) fee += EXPRESS_SURCHARGE;
    if (insured) fee += INSURANCE_SURCHARGE;
    return fee;
}

// Overload C: Parcel object passed by const reference (zero copy)
double charge(const Parcel& p) {
    return charge(p.weightKg, p.express, p.insured);
}

// Alternative Concept Implementation: Single Structure
double chargeAlternative(const ParcelOptions& opt) {
    double fee = opt.weightKg * RATE_PER_KG;
    if (opt.express) fee += EXPRESS_SURCHARGE;
    if (opt.insured) fee += INSURANCE_SURCHARGE;
    if (opt.fragileHandling) fee += 50.0;
    if (opt.saturdayDelivery) fee += 100.0;
    return fee;
}

// ============================================================================
// DEMONSTRATION & PREDICTION VERIFICATION
// ============================================================================

void runAmbiguityExplanation() {
    std::cout << "========================================================\n";
    std::cout << "1. COMPILER ERROR & AMBIGUOUS CALL DIAGNOSIS\n";
    std::cout << "========================================================\n";
    std::cout << "Original Flawed Signatures:\n";
    std::cout << "  (1) charge(double weight)\n";
    std::cout << "  (2) charge(double weight, bool express)\n";
    std::cout << "  (3) charge(double weight, bool express = false, bool insured = false)\n\n";
    std::cout << "Why It Failed to Compile:\n";
    std::cout << "  - When calling 'charge(5.0)', signature (1) is an exact match for 1 argument,\n";
    std::cout << "    while signature (3) can also accept 1 argument by defaulting express and insured.\n";
    std::cout << "    Both functions have the exact same parameter type (double) after default substitution.\n";
    std::cout << "  - Similarly, 'charge(5.0, true)' matches both (2) and (3).\n";
    std::cout << "  - ISO C++ Rule: If two or more candidate functions are equally viable after\n";
    std::cout << "    default argument substitution, the call is ambiguous and compilation fails.\n";
    std::cout << "  - Redesign Solution: Eliminated redundant signatures (1) and (2); signature (3)\n";
    std::cout << "    already handles 1, 2, or 3 arguments cleanly!\n";
    std::cout << "========================================================\n\n";
}

void runPredictionsVerification() {
    std::cout << "========================================================\n";
    std::cout << "2. PREDICT-BEFORE-EXECUTION: SIX DESIGNED CALLS\n";
    std::cout << "========================================================\n";

    std::cout << "Call 1: charge(2.5)\n";
    std::cout << "  Prediction: Selects charge(double, bool, bool). Exact match for double literal (2.5 kg).\n";
    std::cout << "              Fee: 2.5 * 50 = Rs. 125.00\n";
    double res1 = charge(2.5);
    std::cout << "  Observed  : Selected charge(double) -> Rs. " << std::fixed << std::setprecision(2) << res1 << '\n';
    std::cout << "  Status    : " << (res1 == 125.0 ? "MATCHED" : "FAILED") << "\n\n";

    std::cout << "Call 2: charge(500)\n";
    std::cout << "  Prediction: Selects charge(int, bool, bool). Exact match for int literal (500 grams).\n";
    std::cout << "              Fee: 500 * 0.06 = Rs. 30.00\n";
    double res2 = charge(500);
    std::cout << "  Observed  : Selected charge(int) -> Rs. " << res2 << '\n';
    std::cout << "  Status    : " << (res2 == 30.0 ? "MATCHED" : "FAILED") << "\n\n";

    std::cout << "Call 3: charge(2)\n";
    std::cout << "  Prediction: Literal '2' has type int. The compiler checks exact match first.\n";
    std::cout << "              Matches charge(int) exactly without conversion.\n";
    std::cout << "              Weight is evaluated as 2 grams -> 2 * 0.06 = Rs. 0.12\n";
    double res3 = charge(2);
    std::cout << "  Observed  : Selected charge(int) -> Rs. " << res3 << '\n';
    std::cout << "  Status    : " << (res3 == 0.12 ? "MATCHED" : "FAILED") << "\n\n";

    std::cout << "Call 4: charge(2.0f)\n";
    std::cout << "  Prediction: Literal '2.0f' has type float.\n";
    std::cout << "              Overload candidates: charge(double) vs charge(int).\n";
    std::cout << "              C++ conversion rank: float -> double is a Floating-Point Promotion/Conversion\n";
    std::cout << "              (Rank 2), whereas float -> int is a Floating-Integral Conversion with truncation.\n";
    std::cout << "              Standard rules prioritize float -> double! Selects charge(double).\n";
    std::cout << "              Fee: 2.0 * 50 = Rs. 100.00\n";
    double res4 = charge(2.0f);
    std::cout << "  Observed  : Selected charge(double) -> Rs. " << res4 << '\n';
    std::cout << "  Status    : " << (res4 == 100.0 ? "MATCHED" : "FAILED") << "\n\n";

    std::cout << "Call 5: charge(3.0, true)\n";
    std::cout << "  Prediction: Selects charge(double, bool, bool=false). Express delivery (+120).\n";
    std::cout << "              Fee: 3.0 * 50 + 120 = Rs. 270.00\n";
    double res5 = charge(3.0, true);
    std::cout << "  Observed  : Selected charge(double, bool) -> Rs. " << res5 << '\n';
    std::cout << "  Status    : " << (res5 == 270.0 ? "MATCHED" : "FAILED") << "\n\n";

    std::cout << "Call 6: charge(Parcel{4.0, true, true, \"TRK-192472008\"})\n";
    std::cout << "  Prediction: Selects charge(const Parcel&). Exact user-defined type match.\n";
    std::cout << "              Fee: 4.0 * 50 + 120 + 80 = Rs. 400.00\n";
    Parcel p{4.0, true, true, "TRK-192472008"};
    double res6 = charge(p);
    std::cout << "  Observed  : Selected charge(Parcel) -> Rs. " << res6 << '\n';
    std::cout << "  Status    : " << (res6 == 400.0 ? "MATCHED" : "FAILED") << '\n';
    std::cout << "========================================================\n\n";
}

void runInquiryExplanation() {
    std::cout << "========================================================\n";
    std::cout << "3. INQUIRY: OVERLOAD RESOLUTION & DEFAULT ARGUMENTS\n";
    std::cout << "========================================================\n";
    std::cout << "A. How Compiler Chooses Overloads (C++ Standard Clause [over.match]):\n";
    std::cout << "   Stage 1: Identify Candidate Functions (name lookup in current scope).\n";
    std::cout << "   Stage 2: Filter Viable Functions (parameter count matches args + default values).\n";
    std::cout << "   Stage 3: Rank Conversions for Each Parameter:\n";
    std::cout << "     1. Exact Match (Identity, const/volatile qualification, array-to-pointer)\n";
    std::cout << "     2. Promotion (Integral promotion: char/short -> int; Floating promotion: float -> double)\n";
    std::cout << "     3. Standard Conversion (int -> double, double -> int, pointer conversions)\n";
    std::cout << "     4. User-Defined Conversion (conversion constructors, conversion operators)\n";
    std::cout << "     5. Ellipsis (...)\n";
    std::cout << "   A candidate is chosen if it is strictly better in at least one parameter\n";
    std::cout << "   and no worse in all others. If two candidates tie, the compiler issues an ambiguity error.\n\n";
    std::cout << "B. Why Default Arguments Must Be Given From the Right:\n";
    std::cout << "   C++ argument binding is strictly positional from left to right.\n";
    std::cout << "   If a signature were 'void f(int a = 1, int b)', a call 'f(5)' would be ambiguous:\n";
    std::cout << "   does '5' bind to 'a' (leaving 'b' unsupplied) or to 'b'?\n";
    std::cout << "   Enforcing defaults from the right ensures that any supplied arguments\n";
    std::cout << "   unambiguously bind to the leading parameters in order.\n";
    std::cout << "========================================================\n\n";
}

void runAlternativeConceptAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "4. ALTERNATIVE CONCEPT & COMPLEXITY ANALYSIS\n";
    std::cout << "========================================================\n";
    std::cout << "Alternative Design: Single ParcelOptions Struct\n";
    ParcelOptions opt1;
    opt1.weightKg = 5.0;
    opt1.express = true;
    opt1.insured = false;

    ParcelOptions opt2;
    opt2.weightKg = 5.0;
    opt2.express = true;
    opt2.insured = true;
    opt2.fragileHandling = true;

    std::cout << "  Charge for 5kg Express: Rs. " << chargeAlternative(opt1) << '\n';
    std::cout << "  Charge for 5kg Express + Insured + Fragile: Rs. " << chargeAlternative(opt2) << '\n';

    std::cout << "\nDetailed Comparison:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Metric                  | Overloaded Functions + Defaults | ParcelOptions Struct       |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Overload Resolution Time| Compile-time (0 runtime cost)   | Compile-time (0 cost)      |\n";
    std::cout << "| Space Copied per Call   | 1-3 registers (8-16 bytes)      | const Ref: 8 bytes (ptr)   |\n";
    std::cout << "| Adding New Feature      | Requires editing all signatures | Simply add field to struct |\n";
    std::cout << "| Code Clutter / Bleed    | Combinatorial overloads risk    | Modular, clean, scalable   |\n";
    std::cout << "| Call Site Readability   | Positional booleans (unclear)   | Named fields (very clear)  |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "DEFENSE:\n";
    std::cout << "  For small, stable parameter sets, overloaded functions with default arguments\n";
    std::cout << "  provide zero-boilerplate syntax. However, when features expand (e.g. fragile,\n";
    std::cout << "  weekend delivery, hazardous materials), the ParcelOptions struct with designated\n";
    std::cout << "  initializers avoids boolean trap errors (e.g., charge(5.0, true, false)) and\n";
    std::cout << "  scales without breaking existing call sites.\n";
    std::cout << "========================================================\n";
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 2 - TASK 03: OVERLOADING & DEFAULT ARGUMENTS \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runAmbiguityExplanation();
    runPredictionsVerification();
    runInquiryExplanation();
    runAlternativeConceptAnalysis();

    return 0;
}
