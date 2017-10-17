#include <iostream>
#include <string>
#include <regex>
#include "urldata.hpp"
#include "function.hpp"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Please input urls" << std::endl;
        retur 0;
    }

    std::regex reg("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
    std::smatch match;

    Download downer;

    for(std::size_t i = 1;i < argc - 1;++i)
    {
        string tmp(argv[i]);
        if(!std::regex_match(tmp,match,reg))
        {
            std::cout << tmp << " is not a vaile url" << std::endl;
            continue;
        }
        downer.Append(tmp);
    }

    avhttp::setting set;
    SetParameter(set);

    if(!reateWork(downer.GetWorker()))
    {
        std::cout << "error create" << std::endl;
        exit(0);
    }

    run();
}