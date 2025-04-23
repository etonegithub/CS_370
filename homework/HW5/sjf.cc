#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

/*
    needed:
    - list of processes sorted by arrival time
    - list of ARRIVED processes sorted by burst time
    - current time
    - active process
        - remaining time in active process

    each cycle:
    - check for new process arrivals 
        - if yes
            - sort new arrival into current process queue based on burst time
            - check if any waiting processes have a shorter burst time than the remaining time on active process
                - if yes
                    - switch out active processes
        - if no, continue
    - decrement remaining time on active process
        - if active process is finished
            - switch to next process in queue
        - if not
            - start loop again

    notes:
    - active process is just process in position 0 in arrived processes list
        - decrement 1 from burst time for position 0 process every time
        - when new process arrived, just sort arrived process list again by burst time
            - automatically switches active process based on least time
        - TODO consider how to save stats (waiting time, etc.) for active process when it gets switched out
            - maybe just update waiting time every cycle??? seems inefficient
    - how to calculate stats and link them to a process if the arrived_processes vector changes during execution
        - arrived_process vector is static before the iterator (num_completed_processes) bc a new process will never have 
            a burst time of 0 (surely TODO: CONSIDER EDGE CASE)
            - this means that the stats vector accurately maps onto arrived_processes before the iterator, and after the iterator goes
                through the entire vector, the stats vector should completely map to arrived_processes
                    - don't forget to subtract 1 when translating from pid to stats index cuz pid is 1-indexed
            - NVM doesn't work for calculating waiting time
        - just map stats vector to the process id of each process since order doesn't matter for the stat tracking

    */

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

/*
@Parameters
    - data: list of processes sorted by arrival time
@Return
    - none
*/
void sjf(vector<vector<int>> data) {
    // declare and initialize variables
    vector<vector<int>> processes = data; // deep copy of input vector<vector<int>> data
    int num_processes = processes.size();
    vector<vector<int>> arrived_processes; // list of arrived processes
    int num_arrived_processes = 0; // used to determine next process to arrive
    int num_completed_processes = 0; // used to determine active process
    
    /*
    - hold stats for each process: waiting time, turnaround time
    - initialized with num_process number of (-1, -1) vectors so they can be accessed in any order
        - this allows stats to map to process id
    */
    vector<vector<int>> stats(num_processes, vector<int>(2, -1)); 
    
    // stat calculation variables
    int total_waiting_time = 0;
    double avg_waiting_time;
    int total_turnaround_time = 0;
    double avg_turnaround_time;
    double throughput;
    
    int t = 0; // current time
    
    // simulate until all processes are completed
    while (num_completed_processes < num_processes) {
        /*
        - if no processes are queued, add the next arriving process to the queue
        - slight hack to save time
            - the further i go the more i feel like this is more trouble than it's worth jfkm
        */
        if (arrived_processes.empty()) {
            arrived_processes.push_back(processes[num_arrived_processes]); // vector is empty, so no need to sort
            
            // set new process's waiting time to 0
            stats[processes[num_arrived_processes][0] - 1][0] = 0;
            // write in new process's initial burst duration as its turnaround time in the stats vector
            // this is a surprise tool that will help us later  
            stats[processes[num_arrived_processes][0] - 1][1] = processes[num_arrived_processes][2];

            // calculate how much time was skipped and add to time counter and process waiting times
            int skipped_t = processes[num_arrived_processes][1] - t;
            t += skipped_t;
            for (int i = num_completed_processes + 1; i < num_arrived_processes; i++) {
                int pid = arrived_processes[i][0] - 1;
                stats[pid][0] += skipped_t;
            }

            num_arrived_processes++;
        }

        /*
        check if any processes need to arrive
        while loop used for the case of simultaneous arrivals
        */
        while (num_arrived_processes < num_processes && processes[num_arrived_processes][1] == t) {
            /*
            - lower_bound() included in <algorithm>
            - lower_bound() does a binary search based on custom comparator function "compare_burst_time"
            - returns an interator for the sorted position of input that can be used to insert new process
            - this is O(n) time (bc insert() is O(n)), whereas using sort() again is O(n log n)
                probably doesn't provide a noticable speed increase considering the other trash i have in here
                but this is more fun to use
            - TODO: CHECK IF THIS IS ALLOWED
            */
            vector<vector<int>>::iterator pos = lower_bound(
                arrived_processes.begin(),
                arrived_processes.end(),
                processes[num_arrived_processes],
                compare_burst_time
            );
            arrived_processes.insert(pos, processes[num_arrived_processes]);

            // set new process's waiting time to 0
            stats[processes[num_arrived_processes][0] - 1][0] = 0;
            // write in new process's initial burst duration as its turnaround time in the stats vector
            // this is a surprise tool that will help us later  
            stats[processes[num_arrived_processes][0] - 1][1] = processes[num_arrived_processes][2];

            num_arrived_processes++;
        }

        
        // decrement remaining time in active process
        arrived_processes[num_completed_processes][2]--;


        /*
        - gross hack to track waiting time
            - all arrived processes get 1 waiting time per cycle except for the active process
        - idk how else to do it bc there's no point where i actively switch the active process where i could calc
            the waiting time individually
        - starting at num_completed_processes + 1 means we don't affect processes that have already completed or the active process
        */
        // increment waiting time of all waiting processes by 1
        for (int i = num_completed_processes + 1; i < num_arrived_processes; i++) {
            int pid = arrived_processes[i][0] - 1;
            stats[pid][0]++;
        }

        // increment time
        t++;

        // check if active process is completed
        int active_pid = arrived_processes[num_completed_processes][0] - 1;
        if (arrived_processes[num_completed_processes][2] == 0) {
            // completed process's turnaround time = total burst duration (already in stats slot 1) + waiting time (in stats slot 0)
            stats[active_pid][1] = stats[active_pid][1] + stats[active_pid][0];
            
            // add to total stat counters
            total_waiting_time += stats[active_pid][0];
            total_turnaround_time += stats[active_pid][1];

            num_completed_processes++;
        }
    }

    // calculate final stats
    avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_processes);
    avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    throughput = static_cast<double>(num_processes) / t; // number of processes / total time spent

    // output final stats
    printf("--- SJF ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}