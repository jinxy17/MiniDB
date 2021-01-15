#ifndef SINDEX_MANAGER
#define SINDEX_MANAGER

#include "../filesystem/bufmanager/BufPageManager.h"
#include "./BPlusTree.h"
#include <cstring>
#include <bitset>
#include <map>

//索引信息页的OFFSET是16字节,数据页的OFFSET是20字节,暂时留出32字节
#define DATA_OFFSET_IX 32
#define DATA_SIZE_IX (PAGE_SIZE-DATA_OFFSET_IX)

// 索引信息页面分区:
// | ixType  | ixSize  | ixPP    | rootIdx  | offset   | bmPage     |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes  | 16 Bytes | 8160 Bytes |

//对单个索引文件进行管理
class SIndexManager{

public:
	BufPageManager * bpm;
    int infoPage;//索引信息页在缓存中的页号
    /*
     *  ixType:  索引数据类型,用宏定义的INT,FLOAT,STRING标识
     *  ixSize:  单条索引数据的长度
     *  ixPP:    一个索引节点(一个物理页)中能存储的数据条数,对于一条数据,需要存储数据本身,数据在对应文件的页号和页内偏移,同时还有在B+Tree内对应的子节点页号,因此ixPP = DATA_SIZE_IX/(3*8+ ixSize)
     *  rootIdx: B+Tree根节点的物理页号,根节点初始化时是第一页,之后可能因为节点分裂,第一页的节点会变成非根节点,根节点的本质属性是parent == 0
     *  bmPage:  bitmap for empty pages, including page 0,超级块,用于分配和回收页面
    */
    int ixType;
    int ixSize; 
    int ixPP;
    int rootIdx;
    bitset<DATA_SIZE_IX * 8> * bmPage;
public:
    int fileID;// associated to a single file
    
    SIndexManager(BufPageManager * bpm, int fileID);
    ~SIndexManager();

    void readInfo();
    void writeInfo();

    //创建新索引时调用
    bool init(int ixSize,int ixType);

    //插入,删除,查询索引
    bool insertIx(void *key, int page, int offset);
    bool deleteIx(void *key, int page, int offset);

    //获取一个空页,返回页号
    bool getEmptyPage(int& idx);
    //将某一页设置为空,返回成功与否
    bool setEmptyPage(int id);

    /* 从第id个块中读取B+Tree节点数据 */
    void readNode(BPlusNode* node, int id);

    //比较两个key值的大小,若大小相同则进一步比较其存储位置,返回key1<key2
    bool compare(void* key1, void* key2, int page1, int page2,int offset1, int offset2);
    //只进行键值比较,返回key1=key2
    bool compareKeyEq(void *key1, void *key2);
    //只进行键值比较,返回key1<key2
    bool compareKeyLt(void *key1, void *key2);
    //只进行键值比较,返回key1<=key2
    bool compareKeyLe(void *key1, void *key2);
    bool Exists(void *key);

    //索引Scan
    bool OpenScan(void *pData, bool lower,bool scanself);
	bool GetPrevEntry(int &page, int &offset);
	bool GetNextEntry(int &page, int &offset);
	bool CloseScan();
	
    void Print_Tree();
    int Print_Node(int nodeID,int dep);
	int scan_nodeID, scan_entryID;
};


#endif