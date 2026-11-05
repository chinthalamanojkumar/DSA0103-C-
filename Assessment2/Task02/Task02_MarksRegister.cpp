/**
 * ============================================================================
 * ASSIGNMENT 2 - CO2: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 02 (Q2): Return by Reference and Its Dangers
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - EXAMINATION MARKS REGISTER:
 * The marks register keeps the marks of a class in an array. A function returns
 * a reference to the highest mark so that a moderation of +2 can be applied
 * directly to it. A second function returns a reference to a local variable.
 * 
 * SECTIONS:
 * 1. Return by Reference to Array Element (LHS assignment & moderation)
 * 2. Return by Reference to Local Variable (Compiler warning & Undefined Behaviour)
 * 3. Comparative Analysis: int& vs const int& vs int (copy)
 * 4. Changed Condition: std::vector Growth & Iterator/Reference Invalidation
 * 5. Alternative Implementations: Index (int) vs Pointer (int*) vs Reference (int&)
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>

// ============================================================================
// SECTION 1: RETURN BY REFERENCE TO ARRAY ELEMENT
// ============================================================================

// Returns an lvalue reference to the maximum element in the array
int& getHighestMark(int arr[], int n) {
    assert(n > 0);
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (arr[i] > arr[maxIdx]) {
            maxIdx = i;
        }
    }
    return arr[maxIdx]; // Safe: arr outlives the function call (caller's scope)
}

// ============================================================================
// SECTION 2: RETURN BY REFERENCE TO LOCAL VARIABLE (DANGEROUS)
// ============================================================================

// Diagnostic suppression wrapper so we can demonstrate and document the issue
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
int& dangerousLocalRef() {
    int tempModeration = 100; // Local variable on the stack frame
    return tempModeration;    // FATAL FLAW: stack frame is deallocated upon return!
}
#pragma GCC diagnostic pop

// ============================================================================
// SECTION 3: THREE RETURN TYPES COMPARISON
// ============================================================================

// A. Returns non-const reference: Modifiable lvalue, 0 copies
int& returnByReference(int arr[], int idx) {
    return arr[idx];
}

// B. Returns const reference: Read-only lvalue, 0 copies
const int& returnByConstReference(const int arr[], int idx) {
    return arr[idx];
}

// C. Returns by value: Rvalue copy in register, safe for locals
int returnByValue(const int arr[], int idx) {
    return arr[idx];
}

// ============================================================================
// SECTION 4: ALTERNATIVE IMPLEMENTATIONS (INDEX VS POINTER VS REFERENCE)
// ============================================================================

// Alternative 1: Returning Index (int)
int findHighestIndex(const int arr[], int n) {
    assert(n > 0);
    int maxIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (arr[i] > arr[maxIdx]) maxIdx = i;
    }
    return maxIdx;
}

// Alternative 2: Returning Pointer (int*)
int* findHighestPointer(int arr[], int n) {
    if (n <= 0) return nullptr;
    int* maxPtr = &arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > *maxPtr) {
            maxPtr = &arr[i];
        }
    }
    return maxPtr;
}

// ============================================================================
// DEMONSTRATION & VERIFICATION ROUTINES
// ============================================================================

void printMarks(const int arr[], int n, const std::string& label) {
    std::cout << "  " << std::left << std::setw(28) << label << ": [ ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << (i + 1 < n ? ", " : " ");
    }
    std::cout << "]\n";
}

void testArrayReturnByReference() {
    std::cout << "========================================================\n";
    std::cout << "1. ARRAY RETURN BY REFERENCE & MODERATION TEST\n";
    std::cout << "========================================================\n";
    int marks[5] = {78, 85, 92, 88, 76};
    printMarks(marks, 5, "Initial Marks");

    // Moderation of +2 directly using return-by-reference on LHS of +=
    std::cout << "\nExecuting: getHighestMark(marks, 5) += 2;\n";
    getHighestMark(marks, 5) += 2;
    printMarks(marks, 5, "After +2 Moderation");
    std::cout << "Highest mark (92) directly updated to: " << marks[2] << " (index 2)\n";

    // Direct assignment on LHS of assignment operator '='
    std::cout << "\nExecuting: getHighestMark(marks, 5) = 99; (LHS assignment)\n";
    getHighestMark(marks, 5) = 99;
    printMarks(marks, 5, "After Direct LHS Assign");
    std::cout << "Highest mark directly set to: " << marks[2] << '\n';
    std::cout << "========================================================\n\n";
}

void testReturnSemanticsComparison() {
    std::cout << "========================================================\n";
    std::cout << "2. SEMANTICS: int& vs const int& vs int (copy)\n";
    std::cout << "========================================================\n";
    int data[3] = {10, 20, 30};

    // 1. int& allows read and write as an lvalue
    returnByReference(data, 1) = 25; // Valid
    std::cout << "  returnByReference(data, 1) = 25 -> updated data[1] to: " << data[1] << '\n';

    // 2. const int& allows read, but assignment is a compile error
    const int& readOnly = returnByConstReference(data, 1);
    std::cout << "  returnByConstReference(data, 1)  -> read-only value: " << readOnly << '\n';
    // readOnly = 50; // COMPILE ERROR: assignment of read-only reference!

    // 3. int returns an independent rvalue copy
    int val = returnByValue(data, 1);
    val = 100; // Modifies only local copy 'val', not data[1]
    std::cout << "  returnByValue(data, 1)           -> modified copy val to: " << val
              << ", but data[1] remains: " << data[1] << '\n';
    std::cout << "========================================================\n\n";
}

void testCompilerWarningEvidence() {
    std::cout << "========================================================\n";
    std::cout << "3. COMPILER WARNING & DANGLING REFERENCE EVIDENCE\n";
    std::cout << "========================================================\n";
    std::cout << "When compiling code that returns a reference to a local variable:\n";
    std::cout << "  int& dangerousLocalRef() {\n";
    std::cout << "      int tempModeration = 100;\n";
    std::cout << "      return tempModeration;\n";
    std::cout << "  }\n\n";
    std::cout << "The g++ compiler emits the following critical warning:\n";
    std::cout << "  >>> warning: reference to local variable 'tempModeration' returned [-Wreturn-local-addr]\n\n";
    std::cout << "Root Cause of Undefined Behaviour:\n";
    std::cout << "  Local variables are allocated in the activation record (stack frame)\n";
    std::cout << "  of the function. When the function returns, its stack frame is popped\n";
    std::cout << "  and reclaimed. The returned reference points to deallocated stack memory.\n";
    std::cout << "  Subsequent function calls overwrite this memory slot with new stack frames,\n";
    std::cout << "  causing silent data corruption, unpredictable garbage values, or SIGSEGV.\n";
    std::cout << "========================================================\n\n";
}

void testVectorReallocationDanger() {
    std::cout << "========================================================\n";
    std::cout << "4. CHANGED CONDITION: std::vector GROWTH & INVALIDATION\n";
    std::cout << "========================================================\n";
    std::vector<int> reg = {60, 75, 88, 70};
    reg.reserve(4); // Capacity exactly 4

    std::cout << "Initial Vector State:\n";
    std::cout << "  size: " << reg.size() << ", capacity: " << reg.capacity() << '\n';
    std::cout << "  Heap buffer address (data()): " << static_cast<void*>(reg.data()) << '\n';

    // Obtain reference to highest element (88 at index 2)
    int& highestRef = reg[2];
    std::cout << "  Reference 'highestRef' binds to address: " << static_cast<void*>(&highestRef)
              << " with value: " << highestRef << '\n';

    // Now append elements to trigger vector growth / reallocation
    std::cout << "\nAppending elements to exceed initial capacity (4)...\n";
    reg.push_back(95); // Forces dynamic reallocation!
    reg.push_back(82);

    std::cout << "After Reallocation:\n";
    std::cout << "  new size: " << reg.size() << ", new capacity: " << reg.capacity() << '\n';
    std::cout << "  NEW heap buffer address (data()): " << static_cast<void*>(reg.data()) << '\n';
    std::cout << "  Original reference address      : " << static_cast<void*>(&highestRef) << '\n';

    if (&highestRef != &reg[2]) {
        std::cout << "  >>> DANGER CONFIRMED: Vector buffer moved to new heap address!\n";
        std::cout << "      'highestRef' is now a DANGLING REFERENCE pointing to deallocated heap memory.\n";
        std::cout << "      Writing to 'highestRef' invokes Undefined Behaviour (Heap Use-After-Free).\n";
    }
    std::cout << "========================================================\n\n";
}

void testAlternativeConceptAndSafety() {
    std::cout << "========================================================\n";
    std::cout << "5. ALTERNATIVE CONCEPT: INDEX (int) VS POINTER (int*) VS REF (int&)\n";
    std::cout << "========================================================\n";
    int marks[5] = {65, 88, 91, 72, 84};

    // Calling Code Comparison:
    std::cout << "Calling Code Demonstration:\n";
    
    // A. Reference approach:
    getHighestMark(marks, 5) += 2;
    std::cout << "  A. Reference approach : getHighestMark(marks, 5) += 2;    -> Highest: " << marks[2] << '\n';

    // B. Index approach:
    int bestIdx = findHighestIndex(marks, 5);
    marks[bestIdx] += 2;
    std::cout << "  B. Index approach     : int i = findHighestIndex(marks, 5); marks[i] += 2; -> Highest: " << marks[2] << '\n';

    // C. Pointer approach:
    int* bestPtr = findHighestPointer(marks, 5);
    if (bestPtr != nullptr) *bestPtr += 2;
    std::cout << "  C. Pointer approach   : int* p = findHighestPointer(marks, 5); *p += 2;     -> Highest: " << marks[2] << '\n';

    std::cout << "\nComparative Evaluation Matrix:\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "| Metric / Feature          | Return Reference (int&) | Return Pointer (int*) | Return Index (int)     |\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "| Time Complexity to Find   | O(n)                    | O(n)                  | O(n)                   |\n";
    std::cout << "| Auxiliary Space           | O(1)                    | O(1)                  | O(1)                   |\n";
    std::cout << "| Nullability / Empty check | Cannot be null          | Can return nullptr    | Can return -1 (sentinel)|\n";
    std::cout << "| Caller Syntax Cleanliness | Natural (LHS '=')       | Requires dereference  | Requires array lookup  |\n";
    std::cout << "| Safety on Vector Growth   | FATAL (Dangles)         | FATAL (Dangles)       | COMPLETELY SAFE!       |\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << "DEFENSE:\n";
    std::cout << "  While reference (int&) offers the most elegant syntax for fixed arrays,\n";
    std::cout << "  the INDEX approach (int) is vastly superior for dynamic containers (std::vector).\n";
    std::cout << "  An integer index remains valid across dynamic reallocations because elements\n";
    std::cout << "  maintain their logical offsets even when the physical memory buffer relocates.\n";
    std::cout << "========================================================\n";
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 2 - TASK 02: RETURN BY REFERENCE & DANGERS   \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    testArrayReturnByReference();
    testReturnSemanticsComparison();
    testCompilerWarningEvidence();
    testVectorReallocationDanger();
    testAlternativeConceptAndSafety();

    return 0;
}
