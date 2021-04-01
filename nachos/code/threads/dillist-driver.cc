#include "dillist.h"
#include <time.h>

void InsertList(int threadNum, int N, DLList *list) {
    RandomInit(unsigned(time(0) + threadNum));
    for (int i = 0; i < N; i++) {
        void *item;
        int key = Random() % 100;
        int *items = new int[1];
        items[0] = -key;
        item = items;
        list->SortedInsert(item, key);
        printf("Thread %d: Inserted key %2d | ", threadNum, key);
        list->printList(list);
    }
}

void RemoveList(int threadNum, int N, DLList *list) {
    for (int i = 0; i < N; i++) {
        int key;
        list->Remove(&key);
        printf("Thread %d: Removed  key %2d | ", threadNum, key);
        list->printList(list);
    }
}
