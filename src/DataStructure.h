//
// Created by creeper on 22-11-18.
//
///Given that this project is tiny, there is no need to implement these data structures here...
///But it needs to be treated seriously as a complete project.
#ifndef RENJU_DATASTRUCTURE_H
#define RENJU_DATASTRUCTURE_H
typedef int DS_ERROR_CODE;
///using RBT, we can implement DIY std::set and std::map
struct RBTNode
{
    int idx, key;
    int col;
    struct RBTNode* ch[2];
};
DS_ERROR_CODE insert(struct RBTNode*, int);
DS_ERROR_CODE remove(struct RBTNode*, int);
DS_ERROR_CODE kth(struct RBTNode*, int);
DS_ERROR_CODE rank(struct RBTNode*, int);
#endif //RENJU_DATASTRUCTURE_H
