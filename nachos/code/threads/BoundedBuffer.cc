#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int maxsize) {
	bufferSize = maxsize;
	buffer = new char[bufferSize];
	sEmpty = new Semaphore("empty places", bufferSize);
	sFull = new Semaphore("used places", 0);
	mutex = new Semaphore("mutex of buffer", 1);
	for (int i = 0; i < bufferSize; i++) {
		buffer[i] = 0;
	}
	readLoc = writeLoc = buffer;
}

BoundedBuffer::~BoundedBuffer() {
	delete sEmpty;
	delete sFull;
	delete mutex;
	delete buffer;
}

void BoundedBuffer::Read(char *data, int size) {
	for (int i = 0; i < size; i++) {
		sFull->P();
		mutex->P();
		/*临界区*/
		data[i] = *readLoc;
		*readLoc = 0;
		++readLoc;
		if (readLoc == buffer + bufferSize) {
			readLoc = buffer;
		}
		printf("Reader (Thread %p): %c\n", currentThread, data[i]);
		printBuffer();

		mutex->V();
		sEmpty->V();
	}
	data[size] = 0;
}

void BoundedBuffer::Write(char *data, int size) {
	for (int i = 0; i < size; i++) {
		sEmpty->P();
		mutex->P();
		/*临界区*/
		*writeLoc = data[i];
		++writeLoc;
		if (writeLoc == buffer + bufferSize) {
			writeLoc = buffer;
		}
		printf("Writer (Thread %p): %c\n", currentThread, data[i]);
		printBuffer();

		mutex->V();
		sFull->V();
	}
}

void BoundedBuffer::printBuffer() {
	printf("buffer:");
	for (int i = 0; i < bufferSize; i++) {
		printf("%c", buffer[i] ? buffer[i] : ' ');
	}
	printf("\n");
}
