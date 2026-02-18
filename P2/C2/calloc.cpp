#include <iostream>
using namespace std;

int main() {
    int n = 10;

    // Allocate memory and initialize to 0
    int* array = new(nothrow) int[n]();

    if (array == nullptr) {
        cerr << "Memory allocation failed!" << endl;
        return 1;
    }

    cout << "Array elements after initialization: ";
    for (int i = 0; i < n; i++) {
        cout << array[i] << " ";
    }
    cout << endl;

    // Free allocated memory
    delete[] array;

    return 0;
}