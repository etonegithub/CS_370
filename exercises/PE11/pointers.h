#ifndef POINTERS_H
#define POINTERS_H

float* new_float() {
    return (float*) malloc(sizeof(float));
}

void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

#endif