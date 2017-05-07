#ifndef GOOGLE_DRIVE_LIBRARY
#define GOOGLE_DRIVE_LIBRARY

#include <vector>
#include <shared_ptr.h>
#include "../library/Book.h"

class GoogleDriveLibrary
{
public:
	std::vector<shared_ptr<Book> > getBookList();
	void downloadBook(const std::string& filename);
	void logout();
};

#endif // GOOGLE_DRIVE_LIBRARY