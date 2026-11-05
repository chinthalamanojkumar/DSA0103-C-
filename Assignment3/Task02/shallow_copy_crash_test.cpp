#include <iostream>
#include <string>

// Flawed class demonstrating shallow copy double-free crash
class FlawedPrescription {
public:
    int n;
    std::string* medicines;

    explicit FlawedPrescription(int count) : n(count), medicines(new std::string[count]) {}
    
    // Missing copy constructor and copy assignment operator!
    // Compiler generates memberwise shallow copy (copies medicines pointer).

    ~FlawedPrescription() {
        std::cout << "  [Destructor] Deleting medicines array at: " << static_cast<void*>(medicines) << '\n';
        delete[] medicines;
    }
};

int main() {
    std::cout << "Starting shallow copy demonstration...\n";
    {
        FlawedPrescription p1(2);
        p1.medicines[0] = "Paracetamol";
        p1.medicines[1] = "ORS";

        std::cout << "p1 created. Pointer: " << static_cast<void*>(p1.medicines) << '\n';

        // Compiler synthesizes shallow copy: p2.medicines = p1.medicines
        FlawedPrescription p2 = p1;
        std::cout << "p2 copied (shallow). Pointer: " << static_cast<void*>(p2.medicines) << '\n';

        std::cout << "Exiting inner block. Destructors will run...\n";
        // Destructors run in reverse order:
        // p2 destructs and deletes medicines pointer!
        // p1 destructs and tries to delete the SAME medicines pointer again! (DOUBLE FREE CRASH!)
    }
    std::cout << "Finished.\n";
    return 0;
}
