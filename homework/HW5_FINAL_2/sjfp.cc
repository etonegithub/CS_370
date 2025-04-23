#include <iostream>
#include <array>
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

const int max_processes_sjfp = 1000;
const int fields_sjfp = 7;

// function to compare 2 processes by burst time, breaking ties with arrival time, then process id
bool compare_burst_time(const array<int, fields_sjfp> a, const array<int, fields_sjfp> b) {
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

void sjfp(array<array<int, fields_sjfp>, max_processes_sjfp> data, int num_processes) {

    array<array<int, fields_sjfp>, max_processes_sjfp> processes = data;

    array<array<int, fields_sjfp>, max_processes_sjfp> ready_queue;
    int num_ready_processes = 0;

    // active process is ready_queue[active_process_index]
    int active_process_index = -1;
    bool hasActiveProcess = false;

    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    int t = 0;

    // while there are still processes to complete
    while (num_processes != num_ready_processes || num_ready_processes != active_process_index || hasActiveProcess) {
        
        // check for new process arrivals
        // while loop allows for simultaneous arrivals
        while (num_processes != num_ready_processes && processes[0][1] == t) {
            ready_queue[num_ready_processes] = processes[num_ready_processes];
            sort(ready_queue.begin(), ready_queue.begin() + num_ready_processes, compare_burst_time);
            num_ready_processes++;
        }

        if (num_ready_processes != active_process_index) {
            // if there is an active process and the shortest ready process is shorter than the active process
            if (hasActiveProcess && ready_queue[0][2] < ready_queue[active_process_index][2]) {
                // move active process back to ready queue and resort
                hasActiveProcess = false;
                sort(ready_queue.begin(), ready_queue.end(), compare_burst_time);
            }
            
            if (!hasActiveProcess) {
                // move shortest ready process to active process
                active_process_index++;
                hasActiveProcess = true;
            }
        }

        if (hasActiveProcess) {
            ready_queue[active_process_index][2]--;
            // give every waiting process +1 waiting time
            for (int i = 0; i < static_cast<int>(ready_queue.size()); i++) {
                ready_queue[i][4]++;
            }
            ready_queue[active_process_index][4]--;
            t++;
        }


        // check if active process is finished
        // NOTE: the program will still run if a process ends up with negative time somehow but that WILL mess up stats 
        if (ready_queue[active_process_index][2] <= 0) {
            // calculate total wait time for process and add to total
            total_waiting_time += ready_queue[active_process_index][4];

            // calculate turnaround time and add to total
            // turnaround time = waiting time + total burst duration
            ready_queue[active_process_index][5] = ready_queue[active_process_index][4] + ready_queue[active_process_index][6];
            total_turnaround_time += ready_queue[active_process_index][5];

            hasActiveProcess = false;
        }
    }

    // calculate average stats
    double avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_ready_processes);
    double avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_ready_processes);
    double throughput = static_cast<double>(num_ready_processes) / static_cast<double>(t);

    // output final stats
    printf("--- SJFP ---\n");
    printf("Total turnaround time: %.3d\n", total_turnaround_time);
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Total waiting time: %.3d\n", total_waiting_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
    
}