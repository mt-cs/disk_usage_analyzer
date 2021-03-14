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

struct Entries
{
	int bytes;
    char *time;
    char *path;
};

int main(void){
	LOG("Size of Entries: %zu\n", sizeof(struct Entries));
	struct elist *list = elist_create(0, sizeof(struct Entries));
	struct Entries entry;
	entry.bytes = 180;
	entry.time = "Mar 12 2021";
	entry.path = "./lodie";
	elist_add(list, &entry);
	LOG("adding: %s\n", entry.path);
	
	struct Entries entry2;
	entry2.bytes = 290;
	entry2.time = "Mar 10 2021";
	entry2.path = "./nemo";
	elist_add(list, &entry2);
	LOG("adding: %s\n", entry2.path);
	
	struct Entries entry3;
	entry3.bytes = 130;
	entry3.time = "Feb 1 2021";
	entry3.path = "./marisa";
	elist_add(list, &entry3);
	LOG("adding: %s\n", entry3.path);

	LOG("\nSize of list: %zu\n", elist_size(list));
	
	for (size_t i = 0; i < elist_size(list); i++) {
    	struct Entries *e = elist_get(list, i);
    	LOG("Path: %s\n", e->path);	
    	LOG("Time: %s\n", e->time);
    	LOG("Bytes: %d\n", e->bytes);
	}
	elist_destroy(list);
	return 0;
}


