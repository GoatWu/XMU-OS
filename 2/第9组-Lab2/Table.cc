#include "Table.h"

Table::Table(int size) {
	tableSize = size;
	tableLock = new Lock("tableLock");
	table = new void *[size];
	used = new int[size];
	for (int i = 0; i < size; i++) {
		used[i] = 0;
	}
}

Table::~Table() {
	delete[] table;
	delete tableLock;
	delete used;
}

int Table::Alloc(void *object) {
	tableLock->Acquire();
	printf("Thread\t%p\talloc:\t\t", currentThread);
	for (int i = 0; i < tableSize; i++) {
		if (!used[i]) {
			used[i] = 1;
			table[i] = object;
			printf("table[%d]:%d\n", i, *(int *)object);
			tableLock->Release();
			return i;
		}
	}
	printf("alloc fail\n");
	tableLock->Release();
	return -1;
}

void *Table::Get(int index) {
	tableLock->Acquire();
	printf("Thread\t%p\tget:\t\t", currentThread);
	if (index >= 0 && index < tableSize) {
		if (used[index]) {
			printf("table[%d]:%d\n", index, *(int *)table[index]);
			tableLock->Release();
			return table[index];
		}
		else {
			printf("table[%d] is empty\n", index);
		}
	}
	else {
		printf("index %d is out of range\n", index);
	}
	tableLock->Release();
	return NULL;
}

void Table::Release(int index) {
	tableLock->Acquire();
	printf("Thread\t%p\trelease:\t", currentThread);
	if (index >= 0 && index < tableSize) {
		if (used[index]) {
			used[index] = 0;
			table[index] = NULL;
			printf("table[%d] is released\n", index);
		}
		else {
			printf("table[%d] is empty\n", index);
		}
	}
	else {
		printf("index %d is out of range\n", index);
	}
	tableLock->Release();
	return;
}

