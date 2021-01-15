#include "Executer.h"

Executer::Executer() {}

Executer::~Executer() {}

void Executer::init()
{   
    MyBitMap::initConst();
    fileManager = new FileManager();
    bufPageManager = new BufPageManager(fileManager);
    indexManager = new IndexManager(fileManager, bufPageManager);
    smm = new SysManager(indexManager, fileManager, bufPageManager);
    qlm = new DataOperater(smm, indexManager, fileManager, bufPageManager);
    currentdb = "";
    
    if(access(this->rootpath.c_str(),F_OK) == -1)
        system(("mkdir " + this->rootpath).c_str());
    chdir(this->rootpath.c_str());
}

void Executer::exit() {
    if(currentdb != "") smm->CloseDB();
    chdir("..");
}

void Executer::execSysStmt(sysStmt *stmt)
{
    //show databases
    assert(stmt->sttype == Stmt::SYS_STMT);
    // 暂时关闭数据库
    if(currentdb != "")
        smm->CloseDB();
    struct dirent **namelist;
    int cnt;
    cnt = scandir(".", &namelist, 0, alphasort);
    if(cnt <= 2)
    { 
        printf("There is no database\n");
    }
    else
    {
        int index = 2;
        while(index < cnt)
        {
            printf(" %d: %s;\n",index - 1, namelist[index]->d_name);
            free(namelist[index]);
            ++index;
        }
        free(namelist[0]);
        free(namelist[1]);
        free(namelist);
    }

    //再次进入工作目录(当前数据库)
    if(currentdb != "")
	    smm->OpenDB(currentdb);
    return;
}

void Executer::execDbStmt(dbStmt *stmt)
{
    assert(stmt->sttype == Stmt::DB_STMT);
    switch (stmt->dbType)
    {
    case dbStmt::DB_CREATE:
    {   
        // 暂时关闭数据库
        if(currentdb != "")  smm->CloseDB();
        
        // CREATE DATABASE <dbName>
        // 数据库已存在
        if(access(stmt->dbName.c_str(),F_OK) != -1) {
            printf("The database %s has been created.\n",stmt->dbName.c_str());
        } else {
            system(("mkdir " + stmt->dbName).c_str());
            string metapath = stmt->dbName + "/meta.db";
            FILE *fp = fopen(metapath.c_str(),"w+");
            if(fp == NULL) {
                printf("Error occured when write into meta.db\n");
            }else {
                fprintf(fp,"0\n0\n");
                fclose(fp);
                printf("Create database: %s \n",stmt->dbName.c_str());
            }
        }

        //再次进入工作目录(当前数据库)
        if(currentdb != "")   smm->OpenDB(currentdb);
        break;
    }
    case dbStmt::DB_DROP:
    {
        // 暂时关闭数据库
        if(currentdb != "")  smm->CloseDB();

        // DROP DATABASE <dbName>
        if(access(stmt->dbName.c_str(),F_OK) != -1) {
	        system(("rm -r " + stmt->dbName).c_str());
            printf("The database %s has been dropped.\n",stmt->dbName.c_str());
        } else {
            // 数据库不存在
            printf("The database %s does not exist.\n",stmt->dbName.c_str());
        }

        //再次进入工作目录(当前数据库)
        if(currentdb != "")   smm->OpenDB(currentdb);
        break;
    }
    case dbStmt::DB_USE:
    {
        if(stmt->dbName == currentdb) {
            printf("The Database %s is in use;\n",stmt->dbName.c_str());
        }else {
            if(currentdb != "")  smm->CloseDB();
            if(access(stmt->dbName.c_str(),F_OK) != -1) {
                // USE <dbName>
                // 暂时关闭数据库
                currentdb = stmt->dbName;
                smm->OpenDB(currentdb);
                printf("Open Database %s;\n",currentdb.c_str());
                return;
            } else {
                // 数据库不存在
                printf("The database %s does not exist.\n",stmt->dbName.c_str());
                return;
            }
        }
        break;
    }
    case dbStmt::DB_SHOW:
    {
        // SHOW TABLES
        if(currentdb == "") {
            printf("There is no database in use.\n");
        }else
        {
            smm->Show();
        }
        break;
    }
    default:
        assert(1);
        return;
    }
    return;
}

void Executer::execTbStmt(tbStmt *stmt)
{
    if(currentdb == ""){
        printf("Please choose a Database to use.\n");
        return;
    }
    assert(stmt->sttype == Stmt::TB_STMT);
    switch (stmt->tbType)
    {
    case tbStmt::TB_CREATE:
    {   
        //CREATE TABLE tb (id INT NOT NULL,score FLOAT NOT NULL,age INT);
        // CREATE TABLE tb1 (id1 INT,score1 FLOAT,age1 INT);
        // CREATE TABLE tb2 (id2 INT,score2 FLOAT,age2 INT);
        // CREATE TABLE <tbName> ’(’<fieldList>’)’
        // for(int i = 0;i < stmt->tableInfo->attrNum;i++)
        //     printf("col %d :%s\n",i,stmt->tableInfo->attrs[i].attrName.c_str());
        // 设置主键
        if(stmt->pk != nullptr) {
            for(int i = 0;i < stmt->pk->namelist.size();i++) {
                string pkname = stmt->pk->namelist[i]; 
                for(int j = 0;j < stmt->tableInfo->attrNum;j++) {
                    if(stmt->tableInfo->attrs[j].attrName == pkname){
                        stmt->tableInfo->attrs[j].primary = true;
                        stmt->tableInfo->attrs[j].notNull = true;
                    }
                }
            }
        }
        // 设置外键
        if(stmt->fks.size() > 0) {
            for(int i = 0;i < stmt->fks.size();i++) {
                string fkname = stmt->fks[i]->attrName;
                for(int j = 0; j < stmt->tableInfo->attrNum;j++) {
                    if(stmt->tableInfo->attrs[i].attrName == fkname){
                        stmt->tableInfo->attrs[i].reference = stmt->fks[i]->reference;
                        stmt->tableInfo->attrs[i].foreignKeyName = stmt->fks[i]->foreignKeyName;
                    }
                }
            }
        }
        smm->CreateTable(stmt->tableInfo);
        break;
    }
    case tbStmt::TB_DROP:
    {
        smm->DropTable(stmt->tbName);
        break;
    }
    case tbStmt::TB_DESC:
    {
        smm->Show(stmt->tbName);
        break;
    }
    case tbStmt::TB_INSERT:
    {   
        //INSERT INTO NATION VALUES (0,'America',0, 'nothing left');
        //INSERT INTO NATION VALUES  ('id86','America',0, 'nothing left');
        //INSERT INTO ORDERS VALUES (127664,315000,'F',6.5,'2018/2/29',0,'Clerk101',1,'nice service');
        //SELECT * FROM CUSTOMER WHERE C_CUSTKEY=5;
        //DELETE FROM CUSTOMER WHERE C_CUSTKEY=5;
        //SELECT PS_AVAILQTY,PS_PARTKEY FROM PARTSUPP WHERE PS_PARTKEY=12;
        //UPDATE PARTSUPP SET PS_AVAILQTY=8774 WHERE PS_PARTKEY=12;
        //SELECT C_NATIONKEY FROM CUSTOMER WHERE C_NATIONKEY<1;
        //SELECT C_NATIONKEY FROM CUSTOMER WHERE C_NATIONKEY<10;
        //SELECT O_ORDERDATE,O_TOTALPRICE FROM ORDERS WHERE O_ORDERDATE='1996-1-2';
        //SELECT CUSTOMER.C_PHONE,ORDERS.O_ORDERSTATUS,CUSTOMER.C_NAME FROM CUSTOMER,ORDERS WHERE CUSTOMER.C_CUSTKEY=ORDERS.O_CUSTKEY AND CUSTOMER.C_NAME='Customer#000000001';
        //UPDATE NATION SET N_REGIONKEY=316001 WHERE N_NATIONKEY=15;
        //ALTER TABLE NATION ADD n_comment_2 VARCHAR(32);
        //ALTER TABLE NATION DROP n_comment_2;
        //ALTER TABLE NATION DROP PRIMARY KEY;
        //ALTER TABLE NATION ADD CONSTRAINT pk1 PRIMARY KEY (N_NATIONKEY);
        //ALTER TABLE NATION ADD CONSTRAINT NATION_FK1 FOREIGN KEY (N_REGIONKEY) REFERENCES REGION(R_REGIONKEY);
        //ALTER TABLE NATION DROP FOREIGN KEY NATION_FK1;
        //ALTER TABLE NATION RENAME TO province;
        //ALTER TABLE CUSTOMER ADD INDEX Idx_residual (C_ACCTBAL)
        //ALTER TABLE CUSTOMER DROP INDEX Idx_residual;

        //CREATE INDEX IdX_NATION ON CUSTOMER (C_NATIONKEY);


        //INSERT INTO tb VALUES (1,1.5,10), (2,2.5,12),(3,3.5,15)
        //INSERT INTO tb1 VALUES (1,1.5,10), (2,2.5,12),(3,3.5,15)
        //INSERT INTO tb2 VALUES (1,1.5,10), (2,2.5,12),(3,3.5,15)
        //INSERT INTO tb VALUES (NULL,58.6,15),(5,NULL,17);
        //INSERT INTO tbName VALUES valueLists
        for(int i = 0;i < stmt->datas.size();i++) {
            vector<Value*> rdata;
            vector<int> cols;
            rdata.clear();
            cols.clear();
            int nullnub = 0;
            for(int j = 0;j < stmt->datas[i].size();j++){
                if(stmt->datas[i][j]->datatype == DNULL) {
                    nullnub++;
                }else{
                    rdata.push_back(stmt->datas[i][j]);
                    cols.push_back(j);
                }
            }
            if(qlm->Insert(stmt->tbName, rdata, cols, nullnub) == 0){
                break;
            }
        }
        break;
    }
    case tbStmt::TB_DELETE:
    {
        //DELETE FROM tb WHERE id >= 3
        qlm->Delete(stmt->tbName, stmt->relations);
        break;
    }
    case tbStmt::TB_UPDATE:
    {
        //UPDATE tbName SET setClause WHERE whereClauses
        //UPDATE tb SET id = 1 , score = 51 WHERE id > 3 AND id < 6;
        //UPDATE tb SET id = 15 , score = 54.5 WHERE id IS NULL;
        qlm->Update(*stmt->assigns, stmt->relations);
        break; 
    }
    case tbStmt::TB_SELECT:{
        // SELECT tb.id FROM tb WHERE tb.id > 0;
        // SELECT tb.id,tb1.id1 FROM tb,tb1 WHERE tb.id > 0;
        // SELECT tb1.id1 FROM tb1 WHERE tb1.id1 > 0;
        // SELECT tb2.id2 FROM tb2 WHERE tb2.id2 > 0;
        // SELECT tb.id,tb1.id1 FROM tb,tb1 WHERE tb.id = tb1.id1;
        // SELECT * FROM tb,tb1,tb2 WHERE tb.id = tb1.id1;
        // TODO:多表SELECT
        // SELECT <selector> FROM <tableList> WHERE <whereClause>
        // SELECT * FROM tb WHERE id > 0 AND score > 1.0;
        // SELECT id FROM tb WHERE id > 0 AND score > 1.0;
        // SELECT * FROM tb WHERE id IS NULL;
        vector<string> attrNames;
        for(int i = 0;i < stmt->collist.size();i++){
            attrNames.push_back(stmt->collist[i]->colname);
        }
        // printf("%ld,%ld\n",attrNames.size(),stmt->collist.size());
        if(stmt->tablelist.size() == 1)
            qlm->Select(stmt->tablelist[0], stmt->relations,attrNames);
        else
            qlm->Select(stmt->tablelist,stmt->collist,stmt->relations);
        break;
    }
    case tbStmt::TB_LOAD:{
        //LOAD PART FROM 'part.tbl.csv';
        //LOAD REGION FROM 'region.tbl.csv';
        //LOAD NATION FROM 'nation.tbl.csv';
        //LOAD SUPPLIER FROM 'supplier.tbl';
        //LOAD CUSTOMER FROM 'customer.tbl.csv';
        //LOAD PARTSUPP FROM 'partsupp.tbl.csv';
        //LOAD ORDERS FROM 'orders.tbl.csv';
        //LOAD LINEITEM FROM 'lineitem.tbl.csv';
        printf("Load data to tablle %s from %s\n",stmt->tbName.c_str(),stmt->filename.c_str());
        //当前工作目录是某个数据库目录下,因此..回到.MiniDB,../..回到Debug,../../..回到项目根目录
        string filepath = "../../../data/" + stmt->filename;
        char line[1024];
        FILE* stream = fopen(filepath.data(), "r");
	    int i = 0;
	    while (fgets(line, 1024, stream) != NULL)//逐行读取
        {
            i++;
            if(i % 10 == 0)
                printf("%s",line);
            qlm->Load(stmt->tbName,line);
            memset(line,0,1024 * sizeof(char));
        }
        fclose(stream);//文件打开后要进行关闭操作
        break;
    }
    default:
        assert(1);
        return;
    }
    return;
}

void Executer::execIdxStmt(idxStmt *stmt)
{
    assert(stmt->sttype == Stmt::IDX_STMT);
    if(currentdb == ""){
        printf("Please choose a Database to use.\n");
        return;
    }
    switch (stmt->idxType)
    {
    case idxStmt::IDX_CREATE:
    {   
        // CREATE INDEX idxName ON tbName '(' columnList ')'
        // CREATE INDEX idxid ON tb (id)
        // CREATE INDEX idxidsc ON tb (id,score)
        smm->CreateIndex(stmt->idxName,stmt->tbName,stmt->collist);
        break;
    }
    case idxStmt::IDX_DROP:
    {
        // DROP INDEX idxid
        // printf("Drop index\n");
        smm->DropIndex(stmt->idxName);
        break;
    }
    case idxStmt::IDX_ALTER_ADD:
    {
        smm->AddIndex(stmt->idxName,stmt->collist);
        break;
    }
    case idxStmt::IDX_ALTER_DROP:
    {   
        smm->DropIndex(stmt->idxName);
        break;
    }
    default:
        assert(1);
        return;
    }
    return;
}

void Executer::execAlterStmt(alterStmt *stmt)
{
    assert(stmt->sttype == Stmt::ALTER_STMT);
    if(currentdb == ""){
        printf("Please choose a Database to use.\n");
        return;
    }
    switch (stmt->alterType)
    {
    case alterStmt::ALTER_ADD:
    {   
        smm->AddColumn(stmt->tbName,*(stmt->field));
        break;
    }
    case alterStmt::ALTER_DROP:
    {
        smm->DropColumn(stmt->tbName,stmt->colName);
        break;
    }
    case alterStmt::ALTER_CHANGE:
    {
        smm->ChangeColumn(stmt->tbName, stmt->colName, *stmt->field);
        break;
    }
    case alterStmt::ALTER_RENAME:
    {   
        smm->RenameTable(stmt->tbName, stmt->newtbName);
        break;
    }
    case alterStmt::ALTER_DROP_PRIMARY:
    {   
        smm->DropPrimaryKey(stmt->tbName);
        break;
    }
    case alterStmt::ALTER_ADD_PRIMARY:
    {   
        smm->AddPrimaryKey(stmt->tbName,stmt->collist,stmt->pkName);
        break;
    }
    case alterStmt::ALTER_DROP_PRIMARY_WNAME:
    {   
        smm->DropPrimaryKey(stmt->tbName, stmt->pkName);
        break;
    }
    case alterStmt::ALTER_ADD_FOREIGN:
    {   
        smm->AddForeignKey(stmt->tbName,stmt->fkName,stmt->collist,stmt->fktbName,stmt->fkcollist);
        break;
    }
    case alterStmt::ALTER_DROP_FOREIGN_WNAME:
    {   
        smm->DropForeignKey(stmt->tbName,stmt->fkName);
        break;
    }
    default:
        assert(1);
        return;
    }

    return;
}