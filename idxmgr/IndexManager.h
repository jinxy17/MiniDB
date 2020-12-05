#ifndef INDEX_MANAGER
#define INDEX_MANAGER

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "./SIndexManager.h"
#include <stdlib.h>

//索引管理模块
class IndexManager {
private:
	FileManager *fm;
	BufPageManager *bpm;

public:
	IndexManager(FileManager *fm, BufPageManager *bpm);
	~IndexManager();

	bool CreateIndex(const char *ixName, int ixType, int ixSize);
	bool DeleteIndex(const char *ixName);
	
	bool OpenIndex(const char *ixName, int& fileID);
    bool CloseIndex(int fileID);
};

#endif
