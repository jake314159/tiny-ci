
#include <sqlite3.h>

#include "MakeTest.h"
#include "MavenTest.h"
#include "BashTest.h"

class test_database
{
    private:
        
        sqlite3 *db;
        int connectionStatus = false;
    public:
        test_database()
        {
            //openConnection();
            //initTable();
        }
        ~test_database()
        {
            sqlite3_close(db);
        }
        void openConnection();
        void initTable();
        int addTask(MakeTest test);
        int addTask(MavenTest test);
        int addTask(BashTest test);
        void listTasks();
        void listTask(string ID);
        int getTasks(std::vector<Test*> *tests);
        int addTestRun(int taskID, string commitHash, int returnValue);
        void deleteTask(string taskID);
        //the paramiter taskID should be an int but in the form of a string
        void listTestRuns(string taskID);
        string getTestRunsHash(string taskID);
        TestMode getLastTestResult(string taskID);

        int isOpen()
        {
            return connectionStatus;
        }  

};

