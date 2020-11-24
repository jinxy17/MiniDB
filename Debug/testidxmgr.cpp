#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../idxmgr/IndexManager.h"
#include <iostream>

using namespace std;

int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager* fm = new FileManager();
	BufPageManager* bpm = new BufPageManager(fm);
	IndexManager * im = new IndexManager(fm, bpm);
	SIndexManager * sim;

	cout << im->CreateIndex("index", INT, 4);
	cout << im->OpenIndex("index", sim);
    cout << im->CloseIndex(sim);
	return 0;
}
