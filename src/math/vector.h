#ifndef VECTOR_H
#define VECTOR_H

typedef union {
    double e[3];
    
    struct {
        double x, y, z;
    };

    struct {
        double i, j, k;
    };

    struct {
        double r, g, b;
    };
} vec3;

// operations
vec3 vec3_null();
vec3 vec3_new(double x, double y, double z);
vec3 vec3_cpy(vec3 input);
vec3 vec3_scl(vec3 input, double factor);
vec3 vec3_neg(vec3 input);
vec3 vec3_add(vec3 v1, vec3 v2);
vec3 vec3_sub(vec3 v1, vec3 v2);
vec3 vec3_mul(vec3 v1, vec3 v2);
vec3 vec3_sqrt(vec3 input);
vec3 vec3_unit(vec3 input);
vec3 vec3_cross(vec3 v1, vec3 v2);
double vec3_dot(vec3 v1, vec3 v2);
double vec3_len(vec3 input);

// comparisons
unsigned char vec3_eq(vec3 v1, vec3 v2);

int vec3_print(vec3 input);

#endif