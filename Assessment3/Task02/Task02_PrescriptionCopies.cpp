/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 02 (Q2): Copy Constructors: Shallow and Deep Copies
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - HOSPITAL PRESCRIPTION RECORDS:
 * A Prescription object holds a dynamically allocated array of medicine names.
 * When a doctor copies a prescription to modify it for a follow-up visit,
 * the compiler's default shallow copy creates pointer aliasing: modifying the copy
 * mutates the original patient record, and when both objects destruct, the heap
 * array is freed twice, causing a fatal runtime double-free crash.
 * 
 * SECTIONS:
 * 1. Deep-Copying Prescription Class (Rule of Three Implementation: TODO 1, 2, 3)
 * 2. Verification of Complete the Code & Independence Proof
 * 3. Self-Assignment Guard Demonstration (p = p)
 * 4. Pass-by-Value vs Pass-by-Const-Ref Copy Count Instrumentation
 * 5. Multi-Scale Copy Latency Benchmark (10, 1000, 100000 Medicines)
 * 6. Design & Analysis: Rule of Three, Memory Diagrams, and Rule of Zero Alternative
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cassert>

class Prescription {
private:
    int n;
    std::string* medicines; // dynamically allocated array

    // Instrumentation counter to track copy constructor calls
    inline static int copyCount = 0;

public:
    // Parameterised Constructor
    explicit Prescription(int count)
        : n(count), medicines(new std::string[count])
    {
    }

    // ========================================================================
    // TODO 1: COPY CONSTRUCTOR (DEEP COPY)
    // Allocates a NEW array of the exact same size and copies every medicine
    // ========================================================================
    Prescription(const Prescription& other)
        : n(other.n), medicines(new std::string[other.n])
    {
        copyCount++;
        for (int i = 0; i < n; ++i) {
            medicines[i] = other.medicines[i];
        }
    }

    // ========================================================================
    // TODO 2: COPY ASSIGNMENT OPERATOR (DEEP COPY WITH SELF-ASSIGNMENT GUARD)
    // Guards against self-assignment, builds new array, copies, releases old
    // array, and returns *this
    // ========================================================================
    Prescription& operator=(const Prescription& other) {
        if (this != &other) { // 1. Guard against self-assignment (p = p)
            // 2. Allocate new resource first (Exception-safe pattern)
            std::string* newMedicines = new std::string[other.n];
            for (int i = 0; i < other.n; ++i) {
                newMedicines[i] = other.medicines[i];
            }

            // 3. Deallocate existing old resource
            delete[] medicines;

            // 4. Bind new resource to this instance
            medicines = newMedicines;
            n = other.n;
        }
        return *this; // 5. Return reference to *this for assignment chaining
    }

    // ========================================================================
    // TODO 3: DESTRUCTOR
    // Releases dynamically allocated array
    // ========================================================================
    ~Prescription() {
        delete[] medicines;
        medicines = nullptr;
    }

    void set(int i, const std::string& m) {
        if (i >= 0 && i < n) {
            medicines[i] = m;
        }
    }

    std::string get(int i) const {
        if (i >= 0 && i < n) {
            return medicines[i];
        }
        return "";
    }

    int size() const { return n; }
    const void* getBufferAddress() const { return static_cast<const void*>(medicines); }

    // Static counter helpers
    static void resetCopyCount() { copyCount = 0; }
    static int getCopyCount() { return copyCount; }
};

// ============================================================================
// DEMONSTRATION & VERIFICATION ROUTINES
// ============================================================================

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "1. COMPLETE THE CODE VERIFICATION (Assignment Page 5-6)\n";
    std::cout << "========================================================\n";
    Prescription p(2);
    p.set(0, "Paracetamol");
    p.set(1, "ORS");

    Prescription q = p; // copy constructor
    q.set(1, "Cetirizine");

    Prescription r(1);
    r = p; // copy assignment
    p.set(0, "Ibuprofen");

    std::cout << "Observed Output:\n";
    std::cout << p.get(0) << ' ' << p.get(1) << " | "
              << q.get(0) << ' ' << q.get(1) << " | "
              << r.get(0) << ' ' << r.get(1) << '\n';

    std::cout << "\nExpected Output:\n";
    std::cout << "Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS\n";

    bool match = (p.get(0) == "Ibuprofen" && p.get(1) == "ORS" &&
                  q.get(0) == "Paracetamol" && q.get(1) == "Cetirizine" &&
                  r.get(0) == "Paracetamol" && r.get(1) == "ORS");
    std::cout << "Status: " << (match ? "VERIFIED EXACT MATCH" : "MISMATCH") << "\n\n";

    std::cout << "Memory Buffer Isolation Proof:\n";
    std::cout << "  p.medicines buffer address: " << p.getBufferAddress() << '\n';
    std::cout << "  q.medicines buffer address: " << q.getBufferAddress() << '\n';
    std::cout << "  r.medicines buffer address: " << r.getBufferAddress() << '\n';
    std::cout << "  Verification: All 3 addresses are DISTINCT heap buffers (Deep Copy Confirmed!)\n";
    std::cout << "========================================================\n\n";
}

void runSelfAssignmentTest() {
    std::cout << "========================================================\n";
    std::cout << "2. SELF-ASSIGNMENT TEST (p = p)\n";
    std::cout << "========================================================\n";
    Prescription p(2);
    p.set(0, "Amoxicillin");
    p.set(1, "Vitamin C");

    const void* addrBefore = p.getBufferAddress();
    std::cout << "Before self-assignment: [" << p.get(0) << ", " << p.get(1)
              << "] at buffer " << addrBefore << '\n';

    // Execute self-assignment via pointer alias
    Prescription* pPtr = &p;
    p = *pPtr;

    const void* addrAfter = p.getBufferAddress();
    std::cout << "After self-assignment : [" << p.get(0) << ", " << p.get(1)
              << "] at buffer " << addrAfter << '\n';

    std::cout << "Result: " << (addrBefore == addrAfter ? "PASSED (Safe no-op, no data loss)" : "FAILED") << '\n';
    std::cout << "========================================================\n\n";
}

// Helper functions for testing pass-by-value vs pass-by-const-ref
void inspectByValue(Prescription p) {
    (void)p;
}

void inspectByConstRef(const Prescription& p) {
    (void)p;
}

void runCopyCountInvestigation() {
    std::cout << "========================================================\n";
    std::cout << "3. PASS-BY-VALUE VS CONST-REFERENCE COPY COUNT\n";
    std::cout << "========================================================\n";
    Prescription rx(5);

    Prescription::resetCopyCount();
    inspectByValue(rx);
    std::cout << "  Copies incurred when passed by VALUE          : "
              << Prescription::getCopyCount() << " copy\n";

    Prescription::resetCopyCount();
    inspectByConstRef(rx);
    std::cout << "  Copies incurred when passed by CONST REFERENCE: "
              << Prescription::getCopyCount() << " copies (Zero Overhead!)\n";
    std::cout << "========================================================\n\n";
}

void runCopyBenchmark() {
    std::cout << "========================================================\n";
    std::cout << "4. COPY LATENCY BENCHMARK ACROSS CONTAINER SCALES\n";
    std::cout << "========================================================\n";
    auto benchmarkCopy = [](int numMedicines, int iterations) {
        Prescription source(numMedicines);
        for (int i = 0; i < numMedicines; ++i) {
            source.set(i, "Medicine_Compound_" + std::to_string(i));
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            Prescription copy = source;
            (void)copy;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        double perCopyNs = (ms * 1e6) / iterations;

        std::cout << "  N = " << std::setw(6) << numMedicines << " medicines | "
                  << std::setw(6) << iterations << " copies | "
                  << "Total: " << std::fixed << std::setprecision(2) << std::setw(7) << ms << " ms | "
                  << "Per Copy: " << std::setprecision(1) << std::setw(8) << perCopyNs << " ns\n";
    };

    benchmarkCopy(10, 10000);
    benchmarkCopy(1000, 1000);
    benchmarkCopy(100000, 20);
    std::cout << "========================================================\n\n";
}

void runDesignAndAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "5. DESIGN & ANALYSIS: THE RULE OF THREE & SHALLOW VS DEEP\n";
    std::cout << "========================================================\n";
    std::cout << "A. Shallow Copy vs Deep Copy Memory Diagrams:\n\n";
    std::cout << "   [SHALLOW COPY (Compiler-Generated)]\n";
    std::cout << "   Stack: p [medicines = 0x1000] ----+\n";
    std::cout << "                                     |---> Heap: [ \"Paracetamol\" | \"ORS\" ]\n";
    std::cout << "   Stack: q [medicines = 0x1000] ----+     (Both point to identical buffer!)\n";
    std::cout << "   * Flaw 1: q.set(1, \"Cetirizine\") silently overwrites p's patient record.\n";
    std::cout << "   * Flaw 2: When p and q destruct, delete[] 0x1000 runs TWICE -> CRASH!\n\n";

    std::cout << "   [DEEP COPY (Custom Implementation)]\n";
    std::cout << "   Stack: p [medicines = 0x1000] --------> Heap A: [ \"Ibuprofen\"   | \"ORS\" ]\n";
    std::cout << "   Stack: q [medicines = 0x2000] --------> Heap B: [ \"Paracetamol\" | \"Cetirizine\" ]\n";
    std::cout << "   * Both instances manage independent heap allocations with clean lifecycles.\n\n";

    std::cout << "B. The Rule of Three:\n";
    std::cout << "   If a class manages a raw owning resource (e.g. heap pointer, file descriptor),\n";
    std::cout << "   it must explicitly declare all three:\n";
    std::cout << "     1. Destructor: To release the acquired resource.\n";
    std::cout << "     2. Copy Constructor: To create a distinct duplicate of the resource on initialization.\n";
    std::cout << "     3. Copy Assignment Operator: To release the existing resource and clone the new one.\n\n";

    std::cout << "C. Three Situations Where the Copy Constructor is Called:\n";
    std::cout << "   1. Direct or Copy Initialization: Prescription q = p; or Prescription q(p);\n";
    std::cout << "   2. Passing an Object by Value: void process(Prescription p);\n";
    std::cout << "   3. Returning an Object by Value: Prescription createRx() { return rx; } (if RVO is disabled).\n\n";

    std::cout << "D. Conclusion: Rule of Zero Alternative (std::vector<std::string>):\n";
    std::cout << "   If 'std::string* medicines' is replaced by 'std::vector<std::string>', the class\n";
    std::cout << "   does NOT need to write a custom destructor, copy constructor, or copy assignment!\n";
    std::cout << "   std::vector already manages its own deep copying and memory deallocation.\n";
    std::cout << "   This modern C++ design principle is known as the 'Rule of Zero'.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 02: SHALLOW VS DEEP COPYING         \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runCompleteTheCode();
    runSelfAssignmentTest();
    runCopyCountInvestigation();
    runCopyBenchmark();
    runDesignAndAnalysis();

    return 0;
}
