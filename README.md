# 数据库系统概论 项目总结报告
金旭扬 李啸宇

## 系统架构设计
本项目的目标为实现一个单用户的关系数据库管理系统，支持给定的SQL命令。系统分为四个模块：

1. 记录管理模块：依赖于给定的页式文件系统，完成以“记录”（固定长度的字节串）为单位的的读写与遍历。该模块作为 DBMS 的文件系统，管理数据库存储的记录，同时记录数据库元数据。
2. 索引模块：为存储在文件中的记录建立 B+树索引，加快查找速度；支持联合索引的创建和删除。
3. 系统管理模块：实现基本的数据定义语言(DDL)，对数据库和数据表进行管理。支持INT、FLOAT、STRING(X)等数据类型，支持表的创建、修改与删除，列的添加、修改和删除，及主/外键的创建和删除。同时考虑到了数据库的完整性约束。
4. 查询解析模块：实现基本的数据操作语言(DML)，对数据库里的数据进行增删改查等基本操作(包括多表连接)。其中查询等操作适当利用索引来进行加速。

## 小组分工
金旭扬：负责记录管理模块、系统管理模块

李啸宇：负责索引模块、查询解析模块

## 各模块详细设计、主要接口说明
### 模块1: 记录管理
#### 接口：
```cpp
#define DATA_OFFSET 1024 // offset of DATA section for each page, in bytes
#define ID_SEGM 16 // pos of seperator for rec id, in bits
#define DATA_SIZE (PAGE_SIZE-DATA_OFFSET)

class RecManager {
	RecManager(BufPageManager * bpm, int fileID, int recSize, bool reset);
	// bpm: pointer to BufPageManager
	// fileID: current file id
	// recSize: size for each rec, in bytes
	// reset: generate a clean rec / load old rec

	void insertRec(BufType e, unsigned int & id);
	// e: pointer to a rec
	// id: returns id
	
	void deleteRec(unsigned int id);
	// id: id of rec to be deleted 
	
	void updateRec(BufType e, unsigned int id);
	// e: pointer to a rec
	// id: id of rec to be updated
	
	bool GetRec(BufType & e,unsigned int id);
	// e: reference a rec pointer
	// id: id of rec to be fetched
		
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
#### 说明：
此模块基于给定的页式文件系统，添加了对单条记录的插入、删除、遍历等功能。为了提升插入记录时的效率，在0号页面添加了位图以标出空闲页面，在每个页面的开头1024字节添加了位图以标出空闲槽位。遍历采用迭代器模式进行设计。

### 模块2: 索引模块
#### 接口：

```cpp
//SIndexManager对单个索引文件进行管理,执行索引的添加删除遍历等工作
class SIndexManager{
	//读取/写入索引文件头信息
    void readInfo();
    void writeInfo();

    //创建新索引时调用,参数为索引值类型和长度
    bool init(int ixSize,int ixType);

    //插入,删除索引
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

    //索引scan,openscan的参数key为开始遍历的数据值,通过left + scanself来设置遍历开始的位置以及是否遍历该数据本身的条目
    bool OpenScan(void *key, bool left,bool scanself);
	//获取上一个/下一个条目
	bool GetPrevEntry(int &page, int &offset);
	bool GetNextEntry(int &page, int &offset);
	bool CloseScan();
};

//索引管理模块,负责为上层模块提供索引文件的管理接口
class IndexManager {
public:
	IndexManager(FileManager *fm, BufPageManager *bpm);
	~IndexManager();

    //创建,删除,打开,关闭索引,ixName为保存索引的文件名
	bool CreateIndex(const char *ixName, int ixType, int ixSize);
	bool DeleteIndex(const char *ixName);
	
	bool OpenIndex(const char *ixName, int& fileID);
    bool CloseIndex(int fileID);
};

```

#### 说明:
&emsp;&emsp;索引模块的底层实现了一个B+Tree,支持插入条目,删除条目等操作,为上层提供了创建索引,删除索引,插入条目,删除条目,遍历索引等多个接口(如上所示)。下面介绍索引模块的数据结构和文件分区设计:  

&emsp;&emsp;索引信息页的OFFSET是16字节,数据页的OFFSET是20字节,为了方便扩展,整个索引系统的页内OFFSET统一定为32字节;  

一个索引文件的数据类型,数据长度等信息保存在其对应的SIndexManager中,对应到索引文件的文件头(第一页),详细的页面分区(也即SIndexManager的成员变量)如下:  

```
| ixType  | ixSize  | ixPP    | rootIdx  | offset   | bmPage     |  
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes  | 16 Bytes | 8160 Bytes |
```

&emsp;&emsp;其中`ixType`是索引数据类型,用宏定义的`INT,FLOAT,STRING`标识(`DATE`类型在底层采用`INT`存储,`NULL`是通过在每个记录前加bit标记实现的,因此索引不支持NULL); `ixSize`保存单条索引数据的长度; `ixPP`是一个索引节点(一个物理页)中能存储的数据条数,对于一条数据,需要存储数据本身,数据在对应文件的页号和页内偏移,同时还有在B+Tree内对应的子节点页号,因此`ixPP = DATA_SIZE_IX/(3*8+ ixSize)`;`rootIdx`是B+Tree根节点的物理页号,根节点初始化时是第一页,之后可能因为节点分裂,第一页的节点会变成非根节点,根节点的本质属性是`parent == 0`;`bmPage`用于分配和回收页面.  

每个B+Tree节点(BPlusNode)则对应于索引文件的一个具体的数据页,页面分区如下:  

```
| isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |  
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |  
```

&emsp;&emsp;其中`isLeaf`记录节点是否为叶子节点,`keyNum`记录节点中键值数,`parent,prev,next`分别为父节点和邻居节点,经过`offset`之后是储存四类数据的数据区.

### 模块3: 系统管理
#### 接口：
```cpp
class SysManager {
	void OpenDB(const string DBName);
	void CloseDB();
	void Show();
	void Show(const string tableName);
	void CreateTable(TableInfo* table);
	void DropTable(const string tableName);
	void RenameTable(const string tableName, const string newTableName);
	void CreateIndex(const string idxName, const string tableName, const vector<string> attrs);
	void AddIndex(const string idxName, const vector<string> attrs);
	void DropIndex(const string idxName);
	void AddPrimaryKey(const string tableName, const vector<string> attrs, const string pkName = "");
	void DropPrimaryKey(const string tableName, const string pkName = "");
	void AddForeignKey(const string tableName, const string fkName, const vector<string> attrs, const string refName, const vector<string> foreigns);
	void DropForeignKey(const string tableName, const string fkName);
	void AddColumn(const string tableName, AttrInfo attr);
	void DropColumn(const string tableName, string attrName);
	void RenameColumn(const string tableName, string oldAttrName, string newAttrName);
	void ChangeColumn(const string tableName, string oldAttrName, AttrInfo newAttr);
};
```
#### 说明：
系统管理模块调用记录管理模块和索引模块，在元数据上操作，实现基本的基本的数据定义语言。在数据库运行时,将列属性、表属性等元数据存储在内存中，在数据库打开/关闭时会备份到meta.db文件。为了表示记录管理模块中缺少的NULL字段，每个记录实际存储时在头部增加了8字节，用来表示NULL位图。此模块的编写除了要符合SQL语义外，还需要符合数据库的完整性约束，比如AddPrimaryKey()时需要遍历数据保证待添加的主键无重复项，或者AddForeignKey()时保证待添加外键的已有项均包含在了被引用的主键中。

### 模块4: 查询解析
1. DataOperater是数据处理模块,负责数据的插入,删除,更新以及查询,同时额外提供了加载tbl数据的Load函数;
	
	```cpp
	class DataOperater {
	public:
		bool Insert(const string tableName, vector<Value*> insertvalues, vector<int> columns,int nullcolumnnub);
		void Update(const Assigns assigns, vector<Relation*> relations);
		void Delete(const string tableName, vector<Relation*> relations);
		//单表查询
		void Select(const string tableName, vector<Relation*> relations, vector<string> attrNames);
		//多表连接查询
		void Select(vector<string> tableNames, vector<Tcol*> cols, vector<Relation *> relations);
		//加载数据,数据目录固定为/data
		void Load(const string tableName,const char* line);
	};
	```

2. lex.l,yacc.y以及Stmt共同负责SQL命令的解析,lex.l/yacc.y是flex&yacc文件,Stmt是编译时的语法单元模块,基类是Stmt,派生出对应五种Stmt的子类:

	```cpp
	class Stmt {
	public:
	    enum Type {
		    SYS_STMT,DB_STMT,TB_STMT,IDX_STMT,ALTER_STMT
	    };
		Type sttype;
	    Stmt(Type _sttype){
	        sttype = _sttype;
	    };
	    ~Stmt(){};
	};
	```

3. 在语法分析中生成Stmt之后,通过一个全局的Executer来执行SQL命令,yacc会根据stmt的类型调用Executer中的对应函数:

	```cpp
	class Executer {
	public:
	    void execSysStmt(sysStmt* stmt);
		void execDbStmt(dbStmt* stmt);
		void execTbStmt(tbStmt* stmt);
		void execIdxStmt(idxStmt* stmt);
		void execAlterStmt(alterStmt* stmt);
    	void init();
    	void exit();
	};
	```
然后在这些函数里面调用底层提供好的接口,完成数据库的各项操作.

4. 有关文法的说明:在给定的parser文法规则基础上,我们还添加了`QUIT`语句来退出程序,还有`LOAD tbName FROM fileName`语句,用于加载tbl数据.


## 参考文献
1. [db-server](https://github.com/ejacky/db-server)
1. [cs346-redbase](https://github.com/adityabhandari1992/cs346-redbase)
1. [DBNoC](https://github.com/RecursionSheep/DBNoC)
1. [redbase](https://github.com/yifeih/redbase) 

