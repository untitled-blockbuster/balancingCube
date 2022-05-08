#include <stdlib.h>
#include <Queue.h>

typedef struct Node
{
    double data;
    Node *next;
} Node;

typedef struct QueueStruct
{
    Node *front;
    Node *back;
    int size;
} QueueStruct;

void InitQueue(QueueStruct **queue)
{
    (*queue) = (QueueStruct *)malloc(sizeof(QueueStruct));
    (*queue)->front = (*queue)->back = NULL;
    (*queue)->size = 0;
}

Node *CreateNode(int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}
void EnQueue(QueueStruct **queue, Node *node)
{
    if ((*queue)->size == 0)
    {
        (*queue)->front = (*queue)->back = node;
    }
    else
    {
        (*queue)->back->next = node;
        (*queue)->back = node;
    }

    (*queue)->size++;
}

Node *DeQueue(QueueStruct **queue)
{
    Node *n = (*queue)->front;
    (*queue)->front = n->next;
    (*queue)->size--;
    return n;
}
void DestroyQueue(QueueStruct **queue)
{
    int cnt = (*queue)->size;
    for (int i = 0; i < cnt; i++)
    {
        Node *n = DeQueue(queue);
        free(n);
    }
    (*queue)->front = (*queue)->back = NULL;
    (*queue)->size = 0;
    free(*queue);
}

Queue::Queue()
{
    InitQueue(&q);
}
void Queue::push(double value)
{
    EnQueue(&q, CreateNode(value));
}
double Queue::pop()
{
    Node *n = DeQueue(&q);
    double output = n->data;
    free(n);
    return output;
}
void Queue::clear()
{
    DestroyQueue(&q);
}