#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
notes:
    - should be the same as sjf but sorting by prio instead of burst       
    - structure of data import
        - [0] Process ID
        - [1] Arrival Time
        - [2] Burst Duration
        - [3] Priority              
        - [4] Waiting Time
        - [5] Turnaround Time
        - [6] Total Burst Duration
*/

// function to compare 2 processes by priority, breaking ties with burst time, then arrival
// could add process id as tiebreaker instead of arrival
bool compare_priority(vector<int> a, vector<int> b) {
    
    /*
    - if one process has a remaining burst time of 0, it is always sorted lower
    - essentially a hack so i don't have to change this much from my sjf algorithm
    - if statement uses a XOR operator
    */
   /*
    if ((a[2] == 0) != (b[2] == 0)) {
        return a[2] < b[2];
    }
    */

    
    if (a[2] == 0 && b[2] != 0) {
        return false; // `a` should be sorted after `b`
    }
    if (b[2] == 0 && a[2] != 0) {
        return true; // `b` should be sorted after `a`
    }
    
    
    // if priority time is equal, compare arrival time
    if (a[3] == b[3]) {
        // if arrival time is equal, compare burst time
        if (a[1] == b[1]) {
            return a[2] < b[2];
        }
        else {
            return a[1] < b[1];
        }
    }
    else {
        return a[3] < b[3];
    }
}

void priority(vector<vector<int>> data) {
    vector<vector<int>> processes = data;
    int num_processes = processes.size();

    vector<vector<int>> ready_queue;

    vector<int> active_process;
    bool hasActiveProcess = false;

    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    int t = 0;

    // while there are still processes to complete
    while (!processes.empty() || !ready_queue.empty() || hasActiveProcess) {
        
        // check for new process arrivals
        // while loop allows for simultaneous arrivals
        while (!processes.empty() && processes[0][1] == t) {
            ready_queue.push_back(processes[0]);
            processes.erase(processes.begin());
            sort(ready_queue.begin(), ready_queue.end(), compare_priority);
        }

        if (!ready_queue.empty()) {
            // if there is an active process and the lowest prio ready process has lower prio than active
            if (hasActiveProcess && ready_queue[0][3] < active_process[3]) {
                // move active process back to ready queue and resort
                ready_queue.push_back(active_process);
                hasActiveProcess = false;
                sort(ready_queue.begin(), ready_queue.end(), compare_priority);
            }
            
            if (!hasActiveProcess) {
                // move shortest ready process to active process
                active_process = ready_queue[0];
                ready_queue.erase(ready_queue.begin());
                hasActiveProcess = true;
            }
        }

        if (hasActiveProcess) {
            active_process[2]--;
        }
        t++;

        // give every waiting process +1 waiting time
        for (int i = 0; i < static_cast<int>(ready_queue.size()); i++) {
            ready_queue[i][4]++;
        }

        // check if active process is finished
        // NOTE: the program will still run if a process ends up with negative time somehow but that WILL mess up stats 
        if (active_process[2] <= 0) {
            // calculate total wait time for process and add to total
            total_waiting_time += active_process[4];

            // calculate turnaround time and add to total
            // turnaround time = waiting time + total burst duration
            active_process[5] = active_process[4] + active_process[6];
            total_turnaround_time += active_process[5];

            hasActiveProcess = false;
        }
    }

    // calculate average stats
    double avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_processes);
    double avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    double throughput = static_cast<double>(num_processes) / static_cast<double>(t);

    // output final stats
    printf("--- Priority ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}