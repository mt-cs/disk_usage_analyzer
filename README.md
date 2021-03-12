# Project 1: Elastic Array and Disk Usage Analyzer

Author: Marisa Tania  

## About Elist Directory

This directory traversal and disk usage anylisis use a custom elastic array data structure. The elastic array is dynamic and can store unbounded number of elements depending on what memory permits. It is a a reusable C library that will be useful down the line.

The disk analyzer traverses all the files from the current root directory recursively. Every file’s absolute path, byte size, and last access time will be stored in a struct entries and listed out in the elastic array.

### Help/Usage Information
To get a sense of the functionality of the Elist Directory disk usage analyzer, here is the help/usage information:
```bash
$ ./da -h
Disk Analyzer (da): analyzes disk space usage
Usage: ./da [-ahs] [-l limit] [directory]

If no directory is specified, the current working directory is used.

Options:
    * -a              Sort the files by time of last access (descending)
    * -h              Display help/usage information
    * -l limit        Limit the output to top N files (default=unlimited)
    * -s              Sort the files by size (default, ascending)
```
User can sort the directories based on the size ascendingly, or time of last access descendingly. They also have the options to show limited numver of output.

### Elist Functions
The elastic array implements these following functions:
- <b>elist_add</b>: appends an element to the array
- <b>elist_add_new</b>: creates storage space for a new element and returns a pointer to it
- 


### Included Files
There are several files included. These are:
   - <b>Makefile</b>: Including to compile and run the program.
   - <b>da.c</b>: The disk usage analyzer
   - <b>elist.c</b>: Elastic array to
   - <b>util.c</b>: 
   - <b>task_info.c</b>: Includes functions that will count the number of tasks running, number of interrupts, context switches, and forks since boot. Also contains functions that will count the number of processes of the system and print them out as a list.
   - <b>read_files.c</b>: Includes helper functions to read in a file using system call read and also tokenize a string.

There are also header files for each of these files.


To compile and run:

```bash
make
./inspector
```


### Program Output
```bash
$ ./inspector
inspector.c:133:main(): Options selected: hardware system task_list task_summary

System Information
------------------
Hostname: lunafreya
Kernel Version: 4.20.3-arch1-1-ARCH
Uptime: 1 days, 11 hours, 49 minutes, 56 seconds

Hardware Information
------------------
CPU Model: AMD EPYC Processor (with IBPB)
Processing Units: 2
Load Average (1/5/15 min): 0.00 0.00 0.00
CPU Usage:    [--------------------] 0.0%
Memory Usage: [#-------------------] 9.5% (0.1 GB / 1.0 GB)

Task Information
----------------
Tasks running: 88
Since boot:
    Interrupts: 2153905
    Context Switches: 3678668
    Forks: 38849

  PID |        State |                 Task Name |            User | Tasks 
------+--------------+---------------------------+-----------------+-------
    1 |     sleeping |                   systemd |               0 | 1 
    2 |     sleeping |                  kthreadd |               0 | 1 
    3 |         idle |                    rcu_gp |               0 | 1 
    4 |         idle |                rcu_par_gp |               0 | 1 
    6 |         idle |      kworker/0:0H-kblockd |               0 | 1 

```

## Testing

To execute the test cases, use `make test`. To pull in updated test cases, run `make testupdate`. You can also run a specific test case instead of all of them:

```
# Run all test cases:
make test

# Run a specific test case:
make test run=4

# Run a few specific test cases (4, 8, and 12 in this case):
make test run='4 8 12'
```

See spec here: https://www.cs.usfca.edu/~mmalensek/cs521/assignments/project-1.html
