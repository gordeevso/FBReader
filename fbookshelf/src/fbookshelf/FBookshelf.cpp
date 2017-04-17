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
#include "FBookshelf.h"
#include "GridView.h"

#include "BookshelfActions.h"

#include "../formats/FormatPlugin.h"
#include "../library/Book.h"
#include "../library/BookshelfModel.h"
#include "../database/booksdb/BooksDBUtil.h"
#include "../database/booksdb/BooksDB.h"


Fbookshelf &Fbookshelf::Instance() {
    return (Fbookshelf&)ZLApplication::Instance();
}

Fbookshelf::Fbookshelf(const std::string &bookToOpen) : ZLApplication("FBookshelf"),
                                                        EnableTapScrollingOption(ZLCategoryKey::CONFIG, "TapScrolling", "Enabled", true),
                                                        myBindings0(new ZLKeyBindings("Keys")),
                                                        myBindings90(new ZLKeyBindings("Keys90")),
                                                        myBindings180(new ZLKeyBindings("Keys180")),
                                                        myBindings270(new ZLKeyBindings("Keys270"))
{
    setMode(GRID_MODE);

    mBookshelfView = new GridView(*context());
    setView(mBookshelfView);

    addAction(BookshelfActionCode::ADD_TAG, new OpenSimpleDialogAction());
    shared_ptr<Action> booksOrderAction = new SortBooksAction();
    addAction(BookshelfActionCode::SORT_BY_AUTHOR, booksOrderAction);
    addAction(BookshelfActionCode::SORT_BY_ID, booksOrderAction);
    addAction(BookshelfActionCode::SORT_BY_TITLE, booksOrderAction);

  //  std::cout << myBindings0->getBinding(MouseScrollUpKey) << "\n";
  //  std::cout << myBindings0->getBinding(MouseScrollDownKey) << "\n";

  //  myBindings0->bindKey(MouseScrollUpKey, "mouseScrollForward");
  //  myBindings0->bindKey(MouseScrollDownKey, "mouseScrollBackward");

  //  std::cout << myBindings0->getBinding(MouseScrollUpKey) << "\n";
  //  std::cout << myBindings0->getBinding(MouseScrollDownKey) << "\n";

}

Fbookshelf::~Fbookshelf() {
}

void Fbookshelf::setMode(Fbookshelf::ViewMode mode) {
    myViewMode = mode;
}

Fbookshelf::ViewMode Fbookshelf::mode() const {
    return myViewMode;
}

GridView &Fbookshelf::getGridView() {
    return dynamic_cast<GridView&>(*mBookshelfView);
}


void Fbookshelf::initWindow() {
    ZLApplication::initWindow();
    trackStylus(true);

//  grabAllKeys(true);

    std::set<std::string> bookFileNames;
    BookshelfModel::Instance().collectBookFileNames("~/FBooks", false, bookFileNames);

    for(std::set<std::string>::iterator it = bookFileNames.begin(); it != bookFileNames.end(); ++it) {
        BooksDBUtil::getBook(*it);
    }

    BooksDBUtil::getBooks(BookshelfModel::Instance().getLibrary());

    getGridView().updateView(BookshelfModel::SORT_BY_AUTHOR);

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



