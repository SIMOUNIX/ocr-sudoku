#ifndef STACK_H
#define STACK_H


struct tupple
{
	float e1;
	float e2;
};
struct stack
{
    int maxsize;    // define max capacity of the stack
    int top;
    struct tupple *items;
};
struct stack* newStack(int capacity);
int size(struct stack *pt);
int isEmpty(struct stack *pt);
int isFull(struct stack *pt);
void push(struct stack *pt, struct tupple x);
struct tupple peek(struct stack *pt);
struct tupple pop(struct stack *pt);
#endif
