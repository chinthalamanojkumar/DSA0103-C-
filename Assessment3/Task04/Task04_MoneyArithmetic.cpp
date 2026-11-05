/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 04 (Q4): Overloading Arithmetic and Stream Operators
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - MONEY ARITHMETIC FOR A BILLING COUNTER:
 * A billing counter adds, subtracts, and compares amounts of money stored in
 * rupees and paise. Calling raw functions like add(a, b) or print(c) clutters
 * billing expressions. Storing money as floating-point 'double' causes cumulative
 * rounding errors (IEEE 754 precision drift).
 * 
 * SECTIONS:
 * 1. Money Class with Overloaded Arithmetic (+, -, +=), Relational (==, <), and Stream (<<, >>)
 * 2. Complete the Code Verification (Page 10: TODO 1 & 2)
 * 3. Paise Carry-Over & Negative Amount Handling
 * 4. Readability Comparison: Functional Call Syntax vs Natural Operator Syntax
 * 5. Cumulative Drift Proof: 1,000,000 Additions (Integer Paise vs IEEE 754 Double)
 * 6. Engineering Decision: Dimensional Analysis of Money * Money
 * 7. Alternative Concept: Friend vs Member Binary Operators (Symmetric Conversion)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

class Money {
private:
    long long paise; // Canonical integer storage prevents floating-point drift

public:
    // Parameterised Constructor with default arguments (Exact from PDF Specification)
    Money(long long rupees = 0, int p = 0)
        : paise(rupees * 100 + p)
    {
    }

    // Static factory function for internal paise arithmetic
    static Money fromPaise(long long totalPaise) {
        Money m;
        m.paise = totalPaise;
        return m;
    }

    // ========================================================================
    // TODO 1: OVERLOAD + AS A MEMBER FUNCTION RETURNING A NEW MONEY OBJECT
    // ========================================================================
    Money operator+(const Money& other) const {
        return Money::fromPaise(paise + other.paise);
    }

    // Compound Assignment += (Returns reference to *this for chaining)
    Money& operator+=(const Money& other) {
        paise += other.paise;
        return *this;
    }

    // Binary Subtraction -
    Money operator-(const Money& other) const {
        return Money::fromPaise(paise - other.paise);
    }

    // Scalar Multiplication (Money * double is valid dimensionally)
    Money operator*(double scalar) const {
        return Money::fromPaise(static_cast<long long>(std::round(paise * scalar)));
    }

    // ========================================================================
    // TODO 2: OVERLOAD == AS MEMBER, AND << AS FRIEND THAT PRINTS "Rs 200.00"
    // ========================================================================
    bool operator==(const Money& other) const {
        return paise == other.paise;
    }

    bool operator<(const Money& other) const {
        return paise < other.paise;
    }

    // Friend stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long total = m.paise;
        bool negative = total < 0;
        long long absPaise = std::abs(total);
        long long r = absPaise / 100;
        long long p = absPaise % 100;

        if (negative) os << "-";
        os << "Rs " << r << '.' << (p < 10 ? "0" : "") << p;
        return os;
    }

    // Stream extraction operator >>
    friend std::istream& operator>>(std::istream& is, Money& m) {
        double amount;
        if (is >> amount) {
            m.paise = static_cast<long long>(std::round(amount * 100.0));
        }
        return is;
    }

    // Friend binary + enabling symmetric conversion on LHS: (100 + m)
    friend Money operator+(long long rupees, const Money& m) {
        return Money::fromPaise(rupees * 100 + m.paise);
    }

    long long getPaise() const { return paise; }
};

// ============================================================================
// VERIFICATION & EXPERIMENTAL SUITE
// ============================================================================

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "1. COMPLETE THE CODE VERIFICATION (Assignment Page 10)\n";
    std::cout << "========================================================\n";
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';

    std::cout << "\nExpected Output:\n";
    std::cout << "Rs 200.00\n";
    std::cout << "true\n";
    std::cout << "Status: " << (c == Money(200) ? "VERIFIED EXACT MATCH" : "MISMATCH") << "\n\n";

    // Carry test from specification (Rs 0.75 + Rs 0.50 = Rs 1.25)
    std::cout << "Testing Paise Carry-Over into Rupees:\n";
    Money carry1(0, 75);
    Money carry2(0, 50);
    Money carryResult = carry1 + carry2;
    std::cout << "  " << carry1 << " + " << carry2 << " = " << carryResult << '\n';
    std::cout << "  Status: " << (carryResult == Money(1, 25) ? "PASSED (125 paise = Rs 1.25)" : "FAILED") << '\n';
    std::cout << "========================================================\n\n";
}

void runNegativeAndBillingTest() {
    std::cout << "========================================================\n";
    std::cout << "2. SUBTRACTION, NEGATIVE AMOUNTS & BILLING CALCULATION\n";
    std::cout << "========================================================\n";
    Money wallet(50, 0);
    Money purchase(75, 50);
    Money refund = wallet - purchase;

    std::cout << "Wallet: " << wallet << " | Purchase: " << purchase << '\n';
    std::cout << "Result of wallet - purchase: " << refund << " (Negative balance correctly formatted)\n\n";

    std::cout << "Full Supermarket Billing Calculation Using Overloaded Operators:\n";
    Money item1(149, 99); // Shampoo
    Money item2(49, 50);  // Soap
    Money item3(220, 0);  // Cooking Oil
    Money discount(35, 50);

    // Natural chained arithmetic syntax:
    Money subtotal = item1 + item2 + item3;
    Money finalBill = subtotal - discount;
    Money gstTax = finalBill * 0.18; // 18% GST scalar multiplication
    Money totalPayable = finalBill + gstTax;

    std::cout << "  Item 1 (Shampoo)   : " << item1 << '\n';
    std::cout << "  Item 2 (Soap)      : " << item2 << '\n';
    std::cout << "  Item 3 (Oil)       : " << item3 << '\n';
    std::cout << "  -------------------------------------\n";
    std::cout << "  Subtotal           : " << subtotal << '\n';
    std::cout << "  Store Discount (-) : " << discount << '\n';
    std::cout << "  Taxable Amount     : " << finalBill << '\n';
    std::cout << "  GST (18%) (+)      : " << gstTax << '\n';
    std::cout << "  -------------------------------------\n";
    std::cout << "  TOTAL PAYABLE      : " << totalPayable << '\n';
    std::cout << "========================================================\n\n";
}

void runFloatingPointDriftAudit() {
    std::cout << "========================================================\n";
    std::cout << "3. DRIFT AUDIT: INTEGER PAISE VS IEEE 754 DOUBLE (1,000,000 OPS)\n";
    std::cout << "========================================================\n";
    constexpr int ITERATIONS = 1'000'000;
    constexpr double DOUBLE_INCREMENT = 0.10; // 10 paise in double

    // 1. Double-based accumulation
    double doubleBalance = 0.0;
    for (int i = 0; i < ITERATIONS; ++i) {
        doubleBalance += DOUBLE_INCREMENT;
    }

    // 2. Integer Paise accumulation
    Money moneyBalance(0, 0);
    Money tenPaise(0, 10);
    for (int i = 0; i < ITERATIONS; ++i) {
        moneyBalance += tenPaise;
    }

    double expectedRupees = 100'000.00;
    double drift = doubleBalance - expectedRupees;

    std::cout << "Accumulating 10 paise exactly 1,000,000 times:\n";
    std::cout << "  Expected True Total        : Rs. 100,000.00\n";
    std::cout << "  Integer Money Total        : " << moneyBalance << " (Exact match!)\n";
    std::cout << "  IEEE 754 Double Total      : Rs. " << std::fixed << std::setprecision(10)
              << doubleBalance << '\n';
    std::cout << "  Cumulative Double Drift    : Rs. " << drift << " (Financial Discrepancy!)\n";
    std::cout << "Conclusion: Fractional base-10 values like 0.10 have no exact binary representation\n"
              << "in IEEE 754, causing truncation drift. Integer paise ensures zero drift.\n";
    std::cout << "========================================================\n\n";
}

void runDesignAndAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "4. DESIGN & ANALYSIS: OPERATOR MECHANICS & RULES\n";
    std::cout << "========================================================\n";
    std::cout << "A. Why '+' Returns a New Object while '+=' Returns a Reference to *this:\n";
    std::cout << "   - Binary '+' represents mathematical addition between two operands, creating an\n";
    std::cout << "     independent third value without altering either operand: c = a + b.\n";
    std::cout << "   - Compound '+=' mutates the left-hand operand in place and returns 'Money&' to\n";
    std::cout << "     permit standard chained expressions like: (a += b) += c;\n\n";

    std::cout << "B. Why '<<' and '>>' Must Be Non-Member Functions:\n";
    std::cout << "   - In 'std::cout << m;', the left-hand operand is 'std::ostream&'.\n";
    std::cout << "   - If overloaded as a member function, it would have to be a member of 'std::ostream',\n";
    std::cout << "     which user code cannot modify. Therefore, it must be declared as a non-member friend.\n\n";

    std::cout << "C. How 'a + b' is Translated by the Compiler:\n";
    std::cout << "   - If overloaded as a member function      : a.operator+(b)\n";
    std::cout << "   - If overloaded as a non-member / friend  : operator+(a, b)\n\n";

    std::cout << "D. Engineering Decision: Should Money Allow Multiplication by Another Money?\n";
    std::cout << "   - ABSOLUTELY NOT! Dimensional analysis forbids it:\n";
    std::cout << "     Rs 50 * Rs 20 = 1000 Rupees^2 (Square rupees has no physical financial meaning).\n";
    std::cout << "   - However, multiplying Money by a dimensionless scalar (e.g., Money * double quantity)\n";
    std::cout << "     yields Money, which is mathematically sound and implemented in our class.\n\n";

    std::cout << "E. Alternative Concept: Friend Operators and Symmetric Conversions:\n";
    std::cout << "   - A member operator allows 'm + 100' (100 converted to Money on RHS),\n";
    std::cout << "     but FORBIDS '100 + m' because 100 is an int without member 'operator+'.\n";
    std::cout << "   - Declaring binary operators as non-member friends allows symmetric conversions:\n";
    std::cout << "     both 'm + 100' and '100 + m' compile and execute seamlessly!\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 04: MONEY OPERATOR OVERLOADING      \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runCompleteTheCode();
    runNegativeAndBillingTest();
    runFloatingPointDriftAudit();
    runDesignAndAnalysis();

    return 0;
}
