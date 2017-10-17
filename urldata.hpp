#pragma once
#include <string>
#include <vector>

typedef unsigned int size_t;

//存储一条url数据
class Urldata
{
public:
    Urldata(std::string &url,int threadnum):_url(url),_threadnum(threadnum){}
public:
    std::string _url;//保存url
    int _threadnum;//连接数，默认为-1
};

//下载类
class Download
{
public:
    Download(){}

    void Append(std::string &str,int num = -1)
    {
        _udata.push_back(Urldata(str,num));
    }

    int GetNum()
    {
        return _udata.size();
    }

	std::string GetUrl(int pos)
	{
		return _udata[pos]._url;
	}

private:
    std::vector<Urldata> _udata;
};
