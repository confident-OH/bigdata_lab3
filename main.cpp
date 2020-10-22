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
                mtree_head.build();
                mtree_head.show_tree();
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
