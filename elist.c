#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "elist.h"
#include "logger.h"

#define DEFAULT_INIT_SZ 10 //preprocesser macro
#define RESIZE_MULTIPLIER 2 //double the element
// capacity is in number of items not bytes
struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

/**
* checks if index is valid.
*/
bool idx_is_valid(struct elist *list, size_t idx); // private function in c

/**
 * Creates a new elist data structure. If the initial size is set to 0, the
 * default capacity will be used. 
 *
 * @param init_capacity The initial capacity of the list
 * @param element_sz Size of the elements that will be stored in the list (in bytes)
 *
 * @return A pointer to the newly-created list, or NULL on failure.
 */
struct elist *elist_create(size_t list_sz, size_t item_sz)
{
	/* Create space for the container structL */
	struct elist *list = malloc(sizeof(struct elist));
	if (list == NULL) { // check if malloc returns null
		return NULL;
	}

	/* if lisr_sz is 0, then we'll just use the default */
	if (list_sz == 0) {
		list_sz = DEFAULT_INIT_SZ;
	}
	
	list->capacity = list_sz; // the capacity
	list->item_sz = item_sz;
	list->size = 0; // the actual number of element. Here empty list we haven't put anything in the "array list"

	size_t storage_bytes = list->capacity * list->item_sz;
	LOG("Initializing new elist: capacity=[%zu], item_sz=[%zu], bytes=[%zu]\n",
			list->capacity,
			list->item_sz,
			storage_bytes);
			
	list->element_storage = malloc(storage_bytes);

	/*Check if returns NULL */
	if (list-> element_storage == NULL) {
		perror("malloc");
		free(list);
		return NULL;
	}
	
    return list;
}

/**
 * Destroys the specified list and frees any memory that was allocated to it.
 *
 * @param list The list to destroy
 */
void elist_destroy(struct elist *list)
{
	free(list->element_storage); // element_storage first before we free the list itself
	free(list);
}

int elist_set_capacity(struct elist *list, size_t capacity)
{
    return -1;
}

size_t elist_capacity(struct elist *list)
{
    return 0;
}

ssize_t elist_add(struct elist *list, void *item)
{
	//  TODO: move to set capacity and call it here
	/*Check if we need to resize*/
	if (list->size >= list->capacity) {
		list->capacity = list->capacity * RESIZE_MULTIPLIER;
		LOG("Resizing the list. New capacity: %zu\n", list->capacity);

		/* We need to set list->element_storage = realloc() in case realloc
		 * changes the memory address of our element storage */
		list->element_storage = realloc(
			list->element_storage, list->capacity * list->item_sz);
		if (list->element_storage == NULL) {
			return -1;
		}
	}
	size_t idx = list->size++;
	//size_t pos = (idx - 1) * list->item_sz;
	//void *item_ptr = list->element_storage + pos; // pointer to the start of the storage

	void *item_ptr = list->element_storage + idx * list->item_sz;
	memcpy(item_ptr, item, list->item_sz);
	
	// Memory layout (assuming int)
	// list[0] = 0
	// list[1] = 4
	// list[2] = 8
	// list[3] = 12
	
    return idx;
}

void *elist_add_new(struct elist *list)
{
    return NULL;
}

int elist_set(struct elist *list, size_t idx, void *item)
{
    return -1;
}

/**
 * Retrieves the element at the given index.
 *
 * @param list The list to retrieve the element from
 * @param idx Index of the element to retrieve
 *
 * @return A pointer to the element, or NULL if the index is invalid.
 */
void *elist_get(struct elist *list, size_t idx)
{
	if (idx >= list->size) {
		return NULL;
	}
    return list->element_storage + idx * list->item_sz;
}

size_t elist_size(struct elist *list)
{
    return 0;
}

int elist_remove(struct elist *list, size_t idx)
{
    return -1;
}

void elist_clear(struct elist *list)
{

}

void elist_clear_mem(struct elist *list)
{

}

ssize_t elist_index_of(struct elist *list, void *item)
{
    return -1;
}

void elist_sort(struct elist *list, int (*comparator)(const void *, const void *))
{

}

bool idx_is_valid(struct elist *list, size_t idx)
{
    return false;
}

