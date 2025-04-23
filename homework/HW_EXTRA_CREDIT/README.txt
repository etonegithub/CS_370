Run instructions:

> make all
> ./profiler [target pid]

meaning, order, and type of stats in /proc/[pid]/stat can be read in:
https://man7.org/linux/man-pages/man5/proc_pid_stat.5.html
https://man7.org/linux/man-pages/man5/proc_pid_statm.5.html 

Looking for:
/proc/[pid]/stat
    - Executable name
    - Process state
    - Number of page faults
    - Parent Process ID
    - Process Group ID
    - Time spent in Kernel mode
    - Time spent in User mode

/proc/[pid]/statm
    - Virtual memory size
    - Resident pages
    - Shared pages