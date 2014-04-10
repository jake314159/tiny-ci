#include <string>

#include "Test.h"

using namespace std;

int Test::checkForChange()
{
    std::string newHash = getLastHash(dir);

    int returnValue = newHash.compare(lastHash);
    if( returnValue != 0) {
        lastHash = newHash;
    }

    return returnValue;
}

void Test::createNewRepo()
{
    pullNewGit(url, dir);
}
