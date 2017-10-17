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
    int threadnum;//连接数，默认为-1
};

//下载类
class Download
{
public:
    Download():_workernum(0){}

    void Append(string &str,int num = -1)
    {
        _udata.push_back(Urldata(str,num));
        ++_workernum;
    }

    int GetWorker()
    {
        return _workernum
    }
private:
    vector<Urldata> _udata;
    int _workernum;
}