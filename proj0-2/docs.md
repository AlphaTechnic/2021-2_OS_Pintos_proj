# Additional Implementation



## parse_cmd()

- Prototype
  - void parse_cmd(char *cmd)
- Parameter
  - char *cmd
- Return
  - none
- Function
  - 사용자로부터 입력받은 command를 의미 단위로 parsing하여 ARGS[]에 저장한다.
  - 이를테면, 사용자로부터 `create list list0` 를 입력 받았을 때,
    - ARGS[0] = create
    - ARGS[1] = list
    - ARGS[2] = list0
    - 위 처럼 입력 의미 단위로 parsing하여 관리한다.



## str2int()

- Prototype
  - int str2int(const char *)
- Parameter
  - const char * str_num
- Return
  - int형 number
- Function
  - 사용자로부터 입력받은 command는 1차적으로 문자열 형태로 프로그램에 넘어오게 된다.
  - '10'과 같이 string으로 표현된 데이터를 `10`처럼 int형 자료로 변환한다.



## list_swap()

- Prototype
  - void list_swap(struct list_elem *elem1, struct list_elem *elem2)
- Parameter
  - struct list_elem *elem1
  - struct list_elem *elem2
- Return
  - none
- Function
  - list element 2개를 인자로 받아, 리스트 내 데이터의 위치를 뒤바꾼다.



## list_shuffle()

- Prototype
  - void list_shuffle(struct list *lst)
- Parameter
  - struct list *lst (shuffle이 완료된 list)
- Return
  - none
- Function
  - 리스트 내의 원소들을 랜덤하게 뒤섞는 함수이다.



## bitmap_expand()

- Prototype
  - struct bitmap *bitmap_expand(struct bitmap *bm, int add_sz)
- Parameter
  - struct bitmap *bm
  - int add_sz
- Return
  - struct bitmap* (expand가 적용된 bitmap)
- Function
  - bitmap이 관리하는 bit size를 add_sz만큼 더 늘린다.



## hash_int_2()

- Prototype
  - unsigned hash_int_2(int i)
- Parameter
  - int i
- Return
  - unsigned int (hash 함수를 적용한 값)
- Function
  - 인자 i에 대한 hash 값을 리턴한다.



## hash_hash_function()

- Prototype
  - unsigned hash_hash_function(const struct hash_elem *e, void *aux);
- Parameter
  - const struct hash_elem *e
  - void *aux
- Return
  - unsigned hash value (hash 함수가 적용된 값)
- Function
  - 인자로 받는 hash element에 대하여 대응하는 hash 값을 리턴한다.



## hash_less_function()

- Prototype
  - bool hash_less_function(const struct hash_elem *a, const struct hash_elem *b, void *aux)
- Parameter
  - const struct hash_elem *a
  - const struct hash_elem *b
  - void *aux
- Return
  - bool true or false
- Function
  - hash 원소들에 대한 comparator 기능을 수행한다.



## hash_action_on_element()

- Prototype
  - void hash_action_on_element(struct hash_elem *e, void *aux)
- Parameter
  - struct hash_elem *e
  - void *aux
- Return
  - none
- Function
  - 인자로 입력된 hash element에 대하여 *aux로 입력된 hash 함수를 적용한다.



## hash_action_destructor()

- Prototype
  - void hash_action_destructor(struct hash_elem *e, void *aux)
- Parameter
  - struct hash_elem *e
  - void *aux
- Return
  - none
- Function
  - hash element를 제거한다.
  - hash element에 대응하는 hash item이 차지하는 메모리공간을 free 시킨다.



## list_handler()

- Prototype
  - void list_handler(char *, action_t action)
- Parameter
  - char *
  - action_t action
- Return
  - none
- Function
  - 사용자로부터 입력받은 list 관련 command를 핸들링한다.



## hash_handler()

- Prototype
  - void hash_handler(char *, action_t action)
- Parameter
  - char *
  - action_t action
- Return
  - none
- Function
  - 사용자로부터 입력받은 hash 관련 command를 핸들링한다.



## bitmap_handler()

- Prototype
  - void bitmap_handler(char *, action_t action)
- Parameter
  - char *
  - action_t action
- Return
  - none
- Function
  - 사용자로부터 입력받은 bitmap 관련 command를 핸들링한다.





# List



## list_next()

- Prototype
  - struct list_elem* list_next (struct list_elem *elem)
- Parameter
  - struct list_elem *elem
- Return
  - struct list_elem *elem (next list element)
- Function
  - 인자로 입력받은 list element의 다음 list element를 리턴한다.



## list_end()

- Prototype
  - struct list_elem *list_end (struct list *list)
- Parameter
  - struct list *list
- Return
  - struct list_elem *
- Function
  - 입력받은 list의 가장 마지막 list element를 리턴한다.



## list_insert()

- Prototype
  - void list_insert (struct list_elem *before, struct list_elem *elem)
- Parameter
  - struct list_elem *before
  - struct list_elem *elem
- Return
  - none
- Function
  - 입력받은 before list element의 next 자리에 인자로 주어진 list element를 삽입한다.



## list_splice()

- Prototype
  - void list_splice (struct list_elem *before, struct list_elem *first, struct list_elem *last)
- Parameter
  - struct list_elem *before
  - struct list_elem *first
  - struct list_elem *last
- Return
  - none
- Function
  - 인자로 입력받은 before 다음 자리에 list의 first부터 last까지의 element들을 삽입한다.



## list_push_front()

- Prototype
  - void list_push_front (struct list *list, struct list_elem *elem)
- Parameter
  - struct list *list
  - struct list_elem *elem
- Return
  - none
- Function
  - list의 head에 원소를 삽입한다.



## list_push_back()

- Prototype
  - void list_push_back (struct list *list, struct list_elem *elem)
- Parameter
  - struct list *list
  - struct list_elem *elem
- Return
  - none
- Function
  - 리스트의 tail에 원소를 삽입한다.

 

## list_remove()

- Prototype
  - struct list_elem *list_remove (struct list_elem *elem)
- Parameter
  - struct list_elem *elem
- Return
  - struct list_elem
- Function
  - 인자로 입력받은 list element를 제거한다.
  - list element를 제거하면, 바로 그 next 자리에 있던 element가 해당자리로 오게 되는데, 그 element를 리턴한다.



## list_pop_front()

- Prototype
  - struct list_elem *list_pop_front (struct list *list)
- Parameter
  - struct list *list
- Return
  - struct list_elem*
- Function
  - 리스트의 head 원소를 제거한다



## list_pop_back()

- Prototype
  - struct list_elem *list_pop_back (struct list *list)
- Parameter
  - struct list *list
- Return
  - struct list_elem*
- Function
  - 리스트의 tail 원소를 제거한다.



## list_front()

- Prototype
  - struct list_elem *list_front (struct list *list)
- Parameter
  - struct list *list
- Return
  - struct list_elem*
- Function
  - 리스트 head의 원소를 리턴한다.



## list_back()

- Prototype
  - struct list_elem *list_back (struct list *list)
- Parameter
  - struct list *list
- Return
  - struct list_elem*
- Function
  - 리스트 tail의 원소를 리턴한다.



## list_reverse()

- Prototype
  - void list_reverse (struct list *list)
- Parameter
  - struct list *list
- Return
  - none
- Function
  - 리스트 내의 원소들의 순서를 거꾸로 뒤집는다.



## list_sort()

- Prototype
  - void list_sort (struct list *list, list_less_func *less, void *aux)
- Parameter
  - struct list *list 
  - list_less_func *less
  - void *aux
- Return
  - none
- Function
  - 리스트 내의 원소를 정렬한다.
  - 정렬의 기준은 list_less_func comparator를 따른다.



## list_insert_ordered()

- Prototype
  - void list_insert_ordered (struct list *list, struct list_elem *elem, list_less_func *less, void *aux)
- Parameter
  - struct list *list
  - struct list_elem *elem
  - list_less_func *less
  - void *aux
- Return
  - none
- Function
  - 함수의 인자로 넘어온 원소를 리스트에 삽입하고, 정렬한다.



## list_unique()

- Prototype
  - void list_unique (struct list *list, struct list *duplicates, list_less_func *less, void *aux)
- Parameter
  - struct list *list
  - struct list *duplicates
  - list_less_func *less
  - void *aux
- Return
  - none
- Function
  - 리스트내에 중복된 원소를 제거한다.
  - 인자로 들어온 struct list *duplicates에 새로운 list를 넘기면, 해당 리스트에 unique 원소들을 삽입하게 된다.



## list_max()

- Prototype
  - struct list_elem *list_max (struct list *list, list_less_func *less, void *aux)
- Parameter
  - struct list *list
  - list_less_func *less
  - void *aux
- Return
  - struct list_elem* (max값을 가지는 list element)
- Function
  - 리스트 내의 원소 중 최대값의 원소를 리턴한다.



## list_min()

- Prototype
  - struct list_elem *list_min (struct list *list, list_less_func *less, void *aux)
- Parameter
  - struct list *list
  - list_less_func *less
  - void *aux
- Return
  - struct list_elem* (min값을 가지는 list element)
- Function
  - 리스트 내의 원소 중 최솟값의 원소를 리턴한다.



# Hash Table



## hash_init()

- Prototype
  - bool hash_init (struct hash *h, hash_hash_func *hash, hash_less_func *less, void *aux)
- Parameter
  - struct hash *h
  - hash_hash_func *hash
  - hash_less_func *less
  - void *aux
- Return
  - bool true or false
- Function
  - 파라미터로 들어온 정보들을 적용하여 hash를 초기화한다.



## hash_destroy()

- Prototype
  - void hash_destroy (struct hash *h, hash_action_func *destructor)
- Parameter
  - struct hash *h
  - hash_action_func *destructor
- Return
  - none
- Function
  - hash를 메모리 공간에서 free시킨다.



## hash_insert()

- Prototype
  - struct hash_elem *hash_insert (struct hash *h, struct hash_elem *new)
- Parameter
  - struct hash *h 
  - struct hash_elem *new
- Return
  - struct hash_elem*
- Function
  - hash에 원소를 삽입한다.



## hash_find()

- Prototype
  - struct hash_elem *hash_find (struct hash *h, struct hash_elem *e)
- Parameter
  - struct hash *h
  - struct hash_elem *e
- Return
  - struct hash_elem*
- Function
  - hashmap에서 인자로 주어진 hash element를 찾아 리턴한다.



## hash_delete()

- Prototype
  - struct hash_elem *hash_delete (struct hash *h, struct hash_elem *e)
- Parameter
  - struct hash *h
  - struct hash_elem *e
- Return
  - struct hash_elem*
- Function
  - hashmap에서 인자로 주어진 hash element를 찾아 제거한다.



## hash_apply()

- Prototype
  - void hash_apply (struct hash *h, hash_action_func *action)
- Parameter
  - struct hash *h
  - hash_action_func *action
- Return
  - none
- Function
  - 주어진 hashmap에 인자로 주어진 hash함수를 적용한다.



## hash_first()

- Prototype
  - void hash_first (struct hash_iterator *i, struct hash *h)
- Parameter
  - struct hash_iterator *i
  - struct hash *h
- Return
  - none
- Function
  - hash_iterator 가 hashmap의 첫번째 원소를 가리킬 수 있도록 작업한다.



## hash_next()

- Prototype
  - struct hash_elem *hash_next (struct hash_iterator *i)
- Parameter
  - struct hash_iterator *i
- Return
  - struct hash_elem*
- Function
  - hash_iterator가 현재 가리키고 있는 원소의 다음 원소를 가리키도록 작업한다.



## hash_int()

- Prototype
  - unsigned hash_int (int i)
- Parameter
  - int i
- Return
  - unsigned value
- Function
  - 인자 i에 대한 hash 값을 리턴한다.



# Bitmap

## bitmap_create()

- Prototype
  - struct bitmap * bitmap_create (size_t bit_cnt)
- Parameter
  - size_t bit_cnt
- Return
  - struct bitmap* (초기화된 bitmap)
- Function
  - 인자로 입력받은 bit_cnt만큼 bitmap을 초기화한다.



## bitmap_destroy()

- Prototype
  - void bitmap_destroy (struct bitmap *b)
- Parameter
  - struct bitmap *b
- Return
  - none
- Function
  - 인자로 입력받은 bitmap에 할당된 메모리공간을 free시킨다.



## bitmap_set()

- Prototype
  - void bitmap_set (struct bitmap *b, size_t idx, bool value)
- Parameter
  - struct bitmap *b
  - size_t idx, bool value
- Return
  - none
- Function
  - 인자로 입력받은 위치의 bit를 입력받은 value 값으로 set한다.



## bitmap_mark()

- Prototype
  - void bitmap_mark (struct bitmap *b, size_t bit_idx)
- Parameter
  - struct bitmap *b
  - size_t bit_idx
- Return
  - none
- Function
  - 인자로 입력받은 위치의 bit를 입력받은 true 값으로 set한다.



## bitmap_reset()

- Prototype
  - void bitmap_reset (struct bitmap *b, size_t bit_idx)
- Parameter
  - struct bitmap *b
  - size_t bit_idx
- Return
  - none
- Function
  - 인자로 입력받은 위치의 bit를 입력받은 false 값으로 reset한다.



## bitmap_flip()

- Prototype
  - void bitmap_flip (struct bitmap *b, size_t bit_idx)
- Parameter
  - struct bitmap *b
  - size_t bit_idx
- Return
  - none
- Function
  - 인자로 입력받은 위치의 bit를 flip 시킨다.
  - flip이란 기존의 값이 true이면 false로 reset하고, 기존의 값이 false이면 true로 set하는 동작을 말한다.



## bitmap_test()

- Prototype
  - bool bitmap_test (const struct bitmap *b, size_t idx)
- Parameter
  - const struct bitmap *b
  - size_t idx
- Return
  - bool true or false
- Function
  - bitmap의 특정 index에 설정된 값을 리턴한다.



## bitmap_set_all()

- Prototype
  - void bitmap_set_all (struct bitmap *b, bool value)
- Parameter
  - struct bitmap *b
  - bool value
- Return
  - none
- Function
  - bitmap이 관리하는 모든 범위의 값을 인자로 입력받은 value로 설정한다.



## bitmap_set_multiple()

- Prototype
  - void bitmap_set_multiple (struct bitmap *b, size_t start, size_t cnt, bool value)
- Parameter
  - struct bitmap *b
  - size_t start, size_t cnt
  - bool value
- Return
  - none
- Function
  - [start] 인덱스부터 cnt개의 원소를 value로 설정한다.

 

## bitmap_count()

- Prototype
  - size_t bitmap_count (const struct bitmap *b, size_t start, size_t cnt, bool value)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt, bool value
- Return
  - size_t bitmap의 개수
- Function
  - bitmap에서 특정 value(true or false)로 설정된 값이 몇개인지 리턴한다.



## bitmap_contains()

- Prototype
  - bool bitmap_contains (const struct bitmap *b, size_t start, size_t cnt, bool value)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt
  - bool value
- Return
  - bool true or false
- Function
  - [start] 인덱스부터 cnt개의 원소에서 인자로 입력받은 value의 값을 하고 있는 원소가 있는지 check한다.



## bitmap_any()

- Prototype
  - bool bitmap_any (const struct bitmap *b, size_t start, size_t cnt)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt
- Return
  - bool true or false
- Function
  - [start] 인덱스부터 cnt개의 원소에서 인자로 입력받은 value의 값을 하고 있는 원소가 하나라도 있다면, true를 리턴한다.



## bitmap_none()

- Prototype
  - bool bitmap_none (const struct bitmap *b, size_t start, size_t cnt)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt
- Return
  - bool true or false
- Function
  - [start] 인덱스부터 cnt개의 원소에서 bit가 설정되어 있지 않다면(모두 false라면), true를 리턴한다.



## bitmap_all()

- Prototype
  - bool bitmap_all (const struct bitmap *b, size_t start, size_t cnt)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt
- Return
  - bool true or false
- Function
  - [start] 인덱스부터 cnt개의 원소에서, 모든 원소가 인자로 입력받은 value의 값을 하고 있다면, true를 리턴한다.



## bitmap_scan()

- Prototype
  - size_t bitmap_scan (const struct bitmap *b, size_t start, size_t cnt, bool value)
- Parameter
  - const struct bitmap *b
  - size_t start
  - size_t cnt
  - bool value
- Return
  - size_t bitmap 원소의 index
- Function
  - [start] 인덱스부터 cnt개의 원소에서 value로 설정된 bit가 있다면, true를 리턴한다.



## bitmap_scan_and_flip()

- Prototype
  - size_t bitmap_scan_and_flip (struct bitmap *b, size_t start, size_t cnt, bool value)
- Parameter
  - struct bitmap *b
  - size_t start
  - size_t cnt
  - bool value
- Return
  - size_t bitmap 원소의 index
- Function
  - [start] 인덱스부터 cnt개의 원소를 탐색하다가 value로 설정된 bit가 있다면 해당 bit의 값을 flip시킨다.

