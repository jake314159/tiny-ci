#include "Test.h"
#include "gitTools.h"
using namespace std;

class MakeTest: public Test
{
    private:
        std::string lastHash = "";
        std::string dir;
    public:
        MakeTest(string testName, string dir, string url): Test(testName)
        {
            pullNewGit(url, dir);
            this->dir = dir;
        }
        int performTest(Result&);
        int checkForChange();
};
