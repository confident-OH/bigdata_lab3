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
    delete_n(pNode);
    delete_n(oldpNode);
    delete_n(ofolder);
}

bool M_tree::compare() {
    string cp_head = string("cp ");
    string rm_head = string("rm ");
    if(hash_cmp(this->pNode->hashn, this->ofolder->hashn)){
        return true;
    }else{
        if(this->pNode->key != this->ofolder->key){
            int n = this->pNode->key.size();
            int i;
            vector<string>::iterator it;
            for(i=0;i<n;i++){
                string item = this->pNode->key[i];
                it = find(this->ofolder->key.begin(), this->ofolder->key.end(), item);
                if(it == this->ofolder->key.end()){
                    //the file is not in fold2
                    cout << "File: " << item << " are lost. We are copying" << endl;
                    cp_head += folder_name1 + "/" + item + " " + folder_name2;
                    system(cp_head.c_str());
                    cp_head = string("cp ");
                }
            }
            n = this->ofolder->key.size();
            for(i = 0;i<n;i++){
                string item = this->ofolder->key[i];
                it = find(this->pNode->key.begin(), this->pNode->key.end(), item);
                if(it == this->pNode->key.end()){
                    //the file need to be deleted
                    cout << "File: " << item << " are redundant. We are deleting" << endl;
                    rm_head += folder_name2 + "/" + item;
                    system(rm_head.c_str());
                    rm_head = string("rm ");
                }
            }
            this->build(const_cast<char *>(folder_name2.c_str()), 3);
        }
        if(!hash_cmp(this->pNode->hashn, this->ofolder->hashn)){
            vector<string> differ(M_tree::compare_tree(this->pNode, this->ofolder));
            cout << "Wrong files:\n";
            while(!differ.empty()){
                cout << differ.back() << " is wrong. We are fixing it..." << endl;
                rm_head += this->folder_name2 + "/" + differ.back();
                system(rm_head.c_str());
                rm_head = string("rm ");
                cp_head += this->folder_name1 + "/" + differ.back() + " " + folder_name2;
                system(cp_head.c_str());
                cp_head = string("cp ");
                differ.pop_back();
            }
        }
    }
}

void M_tree::build(char *folder, int n) {  //1: pNode; 2: olderpNode; 3: ofolder;
    vector<string> file_names;
    vector<Node*> point_leaves;
    char byte_file[1024*1024];
    std::vector<hash_node> hash_leaves;            //hashs of every file
    std::vector<Node*> hash_b1, hash_b2;
    hash_node hash_item;
    std::string out;
    int i, size;

    //open the folder
    file_names = open_folder(folder);

    // sort the names and using the names as keys
    std::sort(file_names.begin(), file_names.end());
    size = file_names.size();

    //turn all the files to hash
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

    for(i=0;i<size;i++){
        Node *item = new (Node);
        memcpy(item->hashn, hash_leaves[i].hash, 32);   //ssss set the hash
        item->key.push_back(file_names[i]);
        hash_b1.push_back(item);
        point_leaves.push_back(item);                   //save the pionts of leaves
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
        //deal the tail file
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
        // b2->b1 and clear b2
        hash_b1.assign(hash_b2.begin(), hash_b2.end());
        hash_b2.clear();
    }
    switch (n) {
        case 1:
        {
            this->pNode = hash_b1.back();
            this->file_names_p.assign(file_names.begin(), file_names.end());
            this->point_p.assign(point_leaves.begin(), point_leaves.end());
            break;
        }
        case 2:
        {
            this->oldpNode = hash_b1.back();
            this->file_names_ol.assign(file_names.begin(), file_names.end());
            this->point_ol.assign(point_leaves.begin(), point_leaves.end());
            break;
        }
        case 3:
        {
            this->ofolder = hash_b1.back();
            this->file_names_of.assign(file_names.begin(), file_names.end());
            this->point_of.assign(point_leaves.begin(), point_leaves.end());
            break;
        }
    }

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

void M_tree::delete_n(Node *node) {
    if(node == NULL){
        return;
    }
    if(node->right != NULL){
        delete_n(node->right);
    }

    if(node->left != NULL){
        delete_n(node->left);
    }
    delete(node);
}

void M_tree::show(int n) {
    int i, j, level=0;
    Node *show_head;
    ofstream outfile;
    outfile.open("output.txt", ios::out);
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
            printf("level: %d\n", level);
            outfile << "level: " << level << endl;
            show_head = hash1[i];
            int size_c = show_head->key.size();
            printf("key: ");
            outfile << "key: ";
            for(j=0;j<size_c;j++){
                cout << show_head->key[j] << " | ";
                outfile << show_head->key[j] << " | ";
            }
            printf("\n");
            outfile << endl;
            printf("hash: ");
            outfile << "hash: ";
            for(j=0;j<8;j++){
                printf("%2x", show_head->hashn[j]);
                outfile << hex << show_head->hashn[j];
            }
            printf("\n");
            outfile << endl;
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
        outfile << endl;
    }
    outfile.close();
}

vector<string> M_tree::compare_tree(Node *n1, Node*n2) {
    if(n1 == NULL&&n2 == NULL){
        return vector<string> ();
    }
    if(n1 == NULL){
        return n2->key;
    }
    if(n2 == NULL){
        return n1->key;
    }
    if(hash_cmp(n1->hashn, n2->hashn)){
        return vector<string> ();
    }
    if(n1->key.size() == 1){
        return n1->key;
    }
    vector<string> a1 = M_tree::compare_tree(n1->left, n2->left);
    vector<string> a2 = M_tree::compare_tree(n1->right, n2->right);
    while(!a2.empty()){
        a1.push_back(a2.back());
        a2.pop_back();
    }
    return a1;
}

vector<string> M_tree::open_folder(char *folder) {
    vector<string> file_names;
    DIR* d = opendir(folder);
    if (d == NULL)
    {   //Failed open the folder
        printf("d == ULL");
        closedir(d);
        return file_names;
    }
    struct dirent* entry;
    while ( (entry=readdir(d)) != NULL)
    {
        if(strcmp(entry->d_name, ".")&&strcmp(entry->d_name, "..")){
            file_names.push_back(std::string(entry->d_name));
        }
    }
    closedir(d);
    return file_names;
}

