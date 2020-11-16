# MiniDB
Project for Database course @ THUCS

## Authors
Xuyang JIN, Xiaoyu LI

## Checkpoint 1: Record Management

### Author
Xuyang JIN

### Files
`recmgr/RecManager.h`

`Debug/testrecmanager.cpp`

### Class doc
```cpp
#define DATA_OFFSET 1024 // offset of DATA section for each page, in bytes
#define ID_SEGM 16 // pos of seperator for rec id, in bits
#define DATA_SIZE (PAGE_SIZE-DATA_OFFSET)

class RecManager {
	RecManager(BufPageManager * bpm, int fileID, int recSize); 
	// bpm: pointer to BufPageManager
	// fileID: current file id
	// recSize: size for each rec, in bytes
	
	~RecManager();
	
	void insertRec(BufType e, unsigned int & id);
	// e: pointer to a rec
	// id: returns id
	
	void deleteRec(unsigned int id);
	// id: id of rec to be deleted 
	
	void updateRec(BufType e, unsigned int id);
	// e: pointer to a rec
	// id: id of rec to be updated
		
	class Iterator { // nested iterator class
		Iterator(RecManager * rm); 
		// rm: pointer to RecManager
		
		bool next(BufType & e, unsigned & id);
		// e: returns pointer to next rec
		// id: returns id of next rec
		// return: false if reaches end, else true
	};
};
```

## Checkpoint 2: Index Management

### Author
Xiaoyu LI

### Files
`idxmgr/*.h`  
`idxmgr/*.cc`
### Class doc
```cpp
//SIndexManager对单个索引文件进行管理,执行索引的添加删除查询等工作
class SIndexManager{
    SIndexManager(BufPageManager * bpm, int fileID);
    ~SIndexManager();

    //创建新索引时调用,参数为索引值类型和长度
    bool init(int ixSize,int ixType);

    //插入,删除索引
    void insertIx(void *key, int page, int offset);
    bool deleteIx(void *key, int page, int offset);

    //获取键值为key的所有项
    map<int, int> getAllIx(void *key);

    //获取键值为key,位置大于page-offset的第一个项
    bool getIx(void *key,int page,int offset, int& rpage, int& roffset);

    //获取一个空页,返回页号
    int getEmptyPage();
    //将某一页设置为空,返回成功与否
    bool setEmptyPage(int id);

    /* 从第id个块中读取B+Tree节点数据到node中 */
    void readNode(BPlusNode* node, int id);

    //比较两个key值的大小,若大小相同则进一步比较其存储位置,返回key1<key2
    bool compare(void* key1, void* key2, int page1, int page2,int offset1, int offset2)
    //只进行键值比较,返回key1<key2
    bool compareKey(void *key1, void *key2)
};

//索引管理模块
class IndexManager {
public:
	IndexManager(FileManager *fm, BufPageManager *bpm);
	~IndexManager();

    //创建,删除,打开,关闭索引,ixName为保存索引的文件名
	bool CreateIndex(const char *ixName, int ixType, int ixSize);
	bool DeleteIndex(const char *ixName);
	bool OpenIndex(const char *ixName, SIndexManager* sim);
    bool CloseIndex(SIndexManager* sim);
};

```

### 说明:
索引文件的文件头(第一页)分区如下:  

| ixType  | ixSize  | ixPP    | rootIdx  | offset   | bmPage     |  
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes  | 16 Bytes | 8160 Bytes |

&emsp;&emsp;其中`ixType`是索引数据类型,用宏定义的`INT,FLOAT,STRING`标识; `ixSize`保存单条索引数据的长度; `ixPP`是一个索引节点(一个物理页)中能存储的数据条数,对于一条数据,需要存储数据本身,数据在对应文件的页号和页内偏移,同时还有在B+Tree内对应的子节点页号,因此`ixPP = DATA_SIZE_IX/(3*8+ ixSize)`;`rootIdx`是B+Tree根节点的物理页号,根节点初始化时是第一页,之后可能因为节点分裂,第一页的节点会变成非根节点,根节点的本质属性是`parent == 0`;`bmPage`用于分配和回收页面  

索引数据页面分区如下:  
| isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |  
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |

### 存在的问题:
1. 未进行详细测试。
2. 多列索引尚未实现