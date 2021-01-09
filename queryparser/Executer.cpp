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
        }else if(access(stmt->dbName.c_str(),F_OK) != -1) {
            // USE <dbName>
            currentdb = stmt->dbName;
            smm->OpenDB(currentdb);
            printf("Open Database %s;\n",currentdb.c_str());
            return;
        } else {
            // 数据库不存在
            printf("The database %s does not exist.\n",stmt->dbName.c_str());
            return;
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
        // CREATE TABLE <tbName> ’(’<fieldList>’)’
        // for(int i = 0;i < stmt->tableInfo->attrNum;i++)
        //     printf("col %d :%s\n",i,stmt->tableInfo->attrs[i].attrName.c_str());
        // 设置主键,只支持单列主键
        if(stmt->pk != nullptr) {
            string pkname = stmt->pk->namelist[0]; 
            for(int i = 0;i < stmt->tableInfo->attrNum;i++) {
                if(stmt->tableInfo->attrs[i].attrName == pkname){
                    stmt->tableInfo->attrs[i].primary = true;
                    stmt->tableInfo->attrs[i].notNull = true;
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
        //INSERT INTO tb VALUES (1,1.5,10), (2,2.5,12),(3,3.5,15)
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
        qlm->Select(stmt->tablelist[0], stmt->relations,attrNames);
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