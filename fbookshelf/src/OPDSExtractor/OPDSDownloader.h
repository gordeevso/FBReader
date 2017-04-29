#ifndef OPDSDOWNLOADER_HPP
#define OPDSDOWNLOADER_HPP

#include <string>
#include <curl/curl.h>
#include <vector> 
#include <utility> 

const std::string mainDomain = "https://books.fbreader.org";

class OPDSDownloader{
public:
    OPDSDownloader();
    ~OPDSDownloader();
    std::string download(const std::string &url);
    void googleOAuth();
private:
    CURL *curl;
    std::vector <std::pair<std::string, std::string> > OPDS_tree;
};

#endif  /* OPDSDOWNLOADER_HPP */