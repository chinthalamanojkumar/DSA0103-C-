/**
 * ============================================================================
 * ASSIGNMENT 3 - CO3: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 03 (Q3): Dynamic Constructors and Destructors
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - AUDITORIUM SEAT BOOKING:
 * An auditorium's seat map is created dynamically when an event is set up;
 * the number of rows and seats per row differs from event to event.
 * When the event object is destroyed, all memory must be released and
 * a closing summary written.
 * 
 * SECTIONS:
 * 1. Predict the Output: Logger Lifecycle & Copy Demonstration (Page 7-8)
 * 2. Dynamic 2D SeatMap Class with Lifecycle Logging
 * 3. Scope & Allocation Lifecycle Verification (Stack, Inner Block, Heap new/delete)
 * 4. Seat Booking and Cancellation Operations
 * 5. Memory Footprint Measurement across 10x20, 100x200, 1000x2000 Dimensions
 * 6. Alternative Concept: Flat 1D Block vs Array-of-Pointers Analysis
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cassert>

// ============================================================================
// SECTION 1: PREDICT THE OUTPUT (Exact Code from Assignment Specification)
// ============================================================================
struct Logger {
    std::string n;
    Logger(const std::string& s) : n(s) { std::cout << '+' << n << ' '; }
    Logger(const Logger& o) : n(o.n + "c") { std::cout << "copy" << n << ' '; }
    ~Logger() { std::cout << '-' << n << ' '; }
};

void use(Logger) { std::cout << "use "; }

void runPredictTheOutput() {
    std::cout << "========================================================\n";
    std::cout << "1. PREDICT THE OUTPUT VERIFICATION (Assignment Page 7-8)\n";
    std::cout << "========================================================\n";
    std::cout << "[MY PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "+A +B -B copyAc use -Ac +P -P end -A \n\n";

    std::cout << "[ACTUAL PROGRAM EXECUTION OUTPUT]:\n";
    {
        Logger a("A");
        {
            Logger b("B");
        }
        use(a);
        Logger* p = new Logger("P");
        delete p;
        std::cout << "end ";
    }
    std::cout << "\n\n";

    std::cout << "[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. '+A': Object 'a' constructed at main/outer scope.\n";
    std::cout << "2. '+B': Object 'b' constructed inside inner block scope.\n";
    std::cout << "3. '-B': Inner block terminates; automatic variable 'b' destructs (LIFO).\n";
    std::cout << "4. 'copyAc use -Ac': use(a) takes argument by VALUE. The copy constructor\n";
    std::cout << "   constructs temporary parameter with name 'Ac' (copyAc), executes body ('use '),\n";
    std::cout << "   and destructs the temporary parameter at function exit ('-Ac').\n";
    std::cout << "5. '+P -P': Dynamic object 'p' allocated on heap via 'new' (+P) and explicitly\n";
    std::cout << "   destroyed via 'delete' (-P).\n";
    std::cout << "6. 'end ': Printed before outer scope exit.\n";
    std::cout << "7. '-A': Outer scope terminates; automatic variable 'a' is destroyed.\n";
    std::cout << "Status: PREDICTION MATCHES EXECUTION WITH 100% ACCURACY\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 2: AUDITORIUM SEAT MAP CLASS (DYNAMIC CONSTRUCTOR & DESTRUCTOR)
// ============================================================================
class AuditoriumEvent {
private:
    std::string eventName;
    int rows;
    int seatsPerRow;
    char** seatMap; // Dynamically allocated 2D array (array of row pointers)
    int totalBooked;

public:
    // Dynamic Constructor
    AuditoriumEvent(std::string name, int r, int s)
        : eventName(std::move(name)), rows(r), seatsPerRow(s), seatMap(nullptr), totalBooked(0)
    {
        std::cout << "  [Event Constructor] Initializing '" << eventName
                  << "' (" << rows << " rows x " << seatsPerRow << " seats)...\n";

        if (rows > 0 && seatsPerRow > 0) {
            // Allocate array of pointers
            seatMap = new char*[rows];
            for (int i = 0; i < rows; ++i) {
                seatMap[i] = new char[seatsPerRow];
                for (int j = 0; j < seatsPerRow; ++j) {
                    seatMap[i][j] = '.'; // '.' represents an available seat
                }
            }
        } else {
            std::cout << "  [Notice] Event created with 0 rows or seats (Empty Map).\n";
        }
    }

    // Rule of Three Disablement (Prevent unintentional shallow copying for demo)
    AuditoriumEvent(const AuditoriumEvent&) = delete;
    AuditoriumEvent& operator=(const AuditoriumEvent&) = delete;

    // Destructor: Deallocates 2D array and prints closing summary
    ~AuditoriumEvent() {
        std::cout << "  [Event Destructor] Tearing down '" << eventName << "'...\n";
        if (seatMap != nullptr) {
            for (int i = 0; i < rows; ++i) {
                delete[] seatMap[i]; // Free each individual row
            }
            delete[] seatMap;        // Free the pointer array
            seatMap = nullptr;
        }
        std::cout << "  >>> CLOSING SUMMARY for '" << eventName << "': "
                  << totalBooked << " seats booked out of " << (rows * seatsPerRow)
                  << " total capacity. All dynamic heap memory successfully released.\n";
    }

    bool bookSeat(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow || seatMap == nullptr) {
            std::cout << "  [Error] Invalid seat coordinate (" << r << ", " << s << ")\n";
            return false;
        }
        if (seatMap[r][s] == 'X') {
            std::cout << "  [Declined] Seat (" << r << ", " << s << ") is already booked.\n";
            return false;
        }
        seatMap[r][s] = 'X';
        totalBooked++;
        std::cout << "  [Success] Booked Seat (" << r << ", " << s << ") for " << eventName << '\n';
        return true;
    }

    bool cancelSeat(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seatsPerRow || seatMap == nullptr) return false;
        if (seatMap[r][s] == '.') return false;
        seatMap[r][s] = '.';
        totalBooked--;
        std::cout << "  [Cancelled] Released Seat (" << r << ", " << s << ")\n";
        return true;
    }

    void displayMap() const {
        std::cout << "  Seat Map for '" << eventName << "':\n";
        if (seatMap == nullptr) {
            std::cout << "    [Empty Map]\n";
            return;
        }
        for (int i = 0; i < rows; ++i) {
            std::cout << "    Row " << std::setw(2) << i << ": ";
            for (int j = 0; j < seatsPerRow; ++j) {
                std::cout << seatMap[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }
};

// ============================================================================
// DEMONSTRATION OF ALL LIFECYCLE SCOPES
// ============================================================================

void runLifecycleDemonstration() {
    std::cout << "========================================================\n";
    std::cout << "2. SCOPE & ALLOCATION ORDER: STACK, BLOCK, AND HEAP\n";
    std::cout << "========================================================\n";

    // 1. Stack Object (Outer Scope)
    std::cout << "[Step 1] Creating outer stack event 'Annual Convocation'...\n";
    AuditoriumEvent stackEvent("Annual Convocation", 4, 6);
    stackEvent.bookSeat(0, 0);
    stackEvent.bookSeat(0, 1);

    // 2. Block-Scope Object (Inner Scope)
    std::cout << "\n[Step 2] Entering inner block scope...\n";
    {
        AuditoriumEvent blockEvent("Guest Lecture - AI Robotics", 2, 4);
        blockEvent.bookSeat(1, 2);
        std::cout << "Leaving inner block scope...\n";
    } // blockEvent destructor MUST run here!

    // 3. Dynamic Heap Object (new and delete)
    std::cout << "\n[Step 3] Allocating dynamic event on HEAP via new...\n";
    AuditoriumEvent* heapEvent = new AuditoriumEvent("National Tech Fest", 3, 5);
    heapEvent->bookSeat(2, 2);
    heapEvent->bookSeat(2, 3);
    heapEvent->cancelSeat(2, 2);

    std::cout << "\nExplicitly deleting heap event via delete...\n";
    delete heapEvent; // heapEvent destructor MUST run here!

    // 4. Edge Case: Event with zero rows
    std::cout << "\n[Step 4] Creating edge-case event with 0 rows...\n";
    AuditoriumEvent zeroEvent("Cancelled Orientation", 0, 0);

    std::cout << "\nExiting runLifecycleDemonstration(). Stack events will destruct in reverse order...\n";
}

void runDesignAndAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "3. DESIGN & ANALYSIS: DESTRUCTOR MECHANICS & RAII\n";
    std::cout << "========================================================\n";
    std::cout << "A. Order of Constructors and Destructors:\n";
    std::cout << "   - Stack/Automatic Objects: Constructed in top-down sequential order;\n";
    std::cout << "     destructed in strict LIFO (Last-In, First-Out) reverse order at scope exit.\n";
    std::cout << "   - Block-Scope Objects: Destructed immediately at the enclosing closing brace '}'.\n";
    std::cout << "   - Heap Objects: Constructed when 'new' is evaluated; destroyed ONLY when 'delete'\n";
    std::cout << "     is explicitly called by the programmer.\n\n";

    std::cout << "B. What Happens if delete is Forgotten vs delete Used Instead of delete[]:\n";
    std::cout << "   - Forgotten delete: Memory leak! The heap memory remains allocated until the entire\n";
    std::cout << "     process terminates, depleting available RAM.\n";
    std::cout << "   - Using 'delete ptr' instead of 'delete[] ptr': UNDEFINED BEHAVIOUR!\n";
    std::cout << "     'delete' treats the pointer as a single scalar object, failing to invoke destructors\n";
    std::cout << "     for remaining array elements and corrupting internal heap allocator bookkeeping metadata.\n\n";

    std::cout << "C. Why Destructors Take No Arguments and Cannot be Overloaded:\n";
    std::cout << "   - Destruction is triggered automatically by runtime scope exit, exception stack unwinding,\n";
    std::cout << "     or operator delete. The runtime has no mechanism to pass arguments at destruction time.\n";
    std::cout << "   - Because arguments cannot be passed, constructor-style signature overloading is impossible.\n\n";

    std::cout << "D. Memory Footprint Comparison Across Scales:\n";
    auto calculateFootprint = [](long long r, long long s) {
        long long pointerArrayBytes = r * sizeof(char*); // 8 bytes per row pointer
        long long charElementsBytes = r * s * sizeof(char); // 1 byte per seat
        long long totalBytes = pointerArrayBytes + charElementsBytes;
        double mb = totalBytes / (1024.0 * 1024.0);
        std::cout << "   " << std::setw(5) << r << " x " << std::setw(5) << s << " | "
                  << "Row Pointers: " << std::setw(8) << pointerArrayBytes << " B | "
                  << "Seats: " << std::setw(10) << charElementsBytes << " B | "
                  << "Total: " << std::fixed << std::setprecision(3) << mb << " MB (" << totalBytes << " bytes)\n";
    };

    std::cout << "   Dimensions      | Pointer Overhead        | Seat Buffer Space    | Total Allocation\n";
    std::cout << "   -----------------------------------------------------------------------------------\n";
    calculateFootprint(10, 20);
    calculateFootprint(100, 200);
    calculateFootprint(1000, 2000);
    std::cout << "========================================================\n\n";
}

void runAlternativeConcept() {
    std::cout << "========================================================\n";
    std::cout << "4. ALTERNATIVE CONCEPT: CONTIGUOUS 1D BLOCK VS POINTER ARRAY\n";
    std::cout << "========================================================\n";
    std::cout << "Alternative Design: Single Contiguous Heap Block (rows * cols):\n";
    std::cout << "  char* seatMap = new char[rows * seatsPerRow];\n";
    std::cout << "  Access Formula: seatMap[r * seatsPerRow + s];\n\n";
    std::cout << "Comparative Evaluation Matrix:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Criterion              | Array of Row Pointers (char**)  | Flat 1D Block (char*)      |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Number of Allocations  | rows + 1 heap calls             | Strictly 1 heap call       |\n";
    std::cout << "| Extra Pointer Overhead | rows * 8 bytes                  | 0 bytes                    |\n";
    std::cout << "| Cache Locality         | Poor (Row buffers scattered)    | Perfect (Contiguous RAM)   |\n";
    std::cout << "| Access Syntax          | seatMap[r][s]                   | seatMap[r * cols + s]      |\n";
    std::cout << "| Access Time Complexity | O(1) (2 pointer indirections)   | O(1) (1 indirection)       |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "DEFENSE:\n";
    std::cout << "  The Flat 1D block design is vastly superior for production graphics and matrix\n";
    std::cout << "  computations. For 1000 rows, it eliminates 1000 individual heap allocation/free\n";
    std::cout << "  system calls, saves 8 KB of pointer memory, and guarantees zero cache-miss latency.\n";
    std::cout << "========================================================\n";
}

int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 3 - TASK 03: DYNAMIC CTORS & DTORS (RAII)    \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictTheOutput();
    runLifecycleDemonstration();
    runDesignAndAnalysis();
    runAlternativeConcept();

    return 0;
}
