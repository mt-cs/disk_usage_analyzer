#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "elist.h"
#include "logger.h"

/* Forward declarations: */
void print_usage(char *argv[]);


void print_usage(char *argv[]) {
	fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
	fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

	fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

	fprintf(stderr, "Options:\n"
		"    * -a              Sort the files by time of last access (descending)\n"
		"    * -h              Display help/usage information\n"
		"    * -l limit        Limit the output to top N files (default=unlimited)\n"
		"    * -s              Sort the files by size (default, ascending)\n\n"
		);
}

int comp(const void *a, const void *b) {
	int *ap = (int *) a;
	int *bp = (int *) b;
	//return *ap < *bp; // descending
	return *ap > *bp; // ascending
}

int main(int argc, char *argv[])
{
    /* Create a struct to hold program options and initialize it by declaring an
     * 'options' variable. Defaults:
     *      - sort by size (time=false)
     *      - limit of 0 (unlimited)
     *      - directory = '.' (current directory) */
    struct da_options {
        bool sort_by_time;
        unsigned int limit;
        char *directory;
    } options
        = { false, 0, "." };

    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
            case 'a':
                options.sort_by_time = true;
                break;
            case 'h':
                print_usage(argv);
                return 0;
            case 's':
                options.sort_by_time = false;
                break;
            case 'l': {
                /*    ^-- to declare 'endptr' here we need to enclose this case
                 *    in its own scope with curly braces { } */
                char *endptr;
                long llimit = strtol(optarg, &endptr, 10);
                if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                    fprintf(stderr, "Invalid limit: %s\n", optarg);
                    print_usage(argv);
                    return 1;
                }
                options.limit = (int) llimit;
                break;
                }
            case '?':
                if (optopt == 'l') {
                    fprintf(stderr,
                            "Option -%c requires an argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n", optopt);
                }
                print_usage(argv);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
    }

    LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);
}
    /* TODO:
     *  - check to ensure the directory actually exists
     *  - create a new 'elist' data structure
     *  - traverse the directory and store entries in the list
     *  - sort the list (either by size or time)
     *  - print formatted list
     */
   
	// struct elist *list = elist_create(0, sizeof(int));
	// int a = 9;
	// elist_add(list, &a);
	// 
	// a = 6;
	// elist_add(list, &a);
	// 
	// for (int i = 0; i < 1000; ++i) {
		// elist_add(list, &i);
	// }
// 
	// int my_nums[100];
	// for (int i = 0; i < 100; ++i) {
		// my_nums[i] = random() % 1000;
	// }
// 
	// for (int i = 0; i < 100; ++i) {
			// printf("-> %d\n", my_nums[i]);
		// }
		// 
	// qsort(my_nums, 100, sizeof(int), comp);
// 
	// printf("AFTER SORTING:\n");
	// for (int i = 0; i < 100; ++i) {
		// printf("-> %d\n", my_nums[i]);
	// }
	// 
	// int b = 999;
	// elist_add(list, &b);
// 
	// elist_add(list, &options); // casting options to integer and you don't know what you'll get back
// 
	// int *x = elist_get(list, 1);
	// printf("we got an integer back! %d\n", *x);
// 
	// int *y = elist_get(list, 0); // 9 because we copied it
	// printf("we got an integer back! %d\n", *y);
// 
	// int *z = elist_get(list, 3);
	// printf("we got an integer back! %d\n", *z); // this depends on the data on what &options has
		// 
	// elist_destroy(list);
    // return 0;



    // create a helper function that takes in current path, elist -- create new string for path
    // then try to find all the files under the path
    // when get file
    // create string and append the path
    // create struct and append the struct
    // add struct in elist
    // recursive case: when find directory, call it again on it
    // pass the elist
 // int traverseDir(char* in_path) {
 	// struct dirent *entry;
    // struct stat filestat;
    // DIR* directory;
    // char path[100];
// 
    // printf("--> %s\n", options.directory);
    // if ((directory = opendir(options.directory) == NULL)) {
    	// perror("opendir");
    	// return -1;
    // }
    // printf(" ==> %s\n", directory);
    // while ((entry == readdir(directory)) != NULL) {
    	// if (stat(entry->d_name, &filestat) == -1) {
    		// continue;
    	// }
// 
    	// printf(" ==> %s, ", entry->d_name);
    	// 
    // }
 // }
    // 
// }
