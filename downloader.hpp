#pragma once
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class downloader : public boost::enable_shared_from_this<downloader>
{
public:
    downloader(boost::asio::io_service &io,avhttp::setting set)
    : _io_service(io),_stream(io),_set(set){}

    void Start(string url)
    {
        _stream.start(url,_set);
    }

private:
	boost::asio::io_service  _&io_service;
    avhttp::multi_download _stream;
    avhttp::setting _set;
};