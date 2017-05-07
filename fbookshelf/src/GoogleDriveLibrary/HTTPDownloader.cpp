/**
 * HTTPDownloader.cpp
 *
 * A simple C++ wrapper for the libcurl easy API.
 *
 * Written by Uli Köhler (techoverflow.net)
 * Published under CC0 1.0 Universal (public domain)
 */
#include "HTTPDownloader.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <iostream>

#include "AuthorisationManager.h"

using namespace std;

namespace details {
    size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
        string data((const char*) ptr, (size_t) size * nmemb);
        *((stringstream*) stream) << data;
        return size * nmemb;
    }
}


HTTPDownloader::HTTPDownloader() {
    
}

HTTPDownloader::~HTTPDownloader() {
    
}

string HTTPDownloader::download(void * curl, const std::string& url, bool needsAuth)
{

    struct curl_slist *slist=NULL; 
    if(needsAuth)
    {
        slist = curl_slist_append(slist, std::string("Authorization: Bearer " + AuthorisationManager::getInstance().getAuthorisationToken()).c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
    
    std::stringstream out;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, details::write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    
    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);
    
    /* Check for errors */
    if (res != CURLE_OK) 
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_slist_free_all(slist);
    return out.str();
}