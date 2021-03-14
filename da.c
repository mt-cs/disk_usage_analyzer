
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
#include <errno.h>

#include "elist.h"
#include "logger.h"
#include "util.h"
#include "da.h"
/* Forward declarations: */
// void print_usage(char *argv[]);
// int comp(const void *a, const void *b);
// void traverse_dir(char *name, struct elist *list);

/* Create struct for entries */
struct Entries
{
	off_t bytes;
    time_t time;
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

/*
* Compare directory based on the byte size ascendingly
* @param const void *a element directory 
* @param const void *b element directory
*/
int comparator_bytes(const void *a, const void *b)
{
    off_t *ai = (off_t *) a;
    off_t *bi = (off_t *) b;

    return *ai > *bi;
}

/*
* Compare directory based on the last access time descendingly
* @param const void *a element directory 
* @param const void *b element directory
*/
int comparator_time(const void *a, const void *b)
{
    time_t *ai = (time_t *) a;
    time_t *bi = (time_t *) b;

    return *ai < *bi;
}

/*
* Traverse the directory recursively and
* storing path, size, and time to the struct entry
* @param name directory
* @param listt elist elastic array
*/   
void traverse_dir(char *name, struct elist *list){
    DIR* dir;
    struct dirent *ent;

    dir = opendir(name);

    if(!dir) {
        perror("opendir");
        return;
    }

    while((ent=readdir(dir)) != NULL){
        size_t file_path_len = strlen(name) + strlen(ent->d_name) + 2;
        char *file_path = malloc(file_path_len);
        snprintf(file_path, file_path_len, "%s/%s", name, ent->d_name);

        if (ent->d_type == DT_DIR) {
            // is a directory; traverse
            if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0){
            	free(file_path);
                continue;
            }
            traverse_dir(file_path, list);
            free(file_path);
        } else {
            // is a file, get its stats
            struct Entries entry;
            struct stat states;
            if (stat(file_path, &states) == -1) {
                perror("stat");
            }
            entry.bytes = states.st_size;
            entry.time = states.st_atim.tv_sec;
            entry.path = file_path;
            elist_add(list, &entry);
            LOG("adding: %s\n", entry.path);
        }
        //free(file_path);
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

    //LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);
    
	// check to ensure the directory actually exists
	DIR *d;
	if ((d = opendir(options.directory)) == NULL) {
		perror("Directory doesn't exist");
		return -1;
	}
    closedir(d);
 	// create a new 'elist' data structure
    struct elist *list = elist_create(0, sizeof(struct Entries));

    // traverse the directory and store entries in the list
    traverse_dir(options.directory, list);

	// sort the list (either by size or time)
    if (options.sort_by_time) {
    	elist_sort(list, comparator_time);
    } else {
    	elist_sort(list, comparator_bytes);
    }

    //print formatted list
    unsigned int decimals = 1;
    char size_buf[14];
    char time_buf[15];
    char path_buf[51];
    size_t print_sz = 1;

    if (options.limit != 0) {
    	print_sz = options.limit;
    } else {
    	print_sz = elist_size(list);
    }
    
    for (size_t i = 0; i < print_sz; i++) {
    	struct Entries *e = elist_get(list, i);
    	human_readable_size(size_buf, 14, (double)e->bytes, decimals);
    	//LOG("Size_buf: %s\n", size_buf);
    	simple_time_format(time_buf, 15, e->time);
    	//LOG("Path: %s\n", e->path);
    	unsigned int path_len = strlen(e->path);
    	if (path_len > 51) {
    		strcpy(path_buf, "...");
    		strncat(path_buf, &(e->path[path_len-48]), 51);
    	}
    	else
    	{
    		strcpy(path_buf, e->path);
    	}
    	printf("%*s%*s%*s\n", 51, path_buf, 14, size_buf, 15, time_buf);
    }
    
	//destroy all path buffers
	size_t i = 0;
	for(i = 0; i< elist_size(list);i++)
	{
		struct Entries * p = (struct Entries*)(elist_get(list, i));
		free(p->path);
	}
    elist_destroy(list);
    
    return 0;
}



