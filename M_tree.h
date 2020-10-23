//
// Created by leo on 2020/10/22.
//
#include "sha256.h"
#ifndef MERKLE_M_TREE_H
#define MERKLE_M_TREE_H
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

struct hash_node {
    uint32_t hash[8];
    hash_node(){
        memset(hash, 0, sizeof(hash));
    }
};

union hash_add_key{
    uint8_t ahash[64];
    uint32_t ahash2[16];
};

struct Node {
    vector<int> key;                    //节点的键，用来加速检索，可选数据块或其名称的代表性特征
    uint32_t hashn[8];		          //本节点的哈希值，叶子节点为数据块的哈希，中间节点和根节点为左右子节点拼接后值的哈希。
    struct Node * parent;
    struct Node * left;
    struct Node * right;
    Node(){
        memset(hashn, 0, 8*sizeof(uint32_t));
        parent = NULL;
        left = NULL;
        right = NULL;
    }
};

class M_tree {
private:
    int treesize;
    Node *pNode;
    Node *oldpNode;
    Node *ofolder;
public:
    M_tree();
    virtual ~M_tree();
    void update();
    bool compare();
    void build(char *folder, int n);
    void show_tree(int n);
    void delete_n (Node **node);
    void show(int n);
    hash_add_key add_hash (uint32_t* hash1, uint32_t* hash2);
    bool hash_cmp(uint32_t* hash1, uint32_t*hash2);

};


#endif //MERKLE_M_TREE_H
