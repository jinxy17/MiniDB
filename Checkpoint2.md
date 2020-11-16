# MiniDB
Project for Database course @ THUCS

## Authors
Xuyang JIN, Xiaoyu LI

## Checkpoint 2: Index Management

### Author
Xiaoyu LI

### Files
`idxmgr/*.h`  
`idxmgr/*.cc`

### Class doc
```cpp

// 索引数据页面分块:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |

//B+Tree节点数据结构
struct BPlusNode{
    int pageID;// 此块对应的缓存页号

    // 以下是保存到物理页的数据的访问指针
    int* isLeaf; //是否为叶子节点
	int* keyNum; //键值数
	int* parent; //父节点
	int* prev, next;//邻居节点

    //指向四大数据区起始处的指针
	unsigned char *key;
    BufType child, page, offset;
};

//索引信息页的OFFSET是16字节,数据页的OFFSET是20字节,暂时留出32字节
#define DATA_OFFSET_IX 32
#define DATA_SIZE_IX (PAGE_SIZE-DATA_OFFSET_IX)
#define INT 0
#define FLOAT 1
#define STRING 2

// 索引信息页面分块:
// | ixType  | ixSize  | ixPP    | rootIdx  | offset   | bmPage     |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes  | 16 Bytes | 8160 Bytes |

//对单个索引文件进行管理
class SIndexManager{
	BufPageManager * bpm;
    int fileID;// associated to a single file
    int infoPage;//索引信息页在缓存中的页号
    /*
     *  ixType:  索引数据类型,用宏定义的INT,FLOAT,STRING标识
     *  ixSize:  单条索引数据的长度
     *  ixPP:    一个索引节点(一个物理页)中能存储的数据条数,对于一条数据,需要存储数据本身,数据在对应文件的页号和页内偏移,同时还有在B+Tree内对应的子节点页号,因此ixPP = DATA_SIZE_IX/(3*8+ ixSize)
     *  rootIdx: B+Tree根节点的物理页号,根节点初始化时是第一页,之后可能因为节点分裂,第一页的节点会变成非根节点,根节点的本质属性是parent == 0
     *  bmPage:  bitmap for empty pages, including page 0,超级块,用于分配和回收页面
    */
    int * ixType;
    int * ixSize; 
    int * ixPP;
    int * rootIdx;
    bitset<DATA_SIZE_IX>>3> * bmPage;
public:
    SIndexManager(BufPageManager * bpm, int fileID);
    ~SIndexManager();

    //创建新索引时调用
    bool init(int ixSize,int ixType);

    //插入,删除,查询索引
    void insertIx(void *key, int page, int offset);
    void deleteIx(void *key, int page, int offset);

    //获取输出为key的所有项
    map<int, int> getAllIx(void *key);
    //获取键值为key,位置大于page-offset的第一个项
    bool getIx(void *key,int page,int offset, int& rpage, int& roffset);

    //获取一个空页,返回页号
    int getEmptyPage();
    //将某一页设置为空,返回成功与否
    bool setEmptyPage(int id);

    /* 从第id个块中读取B+Tree节点数据 */
    BPlusNode* readNode(BPlusNode* node, int id);

    //比较两个key值的大小,若大小相同则进一步比较其存储位置,返回key1<key2
    bool compare(void* key1, void* key2, int page1, int page2,int offset1, int offset2)
    //只进行键值比较,返回key1<key2
    bool compareKey(void *key1, void *key2)
};

//索引管理模块
class IndexManager {
private:
	FileManager *fm;
	BufPageManager *bpm;

public:
	IndexManager(FileManager *fm, BufPageManager *bpm);
	~IndexManager();

    //创建,删除,打开,关闭索引
	bool CreateIndex(const char *ixName, int ixType, int ixSize);
	bool DeleteIndex(const char *ixName);
	bool OpenIndex(const char *ixName, SIndexManager* sim);
    bool CloseIndex(SIndexManager* sim);
};
```
