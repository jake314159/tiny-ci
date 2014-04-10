#include <fstream>
#include "Test.h"
using namespace std;

class MakeTest: public Test
{
    private:
        bool doTest = false;
    public:
        MakeTest(string testName, string dir, string url): Test(testName, dir, url)
        {
            
        }
        MakeTest(string testName, string dir, string url, bool doTestSet): Test(testName, dir, url)
        {
            doTest = doTestSet;
        }

        int performTest(Result&);
        void saveTest(FILE *fp);
        static MakeTest parseSaveString(string);
};
