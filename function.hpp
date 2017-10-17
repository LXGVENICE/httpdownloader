#pragma once
#include "urldata.hpp"
#include "function.hpp"
#include <string>
#include <unistd.h>

void SetParameter(avhttp::setting &set,std::string &path = './')
{   
    if(path.empty())
    {
        std::cout << "error path" << std::endl;
        exit(0);
    }
    set.save_path = path;
    //disable_multi_download = false;
}

/*
void SignHandler(int sign)
{
    
}
*/

bool CreateWork(int num)
{
    if(num < 0)
    {
        return fasle;
    }
    else
    {
        for(int i = 0;i < num;++i)
        {
            int pid = fork();
            if(pid < 0)
            {
                std::cout << "error create worker" << std::endl;
                exit(0); 
            }
            if(pid == 0)
            {
                break;
            }
            else
            {
                continue;
            }
        }
    }
    return true；
}

void run()
{
    boost
}

