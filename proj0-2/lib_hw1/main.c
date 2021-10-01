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

// MAIN
bool execute(char*);
void create(char*);
void delete(char*);
void dumpdata(char*);
void list_handler(char*, int flag);
void bitmap_handler(char*, int flag);
void hash_handler(char*, int flag);
int str2int(char*);

// Data Structure
char* param[6];
struct list* L[10];
struct hash* H[10];
struct hash_iterator* Hash_Iterator;
struct bitmap* B[10];

int main(){
    char command[100];
    bool exit = false;
    Hash_Iterator = (struct hash_iterator*)malloc(sizeof(struct hash_iterator));

    while (exit == false){
        fgets(command, sizeof(command), stdin);
        exit = execute(command);
    }
}

bool execute(char *command){
    bool exit = false;

    if (strncmp(command, "create", 6) == 0){
        command += 7;
        create(command);
    }
    else if (strncmp(command, "delete", 6) == 0){
        command += 7;
        delete(command);
    }
    else if (strncmp(command, "dumpdata", 8) == 0){
        command += 9;
        dumpdata(command);
    }
    else if (strncmp(command, "quit", 4) == 0){
        exit = true;
    }
    else if (strncmp(command, "list", 4) == 0){
        command += 5;
        list_handler(command, OTHERS);
    }
    else if (strncmp(command, "bitmap", 6) == 0){
        command += 7;
        bitmap_handler(command, OTHERS);
    }
    else if (strncmp(command, "hash", 4) == 0){
        command += 5;
        hash_handler(command, OTHERS);
    }

    return exit;
}

void create(char* command){
    // create list
    if (strncmp(command, "list", 4) == 0){
        list_handler(command + 9, CREATE);
    }

    // create hashtable
    else if (strncmp(command, "hashtable", 9) == 0){
        hash_handler(command + 14, CREATE);
    }

    // create bitmap
    else if (strncmp(command, "bitmap", 6) == 0){
        bitmap_handler(command + 7, CREATE);
    }
}

void delete(char* command){
    // delete list
    if (strncmp(command, "list", 4) == 0){
        list_handler(command + 4, DELETE);
    }

    // delete hashtable
    else if (strncmp(command, "hash", 4) == 0){
        hash_handler(command + 4, DELETE);
    }

    // delete bitmap
    else if (strncmp(command, "bm", 2) == 0){
        bitmap_handler(command + 2, DELETE);
    }
}

void dumpdata(char* command){
    // dump list
    if (strncmp(command, "list", 4) == 0){
        list_handler(command + 4, DUMP);
    }

    // dump hashtable
    else if (strncmp(command, "hash", 4) == 0){
        hash_handler(command + 4, DUMP);
    }

    // dump bitmap
    else if (strncmp(command, "bm", 2) == 0){
        bitmap_handler(command + 2, DUMP);
    }
}

int str2int(char* str){
    int ret = 0;
    int idx = 0;
    bool neg = false;

    if (str[idx++] == '-') {
        neg = true;
    }

    while (true) {
        if (str[idx] == '\0') break;
        if (str[idx] == '\n') break;

        ret *= 10;
        ret += (str[idx] - '0');
        idx++;
    }

    if (neg) {
        ret = - ret;
    }

    return ret;
}

void list_swap(struct list_elem* a, struct list_elem *b){
    struct list_item* aitem = list_entry(a, struct list_item, elem);
    int adata = aitem->data;

    struct list_item* bitem = list_entry(b, struct list_item, elem);
    int bdata = bitem->data;

    aitem->data = bdata;
    bitem->data = adata;
}

void list_shuffle(struct list *list){
    int shuffle_num = (rand() % 5) + 5;
    int lsize = list_size(list);
    int a, b, idx;
    struct list_elem *aelem, *belem, *cur;

    for (int i = 0; i < shuffle_num; i++){
        b = a = rand() % lsize;
        while(a == b) b = rand() % lsize;

        for (cur = list_begin(list), idx = 0; cur != list_end(list); cur = list_next(cur), idx++){
            if (idx == a) aelem = cur;
            else if (idx == b) belem = cur;
        }
        list_swap(aelem, belem);
    }
}

void list_handler(char* command, int flag){
    int idx;
    int i;
    struct list_elem *cur;

    // split command
    param[0] = strtok(command, " ");

    i = 1;
    while (true){
        param[i] = strtok(NULL, " ");
        if (param[i] == NULL) break;
        i++;
    }

    switch (flag){
        case CREATE:
            idx = command[0] - '0';
            L[idx] = (struct list*)malloc(sizeof(struct list));
            list_init(L[idx]);
            break;

        case DELETE:
            idx = command[0] - '0';
            for (cur = list_begin(L[idx]); cur != list_end(L[idx]); cur = list_next(cur)){
                struct list_item* tmp = list_entry(cur, struct list_item, elem);
                free(tmp);
                tmp = NULL;
            }
            free(L[idx]);
            L[idx] = NULL;
            break;

        case DUMP:
            idx = command[0] - '0';
            for (cur = list_begin(L[idx]); cur != list_end(L[idx]); cur = list_next(cur)){
                struct list_item *tmp = list_entry(cur, struct list_item, elem);
                int temp_data = tmp->data;
                printf("%d ", tmp_data);
            }
            printf("\n");
    }

    if (flag == OTHERS){
        idx = param[1][4] - '0';

        if (strncmp(param[0], "front", 5) == 0){
            cur = list_front(L[idx]);
            struct list_item *tmp = list_entry(cur, struct list_item, elem);
            int tmp_data = tmp->data;
            printf("%d\n", tmp_data);
        }
        else if (strncmp(param[0], "back", 4) == 0){
            cur = list_back(L[idx]);
            struct list_item *tmp = list_entry(cur, struct list_item, elem);
            int tmp_data = tmp->data;
            printf("%d\n", tmp_data);
        }
        else if (strncmp(param[0], "pop_back", 8) == 0){
            list_pop_back(L[idx]);
        }
        else if (strncmp(param[0], "pop_front", 9) == 0){
            list_pop_front(L[idx]);
        }
        else if (strncmp(param[0], "push_front", 10) == 0){
            int val = str2int(param[2]);
            struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
            new_item->data = val;
            list_push_front(L[idx], &(new_item->elem));
        }
        else if (strncmp(param[0], "push_back", 9) == 0){
            int val = str2int(param[2]);
            struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
            new_item->data = val;
            list_push_back(L[idx], &(new_item->elem));
        }
        else if (strncmp(param[0], "insert_ordered", 14) == 0){
            // insert 하고 -> sort
            int val = str2int(param[2]);
            void *aux;
            struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
            new_item->data = val;
            list_insert_ordered(L[idx], &(new_item->elem), list_less_function, aux);
        }
        else if (strncmp(param[0], "insert", 6) == 0){
            int before = param[2][0] - '0';
            int val = str2int(param[3]);
            for (i = 0, cur = list_begin(L[idx]); ;cur = list_next(cur)){
                if (i == before){
                    struct list_item* new_item = (struct list_item*)malloc(sizeof(struct list_item));
                    new_item->data = val;
                    list_insert(cur, &(new_item->elem));
                }
                if (cur == list_end(L[idx])){
                    break;
                }
                i++;
            }
        }
        else if (strncmp(param[0], "empty", 5) == 0){
            if (list_empty(L[idx]) != 0){
                printf("true\n");
            }
            else{
                printf("false\n");
            }
        }
        else if (strncmp(param[0], "size", 4) == 0){
            int cnt = 0;
            for (cur = list_begin(L[idx]); cur != list_end(L[idx]); cur = list_next(cur)){
                cnt++;
            }
            printf("%d\n", cnt);
        }
        else if (strncmp(param[0], "max", 3) == 0){
            void* aux;
            cur = list_max(L[idx], list_less_function, aux);
            struct list_item* eitem = list_entry(cur, struct list_item, elem);
            int val = eitem->data;
            printf("%d\n", val);
        }
        else if (strncmp(param[0], "remove", 6) == 0){
            i = 0;
            int tar = str2int(param[2]);
            for (cur = list_begin(L[idx]); cur != list_end(L[idx]); cur = list_next(cur), i++){
                if (i == tar) break;
            }
            list_remove(cur);
            struct list_item *ditem = list_entry(cur, struct lsit_item, elem);
            free(ditem);
            ditem = NULL;
        }
        else if (strncmp(param[0], "sort", 4) == 0){
            void* aux;
            list_sort(L[idx], list_less_function, aux);
        }
        else if (strncmp(param[0], "splice", 6) == 0){
            int num = str2int(param[5]);
            int start1 = str2int(param[2]);
            int idx2 = param[3][4] - '0';
            int start2 = str2int(param[4]);
            int end2 = start2 + num - 1;
            struct list_elem* first;
            struct list_elem* before;
            struct list_elem* last;

            // 이동시킬 놈의 시작 and 끝 움켜쥠
            i = 0;
            for (cur = list_begin(L[idx2]); ;cur = list_next(cur), i++){
                if (i == start2) {
                    first = cur;
                }
                else if (i == end2) {
                    last = cur;
                    break;
                }
            }

            // 받아들일 놈의 위치 탐색
            i = 0;
            for (cur = list_begin(L[idx2]); ; cur = list_next(cur), i++){
                if (i == start1){
                    before = cur;
                    break;
                }
            }
            list_splice(before, first, last);
        }
        else if (strncmp(param[0], "swap", 4) == 0){
            int a = str2int(param[2]);
            int b = str2int(param[3]);
            struct list_elem *aelem, *belem;

            i = 0;
            for (cur = list_begin(L[idx]); cur != list_end(L[idx]); cur = list_next(cur), i++){
                if (i == a) aelem = cur;
                if (i == b) belem = cur;
            }
            list_swap(aelem, belem);
        }
        else if (strncmp(param[0], "unique", 6) == 0){
            void* aux;
            int idx2 = -1;

            if (param[2] != NULL){
                idx2 = param[2][4] - '0';
                list_unique(L[idx], L[idx2], list_less_function, aux);
            }
            else{
                list_unique(L[idx], NULL, list_less_function, aux);
            }
        }
        else if (strncmp(param[0], "shuffle", 7) == 0){
            list_shuffle(L[idx]);
        }
    }
    return;
}


unsigned hash_int_2(int i){
    unsigned hash = FNV_32_BASIS;

    while (i > 0){
        if (i & 1 == 1){
            hash = hash ^ FNV_32_PRIME;
        }
        i >>= 1;
    }
    return hash;
}

unsigned hash_hash_function(const struct hash_elem* e, void *aux){
    struct hash_item* hitem = hash_entry(e, struct hash_item, elem);
    int val = hitem->data;
    return hash_int(val);
}

bool hash_less_function(const struct hash_elem* a, const struct hash_elem* b, void* aux){
    struct hash_item* aitem = hash_entry(a, struct hash_item, elem);
    int aval = aitem->data;

    struct hash_item* bitem = hash_entry(b, struct hash_item, elem);
    int bval = bitem->data;

    return aval < bval;
}

#define SQUARE 0
#define TRIPLE 1
void hash_action_on_element(struct hash_elem* e, void *aux){
    int flag = *(int*)aux;

    struct hash_item *hitem = hash_entry(e, struct hash_item, elem);
    int val = hitem->data;

    switch (flag){
        case SQUARE:
            hitem->data = val * val;
            break;
        case TRIPLE:
            hitem->data = val * val * val;
            break;
    }
}

void hash_action_destructor(struct hash_elem* e, void* aux){
    struct hash_item *hitem = hash_entry(e, struct hash_item, elem);
    free(hitem);
    hitem = NULL;
}

void hash_handler(char* command, int flag){
    int idx;

    // split command
    param[0] = strtok(command, " ");
    int i = 1;
    while (true){
        param[i] = strtok(NULL, " ");
        if (param[i] == NULL) break;
        i++;
    }

    switch(flag){
        case CREATE:
            idx = command[0] - '0';
            H[idx] = (struct hash*)malloc(sizeof(struct hash));
            void* aux;
            hash_init(H[idx], hash_hash_function, hash_less_func, aux);
            break;
        case DELETE:
            idx = command[0] - '0';
            hash_destroy(H[idx], hash_action_destructor);
            break;
        case DUMP:
            idx = command[0] - '0';
            hash_first(Hash_Iterator, H[idx]);
            struct hash_elem* cur;

            while (true){
                cur = hash_next(Hash_Iterator);
                if (cur == NULL) break;

                struct hash_item *hitem = hash_entry(cur, struct hash_item, elem);
                int val = hitem->data;
                printf("%d ", val);
            }
            printf("\n");
    }

    if (flag == OTHERS){
        idx = params[1][4] - '0';

        if (strncmp(param[0], "insert", 6) == 0){
            int val = str2int(param[2]);
            struct hash_item* new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
            new_item->data = val;
            hash_insert(H[idx], &(new_item->elem));
        }
        else if (strncmp(param[0], "apply", 5) == 0){
            void* aux;
            int flag;

            if (strncmp(param[2], "square", 6) == 0){
                flag = SQUARE;
            }
            else if (strncmp(param[2]. "triple", 6) == 0){
                flag = TRIPLE;
            }
            aux = &flag;
            H[idx]->aux = aux;
            hash_apply(H[idx], hash_action_on_element);
        }
        else if (strncmp(param[0], "delete", 6) == 0){
            int target = str2int(param[2]);

            struct hash_item* new_item = (struct hash_item*)malloc(sizeof(struct(hash_item));
            new_item->data = target;
            hash_delete(H[idx], &(new_item->elem));
            free(new_item);
            new_item = NULL;
        }
        else if (strncmp(param[0], "empty", 5) == 0){
            if (hash_empty(H[idx])) {
                printf("true\n");
            }
            else {
                printf("false\n");
            }
        }
        else if (strncmp(param[0], "size", 4) == 0){
            printf("%d\n", (int)hash_size(H[idx]));
        }
        else if (strncmp(param[0], "clear", 5)){
            hash_clear(H[idx], hash_action_destructor);
        }
        else if (strncmp(param[0], "find", 4) == 0){
            int tar = str2int(param[2]);
            struct hash_elem* found;

            struct hash_item *new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
            new_item->data = tar;
            found = hash_find(H[idx], &(new_iterm->elem));

            if (found != NULL){
                printf("%d\n", tar);
            }
        }
        else if (strncmp(param[0], "replace", 7)){
            int val = str2int(param[2]);
            struct hash_item *new_item = (struct hash_item*)malloc(sizeof(struct hash_item));
            new_item->data = val;

            hash_replace(H[idx], &(new_item->elem));
        }
    }
}