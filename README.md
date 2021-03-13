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
- <b>elist_capacity</b>: retrieves the current list capacity
- <b>elist_clear_mem</b>: removes all elements from the array and zeroes them out
- <b>elist_create</b>: initializes a new elist data structure
- <b>elist_destroy</b>: destroys and frees any memory allocated by an elist
- <b>elist_get</b>: retrieves a particular element by its index
- <b>elist_index_of</b>: determines the index of a particular element
- <b>elist_remove</b>: removes an element at a particular index
- <b>elist_set</b>: replaces an element in the array at a particular index
- <b>elist_set_capacity</b>: increases or decreases the storage capacity of the array
- <b>elist_size</b>: retrieves the number of elements in the array
- <b>elist_sort</b>: sorts the array


### Included Files
There are several files included. These are:
   - <b>Makefile</b>: Including to compile and run the program.
   - <b>da.c</b>: The disk usage analyzer to traverse directory
   - <b>elist.c</b>: Elastic array to store elements dinamically
   - <b>util.c</b>: Helper function to converts bytes sizes and time to human-readable format string
   - <b>logger.h</b>: Helps facilitate debugging by providing basic logging functionality.

There are also header files: elist.h, until.h.

To compile and run:

```bash
make
./da
```


### Program Output
```bash
$ ./da -l 15 -s /usr
  /usr/lib/valgrind/libvex-amd64-linux.a      36.5 MiB    Aug 21 2020
                    /usr/lib/libclang.so      38.6 MiB    Aug 21 2020
                 /usr/lib/libclang.so.10      38.6 MiB    Aug 21 2020
                     /usr/bin/containerd      46.9 MiB    Feb 01 2021
                /usr/lib/libclang-cpp.so      47.2 MiB    Mar 08 2021
             /usr/lib/libclang-cpp.so.10      47.2 MiB    Mar 08 2021
                       /usr/lib/libgo.so      52.4 MiB    Sep 09 2020
                    /usr/lib/libgo.so.16      52.4 MiB    Sep 09 2020
                /usr/lib/libgo.so.16.0.0      52.4 MiB    Sep 09 2020
.../lib/docker/cli-plugins/docker-buildx      54.2 MiB    Dec 03 2020
                         /usr/bin/docker      71.0 MiB    Dec 03 2020
              /usr/lib/libLLVM-10.0.1.so      83.7 MiB    Mar 08 2021
                  /usr/lib/libLLVM-10.so      83.7 MiB    Mar 08 2021
                     /usr/lib/libLLVM.so      83.7 MiB    Mar 08 2021
                        /usr/bin/dockerd      84.4 MiB    Feb 01 2021
```

In this output example, the user requested the top 15 files (-l 15), sorted by size (-s) from the /usr directory. 
The output columns include the file name, file size in human readable units, and the last time the particular file was accessed.

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
