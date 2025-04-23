#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Not enough arguments\n");
        return EXIT_FAILURE;
    }

    printf("[./profiler] checking pid %s\n", argv[1]);

    // create filepaths
    char statfilepath[256] = "/proc/";
    strcat(statfilepath, argv[1]);
    strcat(statfilepath, "/stat");
    
    char statmfilepath[256] = "/proc/";
    strcat(statmfilepath, argv[1]);
    strcat(statmfilepath, "/statm");


    // read files
    FILE *statfile = fopen(statfilepath, "r");
    FILE *statmfile = fopen(statmfilepath, "r");

    char statline[256];
    char statmline[256];

    if (statfile == NULL || statmfile == NULL) {
        printf("That process does not exist.\n");
        return EXIT_FAILURE;
    }

    if (fgets(statline, sizeof(statline), statfile) == NULL || fgets(statmline, sizeof(statmline), statmfile) == NULL) {
        fclose(statfile);
        fclose(statmfile);
        printf("That process could not be read.\n");
        return EXIT_FAILURE;
    }

    // stat fields
    char *pid = strtok(statline, " "); // field 1: Process PID
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
    double utimesec = atof(utime) / sysconf(_SC_CLK_TCK); // convert to time in seconds
    char *stime = strtok(NULL, " "); // field 15: Time spent in Kernel mode (in clock ticks)
    double stimesec = atof(stime) / sysconf(_SC_CLK_TCK); // convert to time in seconds


    // statm fields
    char *size = strtok(statmline, " "); // field 1: Total program size (same as Virtual Memory size)
    char *resident = strtok(NULL, " "); // field 2: # of resident pages
    char *shared = strtok(NULL, " "); // field 3: # resident shared pages


    // print stats
    printf("Executable:     %s\n", comm);
    printf("ppid:           %s\n", ppid);
    printf("pgrp:           %s\n", pgrp);
    printf("State:          %s\n", state);
    printf("User mode:      %0.3f sec\n", utimesec);
    printf("Kernel Mode:    %0.3f sec\n", stimesec);
    printf("Virtual memory: %s bytes\n", size);
    printf("Resident pages: %s\n", resident);
    printf("Shared pages:   %s\n", shared);
    printf("Page faults:    %s\n", majflt);

    return EXIT_SUCCESS;
}