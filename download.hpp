#pragma once
#include <avhttp.hpp>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <time.h>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

std::string to_string(int v, int width)
{
    std::stringstream s;
    s.flags(std::ios_base::right);
    s.width(width);
    s.fill(' ');
    s << v;
    return s.str();
}

/*
std::string& to_string(float v, int width, int precision = 3)
{
    // this is a silly optimization
    // to avoid copying of strings
    static std::string sBuf[20];
    static int round_robin = 0;
    std::string& ret = sBuf[round_robin++];
    if (round_robin >= 20) round_robin = 0;
    ret.resize(20);
    int size = sprintf(&ret[0], "%*.*f", width, precision, v);
    ret.resize((std::min)(size, width));
    return ret;
}
*/

std::string add_suffix(float val, char const* suffix = 0)
{
    std::string ret;
    if (val == 0)
    {
        ret.resize(6, ' ');
        if (suffix) ret.resize(6 + strlen(suffix), ' ');
        return ret;
    }

    const char* prefix[] = {"kB", "MB", "GB", "TB"};
    const int num_prefix = sizeof(prefix) / sizeof(char*);
    for (int i = 0; i < num_prefix; ++i)
    {
        val /= 1024.f;
        if (std::fabs(val) < 1024.f)
        {
            ret = to_string(val, 4);
            ret += prefix[i];
            if (suffix) ret += suffix;
            return ret;
        }
    }
    ret = to_string(val, 4);
    ret += "PB";
    if (suffix) ret += suffix;
    return ret;
}

std::string get_time(time_t &start,time_t end)
{
    long len = end - start;
    int hour =  len / 3600;
    len -= len / 3600;
    int minute = len / 60;
    int second = len % 60;

    std::string ans;
    if(hour)
    {
        std::ostringstream oss;
        oss << hour;
        ans.append(oss.str());
        ans.append("h:");
    }
    if(minute)
    {
        std::ostringstream oss;
        oss << minute;
        ans.append(oss.str());
        ans.append("m:");
    }

    std::ostringstream oss;
    oss << second;
    ans.append(oss.str());
    ans.append(1,'s');

    return ans;
}

class Downloader : public boost::enable_shared_from_this<Downloader>
{
public:
    Downloader(boost::asio::io_service &io,std::string &path)
    :_io_service(io),_stream(io)
    {
        _set.save_path = path;
    }

    void Start(std::string &url)
    {
        if(!GetFile(url))
        {
            std::cout << "download error" << std::endl;
        }
    }
    
    bool GetFile(std::string &str)
    {
        try 
        {    
            _stream.start(str, _set);
    
            if (_stream.file_size() != -1)
                std::cout << "file---'" << _stream.file_name().c_str() <<
                "---' size is: " << "(" << _stream.file_size() << " bytes) " << 
                add_suffix(_stream.file_size()).c_str() << std::endl;
            
            time_t start,end;
            start = time(NULL);
            boost::thread twork(boost::bind(&boost::asio::io_service::run, &_io_service));
            
            std::cout << "---------------------------------------------------------------------------------------" << std::endl;
            if (_stream.file_size() != -1)
            {
                int percent = 0;
                long long file_size = _stream.file_size();
                long long bytes_download = 0;
                printf("\n");
                while (percent != 100)
                {
                    bytes_download = _stream.bytes_download();
                    percent = ((double)bytes_download / (double)file_size) * 100.0f;
                    boost::this_thread::sleep(boost::posix_time::millisec(200));
                    printf("%3d%% [", percent);
                    int progress = percent / 2;
                    for (int i = 0; i < progress; i++)
                        printf("=");
                    if (progress != 50)
                        printf(">");
                    for (int i = 0; i < 49 - progress; i++)
                        printf(" ");
                    printf("]%s  %s", add_suffix(bytes_download).c_str(), add_suffix(_stream.download_rate()).append("/s").c_str());
                    end = time(NULL);
                    std::string t = get_time(start,end);
                    printf("   time: %s",t.c_str());

                    fflush(stdout);
                    printf("\r");
                }
                printf("\n");
            }
    
            twork.join();
    
            std::cout << std::endl << "****** download completed! ******" << std::endl;
            std::cout << std::endl << "---------------------------------------------------------------------------------------" << std::endl;
            fflush(stdout);
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    
        return true;;
    }


private:
	boost::asio::io_service  &_io_service;
    avhttp::multi_download _stream;
    avhttp::settings _set;
};