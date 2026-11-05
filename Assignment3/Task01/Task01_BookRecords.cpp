/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 01 (Q1): Constructors and Constructor Overloading
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - LIBRARY BOOK RECORDS:
 * A library creates Book objects in several situations:
 *   1. A new book with full details (Purchased)
 *   2. A donated book with only a title (Author unknown, 1 copy)
 *   3. An empty record that will be filled later (Default record)
 * 
 * Some previous records were created with garbage values in the number of copies
 * because members were not explicitly initialized.
 * 
 * SECTIONS:
 * 1. Book Class with Member Initializer Lists and Constructor Delegation
 * 2. Invariant Validation (Prevention of Negative Copies)
 * 3. Execution of Overloaded Constructors & Object State Inspection
 * 4. Explicit Constructor Conversion Guard Demonstration
 * 5. Design & Analysis: Initializer List Mechanics, Compiler Synthesis, and Delegation
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <vector>

class Book {
private:
    // Declaration Order dictates Initialization Order!
    std::string title;
    std::string author;
    std::string isbn;
    double price;
    int copies;

public:
    // ========================================================================
    // 1. CANONICAL TARGET PARAMETERISED CONSTRUCTOR
    // Uses Member Initializer List for direct, zero-overhead initialization
    // Validates invariants (copies cannot be negative)
    // ========================================================================
    Book(const std::string& t, const std::string& a, const std::string& i, double p, int c)
        : title(t), author(a), isbn(i), price(p >= 0.0 ? p : 0.0), copies(c >= 0 ? c : 0)
    {
        if (c < 0) {
            std::cout << "  [Validation Warning] Negative copies (" << c
                      << ") rejected for '" << t << "'. Clamped to 0.\n";
        }
    }

    // ========================================================================
    // 2. OVERLOADED CONSTRUCTOR (DONATED BOOK)
    // Uses C++11 Constructor Delegation to forward to canonical constructor
    // ========================================================================
    explicit Book(const std::string& t)
        : Book(t, "Donated Community Contributor", "DONATED-LIB", 0.0, 1)
    {
        // Delegated: avoids repeating initialization logic and boundary checks
    }

    // ========================================================================
    // 3. OVERLOADED CONSTRUCTOR (TITLE + AUTHOR)
    // ========================================================================
    Book(const std::string& t, const std::string& a)
        : Book(t, a, "PENDING-ISBN", 0.0, 1)
    {
    }

    // ========================================================================
    // 4. DEFAULT CONSTRUCTOR
    // Delegates to canonical constructor: guarantees NO garbage values
    // ========================================================================
    Book()
        : Book("Untitled Placeholder", "Unknown Author", "000-0000000000", 0.0, 0)
    {
    }

    // Inspection Member Function
    void print(const std::string& label) const {
        std::cout << "  [" << label << "]\n"
                  << "    Title : \"" << title << "\"\n"
                  << "    Author: " << author << "\n"
                  << "    ISBN  : " << isbn << "\n"
                  << "    Price : Rs. " << std::fixed << std::setprecision(2) << price << "\n"
                  << "    Copies: " << copies << " in circulation\n\n";
    }

    // Getters
    const std::string& getTitle() const { return title; }
    int getCopies() const { return copies; }
    double getPrice() const { return price; }
};

// ============================================================================
// DEMONSTRATION & EXPERIMENTAL INVESTIGATION
// ============================================================================

void runConstructorDemonstrations() {
    std::cout << "========================================================\n";
    std::cout << "1. DEMONSTRATING CONSTRUCTORS & CONSTRUCTOR DELEGATION\n";
    std::cout << "========================================================\n";

    // Situation A: Fully Detailed Book (Purchased)
    Book b1("The C++ Programming Language", "Bjarne Stroustrup", "978-0321563842", 2499.00, 12);
    b1.print("Situation 1: Full Parameterised Constructor");

    // Situation B: Donated Book (Title only)
    Book b2("Clean Code: Agile Software Craftsmanship");
    b2.print("Situation 2: Delegating Donated Constructor");

    // Situation C: Empty / Placeholder Record
    Book b3;
    b3.print("Situation 3: Delegating Default Constructor");

    // Situation D: Testing Invariant Enforcement (Negative Copies)
    std::cout << "Attempting to create Book with invalid negative copies (-5):\n";
    Book b4("Corrupted Record", "Buggy Input", "ERR-999", 500.0, -5);
    b4.print("Situation 4: Invariant Enforced Record");
    std::cout << "========================================================\n\n";
}

void runDesignAndAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "2. DESIGN & ARCHITECTURAL ANALYSIS\n";
    std::cout << "========================================================\n";
    std::cout << "A. When Compiler Supplies a Default Constructor and When It Does Not:\n";
    std::cout << "   - If a class defines ZERO constructors, the C++ compiler automatically synthesizes\n";
    std::cout << "     an implicit default constructor: Book::Book() = default.\n";
    std::cout << "   - As soon as the developer declares ANY constructor (such as a parameterised ctor),\n";
    std::cout << "     the compiler SUPPRESSES the synthesized default constructor.\n";
    std::cout << "   - Evidence: If the user removes 'Book()' from the class, instantiating an array\n";
    std::cout << "     'Book catalog[5];' fails immediately at compile-time with:\n";
    std::cout << "     >>> error: no matching function for call to 'Book::Book()'\n\n";

    std::cout << "B. Why Member Initializer Lists are Preferred to Body Assignments:\n";
    std::cout << "   - Direct Initialization: The member initializer list constructs the data members\n";
    std::cout << "     directly in place using the provided arguments (calling the target constructor).\n";
    std::cout << "   - Body Assignment Inefficiency: Assignment inside the constructor body first forces\n";
    std::cout << "     default construction of every member, followed by an extra copy/move assignment!\n";
    std::cout << "   - Language Necessity: Initializer lists are MANDATORY for:\n";
    std::cout << "       1. 'const' member variables (cannot be assigned after initialization).\n";
    std::cout << "       2. Reference data members (&) (must be bound at creation).\n";
    std::cout << "       3. Member objects lacking a default constructor.\n\n";

    std::cout << "C. Member Initialization Order Invariant:\n";
    std::cout << "   - Members are initialized strictly in the order they are DECLARED in the class,\n";
    std::cout << "     regardless of the order they appear in the constructor initializer list!\n";
    std::cout << "   - Reversing the list order causes g++ to emit the warning:\n";
    std::cout << "     >>> warning: 'Book::title' will be initialized after 'Book::copies' [-Wreorder]\n\n";

    std::cout << "D. Memory State Diagrams of Three Constructed Objects:\n";
    std::cout << "   +--------------------------------------------------------------------------------+\n";
    std::cout << "   | Object: b1 (Full)       | Object: b2 (Donated)    | Object: b3 (Default)       |\n";
    std::cout << "   +--------------------------------------------------------------------------------+\n";
    std::cout << "   | title : \"The C++...\"    | title : \"Clean Code\"    | title : \"Untitled Place...\"|\n";
    std::cout << "   | author: \"Bjarne St...\"  | author: \"Donated...\"    | author: \"Unknown Author\"   |\n";
    std::cout << "   | isbn  : \"978-032...\"    | isbn  : \"DONATED-LIB\"   | isbn  : \"000-0000000000\"   |\n";
    std::cout << "   | price : 2499.00         | price : 0.00            | price : 0.00               |\n";
    std::cout << "   | copies: 12              | copies: 1               | copies: 0 (No garbage!)    |\n";
    std::cout << "   +--------------------------------------------------------------------------------+\n";
    std::cout << "========================================================\n\n";
}

void runExplicitInvestigation() {
    std::cout << "========================================================\n";
    std::cout << "3. DESIGN QUESTION: EXPLICIT CONSTRUCTORS\n";
    std::cout << "========================================================\n";
    std::cout << "When should a constructor be marked 'explicit'?\n";
    std::cout << "  - Any constructor that can be called with a single argument acts by default as an\n";
    std::cout << "    implicit conversion operator from that argument type to the class type.\n";
    std::cout << "  - Marking it 'explicit' prevents accidental, silent implicit conversions.\n";
    std::cout << "Demonstration:\n";
    std::cout << "  Suppose a function takes: void registerBook(const Book& b);\n";
    std::cout << "  - Without 'explicit': Calling registerBook(\"Calculus\") silently invokes the\n";
    std::cout << "    single-argument constructor, allocating a temporary Book behind the scenes.\n";
    std::cout << "  - With 'explicit Book(const std::string& t)': registerBook(\"Calculus\") emits\n";
    std::cout << "    >>> error: could not convert '\"Calculus\"' from 'const char[9]' to 'Book'\n";
    std::cout << "    The caller must explicitly write registerBook(Book(\"Calculus\")), clarifying intent.\n";
    std::cout << "========================================================\n\n";
}

void runAlternativeConcept() {
    std::cout << "========================================================\n";
    std::cout << "4. ALTERNATIVE CONCEPT: DEFAULT ARGUMENTS VS DELEGATION\n";
    std::cout << "========================================================\n";
    std::cout << "Alternative Design: Single Constructor with Default Arguments:\n";
    std::cout << "  Book(std::string t = \"Untitled\", std::string a = \"Unknown\",\n";
    std::cout << "       std::string i = \"N/A\", double p = 0.0, int c = 0);\n\n";
    std::cout << "Comparative Evaluation:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Metric                  | Overloaded + Delegating Ctors   | Default Arguments Ctor   |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Time & Space of Creation| O(1) in both                    | O(1) in both             |\n";
    std::cout << "| Permitted Call Shapes   | Specific, tailored signatures   | Strictly left-to-right   |\n";
    std::cout << "| Donated Book with 1 copy| Direct: Book(title) sets c=1    | Impossible without passing|\n";
    std::cout << "|                         |                                 | author, isbn, price first|\n";
    std::cout << "| Validation Centralization| Canonical target constructor    | Single constructor body  |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "DEFENSE:\n";
    std::cout << "  Constructor delegation is superior because default arguments force positional\n";
    std::cout << "  left-to-right binding. A donated book needs 'title' and 'copies=1', but default\n";
    std::cout << "  arguments would force the caller to supply dummy author, isbn, and price to reach 'copies'!\n";
    std::cout << "  Delegating constructors allow distinct domain scenarios to configure defaults cleanly.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 01: CONSTRUCTOR OVERLOADING         \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runConstructorDemonstrations();
    runDesignAndAnalysis();
    runExplicitInvestigation();
    runAlternativeConcept();

    return 0;
}
