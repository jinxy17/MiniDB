# Database System Introduction Project Summary Report
Jin Xuyang Li Xiaoyu

## System Architecture Design
The goal of this project is to realize a single-user relational database management system that supports given SQL commands. The system is divided into four modules:

1. Record management module: Rely on the given page file system to complete the reading, writing and traversal in the unit of "record" (fixed-length byte string). This module serves as the file system of the DBMS, manages the records stored in the database, and records database metadata at the same time.
2. Index module: Create a B+ tree index for the records stored in the file to speed up the search; support the creation and deletion of a joint index.
3. System management module: implement basic data definition language (DDL) to manage databases and data tables. Supports data types such as INT, FLOAT, STRING(X), table creation, modification, and deletion, column addition, modification, and deletion, and primary/foreign key creation and deletion. At the same time, the integrity constraints of the database are taken into account.
4. Query parsing module: implement basic data manipulation language (DML), and perform basic operations such as adding, deleting, modifying, and querying data in the database (including multi-table connection). Among them, queries and other operations appropriately use indexes to accelerate.

## Group division
Jin Xuyang: Responsible for the record management module and system management module

Li Xiaoyu: Responsible for index module, query analysis module

## Detailed design of each module, main interface description
### Module 1: Record Management
#### Interface:
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

class Iterator {// nested iterator class
Iterator(RecManager * rm);
// rm: pointer to RecManager

bool next(BufType & e, unsigned & id);
// e: returns pointer to next rec
// id: returns id of next rec
// return: false if reaches end, else true
};
};
```
#### illustrate:
Based on a given page file system, this module adds functions such as inserting, deleting, and traversing a single record. In order to improve the efficiency of inserting records, a bitmap is added to page 0 to mark free pages, and a bitmap is added to the first 1024 bytes of each page to mark free slots. The traversal is designed using the iterator pattern.

### Module 2: Index Module
#### Interface:

```cpp
//SIndexManager manages a single index file, performs index addition and deletion traversal and other tasks
class SIndexManager{
//Read/write index file header information
    void readInfo();
    void writeInfo();

    //Called when creating a new index, the parameters are the index value type and length
    bool init(int ixSize,int ixType);

    //Insert, delete index
    bool insertIx(void *key, int page, int offset);
    bool deleteIx(void *key, int page, int offset);

    //Get an empty page, return the page number
    bool getEmptyPage(int& idx);
    //Set a page to be empty, return success or not
    bool setEmptyPage(int id);

    /* Read B+Tree node data from the id-th block */
    void readNode(BPlusNode* node, int id);

    //Compare the size of the two key values, if the size is the same, further compare their storage locations, return key1<key2
    bool compare(void* key1, void* key2, int page1, int page2,int offset1, int offset2);
    //Only perform key-value comparison, return key1=key2
    bool compareKeyEq(void *key1, void *key2);
    //Only perform key-value comparison, return key1<key2
    bool compareKeyLt(void *key1, void *key2);
    //Only perform key-value comparison, return key1<=key2
    bool compareKeyLe(void *key1, void *key2);
    bool Exists(void *key);

    //Index scan, the parameter key of openscan is the data value to start traversal, and the position where the traversal starts and whether to traverse the entry of the data itself is set by left + scanself
    bool OpenScan(void *key, bool left,bool scanself);
//Get the previous/next entry
bool GetPrevEntry(int &page, int &offset);
bool GetNextEntry(int &page, int &offset);
bool CloseScan();
};

//Index management module, responsible for providing index file management interface for upper module
class IndexManager {
public:
IndexManager(FileManager *fm, BufPageManager *bpm);
~IndexManager();

    //Create, delete, open, close the index, ixName is the file name to save the index
bool CreateIndex(const char *ixName, int ixType, int ixSize);
bool DeleteIndex(const char *ixName);

bool OpenIndex(const char *ixName, int& fileID);
    bool CloseIndex(int fileID);
};

```

#### illustrate:
&emsp;&emsp; The bottom layer of the index module implements a B+Tree, which supports operations such as inserting entries and deleting entries, and provides the upper layer with multiple interfaces such as creating indexes, deleting indexes, inserting entries, deleting entries, traversing indexes, etc. (as shown above ). The following describes the data structure and file partition design of the index module:

&emsp;&emsp; The OFFSET of the index information page is 16 bytes, and the OFFSET of the data page is 20 bytes. In order to facilitate expansion, the OFFSET of the entire index system page is uniformly set to 32 bytes;

The data type, data length and other information of an index file are stored in its corresponding SIndexManager, which corresponds to the file header (first page) of the index file. The detailed page partitions (that is, the member variables of SIndexManager) are as follows:

```
| ixType | ixSize | ixPP | rootIdx | offset | bmPage |
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 16 Bytes | 8160 Bytes |
```

&emsp;&emsp; where `ixType` is the index data type, which is identified by macro-defined `INT, FLOAT, STRING` (`DATE` type is stored by `INT` at the bottom, and `NULL` is used by adding bit before each record The mark is implemented, so the index does not support NULL); `ixSize` saves the length of a single index data; `ixPP` is the number of data items that can be stored in an index node (a physical page). For a piece of data, the data itself needs to be stored, The data is offset in the page number and page of the corresponding file, and there is also the corresponding child node page number in B+Tree, so `ixPP = DATA_SIZE_IX/(3*8+ ixSize)`; `rootIdx` is B+Tree The physical page number of the root node. The root node is the first page when it is initialized. Later, due to the split of the node, the node of the first page will become a non-root node. The essential attribute of the root node is `parent == 0`; `bmPage` Used to allocate and reclaim pages.

Each B+Tree node (BPlusNode) corresponds to a specific data page of the index file, and the page partitions are as follows:

```
| isLeaf | keyNum | parent | prev | next | offset | childs | pages | offsets | keys |
| 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes |
```

&emsp;&emsp; Among them, `isLeaf` records whether the node is a leaf node, `keyNum` records the number of key values in the node, `parent, prev, next` are the parent node and the neighbor node respectively, after the `offset`, four types of data are stored Data area.

### Module 3: System Management
#### Interface:
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
#### illustrate:
The system management module calls the record management module and the index module, operates on the metadata, and realizes the basic basic data definition language. When the database is running, metadata such as column attributes and table attributes are stored in memory, and backed up to the meta.db file when the database is opened/closed. In order to indicate the missing NULL field in the record management module, 8 bytes are added to the header of each record when it is actually stored, which is used to represent the NULL bitmap. The writing of this module must not only conform to SQL semantics, but also conform to the integrity constraints of the database. For example, when AddPrimaryKey(), it is necessary to traverse the data to ensure that the primary key to be added has no duplicates, or when AddForeignKey(), to ensure that the foreign key to be added exists. The items are included in the referenced primary key.

### Module 4: Query Analysis
1. DataOperater is a data processing module, responsible for data insertion, deletion, update and query, and additionally provides a Load function to load tbl data;

```cpp
class DataOperater {
public:
bool Insert(const string tableName, vector<Value*> insertvalues, vector<int> columns,int nullcolumnnub);
void Update(const Assigns assigns, vector<Relation*> relations);
void Delete(const string tableName, vector<Relation*> relations);
//Single table query
void Select(const string tableName, vector<Relation*> relations, vector<string> attrNames);
//Multi-table join query
void Select(vector<string> tableNames, vector<Tcol*> cols, vector<Relation *> relations);
//Load data, the data directory is fixed to /data
void Load(const string tableName,const char* line);
};
```

2. lex.l, yacc.y and Stmt are jointly responsible for the analysis of SQL commands. lex.l/yacc.y is the flex&yacc file, Stmt is the grammatical unit module at compile time, and the base class is Stmt, which derives the corresponding five kinds of Stmt Subclass:

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

3. After the Stmt is generated in the syntax analysis, the SQL command is executed through a global Executer, and yacc will call the corresponding function in the Executer according to the type of stmt:

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
Then call the good interface provided by the bottom layer in these functions to complete various operations of the database.

4. Explanation of grammar: Based on the given parser grammar rules, we also added a `QUIT` statement to exit the program, and a `LOAD tbName FROM fileName` statement to load tbl data.


## references
1. [db-server](https://github.com/ejacky/db-server)
1. [cs346-redbase](https://github.com/adityabhandari1992/cs346-redbase)
1. [DBNoC](https://github.com/RecursionSheep/DBNoC)
1. [redbase](https://github.com/yifeih/redbase)
