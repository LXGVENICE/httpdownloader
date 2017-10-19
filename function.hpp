#pragma once
#include "urldata.hpp"
#include "function.hpp"
#include "download.hpp"
#include <avhttp.hpp>
#include <string>
#include <unistd.h>
#include <fstream>
#include <regex>


void SetParameter(avhttp::settings &set,std::string &path)
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

std::string CreatePath(int &targc, char* argv[])
{
    std::string path;
    if(targc < 3 || 0 != strcmp(argv[targc-2],"-p"))
    {
        path.append("./");
    }
    else
    {
        /*找不到合适的正则表达式
        std::regex reg("/^\/([/w]+\/?)+$/i");
        std::smatch match;
        if(std::regex_match(std::string(argv[argc-2]),match,reg))
        {
            printf("Warning：%s is not a valid path，will use current directory\n",argv[argc-2]);
        }
        */
        path.append(argv[targc-1]);
        targc -= 2;
    }
    return path;
}

void Run(std::string url,std::string &path)
{
	boost::asio::io_service io;
	Downloader down(io,path);
    down.Start(url);
}

