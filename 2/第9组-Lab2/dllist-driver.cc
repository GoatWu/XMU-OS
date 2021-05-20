#include "dllist.h"
#include <time.h>

void InsertList(int threadNum, int N, SynchDLList *slist) {
    RandomInit(unsigned(time(0) + threadNum));
    for (int i = 0; i < N; i++) {
        void *item;
        int key = Random() % 100;         //生成节点的随机值
        int *items = new int[1];
        items[0] = -key;
        item = items;
        printf("Thread %d: Inserted key %2d\n", threadNum, key);
        slist->SortedInsert(item, key);
        printf("Thread %d: ", threadNum);
        slist->printList();
    }
}

void RemoveList(int threadNum, int N, SynchDLList *slist) {
    for (int i = 0; i < N; i++) {
        int key;
        printf("Thread %d: Removed  key ", threadNum);
        slist->Remove(&key);
        printf("%2d\n", key);
        printf("Thread %d: ", threadNum);
        slist->printList();
    }
}
