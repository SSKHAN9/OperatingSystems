//
// Created by saad on 1/18/23.
//

#include <stdio.h>

void termination(int n) {
    int iterations = 0;

    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
        iterations++;
    }

    printf("Terminated after %d iterations for n = %d.\n", iterations, n);
}

int main() {
    int input;
    printf("Enter a positive integer: ");
    scanf("%d", &input);

    termination(input);

    return 0;
}
