#ifndef _Queue
#define _Queue
#include <stdlib.h>

typedef struct Node
{
    double data;
    Node *next;
} Node;

typedef struct QueueStruct
{
    Node *front;
    Node *back;
    int size = 0;
} QueueStruct;

void initQueue();
Node *createNode();
void endQueue();
Node *deQueue();
void destroyQueue();

class Queue
{
private:
    QueueStruct *q = NULL;
    double sum = 0;
    void push(double value);
    double pop();

public:
    int size;
    double avg;
    Queue(int size);
    void clear();
    double UpdateQ(double value);
};
#endif