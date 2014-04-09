#include <iostream>
#include <string>
using namespace std;

#define GIT_LAST_HASH "git log --pretty=format:'%H' -n 1"
#define GIT_PULL "git pull origin master 1> /dev/null 2> /dev/null"
#define GIT_INIT "git init 1> /dev/null 2> /dev/null"
#define GIT_ADD_REMOTE(URL) ("git remote add origin " + URL + "1> /dev/null 2> /dev/null")

std::string getLastHash(string dir);
int pullNewGit(std::string url, std::string dir);
