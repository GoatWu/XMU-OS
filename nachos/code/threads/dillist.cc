#include "dillist.h"

DLLElement::DLLElement( void *inodetr, int sortKey ) {
	next = prev = NULL;
	item = inodetr;
	key = sortKey;
}

DLList::DLList(int Type) {
    first = last = NULL;
    this->errType = Type;
}

DLList::~DLList() {
    while (Remove(NULL) != NULL);
}

bool DLList::IsEmpty() {
	if (first == NULL && last == NULL) return true;
    else if (first != NULL && last != NULL) return false;
    printf("Error! The list is empty but *first or *last isn't NULL!\n");
    return false;
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
    DLLElement *element;
    void *RemovedItem;
    if (this->IsEmpty()) {
        return keyPtr = NULL;
    }
    element = first;
    *keyPtr = first->key;
    if (this->errType == 1) {
        printf("Remove error\n");
        currentThread->Yield();
    }
    RemovedItem = element->item;
    first = first->next;
    if (first == NULL) {
        last = NULL;
    }
    else {
        if (this->errType == 1) {
            printf("Remove error\n");
            currentThread->Yield();
        }
        first->prev=NULL;
    }
    return RemovedItem;
}

void DLList::SortedInsert(void *item, int sortKey) {
    if (this->IsEmpty()) {
        DLLElement *node = new DLLElement(item, sortKey);
        first = node;
        if (errType == 2) {
            printf("SortedInsert error, first != last\n");
            currentThread->Yield();
        }
        node->prev = node->next = NULL;
        last = node;
    }
    else {
        DLLElement *node = new DLLElement(item,sortKey);
        DLLElement *inst = first;
        while (inst != NULL && sortKey >= inst->key) {
            inst = inst->next;
        }
        if (errType == 3) {
            printf("SortedInsert error, the postion lost\n");
            currentThread->Yield();
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
            if (errType == 4) {
                printf("SortedInsert error, sorting error\n");
                currentThread->Yield();
            }

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

