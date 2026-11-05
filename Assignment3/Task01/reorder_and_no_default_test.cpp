#include <iostream>
#include <string>

// Test class 1: Demonstrating -Wreorder warning (list order differs from declaration order)
class ReorderDemo {
    int copies;
    std::string title; // declared second
public:
    // Warning: 'title' is initialized before 'copies' in list, but declared after
    ReorderDemo(const std::string& t, int c) : title(t), copies(c) {}
};

// Test class 2: Demonstrating compiler error when array is created without default constructor
class NoDefaultBook {
    std::string title;
public:
    NoDefaultBook(const std::string& t) : title(t) {}
};

// Test class 3: Demonstrating explicit constructor preventing accidental implicit conversion
class ExplicitDemo {
public:
    explicit ExplicitDemo(int copies) { (void)copies; }
};

void checkExplicit(ExplicitDemo e) { (void)e; }

int main() {
    // 1. Causes -Wreorder warning
    ReorderDemo r("Demo", 5);

    // 2. Uncommenting this causes: error: no matching function for call to 'NoDefaultBook::NoDefaultBook()'
    // NoDefaultBook catalog[3];

    // 3. Demonstrating explicit:
    // checkExplicit(42); // Error: cannot convert 'int' to 'ExplicitDemo'

    return 0;
}
