# MiniDB 期中报告
清华计算机系 数据库系统概论课程

## 作者
金旭扬，李啸宇

## 进展一：记录管理模块

### 负责人
金旭扬

### 相关文件
`recmgr/RecManager.h`

`Debug/testfilesystem.cpp`

### 接口文档
```cpp
#define DATA_OFFSET 1024 // offset of DATA section for each page, in bytes
#define ID_SEGM 16 // pos of seperator for rec id, in bits
#define DATA_SIZE (PAGE_SIZE-DATA_OFFSET)

class RecManager {
	RecManager(BufPageManager * bpm, int fileID, int recSize); 
	// bpm: pointer to BufPageManager
	// fileID: current file id
	// recSize: size for each rec, in bytes
	
	~RecManager();
	
	void insertRec(BufType e, unsigned int & id);
	// e: pointer to a rec
	// id: returns id
	
	void deleteRec(unsigned int id);
	// id: id of rec to be deleted 
	
	void updateRec(BufType e, unsigned int id);
	// e: pointer to a rec
	// id: id of rec to be updated
		
	class Iterator { // nested iterator class
		Iterator(RecManager * rm); 
		// rm: pointer to RecManager
		
		bool next(BufType & e, unsigned & id);
		// e: returns pointer to next rec
		// id: returns id of next rec
		// return: false if reaches end, else true
	};
};
```

### 存在的问题
1. 测试不够全面。目前只实现了`testfilesystem.cpp`中对各个接口功能的简单测试，未充分考虑边界条件和规模变大后的情况；应该进一步扩大规模，用随机插入/删除/更新的方法来进行测试。
2. `#define DATA_OFFSET 1024`相当于将文件页的页头固定为1024字节，占据了1/8的文件页空间，非常浪费。（SQL Server中页头仅96字节。）这样做的原因是为了应对记录大小为1字节时，位图所占空间最大为1024字节时的情况。但实际上可以强制每个记录所占空间最小为4字节，进一步节省页头大小。
3. 未按照作业要求将“新建文件、删除文件、打开文件、关闭文件”封装进`RecManager.h`，需要用户手动调用`FileManager`和`BufPageManager`。

## 进展二：索引管理模块

### 负责人
李啸宇

### 相关文件
`xxx.cpp`

`xxx.cpp`

### 接口文档
```cpp
class doc here...
```

