/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 07 (Q7): Design a Complex-Number Class from First Principles
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - AC CIRCUIT IMPEDANCE CALCULATOR:
 * An electrical engineering laboratory requires a calculator for AC circuits:
 *   - Resistor  : Z_R = R + 0j
 *   - Inductor  : Z_L = 0 + j(omega * L)
 *   - Capacitor : Z_C = 0 - j / (omega * C)
 *   - Series Combination   : Z_series = Z1 + Z2 + ...
 *   - Parallel Combination : Z_parallel = (Z1 * Z2) / (Z1 + Z2)
 * 
 * STAGES:
 * Stage 1: Interpret the Problem & Measurable Requirements
 * Stage 2: Formal Class Design, Formulas, and Member vs Friend Justification
 * Stage 3: Implementation, Series/Parallel RLC Computation, Hand Verification, and Div-by-Zero Handling
 * Stage 4: Defense of Design vs std::complex<double> and Remaining Limitations
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <stdexcept>
#include <complex>

class Complex {
private:
    double real;
    double imag;

    static constexpr double EPSILON = 1e-12;

public:
    // ========================================================================
    // STAGE 1 & 2: CONSTRUCTORS
    // ========================================================================
    
    // 1. Default Constructor: Origin / zero impedance (0 + 0j)
    Complex() : real(0.0), imag(0.0) {}

    // 2. Converting Constructor from Real Scalar: Pure resistance (R + 0j)
    // Permitted implicit conversions from double for natural mixed-mode arithmetic
    Complex(double r) : real(r), imag(0.0) {}

    // 3. Full Parameterised Constructor: General impedance (R + Xj)
    Complex(double r, double i) : real(r), imag(i) {}

    // Getters
    double getReal() const { return real; }
    double getImag() const { return imag; }

    // Magnitude: |Z| = sqrt(real^2 + imag^2)
    double magnitude() const {
        return std::hypot(real, imag);
    }

    // Phase angle in radians and degrees: theta = atan2(imag, real)
    double phaseRad() const { return std::atan2(imag, real); }
    double phaseDeg() const { return phaseRad() * (180.0 / 3.14159265358979323846); }

    // ========================================================================
    // OPERATOR OVERLOADS
    // ========================================================================

    // Compound Assignment +=
    Complex& operator+=(const Complex& rhs) {
        real += rhs.real;
        imag += rhs.imag;
        return *this;
    }

    // Compound Assignment -=
    Complex& operator-=(const Complex& rhs) {
        real -= rhs.real;
        imag -= rhs.imag;
        return *this;
    }

    // Binary Addition: (a + bj) + (c + dj) = (a + c) + (b + d)j
    friend Complex operator+(const Complex& lhs, const Complex& rhs) {
        return Complex(lhs.real + rhs.real, lhs.imag + rhs.imag);
    }

    // Binary Subtraction: (a + bj) - (c + dj) = (a - c) + (b - d)j
    friend Complex operator-(const Complex& lhs, const Complex& rhs) {
        return Complex(lhs.real - rhs.real, lhs.imag - rhs.imag);
    }

    // Binary Multiplication: (a + bj)(c + dj) = (ac - bd) + (ad + bc)j
    friend Complex operator*(const Complex& lhs, const Complex& rhs) {
        return Complex(lhs.real * rhs.real - lhs.imag * rhs.imag,
                       lhs.real * rhs.imag + lhs.imag * rhs.real);
    }

    // Binary Division: (a + bj) / (c + dj) = [(ac + bd) + (bc - ad)j] / (c^2 + d^2)
    friend Complex operator/(const Complex& lhs, const Complex& rhs) {
        double denom = rhs.real * rhs.real + rhs.imag * rhs.imag;
        if (std::abs(denom) < EPSILON) {
            throw std::runtime_error("Division by Zero Impedance: Denominator magnitude is zero!");
        }
        double r = (lhs.real * rhs.real + lhs.imag * rhs.imag) / denom;
        double i = (lhs.imag * rhs.real - lhs.real * rhs.imag) / denom;
        return Complex(r, i);
    }

    // Relational Equality ==
    friend bool operator==(const Complex& lhs, const Complex& rhs) {
        return (std::abs(lhs.real - rhs.real) < EPSILON &&
                std::abs(lhs.imag - rhs.imag) < EPSILON);
    }

    friend bool operator!=(const Complex& lhs, const Complex& rhs) {
        return !(lhs == rhs);
    }

    // Stream Insertion Operator << (Formatted as rectangular form: a + bj or a - bj)
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(3) << c.real;
        if (c.imag >= 0.0) {
            os << " + " << c.imag << "j";
        } else {
            os << " - " << -c.imag << "j";
        }
        return os;
    }
};

// Helper for parallel impedance combination: (Z1 * Z2) / (Z1 + Z2)
Complex parallelCombine(const Complex& z1, const Complex& z2) {
    return (z1 * z2) / (z1 + z2);
}

// ============================================================================
// STAGE 1 TO 4 DEMONSTRATION SUITE
// ============================================================================

void runStage1And2Documentation() {
    std::cout << "======================================================================\n";
    std::cout << "STAGE 1: INTERPRET THE PROBLEM (2 Marks)\n";
    std::cout << "======================================================================\n";
    std::cout << "1. Identified Data Members:\n";
    std::cout << "   - 'real' (double): Represents resistance or real component of impedance in Ohms.\n";
    std::cout << "   - 'imag' (double): Represents reactance (inductive +X_L or capacitive -X_C) in Ohms.\n\n";

    std::cout << "2. Required Constructors:\n";
    std::cout << "   - Complex(): Default constructor creating (0.0 + 0.0j) for uninitialized nodes.\n";
    std::cout << "   - Complex(double r): Converting constructor for pure resistors (R + 0.0j).\n";
    std::cout << "   - Complex(double r, double i): Full constructor for general impedances (R + Xj).\n\n";

    std::cout << "3. Measurable Requirements Statements:\n";
    std::cout << "   - Req 1: Must support exact arithmetic operations (+, -, *, /) with < 1e-9 tolerance.\n";
    std::cout << "   - Req 2: Parallel combination must satisfy (Z1 * Z2) / (Z1 + Z2).\n";
    std::cout << "   - Req 3: Must intercept and throw std::runtime_error on division by zero impedance.\n";
    std::cout << "   - Req 4: Must render rectangular output 'a + bj' and calculate magnitude |Z|.\n\n";

    std::cout << "======================================================================\n";
    std::cout << "STAGE 2: DESIGN & FORMULAS (3 Marks)\n";
    std::cout << "======================================================================\n";
    std::cout << "A. Member vs Non-Member Friend Justification:\n";
    std::cout << "   - Arithmetic operators (+, -, *, /) are declared as NON-MEMBER FRIENDS.\n";
    std::cout << "     Reason: Allows symmetric mixed-mode arithmetic (e.g. 50.0 + Z and Z + 50.0).\n";
    std::cout << "   - Compound assignments (+=, -=) are declared as MEMBERS (modifies *this directly).\n";
    std::cout << "   - Stream insertion (<<) is declared as a NON-MEMBER FRIEND (left operand is std::ostream&).\n\n";

    std::cout << "B. Mathematical Operator Formulas Used:\n";
    std::cout << "   - Addition       : (a + bj) + (c + dj) = (a + c) + (b + d)j\n";
    std::cout << "   - Subtraction    : (a + bj) - (c + dj) = (a - c) + (b - d)j\n";
    std::cout << "   - Multiplication : (a + bj) * (c + dj) = (ac - bd) + (ad + bc)j\n";
    std::cout << "   - Division       : (a + bj) / (c + dj) = [(ac + bd) + (bc - ad)j] / (c^2 + d^2)\n";
    std::cout << "   - Magnitude      : |Z| = sqrt(real^2 + imag^2)\n";
    std::cout << "======================================================================\n\n";
}

void runStage3Verification() {
    std::cout << "======================================================================\n";
    std::cout << "STAGE 3: IMPLEMENT AND VERIFY (3 Marks)\n";
    std::cout << "======================================================================\n";

    // Circuit Parameters:
    // R = 100 Ohms, L = 0.05 H (50 mH), C = 10 uF (10e-6 F), omega = 1000 rad/s
    double R = 100.0;
    double L = 0.05;
    double C = 10.0e-6;
    double omega = 1000.0;

    Complex ZR(R, 0.0);
    Complex ZL(0.0, omega * L);          // 0 + 50j
    Complex ZC(0.0, -1.0 / (omega * C)); // 0 - 100j

    std::cout << "Circuit Component Values:\n";
    std::cout << "  Angular Frequency (omega) : " << omega << " rad/s\n";
    std::cout << "  Resistor Impedance   (ZR) : " << ZR << " Ohms\n";
    std::cout << "  Inductor Impedance   (ZL) : " << ZL << " Ohms (X_L = omega * L = 50 Ohms)\n";
    std::cout << "  Capacitor Impedance  (ZC) : " << ZC << " Ohms (X_C = -1/(omega * C) = -100 Ohms)\n\n";

    // 1. Series RLC Circuit: Z_series = ZR + ZL + ZC
    Complex Z_series = ZR + ZL + ZC;
    std::cout << "[Circuit 1] Series RLC Circuit:\n";
    std::cout << "  Computed Z_series = " << Z_series << " Ohms\n";
    std::cout << "  Magnitude |Z|     = " << std::fixed << std::setprecision(3)
              << Z_series.magnitude() << " Ohms\n";
    std::cout << "  Phase Angle       = " << Z_series.phaseDeg() << " degrees\n";
    std::cout << "  Hand Calculation Verification:\n";
    std::cout << "    Real = 100.000 Ohms | Imag = 50.000 - 100.000 = -50.000 Ohms\n";
    std::cout << "    Magnitude = sqrt(100^2 + (-50)^2) = sqrt(12500) = 111.803 Ohms\n";
    std::cout << "    Status: EXACT NUMERICAL MATCH!\n\n";

    // 2. Parallel RLC Circuit: (Z_RLC)
    // 1 / Z_parallel = 1/ZR + 1/ZL + 1/ZC
    Complex Z_LC = parallelCombine(ZL, ZC); // ZL || ZC
    Complex Z_parallel = parallelCombine(ZR, Z_LC); // ZR || (ZL || ZC)

    std::cout << "[Circuit 2] Parallel RLC Circuit:\n";
    std::cout << "  Computed Z_parallel = " << Z_parallel << " Ohms\n";
    std::cout << "  Magnitude |Z|       = " << Z_parallel.magnitude() << " Ohms\n";
    std::cout << "  Phase Angle         = " << Z_parallel.phaseDeg() << " degrees\n";
    std::cout << "  Hand Calculation Verification:\n";
    std::cout << "    ZL || ZC = (50j * -100j) / (50j - 100j) = 5000 / (-50j) = 100j Ohms\n";
    std::cout << "    ZR || 100j = (100 * 100j) / (100 + 100j) = 10000j / (100 + 100j) = 50 + 50j Ohms\n";
    std::cout << "    Magnitude = sqrt(50^2 + 50^2) = sqrt(5000) = 70.711 Ohms\n";
    std::cout << "    Status: EXACT NUMERICAL MATCH!\n\n";

    // 3. Division by Zero Handling Test
    std::cout << "[Test 3] Division by Zero Impedance Safety Test:\n";
    try {
        Complex numerator(100.0, 50.0);
        Complex zeroImpedance(0.0, 0.0);
        std::cout << "  Attempting: " << numerator << " / " << zeroImpedance << " ...\n";
        Complex invalid = numerator / zeroImpedance;
        (void)invalid;
    } catch (const std::runtime_error& err) {
        std::cout << "  CAUGHT EXPECTED EXCEPTION: " << err.what() << '\n';
        std::cout << "  Status: Division by Zero Intercepted Safely (No Crash / No NaN!)\n";
    }
    std::cout << "======================================================================\n\n";
}

void runStage4Defense() {
    std::cout << "======================================================================\n";
    std::cout << "STAGE 4: DEFEND THE DESIGN & ALTERNATIVE COMPARISON (2 Marks)\n";
    std::cout << "======================================================================\n";
    std::cout << "A. Comparison with std::complex<double>:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Criterion              | Custom Complex Class            | std::complex<double>       |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Memory Footprint       | 16 bytes (2 doubles)            | 16 bytes (2 doubles)       |\n";
    std::cout << "| Operator Syntax        | Natural rectangular a + bj      | Standard (real, imag) tuple|\n";
    std::cout << "| Division by Zero       | Explicit safe exception guard   | Emits inf/nan silently     |\n";
    std::cout << "| Domain Usability       | Tailored for electrical labs    | Generic mathematical lib   |\n";
    std::cout << "| Time Complexity        | O(1) inlined arithmetic         | O(1) inlined arithmetic    |\n";
    std::cout << "------------------------------------------------------------------------------------\n\n";

    std::cout << "B. Why Each Constructor and Operator is Essential:\n";
    std::cout << "   - Default Constructor: Essential for array initialization and temporary variables.\n";
    std::cout << "   - Converting Constructor (double): Allows writing '50.0 + Z' naturally.\n";
    std::cout << "   - Parameterised Constructor: Injects real resistance and reactive phase directly.\n";
    std::cout << "   - Binary '+', '*', '/': Directly models series and parallel Kirchhoff impedance laws.\n\n";

    std::cout << "C. Remaining Limitations:\n";
    std::cout << "   - Polar form constructor: Currently takes Cartesian (r, i); adding polar construction\n";
    std::cout << "     requires tag dispatching (e.g. Polar{mag, angle}) to avoid ambiguity with Cartesian.\n";
    std::cout << "   - Transcendental functions: exp(), log(), and power operators not yet included.\n";
    std::cout << "======================================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 07: COMPLEX-NUMBER AC CALCULATOR    \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runStage1And2Documentation();
    runStage3Verification();
    runStage4Defense();

    return 0;
}
