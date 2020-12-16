#ifndef REC_MANAGER
#define REC_MANAGER
#define DATA_OFFSET 1024
#define ID_SEGM 16
#define DATA_SIZE (PAGE_SIZE-DATA_OFFSET)

#include "../filesystem/bufmanager/BufPageManager.h"
#include "../filesystem/fileio/FileManager.h"
#include "../filesystem/utils/pagedef.h"
#include <iostream>
#include <cstring>
#include <bitset> 
using namespace std;

class RecManager {
private:
	// stored in memory:
	BufPageManager * bpm;
	int fileID; // associated to a single file
	int idxPage; // buffer index for page 0
	
	// stored in file - page 0:
	int * recSize; // sizeof(rec)
	int * recPP; // #recs per page
	int * recNum; // #recs
	int * pageNum; // #pages, including page 0
	bitset<DATA_SIZE*8> * bmPage; // bitmap for free pages, including page 0
		
public:
	RecManager(BufPageManager * bpm, int fileID, int recSize, bool reset) {
		this->bpm = bpm;
		this->fileID = fileID;
		BufType b = bpm->getPage(fileID, 0, idxPage);

		this->recSize = (int *)&b[0];
		this->recPP = (int *)&b[1];
		this->recNum = (int *)&b[2];
		this->pageNum = (int *)&b[3];
		this->bmPage = (bitset<DATA_SIZE*8> *)((unsigned char*)b + DATA_OFFSET);
		if (reset) {
			*this->recSize = recSize;
			*this->recPP = DATA_SIZE/recSize;
			*this->recNum = 0;
			*this->pageNum = 1;
			this->bmPage->reset();
			(*this->bmPage)[0] = true;
		}
		bpm->markDirty(idxPage);
	}
	~RecManager() {
		this->bpm->close();
	}
	void insertRec(BufType e, unsigned int & id) {
		*recNum += 1;
		int curPage;
		for (curPage = 0; ; curPage++) {
			if (! (*bmPage)[curPage])
				break;
		}
		int idxRec;
		BufType b;
		bitset<DATA_OFFSET*8> * bmRec;
		if (curPage == *pageNum) {
			// create new page
			*pageNum += 1;
			b = bpm->getPage(fileID, curPage, idxRec);
			bmRec = (bitset<DATA_OFFSET*8> *)b;
			bmRec->reset();
		} else {
			b = bpm->getPage(fileID, curPage, idxRec);
			bmRec = (bitset<DATA_OFFSET*8> *)b;
		}
		int curRec;
		for (curRec = 0; ; curRec++) {
			if (! (*bmRec)[curRec])
				break;
		}
		memcpy((unsigned char *)b + DATA_OFFSET + *recSize * curRec, e, *recSize);
		(*bmRec)[curRec] = true;
		if (bmRec->count() == *recPP) {
			(*bmPage)[curPage] = true;
		}
		bpm->markDirty(idxPage);
		bpm->markDirty(idxRec);
		id = (curPage << ID_SEGM) + curRec;
	}
	void deleteRec(unsigned int id) {
		*recNum -= 1;
		int curPage = id >> ID_SEGM;
		int curRec = id << ID_SEGM >> ID_SEGM;
		int idxRec;
		BufType b = bpm->getPage(fileID, curPage, idxRec);
		bitset<DATA_OFFSET*8> * bmRec = (bitset<DATA_OFFSET*8> *)b;
		(*bmRec)[curRec] = false;
		(*bmPage)[curPage] = false;
		bpm->markDirty(idxPage);
		bpm->markDirty(idxRec);
	}
	void updateRec(BufType e, unsigned int id) {
		int curPage = id >> ID_SEGM;
		int curRec = id << ID_SEGM >> ID_SEGM;
		int idxRec;
		BufType b = bpm->getPage(fileID, curPage, idxRec);
		memcpy((unsigned char *)b + DATA_OFFSET + *recSize * curRec, e, *recSize);
		bpm->access(idxPage);
		bpm->markDirty(idxRec);
	}
	bool GetRec(BufType & e,unsigned int id){
		int curPage = id >> ID_SEGM;
		int curRec = id << ID_SEGM >> ID_SEGM;
		if (curPage >= *this->pageNum) return false;
		int idxRec;
		BufType b = bpm->getPage(fileID, curPage, idxRec);
		e = (BufType)((unsigned char *)b + DATA_OFFSET + *this->recSize * curRec);
		bpm->access(this->idxPage);
		bpm->access(idxRec);
		return true;
	}
	void debug() {
		cout << "recsize:" << *recSize << "\trecPP:" << *recPP << 
			"\trecNum:" << *recNum << "\tpageNum:" << *pageNum << "\n";
		cout << "bmPage:" << (*bmPage)[0] << (*bmPage)[1] << (*bmPage)[2] << (*bmPage)[3] <<
			(*bmPage)[4] << (*bmPage)[5] << (*bmPage)[6] << (*bmPage)[7] << "\n";
	}
	class Iterator {
	private:
		RecManager * rm;
		int curPage;
		int curRec;
	public:
		Iterator(RecManager * rm) {
			this->rm = rm;
			this->curPage = 1;
			this->curRec = -1;
		}
		bool next(BufType & e, unsigned & id) {
			int idxRec;
			BufType b = rm->bpm->getPage(rm->fileID, curPage, idxRec);
			bitset<DATA_OFFSET*8> * bmRec = (bitset<DATA_OFFSET*8> *)b;
			bool found = false;
			for (curRec = curRec + 1; curRec < *rm->recPP; curRec++) {
				if ((*bmRec)[curRec]) {
					found = true;
					break;
				}
			}
			rm->bpm->access(idxRec);
			

			if (!found) {
				for (curPage = curPage+1; curPage < *rm->pageNum; curPage++) {
					b = rm->bpm->getPage(rm->fileID, curPage, idxRec);
					rm->bpm->access(idxRec);
					bmRec = (bitset<DATA_OFFSET*8> *)b;
					rm->bpm->access(idxRec);
					if (bmRec->count() > 0) {
						found = true;
						break;
					}
				}
				if (!found) {
					rm->bpm->access(rm->idxPage);
					return false;
				}
				b = rm->bpm->getPage(rm->fileID, curPage, idxRec);
				bmRec = (bitset<DATA_OFFSET*8> *)b;
				for (curRec = 0; ; curRec++) {
					if ((*bmRec)[curRec])
						break;
				}

			}
			rm->bpm->access(rm->idxPage);
			rm->bpm->access(idxRec);
			e = (BufType)((unsigned char *)b + DATA_OFFSET + *rm->recSize * curRec);
			id = (curPage << ID_SEGM) + curRec;
			return true;
		}

	};
};
#endif
