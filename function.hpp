#pragma once
#include "urldata.hpp"
#include "function.hpp"
#include "download.hpp"
#include <avhttp.hpp>
#include <string>
#include <unistd.h>
#include <fstream>

void SetParameter(avhttp::setting &set,std::string &path)
{   
    set.save_path = path;
    //disable_multi_download = false;
}

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

std::string CreatePath(int &argc, char* argv[])
{
    std::string path;
    if(argc < 3 || !strcmp(argv[argc-2],"-p"))
    {
        path.append("./")
    }
    else
    {
        std::regex reg("/^\/([/w]+\/?)+$/i");
        std::smatch match;
        if(std::regex_match(argv[argc-2],match,reg))
        {
            printf("Warning：%s is not a valid path，will use current directory\n");
        }
        path.append(argv[argc-2]);
        argc -= 2;
    }
    return path;
}

void Run(std::string url)
{
	boost::asio::io_service io;
	avhttp::settings set;
	Downloader down(io,set);
    down.Start(url);
}

