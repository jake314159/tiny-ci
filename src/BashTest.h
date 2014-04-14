
#include <fstream>
#include "Test.h"

using namespace std;

class BashTest: public Test
{
    private:
        string scriptName;
    public:
        BashTest(int id, string testName, string dir, string scriptName, string url): Test(id, testName, dir, url)
        {
            this->scriptName = scriptName;
        }
        
        int performTest(Result&);
        void saveTest(FILE *fp);

        string getScriptName()
        {
            return scriptName;
        }
};

