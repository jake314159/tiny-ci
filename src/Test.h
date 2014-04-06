#include <iostream>
#include <string>
#include "Result.h"
using namespace std;

class Test
{
    protected:
        string testName;
        int returnCode;
        string returnString;
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
};
