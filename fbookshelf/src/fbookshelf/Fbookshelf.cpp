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
#include <ZLOptions.h>

#include "ScrollingAction.h"
#include "Fbookshelf.h"
#include "BookshelfView.h"

#include "BookshelfActions.h"

#include "../formats/FormatPlugin.h"
#include "../library/Book.h"
#include "../library/BookshelfModel.h"
#include "../database/booksdb/BooksDBUtil.h"
#include "../database/booksdb/BooksDB.h"


Fbookshelf &Fbookshelf::Instance() {
    return (Fbookshelf&)ZLApplication::Instance();
}

Fbookshelf::Fbookshelf(const std::string &bookToOpen) : ZLApplication("Fbookshelf"),
                                                        EnableTapScrollingOption(ZLCategoryKey::CONFIG, "TapScrolling", "Enabled", true),
//                                                        mBookToOpen(bookToOpen),
                                                        myBindings0(new ZLKeyBindings("Keys")),
                                                        myBindings90(new ZLKeyBindings("Keys90")),
                                                        myBindings180(new ZLKeyBindings("Keys180")),
                                                        myBindings270(new ZLKeyBindings("Keys270"))
{


    addAction(SIMPLE_DIALOG, new OpenSimpleDialogAction());
    addAction("mouseScrollForward", new MouseWheelScrollingAction(true));
    addAction("mouseScrollBackward", new MouseWheelScrollingAction(false));


  //  std::cout << myBindings0->getBinding(MouseScrollUpKey) << "\n";
  //  std::cout << myBindings0->getBinding(MouseScrollDownKey) << "\n";

  //  myBindings0->bindKey(MouseScrollUpKey, "mouseScrollForward");
  //  myBindings0->bindKey(MouseScrollDownKey, "mouseScrollBackward");

  //  std::cout << myBindings0->getBinding(MouseScrollUpKey) << "\n";
  //  std::cout << myBindings0->getBinding(MouseScrollDownKey) << "\n";


    mBookshelfView = new BookshelfView(*context());
    setView(mBookshelfView);



  //  std::cout << "path from main = " << mBookToOpen << "\n";

}

Fbookshelf::~Fbookshelf() {

}

BookshelfView &Fbookshelf::getBookshelfView()
{
    return dynamic_cast<BookshelfView&>(*mBookshelfView);
}

//bool Fbookshelf::addBook(const std::string &pathToBook)
//{
//
//    shared_ptr<Book> book;
//    if (!mBookToOpen.empty()) {
//        createBook(ZLFile(mBookToOpen), book);
//
//        if (book.isNull()) {
//            const BookList &books = Library::Instance().recentBooks();
//            if (!books.empty()) {
//                book = books[0];
//            }
//        }
//
//        if (book.isNull()) {
//            book = BooksDBUtil::getBook(helpFileName(ZLibrary::Language()));
//        }
//        if (book.isNull()) {
//            book = BooksDBUtil::getBook(helpFileName("en"));
//        }
//
//        assert(book != NULL);
//     //   std::cout << "title =" << book->title() << "\n";
//
//
//        return true;
//    }
//    return false;
//}

void Fbookshelf::initWindow() {
    ZLApplication::initWindow();
    trackStylus(true);

//    grabAllKeys(true);

    std::set<std::string> bookFileNames;
    BookshelfModel::Instance().collectBookFileNames("~/FBooks", false, bookFileNames);

    for(std::set<std::string>::iterator it = bookFileNames.begin(); it != bookFileNames.end(); ++it)
    {
        BooksDBUtil::getBook(*it);
    }
    BooksDBUtil::getBooks(BookshelfModel::Instance().getLibrary());
    BookshelfModel::Instance().loadLibrarySortedByBookId();

//    if(!mBookToOpen.empty())
//    {
//        if(!addBook(mBookToOpen))
//            std::cout << "couldn't open " << mBookToOpen << "\n";
//    }
//    else
//        std::cout << "empty path " << mBookToOpen << "\n";


//    BooksDBUtil::getBooks(mLibrary);

/*
    TagList tl1 = (*mLibrary.begin()).second->tags();

    for(TagList::iterator it = tl1.begin(); it != tl1.end(); ++it)
    {
        std::cout << (*it)->name() << "\n";
    }
*/


    /*
    for(std::map<std::string, shared_ptr<Book> >::const_iterator it = mLibrary.begin(); it != mLibrary.end(); ++it)
    {
        std::cout << (*(*it).second->tags().begin())->name() << "\n";
    }
*/


    getBookshelfView().init();

    refreshWindow();
/*
    std::cout << "do action ->\n";
    doActionByKey(MouseScrollUpKey);
    std::cout << "did action <-\n";
*/
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



