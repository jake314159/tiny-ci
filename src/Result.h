#include <iostream>
#include <string>
using namespace std;

class Result
{
    private:
        int returnValue;
        string returnString;
    public:
        int getReturnValue();
        string getReturnString();
        void setReturnValue(int);
};
