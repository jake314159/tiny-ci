#include <iostream>
#include <string>

using namespace std;

#include "Result.h"
#include "gitTools.h"

// Numbers are fixed so we can be sure the values don't change between versions
enum TestMode {
    PASS = 1,
    FAIL = 0,
    UNTESTED = 3,
    PAUSED = 5
};

class Test
{
    protected:
        int id;
        string testName;
        int returnCode;
        string returnString;
        TestMode mode = UNTESTED;
        std::string dir;
        std::string lastHash = "";
        std::string url; 
    public:
        Test(int id, string testName, string dir, string url)
        {
            this->id = id;
            this->testName = testName;
            this->dir = dir;
            this->url = url;
            //pullNewGit(url, dir);
        }
        virtual ~Test()
        {
            testName.clear();
            returnString.clear();
        }

        //Methods which should be implemented in the subclasses
        virtual int performTest(Result&) = 0;
        void saveTest(FILE *fp);


        // 0    No changes
        //!0    Some changes have been applied
        int checkForChange();
        void createNewRepo();

        string getTestName()
        {
            return testName;
        }

        string getReturnString()
        {
            return returnString;
        }

        int getReturnCode()
        {
            return returnCode;
        }

        TestMode getMode()
        {
            return mode;
        }

        void pause()
        {
            mode = PAUSED;
        }

        void unpause()
        {
            mode = UNTESTED;
        }

        void setLastHash(std::string nHash)
        {
            lastHash = nHash;
        }
        string getStoredLastHash()
        {
            return lastHash;
        }
        string getDir()
        {
            return dir;
        }
        string getURL()
        {
            return url;
        }
        int getID()
        {
            return id;
        }
};
