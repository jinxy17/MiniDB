/*
 * testrecmgmr.cpp
 *
 *  Created on: 2020年11月22日
 *      Author: xuyang jin
 * 	随机insertRec/deleteRec/updateRec，用std::set检验正确性。
 * 
 */
#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../recmgr/RecManager.h"
#include <iostream>
#include <utility>
#include <set>
#include <cstdlib>
#include <string>

using namespace std;

struct Rec {
	int ph1;
	int ph2;
	int b1;
	char b2[8];
	char b3[8];
};


int main(int argc, char **argv) {
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " RecFile\n";
		return 1;
	}
	MyBitMap::initConst();   //新加的初始化
	FileManager* fm = new FileManager();
	BufPageManager* bpm = new BufPageManager(fm);
	int fileID;
	fm->openFile(argv[1], fileID); //打开文件，fileID是返回的文件id
	RecManager * rm = new RecManager(bpm, fileID, 0, false);
	rm->debug();
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	BufType data;
	unsigned int id;
	while (iter->next(data, id)) {
		int pageID = id >> 16;
		int slotID = (id << 16 >> 16);
		cout << "entry:";
		for (int i = 0; i < 36; i++) {
			if (*((char *)data + i) < 32 || *((char *)data + i) > 126) 
				cout << "*";
			else
				cout << *((char *)data + i);
		}
		cout << endl;
	}
	delete iter;
	delete rm;
	fm->closeFile(fileID);
	delete bpm;
	delete fm;
	
	return 0;
}
