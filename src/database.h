
#include <sqlite3.h>

#include "MakeTest.h"
#include "MavenTest.h"

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
        void listTasks();
        int getTasks(std::vector<Test*> *tests);
        int addTestRun(int taskID, string commitHash, int returnValue);
        //the paramiter taskID should be an int but in the form of a string
        void listTestRuns(string taskID);
        string getTestRunsHash(string taskID);

        int isOpen()
        {
            return connectionStatus;
        }  

};

