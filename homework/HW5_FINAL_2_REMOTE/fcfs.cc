#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
notes:        
    - structure of data import
        - [0] Process ID
        - [1] Arrival Time
        - [2] Burst Duration
        - [3] Priority              // NOT USED IN FCFS
*/

// function to compare 2 processes by arrival time, breaking ties with process id
bool fcfs_compare_arrival_time(vector<int> a, vector<int> b) {
    if (a[1] == b[1]) {
        return a[0] < b[0];
    }
    else {
        return a[1] < b[1];
    }
}

void fcfs(vector<vector<int>> processes) {
    int num_processes = processes.size();
    
    // sort input by arrival time
    sort(processes.begin(), processes.end(), fcfs_compare_arrival_time);

    // time counter
    int t = 0;

    // total stats
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < num_processes; i++) {
        // calculate waiting time for active process
        int waiting_time = t - processes[i][1];
        total_waiting_time += waiting_time;

        // iterate time counter by process burst duration
        t += processes[i][2];

        // calculate turnaround time for active process
        int turnaround_time = t - processes[i][1];
        total_turnaround_time += turnaround_time;
    }

    // calculate average stats
    double avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_processes);
    double avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    double throughput = static_cast<double>(num_processes) / static_cast<double>(t);

    // output final stats
    printf("--- FCFS ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}