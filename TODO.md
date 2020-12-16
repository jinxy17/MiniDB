功能检查:  
1. 索引相关检查
2. 主键相关检查
3. 外键相关检查

功能添加:
1. \<dbStmt> 需要修改
2. DESC \<tbName>没有实现
3. ~~INSERT暂时只支持单条数据插入 => 底层不好改,多次调用insert~~
4. ~~UPDATE需要实现WHERE~~
5. ~~DEFAULT => 实际无意义,可以无视~~
6. DATE类型的处理 => 用INT保存?
7. ~~NULL的处理 => update不能修改null~~
8. ~~UPDATE需要实现支持多列修改~~
9. char不能建表 ？
10. 列应该改为默认NOT NULL
