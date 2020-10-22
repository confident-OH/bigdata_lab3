//
// Created by leo on 2020/10/22.
//
#include "picosha2.h"
#ifndef MERKLE_M_TREE_H
#define MERKLE_M_TREE_H

struct Node {
    int key;                    //节点的键，用来加速检索，可选数据块或其名称的代表性特征
    int hashval;		          //本节点的哈希值，叶子节点为数据块的哈希，中间节点和根节点为左右子节点拼接后值的哈希。
    struct Node * parent;
    struct Node * left;
    struct Node * right;
    Node(){

    }
};

class M_tree {
private:
    int treesize;
    Node **pNode;
    Node **oldpNode;
    Node **ofolder;
public:
    M_tree();
    virtual ~M_tree();
    bool compare(Node **nownode, Node **oldernode);
    void build();
    void show_tree();
};


#endif //MERKLE_M_TREE_H
