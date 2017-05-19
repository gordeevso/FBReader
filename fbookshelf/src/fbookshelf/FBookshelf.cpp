#include <cassert>
#include <queue>
#include <iostream>
#include <sstream>
#include <string>
#include <ZLibrary.h>
#include <ZLFile.h>
#include <ZLDialogManager.h>
#include <ZLOptionsDialog.h>
#include <ZLDir.h>
#include <ZLStringUtil.h>
#include <ZLResource.h>
#include <ZLOptions.h>

#include "FBookshelf.h"
#include "GridView.h"
#include "BookStackView.h"
#include "WebView.h"

#include "BookshelfActions.h"

#include "../formats/FormatPlugin.h"
#include "../library/Book.h"
#include "../library/BookshelfModel.h"
#include "../database/booksdb/BooksDBUtil.h"
#include "../database/booksdb/BooksDB.h"


Fbookshelf &Fbookshelf::Instance() {
    return (Fbookshelf&)ZLApplication::Instance();
}

Fbookshelf::Fbookshelf(const std::string & bookToOpen) : ZLApplication("FBookshelf"),
                                                        myBindings0(new ZLKeyBindings("Keys"))
{
    myGridView = new GridView(*context());
    myBookStackView = new BookStackView(*context());
    myWebView = new WebView(*context());

    myViewMode = GRID_MODE;
    setView(myGridView);

    addAction(BookshelfActionCode::SET_GRIDVIEW, new SetGridViewAction());
    addAction(BookshelfActionCode::SET_BOOKSTACKVIEW, new SetBookStackViewAction());
    addAction(BookshelfActionCode::SET_WEBVIEW, new SetWebViewAction());

    addAction(BookshelfActionCode::ADD_TO_SHELF, new AddToShelfDialogAction());
    addAction(BookshelfActionCode::REMOVE_FROM_SHELF, new RemoveFromShelfDialogAction());

    shared_ptr<Action> booksOrderAction = new SortBooksAction();
    addAction(BookshelfActionCode::SORT_BY_AUTHOR, booksOrderAction);
//    addAction(BookshelfActionCode::SORT_BY_ID, booksOrderAction);
    addAction(BookshelfActionCode::SORT_BY_TITLE, booksOrderAction);

    addAction(BookshelfActionCode::MOUSE_SCROLL_FORWARD, new MouseWheelScrollingAction(true));
    addAction(BookshelfActionCode::MOUSE_SCROLL_BACKWARD, new MouseWheelScrollingAction(false));

    addAction(BookshelfActionCode::SHOW_TAG_MENU, new ShowTagMenuAction());

    addAction(BookshelfActionCode::RUN_FBREADER, new RunFBReaderAction());

    addAction(BookshelfActionCode::RESIZE_SMALLER, new ResizeSmallerAction());
    addAction(BookshelfActionCode::RESIZE_BIGGER, new ResizeBiggerAction());

    addAction(BookshelfActionCode::SIGNIN_RELOAD_GOOGLE_DRIVE, new SignInReloadGoogleDrive());
    addAction(BookshelfActionCode::SIGNOUT_GOOGLE_DRIVE, new SignOutGoogleDrive());
    addAction(BookshelfActionCode::SIGNIN_RELOAD_BOOKS_FBREADER_ORG, new SignInReloadBooksFbreaderOrg());
    addAction(BookshelfActionCode::SIGNOUT_BOOKS_FBREADER_ORG, new SignOutBooksFbreaderOrg());
    addAction(BookshelfActionCode::DOWNLOAD_BOOK, new DownloadBookAction());
}

Fbookshelf::~Fbookshelf() {
}

void Fbookshelf::setMode(Fbookshelf::ViewMode mode, WebView::ViewMode web_mode) {
    myViewMode = mode;
    switch (mode) {
    case GRID_MODE:
        static_cast<GridView&>(*myGridView).setMode(GridView::WITHOUT_TAGS_MENU);
        setView(myGridView);
        break;
    case BOOKSTACK_MODE:
        static_cast<BookStackView&>(*myBookStackView).setMode(BookStackView::WITHOUT_TAGS_MENU);
        setView(myBookStackView);
        break;
    case WEB_MODE:
        static_cast<WebView&>(*myWebView).setMode(web_mode);
        setView(myWebView);
        break;
    default:
        break;
    }
    refreshWindow();
}

Fbookshelf::ViewMode Fbookshelf::mode() const {
    return myViewMode;
}

shared_ptr<ZLView> Fbookshelf::getGridView()
{
    return myGridView;
}

shared_ptr<ZLView> Fbookshelf::getBookStackView()
{
    return myBookStackView;
}

shared_ptr<ZLView> Fbookshelf::getWebView()
{
    return myWebView;
}

shared_ptr<ZLKeyBindings> Fbookshelf::keyBindings() {
    return myBindings0;
}

void Fbookshelf::initWindow() {
    ZLApplication::initWindow();
    trackStylus(true);

    BooksDBUtil::getBooks(BookshelfModel::Instance().getLibrary());
    BooksMap::iterator it = BookshelfModel::Instance().getLibrary().begin();
    BooksMap::iterator itEnd = BookshelfModel::Instance().getLibrary().end();
    for(; it != itEnd; ++it)
    {
        if((*it).second->title() == "About FBReader") {
            BookshelfModel::Instance().getLibrary().erase(it);
        }
    }

    shared_ptr<ZLView> view = this->currentView();
    if(view->isInstanceOf(GridView::TYPE_ID)) {
        static_cast<GridView&>(*view).setMode(GridView::WITHOUT_TAGS_MENU);
    }

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



