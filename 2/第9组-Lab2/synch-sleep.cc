// synch.cc 
//  Routines for synchronizing threads.  Three kinds of
//  synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//  are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//  Initialize a semaphore, so that it can be used for synchronization.
//
//  "debugName" is an arbitrary name, useful for debugging.
//  "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//  De-allocate semaphore, when no longer needed.  Assume no one
//  is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//  Wait until semaphore value > 0, then decrement.  Checking the
//  value and decrementing must be done atomically, so we
//  need to disable interrupts before checking the value.
//
//  Note that Thread::Sleep assumes that interrupts are disabled
//  when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // disable interrupts
    
    while (value == 0) {            // semaphore not available
    queue->Append((void *)currentThread);   // so go to sleep
    currentThread->Sleep();
    } 
    value--;                    // semaphore available, 
                        // consume its value
    
    (void) interrupt->SetLevel(oldLevel);   // re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//  Increment semaphore value, waking up a waiter if necessary.
//  As with P(), this operation must be atomic, so we need to disable
//  interrupts.  Scheduler::ReadyToRun() assumes that threads
//  are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)    // make thread ready, consuming the V immediately
    scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
    name = debugName;
    mutex = 0;
    queue = new List;
    heldThread = NULL;
}
Lock::~Lock() {
    delete queue;
}
void Lock::Acquire() {
    ASSERT(!isHeldByCurrentThread()); // 当前线程并未占有锁
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // 禁用中断
    while (mutex) { // 锁被占有
        queue->Append((void *)currentThread);
        currentThread->Sleep();
    }
    mutex = 1; // 上锁
    heldThread = currentThread; // 当前线程占有锁
    (void) interrupt->SetLevel(oldLevel);   // 重新开中断
}
void Lock::Release() {
    ASSERT(isHeldByCurrentThread()); // 当前线程占有锁
    IntStatus oldLevel = interrupt->SetLevel(IntOff);   // 禁用中断
    Thread *thread = (Thread *)queue->Remove(); // 取sleep队列头部的线程
    if (thread != NULL) {
        scheduler->ReadyToRun(thread);  // 唤醒线程，进入准备运行态
    }
    mutex = 0; // 释放锁
    heldThread = NULL; // 无线程占有锁
    (void) interrupt->SetLevel(oldLevel);   // 重新开中断
}
bool Lock::isHeldByCurrentThread() {
    return currentThread == heldThread;
}

Condition::Condition(char* debugName) {
    name = debugName;
    queue = new List;
    heldLock = NULL;
}
Condition::~Condition() {
    delete queue;
}
void Condition::Wait(Lock* conditionLock) {
    conditionLock->Release(); // 释放当前占据的锁，准备进入休眠
    heldLock = NULL;
    queue->Append((void *)currentThread); // 当前线程加入休眠队列
    // nachos要求线程休眠前必须要关中断
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    currentThread->Sleep();
/*---------- 进入readtorun 后重新被唤醒 ----------*/
    conditionLock->Acquire(); // 重新获得锁
    heldLock = conditionLock;
    (void) interrupt->SetLevel(oldLevel);   // 重新开中断
}
void Condition::Signal(Lock* conditionLock) { 
    ASSERT(heldLock == conditionLock || heldLock == NULL);
    Thread *thread = (Thread *)queue->Remove();
    if (thread != NULL) {
        scheduler->ReadyToRun(thread);
    }
}
void Condition::Broadcast(Lock* conditionLock) { 
    ASSERT(heldLock == conditionLock);
    Thread *thread = (Thread *)queue->Remove();
    while (thread != NULL) {
        scheduler->ReadyToRun(thread);
        thread = (Thread *)queue->Remove();
    }
}
