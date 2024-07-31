#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
    double r, i;
} compx;

// operations
compx compx_null();
compx compx_new(double r, double i);
compx copmx_cpy(compx input);
compx compx_scl(compx input, double factor);
compx compx_add(compx v1, compx v2);
compx compx_sub(compx v1, compx v2);
compx compx_mul(compx v1, compx v2);
compx compx_div(compx v1, compx v2);
compx compx_sqr(compx input);
compx compx_rot(compx input);
compx compx_conj(compx input);
double compx_len(compx input);
double compx_ang(compx input);

// comp
unsigned char compx_eq(compx v1, compx v2);

int compx_print(compx input);

#endif