#include <sqlite3.h>

#include "MakeTest.h"

class test_database
{
    private:
        void openConnection();
        void initTable();
        sqlite3 *db;
        int connectionStatus = false;
    public:
        test_database()
        {
            openConnection();
            initTable();
        }
        ~test_database()
        {
            sqlite3_close(db);
        }
        int addTask(MakeTest test);
        void listTasks();
        int getTasks(std::vector<MakeTest> *tests);

        int isOpen()
        {
            return connectionStatus;
        }  

};
