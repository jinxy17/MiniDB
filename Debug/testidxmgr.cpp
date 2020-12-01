#include <iostream>
#include <cstdlib>
#include <set>

#include "../idxmgr/IndexManager.h"

set< pair<double, pair<int, int> > > s;
int testsize = 100000;

void test_insert()
{
	printf("test insert:\n");
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->CreateIndex("name", FLOAT, 8);
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	printf(" - index opened\n");
	// sim->Print_Tree();
	//随机插入testsize条数据
	for (int i = 1; i <= testsize; i++)
	{
		double x = (rand() % 1000) / 1000.0;
		sim->insertIx(&x, i, i);
		// printf("%f, %d\n", x, i);
		s.insert(make_pair(x, make_pair(i, i)));
	}
	printf(" - inserted %d entries\n", testsize);
	// sim->Print_Tree();
	bpm->close();
	im->CloseIndex(sim);
	printf(" - index closed\n");
}

void test_delete()
{
	printf("test delete:\n");
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	printf(" - index opened\n");
	// sim->Print_Tree();
	set<pair<double, pair<int, int> > >::iterator iter = s.begin();
	int i = 0;
	while (iter != s.end())
	{
		pair<double, pair<int, int> > _pair = *iter;
		double x = _pair.first;
		pair<int, int> pos = _pair.second;
		int page = pos.first;
		int offset = pos.second;
		if (rand() % 10 < 2)
		{
			// printf(" - delete index:%.8lf,%d,%d\n",x,page,offset);
			sim->deleteIx(&x, page, offset);
			iter = s.erase(iter);
		} else {
			iter++;
		}
	}
	printf(" - delete completed:\n");
	// sim->Print_Tree();
	bpm->close();
	im->CloseIndex(sim);
	printf(" - index closed\n");
}

void test_filesave()
{
	printf("test filesave:\n");
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	printf(" - index opened\n");
	// sim->Print_Tree();
	bpm->close();
	im->CloseIndex(sim);
	printf(" - index closed\n");
}

void test_scan()
{
	printf("test scan:\n");
	FileManager *fm = new FileManager();
	BufPageManager *bpm = new BufPageManager(fm);
	IndexManager *im = new IndexManager(fm, bpm);
	int fileID;
	im->OpenIndex("name", fileID);
	SIndexManager *sim = new SIndexManager(bpm, fileID);
	printf(" - index opened\n");
	// sim->Print_Tree();
	int cnt = 0;
	for (int i = 0; i < 10; i++)
	{
		double x = (rand() % 1000) / 1000.0;
		x = 0.9;
		// printf(" - start from: %.3lf\n", x);
		set<pair<double, pair<int, int> > >::iterator it = s.lower_bound(make_pair(x, make_pair(-1, -1)));
		// fprintf(stderr, "set %.8lf\n", (*it).first);
		bool valid = sim->OpenScan(&x, true);
		while (valid) {
			int page = -1, slot = -1;
			valid = sim->GetNextEntry(page, slot);
			// printf("%d\n", page);
			if ((*it).second.first != page || (*it).second.second != slot)
			{
				printf("Error,expect:%d,but receive:%d;scanid:%d,%d\n",(*it).second.first ,page,sim->scan_nodeID, sim->scan_entryID);
				// BPlusNode *node = new BPlusNode();
				// sim->readNode(node,sim->scan_nodeID);
				// for(int i = 0;i < *node->keyNum;i++){
            	// 	printf("pos:%d, data: ",i);
            	// 	node->print_key(i,sim->ixType,sim->ixSize);
            	// 	printf(",page:%d,slot:%d;\n",node->pages[i],node->offsets[i]);
        		// }
				return;
			}
			it++;
		};
		printf(" - test #%d succeed\n",++cnt);
		sim->CloseScan();
	}
	bpm->close();
	im->CloseIndex(sim);
	printf(" - index closed\n");
}

int main()
{
	MyBitMap::initConst(); //新加的初始化
	test_insert();
	test_scan();
	test_delete();
	test_scan();
	test_filesave();
	return 0;
}
