#include <stdio.h>
#include <stdlib.h>

double factorial(int n) {
    double fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

double power(double base, int exponent) {
    double result = 1;
    for (int i = 1; i <= exponent; i++) {
        result *= base;
    }
    return result;
}

double degree_to_radian(double degree) {
    return degree * (3.14159265358979323846 / 180);
}

double sin_taylor_series(double x, int terms) {
    double sine = 0;

    for (int i = 0; i < terms; i++) {
        double term = power(-1, i) * power(x, 2 * i + 1) / factorial(2 * i + 1);
        sine += term;
    }

    return sine;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <angle in degrees>\n", argv[0]);
        return 1;
    }

    double angle_degrees = atof(argv[1]);
    double angle_radians = degree_to_radian(angle_degrees);

    int terms = 200; // Increase this value for better precision
    double sine = sin_taylor_series(angle_radians, terms);

    printf("sin(%f) = %f\n", angle_degrees, sine);

    return 0;
}