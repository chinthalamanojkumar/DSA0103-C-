/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 02: University People Management - From Requirement to Hierarchy
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

// ============================================================================
// PREDICT THE OUTPUT STRUCTURES (Exact from Assignment Specification)
// ============================================================================
namespace PredictOutput {
    struct Person { Person() { std::cout << "Person "; } };
    struct Student : virtual Person { Student() { std::cout << "Student "; } };
    struct Staff : virtual Person { Staff() { std::cout << "Staff "; } };
    struct TeachingAssistant : Student, Staff {
        TeachingAssistant() { std::cout << "TA "; }
    };
}

// Without virtual keyword (for empirical contrast)
namespace WithoutVirtual {
    struct Person { Person() { std::cout << "Person "; } };
    struct Student : Person { Student() { std::cout << "Student "; } };
    struct Staff : Person { Staff() { std::cout << "Staff "; } };
    struct TeachingAssistant : Student, Staff {
        TeachingAssistant() { std::cout << "TA "; }
    };
}

// ============================================================================
// UNIVERSITY PEOPLE MANAGEMENT HIERARCHY
// ============================================================================

// Base Class: Person
class Person {
protected:
    std::string name;
    int id;
    std::string phone;

public:
    Person(std::string n = "Unknown", int i = 0, std::string ph = "0000000000")
        : name(std::move(n)), id(i), phone(std::move(ph)) {}

    virtual ~Person() = default;

    virtual void display() const {
        std::cout << "ID: " << std::setw(6) << id << " | Name: " << std::setw(16) << name
                  << " | Phone: " << phone;
    }

    const std::string& getName() const { return name; }
    int getId() const { return id; }
};

// Virtual Derived Class 1: Student
class Student : virtual public Person {
protected:
    std::string programme;
    double cgpa;

public:
    Student(std::string n, int i, std::string ph, std::string prog, double gpa)
        : Person(std::move(n), i, std::move(ph)), programme(std::move(prog)), cgpa(gpa) {}

    void display() const override {
        Person::display();
        std::cout << " | Prog: " << programme << " | CGPA: " << std::fixed << std::setprecision(2) << cgpa;
    }
};

// Virtual Derived Class 2: Staff
class Staff : virtual public Person {
protected:
    std::string department;
    double salary;

public:
    Staff(std::string n, int i, std::string ph, std::string dept, double sal)
        : Person(std::move(n), i, std::move(ph)), department(std::move(dept)), salary(sal) {}

    void display() const override {
        Person::display();
        std::cout << " | Dept: " << department << " | Salary: Rs. " << std::fixed << std::setprecision(2) << salary;
    }
};

// Hybrid Derived Class: TeachingAssistant (Resolves Diamond Problem via virtual inheritance)
class TeachingAssistant : public Student, public Staff {
private:
    int teachingHours;

public:
    // In virtual inheritance, the MOST-DERIVED class calls the virtual base constructor directly!
    TeachingAssistant(std::string n, int i, std::string ph, std::string prog,
                      double gpa, std::string dept, double sal, int hours)
        : Person(n, i, ph), // Direct virtual base initialization
          Student(n, i, ph, std::move(prog), gpa),
          Staff(n, i, ph, std::move(dept), sal),
          teachingHours(hours) {}

    void display() const override {
        std::cout << "[Teaching Assistant] ";
        Person::display();
        std::cout << " | Prog: " << programme << " (CGPA: " << cgpa << ")"
                  << " | Dept: " << department << " | Stipend: Rs. " << salary
                  << " | Load: " << teachingHours << " hrs/week\n";
    }
};

// Multilevel Derived Class: ResearchScholar (Student -> ResearchScholar)
class ResearchScholar : public Student {
private:
    std::string fellowshipName;

public:
    ResearchScholar(std::string n, int i, std::string ph, std::string prog,
                    double gpa, std::string fellow)
        : Person(n, i, ph),
          Student(std::move(n), i, std::move(ph), std::move(prog), gpa),
          fellowshipName(std::move(fellow)) {}

    void display() const override {
        std::cout << "[Research Scholar ] ";
        Student::display();
        std::cout << " | Fellowship: " << fellowshipName << '\n';
    }
};

// ============================================================================
// DEMONSTRATION & VERIFICATION
// ============================================================================

void runPredictOutput() {
    std::cout << "========================================================\n";
    std::cout << "PREDICT THE OUTPUT VERIFICATION (Assignment Page 4-5)\n";
    std::cout << "========================================================\n";
    std::cout << "[MY PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "Person Student Staff TA \n\n";

    std::cout << "[ACTUAL PROGRAM EXECUTION OUTPUT]:\n";
    {
        PredictOutput::TeachingAssistant ta;
        std::cout << '\n';
    }

    std::cout << "\n[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. Virtual Base Initialization Order Rule:\n";
    std::cout << "   - Virtual base classes (Person) are ALWAYS constructed first, exactly ONCE,\n";
    std::cout << "     by the most-derived class (TeachingAssistant) before any non-virtual bases.\n";
    std::cout << "2. Non-Virtual Bases in Order of Declaration:\n";
    std::cout << "   - 'Student' is listed first in 'struct TeachingAssistant : Student, Staff',\n";
    std::cout << "     so Student() runs next.\n";
    std::cout << "   - 'Staff' is listed second, so Staff() runs next.\n";
    std::cout << "3. Most-Derived Constructor:\n";
    std::cout << "   - TeachingAssistant() runs last.\n";
    std::cout << "Result: 'Person Student Staff TA '. Prediction matched 100%!\n\n";

    std::cout << "Contrast: If 'virtual' is removed from base classes:\n";
    std::cout << "Output without virtual: ";
    {
        WithoutVirtual::TeachingAssistant nonVirtualTa;
        std::cout << '\n';
    }
    std::cout << "Explanation: Person constructor executed TWICE because two separate Person\n"
              << "sub-objects were instantiated along Student and Staff inheritance paths!\n";
    std::cout << "========================================================\n\n";
}

void printAccessTableAndDesign() {
    std::cout << "========================================================\n";
    std::cout << "TASK 2.1 & 2.4: RELATIONSHIP CLASSIFICATION & ACCESS TABLE\n";
    std::cout << "========================================================\n";
    std::cout << "A. Relationship Classification:\n";
    std::cout << "   1. Student IS-A Person          : Hierarchical Inheritance\n";
    std::cout << "   2. Staff IS-A Person            : Hierarchical Inheritance\n";
    std::cout << "   3. TeachingAssistant IS-A Student AND Staff : Multiple / Diamond (Hybrid) Inheritance\n";
    std::cout << "   4. ResearchScholar IS-A Student : Multilevel Inheritance\n\n";

    std::cout << "B. C++ Inheritance Access Specifier Visibility Table:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Member in Person | Public Inheritance    | Protected Inheritance | Private Inheritance  |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| public           | public in derived     | protected in derived  | private in derived   |\n";
    std::cout << "| protected        | protected in derived  | protected in derived  | private in derived   |\n";
    std::cout << "| private          | Hidden (Inaccessible) | Hidden (Inaccessible) | Hidden (Inaccessible)|\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "========================================================\n\n";
}

void runHierarchyExecution() {
    std::cout << "========================================================\n";
    std::cout << "TASK 2.3: UNIVERSITY HIERARCHY EXECUTION TEST\n";
    std::cout << "========================================================\n";
    TeachingAssistant ta("Karthik M", 192472008, "9876543210", "M.Tech CSE", 9.45, "Computer Science", 25000.0, 10);
    ta.display();

    ResearchScholar rs("Divya S", 192472015, "9123456780", "Ph.D AI", 9.80, "DST-INSPIRE Fellowship");
    rs.display();

    std::cout << "\nAmbiguity Check: Directly accessing ta.getName(): \"" << ta.getName() << "\"\n";
    std::cout << "Status: Zero ambiguity! Virtual inheritance guarantees exactly 1 Person sub-object.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 02: UNIVERSITY HIERARCHY (DIAMOND)  \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictOutput();
    printAccessTableAndDesign();
    runHierarchyExecution();

    return 0;
}
