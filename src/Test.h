#include <iostream>
#include <string>
#include "Result.h"
using namespace std;

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
        string testName;
        int returnCode;
        string returnString;
        TestMode mode = UNTESTED;
    public:
        Test(string testName)
        {
            this->testName = testName;
        }
        ~Test()
        {
            testName.clear();
            returnString.clear();
        }


        int performTest(Result&);
        // 0    No changes
        //!0    Some changes have been applied
        int checkForChange();

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
};
