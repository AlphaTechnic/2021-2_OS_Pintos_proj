#include "list.h"
#include "hash.h"
#include "bitmap.h"
#include "debug.h"
#include "limits.h"
#include "round.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typdef enum action {
    CREATE,
    DELETE,
    DUMP,
    OTHERS
};


// LIST
void list_swap(struct list_elem* a, struct list_elem* b);
void list_shuffle(struct list* list);

// HASH
unsigned hash_int_2(int i);
unsigned hash_hash_function(const struct hash_elem* e, void* aux);
bool hash_less_function(const struct hash_elem* a, const struct hash_elem* b, void* aux);
void hash_action_on_element(struct hash_elem* e, void* aux);
void hash_action_destructor(struct hash_elem* e, void* aux);

// BITMAP
struct bitmap *bitmap_expand(struct bitmap* b, int size);

