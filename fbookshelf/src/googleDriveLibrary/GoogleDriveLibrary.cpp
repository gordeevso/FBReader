#include "GoogleDriveLibrary.h"
#include <curl/curl.h>
#include "../third-party/json.hpp"
#include "HTTPDownloader.h"
#include "AuthorisationManager.h"
#include "../netLib/OPDSExtractor/OPDSDownloader.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

void save_to_string(void * curl, const char * url, std::string& result, bool needsAuth = false)
{
    result = HTTPDownloader().download(curl, url, needsAuth);
}

void get_page(const char* url, const char* file_name, bool needsAuth = false)
{
    CURL* easyhandle = curl_easy_init();
    struct curl_slist *slist=NULL;

    if(needsAuth)
    {
        // access token is received prior to using this program; to refresh it, use refresh_token from Google
        slist = curl_slist_append(slist, std::string("Authorization: Bearer " + AuthorisationManager::getInstance().getAuthorisationToken()).c_str());
        curl_easy_setopt( easyhandle, CURLOPT_HTTPHEADER, slist);
    }

    curl_easy_setopt( easyhandle, CURLOPT_URL, url ) ;

    FILE* file = fopen( file_name, "w");
    curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file) ;

    curl_easy_perform( easyhandle );

    // cleanup
    if(needsAuth)
    {
        curl_slist_free_all(slist);    
    }
    
    curl_easy_cleanup(easyhandle);
    fclose(file);
}


std::string to_string(const json& j)
{
    std::stringstream ss;
    ss << j;
    return ss.str().substr(1, ss.str().size()-2);
}


GoogleDriveLibrary::GoogleDriveLibrary()
{
   //login(); // TODO: maybe make this public function in NetworkActions and make user do it? 
}

void find_library(const json& filelist, std::string& id)
{
    for(auto it = filelist["items"].begin(); it != filelist["items"].end(); ++it)
    {
        if(to_string((*it)["title"]).find("FBReader") != std::string::npos)
        {
            id = to_string((*it)["id"]);
            return;
        }
    }
}

void GoogleDriveLibrary::logOut()
{

}

void GoogleDriveLibrary::login()
{
    AuthorisationManager::getInstance().authorise();
}

std::vector<BookModelFill> GoogleDriveLibrary::getNetworkLibrary()
{
    
    login();
    std::cout << "authorised successfully" << std::endl;

    std::string filelist;
    void * curl = curl_easy_init();
    save_to_string(curl, "https://www.googleapis.com/drive/v2/files", filelist, true);
    curl_easy_cleanup(curl);
    std::cout << "got filelist" << std::endl;
  
    json filelist_json = json::parse(filelist);
    std::string id;

    find_library(filelist_json, id);
    std::cout << "found library" << std::endl;

    std::vector<BookModelFill> result;
  
    for(auto it = filelist_json["items"].begin(); it != filelist_json["items"].end(); ++it)
    {
        auto parents = (*it)["parents"];
        for(auto jt = parents.begin(); jt != parents.end(); ++jt)
        {
            std::string parent_id = to_string((*jt)["id"]);
            
            if(parent_id == id)
            {
                std::string file_id = to_string((*it)["id"]);
                std::string title = to_string((*it)["title"]);
                //std::cout << "found book: " << title << std::endl;

                std::string home_dir = OPDSDownloader().getHomeDir();
		
                std::string file_output_name =  home_dir + "/" + title;
                std::string thumb_output_name = home_dir + "/" + file_id + "_thumb.jpg";

                std::string file_download_link = to_string((*it)["selfLink"]) + "?alt=media";     

                std::string link = to_string((*it)["thumbnailLink"]);
                if(link != "ul")
                {
                    std::string thumb_download_link = to_string((*it)["thumbnailLink"]) + "?alt=media";
		    std::cout << thumb_download_link << std::endl;

                    get_page(thumb_download_link.c_str(), thumb_output_name.c_str(), true);
                }

                shared_ptr<ZLFileImage> title_image;
                if(link != "ul")
                {
                    ZLFile image_file(thumb_output_name);
                    title_image = new ZLFileImage(image_file, 0);
                }
                else
                {
                    static const std::string no_pic = "/usr/share/pixmaps/FBookshelf/title.png";
                    ZLFile image_file(no_pic);
                    title_image = new ZLFileImage(image_file, 0);
                }

                shared_ptr<Book> bookptr = Book::createNetBook(
                    title_image,
                    file_download_link,
                    title,
                    "fb2.zip"
                );

                bookptr->addAuthor("author");
                result.push_back(std::make_pair(file_download_link, bookptr));
            }
        }
    }


    return result;
}

namespace details
{
std::string deleteSpaces(std::string name){
	std::string newName = "";
	for(int i = 0; i < name.length(); i++){
		if (name[i] != ' ') newName += name[i];
	}
	return newName;
}
}

std::string GoogleDriveLibrary::downloadBook(shared_ptr<Book> book){
    std::string book_name = details::deleteSpaces(ZLFile::replaceIllegalCharacters(book->title(), '_')) + "." + book->extension();
    std::string book_path = OPDSDownloader().getHomeDir() + "/FBookshelfNet/";
    std::string url = book->url();

    get_page(url.c_str(), (book_path + book_name).c_str(), true);
    return (book_path + book_name);
}
