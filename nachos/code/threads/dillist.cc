#include "dillist.h"

DLLElement::DLLElement( void *inodetr, int sortKey ) {
	next = prev = NULL;
	item = inodetr;
	key = sortKey;
}

DLList::DLList() {
    first = last = NULL;
}

DLList::~DLList() {}

bool DLList::IsEmpty() {
	return first == NULL;
}

void DLList::Prepend(void *item) {
	if (this->IsEmpty()) {
		DLLElement *node = new DLLElement(item, INITKEY);
		node->prev = node->next = NULL;
		first = last = node;
	}
	else {
		DLLElement *node = new DLLElement(item, first->key - 1);
		node->prev = NULL;
		node->next = first;
		first->prev = node;
		first = node;
	}
}

void DLList::Append(void *item) {
	if (this->IsEmpty()) {
		DLLElement *node = new DLLElement(item, INITKEY);
		node->prev = node->next = NULL;
		first = last = node;
	}
	else {
		DLLElement *node = new DLLElement(item, last->key + 1);
		node->prev = last;
		node->next = NULL;
		last->next = node;
		last = node;
	}
}

void *DLList::Remove(int *keyPtr) {
    void *RemovedItem;
    if (this->IsEmpty()) {
        return keyPtr = NULL;
    }
    else {
        *keyPtr = first->key;
        RemovedItem = first->item;
        first = first->next;
        if (first == NULL) {
            last = NULL;
        }
        else {
            first->prev=NULL;
        }
    }
    return RemovedItem;
}

void DLList::SortedInsert(void *item, int sortKey) {
    if (this->IsEmpty()) {
        DLLElement *node = new DLLElement(item, sortKey);
        node->prev = node->next = NULL;
        first = last = node;
    }
    else {
        DLLElement *node = new DLLElement(item,sortKey);
        DLLElement *inst = first;
        while (inst != NULL && sortKey >= inst->key) {
            inst = inst->next;
        }
        if (inst == NULL) {	//在表尾插入
        	node->prev = last;
        	node->next = NULL;
        	last->next = node;
        	last=node;
        }
        else if (inst == first) { //在表头插入
        	node->prev = NULL;
        	node->next = first;
        	first->prev = node;
        	first = node;
        }
        else {
            node->prev = inst->prev;
            inst->prev->next = node;
            inst->prev = node;
            node->next = inst;
        }
    }
}


void *DLList::SortedRemove(int sortKey) {
    void *ReturnItem;
    if (this->IsEmpty()) {
        return NULL;
    }
    else {
        DLLElement *delt = first;
        while (delt != NULL && sortKey != delt->key) {
            delt=delt->next;
        }
        if (delt == NULL) {
            return NULL;
        }
        else {
            ReturnItem = delt->item;
            if (delt == first) {
                int *key;
                this->Remove(key);
            }
            else if (delt == last) {
                last = last->prev;
                last->next = NULL;
                if (last == NULL) {
                    first = NULL;
                }
            }
            else {
                delt->prev->next = delt->next;
                delt->next->prev = delt->prev;
            }
            return ReturnItem;
        }
    }

}

void DLList::printList(DLList *list) {
	printf("Current List: ");
    for (DLLElement *item = list->first; item != NULL; item = item->next) {
        printf("%2d ", item->key);
    }
    putchar(10);
}

