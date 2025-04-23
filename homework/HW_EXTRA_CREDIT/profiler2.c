#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// does not validate file, do so before use
void read_file(FILE *file, char *line) {
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("[read_file] Failed to read files\n");
        return;
    }
}

int print_stats(char* stat, char* statm) {

    /*
    meaning, order, and type of stats in /proc/[pid]/stat can be read in:
    https://man7.org/linux/man-pages/man5/proc_pid_stat.5.html

    Looking for:
    /proc/[pid]/stat
        - X Executable name
        - X Process state
        - Number of page faults
        - X Parent Process ID
        - X Process Group ID
        - Time spent in Kernel mode
        - Time spent in User mode
    
    /proc/[pid]/statm
        - Virtual memory size
        - Resident pages
        - Shared pages
    */

    // stat fields
    char *pid = strtok(stat, " "); // field 1: Process PID
    char *comm = strtok(NULL, " "); // field 2: Executable name
    char *state = strtok(NULL, " "); // field 3: Process state
    char *ppid = strtok(NULL, " "); // field 4: Parent PID
    char *pgrp = strtok(NULL, " "); // field 5: Group PID

    // skip fields 6-11
    for (int i = 0; i < 6; i++) {
        strtok(NULL, " ");
    }

    char *majflt = strtok(NULL, " "); // field 12: # page faults
    strtok(NULL, " "); // skip field 13
    char *utime = strtok(NULL, " "); // field 14: Time spent in User mode (in clock ticks)
    char *stime = strtok(NULL, " "); // field 15: Time spent in Kernel mode (in clock ticks)

    // statm fields
    char *size = strtok(statm, " "); // field 1: Total program size (same as Virtual Memory size)
    strtok(NULL, " "); // skip field 2
    char *resident = strtok(NULL, " "); // field 3: # of resident pages
    char *shared = strtok(NULL, " "); // field 4: # resident shared pages


    // stat fields
    printf("PID: %s\n", pid);
    printf("Executable: %s\n", comm);
    printf("ppid: %s\n", ppid);
    printf("pgrp: %s\n", pgrp);
    printf("State: %s\n", state);
    printf("User mode: %s\n", utime);
    printf("Kernel Mode: %s\n", stime);
    printf("Page faults: %s\n", majflt);

    // statm fields
    printf("Virtual memory: %s\n", size);
    printf("Resident pages: %s\n", resident);
    printf("Shared pages: %s\n", shared);
}

int main2() {
    FILE *statfile = fopen("/proc/1/stat", "r");
    FILE *statmfile = fopen("/proc/1/statm", "r");

    if (statfile == NULL || statmfile == NULL) {
        return EXIT_FAILURE;
    }

    char statline[256];
    char statmline[256];

    read_file(statfile, statline);
    read_file(statmfile, statmline);

    if (statline == NULL || statmline == NULL) {
        printf("[main] Failed to read files\n");
        return EXIT_FAILURE;
    }

    print_stats(statline, statmline);
    
    return EXIT_SUCCESS;
}