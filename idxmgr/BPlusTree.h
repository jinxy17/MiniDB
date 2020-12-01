#ifndef BPlusTree_h
#define BPlusTree_h

#include "../filesystem/utils/pagedef.h"

#define INT 0
#define FLOAT 1
#define STRING 2
// 索引数据页面分区:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |

//B+Tree节点数据结构
struct BPlusNode{
    int pageID;// 此块对应的缓存页号

    // 以下是保存到物理页的数据的访问指针
    int* isLeaf; //是否为叶子节点
	int* keyNum; //键值数
	int* parent; //父节点
	int* prev;
    int* next;//邻居节点

    //获取指向四大数据区起始处的指针
	unsigned char *key;
    int* childs;
    int* pages;
    int* offsets;
public:
    void print_st() {
        printf("Node: \n  pageID:%d\n  isLeaf:%d\n  keyNum:%d\n  parent:%d\n  prev:%d\n  next:%d\n",pageID,*isLeaf,*keyNum,*parent,*prev,*next);
    }

    void print_key(int id,int ixType,int ixSize) {
        //先行比较数值
        if (ixType == INT)
        {
            int* key1 = (int *)(this->key + id * (ixSize));
            printf("%d",*((int *)key1));
        }
        else if (ixType == FLOAT)
        {
            double* key1 = (double *)(this->key + id * (ixSize));
            printf("%.4lf",*((double *)key1));
        }
    }
};


#endif