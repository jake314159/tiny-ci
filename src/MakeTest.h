
#include <fstream>
#include "Test.h"

using namespace std;

class MakeTest: public Test
{
    private:
        bool doTest = false;
    public:
        MakeTest(int id, string testName, string dir, string url): Test(id, testName, dir, url)
        {
            
        }
        MakeTest(int id, string testName, string dir, string url, bool doTestSet): Test(id, testName, dir, url)
        {
            doTest = doTestSet;
        }

        int performTest(Result&);
        void saveTest(FILE *fp);
        static MakeTest parseSaveString(string);

        bool getDoTest()
        {
            return doTest;
        }
};

