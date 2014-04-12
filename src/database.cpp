#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#include "database.h"

#define DATABASE_FILE "taskDB.db"

void test_database::openConnection()
{
    connectionStatus = !sqlite3_open(DATABASE_FILE, &db);

    if( connectionStatus ){
        fprintf(stderr, "Opened database successfully\n");
    }else{
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
}

static int initTableCallback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void test_database::initTable()
{
    char *zErrMsg = NULL;
    std::string sqlTaskTable = "CREATE TABLE IF NOT EXISTS TASKS("  \
         "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
         "TYPE           CHAR(1)      NOT NULL," \
         "NAME           VARCHAR(100) NOT NULL," \
         "GIT_URL        TEXT," \
         "DIR            TEXT         NOT NULL);";

    std::string sqlRunTable = "CREATE TABLE IF NOT EXISTS TEST_RUNS("  \
         "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
         "TASK           INTEGER      NOT NULL," \
         "COMMIT_HASH    CHAR(40)     NOT NULL," \
         "VALUE          INTEGER      NOT NULL," \
         "TIME           LONG         NOT NULL," \
         "FOREIGN KEY (TASK) REFERENCES TASKS(ID));";

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, (char*)sqlTaskTable.c_str(), initTableCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Task table created successfully\n");
    }

    rc = sqlite3_exec(db, (char*)sqlRunTable.c_str(), initTableCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Test run table created successfully\n");
    }
}

int test_database::addTask(MakeTest test)
{
    char *zErrMsg = NULL;

    string sql = "INSERT INTO TASKS (TYPE,NAME,GIT_URL,DIR) "  \
         "VALUES ('M', '"+test.getTestName()+"', '"+test.getURL()+"', '"+test.getDir()+"' ); ";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), initTableCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Task added successfully\n");
    }
    return 0;
}

static int listTasksCallback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      printf("%s\t", argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void test_database::listTasks()
{
    char *zErrMsg = NULL;

    string sql = "SELECT * from TASKS";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), listTasksCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Task added successfully\n");
    }
}


static int getTasksCallback(void *testVector, int argc, char **argv, char **azColName){
   std::vector<MakeTest> *tests = (std::vector<MakeTest>*) testVector;
   int i;
    string name;
    string dir;
    string url;
    bool runTest;
   for(i=0; i<argc; i++){
        if(!strcmp(azColName[i], "ID")) {
            cout << "THIS IS AN ID";
        } else if(!strcmp(azColName[i], "NAME")) {
            name = argv[i];
        } else if(!strcmp(azColName[i], "GIT_URL")) {
            url = argv[i];
        } else if(!strcmp(azColName[i], "DIR")) {
            dir = argv[i];
        } else if(!strcmp(azColName[i], "TYPE")) {
            if(!strcmp(argv[i], "M")) { //Make with test
                runTest = true;
            } else if(!strcmp(argv[i], "m")) { //Make without test
                runTest = false;
            } else {
                cout << "Unsupported test type: " << argv[i] << endl;
                return 1;
            }
        } else {
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
        //printf("%s\t", argv[i] ? argv[i] : "NULL");
    }
    MakeTest t(name, dir, url, runTest);
    t.createNewRepo();
    tests->push_back(t);
    printf("\n");
    return 0;
}

int test_database::getTasks(std::vector<MakeTest> *tests)
{
    char *zErrMsg = NULL;

    string sql = "SELECT * from TASKS";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), getTasksCallback, (void*)tests, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Task added to vector successfully\n");
    }
    return rc;
}

