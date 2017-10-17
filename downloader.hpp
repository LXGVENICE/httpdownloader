#pragma once
#include <avhttp.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class Downloader : public boost::enable_shared_from_this<Downloader>
{
public:
    Downloader(boost::asio::io_service &io,avhttp::settings set)
    : _io_service(io),_stream(io),_set(set){}

	~Downloader()
	{
		_stream.stop();
	}
    void Start(std::string url)
    {
        _stream.start(url);
    }
private:
	boost::asio::io_service  &_io_service;
    avhttp::multi_download _stream;
    avhttp::settings _set;
};
