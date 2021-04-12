#include "dillist.h"
#include <time.h>

void InsertList(int threadNum, int N, DLList *list) {
    RandomInit(unsigned(time(0) + threadNum));
    for (int i = 0; i < N; i++) {
        void *item;
        int key = Random() % 100;         //生成节点的随机值
        int *items = new int[1];
        items[0] = -key;
        item = items;
        printf("Thread %d: Inserted key %2d | ", threadNum, key);
        list->SortedInsert(item, key);
        list->printList(list);
    }
}

void RemoveList(int threadNum, int N, DLList *list) {
    for (int i = 0; i < N; i++) {
        int key;
        printf("Thread %d: Removed  key ", threadNum);
        list->Remove(&key);
        list->printList(list);
    }
}
