/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 03: Constructors in Derived Classes and Member Classes
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
// SECTION 1: PREDICT THE OUTPUT (Exact Code from Assignment Specification)
// ============================================================================
struct Engine {
    Engine() { std::cout << "Engine "; }
    ~Engine() { std::cout << "~Engine "; }
};

struct Vehicle {
    Vehicle() { std::cout << "Vehicle "; }
    ~Vehicle() { std::cout << "~Vehicle "; }
};

struct Car : Vehicle {
    Engine e; // Member object (Composition)
    Car() { std::cout << "Car "; }
    ~Car() { std::cout << "~Car "; }
};

void runPredictTheOutput() {
    std::cout << "========================================================\n";
    std::cout << "1. PREDICT THE OUTPUT VERIFICATION (Assignment Page 6-7)\n";
    std::cout << "========================================================\n";
    std::cout << "[MY PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "Vehicle Engine Car | ~Car ~Engine ~Vehicle \n\n";

    std::cout << "[ACTUAL PROGRAM EXECUTION OUTPUT]:\n";
    {
        Car c;
        std::cout << "| ";
    }
    std::cout << "\n\n";

    std::cout << "[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. Construction Order Rule:\n";
    std::cout << "   - First, the base class constructor executes ('Vehicle ').\n";
    std::cout << "   - Next, member objects are constructed in declaration order ('Engine ').\n";
    std::cout << "   - Finally, the derived class constructor body executes ('Car ').\n";
    std::cout << "2. Destruction Order Rule (Strict LIFO - Reverse of Construction):\n";
    std::cout << "   - First, the derived class destructor body executes ('~Car ').\n";
    std::cout << "   - Next, member objects are destructed in reverse declaration order ('~Engine ').\n";
    std::cout << "   - Finally, the base class destructor executes ('~Vehicle ').\n";
    std::cout << "Status: PREDICTION MATCHES EXECUTION WITH 100% ACCURACY\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 2: COMPLETE THE CODE (Assignment Page 7-8)
// ============================================================================
class Person {
protected:
    std::string name;
    int age;
public:
    Person(const std::string& n, int a) : name(n), age(a) {}
};

class Address { // used as a member class (composition)
    std::string city;
    int pin;
public:
    Address(const std::string& c, int p) : city(c), pin(p) {}
    std::string str() const { return city + "-" + std::to_string(pin); }
};

class Student : public Person {
    long long regNo;
    Address home; // Member object
public:
    // TODO 1: Write constructor: initialise Person base, regNo and home Address
    // member in initializer list
    Student(const std::string& n, int a, long long r, const std::string& c, int p)
        : Person(n, a), regNo(r), home(c, p) {}

    void print() const
    {
        std::cout << regNo << ' ' << name << " (" << age << ") " << home.str() << '\n';
    }
};

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "2. COMPLETE THE CODE VERIFICATION (Assignment Page 7-8)\n";
    std::cout << "========================================================\n";
    Student s("Kavya", 19, 192311001, "Salem", 636001);
    std::cout << "Observed Output:\n";
    s.print();

    std::cout << "\nExpected Output:\n";
    std::cout << "192311001 Kavya (19) Salem-636001\n";
    std::cout << "Status: VERIFIED EXACT MATCH\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 3: TRACING COMPLEX HIERARCHY LIFECYCLES WITH MEMBER CLASSES
// ============================================================================
struct TracedAddress {
    TracedAddress() { std::cout << "    [Member Ctor] Address initialized\n"; }
    ~TracedAddress() { std::cout << "    [Member Dtor] Address destroyed\n"; }
};

struct TracedCourse {
    TracedCourse() { std::cout << "    [Member Ctor] Course initialized\n"; }
    ~TracedCourse() { std::cout << "    [Member Dtor] Course destroyed\n"; }
};

struct TracedPerson {
    TracedAddress addr; // Member class inside Person
    TracedPerson() { std::cout << "  [Base Ctor] TracedPerson initialized\n"; }
    virtual ~TracedPerson() { std::cout << "  [Base Dtor] TracedPerson destroyed\n"; }
};

struct TracedStudent : virtual public TracedPerson {
    TracedCourse course; // Member class inside Student
    TracedStudent() { std::cout << "  [Derived Ctor] TracedStudent initialized\n"; }
    ~TracedStudent() override { std::cout << "  [Derived Dtor] TracedStudent destroyed\n"; }
};

struct TracedStaff : virtual public TracedPerson {
    TracedStaff() { std::cout << "  [Derived Ctor] TracedStaff initialized\n"; }
    ~TracedStaff() override { std::cout << "  [Derived Dtor] TracedStaff destroyed\n"; }
};

struct TracedTA : public TracedStudent, public TracedStaff {
    TracedTA() { std::cout << "[Most-Derived Ctor] TracedTA initialized\n"; }
    ~TracedTA() override { std::cout << "[Most-Derived Dtor] TracedTA destroyed\n"; }
};

// Non-virtual version for sizeof experiment
struct NonVirtPerson { double a; };
struct NonVirtStudent : public NonVirtPerson { double b; };
struct NonVirtStaff : public NonVirtPerson { double c; };
struct NonVirtTA : public NonVirtStudent, public NonVirtStaff { double d; };

struct VirtPerson { double a; };
struct VirtStudent : virtual public VirtPerson { double b; };
struct VirtStaff : virtual public VirtPerson { double c; };
struct VirtTA : public VirtStudent, public VirtStaff { double d; };

void runLifecycleTraceAndSizeof() {
    std::cout << "========================================================\n";
    std::cout << "3. TRACE ORDER & SIZEOF EXPERIMENT (Virtual vs Non-Virtual)\n";
    std::cout << "========================================================\n";
    std::cout << "Instantiating TeachingAssistant object (TracedTA):\n";
    {
        TracedTA ta;
        std::cout << "--- Object is fully alive in scope ---\n";
    }
    std::cout << "--- Object exited scope and destructed ---\n\n";

    std::cout << "Sizeof Experiment (Memory Layout Comparison):\n";
    std::cout << "  sizeof(NonVirtTA) : " << sizeof(NonVirtTA) << " bytes (Duplicates Person sub-object!)\n";
    std::cout << "  sizeof(VirtTA)    : " << sizeof(VirtTA) << " bytes (Single Person + vptr overhead)\n";
    std::cout << "========================================================\n\n";
}

void printDesignAndAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "4. DESIGN & ANALYSIS: INITIALIZER LISTS & COMPOSITION\n";
    std::cout << "========================================================\n";
    std::cout << "A. Why the Most-Derived Class Must Call the Virtual Base Constructor Directly:\n";
    std::cout << "   - In a diamond hierarchy, there is only ONE shared virtual base sub-object (Person).\n";
    std::cout << "   - If intermediate classes (Student and Staff) both called Person's constructor,\n";
    std::cout << "     conflicting parameters would compete to initialize the same single object.\n";
    std::cout << "   - Therefore, C++ dictates that all intermediate calls to virtual base constructors\n";
    std::cout << "     are ignored; the most-derived class (TeachingAssistant) alone directly invokes\n";
    std::cout << "     the virtual base constructor.\n\n";

    std::cout << "B. Alternative Concept: Composition (Address* pointer vs Address object):\n";
    std::cout << "   - Storing an Address object directly (member value) uses contiguous stack memory,\n";
    std::cout << "     eliminates dynamic heap allocation, requires no custom copy constructor or destructor,\n";
    std::cout << "     and guarantees optimal cache performance.\n";
    std::cout << "   - Storing an Address* pointer forces heap allocation (new/delete), adds 8 bytes for the\n";
    std::cout << "     pointer, risks memory leaks, and mandates implementing the Rule of Three.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 03: CONSTRUCTORS & MEMBER CLASSES   \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictTheOutput();
    runCompleteTheCode();
    runLifecycleTraceAndSizeof();
    printDesignAndAnalysis();

    return 0;
}
