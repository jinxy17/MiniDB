#include "SIndexManager.h"

SIndexManager::SIndexManager(BufPageManager *bpm, int fileID)
{
    this->bpm = bpm;
    this->fileID = fileID;
    BufType b = bpm->getPage(fileID, 0, this->infoPage); //将第0个页面(idx页)读入到缓存中，在缓存中页号为idxPage

    this->ixType = (int *)&b[0];
    this->ixSize = (int *)&b[1];
    this->ixPP = (int *)&b[2];
    this->rootIdx = (int *)&b[3];
    this->bmPage = (bitset<DATA_SIZE_IX * 8> *)((unsigned char *)b + DATA_OFFSET_IX);

    bpm->access(idxPage)
}

SIndexManager::~SIndexManager()
{
    this->bpm->close();
}

//按ixSize和ixType重置索引数据,在新建索引时使用
// 索引数据页面分区:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |
bool SIndexManager::init(int ixSize, int ixType)
{
    *this->ixType = ixType;
    *this->ixSize = ixSize;
    *this->ixPP = DATA_SIZE_IX / (sizeof(int) * 3 + ixSize);
    *this->rootIdx = 1;
    this->bmPage->reset();
    (*this->bmPage)[0] = true;
    (*this->bmPage)[1] = true;
    bpm->markDirty(this->infoPage);

    // 第一页 = 根节点
    int bufroot;
    BufType rootpage = bmp->getPage(this->fileID, 1, bufroot);
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
    bmp->markDirty(bufroot);
    bmp->writeBack(bufroot);
}

// 从第id个页中读取B+Tree节点信息
// 索引数据页面分区:
// | isLeaf  | keyNum  | parent  | prev    | next    | offset   | childs | pages | offsets | keys |
// | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 4 Bytes | 12 Bytes | 8160 Bytes                      |
void SIndexManager::readNode(BPlusNode *node, int id)
{
    // 缓存中调入第id页
    BufType pageStart = bmp->getPage(this->fileID, id, node->pageID);
    //读取此节点数据块信息
    node->isLeaf = (int *)&rootpage[0];
    node->keyNum = (int *)&rootpage[1];
    node->parent = (int *)&rootpage[2];
    node->prev = (int *)&rootpage[3];
    node->next = (int *)&rootpage[4];
    //获取指向四大数据区起始处的指针
    node->childs = pageStart + DATA_OFFSET_IX / sizeof(int);
    node->pages = pageStart + this->ixPP;
    node->offsets = pageStart + this->ixPP * 2;
    node->key = (unsigned char *)(pageStart + this->ixPP * 3);
    //留下缓存访问记录
    bmp->access(node->pageID);
    return node;
}

void SIndexManager::insertIx(void *key, int page, int offset)
{
    //获取根节点的页号
    int id = (*this->rootIdx);
    BPlusNode *node = new BPlusNode();
    this->readNode(node, id);
    while (node->isLeaf == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        while (i > 0 && !compare(node->key + i * (*this->ixSize), key, node->page[i], page, node->offset[i], offset))
            i--;
        id = node->child[i];
        this->readNode(node, id);
    }
    //找到了叶子节点,并将在节点中的某个位置插入
    int i = (*node->keyNum) - 1;
    while (1)
    {
        if (i < 0 || compare(node->key + i * (*this->ixSize), key, node->page[i], page, node->offset[i], offset))
        {
            // 插入条目
            memcpy(node->key + (i + 1) * (*this->ixSize), key, (*this->ixSize));
            node->page[i + 1] = page;
            node->offset[i + 1] = offset;
            node->child[i + 1] = 0;
            break;
        }
        //右侧条目右移
        memcpy(node->key + (i + 1) * (*this->ixSize), node->key + i * (*this->ixSize), (*this->ixSize));
        node->page[i + 1] = node->page[i];
        node->offset[i + 1] = node->offset[i];
        i--;
    }
    (*node->keyNum)++;
    bmp->markDirty(node->pageID);

    bool hasSplit = false;
    //节点满了,需要分裂节点
    while ((*node->keyNum) == this->ixPP)
    {
        hasSplit = true;
        //先获取父节点
        BPlusNode *parentNode;
        if (parent == 0)
        {
            //没有父节点,即根节点特判
            //产生一个新节点作为父节点(也是新的根节点)
            (*node->parent) = this->getEmptyPage();
            (*this->rootIdx) = (*node->parent);
            bpm->markDirty(infoPage);
            this->readNode(parentNode, (*node->parent));
            //设置新根节点的各项属性，新根节点初始有一个键值和一个子节点(即为原根节点),父节点为空,没有邻居节点
            *parentNode->isLeaf = false;
            *parentNode->keyNum = 1;
            *parentNode->parent = 0;
            *parentNode->prev = 0;
            *parentNode->next = 0;
            parentNode->child[0] = id;
            parentNode->page[0] = node->page[0];
            parentNode->offset[0] = node->offset[0];
            memcpy(parentNode->key, node->key, (*this->ixSize));
        }
        else
            this->readNode(parentNode, *node->parent);

        //产生新节点
        int newID = this->getEmptyPage();
        //设置新节点属性
        BPlusNode *newNode = new BPlusNode(newID);
        this->readNode(newNode, newID);
        *newNode->isLeaf = *node->isLeaf;
        *newNode->keyNum = (*node->keyNum) / 2; //键值数目减半
        *newNode->parent = (*node->parent);
        *newNode->next = (*node->next);
        *newNode->prev = id;
        *node->next = newID

            //迁移数据
            (*node->keyNum) = (*node->keyNum) - (*newNode->keyNum);
        for (int i = 0; i < (*newNode->keyNum); i++)
        {
            newNode->child[i] = node->child[i + (*node->keyNum)];
            newNode->page[i] = node->page[i + (*node->keyNum)];
            newNode->offset[i] = node->offset[i + (*node->keyNum)];
        }
        memcpy(newNode->key, node->key + (*node->keyNum) * (*this->ixSize), (*newNode->keyNum) * (*this->ixSize));

        // 更新子节点中的父节点序号
        BPlusNode *tmpNode = new BPlusNode();
        for (int i = 0; i < (*newNode->keyNum); i++)
        {
            if (!newNode->header.isLeaf && newNode->child[i] != 0)
            {
                readNode(tmpNode, newNode->child[i]);
                tmpNode->header.parent = newID;
                bmp->markDirty(tmpNode->pageID);
            }
        }
        // 更新邻居节点中的邻节点序号
        if ((*newNode->next) != 0)
        {
            readNode(tmpNode, (*newNode->next));
            (*tmpNode->prev) = newID;
            bmp->markDirty(tmpNode->pageID);
        }
        delete tmpNode;

        //更新父节点中的子节点信息,右边的节点给新节点腾出位置
        for (int i = (*parentNode->keyNum); i >= 1; i--)
        {
            if (parentNode->child[i - 1] == id)
            {
                //父节点的子节点数+1
                (*parentNode->keyNum)++;
                //设置父节点中的子节点信息,新旧节点都要设置
                memcpy(parentNode->key + (i - 1) * (*this->ixSize), node->key, (*this->ixSize));
                parentNode->page[i - 1] = node->page[0];
                parentNode->offset[i - 1] = node->offset[0];
                parentNode->child[i - 1] = id;

                memcpy(parentNode->key + i * (*this->ixSize), newNode->key, (*this->ixSize));
                parentNode->page[i] = newNode->page[0];
                parentNode->offset[i] = newNode->offset[0];
                parentNode->child[i] = newID;

                break;
            }
            memcpy(parentNode->key + i * (*this->ixSize), parentNode->key + (i - 1) * (*this->ixSize), (*this->ixSize));
            parentNode->page[i] = parentNode->page[i - 1];
            parentNode->offset[i] = parentNode->offset[i - 1];
            parentNode->child[i] = parentNode->child[i - 1];
        }

        bmp->markDirty(node->pageID);
        delete node;

        bmp->markDirty(newNode->pageID);
        delete newNode;

        id = *node->parent;
        node = parentNode;
        bmp->markDirty(node->pageID);
    }

    //向上更新父节点信息
    BPlusNode *parentNode = new BPlusNode();
    while (1)
    {
        if (id == (*this->rootIdx))
        {
            delete node;
            return true;
        }
        BPlusNode *parentNode = new BPlusNode();
        this->readNode(parentNode, (*node->parent));
        for (int i = (*parentNode->keyNum) - 1; i >= 0; i--)
        {
            if (parentNode->child[i] == id)
            {
                memcpy(parentNode->key + i * (*this->ixSize), node->key, (*this->ixSize));
                parentNode->page[i] = node->page[0];
                parentNode->offset[i] = node->offset[0];
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
    int id = (*this->rootIdx);
    BPlusNode *node = new BPlusNode();
    this->readNode(node, id);
    while (node->isLeaf == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        while (i > 0 && !compare(node->key + i * (*this->ixSize), key, node->page[i], page, node->offset[i], offset))
            i--;
        id = node->child[i];
        this->readNode(node, id);
    }
    //找到了叶子节点,进一步获取该key值在节点中的位置
    int pos = (*node->keyNum) - 1;
    while (1)
    {
        if (pos < 0)
        {
            cout << "Error!The key to delete is not existing\n";
            delete node;
            return false;
        }
        else if (compare( key, node->key + pos * (*this->ixSize), page,node->page[pos], offset, node->offset[pos]) && compare(node->key + pos * (*this->ixSize), key, node->page[pos], page, node->offset[pos], offset))
            break;
        pos--;
    }
    (*node->keyNum)--;

    //执行删除
    for (int i = pos; i < (*node->KeyNum); i++)
    {
        node->page[i] = node->page[i + 1];
        node->offset[i] = node->offset[i + 1];
    }
    memcpy(node->key + pos * (*this->ixSize), node->key + (i + 1) * (*this->ixSize), ((*node->KeyNum) - pos) * (*this->ixSize));
    bmp->markDirty(node->pageID);

    //向上更新父节点信息,并对空节点执行删除
    while (id != _header.rootPage)
    {
        bool empty = ((*node->KeyNum) == 0);
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
            if (tmpNode->child[i] == id)
            {
                //节点为空,删除之
                if (empty)
                {
                    for (int j = i; i < *tmpNode->keyNum - 1; j++)
                    {
                        tmpNode->child[j] = tmpNode->child[j + 1];
                        tmpNode->page[j] = tmpNode->page[j + 1];
                        tmpNode->offset[j] = tmpNode->offset[j + 1];
                    }
                    *tmpNode->keyNum--;
                    memcpy(tmpNode->key + i * (*this->ixSize), tmpNode->key + (i + 1) * (*this->ixSize), (*tmpNode->keyNum - i) * (*this->ixSize));
                }
                //节点非空,更新父节点
                else
                {
                    memcpy(tmpNode->key + i * (*this->ixSize), node->key, (*this->ixSize));
                    tmpNode->page[i] = node->page[0];
                    tmpNode->offset[i] = node->offset[0];
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

 //获取键值为key,位置大于page-offset的第一个项
bool SIndexManager::getIx(void *key,int page,int offset, int& rpage, int& roffset)
{
    //获取根节点的页号
    int id = (*this->rootIdx);
    BPlusNode* node = new BPlusNode();
    this->readNode(node, id);
    while (node->isLeaf == false)
    {
        //非叶子节点,继续向下深入
        //由右边向左遍历,寻找插入位置
        int i = (*node->keyNum) - 1;
        while (i > 0 && !compare(node->key + i * (*this->ixSize), key, node->page[i], page, node->offset[i], offset))
            i--;
        id = node->child[i];
        this->readNode(node, id);
    }
    //找到了叶子节点,进一步获取该key值在节点中的位置
    int pos = (*node->keyNum) - 1;
    while (1)
    {
        if (pos < 0)
        {
            cout << "Error!The key is not existing\n";
            delete node;
            return false;
        }
        else if (compareKey(key,node->key + pos * (*this->ixSize)) && compareKey(node->key + pos * (*this->ixSize), key))
            break;
        pos--;
    }
    rpage = node->page[pos];
    roffset = node->offset[pos];
    return true;
}

map<int, int> SIndexManager::getAllIx(void *key){
    map<int, int> mp;
    int page,offset,rpage,roffset;
    page = -1;
    offset = -1;
    while(getIx(key,page,offset,rpage,roffset)){
        mp.insert(make_pair(rpage,roffset));
        page = rpage;
        offset = roffset;
    }
    return mp;
}

//获取一个空页,返回页号
int SIndexManager::getEmptyPage()
{
    int curPage;
    for (curPage = 0;; curPage++)
    {
        if (curPage >= DATA_SIZE_IX << 3)
        {
            cout << "Error,The index file is full" << endl;
            return -1
        }
        else if (!(*this->bmPage)[curPage])
            break;
    }
    (*this->bmPage)[curPage] = true;
    bpm->markDirty(infoPage);
    return curPage;
}

//将某一页设置为空,返回成功与否
bool SIndexManager::setEmptyPage(int id)
{
    if (id >= 1 && id < DATA_SIZE_IX << 3)
    {
        (*this->bmPage)[curPage] = false;
        bpm->markDirty(infoPage);
        return true;
    }
    return false;
}

bool SIndexManager::compare(void *key1, void *key2, int page1, int page2, int offset1, int offset2)
{
    //先行比较数值
    if ((*this->ixType) == INT)
    {
        if (*((int *)key1) < *((int *)key2))
            return true;
        else if (*((int *)key1) > *((int *)key2))
            return false;
    }
    else if ((*this->ixType) == FLOAT)
    {
        if (*((double *)key1) < *((double *)key2))
            return true;
        else if (*((double *)key1) > *((double *)key2))
            return false;
    }
    else if ((*this->ixType) == STRING)
    {
        for (int i = 0; i < (*this->ixSize); i++)
        {
            if (((char *)key1)[i] < ((char *)key2)[i])
                return true else if (((char *)key1)[i] > ((char *)key2)[i]) return false;
        }
    }

    //再比较存储的位置
    return (page1<<13+offset1) < (page2<<13+offset2)
    // if (page1 < page2)
    //     return true;
    // else if (page1 > page2)
    //     return false;
    // else if (offset1 < offset2)
    //     return true;
    // else
    //     return false;
}

//只进行键值比较,返回key1<key2
bool SIndexManager::compareKey(void *key1, void *key2)
{
    //先行比较数值
    if ((*this->ixType) == INT)
    {
        if (*((int *)key1) < *((int *)key2))
            return true;
        else if (*((int *)key1) > *((int *)key2))
            return false;
    }
    else if ((*this->ixType) == FLOAT)
    {
        if (*((double *)key1) < *((double *)key2))
            return true;
        else if (*((double *)key1) > *((double *)key2))
            return false;
    }
    else if ((*this->ixType) == STRING)
    {
        for (int i = 0; i < (*this->ixSize); i++)
        {
            if (((char *)key1)[i] < ((char *)key2)[i])
                return true else if (((char *)key1)[i] > ((char *)key2)[i]) return false;
        }
    }
    return false;
}