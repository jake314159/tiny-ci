#include "Test.h"
#include "gitTools.h"
using namespace std;

class MakeTest: public Test
{
    private:
        std::string lastHash = "";
        std::string dir;
        bool doTest = false;
    public:
        MakeTest(string testName, string dir, string url): Test(testName)
        {
            pullNewGit(url, dir);
            this->dir = dir;
        }
        MakeTest(string testName, string dir, string url, bool doTestSet): Test(testName)
        {
            pullNewGit(url, dir);
            this->dir = dir;
            doTest = doTestSet;
        }
        int performTest(Result&);
        int checkForChange();
};
