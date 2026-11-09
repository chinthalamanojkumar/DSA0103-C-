/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 05: Pointers to Objects and the 'this' Pointer
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

class Cart {
private:
    std::string items[10];
    double prices[10];
    int n = 0;
    double discount = 0; // percent
    std::string customerName;

public:
    Cart(std::string name = "Customer") : customerName(std::move(name)) {}

    // ========================================================================
    // TODO 1: add(): store item and price if space permits, return *this
    // ========================================================================
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this; // Enables fluent method chaining: c.add().add()
    }

    // ========================================================================
    // TODO 2: setDiscount(): use this-> to differentiate member from parameter
    // ========================================================================
    Cart& setDiscount(double discount) {
        this->discount = discount; // Disambiguates member 'discount' from parameter 'discount'
        return *this;
    }

    double total() const {
        double t = 0;
        for (int i = 0; i < n; ++i) t += prices[i];
        return t * (100 - discount) / 100;
    }

    const std::string& getCustomerName() const { return customerName; }
    int getItemCount() const { return n; }
};

// ============================================================================
// VERIFICATION ROUTINES
// ============================================================================

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "1. COMPLETE THE CODE VERIFICATION (Assignment Page 12-13)\n";
    std::cout << "========================================================\n";
    Cart c;
    c.add("Rice", 60).add("Dal", 120).setDiscount(10);
    Cart* p = &c; // pointer to an object
    p->add("Oil", 180);

    std::cout << "Observed Output:\n";
    std::cout << std::fixed << std::setprecision(2) << p->total() << '\n';

    std::cout << "\nExpected Output:\n324.00\n";
    std::cout << "Calculation Breakdown:\n";
    std::cout << "  Items: Rice (60) + Dal (120) + Oil (180) = Rs. 360.00\n";
    std::cout << "  Discount: 10% => 360 * (100 - 10) / 100 = Rs. 324.00\n";
    std::cout << "Status: " << (p->total() == 324.0 ? "VERIFIED EXACT MATCH" : "MISMATCH") << "\n\n";

    std::cout << "Analysis: What would happen if add() returned Cart instead of Cart&?\n";
    std::cout << "  - Returning 'Cart' (by value) creates an unnamed temporary copy at each chained call.\n";
    std::cout << "  - Subsequent chained methods would mutate the temporary copy, leaving original 'c' mutated\n";
    std::cout << "    only by the very first call and completely missing subsequent items!\n";
    std::cout << "========================================================\n\n";
}

void runFourWaysToReachAnObject() {
    std::cout << "========================================================\n";
    std::cout << "2. TASK 5.1: FOUR WAYS TO REACH AN OBJECT\n";
    std::cout << "========================================================\n";

    // (a) Directly by name
    Cart directCart("Direct Stack Cart");
    directCart.add("Milk", 35).setDiscount(5);
    std::cout << "  (a) By Name     : " << directCart.getCustomerName()
              << " -> Total: Rs. " << directCart.total() << '\n';

    // (b) Through a pointer with ->
    Cart* ptrCart = &directCart;
    ptrCart->add("Bread", 40);
    std::cout << "  (b) Pointer (->): " << ptrCart->getCustomerName()
              << " -> Total: Rs. " << ptrCart->total() << '\n';

    // (c) Through a reference with .
    Cart& refCart = directCart;
    refCart.add("Eggs", 60);
    std::cout << "  (c) Reference   : " << refCart.getCustomerName()
              << " -> Total: Rs. " << refCart.total() << '\n';

    // (d) Through a dynamic pointer created with new and released with delete
    Cart* heapCart = new Cart("Heap Dynamic Cart");
    heapCart->add("Coffee", 150).add("Sugar", 45).setDiscount(10);
    std::cout << "  (d) Heap (new)  : " << heapCart->getCustomerName()
              << " -> Total: Rs. " << heapCart->total() << '\n';
    delete heapCart; // Explicit memory release
    std::cout << "========================================================\n\n";
}

void runHighestTotalArraySearch() {
    std::cout << "========================================================\n";
    std::cout << "3. TASK 5.2: ARRAY OF POINTERS & HIGHEST TOTAL SEARCH\n";
    std::cout << "========================================================\n";
    Cart c1("Asha");   c1.add("Apples", 180).add("Bananas", 60);
    Cart c2("Bala");   c2.add("Ghee", 450).add("Rice", 250).setDiscount(5);
    Cart c3("Chitra"); c3.add("Biscuits", 50);
    Cart c4("Dev");    c4.add("Dry Fruits", 600).add("Honey", 200).setDiscount(15);

    Cart* activeCarts[4] = { &c1, &c2, &c3, &c4 };

    // Search for highest cart using pointer arithmetic / indexing
    int maxIdx = 0;
    for (int i = 1; i < 4; ++i) {
        if (activeCarts[i]->total() > activeCarts[maxIdx]->total()) {
            maxIdx = i;
        }
    }

    std::cout << "Active Customer Shopping Carts:\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "  [" << (i == maxIdx ? "HIGHEST" : "       ") << "] "
                  << activeCarts[i]->getCustomerName() << " | Items: "
                  << activeCarts[i]->getItemCount() << " | Total: Rs. "
                  << std::fixed << std::setprecision(2) << activeCarts[i]->total() << '\n';
    }

    std::cout << "\nResult: Highest cart belongs to " << activeCarts[maxIdx]->getCustomerName()
              << " with total Rs. " << activeCarts[maxIdx]->total() << '\n';
    std::cout << "========================================================\n\n";
}

void printComparisonAndThisDetails() {
    std::cout << "========================================================\n";
    std::cout << "4. TASK 5.3: COMPARATIVE EVALUATION & 'this' POINTER\n";
    std::cout << "========================================================\n";
    std::cout << "A. Comparison of the 4 Ways:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Approach          | Lifetime              | Nullability | Best Use Case          |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| (a) By Name       | Automatic block scope | Never null  | Standard local variables|\n";
    std::cout << "| (b) Pointer (->)  | Tied to referee       | Can be null | Optional / retargetable |\n";
    std::cout << "| (c) Reference (.) | Tied to referee       | Never null  | In-out function args    |\n";
    std::cout << "| (d) Heap (new)    | Manual (new/delete)   | Can be null | Dynamic lifecycles      |\n";
    std::cout << "------------------------------------------------------------------------------------\n\n";

    std::cout << "B. Why 'this' is a Pointer and What '*this' Refers to:\n";
    std::cout << "   - 'this' is a hidden parameter passed to all non-static member functions.\n";
    std::cout << "   - Type: ClassName* const this (a const pointer to the current instance).\n";
    std::cout << "   - It holds the physical memory address where the invoking object is stored.\n";
    std::cout << "   - '*this' dereferences the pointer, yielding the lvalue of the calling object itself,\n";
    std::cout << "     allowing member functions to return 'ClassName&' for fluent method chaining.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 05: POINTERS & THE this POINTER     \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runCompleteTheCode();
    runFourWaysToReachAnObject();
    runHighestTotalArraySearch();
    printComparisonAndThisDetails();

    return 0;
}
