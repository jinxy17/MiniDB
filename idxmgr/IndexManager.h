#ifndef INDEX_MANAGER
#define INDEX_MANAGER

#include "SIndexManager.h"

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
	
	bool OpenIndex(const char *ixName, SIndexManager* sim);
    bool CloseIndex(SIndexManager* sim);
};
#endif
