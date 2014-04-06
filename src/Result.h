#include <iostream>
#include <string>
using namespace std;

class Result
{
    private:
        int returnValue;
        string returnString;
    public:
        ~Result()
        {
            returnString.clear();
        }
        int getReturnValue();
        string getReturnString();
        void setReturnValue(int);
        void setReturnString(string);
};
