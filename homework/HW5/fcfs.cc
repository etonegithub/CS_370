#include <iostream>

#include <vector>

using namespace std;

void fcfs(vector<vector<int>> data) {

    // declare and initialize variables
    vector<vector<int>> processes = data; // deep copy of input vector<vector<int>> data
    vector<vector<int>> stats; // hold stats for each process: waiting time, turnaround time
    int num_processes = processes.size();

    int total_waiting_time = 0;
    double avg_waiting_time;

    int total_turnaround_time = 0;
    double avg_turnaround_time;

    double throughput;

    int t = 0;

    /*
    needed:
        - vector of processes sorted by arrival time
        - vector of equal size to above vector to store stats for each process
        - current time counter
    plan:
        - iterate through vector of process pre-sorted by arrival time
            - calculate stats (waiting time, turnaround time) for that process and 
                push into stats vector
            - iterate current time by current process's burst duration
        - calculate average of stats
    notes:
        - pretty straightforward
    */

    // print out processes (TODO: FOR DEBUG, REMOVE)
    for (const auto& p : processes) {
        cout << "ID: " << p[0] << ", Arrival: " << p[1] 
             << ", Burst: " << p[2] << ", Priority: " << p[3] << endl;
    }

    // simulate processing by iterating through processes vector
    for (int i = 0; i < num_processes; i++) {
        /*
        waiting time (avg) -> start processing time - arrival time (for each p)
        turnaround time (avg) -> waiting time + burst duration (for each p)
        throughput -> num tasks completed / total time spent
        */
        vector<int> stat; // waiting time, turnaround time

        // set time to max of time and process arrival time (prevents negative waiting time)
        if (t < processes[i][1]) {
            t = processes[i][1];
        }

        int waiting_t = t - processes[i][1]; // current time - process arrival time
        stat.push_back(waiting_t);
        total_waiting_time += waiting_t;

        int turnaround_t = waiting_t + processes[i][2]; // waiting time + burst duration
        stat.push_back(turnaround_t);
        total_turnaround_time += turnaround_t;

        stats.push_back(stat);
        t += processes[i][2]; // add burst duration of current process to total time before going to next process
    }

    // calculate final stats
    avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_processes);
    avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    throughput = static_cast<double>(num_processes) / t; // number of processes / time spent

    // output final stats
    printf("--- FCFS ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}