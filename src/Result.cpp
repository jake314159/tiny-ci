#include "Result.h"
using namespace std;

string Result::getReturnString()
{
    return returnString;
}

int Result::getReturnValue()
{
    return returnValue;
}

void Result::setReturnValue(int newValue)
{
    returnValue = newValue;
}

void Result::setReturnString(string newString)
{
    returnString = newString;
}
