//
// Created by creeper on 22-11-18.
//
#ifndef RENJU_DATASTRUCTURE_H
#define RENJU_DATASTRUCTURE_H
#include "utils.h"
#define SIZE 15
#define DOUBLY_LINKED_LIST(TYPE) TYPE *prv, *nxt
#define LINK(NODE_A, NODE_B) NODE_A->nxt = NODE_B; \
                                NODE_B->prv = NODE_A
#define MAX_HEAPSIZE (SIZE * SIZE)
/**
 * The interfaces of binary heap are all macros.
 * Most of the time we will be operating on only one binary heap.
 * So if I use functions to implement a dynamic version, the address of the heap will be passed as an argument
 * quite frequently. But there is no need to pass a same variable and referencing a same pointer over and over again.
 * So I'd rather use a static version when using heap to avoid these.
 * So for every heap used in this project, a copy of corresponding functions will be generated.
 * And that will make the size of the codes bigger.
 * But that's fine, because it is used in rare occasions in this project(in fact only one heap is used)
 */
#define STATIC_BINARY_HEAP(hp,size) static struct HeapNode
/**
 * positions with greatest values can be accessed at a fast speed if I use a binary heap.
 * A heap will be stored as an array of HeapNodes.
 * Index of the root node will be 1, and the left child of node o will be o<<1.
 * the right child of node o will be o<<1|1.
 */
struct HeapNode
{
    int x, y, val;
};
/**
 * use Binary Heap to maintain the maximum or the minimum quickly
 * in this project, heap will be created frequently!
 * So I WON'T ALLOCATE MEMORY DYNAMICALLY BECAUSE IT IS SLOW!
 * using an array with fixed size is ok
 */
struct BinaryHeap
{
    struct HeapNode hp[MAX_HEAPSIZE];
    int sz;
};
static inline void clear(struct BinaryHeap *H) { H->sz = 0; }
static inline bool empty(struct BinaryHeap *H) { return !(H->sz); }
static inline struct HeapNode top(struct BinaryHeap *H) { return H->hp[1]; }
static void up(struct BinaryHeap *H, int o)
{

    while(H->hp[o].val >= H->hp[o >> 1].val && o)
    {
        swap(struct HeapNode, H->hp[o], H->hp[o >> 1]);
        o >>= 1;
    }
}

static void down(struct BinaryHeap *H, int o)
{
    while(((o << 1|1) <= H->sz && H->hp[o].val < H->hp[o << 1|1].val)
          || ((o << 1) <= H->sz && H->hp[o].val < H->hp[o << 1].val))
    {
        if((o << 1|1) <= H->sz)
        {
            if(H->hp[o << 1].val < H->hp[o << 1|1].val)
            {
                swap(struct HeapNode, H->hp[o], H->hp[o << 1]);
                o <<= 1;
            }
            else
            {
                swap(struct HeapNode, H->hp[o], H->hp[o << 1]);
                o = o << 1|1;
            }
        }
        else if((o << 1) <= H->sz)
        {
            swap(struct HeapNode, H->hp[o], H->hp[o << 1]);
            o <<= 1;
        }
    }
}

static inline void insert(struct BinaryHeap *H, int x, int y, int val)
{
    /*if(H->sz == MAX_HEAPSIZE)
    {
        printf("Error: Binary heap is full, cannot insert new node.\n");
        return ;
    }*/
    H->hp[++(H->sz)] = (struct HeapNode){x, y, val};
    up(H, H->sz);
}

static inline void pop(struct BinaryHeap *H)
{
    /*if(!(H->sz))
    {
        printf("Error: Binary heap is empty, cannot pop.\n");
        return ;
    }*/
    swap(struct HeapNode, H->hp[1], H->hp[H->sz]);
    H->sz--;
    down(H, 1);
}
#endif //RENJU_DATASTRUCTURE_H
