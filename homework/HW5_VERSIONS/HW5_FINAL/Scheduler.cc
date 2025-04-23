#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "fcfs.cc"
#include "sjf.cc"
#include "priority.cc"

using namespace std;

// function to compare 2 processes by arrival time, breaking ties with process id
bool compare_arrival_time(vector<int> a, vector<int> b) {
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
    vector<vector<int>> data;

    if (argc < 2) {
        return 0;
    }

    infile.open(argv[1]);
    //infile.open("input.csv"); // TODO: TEMP HACK FOR EASY DEV, CHANGE BEFORE FINAL
    while(getline(infile, line)) {
        stringstream ss (line);
        vector<int> data_row;
        while(getline(ss, value, ',')) {
            data_row.push_back(stoi(value));
        }

        // add 2 extra fields to each process for Waiting Time and Turnaround Time
        data_row.push_back(0);
        data_row.push_back(0);

        // add copy of burst duration field to each process (used for preemptive algorithms)
        data_row.push_back(data_row[2]);

        data.push_back(data_row);
    }
    //printf("Done reading csv.");

    // sort processes by arrival time
    /*
    - sort() is included from <algorithm> and used for ease of implementation
    - manually writing a sorting algorithm for the processes seems a bit adjacent to actually understanding CPU Scheduling 
        so i think it's fine since I have to write a custom comparator function anyway so I understand the sorting criteria being used
    */
    sort(data.begin(), data.end(), compare_arrival_time);

    fcfs(data);

    printf("\n");

    sjf(data);

    printf("\n");

    priority(data);    
}
