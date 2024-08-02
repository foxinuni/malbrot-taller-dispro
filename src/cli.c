#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef CLI_TARGET_PNG
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#endif

#include "math/vector.h"
#include "math/complex.h"
#include "math/matrix.h"

#define WIDTH 4096
#define HEIGHT 2160 
#define THREADS 8

typedef struct {
    unsigned char* image;
    int max_iter;
    int offset, stride;
} malbrot_thread_data;

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

int malbrot_converges(compx c, int max_iter) {
    compx zn = compx_null();

    int i = 0;  
    while(compx_len(zn) <= 2.0 && (i++) < max_iter) {
        zn = compx_add(compx_sqr(zn), c);

        // derivative calculation
        #ifdef CLI_DERIVATIVE_OPTIMIZATION
        double modulus_derivative = abs(compx_scl(zn, 2.0).r);
        if (modulus_derivative > 1.0) {
            // adjust iter based on the derivative to speed up calculations
            i += log(abs(modulus_derivative));
            if (i >= max_iter) break;
        }
        #endif 

        ++i;
    }

    return i;
}

void* malbrot_thread(void* data_ptr) {
    malbrot_thread_data data = *(malbrot_thread_data*) data_ptr;

    for (int pixel = data.offset; pixel < data.stride; pixel++) {
        int x = pixel % WIDTH;
        int y = pixel / WIDTH;

        double real = pixel_to_complex(x, WIDTH, -2.5, 1.0);
        double imag = pixel_to_complex(y, HEIGHT, -1.0, 1.0);

        compx c = compx_new(real, imag);

        int iterations = malbrot_converges(c, data.max_iter);
        vec3 color = iteration_to_color(iterations, data.max_iter);

        data.image[(y * WIDTH + x) * 3 + 0] = color.r;
        data.image[(y * WIDTH + x) * 3 + 1] = color.g;
        data.image[(y * WIDTH + x) * 3 + 2] = color.b;
    }
}

void malbrot_trace(unsigned char* image, int max_iter) {
    pthread_t threads[THREADS];
    malbrot_thread_data data[THREADS];
    int pixels_per_thread = WIDTH * HEIGHT / THREADS;

    int pixel_count = 0;
    for (int i = 0; i < THREADS; i++) {
        malbrot_thread_data current = {
            image, max_iter,
            pixel_count, pixel_count + pixels_per_thread
        };

        // update state
        pixel_count += pixels_per_thread;
        data[i] = current;

        // create thread
        if (pthread_create(&threads[i], NULL, malbrot_thread, &data[i])) {
            printf("Error creating thread %d\n", i);
            exit(-1);
        }

        printf("Thread %d created (pid: %ld, offset: %d, stride %d)\n", i + 1, threads[i], current.offset, current.stride);
    }

    // wait for threads to finish
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("\rCompleted thread %d/%d ", i + 1, THREADS);
    }

    printf("DONE\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <output_image> <max_iterations>\n", argv[0]);
        return 1;
    }

    // parse arguments
    char* output_image = argv[1];
    int max_iter = atoi(argv[2]);

    // alocate some memory for the image
    unsigned char* image = malloc(WIDTH * HEIGHT * 3);

    // maldelbrot trace
    printf("Tracing mandelbrot set with %d iterations\n", max_iter);
    malbrot_trace(image, max_iter);

    // write image to disk
    printf("Writing image to %s...\n", output_image);

    #ifdef CLI_TARGET_PNG
    // write image to disk
    stbi_write_png(output_image, WIDTH, HEIGHT, 3, image, WIDTH * 3);
    #else
    FILE* file = fopen(output_image, "wb");
    if (!file) {
        printf("Failed to open file: %s\n", output_image);
        return 1;
    }

    // write ppm header
    fwrite("P6\n", 1, 3, file);
    fprintf(file, "%d %d\n", WIDTH, HEIGHT);
    fprintf(file, "%d\n", 255);
    fwrite(image, 1, WIDTH * HEIGHT * 3, file);

    // close file
    fclose(file);
    #endif

    // free memory
    free(image);

    return 0;
}