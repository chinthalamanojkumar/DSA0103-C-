// Demonstrating the original flawed code that causes ambiguity compiler errors
double charge(double weight) {
    return weight * 50.0;
}

double charge(double weight, bool express) {
    return weight * 50.0 + (express ? 100.0 : 0.0);
}

double charge(double weight, bool express = false, bool insured = false) {
    return weight * 50.0 + (express ? 100.0 : 0.0) + (insured ? 50.0 : 0.0);
}

int main() {
    // Calling charge with 1 argument:
    // Matches BOTH charge(double) AND charge(double, bool=false, bool=false)
    charge(5.0);

    // Calling charge with 2 arguments:
    // Matches BOTH charge(double, bool) AND charge(double, bool, bool=false)
    charge(5.0, true);

    return 0;
}
