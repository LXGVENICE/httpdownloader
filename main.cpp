#include <iostream>
#include <string>
#include <regex>
#include "urldata.hpp"
#include "function.hpp"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "No input urls!" << std::endl;
        return 0;
    }

    std::string path = CreatePath(argc,argv);

    std::regex reg("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
    std::smatch match;

    Download downer;

    for(std::size_t i = 1;i < argc;++i)
    {
        std::string tmp(argv[i]);
        if(!std::regex_match(tmp,match,reg))
        {
            std::cout << tmp << " is not a vaile url" << std::endl;
            continue;
        }
        downer.Append(tmp);
    }

    avhttp::settings set;
    SetParameter(set,path);
	
	int num = downer.GetNum();
	if(num == 0)
	{
		return 0;
	}
    int workid = CreateWork(num);
    if(workid == -1)
	{
		std::cout << "error create" << std::endl;
		exit(0);
    }
    Run(downer.GetUrl(workid),set);
	return 0;
}
