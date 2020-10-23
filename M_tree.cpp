//
// Created by leo on 2020/10/22.
//

#include "M_tree.h"

M_tree::M_tree() {
    this->treesize = 0;
    this->pNode = NULL;
    this->oldpNode = NULL;
    this->ofolder = NULL;
}
M_tree::~M_tree() {

}

bool M_tree::compare() {

    return false;
}

void M_tree::show_tree(int n) {  //1: pNode; 2: olderpNode; 3: ofolder;

}

void M_tree::build(char *folder, int n) {  //1: pNode; 2: olderpNode; 3: ofolder;
    char byte_file[1024*1024];
    std::vector<std::string> file_names;      //File names
    std::vector<hash_node> hash_leaves;            //hashs of every file
    std::vector<Node*> hash_b1, hash_b2;
    hash_node hash_item;
    std::string out;
    int i, size;
    DIR* d = opendir(folder);
    if (d == NULL)
    {   //Failed open the folder
        printf("d == ULL");
    }
    struct dirent* entry;
    while ( (entry=readdir(d)) != NULL)
    {
        if(strcmp(entry->d_name, ".")&&strcmp(entry->d_name, "..")){
            file_names.push_back(std::string(entry->d_name));
        }
    }
    std::sort(file_names.begin(), file_names.end());
    size = file_names.size();
    for(i = 0;i<size;i++){
        out = file_names[i];
        ifstream file((string(folder)+string("/")+out).c_str(), ios::in | ios::binary);
        if(!file){
            cout << "Failed open the file" << string(folder)+string("/")+out << endl;
            continue;
        }
        memset(byte_file, 0, sizeof(byte_file));
        file.read(reinterpret_cast<char *>(&byte_file), sizeof(byte_file));
        cout << out << endl;                //output the file names
        sha256((uint8_t*)byte_file,file.gcount(), hash_item.hash);
        hash_leaves.push_back(hash_item);
        file.close();
    }
    cout << endl;
    closedir(d);
    for(i=0;i<size;i++){
        Node *item = new (Node);
        memcpy(item->hashn, hash_leaves[i].hash, 32);   //ssss set the hash
        item->key.push_back(i);
        hash_b1.push_back(item);
    }
    hash_b2.clear();
    while(hash_b1.size()>1){
        int size1 = hash_b1.size();
        for(i=0; i<size1-1;i = i+2){
            uint32_t newhash[8];
            hash_add_key hasha;
            Node *item = new(Node);
            item->left = hash_b1[i];
            item->right = hash_b1[i+1];
            int j;
            for (j=0;j<hash_b1[i]->key.size();j++){
                item->key.push_back(hash_b1[i]->key[j]);
            }
            for (j=0;j<hash_b1[i+1]->key.size();j++){
                item->key.push_back(hash_b1[i+1]->key[j]);
            }
            hash_b1[i]->parent = item;
            hash_b1[i+1]->parent = item;
            hasha = add_hash(hash_b1[i]->hashn, hash_b1[i+1]->hashn);
            sha256(hasha.ahash, 64, newhash);
            memcpy(item->hashn, newhash, 32);
            hash_b2.push_back(item);
        }
        if(size1%2==1){
            Node *item = new(Node);
            item->left = hash_b1[i];
            int j;
            for (j=0;j<hash_b1[i]->key.size();j++){
                item->key.push_back(hash_b1[i]->key[j]);
            }
            hash_b1[i]->parent = item;
            memcpy(item->hashn, hash_b1[i]->hashn, 32);
            hash_b2.push_back(item);
        }
        hash_b1.assign(hash_b2.begin(), hash_b2.end());
        hash_b2.clear();
    }
    this->pNode = hash_b1.back();
    this->pNode->parent == NULL;
}

void M_tree::update() {

}

//append hash2 after hash1
hash_add_key M_tree::add_hash(uint32_t *hash1, uint32_t *hash2) {
    hash_add_key result;
    memcpy(result.ahash, hash1, 32);
    memcpy(result.ahash+32, hash2, 32);
    return result;
}

bool M_tree::hash_cmp(uint32_t *hash1, uint32_t *hash2) {
    int i;
    for(i=0;i<8;i++){
        if(hash1[i] != hash2[i]){
            return false;
        }
    }
    return true;
}

void M_tree::delete_n(Node **node) {

}

void M_tree::show(int n) {
    int i, j, level=0;
    Node *show_head;
    FILE *fp = fopen("output.txt", "w");
    vector<Node*> hash1;
    vector<Node*> hash2;
    switch(n){
        case 1:
        {
            show_head = this->pNode;
            break;
        }
        case 2:
        {
            show_head = this->oldpNode;
            break;
        }
        case 3:
        {
            show_head = this->ofolder;
            break;
        }
    }
    hash1.push_back(show_head);
    hash2.clear();
    while(!hash1.empty()){
        int size = hash1.size();
        for(i=0;i<size;i++){
            printf("level: %d", level);
            fprintf(fp, "level: %d", level);
            printf("\n");
            fprintf(fp, "\n");
            show_head = hash1[i];
            int size_c = show_head->key.size();
            printf("key: ");
            fprintf(fp, "key: ");
            for(j=0;j<size_c;j++){
                printf("%d ", show_head->key[j]);
                fprintf(fp, "%d ", show_head->key[j]);
            }
            printf("\n");
            fprintf(fp, "\n");
            printf("hash: ");
            fprintf(fp, "hash: ");
            for(j=0;j<8;j++){
                printf("%2x", show_head->hashn[j]);
                fprintf(fp, "%2x", show_head->hashn[j]);
            }
            printf("\n");
            fprintf(fp, "\n");
            if(show_head->left){
                hash2.push_back(show_head->left);
            }
            if(show_head->right){
                hash2.push_back(show_head->right);
            }
        }
        hash1.assign(hash2.begin(), hash2.end());
        hash2.clear();
        level++;
        printf("\n");
        fprintf(fp, "\n");
    }
    fclose(fp);
}

