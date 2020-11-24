#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include "../idxmgr/IndexManager.h"
#include <iostream>
#include <cstdlib>
#include <set>

using namespace std;

set<pair<double, pair<int, int> > > s;

int main() {
	MyBitMap::initConst();   //新加的初始化
	FileManager* fm = new FileManager();
	BufPageManager* bpm = new BufPageManager(fm);
	IndexManager * im = new IndexManager(fm, bpm);
	SIndexManager * sim;

	im->CreateIndex("name", FLOAT, 8);
	im->OpenIndex("name", sim);
	for (int i = 1; i <= 1000000; i++) {
		double x = (rand() % 2000) / 1000.0;
		//fprintf(stderr, "Insert %.3lf %d\n", x, i);
		sim->insertIx(&x, i, i);
		s.insert(make_pair(x, make_pair(i, i)));

		if (i > 100 && (rand() % 100 < 15)) {
			int k = rand() % s.size();
			auto it = s.begin();
			for (int j = 0; j < k; j++) it++;
			//fprintf(stderr, "Delete %.3lf %d\n", (*it).first, (*it).second.first);
			x = (*it).first;
			sim->deleteIx(&x, (*it).second.first, (*it).second.second);
			s.erase(it);
		}
		x = (rand() % 2000) / 1000.0;
		//fprintf(stderr, "Query %.3lf\n", x);
		if (i >= 999995) {
			auto it = s.lower_bound(make_pair(x, make_pair(-1, -1)));

			// this is an iterator: 
			// will start from key x and output in ascending (descending) sequence everything >= (<=) x
			IX_IndexScan *scan = new IX_IndexScan(fileManager, bufPageManager, fileID);
			scan->OpenScan(&x, true);
			//fprintf(stderr, "set %.3lf\n", (*it).first);
			while (it != s.end()) {
				int page = -1, slot = -1;
				scan->GetNextEntry(page, slot);
				//fprintf(stderr, "%d %.3lf\n", (*it).second.first, (*it).first);
				if ((*it).second.first != page) {
					cout << (*it).second.first << " " << page << endl;
					return 0;
				}
				it++;
			}
			scan->CloseScan();
			delete scan;
			//fprintf(stderr, "\n");
		}
	}
	bufPageManager->close();
	ixm->CloseIndex(fileID);
	fprintf(stderr, "close\n");


    im->CloseIndex(sim);
	return 0;
}
