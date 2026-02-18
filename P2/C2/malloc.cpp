#include <iostream>
using namespace std;

int main() {
    int n;

    cout << "Enter number of integers: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid input." << endl;
        return 1;
    }

    // Allocate memory for n integers
    int* arr = new(nothrow) int[n];

    if (arr == nullptr) {
        cout << "Memory allocation failed." << endl;
        return 1;
    }

    // Initialize values
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Free allocated memory
    delete[] arr;

    return 0;
}