#include "complex.h"
#include <math.h>
#include <stdio.h>

compx compx_null() {
    return compx_new(0, 0);
}

compx compx_new(double r, double i) {
    compx compl = { r, i };
    return compl;
}

compx copmx_cpy(compx input) {
    return input;
}

compx compx_scl(compx input, double factor) {
    return compx_new(input.r * factor, input.i * factor);
}

compx compx_add(compx v1, compx v2) {
    return compx_new(v1.r + v2.r, v1.i + v2.i);
}

compx compx_sub(compx v1, compx v2) {
    return compx_new(v1.r - v2.r, v1.i - v2.i);
}

compx compx_mul(compx v1, compx v2) {
    return compx_new(v1.r * v2.r - v1.i * v2.i, v1.r * v2.i + v2.r * v1.i);
}

compx compx_div(compx v1, compx v2) {
    return compx_scl(compx_mul(v1, compx_conj(v2)), 1.0f/(v2.r * v2.r + v2.i * v2.i));
}

compx compx_sqr(compx input) {
    return compx_mul(input, input);
}

compx compx_rot(compx input) {
    return compx_mul(input, compx_new(0, 1));
}

compx compx_conj(compx input) {
    return compx_new(input.r, -input.i);
}

double compx_len(compx input) {
    return sqrt(input.r * input.r + input.i * input.i);
}

double compx_ang(compx input) {
    return atan2(input.i, input.r);
}

unsigned char compx_eq(compx v1, compx v2) {
    return v1.r == v2.r && v1.i == v2.i;
}

int compx_print(compx input) {
    return printf("%0.2f %c i%0.2f\n", input.r, (input.i >= 0) ? '+' : '-', fabs(input.i));
}