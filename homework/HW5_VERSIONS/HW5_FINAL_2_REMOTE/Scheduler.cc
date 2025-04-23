#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "fcfs.cc"
#include "sjfp.cc"
#include "priority.cc"

using namespace std;

vector<vector<int>> read_csv(char* filename) {
    // Read from CSV
    // Structure: Process ID, Arrival Time, Burst Duration, Priority
    ifstream infile;
    string line;
    string value;

    //vector<vector<int>> is a dynamic 2d array of ints
    vector<vector<int>> data;

    infile.open(filename);
    while(getline(infile, line)) {
        stringstream ss (line);
        vector<int> data_row;
        while(getline(ss, value, ',')) {
            data_row.push_back(stoi(value));
        }

        // check that arrival time is >= 0 and that burst duration is > 0 and that 1 <= priority <= 50
        if (data_row[1] >= 0 && data_row[2] > 0 && data_row[3] >= 1 && data_row[3] <= 50) {
            data.push_back(data_row);
        } else {
            printf("Data row has bad inputs.\n");
        }
    }
    return data;
}

vector<vector<int>> read_csv_debug(string filename) {
    // Read from CSV
    // Structure: Process ID, Arrival Time, Burst Duration, Priority
    ifstream infile;
    string line;
    string value;

    //vector<vector<int>> is a dynamic 2d array of ints
    vector<vector<int>> data;

    infile.open(filename);
    while(getline(infile, line)) {
        stringstream ss (line);
        vector<int> data_row;
        while(getline(ss, value, ',')) {
            data_row.push_back(stoi(value));
        }

        // check that arrival time is >= 0 and that burst duration is > 0 and that 1 <= priority <= 50
        if (data_row[1] >= 0 && data_row[2] > 0 && data_row[3] >= 1 && data_row[3] <= 50) {
            data.push_back(data_row);
        } else {
            printf("Data row has bad inputs.\n");
        }
    }
    return data;
}

int main(int, char** argv) {
    bool debug = false;
    string debug_filename = "input2.csv";
    // sort processes by arrival time
    /*
    - sort() is included from <algorithm> and used for ease of implementation
    - manually writing a sorting algorithm for the processes seems a bit adjacent to actually understanding CPU Scheduling 
    so i think it's fine since I have to write a custom comparator function anyway so I understand the sorting criteria being used
    */
    //string filename2 = "input2.csv";
   
    vector<vector<int>> fcfs_data;
    if (!debug) {
        printf("Prod\n");
        fcfs_data = read_csv(argv[1]);
    } else {
        printf("Debug\n");
        fcfs_data = read_csv_debug(debug_filename);
    }
    if (!fcfs_data.empty()) {
       fcfs(fcfs_data);
       printf("\n");
    } else {
        printf("There are no processes.\n");
    }
    
    vector<vector<int>> sjfp_data;
    if (!debug) {
        sjfp_data = read_csv(argv[1]);
    } else {
        sjfp_data = read_csv_debug(debug_filename);
    }
    if (!sjfp_data.empty()) {
        sjfp(sjfp_data);
        printf("\n");
    } else {
        printf("There are no processes.\n");
    }

    vector<vector<int>> prio_data;
    if (!debug) {
        prio_data = read_csv(argv[1]);
    } else {
        prio_data = read_csv_debug(debug_filename);
    }
    if (!prio_data.empty()) {
        priority(prio_data);
        printf("\n");
    } else {
        printf("There are no processes.\n");
    }   
}
