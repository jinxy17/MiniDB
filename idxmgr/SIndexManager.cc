#include "SIndexManager.h"

SIndexManager::SIndexManager(BufPageManager *bpm, int fileID)
{
    this->bpm = bpm;
    this->fileID = fileID;
    this->readInfo();
}

SIndexManager::~SIndexManager()
{
    this->bpm->close();
}

void SIndexManager::readInfo(){
    BufType b = this->bpm->getPage(this->fileID, 0, this->infoPage); //将第0个页面(idx页)读入到缓存中，在缓存中页号为this->infoPage

    this->ixType = b[0];
    this->ixSize = b[1];
    this->ixPP = b[2];
    this->rootIdx = b[3];
    this->bmPage = new bitset<DATA_SIZE_IX * 8>;
    memcpy(this->bmPage,(unsigned char *)b + DATA_OFFSET_IX,sizeof(bitset<DATA_SIZE_IX * 8>));

    //DEBUG
    printf("ReadInfo--Index: \n  ixType:%d\n  ixSize:%d\n  ixPP:%d\n  rootIdx:%d\n",ixType,ixSize,ixPP,rootIdx);

    bpm->access(this->infoPage);
}

void SIndexManager::writeInfo(){
    BufType b = this->bpm->getPage(this->fileID, 0, this->infoPage); //将第0个页面(idx页)读入到缓存中，在缓存中页号为this->infoPage

    b[0] = this->ixType;
    b[1] = this->ixSize;
    b[2] = this->ixPP;
    b[3] = this->rootIdx;
    memcpy((unsigned char *)b + DATA_OFFSET_IX, this->bmPage, sizeof(bitset<DATA_SIZE_IX * 8>));

    bpm->markDirty(this->infoPage);
    bpm->writeBack(this->infoPage);
}

//按ixSize和ixType重置索引数据,在新建索引时使用
// 索引数据页面分区:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |
bool SIndexManager::init(int ixSize, int ixType)
{
    this->ixType = ixType;
    this->ixSize = ixSize;
    this->ixPP = DATA_SIZE_IX / (sizeof(int) * 3 + ixSize);
    this->rootIdx = 1;
    this->bmPage->reset();
    (*this->bmPage)[0] = true;
    (*this->bmPage)[1] = true;

    //DEBUG
    printf("Index: \n  ixType:%d\n  ixSize:%d\n  ixPP:%d\n  rootIdx:%d\n",ixType,ixSize,ixPP,rootIdx);

    writeInfo();

    // 第一页 = 根节点
    int bufroot;
    BufType rootpage = bpm->getPage(this->fileID, 1, bufroot);
    BPlusNode *root = new BPlusNode();
    root->isLeaf = (int *)&rootpage[0];
    root->keyNum = (int *)&rootpage[1];
    root->parent = (int *)&rootpage[2];
    root->prev = (int *)&rootpage[3];
    root->next = (int *)&rootpage[4];
    // 刚开始根节点也是叶子节点,key数为0,父节点为空,没有邻居节点
    *root->isLeaf = true;
    *root->keyNum = 0;
    *root->parent = 0;
    *root->prev = 0;
    *root->next = 0;
    bpm->markDirty(bufroot);
    bpm->writeBack(bufroot);
    return true;
}

// 从第id个页中读取B+Tree节点信息
// 索引数据页面分区:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |
void SIndexManager::readNode(BPlusNode *node, int id)
{
    // 缓存中调入第id页
    BufType pageStart = bpm->getPage(this->fileID, id, node->pageID);
    //读取此节点数据块信息
    node->isLeaf = (int *)&pageStart[0];
    node->keyNum = (int *)&pageStart[1];
    node->parent = (int *)&pageStart[2];
    node->prev = (int *)&pageStart[3];
    node->next = (int *)&pageStart[4];
    //获取指向四大数据区起始处的指针
    node->childs = (int *)(pageStart + DATA_OFFSET_IX / sizeof(int));
    node->pages = (int *)(pageStart + DATA_OFFSET_IX / sizeof(int) + this->ixPP);
    node->offsets = (int *)(pageStart + DATA_OFFSET_IX / sizeof(int) + (this->ixPP * 2));
    node->key = (unsigned char *)(pageStart + DATA_OFFSET_IX / sizeof(int) + (this->ixPP) * 3);
    //留下缓存访问记录
    bpm->access(node->pageID);
    return;
}

bool SIndexManager::insertIx(void *key, int page, int offset)
{
    //获取根节点的页号
    int id = this->rootIdx;
    BPlusNode *node = new BPlusNode();
    this->readNode(node, id);

    while ((*node->isLeaf) == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        //printf("(*node->keyNum): %d\n",(*node->keyNum));
        while (i > 0 && !compare(node->key + i * (this->ixSize), key, node->pages[i], page, node->offsets[i], offset)) 
            i--;
        id = node->childs[i];
        //printf("i: %d,id: %d\n",i,id);
        this->readNode(node, id);
    }
    
    // printf("find leaf:%d\n",id);
    // node->print_st();
    //找到了叶子节点,并将在节点中的某个位置插入
    int i = (*node->keyNum) - 1;
    while (1)
    {
        if (i < 0 || compare(node->key + i * (this->ixSize), key, node->pages[i], page, node->offsets[i], offset))
        {
            // printf("pos:%d,page: %d \n",i+1,page);
            // 插入条目
            memcpy(node->key + (i + 1) * (this->ixSize), key, (this->ixSize));
            node->pages[i + 1] = page;
            node->offsets[i + 1] = offset;
            node->childs[i + 1] = 0;
            break;
        }
        // printf(" %d ",node->pages[i]);
        //右侧条目右移
        memcpy(node->key + (i + 1) * (this->ixSize), node->key + i * (this->ixSize), (this->ixSize));
        node->pages[i + 1] = node->pages[i];
        node->offsets[i + 1] = node->offsets[i];
        i--;
    }
    // printf("\n");
    (*node->keyNum)++;
    bpm->markDirty(node->pageID);

    // for(int i = 0;i < *node->keyNum;i++){
    //     if(page == 13483) {
    //         printf("pos:%d, data: ",i);
    //         node->print_key(i,this->ixType,this->ixSize);
    //         printf(",page:%d,slot:%d;\n",node->pages[i],node->offsets[i]);
    //     }
    // }

    //节点满了,需要分裂节点
    while ((*node->keyNum) == (this->ixPP))
    {
        //先获取父节点
        BPlusNode *parentNode = new BPlusNode();
        if ((*node->parent) == 0)
        {
            //没有父节点,即根节点特判
            //产生一个新节点作为父节点(也是新的根节点)
            if(this->getEmptyPage(*node->parent) == false){
                return false;
            }
            this->rootIdx = (*node->parent);
            this->writeInfo();

            this->readNode(parentNode, (*node->parent));
            //设置新根节点的各项属性，新根节点初始有一个键值和一个子节点(即为原根节点),父节点为空,没有邻居节点
            *parentNode->isLeaf = false;
            *parentNode->keyNum = 1;
            *parentNode->parent = 0;
            *parentNode->prev = 0;
            *parentNode->next = 0;
            parentNode->childs[0] = id;
            parentNode->pages[0] = node->pages[0];
            parentNode->offsets[0] = node->offsets[0];
            memcpy(parentNode->key, node->key, (this->ixSize));
        }
        else
            this->readNode(parentNode, *node->parent);

        //产生新节点
        int newID;
        if(this->getEmptyPage(newID) == false) return false;
        //printf("split:id = %d,newID, %d\n",id,newID);
        //设置新节点属性
        BPlusNode *newNode = new BPlusNode();
        this->readNode(newNode, newID);
        *newNode->isLeaf = *node->isLeaf;
        *newNode->keyNum = (*node->keyNum) / 2; //键值数目减半
        *newNode->parent = *node->parent;
        *newNode->next = *node->next;
        *newNode->prev = id;
        *node->next = newID;

        //迁移数据
        *node->keyNum = (*node->keyNum) - (*newNode->keyNum);
        for (int i = 0; i < (*newNode->keyNum); i++)
        {
            newNode->childs[i] = node->childs[i + (*node->keyNum)];
            newNode->pages[i] = node->pages[i + (*node->keyNum)];
            newNode->offsets[i] = node->offsets[i + (*node->keyNum)];
        }
        memcpy(newNode->key, node->key + (*node->keyNum) * (this->ixSize), (*newNode->keyNum) * (this->ixSize));

        // 更新子节点中的父节点序号
        BPlusNode *tmpNode = new BPlusNode();
        for (int i = 0; i < (*newNode->keyNum); i++)
        {
            if (!(*newNode->isLeaf) && newNode->childs[i] != 0)
            {
                readNode(tmpNode, newNode->childs[i]);
                (*tmpNode->parent) = newID;
                bpm->markDirty(tmpNode->pageID);
                bpm->writeBack(tmpNode->pageID);
            }
        }
        // 更新邻居节点中的邻节点序号
        if ((*newNode->next) != 0)
        {
            readNode(tmpNode, (*newNode->next));
            (*tmpNode->prev) = newID;
            bpm->markDirty(tmpNode->pageID);
            bpm->writeBack(tmpNode->pageID);
        }
        delete tmpNode;

        //更新父节点中的子节点信息,右边的节点给新节点腾出位置
        for (int i = (*parentNode->keyNum); i >= 1; i--)
        {
            if (parentNode->childs[i - 1] == id)
            {
                //父节点的子节点数+1
                (*parentNode->keyNum)++;
                //设置父节点中的子节点信息,新旧节点都要设置
                memcpy(parentNode->key + (i - 1) * (this->ixSize), node->key, (this->ixSize));
                parentNode->pages[i - 1] = node->pages[0];
                parentNode->offsets[i - 1] = node->offsets[0];
                parentNode->childs[i - 1] = id;

                memcpy(parentNode->key + i * (this->ixSize), newNode->key, (this->ixSize));
                parentNode->pages[i] = newNode->pages[0];
                parentNode->offsets[i] = newNode->offsets[0];
                parentNode->childs[i] = newID;

                break;
            }
            memcpy(parentNode->key + i * (this->ixSize), parentNode->key + (i - 1) * (this->ixSize), (this->ixSize));
            parentNode->pages[i] = parentNode->pages[i - 1];
            parentNode->offsets[i] = parentNode->offsets[i - 1];
            parentNode->childs[i] = parentNode->childs[i - 1];
        }

        // for (int i = (*parentNode->keyNum - 1); i >= 0; i--)    
        // {
        //     printf(" %d ",parentNode->childs[i]);
        // }
        // printf(" \n");

        bpm->markDirty(node->pageID);
        bpm->writeBack(node->pageID);
        delete node;

        bpm->markDirty(newNode->pageID);
        bpm->writeBack(newNode->pageID);
        delete newNode;

        id = *node->parent;
        //printf("parent:%d,%d\n",id,this->rootIdx);
        node = parentNode;
        bpm->markDirty(node->pageID);
    }

    //向上更新父节点信息
    //printf("bid:%d\n",id);
    while (1)
    {
        if (id == (this->rootIdx))
        {
            delete node;
            return true;
        }
        BPlusNode *parentNode = new BPlusNode();
        this->readNode(parentNode, (*node->parent));
        for (int i = (*parentNode->keyNum) - 1; i >= 0; i--)
        {
            if (parentNode->childs[i] == id)
            {
                memcpy(parentNode->key + i * (this->ixSize), node->key, (this->ixSize));
                parentNode->pages[i] = node->pages[0];
                parentNode->offsets[i] = node->offsets[0];
                bpm->markDirty(*node->parent);
                break;
            }
        }
        delete node;
        id = *node->parent;
        node = parentNode;
    }
}

bool SIndexManager::deleteIx(void *key, int page, int offset)
{
    //获取根节点的页号
    int id = this->rootIdx;
    BPlusNode *node = new BPlusNode();
    this->readNode(node, id);
    while (*node->isLeaf == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        while (i > 0 && compare(key, node->key + i * (this->ixSize), page, node->pages[i], offset, node->offsets[i]))
            i--;
        id = node->childs[i];
        this->readNode(node, id);
    }
    //找到了叶子节点,进一步获取该key值在节点中的位置
    int pos = (*node->keyNum) - 1;
    //printf("page:%d,pos:%d\n",page,pos);
    while (1)
    {
        //if(node->pages[pos] == page)    printf("_page:%d,%d\n",page,node->pages[pos]);
        if (pos < 0)
        {
            // printf("leaf:%d,page:%d,%d;slot:%d,%d;data:%.4f,",id,page,node->pages[0],offset,node->offsets[0],*((double *)key));
            // node->print_key(0,this->ixType,this->ixSize);
            printf("Error!The key to delete is not existing\n");
            delete node;
            return false;
        }
        else if (!compare(key, node->key + pos * (this->ixSize), page, node->pages[pos], offset, node->offsets[pos]) && !compare(node->key + pos * (this->ixSize), key, node->pages[pos], page, node->offsets[pos], offset))
            break;
        pos--;
    }
    (*node->keyNum)--;

    //执行删除
    for (int i = pos; i < (*node->keyNum); i++)
    {
        node->pages[i] = node->pages[i + 1];
        node->offsets[i] = node->offsets[i + 1];
    }
    memcpy(node->key + pos * (this->ixSize), node->key + (pos + 1) * (this->ixSize), ((*node->keyNum) - pos) * (this->ixSize));
    bpm->markDirty(node->pageID);

    //printf("%d,delete succeed,rest node nub:%d\n",page,*node->keyNum);
    //向上更新父节点信息,并对空节点执行删除
    while (id != this->rootIdx)
    {
        bool empty = ((*node->keyNum) == 0);
        BPlusNode *tmpNode = new BPlusNode();
        if (empty && (*node->next) != 0)
        {
            this->readNode(tmpNode, (*node->next));
            *tmpNode->prev = *node->prev;
            bpm->markDirty(*node->next);
        }
        if (empty && (*node->prev) != 0)
        {
            this->readNode(tmpNode, (*node->prev));
            *tmpNode->next = *node->next;
            bpm->markDirty(*node->prev);
        }

        this->readNode(tmpNode, (*node->parent));
        for (int i = (*tmpNode->keyNum) - 1; i >= 0; i--)
        {
            if (tmpNode->childs[i] == id)
            {
                //节点为空,删除之
                if (empty)
                {
                    for (int j = i; j < *tmpNode->keyNum - 1; j++)
                    {
                        tmpNode->childs[j] = tmpNode->childs[j + 1];
                        tmpNode->pages[j] = tmpNode->pages[j + 1];
                        tmpNode->offsets[j] = tmpNode->offsets[j + 1];
                    }
                    *tmpNode->keyNum = *tmpNode->keyNum - 1;
                    memcpy(tmpNode->key + i * (this->ixSize), tmpNode->key + (i + 1) * (this->ixSize), (*tmpNode->keyNum - i) * (this->ixSize));
                    this->setEmptyPage(id);
                }
                //节点非空,更新父节点
                else
                {
                    memcpy(tmpNode->key + i * (this->ixSize), node->key, (this->ixSize));
                    tmpNode->pages[i] = node->pages[0];
                    tmpNode->offsets[i] = node->offsets[0];
                }
                bpm->markDirty(*node->parent);
                break;
            }
        }
        id = *node->parent;
        delete node;
        node = tmpNode;
    }
    delete node;
    return true;
}

//获取一个空页,返回页号
bool SIndexManager::getEmptyPage(int& idx)
{
    int curPage;
    for (curPage = 0;; curPage++)
    {
        if (curPage >= DATA_SIZE_IX << 3)
        {
            printf("Error,The index file is full\n");
            return false;
        }
        else if (!(*this->bmPage)[curPage])
            break;
    }
    (*this->bmPage)[curPage] = true;
    this->writeInfo();

    idx = curPage;
    return true;
}

//将某一页设置为空,返回成功与否
bool SIndexManager::setEmptyPage(int idx)
{
    if (idx > 1 && idx < DATA_SIZE_IX << 3)
    {
        (*this->bmPage)[idx] = false;
        this->writeInfo();
        return true;
    }
    return false;
}

bool SIndexManager::compare(void *key1, void *key2, int page1, int page2, int offset1, int offset2)
{
    //先行比较数值
    if ((this->ixType) == INT)
    {
        if (*((int *)key1) < *((int *)key2))
            return true;
        else if (*((int *)key1) > *((int *)key2))
            return false;
    }
    else if ((this->ixType) == FLOAT)
    {
        if (*((double *)key1) < *((double *)key2))
            return true;
        else if (*((double *)key1) > *((double *)key2))
            return false;
    }
    else if ((this->ixType) == STRING)
    {
        for (int i = 0; i < (this->ixSize); i++)
        {
            if (((char *)key1)[i] < ((char *)key2)[i])
                return true;
            else if (((char *)key1)[i] > ((char *)key2)[i])
                return false;
        }
    }

    //再比较存储的位置
    // return ((page1 << 13) + offset1) < ((page2 << 13) + offset2);
    if (page1 < page2)
        return true;
    else if (page1 > page2)
        return false;
    else if (offset1 < offset2)
        return true;
    else
        return false;
}

//只进行键值比较,返回key1<key2
bool SIndexManager::compareKey(void *key1, void *key2)
{
    //先行比较数值
    if ((this->ixType) == INT)
    {
        if (*((int *)key1) < *((int *)key2))
            return true;
        else if (*((int *)key1) > *((int *)key2))
            return false;
    }
    else if ((this->ixType) == FLOAT)
    {
        if (*((double *)key1) < *((double *)key2))
            return true;
        else if (*((double *)key1) > *((double *)key2))
            return false;
    }
    else if ((this->ixType) == STRING)
    {
        for (int i = 0; i < (this->ixSize); i++)
        {
            if (((char *)key1)[i] < ((char *)key2)[i])
                return true;
            else if (((char *)key1)[i] > ((char *)key2)[i])
                return false;
        }
    }
    return false;
}

bool SIndexManager::OpenScan(void *key, bool lower) {
	int page = lower ? -1 : 1 << 31;
	int offset = page;

    //获取根节点的页号
    int id = this->rootIdx;
    BPlusNode *node = new BPlusNode();
    this->readNode(node, id);
    while (*node->isLeaf == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        while (i > 0 && compare(key, node->key + i * (this->ixSize), page, node->pages[i], offset, node->offsets[i]))
            i--;
        id = node->childs[i];
        this->readNode(node, id);
    }
    scan_nodeID = id;
	scan_entryID = -1;
	for (int i = 0; i <= *node->keyNum; i++) {
        if (i == *node->keyNum) {
            scan_nodeID = *node->next;
		    scan_entryID = 0;
        } else if (compare(key, node->key + i * (this->ixSize), page, node->pages[i], offset, node->offsets[i])) {
            scan_entryID = i;
			break;
        }
	}
    delete node;
	if (scan_nodeID == 0) return false;
	return true;
}

bool SIndexManager::GetNextEntry(int &page, int &offset) {
	if (scan_nodeID == 0) return false;
	BPlusNode *node = new BPlusNode();
    this->readNode(node, scan_nodeID);
	bpm->access(node->pageID);
	page = node->pages[scan_entryID]; offset = node->offsets[scan_entryID];
	if (scan_entryID == *node->keyNum - 1) {
        // printf("Enter next node\n");
		if (*node->next == 0) {
			delete node;
			return false;
		}
		scan_nodeID = *node->next;
		scan_entryID = 0;
	} else
		scan_entryID++;
	delete node;
	return true;
}

bool SIndexManager::GetPrevEntry(int &page, int &offset) {
	if (scan_nodeID == 0) return false;
	BPlusNode *node = new BPlusNode();
    this->readNode(node, scan_nodeID);
	bpm->access(node->pageID);
	page = node->pages[scan_entryID]; offset = node->offsets[scan_entryID];
	if (scan_entryID == 0) {
		if (*node->prev == 0) {
			delete node;
			return true;
		}
		scan_nodeID = *node->prev;
		this->readNode(node, scan_nodeID);
        bpm->access(node->pageID);
		scan_entryID = *node->keyNum - 1;
	} else
		scan_entryID--;
	delete node;
	return true;
}

bool SIndexManager::CloseScan() {
    return true;
}

void SIndexManager::Print_Tree() {
    //获取根节点的页号
    int id = this->rootIdx;
    int keynub = Print_Node(id, 0);
    printf("共有%d个索引项\n",keynub);
}

int SIndexManager::Print_Node(int nodeID,int depth) {
    for(int i = 0;i < depth;i++){
        printf("    ");
    }
    BPlusNode *node = new BPlusNode();
    this->readNode(node, nodeID);
    printf("第%d层:%d, Parent:%d, Prev:%d, Next:%d, Min:",depth + 1,nodeID,*node->parent,*node->prev,*node->next);
    node->print_key(0,this->ixType,this->ixSize);
    printf(", Max:");
    node->print_key(*node->keyNum - 1,this->ixType,this->ixSize);
    printf(",KeyNum:%d\n",*node->keyNum);
    int keynub = *node->keyNum;
    if(*node->isLeaf == false){
        keynub = 0;
        for(int i = 0;i < *node->keyNum;i++) {
            keynub = keynub + Print_Node(node->childs[i], depth + 1);
        }
    }
    delete node;
    return keynub;
}