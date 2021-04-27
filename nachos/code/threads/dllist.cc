#include "dllist.h"

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
    printf("%2d |", first->key);                //输出要删除的元素的值
    if (this->errType == 7) {                 //（错误七）找出了要删除的元素，但还并未将其删除就切换了线程
        printf("Remove error 1\n");               
        currentThread->Yield();
    }
    RemovedItem = element->item;                
    first = first->next;                        //删除操作
    if (first == NULL) {
        last = NULL;
    }
    else {
        if (this->errType == 8) {            //（错误八）已经删除了元素，但是还未返回删除元素的数值，此时切换
            printf("Remove error 2\n");
            currentThread->Yield();
        }
        first->prev=NULL;
    }
    return RemovedItem;                     //返回刚刚返回的值
}

void DLList::SortedInsert(void *item, int sortKey) {
    if (errType == 1) {                                    //（错误一）在应当插入元素之前就切换了线程        
        printf("SortedInsert error, Switching before inserting\n");      //可能会造成由线程X插入的元素之后被线程Y删除，而不能被自己删除
        currentThread->Yield();
    }                                                        
    if (this->IsEmpty()) {
        DLLElement *node = new DLLElement(item, sortKey);       //创建新的待插入节点
        first = node;
        node->prev = node->next = NULL;
        last = node;
        if (errType == 2) {                                    //（错误二）链表为空的情况下，插入该节点之后立即就切换了进程,同样会造成
            printf("SortedInsert error, insert one and switch immediately \n");      //由线程X插入的元素之后被线程Y删除，而不能被自己删除
            currentThread->Yield();
        }
    }
    else {
        DLLElement *node = new DLLElement(item,sortKey);
        DLLElement *inst = first;
        while (inst != NULL && sortKey >= inst->key) {      //找出插入的位置
            inst = inst->next;
        }
        
        if (inst == NULL) { //在表尾插入
            node->prev = last;                                 //（错误三）在表尾插入时，只修改了node->prev，而没有修改其他指针
            if (errType == 3) {                                   
            printf("SortedInsert error, pointer error while inserting end of list\n");      
            currentThread->Yield();
        }
            node->next = NULL;
            last->next = node;
            last=node;
        }
        else if (inst == first) { //在表头插入

            if (errType == 4) {                                 //（错误四）此时链表不空，而且要在表头进行插入
                printf("SortedInsert error, switch before insert at the head of list\n");   //但是还未插入该节点就切换了进程
                currentThread->Yield();
            }
            node->prev = NULL;
            node->next = first;
            if (errType == 5) {                                 //（错误五）此时链表不空，而且要在表头进行插入
                printf("SortedInsert error, pointer error while inserting head of list\n");   //但是只将node->next连接，而并未连接first->prev
                currentThread->Yield();
            }
            first->prev = node;
            first = node;
        }
        else {                                               //执行普通的插入操作
            node->prev = inst->prev;                      
            if (errType == 6) {                                //（错误六）在链表中间插入时，还未成功插入该节点就切换了进程
                printf("SortedInsert error, pointer error while inserting middle of list\n");  //只指定了 node->prev而未指定之后指针的操作
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

void DLList::printList() {
    printf("Current List: ");
    for (DLLElement *item = this->first; item != NULL; item = item->next) {
        printf("%2d ", item->key);
    }
    printf("\n");
    putchar(10);
}

SynchDLList::SynchDLList(int errType) {
    list = new DLList(errType);
    lock = new Lock("Lock for DLList");
    empty = new Condition("List empty");
}

SynchDLList::~SynchDLList() {
    delete list;
    delete lock;
    delete empty;
}

void *SynchDLList::Remove(int *keyPtr) {
    void *item;
    lock->Acquire();
    while (list->IsEmpty()) {
        empty->Wait(lock);
    }
    item = list->Remove(keyPtr);
    lock->Release();
    return item;
}

void SynchDLList::SortedInsert(void *item, int sortKey) {
    lock->Acquire();
    list->SortedInsert(item, sortKey);
    empty->Signal(lock);
    lock->Release();
}

void SynchDLList::printList() {
    list->printList();
}
