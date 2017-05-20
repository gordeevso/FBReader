#ifndef GOOGLE_DRIVE_LIBRARY
#define GOOGLE_DRIVE_LIBRARY

#include <vector>
#include <shared_ptr.h>
#include "../library/Book.h"
#include "../netLib/NetworkActions.h"

class GoogleDriveLibrary : public NetworkActions
{
public:
	GoogleDriveLibrary();

	// returns the path to which the book was saved
	virtual std::string downloadBook(shared_ptr<Book> book);
	
	// returns such a vector with which can easily fill the BookshelfModel
	virtual std::vector<BookModelFill> getNetworkLibrary();

	virtual void logOut();

	void login();
};

#endif // GOOGLE_DRIVE_LIBRARY