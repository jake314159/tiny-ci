
#include <fstream>
#include "Test.h"

using namespace std;

class MavenTest: public Test
{
    private:
    public:
        MavenTest(int id, string testName, string dir, string url): Test(id, testName, dir, url)
        {
            
        }

        int performTest(Result&);
        void saveTest(FILE *fp);    
};
