#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// function to compare 2 processes by arrival time, breaking ties with process id
bool sjfp_compare_arrival_time(vector<int> a, vector<int> b) {
    if (a[1] == b[1]) {
        return a[0] < b[0];
    }
    else {
        return a[1] < b[1];
    }
}

// function to compare 2 processes by burst time, breaking ties with arrival time, then process id
bool compare_burst_time(vector<int> a, vector<int> b) {
    // if burst time is equal, compare arrival time
    if (a[2] == b[2]) {
        // if arrival time is equal, compare process id
        if (a[1] == b[1]) {
            return a[0] < b[0];
        }
        else {
            return a[1] < b[1];
        }
    }
    else {
        return a[2] < b[2];
    }
}

void sjfp(vector<vector<int>> processes) {
    // total number of processes inputted
    int num_processes = processes.size();

    sort(processes.begin(), processes.end(), sjfp_compare_arrival_time);

    // index of the next process to arrive
    int next_arrival_index = 0;
    // index of the active process
    int active_index = 0;

    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int time = 0;
    
    // while not all processes have completed
    while (active_index < num_processes) {

        // while there is a next process that is set to arrive at current time
        while (next_arrival_index < num_processes && processes[next_arrival_index][1] == time) {
            next_arrival_index++;
            // sort arrived processes by burst time
            sort(processes.begin() + active_index, processes.begin() + next_arrival_index, compare_burst_time);
        }
        

        // increment total wait time for each waiting process
        for (int i = active_index + 1; i < next_arrival_index; i++) {
            total_wait_time++;
        }

        if (active_index < next_arrival_index) {
            processes[active_index][2]--;
        }

        // increment global time
        time++;

        if (processes[active_index][2] <= 0) {
            // turnaround time = current time - arrival time
            int turnaround_time = time - processes[active_index][1];
            total_turnaround_time += turnaround_time;

            active_index++;
        }
    }

    // calculate average stats
    double avg_waiting_time = static_cast<double>(total_wait_time) / static_cast<double>(num_processes);
    double avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    double throughput = static_cast<double>(num_processes) / static_cast<double>(time);

    // output final stats
    printf("--- SJFP ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}