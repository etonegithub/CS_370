const int max_processes = 1000;
const int fields = 7;

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>

#include "fcfs.cc"
#include "sjfp.cc"
#include "priority.cc"

using namespace std;


// function to compare 2 processes by arrival time, breaking ties with process id
bool compare_arrival_time(const array<int, fields> a, const array<int, fields> b) {
    if (a[1] == b[1]) {
        return a[0] < b[0];
    }
    else {
        return a[1] < b[1];
    }
}

int main(int argc, char** argv) {

    // Read from CSV
    // Structure: Process ID, Arrival Time, Burst Duration, Priority
    ifstream infile;
    string line;
    string value;
    /*
    - vector<vector<int>> is a dynamic 2d array of ints
    */
    array<array<int, fields>, max_processes> data;
    int num_processes = 0;

    if (argc < 2) {
        printf("No input file.");
        return 0;
    }

    infile.open(argv[1]);
    //infile.open("input.csv"); // TODO: TEMP HACK FOR EASY DEV, CHANGE BEFORE FINAL
    while(getline(infile, line)) {
        stringstream ss (line);
        int col = 0;
        while(getline(ss, value, ',')) {
            data[num_processes][col] = stoi(value);
            col++;
        }

        // add 2 extra fields to each process for Waiting Time and Turnaround Time
        data[num_processes][4] = 0;
        data[num_processes][5] = 0;

        // add copy of burst duration field to each process (used for preemptive algorithms)
        data[num_processes][6] = data[num_processes][2];

        num_processes++;
    }
    //printf("Done reading csv.");

    // sort processes by arrival time
    /*
    - sort() is included from <algorithm> and used for ease of implementation
    - manually writing a sorting algorithm for the processes seems a bit adjacent to actually understanding CPU Scheduling 
        so i think it's fine since I have to write a custom comparator function anyway so I understand the sorting criteria being used
    */
    sort(data.begin(), data.begin() + num_processes, compare_arrival_time);

    for (int i = 0; i < num_processes; i++) {
        printf("%d, %d, %d, %d, %d, %d, %d\n", data[i][0], data[i][1], data[i][2], data[i][3], data[i][4], data[i][5], data[i][6]);
    }

    fcfs(data, num_processes);

    printf("\n");

    sjfp(data, num_processes);

    //printf("\n");

    //priority(data);    
}
