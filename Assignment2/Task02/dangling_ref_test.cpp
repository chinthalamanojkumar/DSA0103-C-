// Demonstrator to capture g++ warning on returning reference to local variable
int& dangerousLocalRef() {
    int temp = 42;
    return temp;
}

int main() {
    int& ref = dangerousLocalRef();
    return ref;
}
