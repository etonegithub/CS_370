#include <iostream>
#include <fstream>
#include "fcfs.cc"
#include "sjf.cc"
#include "priority_q.cc"

// allowed?
#include <sstream>
#include <vector>
#include <algorithm>

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

int main(int, char**) {

    // Read from CSV
    // Structure: Process ID, Arrival Time, Burst Duration, Priority
    ifstream infile;
    string line;
    string value;
    /*
    - vector is used as a dynamically-sized array since there seems to be no restriction on includes (TODO: ASK IF THIS IS OKAY)
    - vector<vector<int>> is a dynamic 2d array of ints
    */
    vector<vector<int>> data;

    //infile.open(argv[1]);
    infile.open("input.csv"); // TODO: TEMP HACK FOR EASY DEV, CHANGE BEFORE FINAL
    while(getline(infile, line)) {
        stringstream ss (line);
        vector<int> data_row;
        while(getline(ss, value, ',')) {
            data_row.push_back(stoi(value));
        }
        data.push_back(data_row);
    }
    printf("Done reading csv.");

    // print out processes (TODO: FOR DEBUG, REMOVE)
    for (const auto& p : data) {
        cout << "ID: " << p[0] << ", Arrival: " << p[1] 
             << ", Burst: " << p[2] << ", Priority: " << p[3] << endl;
    }

    // sort processes by arrival time
    /*
    - sort() is included from <algorithm> and used for ease of implementation
    - manually writing a sorting algorithm for the processes seems a bit adjacent to actually understanding CPU Scheduling 
        so i think it's fine since I have to write a custom comparator function anyway so I understand the sorting criteria being used
        (TODO: ASK IF THIS IS OKAY)
    */
    sort(data.begin(), data.end(), compare_arrival_time);

    fcfs(data);

    sjf(data);

    //priority_q(data);
}
