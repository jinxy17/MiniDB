#pragma once

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../recmgr/RecManager.h"
#include "../idxmgr/IndexManager.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include <map>

struct AttrInfo {
	string attrName;
	int attrType;
	int attrLength; // bytes, completed by SM for INTEGER & FLOAT
	bool notNull;
	bool primary;
	BufType defaultValue;
	string reference; // table to be referenced
	string foreignKeyName; // attr to be referenced, must be primary
	bool haveIndex;
	int offset; // 4 bytes, completed by SM

	//仅为了适应语法,支持创建表时设置PRIMARY KEY,这是一个简便但本不合理的实现
	bool createPk = false;
	vector<string> namelist;
	//仅为了适应语法,支持创建表时设置FOREIGN KEY,这是一个简便但本不合理的实现
	bool createFk = false;
	//列名表名沿用上面的成员变量,但其内涵完全不同;
	//实际实现冲发现createPk/createFk为True则直接调用Alter语句
};

struct TableInfo {
	string tableName;
	string pkName; // primary key name, defaulted to be ""
	map<string, string> fkNames; // mapping fkName to refName
	int attrNum, foreignNum; // foreignNum completed by SM
	int recordSize, primarySize; // bytes, completed by SM
	vector<int> primary; // completed by SM
	vector<AttrInfo> attrs;
	vector<string> foreign; // completed by SM
	set<string> foreignSet; // completed by SM

};

class SysManager {
public:
	FileManager *fileManager;
	BufPageManager *bufPageManager;
	
	SysManager(IndexManager *ixm, FileManager *_fileManager, BufPageManager *_bufPageManager);
	~SysManager();
	
	// each record is started with 8 bytes containing the NULL bitmap. 1 represents not null and 0 represents null.
	void OpenDB(const string DBName);
	void CloseDB();
	void Show();
	void Show(const string tableName);
	void CreateTable(TableInfo* table);
	void DropTable(const string tableName);
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
	void RenameTable(const string tableName, const string newTableName);
	
	void _CreateIndex(const string tableName, const string attr);
	void _DropIndex(const string tableName, const string attr);
	bool _checkForeignKeyOnTable(int tableID);
	int _fromNameToID(const string tableName);
	int _fromNameToID(const string attrName, const int tableID);
	BufType _getPrimaryKey(int tableID, BufType data);
	string _DBName;
	int _tableNum;
	std::vector<TableInfo> _tables;
	std::map<std::string, int> _tableFileID;
	std::map<std::string, std::pair<std::string, std::vector<string> > > _indexes; // {idxName: (tableName, attrs), ...}
private:
	IndexManager *_ixm;
};
