#include "OPDSDownloader.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>


size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    std::string data((const char*) ptr, (size_t) size * nmemb);
    *((std::stringstream*) stream) << data;
    return size * nmemb;
}

OPDSDownloader::OPDSDownloader() {
    curl = curl_easy_init();
}

OPDSDownloader::~OPDSDownloader() {
    curl_easy_cleanup(curl);
}

void OPDSDownloader::googleOAuth(){
    std::system("cookiebrowser");
}

std::string OPDSDownloader::getHomeDir(){
    int myuid;
    passwd *mypasswd;
    std::string TestFileName;
    myuid = getuid();
    mypasswd = getpwuid(myuid);
    TestFileName= mypasswd->pw_dir;
    return TestFileName;
}

std::string OPDSDownloader::download(const std::string &url) {
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1 );

    bool tried = false;
    char* result_str;
    std::stringstream out;
    do{
        if (tried){
           googleOAuth();
        }
        std::string filepath = getHomeDir() + "/FBookshelfNet/cookie.txt";
        std::ifstream fin(filepath.c_str());
        std::string s;
        std::getline(fin, s);

        curl_easy_setopt(curl, CURLOPT_COOKIE, s.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.");
     
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &result_str);
        //std::cout << result_str;

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        tried = true;
    }while(strcmp(url.c_str(), result_str) != 0);

    return out.str();
}