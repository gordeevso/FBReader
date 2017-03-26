#include <cassert>
#include <queue>
#include <iostream>

#include <ZLibrary.h>
#include <ZLFile.h>
#include <ZLDialogManager.h>
#include <ZLOptionsDialog.h>
#include <ZLDir.h>
#include <ZLStringUtil.h>
#include <ZLResource.h>


#include "Fbookshelf.h"
#include "SimpleBlackView.h"


#include "../formats/FormatPlugin.h"
#include "../library/Book.h"
#include "../database/booksdb/BooksDBUtil.h"
#include "../database/booksdb/BooksDB.h"



Fbookshelf &Fbookshelf::Instance() {
    return (Fbookshelf&)ZLApplication::Instance();
}

Fbookshelf::Fbookshelf(const std::string &bookToOpen) : ZLApplication("Fbookshelf"),
                                                        mBookToOpen(bookToOpen)
{

    mSimpleBlackView = new SimpleBlackView(*context());
    setView(mSimpleBlackView);

    std::cout << "path from main = " << mBookToOpen << "\n";
}

Fbookshelf::~Fbookshelf() {

}

bool Fbookshelf::addBook(const std::string &pathToBook)
{

    shared_ptr<Book> book;
    if (!mBookToOpen.empty()) {
        createBook(ZLFile(mBookToOpen), book);

        if (book.isNull()) {
            const BookList &books = Library::Instance().recentBooks();
            if (!books.empty()) {
                book = books[0];
            }
        }

        if (book.isNull()) {
            book = BooksDBUtil::getBook(helpFileName(ZLibrary::Language()));
        }
        if (book.isNull()) {
            book = BooksDBUtil::getBook(helpFileName("en"));
        }

        assert(book != NULL);
        std::cout << "title =" << book->title() << "\n";

        mLibrary.push_back(book);
        return true;
    }
    return false;
}

const shared_ptr<Book> Fbookshelf::getBook() const
{
    return mLibrary.back();
}

void Fbookshelf::initWindow() {
    ZLApplication::initWindow();
    trackStylus(true);


    if(!mBookToOpen.empty())
    {
        if(!addBook(mBookToOpen))
            std::cout << "couldn't open " << mBookToOpen << "\n";
        else{
            shared_ptr<Book> book = getBook();
            std::cout << "title from vector = " << book->title() << "\n";
        }
    }
    else
        std::cout << "empty path " << mBookToOpen << "\n";

//	openBook(book);


    refreshWindow();
}

std::string Fbookshelf::helpFileName(const std::string &language) const {
    return ZLibrary::ApplicationDirectory() + ZLibrary::FileNameDelimiter + "help" + ZLibrary::FileNameDelimiter + "MiniHelp." + language + ".fb2";
}

void Fbookshelf::refreshWindow() {
    ZLApplication::refreshWindow();
}

bool Fbookshelf::createBook(const ZLFile &bookFile, shared_ptr<Book> &book) {
    shared_ptr<FormatPlugin> plugin =
        PluginCollection::Instance().plugin(bookFile, false);

    if (!plugin.isNull()) {
        std::string error = plugin->tryOpen(bookFile);
        if (!error.empty()) {
            ZLResourceKey boxKey("openBookErrorBox");
            ZLDialogManager::Instance().errorBox(
                boxKey,
                ZLStringUtil::printf(ZLDialogManager::dialogMessage(boxKey), error)
            );
        } else {
            book = BooksDBUtil::getBook(bookFile.path());
            if (!book.isNull()) {
                BooksDB::Instance().insertIntoBookList(*book);
            }
        }
        return true;
    }

    return false;
}



