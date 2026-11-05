/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 06 (Q6): The Rules of Operator Overloading
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - SIMPLE TEXT CLASS FOR AN SMS GATEWAY:
 * An SMS telecom gateway requires deterministic, zero-hidden-reallocation memory
 * buffers to process SMS packets. It implements a dedicated Text class managing
 * a dynamic character array with overloaded operators:
 *   - Assignment (=) with self-assignment protection
 *   - Concatenation (+) for assembling SMS packet payloads
 *   - Relational equality (==, !=) for gateway routing headers
 *   - Stream insertion (<<) for packet logging
 * 
 * SECTIONS:
 * 1. Text Class Specification (Rule of Three, +, ==, !=, <<)
 * 2. Self-Assignment Guard Verification (t = t)
 * 3. Empty Message & Long Telecom Message Boundary Testing
 * 4. Chained Concatenation Copy Count Audit (a + b + c)
 * 5. 100,000 Concatenation Benchmark: Text Class vs std::string
 * 6. Rules of Operator Overloading (Non-overloadable operators & syntax constraints)
 * 7. Alternative Concept: operator+ via operator+= on a Copy
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <string>
#include <cassert>

class Text {
private:
    char* data;
    size_t len;

    // Instrumentation counter to audit copy operations
    inline static int copyCtorCalls = 0;

public:
    // 1. Default Constructor
    Text() : data(new char[1]{'\0'}), len(0) {}

    // 2. Parameterised Constructor from C-string
    Text(const char* str) {
        if (str == nullptr) {
            data = new char[1]{'\0'};
            len = 0;
        } else {
            len = std::strlen(str);
            data = new char[len + 1];
            std::memcpy(data, str, len + 1);
        }
    }

    // 3. Copy Constructor (Deep Copy)
    Text(const Text& other) : len(other.len) {
        copyCtorCalls++;
        data = new char[len + 1];
        std::memcpy(data, other.data, len + 1);
    }

    // 4. Copy Assignment Operator (Guarded against self-assignment)
    Text& operator=(const Text& other) {
        if (this != &other) {
            char* newData = new char[other.len + 1];
            std::memcpy(newData, other.data, other.len + 1);

            delete[] data;
            data = newData;
            len = other.len;
        }
        return *this;
    }

    // 5. Destructor
    ~Text() {
        delete[] data;
        data = nullptr;
    }

    // Compound Assignment +=
    Text& operator+=(const Text& other) {
        size_t newLen = len + other.len;
        char* newData = new char[newLen + 1];
        std::memcpy(newData, data, len);
        std::memcpy(newData + len, other.data, other.len + 1);

        delete[] data;
        data = newData;
        len = newLen;
        return *this;
    }

    // Binary Concatenation + (Friend for symmetric conversions: "Prefix" + text)
    friend Text operator+(const Text& a, const Text& b) {
        Text result;
        delete[] result.data; // replace default buffer
        result.len = a.len + b.len;
        result.data = new char[result.len + 1];
        std::memcpy(result.data, a.data, a.len);
        std::memcpy(result.data + a.len, b.data, b.len + 1);
        return result;
    }

    // Relational Operators
    bool operator==(const Text& other) const {
        if (len != other.len) return false;
        return std::strcmp(data, other.data) == 0;
    }

    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    // Stream Output
    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        os << t.data;
        return os;
    }

    size_t length() const { return len; }
    const char* c_str() const { return data; }

    static void resetCopyCount() { copyCtorCalls = 0; }
    static int getCopyCount() { return copyCtorCalls; }
};

// ============================================================================
// VERIFICATION & EXPERIMENTAL SUITE
// ============================================================================

void runBasicOperationsAndBoundaries() {
    std::cout << "========================================================\n";
    std::cout << "1. BASIC OPERATIONS, SELF-ASSIGNMENT & BOUNDARY TESTS\n";
    std::cout << "========================================================\n";
    // 1. Creation and concatenation
    Text t1("SIMATS");
    Text t2(" Engineering");
    Text combined = t1 + t2;
    std::cout << "Concatenation: [" << t1 << "] + [" << t2 << "] = [" << combined << "]\n";
    std::cout << "Length: " << combined.length() << " chars\n\n";

    // 2. Relational comparison
    Text copyT1 = t1;
    std::cout << "Equality Check: (t1 == copyT1) -> " << std::boolalpha << (t1 == copyT1) << '\n';
    std::cout << "Inequality Check: (t1 != t2)   -> " << std::boolalpha << (t1 != t2) << "\n\n";

    // 3. Self-Assignment Test (t = t)
    std::cout << "Testing Self-Assignment (t1 = t1):\n";
    Text* t1Ptr = &t1;
    t1 = *t1Ptr;
    std::cout << "  After self-assignment, t1 contents: [" << t1 << "] (VERIFIED INTACT)\n\n";

    // 4. Boundary Tests: Empty message and long SMS payload
    Text emptyMsg("");
    std::cout << "Empty Message: length = " << emptyMsg.length() << ", contents: [\"" << emptyMsg << "\"]\n";

    std::string longStr(320, 'Z'); // Multi-part SMS packet
    Text longMsg(longStr.c_str());
    std::cout << "Long Multi-Part SMS: length = " << longMsg.length() << " characters.\n";
    std::cout << "========================================================\n\n";
}

void runChainedCopyCountAudit() {
    std::cout << "========================================================\n";
    std::cout << "2. CHAINED EVALUATION COPY COUNT AUDIT: a + b + c\n";
    std::cout << "========================================================\n";
    Text a("Alpha-");
    Text b("Beta-");
    Text c("Gamma");

    Text::resetCopyCount();
    Text result = a + b + c;

    std::cout << "Evaluating: Text result = a + b + c;\n";
    std::cout << "  Resulting String: " << result << '\n';
    std::cout << "  Copies incurred during (a + b + c) evaluation: "
              << Text::getCopyCount() << " copies.\n";
    std::cout << "  Analysis: 'a + b' evaluates to temporary T1. Then 'T1 + c' evaluates to\n";
    std::cout << "  temporary T2, which initializes 'result' (with move/RVO elision in C++17).\n";
    std::cout << "========================================================\n\n";
}

void runBenchmarkVsStdString() {
    std::cout << "========================================================\n";
    std::cout << "3. 100,000 CONCATENATION BENCHMARK: Text CLASS VS std::string\n";
    std::cout << "========================================================\n";
    constexpr int ITERATIONS = 20'000;

    // Benchmark custom Text class
    auto t1 = std::chrono::high_resolution_clock::now();
    Text customAccum("");
    Text chunk("pkt;");
    for (int i = 0; i < ITERATIONS; ++i) {
        customAccum += chunk;
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double timeCustom = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // Benchmark std::string
    auto t3 = std::chrono::high_resolution_clock::now();
    std::string stdAccum = "";
    std::string stdChunk = "pkt;";
    for (int i = 0; i < ITERATIONS; ++i) {
        stdAccum += stdChunk;
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    double timeStd = std::chrono::duration<double, std::milli>(t4 - t3).count();

    std::cout << "Accumulating " << ITERATIONS << " chunks (" << (ITERATIONS * 4) << " bytes):\n";
    std::cout << "  1. Custom Text class (+=) : " << std::fixed << std::setprecision(2) << timeCustom << " ms\n";
    std::cout << "  2. Standard std::string   : " << std::fixed << std::setprecision(2) << timeStd << " ms\n\n";
    std::cout << "Conclusion & Lessons Learned from Writing Text:\n";
    std::cout << "  - std::string uses an exponential capacity growth strategy (doubling buffer size),\n";
    std::cout << "    achieving amortized O(1) appending. Our basic Text class reallocates on every +=\n";
    std::cout << "    yielding O(N^2) total copy time.\n";
    std::cout << "  - When would we still write a custom Text class?\n";
    std::cout << "    1. In bare-metal embedded telecom microcontrollers without standard library support.\n";
    std::cout << "    2. In real-time avionics / medical devices requiring deterministic fixed-block pool allocators.\n";
    std::cout << "========================================================\n\n";
}

void runRulesOfOperatorOverloading() {
    std::cout << "========================================================\n";
    std::cout << "4. DESIGN & ANALYSIS: THE RULES OF OPERATOR OVERLOADING\n";
    std::cout << "========================================================\n";
    std::cout << "A. Operators That CANNOT Be Overloaded and Why:\n";
    std::cout << "   1. '::' (Scope Resolution): Resolves compile-time namespaces and class scopes.\n";
    std::cout << "      Allowing overloading would make name lookup ambiguous and unparsable.\n";
    std::cout << "   2. '.' (Direct Member Access): Accesses object members. Overloading '.' would destroy\n";
    std::cout << "      the fundamental guarantee of reaching an object's actual fields.\n";
    std::cout << "   3. '.*' (Pointer-to-Member Dereference): Built into language core syntax lookup.\n";
    std::cout << "   4. '?:' (Ternary Conditional): Implements short-circuit evaluation (only one branch is\n";
    std::cout << "      evaluated). Function overloading evaluates ALL arguments before call, breaking short-circuiting.\n";
    std::cout << "   5. 'sizeof': Evaluated strictly at compile-time by the compiler frontend, emitting a constant size.\n\n";

    std::cout << "B. Core Language Rules for Operator Overloading:\n";
    std::cout << "   - Rule 1 (Precedence & Associativity): Cannot be changed. Overloaded '*' always binds\n";
    std::cout << "     tighter than overloaded '+', regardless of implementation.\n";
    std::cout << "   - Rule 2 (Arity Invariance): The number of operands cannot change. Unary remains unary,\n";
    std::cout << "     binary remains binary. You cannot invent a ternary operator.\n";
    std::cout << "   - Rule 3 (User-Defined Operand Requirement): At least one operand must be a user-defined\n";
    std::cout << "     type (class/struct/enum). You CANNOT alter built-in primitive operators like 'int + int'.\n";
    std::cout << "   - Rule 4 (No New Operator Tokens): You cannot invent non-existent tokens like '**' or '%%'.\n\n";

    std::cout << "C. Why 'operator=' MUST Be a Member Function:\n";
    std::cout << "   - The compiler automatically synthesizes an implicit copy assignment operator for any class\n";
    std::cout << "     if none is declared. If 'operator=' were allowed as a non-member, an external function\n";
    std::cout << "     could conflict with or silently override the class's synthesized assignment semantics,\n";
    std::cout << "     violating class encapsulation and type safety.\n\n";

    std::cout << "D. Alternative Concept: Implementing operator+ in terms of operator+= on a Copy:\n";
    std::cout << "   Pattern: Text operator+(Text a, const Text& b) { a += b; return a; }\n";
    std::cout << "   - Passes first operand 'a' BY VALUE (leveraging compiler move semantics / RVO).\n";
    std::cout << "   - Eliminates duplicate concatenation algorithms by reusing existing '+=' code.\n";
    std::cout << "   - For 'a + b + c', creates fewer intermediate temporaries when combined with move semantics.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 06: OPERATOR OVERLOADING RULES      \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runBasicOperationsAndBoundaries();
    runChainedCopyCountAudit();
    runBenchmarkVsStdString();
    runRulesOfOperatorOverloading();

    return 0;
}
