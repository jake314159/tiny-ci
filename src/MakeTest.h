#include "Test.h"
using namespace std;

class MakeTest: public Test
{
    public:
        MakeTest(string testName): Test(testName)
        {

        }
        int performTest(Result&);
};
