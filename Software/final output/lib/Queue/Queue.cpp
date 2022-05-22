#include <stdlib.h>
#include <Queue.h>

void initQueue(QueueStruct **queue)
{
    (*queue) = (QueueStruct *)malloc(sizeof(QueueStruct));
    (*queue)->front = (*queue)->back = NULL;
    (*queue)->size = 0;
}

Node *createNode(double data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void enQueue(QueueStruct **queue, Node *node)
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
Node *deQueue(QueueStruct **queue)
{
    Node *n = (*queue)->front;
    (*queue)->front = n->next;
    (*queue)->size--;
    return n;
}
int getNodeCount(QueueStruct **queue)
{
    return (*queue)->size;
}

void destroyQueue(QueueStruct **queue)
{
    int cnt = getNodeCount(queue);
    for (int i = 0; i < cnt; i++)
    {
        Node *n = deQueue(queue);
        free(n);
    }
    (*queue)->front = (*queue)->back = NULL;
    (*queue)->size = 0;
    free(*queue);
}

Queue::Queue(int size)
{
    initQueue(&q);
    this->size = size;
    for (int i = 0; i < size; i++)
    {
        push(0);
    }
}
void Queue::push(double value)
{
    enQueue(&q, createNode(value));
}
double Queue::pop()
{
    Node *n = deQueue(&q);
    double output = n->data;
    free(n);
    return output;
}
void Queue::clear()
{
    destroyQueue(&q);
}
double Queue::UpdateQ(double value)
{
    push(value);
    double out = pop();
    sum += value - out;
    avg = sum / size;
    return out;
}