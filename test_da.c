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
#include "da.h"
#include "elist.h"
#include "logger.h"
#include "util.h"

struct Entries
{
	off_t bytes;
    time_t time;
    char *path;
};

int main()
{
	char *test_dir = ".";
	struct elist *list = elist_create(0, sizeof(struct Entries));
	traverse_dir(test_dir, list);
	for (size_t i = 0; i < elist_size(list); i++) {
    	struct Entries *e = elist_get(list, i);
    	LOG("Path: %s\n", e->path);
    	
    }
	return 0;
}
