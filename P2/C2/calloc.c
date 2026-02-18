#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = 10;
    int *array;

    // Allocate memory using calloc
    array = (int*)calloc(n, sizeof(int));

    // Check if memory allocation was successful
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    // Display the array values
    printf("Array elements after calloc: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(array);
    return 0;
}