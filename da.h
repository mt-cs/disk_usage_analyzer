/**
 * @file elist.h
 */

#ifndef _DA_H_
#define _DA_H_

#include <sys/types.h>

struct Entries;
void print_usage(char *argv[]);
int comparator_bytes(const void *a, const void *b);
int comparator_time(const void *a, const void *b);
void traverse_dir(char *name, struct elist *list);

#endif
