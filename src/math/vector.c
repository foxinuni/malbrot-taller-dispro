#include "vector.h"
#include <math.h>
#include <stdio.h>

vec3 vec3_null() {
    return vec3_new(0, 0, 0);
}

vec3 vec3_new(double x, double y, double z) {
    vec3 vect = { x, y, z };
    return vect;
}

vec3 vec3_cpy(vec3 input) {
    return input;
}

vec3 vec3_scl(vec3 input, double factor) {
    for (unsigned char i = 0; i < 3; i++) {
        input.e[i] *= factor;
    }

    return input;
}

vec3 vec3_neg(vec3 input) {
    for (unsigned char i = 0; i < 3; i++) {
        input.e[i] *= -1.0;
    }

    return input;
}

vec3 vec3_add(vec3 v1, vec3 v2) {
    for (unsigned char i = 0; i < 3; i++) {
        v1.e[i] += v2.e[i];
    }
    
    return v1;
}

vec3 vec3_sub(vec3 v1, vec3 v2) {
    return vec3_add(v1, vec3_neg(v2));
}

vec3 vec3_mul(vec3 v1, vec3 v2) {
    for (unsigned char i = 0; i < 3; i++) {
        v1.e[i] *= v2.e[i];
    }
    
    return v1;
}

vec3 vec3_sqrt(vec3 input) {
    for (unsigned char i = 0; i < 3; i++) {
        input.e[i] = sqrt(input.e[i]);
    }
    
    return input;
}

vec3 vec3_unit(vec3 input) {
    return vec3_scl(input, 1.0/vec3_len(input));
}

vec3 vec3_cross(vec3 v1, vec3 v2) {
    vec3 vect = { (v1.j * v2.k - v2.j * v1.k), -(v1.i * v2.k - v2.i * v1.k), (v1.i * v2.j - v2.i * v1.j) };
    return vect;
}

double vec3_dot(vec3 v1, vec3 v2) {
    double res = 0;
    vec3 mul = vec3_mul(v1, v2);
    
    for (unsigned char i = 0; i < res; i++) {
        res += mul.e[i];
    }

    return res;
}

double vec3_len(vec3 input) {
    return sqrt(vec3_dot(input, input));
}

unsigned char vec3_eq(vec3 v1, vec3 v2) {
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

int vec3_print(vec3 input) {
    return printf("[%0.2f, %0.2f, %0.2f]\n", input.x, input.y, input.z);
}