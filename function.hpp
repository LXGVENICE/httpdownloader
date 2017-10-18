#pragma once
#include "urldata.hpp"
#include "function.hpp"
#include "downloader.hpp"
#include <avhttp.hpp>
#include <string>
#include <unistd.h>
#include <fstream>

/*
void SetParameter(avhttp::setting &set,std::string path = "./")
{   
    if(path.empty())
    {
        std::cout << "error path" << std::endl;
        exit(0);
    }
    //set.save_path = path;
    //disable_multi_download = false;
}
*/

/*
void SignHandler(int sign)
{
    
}
*/

int CreateWork(int num)
{
	int workid = -1;
    if(num <= 0)
    {
        return workid;
    }
    else
    {
        for(int i = 1;i < num;++i)
        {
            int pid = fork();
            if(pid < 0)
            {
                std::cout << "error create worker" << std::endl;
                exit(0); 
            }
            if(pid == 0)
            {
				workid = i;
                break;
            }
            else
            {
				workid = 0;	
                continue;
            }
        }
    }
	if(num == 1)
	{
		workid = 0;
	}
	return workid;
}

void Run(std::string url)
{
	boost::asio::io_service io;
	avhttp::settings set;
	Downloader down(io,set);
    down.Start(url);
    down.Stop();
}

