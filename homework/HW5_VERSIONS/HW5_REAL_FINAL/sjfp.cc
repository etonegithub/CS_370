#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*

process:
    processes are done in order of fastest to complete first. when
    a process arrives, we must check if its burst duration is shorter 
    than that of the currently active process. if so, make the new process
    the active process and sort the active process back into the ready
    queue based on the remaining time left in the process

needed:
    - list of processes sorted by arrival time
    - a list of processes that are ready to execute, sorted by burst duration
    - a single active process

each cycle:
    - check for new process arrivals
        - if yes, put new process in ready queue
        - sort ready queue (inefficient to do every time but whatever)
    - check if shortest ready process is shorter than remaining time in active process
        - if yes, put new process in active and active process in ready queue
                - calculate wait time for new active process
    - decrement remaining time in current process
    - increment time
    - check if process is finished
        - if yes, calculate turnaround time for process

notes:        
    - structure of data import
        - [0] Process ID
        - [1] Arrival Time
        - [2] Burst Duration
        - [3] Priority              // NOT USED IN SJF
        - [4] Waiting Time
        - [5] Turnaround Time
        - [6] Total Burst Duration
*/

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
    int num_processes = processes.size();
    // sort input by arrival time
    sort(processes.begin(), processes.end(), sjfp_compare_arrival_time);

    vector<vector<int>> ready_queue;

    vector<int> active_process;
    bool hasActiveProcess = false;

    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    int t = 0;

    /*
    for (vector<int> p : processes) {
        printf("%d,%d,%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
    }
    */

    // while there are still processes to complete
    while (!processes.empty() || !ready_queue.empty() || hasActiveProcess) {
        //printf("%d, %d, %d\n", processes.empty(), ready_queue.empty(), hasActiveProcess);
        /*
        printf("\n\nPROCESSES:\n");
        for (vector<int> p : processes) {
            printf("%d,%d,%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
        }

        printf("\n\nREADY QUEUE:\n");
        for (vector<int> p : ready_queue) {
            printf("%d,%d,%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
        }

        if (hasActiveProcess) {
            printf("\n\nACTIVE PROCESS: %d,%d,%d,%d,%d,%d,%d\n", active_process[0], active_process[1], active_process[2], active_process[3], active_process[4], active_process[5], active_process[6]);
        }
            */

        // check for new process arrivals
        // while loop allows for simultaneous arrivals
        while (!processes.empty() && processes[0][1] == t) {
            ready_queue.push_back(processes[0]);
            processes.erase(processes.begin());
            sort(ready_queue.begin(), ready_queue.end(), compare_burst_time);
        }

        if (!ready_queue.empty()) {
            // if there is an active process and the shortest ready process is shorter than the active process
            if (hasActiveProcess && compare_burst_time(ready_queue[0], active_process)) {
                // move active process back to ready queue and resort
                ready_queue.push_back(active_process);
                hasActiveProcess = false;
                sort(ready_queue.begin(), ready_queue.end(), compare_burst_time);
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
    printf("--- SJFP ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
    //printf("%d", t);
    
}

void sjfp2(vector<vector<int>> processes) {
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
    printf("--- SJFP2 ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}