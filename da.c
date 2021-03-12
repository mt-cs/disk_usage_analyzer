#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "elist.h"
#include "logger.h"
#include "util.h"

/* Forward declarations: */
void print_usage(char *argv[]);
int comp(const void *a, const void *b);
void traverse_dir(char *name, struct elist *list);

/* Create struct for entries */
struct Entries
{
	off_t bytes;
    time_t time;
    char *path;
} entry;

void print_usage(char *argv[]) {
	fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
	fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

	fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

	fprintf(stderr, "Options:[<64;34;23M[<65;34;23M[<65;34;23M[<65;34;23M[<65;34;23M[<65;34;23M[<65;34;23M[<65;34;23M[<65;34;23M]]]]]]]]]\n"
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

int comparator_bytes(const void *a, const void *b)
{
    off_t *ai = (off_t *) a;
    off_t *bi = (off_t *) b;

    return *ai > *bi;
}

int comparator_time(const void *a, const void *b)
{
    time_t *ai = (time_t *) a;
    time_t *bi = (time_t *) b;

    return *ai > *bi;
}
    
void traverse_dir(char *name, struct elist *list){
    DIR* dir;
    struct dirent *ent;
    struct stat states;

    dir = opendir(name);

    if(!dir) {
        return;
    }

	// if (stat("etc/passwd", &states) == -1) {
		// perror("stat");
	// }

    while((ent=readdir(dir)) != NULL){
        stat(ent->d_name,&states);
        //printf("%d\n",(double)states.st_size);

        if(!strcmp(".", ent->d_name) || !strcmp("..", ent->d_name)){
            continue;
        }
        else{
        	//struct Entries *entry = elist_add_new(list);
            //struct Entries *entry = (struct Entries*)malloc(sizeof(struct Entries));
            entry.bytes = states.st_size;
           	
            entry.time = states.st_atim.tv_sec;

            // double size = (double)states.st_size;
            // char *buf_bytes;
            // size_t sz = 80;
// 
            // unsigned int decimals = 1;
            // human_readable_size(&buf_bytes, sz, size, decimals);
		// 
            // char buf_time[80];
            // simple_time_format(buf_time, 80, entry.time);

            //printf(GREEN "%-10s/%-8s 	%-8s 	 %s\n" WHITE, name, ent->d_name, buf_bytes, buf_time);
            if(S_ISDIR(states.st_mode)){
                size_t file_path_len = strlen(name) + strlen(ent->d_name) + 2;
                char *file_path = malloc(file_path_len);
                snprintf(file_path, file_path_len, "%s/%s", name, ent->d_name);
                // printf("%51s 	%14s 	%15s\n", file_path, buf_bytes, buf_time);
                entry.path =  file_path;
                traverse_dir(file_path, list);
                free(file_path);
            }
            elist_add(list, &entry);
           
            //free(buf_bytes);
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

	 //printf("%s", options.directory);
	 // if (opendir(options.directory) == NULL) {
	 	// perror("Directory doesn't exist");
	 	// return -1;
	 // }
	 
     struct elist *list = elist_create(0, sizeof(int));
     traverse_dir(options.directory, list);

     struct Entries *e = elist_get(list, 0);
     
     if (options.sort_by_time) {
     	elist_sort(list, comparator_time);
     } else {
     	elist_sort(list, comparator_bytes);
     }
    
     return 0;
}


   
