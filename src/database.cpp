#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>

using namespace std;

#include "database.h"

#define DATABASE_FILE "taskDB.db"

extern const char *homedir;

void test_database::openConnection()
{
    cout << "Database file: " << std::string(homedir) +"/"+ DATABASE_FILE << endl;
    connectionStatus = !sqlite3_open( (char*)(std::string(homedir) +"/"+ DATABASE_FILE).c_str(), &db);

    if( !connectionStatus ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }
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
    int rc = sqlite3_exec(db, (char*)sqlTaskTable.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, (char*)sqlRunTable.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

int test_database::addTask(MakeTest test)
{
    char *zErrMsg = NULL;

    string mode;
    
    if(test.getDoTest()) {
        mode = "M";
    } else {
        mode = "m";
    }
    string sql = "INSERT INTO TASKS (TYPE,NAME,GIT_URL,DIR) "  \
             "VALUES ('"+mode+"', '"+test.getTestName()+"', '"+test.getURL()+"', '"+test.getDir()+"' ); ";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), NULL, 0, &zErrMsg);
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
    bool printNewLine = true;
    for(i=0; i<argc; i++){
        //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        if(!strcmp(azColName[i], "TIME")) {
            long int time = atol(argv[i]);
            printf("%s", ctime((time_t*)&time) );
            printNewLine = false;
        } else  if(!strcmp(azColName[i], "COMMIT_HASH")) {
            // Only print the first few characters
            for(int j=0; j<12; j++) {
                printf("%c", argv[i][j]);
            }
            printf("\t");
        } else if(!strcmp(azColName[i], "VALUE")) {
            if(!strcmp(argv[i], "0")) {
                printf("PASS\t");
            } else {
                printf("FAIL\t");
            }
        } else {
            printf("%s\t", argv[i] ? argv[i] : "NULL");
        }
    }
    if(printNewLine) {
        printf("\n");
    }
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
    }
}


static int getTasksCallback(void *testVector, int argc, char **argv, char **azColName){
    std::vector<MakeTest> *tests = (std::vector<MakeTest>*) testVector;
    int i;
    int id;
    string name;
    string dir;
    string url;
    bool runTest;
    for(i=0; i<argc; i++){
        if(!strcmp(azColName[i], "ID")) {
           id = atoi(argv[i]);
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
    MakeTest t(id, name, dir, url, runTest);
    t.createNewRepo();
    tests->push_back(t);
    //printf("\n");
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
    }
    return rc;
}

string newHash_for_callback;
static int getLastHashCallback(void *stringTarget, int argc, char **argv, char **azColName){
    newHash_for_callback = argv[0];
    return 0;
}
string test_database::getTestRunsHash(string taskID)
{
    char *zErrMsg = NULL;
    
    newHash_for_callback = "";

    string sql = "SELECT COMMIT_HASH from TEST_RUNS WHERE TASK='"+taskID+"' ORDER BY ID DESC LIMIT 1";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), getLastHashCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return newHash_for_callback;
}

int test_database::addTestRun(int taskID, string commitHash, int returnValue)
{
    char *zErrMsg = NULL;
    long int t = static_cast<long int> (time(NULL));
    string sql = "INSERT INTO TEST_RUNS (TASK, COMMIT_HASH, VALUE, TIME) "  \
             "VALUES ('"+std::to_string(taskID)+"', '"+commitHash+"', '"+std::to_string(returnValue)+"', '"+std::to_string(t)+"' ); ";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    return rc;
}

//the paramiter taskID should be an int but in the form of a string
void test_database::listTestRuns(string taskID)
{
    char *zErrMsg = NULL;

    cout <<   "ID \tTask\tCommit hash\tResult\tTime" << endl;
//             3	3	defcbeafa43e4cb9368147dd675a8507f5b9c56f	0	1397321152
    string sql = "SELECT * from TEST_RUNS WHERE TASK='"+taskID+"'";

    int rc = sqlite3_exec(db, (char*)sql.c_str(), listTasksCallback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

