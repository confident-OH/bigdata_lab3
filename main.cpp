#include <iostream>
#include "M_tree.h"
using namespace std;


int main(int argc, char *argv[]) {
    if(argc == 1 || argc > 3) {
        puts("error parameters");
        return 0;
    }

    //含程序自身参数，总参数数为2时，第2个参数为用于构建默克尔树的目录
    //含程序自身参数，总参数数为3时，第2个参数为用于构建默克尔树的目录，第3个参数为用于构建默克尔树和比较一致性的目录，
    //相关逻辑自行补充

    M_tree mtree_head;
    char input_lable;
    char check_fold[100];

    while(1) //整体实验大循环，完成实验3-1至3-4
    {
        //初始化创建叶子节点，注意根据目录下文件数分配空间
        //initalize_leaf函数，输出Node类型的数组，每个叶子节点的key可使用文件名，哈希值为文件所有数据的哈希，叶子节点初始化时，父节点、左右子节点为空，数据结构参考前述数据结构。未初始化时，key值可以设成-1.
        //hash函数可以是MD5，也可以是SHA-256，或自行设计哈希函数
        cout << "1. Building the M_tree." << endl;
        cout << "2. Updating the M_tree." << endl;
        cout << "3. Checking the difference between two folder." << endl;
        cout << "Please input 1~3 to using these function!" << endl;
        cin >> input_lable;
        switch(input_lable){
            case '1':
            {
                cout << "**************** We are building the Merkle tree. Please wait... ****************" << endl;
                mtree_head.build(argv[1], 1);
                mtree_head.show(1);
                break;
            }
            case '2':
            {
                cout << "**************** We are updating the Merkle tree. Please wait... ****************" << endl;

                break;
            }
            case '3':
            {
                if(argc == 2){
                    cout << "Please input the address, we will check their difference." << endl;

                    cin >> check_fold;
                }

                cout << "**************** We are checking the difference. Please wait... ****************" << endl;
                break;
            }
            case 'q':
            {
                cout << "Thanks for your using. Good bye~" << endl;
                break;
            }
            default:{
                cout << "You inputed a wrong alphabet, Please input again." << endl;
            }
        }
        cout << endl;
        if(input_lable == 'q'){
            break;
        }
    }//结束大循环
}

/*
                    char a1[100] = "jjjjjjj";
                    char a2[100] = "kkkkkkk";
                    char a3[100] = "lllllll";
                    int i;
                    uint32_t b1[8], b2[8], b3[8];

                    sha256((uint8_t*)a1, strlen(a1), b1);
                    sha256((uint8_t*)a2, strlen(a2), b2);
                    sha256((uint8_t*)a3, strlen(a3), b3);

                    hash_add_key test1 = mtree_head.add_hash(b1, b2);
                    hash_add_key test2 = mtree_head.add_hash(b2, b3);
                    uint32_t c1[8], c2[8], c3[8], c4[8];
                    sha256(test1.ahash, 64, c1);
                    sha256((uint8_t*)a3, 32, c2);
                    sha256(test2.ahash, 64, c3);
                    sha256((uint8_t*)b1, 32, c4);
                    test1 = mtree_head.add_hash(c1, c2);
                    test2 = mtree_head.add_hash(c3, c4);
                    sha256(test1.ahash, 64, c1);
                    sha256(test2.ahash, 64, c3);
                    for(i=0;i<8;i++){
                        printf("%x", c1[i]);
                    }
                    printf("\n");
                    for(i=0;i<8;i++){
                        printf("%x", c2[i]);
                    }
                    printf("\n");
 */
