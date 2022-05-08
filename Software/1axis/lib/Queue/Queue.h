#ifndef _Queue
#define _Queue
#include <stdlib.h>

typedef struct Node;
typedef struct QueueStruct;

void InitQueue(QueueStruct **queue);

Node *CreateNode(int data);
void EnQueue(QueueStruct **queue, Node *node);
Node *DeQueue(QueueStruct **queue);
void DestroyQueue(QueueStruct **queue);

class Queue
{
public:
    QueueStruct *q = NULL;
    Queue();
    void push(double value);
    double pop();
    void clear();
};
#endif