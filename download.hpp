#pragma once
#include <avhttp.hpp>
#include <iostream>
#include <stdio.h>
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
    
            boost::thread twork(boost::bind(&boost::asio::io_service::run, &_io_service));
            
            std::cout << "-------------------------------------------------------------------------" << std::endl;
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
                    //boost::this_thread::sleep(boost::posix_time::millisec(200));
                    printf("%3d%% [", percent);
                    int progress = percent / 2;
                    for (int i = 0; i < progress; i++)
                        printf("=");
                    if (progress != 50)
                        printf(">");
                    for (int i = 0; i < 49 - progress; i++)
                        printf(" ");
                    printf("]%s%s", add_suffix(bytes_download).c_str(), add_suffix(_stream.download_rate()).append("/s").c_str());
                    fflush(stdout);
                    printf("\r");
                }
                printf("\n");
            }
    
            twork.join();
    
            std::cout << std::endl << "****** download completed! ******" << std::endl;
            std::cout << "-------------------------------------------------------------------------" << std::endl;
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