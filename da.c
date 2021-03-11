#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#include "elist.h"
#include "logger.h"
#include "util.h"

#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"

/* Forward declarations: */
void print_usage(char *argv[]);
int comp(const void *a, const void *b);
void traverse_dir(char *name, struct elist *list);

/* Create struct for entries */
struct Entries
{
	unsigned int *bytes;
    unsigned int *time;
    char *path;
};

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


 // create a helper function that takes in current path, elist -- create new string for path
    // 	- then try to find all the files under the path
    // 	- when get file
    //	 	- - create string and append the path
    //	 	- - create struct and append the struct
    //	 	add struct in elist
    // recursive case: when find directory, call it again on it
    // pass the elist
    
void traverse_dir(char *name, struct elist *list){
    DIR* dir;
    struct dirent *ent;
    struct stat states;

    dir = opendir(name);
    
    if(!dir) {
        return;
    }

    while((ent=readdir(dir)) != NULL){
        stat(ent->d_name,&states);
        //printf("%d\n",(double)states.st_size);
        
        if(!strcmp(".", ent->d_name) || !strcmp("..", ent->d_name)){
            continue;
        }
        else{
        	//char *buf_size = human_readable_size(*buf_size, 100, name.size);
			double size = (double)states.st_size;
			char *buf_size;
			size_t sz = 80;
        	struct Entries *entry;
        	unsigned int decimals;
        	human_readable_size(buf_size, sz, size, decimals);
			entry->bytes = buf_size;
			
        
            printf(GREEN "%s/%s\n" WHITE, name, ent->d_name);
            if(S_ISDIR(states.st_mode)){
                size_t file_path_len = strlen(name) + strlen(ent->d_name) + 2;
                char *file_path = malloc(file_path_len);
                snprintf(file_path, file_path_len, "%s/%s", name, ent->d_name);
                elist_add(list, file_path);
                traverse_dir(file_path, list);
                free(file_path);
            }
        }
    }
    closedir(dir);
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

    /* TODO:
     *  - check to ensure the directory actually exists
     *  - create a new 'elist' data structure
     *  - traverse the directory and store entries in the list
     *  - sort the list (either by size or time)
     *  - print formatted list
     */

	 if (opendir(options.directory) == NULL) {
	 	perror("Directory doesn't exist");
	 	return -1;
	 }
     struct elist *list = elist_create(0, sizeof(int));
     traverse_dir(options.directory, list);
     
     	
     
     
   
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
	//elist_destroy(list);
    return 0;
}


   
