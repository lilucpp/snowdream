# database

## 简介
数据库的学习过程，入门使用了《SQL必知必会 第四版》，想系统了解数据库和表设计知识，选用的是《数据库系统概念 第六版》。

不错的资料：
- [sqlite教程](https://www.runoob.com/sqlite/sqlite-tutorial.html)
- [sql tutorial](https://www.tutorialspoint.com/sql/index.htm)
- [sqlite keywords](https://sqlite.org/lang_keywords.html)

## 笔记
1. 外键约束是个好的设计。

    person(person_id, name, dept_id); dept(dept_id, name);  foreign key (dept_id) references dept(dept_d);  如果存在dept(2, test) && person(1, peanut, 2); 
    删除数据 delete from dept where dept_id =2; 会出错（foreign key constraint failed）。可以删除person(1, peanut, 2);后再删除dept(2, test)。
    同样drop table dept;同样出错，因为存person 的外键约束。

    如果存在涉及多个关系额外键依赖链，则在链一段所做的删除或更新可能传至整个链。

    创建外键时可以指定外键的行为，动作：on delete 、on update 行为： no action、set null、set default、cascade
    ```sql
    FOREIGN KEY("dept_id") REFERENCES "dept"("dept_id") on delete cascade
    ```

2. distinct关键字
   
    DISTINCT 关键字与 SELECT 语句一起使用，来消除所有重复的记录，并只获取唯一一次记录。

3. sql查询的含义可以理解如下
   - 1 from子句列出的关系产生笛卡尔积
   - 2 在步骤1的结果上应用where子句中指定的谓语
   - 3 对于步骤2中的每个元组，输出select子句中指定的属性

4. 如果单引号是字符串的组成部分，那就用两个单引号字符表示。
5. [连接](https://blog.csdn.net/qq_38125058/article/details/79946850)
   
   - 自然连接（natural join）
   只考虑哪些在两个关系模式中都出现的属性上取值相同的元组对。
   不用指定连接列，也不能使用on语句，默认比较两张表里相同的列。
   ```sql
   SELECT * FROM student NATURAL JOIN score;
   ```
   - 外连接（outer join）

   sqlite3仅支持左外连接。
      - 左外连接（left outer join）返回指定左表的全部行+右表对应的行，如果左表中数据在右表中没有与其相匹配的行，则在查询结果集中显示为空值。
      ```sql
      SELECT student.name,score.code FROM student LEFT OUTER JOIN score ON score.code=student.code;
      SELECT student.name,score.code FROM student NATURAL LEFT OUTER JOIN score;
      ```

      - 右外连接（right outer join) 与左外连接类似，是左外连接的反向连接。
      ```sql
      SELECT student.name,score.code FROM student RIGHT OUTER JOIN score ON score.code=student.code;
      ```

      - 全外连接（full outer join）把左右两表进行自然连接，左表在右表没有的显示NULL，右表在左表没有的显示NULL。（MYSQL不支持全外连接，适用于Oracle和DB2。）
      在MySQL中，可通过求左外连接与右外连接的合集来实现全外连接。


   - 内连接（inner join）    
   不保留未匹配元组的连接运算称为内连接（inner join）运算。
   和自然连接区别之处在于内连接可以自定义两张表的不同列字段。
   内连接有两种形式：显式和隐式。

      - 隐式的内连接，没有INNER JOIN，形成的中间表为两个表的笛卡尔积。
      ```sql
      SELECT student.name,score.code FROM student,score WHERE score.code=student.code;
      ```
      - 显示的内连接，一般称为内连接，有INNER JOIN，形成的中间表为两个表经过ON条件过滤后的笛卡尔积。
      ```sql
      SELECT student.name,score.code FROM student INNER JOIN score ON score.code=student.code;
      ```
6. 视图
   
   - 安全考虑。可能需要向用户隐藏特定的数据。
   - 希望创建一个比逻辑模型更符合特定用户直觉的个人化的关系集合。

   ```sql
   create view v as <query expression>;
   ```
7. 完整性约束
   
   保证授权用户对数据库所做的修改不会破坏数据的一致性。因此，完整性约束防止的是对数据的意外破坏。
   完整性约束通常被看作是数据库模式设计的一部分，它作为用于创建关系的create table 命令的一部分声明。也可以通过alter table table-name add constraint命令添加到已有的关系上。
   not null、unique、check(P)。

8. 索引
   
   在关系的属性上所创建的索引是一种数据结构，它允许数据库系统高效的找到关系中那些在索引属性上取给定值的元组，而不用扫描关系中的所有元组。

9.  **预备语句**
    
    通过以“?”来代表以后再给出的实际值，而创建一个预备语句。数据库系统在准备好了查询语句的时候对它进行编译。在每次执行该语句时（用新值替换“?”）,数据库可以重用预先编译的查询的形式，应用新值进行查询。

    - 预备语句使得执行更加高效：同一查询编译一次然后设置不同的参数值执行多次的情况下。

    - 安全：使用了用户输入值时，预备语句是执行SQL的首选方法。检查用户输入了单引号，除非进行检查 ，否则生成的sql语句会出现语法错误。bind的设值，会自动完成检查，并插入需要的转义字符，保证语法的正确性。更重要的是能防止sql注入。

10. 函数和过程
    
   sqlite不支持。MariaDb下的一个example
   ```sql
   DELIMITER //
   create function dept_count(dept_name varchar(20))
   returns integer
   begin
   declare d_count integer;
   select count(*) into d_count from instructor
   where instructor.dept_name = dept_name;
   return d_count;
   end //
   DELIMITER ;

   select dept_name, budget from department where dept_count(dept_name) > 1;
   ```
11. 递归查询
    If you're lucky enough to be using SQLite 3.8.3 or higher then you do have access to recursive and non-recursive CTEs using WITH。
    https://sqlite.org/lang_with.html

## 数据库设计喝E-R模型
最终，数据库的设计将会表示为一个关系数据库设计和一个与之关联的约束集合。

实体-联系（entity-relationship， E-R）、E-R图

使用实体集还是用属性，看具体情况，如果需要那个属性的额外信息，就使用实体集。另外不要用一个实体集的主键作为另一个实体集的属性，而应该用联系（视情况而定）。

附录：
1. [数据库系统概念(原书第六版)》中样例的建表及插入简单实例的SQL语句](https://blog.csdn.net/m0_37961948/article/details/88426110)
   
   sql脚本名：[database.sql](./database.sql)

2. [笛卡尔积](https://zh.wikipedia.org/wiki/%E7%AC%9B%E5%8D%A1%E5%84%BF%E7%A7%AF)
   example X{A,K,Q,J,10,9,8,7,6,5,4,3,2} Y{♠, ♥, ♦, ♣}, 则这两个集合的笛卡儿积是有52个元素的标准扑克牌的集合{(A,♠),(K,♠),...,(2,♠),...,(A,♣),...,(3,♣),(2,♣)}。

   在数据库中的体现就是:每一条数据相当于集合中的一个数，而(A,♠)的集合相当于并。如下
   a.id  a.name a.b_id b.b_id b.name 

   1  lilu  2  2 wlh

   1  lilu  2  3 xz

   2  zyh   3  2 wlh

   2  zyh   3  3 xz

3. mysql命令自动补全
   mysql -u root -p --auto-rehash

4. sqlite不支持 create function
   SQLite does not have a stored function/stored procedure language. So CREATE FUNCTION does not work.