#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "math/vector.h"
#include "math/complex.h"
#include "math/matrix.h"

#define WIDTH 15360
#define HEIGHT 8640

int malbrot_converges(compx c, int max_iter) {
    compx zn = compx_null();

    int i = 0;
    while(compx_len(zn) <= 2.0 && (i++) < max_iter) {
        zn = compx_add(compx_sqr(zn), c);
    }

    return i;
}

vec3 iteration_to_color(int iteration, int max_iter) {
    double t = (double)iteration / max_iter;
    if (t >= 1.0) {
        return vec3_new(0, 0, 0);
    }

    unsigned char r = (unsigned char)(9 * (1 - t) * t * t * t * 255);
    unsigned char g = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255);
    unsigned char b = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
   
    return vec3_new(r, g, b);
}

double pixel_to_complex(int pixel, int size, double min, double max) {
    return min + (max - min) * ((double) pixel / (double) size);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <output_image> <max_iterations>\n", argv[0]);
        return 1;
    }

    char* output_image = argv[1];
    int max_iter = atoi(argv[2]);

    unsigned char* image = malloc(WIDTH * HEIGHT * 3);
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double real = pixel_to_complex(x, WIDTH, -2.5, 1.0);
            double imag = pixel_to_complex(y, HEIGHT, -1.0, 1.0);

            compx c = compx_new(real, imag);

            int iterations = malbrot_converges(c, max_iter);
            vec3 color = iteration_to_color(iterations, max_iter);

            image[(y * WIDTH + x) * 3 + 0] = color.r;
            image[(y * WIDTH + x) * 3 + 1] = color.g;
            image[(y * WIDTH + x) * 3 + 2] = color.b;
        }
    }

    stbi_write_png(output_image, WIDTH, HEIGHT, 3, image, WIDTH * 3);

    free(image);

    return 0;
}