//
// Created by creeper on 22-11-18.
//
///Given that this project is tiny, there is no need to implement these data structures here...
///But it needs to be treated seriously as a complete project.
#ifndef RENJU_DATASTRUCTURE_H
#define RENJU_DATASTRUCTURE_H
#define DOUBLY_LINKED_LIST(TYPE) TYPE *prv, *nxt
#define LINK(NODE_A, NODE_B) NODE_A->nxt = NODE_B; \
                                NODE_B->prv = NODE_A
typedef int DS_ERROR_CODE;
///using RBT, we can implement DIY std::set and std::map
///Well, seems like I have no time for this...
struct RBTNode
{
    int idx, key;
    int col;
    struct RBTNode* ch[2];
};
DS_ERROR_CODE insert(struct RBTNode*, int);
DS_ERROR_CODE kth(struct RBTNode*, int);
DS_ERROR_CODE rank(struct RBTNode*, int);
#endif //RENJU_DATASTRUCTURE_H
