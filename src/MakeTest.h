#include <fstream>
#include "Test.h"
#include "gitTools.h"
using namespace std;

class MakeTest: public Test
{
    private:
        std::string lastHash = "";
        std::string dir;
        std::string url;
        bool doTest = false;
    public:
        MakeTest(string testName, string dir, string url): Test(testName)
        {
            pullNewGit(url, dir);
            this->dir = dir;
            this->url = url;
        }
        MakeTest(string testName, string dir, string url, bool doTestSet): Test(testName)
        {
            pullNewGit(url, dir);
            this->dir = dir;
            this->url = url;
            doTest = doTestSet;
        }

        void setLastHash(std::string nHash)
        {
            lastHash = nHash;
        }

        int performTest(Result&);
        int checkForChange();
        void saveTest(FILE *fp);
        static MakeTest parseSaveString(string);
};
