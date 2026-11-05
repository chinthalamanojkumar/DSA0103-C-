/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 01: Class Design Review - Diagnose and Repair Duplicated Classes
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
#include <string>
#include <vector>
#include <memory>

// ============================================================================
// TASK 1.2: REPAIRED CLASS HIERARCHY
// ============================================================================

// Abstract Base Class: Defines common identity and polymorphic interface
class Staff {
protected:
    std::string name;
    int id;

public:
    Staff(std::string n, int i) : name(std::move(n)), id(i) {}
    virtual ~Staff() = default; // Essential virtual destructor for polymorphic deletion

    // Pure virtual function: forces derived classes to specify pay calculation
    virtual double pay() const = 0;
    virtual std::string getRole() const = 0;

    virtual void printDetails() const {
        std::cout << "  [" << std::left << std::setw(14) << getRole() << "] ID: "
                  << std::setw(6) << id << " | Name: " << std::setw(20) << name
                  << " | Monthly Pay: Rs. " << std::fixed << std::setprecision(2)
                  << std::setw(9) << pay() << '\n';
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
};

// Intermediate Base Class for Salaried/Regular Employees
class RegularStaff : public Staff {
protected:
    double basic;
    static constexpr double DA_RATE = 0.42; // Eliminated magic number (DA = 42%)

public:
    RegularStaff(std::string n, int i, double b)
        : Staff(std::move(n), i), basic(b) {}

    double getBasicPay() const { return basic; }
    double getDearnessAllowance() const { return basic * DA_RATE; }
};

// Concrete Derived Class 1: Professor
class Professor : public RegularStaff {
private:
    double researchAllowance;

public:
    Professor(std::string n, int i, double b, double research)
        : RegularStaff(std::move(n), i, b), researchAllowance(research) {}

    double pay() const override {
        return basic + (basic * DA_RATE) + researchAllowance;
    }

    std::string getRole() const override { return "Professor"; }
};

// Concrete Derived Class 2: LabAssistant
class LabAssistant : public RegularStaff {
public:
    LabAssistant(std::string n, int i, double b)
        : RegularStaff(std::move(n), i, b) {}

    double pay() const override {
        return basic + (basic * DA_RATE);
    }

    std::string getRole() const override { return "Lab Assistant"; }
};

// Concrete Derived Class 3: GuestFaculty (Hourly billing)
class GuestFaculty : public Staff {
private:
    double hours;
    double hourlyRate;

public:
    GuestFaculty(std::string n, int i, double h, double r)
        : Staff(std::move(n), i), hours(h), hourlyRate(r) {}

    double pay() const override {
        return hours * hourlyRate;
    }

    std::string getRole() const override { return "Guest Faculty"; }
};

// ============================================================================
// EVALUATION & DEMONSTRATION
// ============================================================================

void printDiagnosis() {
    std::cout << "========================================================\n";
    std::cout << "TASK 1.1: DIAGNOSIS OF DESIGN DEFICIENCIES\n";
    std::cout << "========================================================\n";
    std::cout << "1. Severe Code & Field Duplication:\n";
    std::cout << "   - 'name', 'id', and 'basic' are copy-pasted across Professor and LabAssistant.\n";
    std::cout << "2. Complete Violation of Encapsulation:\n";
    std::cout << "   - All data members are declared 'public', exposing internal financial state to tampering.\n";
    std::cout << "3. Inconsistent Naming Conventions:\n";
    std::cout << "   - GuestFaculty uses 'nm' and 'code' instead of 'name' and 'id'.\n";
    std::cout << "4. Dead / Unused Fields:\n";
    std::cout << "   - The 'da' field is stored in memory but completely ignored in pay() calculations.\n";
    std::cout << "5. Hardcoded Magic Numbers:\n";
    std::cout << "   - The Dearness Allowance multiplier '0.42' is hardcoded as a literal in multiple classes.\n";
    std::cout << "6. Inability to Process Polymorphically in One Loop:\n";
    std::cout << "   - Because classes share no common base type, the university payroll cannot iterate\n";
    std::cout << "     through a unified container (e.g. vector<Staff*>), forcing separate loops for each type.\n";
    std::cout << "========================================================\n\n";
}

void runPayrollDemo() {
    std::cout << "========================================================\n";
    std::cout << "TASK 1.2: REPAIRED POLYMORPHIC PAYROLL EXECUTION\n";
    std::cout << "========================================================\n";

    // Polymorphic array of base-class pointers
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Dr. A. Raman", 101, 80000.0, 15000.0));
    payroll.push_back(std::make_unique<LabAssistant>("B. Suresh", 205, 35000.0));
    payroll.push_back(std::make_unique<GuestFaculty>("C. Priya", 309, 45.0, 1200.0));
    payroll.push_back(std::make_unique<Professor>("Dr. D. Varma", 104, 95000.0, 20000.0));

    double totalPayroll = 0.0;
    std::cout << "University Consolidated Payroll (Unified Processing Loop):\n";
    for (const auto& member : payroll) {
        member->printDetails();
        totalPayroll += member->pay();
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "  TOTAL MONTHLY DISBURSEMENT: Rs. " << std::fixed << std::setprecision(2)
              << totalPayroll << '\n';
    std::cout << "========================================================\n\n";
}

void printDefense() {
    std::cout << "========================================================\n";
    std::cout << "TASK 1.3: DESIGN DEFENCE (< 100 Words)\n";
    std::cout << "========================================================\n";
    std::cout << "Our hierarchy establishes Staff as an abstract base class with a pure virtual pay()\n"
              << "method. Salaried personnel derive from RegularStaff, centralizing the 42% DA formula.\n"
              << "Adding a new category (e.g. ResearchScholar) requires only inheriting from Staff and\n"
              << "implementing pay(). Existing working classes remain untouched (Open-Closed Principle),\n"
              << "and the unified payroll processing loop seamlessly accepts new types via runtime polymorphism.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 01: CLASS DESIGN REVIEW             \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    printDiagnosis();
    runPayrollDemo();
    printDefense();

    return 0;
}
