/**
 * HTTPDownloader.h
 *
 * A simple C++ wrapper for the libcurl easy API.
 *
 * Written by Uli Köhler (techoverflow.net)
 * Published under CC0 1.0 Universal (public domain)
 */
#ifndef HTTPDOWNLOADER_H
#define HTTPDOWNLOADER_H

#include <string>

/**
 * A non-threadsafe simple libcURL-easy based HTTP downloader
 */
class HTTPDownloader {
public:
    HTTPDownloader();
    ~HTTPDownloader();
    /**
     * Download a file using HTTP GET and store in in a std::string
     * @param url The URL to download
     * @return The download result
     */
    std::string download(void * curl, const std::string& url);

    std::string download_authorised(void * curl, const std::string& url);
private:
    
};

#endif  /* HTTPDOWNLOADER_H */