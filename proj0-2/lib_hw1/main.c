#include "list.h"
#include "hash.h"
#include "bitmap.h"
#include "bitmap.c"
#include "debug.h"
#include "limits.h"
#include "round.h"
#include "hex_dump.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// for hash_func
#define FNV_32_PRIME 16777619u
#define FNV_32_BASIS 2166136261u
#define SQUARE 0
#define TRIPLE 1


typedef enum action {
    CREATE,
    DELETE,
    DUMP,
    METHOD
} action_t;


// UTILS
int str2int(const char *);
void parse_cmd(char *cmd);

// MAIN
bool run(char *);
void create(char *);
void delete(char *);
void dumpdata(char *);
void list_handler(char *, action_t action);
void bitmap_handler(char *, action_t action);
void hash_handler(char *, action_t action);


// HASH
unsigned hash_hash_function(const struct hash_elem *e, void *aux);
bool hash_less_function(const struct hash_elem *a, const struct hash_elem *b, void *aux);
void hash_action_on_element(struct hash_elem *e, void *aux);
void hash_action_destructor(struct hash_elem *e, void *aux);

// BITMAP

// Data Structure
char *ARGS[6];
struct list *LISTS[10];
struct hash *HASHS[10];
struct hash_iterator *Hash_Iterator;
struct bitmap *BITMAPS[10];


int main() {
    char cmd[100];
    bool exit_flag = false;
    Hash_Iterator = (struct hash_iterator *) malloc(sizeof(struct hash_iterator));

    while (exit_flag == false) {
        fgets(cmd, sizeof(cmd), stdin);
        exit_flag = run(cmd);
    }
}


// utils
int str2int(const char *str_num) {
    bool neg = false;
    int ret = 0;
    int idx = 0;

    if (str_num[idx] == '-') {
        neg = true;
        idx++;
    }

    while (true) {
        if (str_num[idx] == '\0') break;
        if (str_num[idx] == '\n') break;

        ret *= 10;
        ret += (str_num[idx] - '0');
        idx++;
    }

    if (neg) {
        ret = -ret;
    }
    return ret;
}

void parse_cmd(char *cmd) {
    ARGS[0] = strtok(cmd, " ");
    int i = 1;
    while (true) {
        ARGS[i] = strtok(NULL, " ");
        if (ARGS[i] == NULL) break;
        i++;
    }
}

// main
bool run(char *cmd) {
    bool exit_flag = false;

    if (strncmp(cmd, "create", 6) == 0) {
        cmd += 7;
        create(cmd);
    }
    else if (strncmp(cmd, "delete", 6) == 0) {
        cmd += 7;
        delete(cmd);
    }
    else if (strncmp(cmd, "dumpdata", 8) == 0) {
        cmd += 9;
        dumpdata(cmd);
    }
    else if (strncmp(cmd, "list", 4) == 0) {
        cmd += 5;
        list_handler(cmd, METHOD);
    }
    else if (strncmp(cmd, "hash", 4) == 0) {
        cmd += 5;
        hash_handler(cmd, METHOD);
    }
    else if (strncmp(cmd, "bitmap", 6) == 0) {
        cmd += 7;
        bitmap_handler(cmd, METHOD);
    }
    else if (strncmp(cmd, "quit", 4) == 0) {
        exit_flag = true;
    }

    return exit_flag;
}

void create(char *cmd) {
    if (strncmp(cmd, "list", 4) == 0) {
        list_handler(cmd + 9, CREATE);
    }
    else if (strncmp(cmd, "hashtable", 9) == 0) {
        hash_handler(cmd + 14, CREATE);
    }
    else if (strncmp(cmd, "bitmap", 6) == 0) {
        bitmap_handler(cmd + 7, CREATE);
    }
}


void delete(char *cmd) {
    if (strncmp(cmd, "list", 4) == 0) {
        list_handler(cmd + 4, DELETE);
    }
    else if (strncmp(cmd, "hash", 4) == 0) {
        hash_handler(cmd + 4, DELETE);
    }
    else if (strncmp(cmd, "bm", 2) == 0) {
        bitmap_handler(cmd + 2, DELETE);
    }
}


void dumpdata(char *cmd) {
    if (strncmp(cmd, "list", 4) == 0) {
        list_handler(cmd + 4, DUMP);
    }
    else if (strncmp(cmd, "hash", 4) == 0) {
        hash_handler(cmd + 4, DUMP);
    }
    else if (strncmp(cmd, "bm", 2) == 0) {
        bitmap_handler(cmd + 2, DUMP);
    }
}


// list
void list_handler(char *cmd, action_t action) {
    struct list_elem *cur_elem, *nxt_elem;
    void *aux = NULL;
    int nth, i;

    parse_cmd(cmd);

    switch (action) {
        case CREATE:
            nth = cmd[0] - '0';
            LISTS[nth] = (struct list *) malloc(sizeof(struct list));
            list_init(LISTS[nth]);
            break;

        case DELETE:
            nth = cmd[0] - '0';
            for (cur_elem = list_begin(LISTS[nth]); cur_elem != list_end(LISTS[nth]); cur_elem = nxt_elem) {
                nxt_elem = list_next(cur_elem);
                struct list_item *itm = list_entry(cur_elem, struct list_item, elem);
                free(itm);
                itm = NULL;
            }
            free(LISTS[nth]);
            LISTS[nth] = NULL;
            break;

        case DUMP:
            nth = cmd[0] - '0';
            for (cur_elem = list_begin(LISTS[nth]); cur_elem != list_end(LISTS[nth]); cur_elem = list_next(cur_elem)) {
                struct list_item *itm = list_entry(cur_elem, struct list_item, elem);
                printf("%d ", itm->data);
            }
            printf("\n");
            break;

        case METHOD:
            nth = ARGS[1][4] - '0';

            if (strncmp(ARGS[0], "front", 5) == 0) {
                // list_front list0
                cur_elem = list_front(LISTS[nth]);
                struct list_item *itm = list_entry(cur_elem, struct list_item, elem);
                printf("%d\n", itm->data);
            }
            else if (strncmp(ARGS[0], "back", 4) == 0) {
                // list_back list0
                cur_elem = list_back(LISTS[nth]);
                struct list_item *itm = list_entry(cur_elem, struct list_item, elem);
                printf("%d\n", itm->data);
            }
            else if (strncmp(ARGS[0], "empty", 5) == 0) {
                // list_empty list0
                if (list_empty(LISTS[nth]) != 0) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "size", 4) == 0) {
                // list_size list0
                int cnt = 0;
                for (cur_elem = list_begin(LISTS[nth]); cur_elem != list_end(LISTS[nth]); cur_elem = list_next(cur_elem)) {
                    cnt++;
                }
                printf("%d\n", cnt);
            }
            else if (strncmp(ARGS[0], "max", 3) == 0) {
                // list_max list0
                cur_elem = list_max(LISTS[nth], list_less_function, aux);
                struct list_item *mx_itm = list_entry(cur_elem, struct list_item, elem);
                printf("%d\n", mx_itm->data);
            }
            else if (strncmp(ARGS[0], "min", 3) == 0) {
                // list_min list0
                cur_elem = list_min(LISTS[nth], list_less_function, aux);
                struct list_item *mn_itm = list_entry(cur_elem, struct list_item, elem);
                printf("%d\n", mn_itm->data);
            }
            else if (strncmp(ARGS[0], "push_front", 10) == 0) {
                // list_push_front list0 3
                int val = str2int(ARGS[2]);
                struct list_item *new_itm = (struct list_item *) malloc(sizeof(struct list_item));
                new_itm->data = val;
                list_push_front(LISTS[nth], &(new_itm->elem));
            }
            else if (strncmp(ARGS[0], "push_back", 9) == 0) {
                // list_push_back list0 1
                int val = str2int(ARGS[2]);
                struct list_item *new_itm = (struct list_item *) malloc(sizeof(struct list_item));
                new_itm->data = val;
                list_push_back(LISTS[nth], &(new_itm->elem));
            }
            else if (strncmp(ARGS[0], "pop_back", 8) == 0) {
                // list_pop_back list0
                list_pop_back(LISTS[nth]);
            }
            else if (strncmp(ARGS[0], "pop_front", 9) == 0) {
                // list_pop_front list0
                list_pop_front(LISTS[nth]);
            }
            else if (strncmp(ARGS[0], "insert_ordered", 14) == 0) {
                // list_insert_ordered list0 5
                // insert 하고 -> sort
                int val = str2int(ARGS[2]);
                struct list_item *new_itm = (struct list_item *) malloc(sizeof(struct list_item));
                new_itm->data = val;
                list_insert_ordered(LISTS[nth], &(new_itm->elem), list_less_function, aux);
            }
            else if (strncmp(ARGS[0], "insert", 6) == 0) {
                // list_insert list0 0 1
                // [0]에 1 넣음
                int val = str2int(ARGS[3]);
                int prev = ARGS[2][0] - '0';
                for (i = 0, cur_elem = list_begin(LISTS[nth]);; cur_elem = list_next(cur_elem)) {
                    if (i == prev) {
                        struct list_item *new_item = (struct list_item *) malloc(sizeof(struct list_item));
                        new_item->data = val;
                        list_insert(cur_elem, &(new_item->elem));
                    }
                    if (cur_elem == list_end(LISTS[nth])) {
                        break;
                    }
                    i++;
                }
            }
            else if (strncmp(ARGS[0], "remove", 6) == 0) {
                // list_remove list0 0
                int tar = str2int(ARGS[2]);
                for (cur_elem = list_begin(LISTS[nth]), i = 0;
                     cur_elem != list_end(LISTS[nth]); cur_elem = list_next(cur_elem), i++) {
                    if (i == tar) break;
                }
                list_remove(cur_elem);
                struct list_item *ditem = list_entry(cur_elem, struct list_item, elem);
                free(ditem);
                ditem = NULL;
            }

            else if (strncmp(ARGS[0], "unique", 6) == 0) {
                // list_unique list0
                // list_unique list0 list1
                // list0에서 unique 원소들 뽑아다가 list1에 넣음
                if (ARGS[2] != NULL) {
                    int idx2 = ARGS[2][4] - '0';
                    list_unique(LISTS[nth], LISTS[idx2], list_less_function, aux);
                }
                else {
                    list_unique(LISTS[nth], NULL, list_less_function, aux);
                }
            }
            else if (strncmp(ARGS[0], "sort", 4) == 0) {
                // list_sort list0
                list_sort(LISTS[nth], list_less_function, aux);
            }
            else if (strncmp(ARGS[0], "reverse", 7) == 0) {
                // list_reverse list0
                list_reverse(LISTS[nth]);
            }
            else if (strncmp(ARGS[0], "splice", 6) == 0) {
                // list_splice list0 2 list1 1 4
                // list0[2] list1[2]..list1[5] list0[3]
                int pre = str2int(ARGS[2]);
                int idx2 = ARGS[3][4] - '0';
                int s = str2int(ARGS[4]);
                int cnt = str2int(ARGS[5]);
                int e = s + cnt - 1;

                struct list_elem *s_elem;
                struct list_elem *pre_elem;
                struct list_elem *e_elem;
                // 이동시킬 원소들 시작 and 끝 움켜쥠
                for (cur_elem = list_begin(LISTS[idx2]), i = 0;; cur_elem = list_next(cur_elem), i++) {
                    if (i == s) {
                        s_elem = cur_elem;
                    }
                    else if (i == e) {
                        e_elem = cur_elem;
                        break;
                    }
                }

                // 받아들일 리스트가 받아들일 위치를 탐색
                for (cur_elem = list_begin(LISTS[nth]), i = 0;; cur_elem = list_next(cur_elem), i++) {
                    if (i == pre) {
                        pre_elem = cur_elem;
                        break;
                    }
                }
                list_splice(pre_elem, s_elem, e_elem);
            }
            else if (strncmp(ARGS[0], "swap", 4) == 0) {
                // list_swap list0 1 3
                int a = str2int(ARGS[2]);
                int b = str2int(ARGS[3]);
                struct list_elem *elem1, *elem2;

                for (cur_elem = list_begin(LISTS[nth]), i = 0;
                     cur_elem != list_end(LISTS[nth]); cur_elem = list_next(cur_elem), i++) {
                    if (i == a) elem1 = cur_elem;
                    if (i == b) elem2 = cur_elem;
                }
                list_swap(elem1, elem2);
            }
            else if (strncmp(ARGS[0], "shuffle", 7) == 0) {
                // list_shuffle list0
                list_shuffle(LISTS[nth]);
            }
            break;

        default:
            printf("invalid entry");
            break;
    }
}


// hash
unsigned hash_hash_function(const struct hash_elem *e, void *aux) {
    struct hash_item *itm = hash_entry(e, struct hash_item, elem);
    return hash_int(itm->data);
}

bool hash_less_function(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
    struct hash_item *itm1 = hash_entry(a, struct hash_item, elem);
    int val1 = itm1->data;

    struct hash_item *itm2 = hash_entry(b, struct hash_item, elem);
    int val2 = itm2->data;

    return val1 < val2;
}

void hash_action_on_element(struct hash_elem *e, void *aux) {
    int fun = *(int *) aux;
    struct hash_item *itm = hash_entry(e, struct hash_item, elem);
    int val = itm->data;

    switch (fun) {
        case SQUARE:
            itm->data = val * val;
            break;
        case TRIPLE:
            itm->data = val * val * val;
            break;
        default:
            printf("invalid entry");
            break;
    }
}

void hash_action_destructor(struct hash_elem *e, void *aux) {
    struct hash_item *itm = hash_entry(e, struct hash_item, elem);
    free(itm);
    itm = NULL;
}

void hash_handler(char *cmd, action_t action) {
    void *aux = NULL;
    int nth;

    // split cmd
    parse_cmd(cmd);

    switch (action) {
        case CREATE:
            nth = cmd[0] - '0';
            HASHS[nth] = (struct hash *) malloc(sizeof(struct hash));
            hash_init(HASHS[nth], hash_hash_function, hash_less_function, aux);
            break;
        case DELETE:
            nth = cmd[0] - '0';
            hash_destroy(HASHS[nth], hash_action_destructor);
            break;
        case DUMP:
            nth = cmd[0] - '0';
            hash_first(Hash_Iterator, HASHS[nth]);
            struct hash_elem *cur_elem;

            while (true) {
                cur_elem = hash_next(Hash_Iterator);
                if (cur_elem == NULL) break;

                struct hash_item *itm = hash_entry(cur_elem, struct hash_item, elem);
                printf("%d ", itm->data);
            }
            printf("\n");
            break;

        case METHOD:
            nth = ARGS[1][4] - '0';

            if (strncmp(ARGS[0], "apply", 5) == 0) {
                // hash_apply hash0 square, triple
                // element를 제곱 혹은 세제곱
                int fun;

                if (strncmp(ARGS[2], "square", 6) == 0) {
                    fun = SQUARE;
                }
                else if (strncmp(ARGS[2], "triple", 6) == 0) {
                    fun = TRIPLE;
                }
                aux = &fun;
                HASHS[nth]->aux = aux;
                hash_apply(HASHS[nth], hash_action_on_element);
            }
            else if (strncmp(ARGS[0], "empty", 5) == 0) {
                // hash_empty hash0
                if (hash_empty(HASHS[nth])) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "size", 4) == 0) {
                // hash_size hash0
                printf("%d\n", (int) hash_size(HASHS[nth]));
            }
            else if (strncmp(ARGS[0], "clear", 5) == 0) {
                // hash_clear hash0
                hash_clear(HASHS[nth], hash_action_destructor);
            }
            else if (strncmp(ARGS[0], "find", 4) == 0) {
                // hash_find hash0 10
                int tar = str2int(ARGS[2]);
                struct hash_elem *tar_elem;

                struct hash_item *new_item = (struct hash_item *) malloc(sizeof(struct hash_item));
                new_item->data = tar;
                tar_elem = hash_find(HASHS[nth], &(new_item->elem));

                if (tar_elem != NULL) {
                    printf("%d\n", tar);
                }
            }


            else if (strncmp(ARGS[0], "insert", 6) == 0) {
                // hash_insert hash0 -1
                int val = str2int(ARGS[2]);
                struct hash_item *new_itm = (struct hash_item *) malloc(sizeof(struct hash_item));
                new_itm->data = val;
                hash_insert(HASHS[nth], &(new_itm->elem));
            }
            else if (strncmp(ARGS[0], "delete", 6) == 0) {
                // hash_delete hash0 10
                // value가 10인 원소를 제거
                int target = str2int(ARGS[2]);

                struct hash_item *new_itm = (struct hash_item *) malloc(sizeof(struct hash_item));
                new_itm->data = target;
                hash_delete(HASHS[nth], &(new_itm->elem));
                free(new_itm);
                new_itm = NULL;
            }
            else if (strncmp(ARGS[0], "replace", 7) == 0) {
                // hash_replace hash0 10
                int val = str2int(ARGS[2]);
                struct hash_item *new_itm = (struct hash_item *) malloc(sizeof(struct hash_item));
                new_itm->data = val;

                hash_replace(HASHS[nth], &(new_itm->elem));
            }
            break;
        default:
            printf("invalid entry");
            break;
    }
}


// bitmap
void bitmap_handler(char *cmd, action_t action) {
    int bm_sz;
    int nth;
    int i;

    // split cmd
    parse_cmd(cmd);

    switch (action) {
        case CREATE:
            // create bitmap bm0 16
            nth = ARGS[0][2] - '0';
            bm_sz = str2int(ARGS[1]);
            BITMAPS[nth] = bitmap_create(bm_sz);
            break;

        case DELETE:
            nth = cmd[0] - '0';
            bitmap_destroy(BITMAPS[nth]);
            break;

        case DUMP:
            nth = cmd[0] - '0';
            bm_sz = (int) (bitmap_size(BITMAPS[nth]));
            for (i = 0; i < bm_sz; i++) {
                printf("%d", (int) (bitmap_test(BITMAPS[nth], i)));
            }
            printf("\n");
            break;

        case METHOD:
            nth = ARGS[1][2] - '0';
            int idx1, num;

            if (strncmp(ARGS[0], "size", 4) == 0) {
                // bitmap_size bm0
                printf("%d\n", (int) (bitmap_size(BITMAPS[nth])));
            }
            else if (strncmp(ARGS[0], "mark", 4) == 0) {
                // bitmap_mark bm0 0: [0]을 true로
                idx1 = str2int(ARGS[2]);
                bitmap_mark(BITMAPS[nth], idx1);
            }
            else if (strncmp(ARGS[0], "dump", 4) == 0) {
                // bitmap_dump bm0
                bitmap_dump(BITMAPS[nth]);
            }
            else if (strncmp(ARGS[0], "test", 4) == 0) {
                // bitmap_test bm0 4
                idx1 = str2int(ARGS[2]);
                if (bitmap_test(BITMAPS[nth], idx1)) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "all", 3) == 0) {
                // bitmap_all bm0 0 1
                // all set true?
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                if (bitmap_all(BITMAPS[nth], idx1, num)) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "any", 3) == 0) {
                // bitmap_any bm0 0 1
                // any set true?
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                if (bitmap_any(BITMAPS[nth], idx1, num)) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "contains", 8) == 0) {
                // [a]부터 n개가 하나라도 contain true?
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                bool val = false;
                if (strncmp(ARGS[4], "true", 4) == 0) {
                    val = true;
                }
                if (bitmap_contains(BITMAPS[nth], idx1, num, val)) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }
            else if (strncmp(ARGS[0], "count", 5) == 0) {
                // bitmap_count bm0 0 8 true
                // [0]부터 8개 true 몇 개?
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                bool val = false;
                if (strncmp(ARGS[4], "true", 4) == 0) {
                    val = true;
                }
                printf("%d\n", (int) bitmap_count(BITMAPS[nth], idx1, num, val));
            }
            else if (strncmp(ARGS[0], "set_multiple", 12) == 0) {
                // bitmap_set_multiple bm0 0 4 true
                // [0] 부터 4개 true로 set
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                bool val = false;
                if (strncmp(ARGS[4], "true", 4) == 0) val = true;

                bitmap_set_multiple(BITMAPS[nth], idx1, num, val);
            }
            else if (strncmp(ARGS[0], "set_all", 7) == 0) {
                // bitmap_set_all bm0 false
                bool val = false;
                if (strncmp(ARGS[2], "true", 4) == 0) val = true;
                bitmap_set_all(BITMAPS[nth], val);
            }
            else if (strncmp(ARGS[0], "set", 3) == 0) {
                // bitmap_set bm0 0 true
                idx1 = str2int(ARGS[2]);
                bool val = false;
                if (strncmp(ARGS[3], "true", 4) == 0) val = true;

                bitmap_set(BITMAPS[nth], idx1, val);
            }
            else if (strncmp(ARGS[0], "reset", 5) == 0) {
                // bitmap_reset bm0 0
                idx1 = str2int(ARGS[2]);
                bitmap_reset(BITMAPS[nth], idx1);
            }
            else if (strncmp(ARGS[0], "flip", 4) == 0) {
                // bitmap_flip bm0 4
                idx1 = str2int(ARGS[2]);
                bitmap_flip(BITMAPS[nth], idx1);
            }
            else if (strncmp(ARGS[0], "none", 4) == 0) {
                // bitmap_none bm0 0 1
                // [0] 부터 1개 전부다 none이냐?
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                if (bitmap_none(BITMAPS[nth], idx1, num)) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
            }


            else if (strncmp(ARGS[0], "scan_and_flip", 13) == 0) {
                // bitmap_scan_and_flip bm0 0 1 true
                // [0] 부터 1개 탐색, 처음 true인 것을 찾아서 flip
                // 찾은 원소의 index를 리턴함
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                bool val = false;
                if (strncmp(ARGS[4], "true", 4) == 0) val = true;

                size_t ret = bitmap_scan_and_flip(BITMAPS[nth], idx1, num, val);
                if (ret == BITMAP_ERROR) {
                    printf("18446744073709551615\n");
                }
                else {
                    printf("%d\n", (int) ret);
                }
            }
            else if (strncmp(ARGS[0], "scan", 4) == 0) {
                // bitmap_scan bm0 0 1 true
                idx1 = str2int(ARGS[2]);
                num = str2int(ARGS[3]);
                bool val = false;
                if (strncmp(ARGS[4], "true", 4) == 0) val = true;

                size_t ret = bitmap_scan(BITMAPS[nth], idx1, num, val);
                if (ret == BITMAP_ERROR) {
                    printf("18446744073709551615\n");
                }
                else {
                    printf("%d\n", (int) ret);
                }
            }
            else if (strncmp(ARGS[0], "expand", 6) == 0) {
                // bitmap_expand bm0 4
                // 자리 4개 추가
                num = str2int(ARGS[2]);
                bitmap_expand(BITMAPS[nth], num);
            }
            break;

        default:
            printf("invalid entry");
            break;
    }
}