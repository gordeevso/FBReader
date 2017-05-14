#include <string>
#include "../third-party/mongoose.h"

class AuthorisationManager
{
public:
	
	
	static AuthorisationManager& getInstance();

	void authorise();

	std::string refreshAuthorisationToken();
	std::string getAuthorisationToken() const;

	/* return true if code is returned */
	bool ParseResponce(struct http_message * message, std::string& result);

private:
	std::string authCode;
	std::string authToken;
	std::string refreshToken;

	AuthorisationManager() {}
	~AuthorisationManager() {}

	AuthorisationManager(const AuthorisationManager& other) = delete;
	AuthorisationManager& operator= (const AuthorisationManager& other) = delete;


	void obtainAuthorisationCode();
	void obtainTokens();
};