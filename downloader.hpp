#pragma once
#include <avhttp.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <fstream>

class Downloader : public boost::enable_shared_from_this<Downloader>
{
public:
    Downloader(boost::asio::io_service &io,avhttp::settings set)
    : _io_service(io),_stream(io){}

    void Start(std::string url)
    {
        try
        {
            _stream.start(_url,_set);
        }
        catch(std::exception &e)
        {
            std::cerr << "Error:" << e.what() << std::endl;
        }
    }
private:
	boost::asio::io_service  &_io_service;
    avhttp::http_stream _stream;
    avhttp::settings _set;
};
