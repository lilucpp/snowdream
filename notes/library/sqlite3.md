# sqlite
## 概述
1. sqlite3的学习可以是先使用下面的示例，搞清楚基本用法。然后再看官网文档：[intro](https://www.sqlite.org/cintro.html)

- 两个对象：
  - sqlite3
  - sqlite3_stmt）

- 八个方法：
  - sqlite3_open()
  - sqlite3_prepare()
  - sqlite3_bind()
  - sqlite3_step()
  - sqlite3_column()
  - sqlite3_finalize()
  - sqlite3_close()
  - sqlite3_exec()
2. 中文版书籍没有适合自己的，sqlite权威指南完全不对自己的胃口，仅学习了上面的文档。
c++包装库也特别推荐的，看了一部分，没有特别想用的，不如直接使用api简单明了。仅在项目中使用过[CppSQLite](https://www.codeproject.com/Articles/6343/CppSQLite-C-Wrapper-for-SQLite)。
3. **sqlite3支持的sql语句，参考官方文档：https://sqlite.org/lang.html**
4. 一些总结：
- bind 是一些数据，不能是表名， where语句，可以是where 的判断条件数据。
- 多进程同时操作数据库可能会产生busy， 多线程不会。尽量避免多进程操作同一数据库，如果无法避免可以考虑使用rpc，在统一进程中完成操作。<br>
[threadsafe](https://www.sqlite.org/threadsafe.html)
- 多看手册：https://www.sqlite.org/docs.html
- 尽量使用预处理语句，防止sql注入。
- sqlite中的转义符号
  ```txt
   /   ->    // 
   '   ->    '' 
   [   ->    /[ 
   ]   ->    /] 
   %   ->    /% 
   &   ->    /& 
   _   ->    /_ 
   (   ->    /( 
   )   ->    /) 
  ```
  - 使用sqlite3_mprintf("%q", sql);// 注意使用完用sqlite3_free释放
  - 使用替换函数替换
  - bind

## sqlite3使用
```c++
// 每一行数据调用一次
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int test_sqlite3() {
  sqlite3 *db;
  char *error_msg = 0;
  int rc;
  rc = sqlite3_open("test.db", &db);
  if (rc) {
    fprintf(stderr, "Failed to open DB: %s\n", sqlite3_errmsg(db));
    return (1);
  }
  rc = sqlite3_exec(db, "select * from test;", callback, 0, &error_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
  }
  sqlite3_close(db);
  return 0;
}
```
## sqlite3_stmt使用
```c++
int test_sqlite3_stmt() {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  int rc;
  rc = sqlite3_open("test.db", &db);
  if (rc) {
    fprintf(stderr, "Failed to open DB: %s\n", sqlite3_errmsg(db));
    return (1);
  }

  int ret = sqlite3_prepare_v2(db, "select * from test;", -1, &stmt, NULL);
  if (ret) {
    fprintf(stderr, "sqlite3_prepare_v2 error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }

  printf("Got results:\n");
  int count = 1;
  while (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("\ncount=%d\n", count++);
    int i;
    int num_cols = sqlite3_column_count(stmt);

    for (i = 0; i < num_cols; i++) {
      switch (sqlite3_column_type(stmt, i)) {
        case (SQLITE3_TEXT):
          printf("%s, ", sqlite3_column_text(stmt, i));
          break;
        case (SQLITE_INTEGER):
          printf("%d, ", sqlite3_column_int(stmt, i));
          break;
        case (SQLITE_FLOAT):
          printf("%g, ", sqlite3_column_double(stmt, i));
          break;
        default:
          break;
      }
    }
    printf("\n");
  }

  sqlite3_finalize(stmt);

  sqlite3_close(db);
  return 0;
}
```
## bind的使用

bind 是一些数据，不能是表名， where语句，可以是where 的判断条件数据。
```c++
int test_bind_values() {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    sqlite3_open(argv[1], &db);
    if (db == NULL) {
      printf("Failed to open DB\n");
      return 1;
    }

    int ret =  sqlite3_prepare(db, "select * from file_paths where fp_id < ?", -1, &stmt, NULL);
    if (ret) {
      fprintf(stderr, "sqlite3_prepare_v2 error: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 0;
    }

    int num = 20;
    ret = sqlite3_bind_text(stmt, 1, num,-1, SQLITE_STATIC);
    if (ret) {
      fprintf(stderr, "sqlite3_bind_text error: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 0;
    }

    printf("Got results:\n");
    int count = 1;
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      printf("\ncount=%d\n", count++);
      int i;
      int num_cols = sqlite3_column_count(stmt);

      for (i = 0; i < num_cols; i++) {
        switch (sqlite3_column_type(stmt, i)) {
          case (SQLITE3_TEXT):
            printf("%s, ", sqlite3_column_text(stmt, i));
            break;
          case (SQLITE_INTEGER):
            printf("%d, ", sqlite3_column_int(stmt, i));
            break;
          case (SQLITE_FLOAT):
            printf("%g, ", sqlite3_column_double(stmt, i));
            break;
          default:
            break;
        }
      }
      printf("\n");
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}
```
## 多线程下测试
多进程同时操作数据库可能会产生busy， 多线程不会。
```c++
void test_muil_thread(int base) {
  sqlite3 *DB;
  std::string sql = "CREATE TABLE test ("
	"id	INTEGER PRIMARY KEY AUTOINCREMENT,"
	"name	TEXT);";

  int exit = sqlite3_open("example.db", &DB);
  char *error_msg=NULL;
  exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error_msg);
  if (exit != SQLITE_OK) {
    std::cerr << "Error Create Table: " << error_msg << std::endl;
    sqlite3_free(error_msg);
  } else
    std::cout << "Table created Successfully" << std::endl;

  std::thread t1([&DB, base]() {
    for (int i = 0+base; i < 1000+base; ++i) {
      std::string sql = "INSERT INTO test (name) VALUES(" + std::to_string(i) + ");";

      char *error_msg;
      int ret = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error_msg);
      int count = 0;
      while (SQLITE_BUSY == ret) {
       ++count;
        sqlite3_free(error_msg);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ret = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error_msg);
      }
      if (count > 0) {
        std::cerr << std::this_thread::get_id() << " Error Insert (" << i
                  << ") " << count++ << std::endl;
      }
      if (ret != SQLITE_OK) {
        std::cerr << std::this_thread::get_id()
                  << " Error Insert " << i << " : " << error_msg << std::endl;
        sqlite3_free(error_msg);
      }
    }
    });
  std::thread t2([&DB, base]() {
    for (int i = 1000+base; i < 2000+base; ++i) {
      std::string sql = "INSERT INTO test (name) VALUES(" + std::to_string(i) + ");";

      char *error_msg;
      int count = 0;
      int ret = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error_msg);
      while (SQLITE_BUSY == ret ) {
        ++count;
        sqlite3_free(error_msg);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ret = sqlite3_exec(DB, sql.c_str(), NULL, 0, &error_msg);
      }
      if (count > 0) {
        std::cerr << std::this_thread::get_id() << " Error Insert (" << i
                  << ") " << count++  << std::endl;
      }
      if (ret != SQLITE_OK) {
        std::cerr << std::this_thread::get_id()
        << " Error Insert " << i << " : " << error_msg << std::endl;
        sqlite3_free(error_msg);
      }
    }
  });

  t1.join();
  t2.join();
  sqlite3_close(DB);
}
```

## 加密数据库

常见的有sqlcipher、wxsqlite3。（编译问题参考《常见库的编译.md》）  

[sqlcipher加解密](http://yzsijin.cn/2021/06/06/%E7%BD%91%E7%BB%9C%E5%AE%89%E5%85%A8-%E6%95%B0%E6%8D%AE%E5%BA%93%E5%8A%A0%E5%AF%86/)：
对于一开始就是用 sqlite3 代码来实现的数据库操作，切到 sqlcipher 进行数据库加密是很简单的，只需要在 sqlite3_open 操作后，仅跟着 sqlite3_key 接口即可，其他操作逻辑都不需要修改，这样新创建的数据库就是经过加密了的。  

但如果需要考虑低版本升级到高版本这样的兼容性问题，则还需要进行明文数据库迁移到加密数据库操作。    

对应的执行语句，官方也给了详细的操作语句，如下：  

```txt
Example 1: Encrypt a Plaintext Database

$ ./sqlcipher plaintext.db
sqlite> ATTACH DATABASE 'encrypted.db' AS encrypted KEY 'testkey';
sqlite> SELECT sqlcipher_export('encrypted');
sqlite> DETACH DATABASE encrypted;
Example 2: Decrypt a SQLCipher database to a Plaintext Database

$ ./sqlcipher encrypted.db
sqlite> PRAGMA key = 'testkey';
sqlite> ATTACH DATABASE 'plaintext.db' AS plaintext KEY '';  -- empty key will disable encryption
sqlite> SELECT sqlcipher_export('plaintext');
sqlite> DETACH DATABASE plaintext;
```

对应的语句在 C 代码中都可以通过 sqlite3_exec 接口进行执行实现。  

## 事务
You use sqlite3_exec and pass "BEGIN TRANSACTION" and "END TRANSACTION" respectively.
```c++
// 'db' is the pointer you got from sqlite3_open*
sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
// Any (modifying) SQL commands executed here are not committed until at the you call:
sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
```

There are synonyms for these SQL commands (like COMMIT instead of END TRANSACTION). For reference, here is the SQLite documentation for transactions.

[How to commit a SQLite transaction in C/C++?](https://stackoverflow.com/a/2893146)