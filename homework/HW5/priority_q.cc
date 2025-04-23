#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

/*
notes:
    - should be the same as sjf but sorting by priority rather than remaining burst time
    - like i'm deadass copy and pasting my sjf
*/

// function to compare 2 processes by priority, breaking ties with burst time, then process id
// could add arrival time right before process id as tiebreaker but that seems a bit extra
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
    
    // if priority time is equal, compare burst time
    if (a[3] == b[3]) {
        // if arrival time is equal, compare process id
        if (a[2] == b[2]) {
            return a[0] < b[0];
        }
        else {
            return a[2] < b[2];
        }
    }
    else {
        return a[3] < b[3];
    }
}

/*
@Parameters
    - data: list of processes sorted by arrival time
@Return
    - none
*/
void priority_q(vector<vector<int>> data) {
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
        // if no processes are queued, add the next arriving process to the queue
        // slight hack to save time
        if (arrived_processes.empty()) {
            // since there are no processes in queue, there's no need to sort or actually simulate until arrival
            arrived_processes.push_back(processes[num_arrived_processes]);
            
            // set new process's waiting time to 0
            stats[processes[num_arrived_processes][0] - 1][0] = 0;
            // write in new process's initial burst duration as its turnaround time in the stats vector
            // this is a surprise tool that will help us later (maybe)
            stats[processes[num_arrived_processes][0] - 1][1] = processes[num_arrived_processes][2];

            // set t to arrival time of new process
            t = processes[num_arrived_processes][1];

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
                compare_priority
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

        // increment time
        t++;
    }

    // calculate final stats
    avg_waiting_time = static_cast<double>(total_waiting_time) / static_cast<double>(num_processes);
    avg_turnaround_time = static_cast<double>(total_turnaround_time) / static_cast<double>(num_processes);
    throughput = static_cast<double>(num_processes) / t; // number of processes / total time spent

    // output final stats
    printf("--- Priority ---\n");
    printf("Average Turnaround Time: %.3f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.3f\n", avg_waiting_time);
    printf("Throughput: %.3f\n", throughput);
}