#include <string>
#include <iostream>
#include <stdio.h>
#include "gitTools.h"
using namespace std;

#define HASH_LENGTH 40

std::string getLastHash(std::string dir)
{
    std::string cmd = std::string("cd ") + dir + " && " + GIT_PULL + " && " + GIT_LAST_HASH;
    //cout << "cmd " << cmd << endl;
    FILE* pipe = popen((char*)cmd.c_str(), "r");
    if (!pipe) return "ERROR";

    //+1 for the '\0'
    char buffer[HASH_LENGTH+1];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, HASH_LENGTH, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

int pullNewGit(std::string url, std::string dir)
{
    int value;
    // Check if processor is available
    if (!system(NULL)) {
        exit (EXIT_FAILURE);
    }

    //run command
    std::string cmd = std::string("mkdir -p ") + dir + " && " + 
                        "cd " + dir + " && " + 
                        GIT_INIT + " && " + 
                        GIT_ADD_REMOTE(url) + " && "
                        GIT_PULL;
    value = system ((char*)cmd.c_str());
    return value;
}
