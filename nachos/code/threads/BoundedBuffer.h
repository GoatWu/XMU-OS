#include "synch.h"
#include "system.h"

class BoundedBuffer {
public:
    // create a bounded buffer with a limit of 'maxsize' bytes
    BoundedBuffer(int maxsize);
    ~BoundedBuffer();

    // read 'size' bytes from the bounded buffer, storing into 'data'.
    // ('size' may be greater than 'maxsize')
    void Read(char *data, int size);

    // write 'size' bytes from 'data' into the bounded buffer.
    // ('size' may be greater than 'maxsize')
    void Write(char *data, int size);

    void printBuffer();

private:
    // ???
    int bufferSize;
    char *buffer;
    char *readLoc, *writeLoc;

    Semaphore *sEmpty;
    Semaphore *sFull;
    Semaphore *mutex;
};

