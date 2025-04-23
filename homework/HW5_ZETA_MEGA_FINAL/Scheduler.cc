#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "fcfs.cc"
#include "sjfp.cc"
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
    // sort data by arrival time
    sort(data.begin(), data.end(), compare_arrival_time);
    return data;
}

int main(int, char** argv) {


    vector<vector<int>> fcfs_data = read_csv(argv[1]);
    if (!fcfs_data.empty()) {
       fcfs(fcfs_data);
       printf("\n");
    } else {
        printf("There are no processes.\n");
    }
    
    vector<vector<int>> sjfp_data = read_csv(argv[1]);
    if (!sjfp_data.empty()) {
        sjfp(sjfp_data);
        printf("\n");
    } else {
        printf("There are no processes.\n");
    }

    vector<vector<int>> prio_data = read_csv(argv[1]);;
    if (!prio_data.empty()) {
        priority(prio_data);
        printf("\n");
    } else {
        printf("There are no processes.\n");
    }   
}
