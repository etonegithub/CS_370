#ifndef SORT_H
#define SORT_H

void sort(int arr[], int size) {
    
    // selection sort
    for (int i = 0; i < size - 1; i++) {
        int curr_min = i;
        for (int j = i + 1; j < size - 1; j++) {
            if (arr[j] < arr[curr_min]) {
                curr_min = j;
            }
        }
        int tmp = arr[curr_min];
        arr[curr_min] = arr[i];
        arr[i] = tmp;
    }
}

#endif