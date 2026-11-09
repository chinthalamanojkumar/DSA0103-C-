/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 04: Pointers to Derived Classes - Forensic Defect Analysis & Repair
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

// ============================================================================
// SECTION 1: PREDICT THE OUTPUT (Exact Code from Assignment Specification)
// ============================================================================
namespace PredictOutput {
    struct Account {
        double rate() const { return 3.5; }
        virtual double vrate() const { return 3.5; }
        virtual ~Account() = default;
    };

    struct SeniorAccount : Account {
        double rate() const { return 4.0; }
        double vrate() const override { return 4.0; }
    };
}

void runPredictTheOutput() {
    std::cout << "========================================================\n";
    std::cout << "1. PREDICT THE OUTPUT VERIFICATION (Assignment Page 10)\n";
    std::cout << "========================================================\n";
    std::cout << "[MY PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "3.5 4 3.5\n\n";

    std::cout << "[ACTUAL PROGRAM EXECUTION OUTPUT]:\n";
    PredictOutput::SeniorAccount s;
    PredictOutput::Account* p = &s;
    PredictOutput::Account copy = s; // Object slicing occurs here!
    std::cout << p->rate() << ' ' << p->vrate() << ' ' << copy.vrate() << '\n';

    std::cout << "\n[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. p->rate() evaluates to 3.5:\n";
    std::cout << "   - 'rate()' is non-virtual. The call is statically bound at COMPILE-TIME\n";
    std::cout << "     based on the pointer type (Account*), ignoring the runtime SeniorAccount object.\n";
    std::cout << "2. p->vrate() evaluates to 4 (or 4.0):\n";
    std::cout << "   - 'vrate()' is virtual. The call is dynamically dispatched at RUNTIME via the vtable,\n";
    std::cout << "     successfully invoking SeniorAccount::vrate().\n";
    std::cout << "3. copy.vrate() evaluates to 3.5:\n";
    std::cout << "   - Object Slicing: 'Account copy = s' copies only the base Account sub-object into 'copy'.\n";
    std::cout << "     'copy' is a pure Account instance with Account's vtable pointer, returning 3.5.\n";
    std::cout << "Status: PREDICTION MATCHES EXECUTION WITH 100% ACCURACY\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 2: FORENSIC DEFECT DEMONSTRATION & REPAIRED ARCHITECTURE
// ============================================================================

// Repaired Base Class
class Account {
public:
    virtual ~Account() {
        std::cout << "    [Destructor] Account base cleaned up.\n";
    }

    // Virtual function enables dynamic polymorphism
    virtual double rate() const {
        return 3.5;
    }

    virtual std::string getAccountType() const {
        return "Standard Account";
    }
};

// Repaired Derived Class
class SeniorAccount : public Account {
private:
    double* history;

public:
    SeniorAccount() : history(new double[100]{0.0}) {
        std::cout << "    [Constructor] SeniorAccount allocated 100-element history buffer.\n";
    }

    ~SeniorAccount() override {
        std::cout << "    [Destructor] SeniorAccount deallocating history buffer...\n";
        delete[] history;
        history = nullptr;
    }

    double rate() const override {
        return 4.0;
    }

    std::string getAccountType() const override {
        return "Senior Citizen Account";
    }
};

void runForensicInvestigation() {
    std::cout << "========================================================\n";
    std::cout << "2. FORENSIC DEFECT AUDIT & REPAIR VERIFICATION\n";
    std::cout << "========================================================\n";
    std::cout << "Task 4.1 - Defect Identification:\n";
    std::cout << "  Defect 1: Static Binding - Without 'virtual', pointer calls default to base class.\n";
    std::cout << "  Defect 2: Non-Virtual Destructor Memory Leak - Deleting through Account* fails to\n";
    std::cout << "            invoke ~SeniorAccount(), permanently leaking the 800-byte history array.\n";
    std::cout << "  Defect 3: Object Slicing - Storing polymorphic objects by value in vector<Account>\n";
    std::cout << "            slices derived members and resets the vtable to base Account.\n\n";

    std::cout << "Task 4.3 - Executing Repaired Polymorphic System:\n";
    std::vector<std::unique_ptr<Account>> bankLedger;
    bankLedger.push_back(std::make_unique<Account>());
    bankLedger.push_back(std::make_unique<SeniorAccount>());

    std::cout << "\nIterating over accounts polymorphically:\n";
    for (size_t i = 0; i < bankLedger.size(); ++i) {
        std::cout << "  Account #" << i << " [" << bankLedger[i]->getAccountType() << "]: "
                  << "Interest Rate = " << std::fixed << std::setprecision(2)
                  << bankLedger[i]->rate() << "%\n";
    }

    std::cout << "\nExiting scope: unique_ptr automatically cleans up with virtual destructors:\n";
}

void printPreventionRules() {
    std::cout << "\n========================================================\n";
    std::cout << "TASK 4.4: PREVENTION RULES (Best Practices in C++)\n";
    std::cout << "========================================================\n";
    std::cout << "Rule 1 (Polymorphic Base Destructors):\n";
    std::cout << "  Any base class designed for polymorphic deletion via base pointer MUST declare a\n";
    std::cout << "  'virtual' destructor: virtual ~Base() = default;\n\n";

    std::cout << "Rule 2 (Explicit Override Specifier):\n";
    std::cout << "  Always mark overriding functions with 'override'. If the signature mismatches,\n";
    std::cout << "  the compiler immediately flags the bug instead of silently creating a new overload.\n\n";

    std::cout << "Rule 3 (Smart Pointers & No Slicing):\n";
    std::cout << "  Store polymorphic hierarchies as containers of pointers (std::vector<std::unique_ptr<Base>>)\n";
    std::cout << "  rather than containers of values (std::vector<Base>) to avoid object slicing.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 04: POLYMORPHISM FORENSICS          \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictTheOutput();
    runForensicInvestigation();
    printPreventionRules();

    return 0;
}
