using namespace std;
#include <iostream>
#include "MakeTest.h"

int main(int argc, char *argv[])
{
    MakeTest t("My test!");
    Result r;
    t.performTest(r);
    cout << "Result output of the test is " << r.getReturnValue() << endl;
}
