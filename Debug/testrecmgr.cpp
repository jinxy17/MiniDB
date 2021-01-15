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

using namespace std;

set<pair<double, pair<int, int> > > rec;

int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager* fm = new FileManager();
	BufPageManager* bpm = new BufPageManager(fm);
	fm->createFile("testrm"); //新建文件
	int fileID;
	fm->openFile("testrm", fileID); //打开文件，fileID是返回的文件id
	RecManager * rm = new RecManager(bpm, fileID, sizeof(double), true);

	puts("inserting data");
	for (int i = 1; i <= 10000; i++) {
		int page, slot;
		unsigned int id;
		double *d = new double(rand() / 1000.0);
		rm->insertRec((BufType)d, id);
		page = id >> 16;
		slot = id << 16 >> 16;
		// cout << "insert: page=" << page << " slot=" << slot << " content=" << *d << endl;
		rec.insert(make_pair(*d, make_pair(page, slot)));

		if (i > 100 && rand() % 10 < 3) {
			double t = rand() / 1000.0;
			auto it = rec.lower_bound(make_pair(t, make_pair(-1, -1)));
			if (it == rec.end()) continue;
			page = (*it).second.first; slot = (*it).second.second;
			// cout << "delete: page=" << page << " slot=" << slot << endl;
			rm->deleteRec((page << 16) + slot); 
			rec.erase(it);
		}

		if (i > 50 && rand() % 10 < 7) {
			double t = rand() / 1000.0;
			auto it = rec.lower_bound(make_pair(t, make_pair(-1, -1)));
			if (it == rec.end()) continue;
			page = (*it).second.first; slot = (*it).second.second;
			t = rand() / 1000.0;
			// cout << "update: page=" << page << " slot=" << slot << endl;
			rm->updateRec((BufType)&t, (page << 16) + slot);
			rec.erase(it);
			rec.insert(make_pair(t, make_pair(page, slot)));
		}
	}

	puts("scanning data");
	RecManager::Iterator * iter = new RecManager::Iterator(rm);
	BufType e;
	int page, slot;
	unsigned int id;
	double *t;
	while (iter->next(e, id)) {
		t = (double *)e;
		page = id >> 16;
		slot = (id << 16 >> 16);
		// cout << "iter: page=" << page << " slot=" << slot << " content=" << *t << endl;
		auto it = rec.find(make_pair(*t, make_pair(page, slot)));
		if (it == rec.end()) {
			puts("failed");
			break;
		}
		rec.erase(it);
	}
	if (rec.size() > 0) puts("failed");
	
	delete iter;
	delete rm;
	fm->closeFile(fileID);
	delete bpm;
	delete fm;
	puts("test completed");
	system("rm testrm");

	
	return 0;
}
