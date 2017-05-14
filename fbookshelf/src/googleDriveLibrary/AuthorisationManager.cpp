#include "AuthorisationManager.h"

#include <curl/curl.h>
#include <string>
#include <iostream>

#include "BrowserLauncher.h"
#include "../third-party/json.hpp"
#include "../third-party/mongoose.h"

using json = nlohmann::json;

void save_to_string(void * curl, const char * url, std::string& result, bool needsAuth = false);


static const std::string clientId = "1060324431339-tois5j0useksl71234s39sl16ga6dc2f.apps.googleusercontent.com";
static const std::string clientSecret = "BLu33m7ESxYr5oFReloiSi33";

static const std::string successResponce = "Authorised successfully. Please close the browser and return to your app.";
static const std::string errorResponce = "An error occured during authorisation: ";
static bool flag = false;


static void ev_handler(struct mg_connection *nc, int ev, void * data) {
    struct mbuf *io = &nc->recv_mbuf;
    struct http_message * message = (struct http_message *) data;
    mbuf resp;
    mbuf_init(&resp, 10000);
  
    std::string query;
    switch (ev) 
    {
        case MG_EV_HTTP_REQUEST:
            if(AuthorisationManager::getInstance().ParseResponce(message, query))
            {
                mbuf_append(&resp, successResponce.c_str(), successResponce.size());
            }
            else
            {
                mbuf_append(&resp, errorResponce.c_str(), errorResponce.size());
                mbuf_append(&resp, query.c_str(), query.size());
            }
            mbuf_trim(&resp);

            mg_send_head(nc, 200, message->message.len, "Content-Type: text/plain");
            mg_send(nc, resp.buf, resp.len);
            mbuf_remove(io, io->len);

            flag = true;
            break;

        default:
            break;
    }
}

bool AuthorisationManager::ParseResponce(struct http_message * message, std::string& result)
{

    std::string query(message->query_string.p, message->query_string.len);

    size_t delim = query.find('=');
    if(delim == std::string::npos)
    {
        return false;
    }

    std::string key = query.substr(0, delim);
    result = query.substr(delim+1);

    std::cout << "key: " << key << std::endl;
    std::cout << "result: " << result << std::endl;

    if(key == "code")
    {
        authCode = result;
        return true;
    }
    else
    {
        return false;
    }
}


AuthorisationManager& AuthorisationManager::getInstance()
{
    static AuthorisationManager s;
    return s;
}

void AuthorisationManager::obtainAuthorisationCode()
{
    std::string url = "https://accounts.google.com/o/oauth2/auth?scope=https%3A%2F%2Fwww.googleapis.com%2Fauth%2Fdrive.readonly&redirect_uri=http%3A%2F%2F127.0.0.1%3A8081&response_type=code&client_id=" + clientId;
    launchDefaultBrowser(url);

    std::cout << "trying to open a server" << std::endl;

    const char * port = "8081";

    struct mg_mgr mgr;
    mg_mgr_init(&mgr, NULL);

    auto nc = mg_bind(&mgr, port, ev_handler);
    mg_set_protocol_http_websocket(nc);

    std::cout << "opened server" << std::endl;

    while(!flag)
    {
      mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
}

void AuthorisationManager::obtainTokens()
{
    std::string url = "https://www.googleapis.com/oauth2/v4/token";
    CURL * curl = curl_easy_init();
    std::string opts = "code=" + authCode + "&client_id=" + clientId + "&client_secret=" + clientSecret + "&redirect_uri=http%3A%2F%2F127.0.0.1%3A8081&grant_type=authorization_code";
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); 

    std::string res;
    save_to_string(curl, url.c_str(), res);
    curl_easy_cleanup(curl);

    json resJson = json::parse(res);

    authToken = resJson["access_token"];
    refreshToken = resJson["refresh_token"];
}

void AuthorisationManager::authorise()
{
    obtainAuthorisationCode();
    obtainTokens();
}

std::string AuthorisationManager::getAuthorisationToken() const
{
    return authToken;
}