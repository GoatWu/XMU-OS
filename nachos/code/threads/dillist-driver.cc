#include "dillist.h"

void InsertList(int N, DLList *list) {
    for (int i = 0; i < N; i++) {
        void *item;
        int key = Random() % 100;
        int *items = new int[1];
        items[0] = -key;
        item = items;
        // printf("%s in:%d %d\n", currentThread->getName(), key, *(int*)item);
        list->SortedInsert(item, key);
        printf("Insert: %2d | ", key);
        list->printList(list);
    }
}

void RemoveList(int N, DLList *list) {
    for (int i = 0; i < N; i++) {
        int key;
        void *item = list->Remove(&key);
        // printf("%s out:%d %d\n",currentThread->getName(), key, *(int*)item);
        printf("Remove: %2d | ", key);
        list->printList(list);
    }
}