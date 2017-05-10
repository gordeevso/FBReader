#include <iostream>
#include <cstdlib>
#include <fstream>

#include "BookshelfActions.h"
#include "FBookshelf.h"
#include "BookStackView.h"
#include "../OPDSExtractor/OPDSDownloader.h"
#include "../OPDSExtractor/OPDSSimpleParser.h"
#include "../GoogleDriveLibrary/GoogleDriveLibrary.h"
#include "../library/BookshelfModel.h"


const std::string BookshelfActionCode::SORT_BY_AUTHOR = "sortByAuthor";
const std::string BookshelfActionCode::SORT_BY_ID = "sortById";
const std::string BookshelfActionCode::SORT_BY_TITLE = "sortByTitle";
const std::string BookshelfActionCode::ADD_TO_SHELF = "add to shelf";
const std::string BookshelfActionCode::REMOVE_FROM_SHELF = "remove from shelf";
const std::string BookshelfActionCode::MOUSE_SCROLL_FORWARD = "mouseScrollForward";
const std::string BookshelfActionCode::MOUSE_SCROLL_BACKWARD = "mouseScrollBackward";
const std::string BookshelfActionCode::SHOW_TAG_MENU = "showTagMenu";
const std::string BookshelfActionCode::SET_BOOKSTACKVIEW = "setBookStackView";
const std::string BookshelfActionCode::SET_GRIDVIEW = "setGridView";
const std::string BookshelfActionCode::SET_WEBVIEW = "setWebView";
const std::string BookshelfActionCode::RUN_FBREADER = "runFbreader";
const std::string BookshelfActionCode::RESIZE_SMALLER = "resizeSmaller";
const std::string BookshelfActionCode::RESIZE_BIGGER = "resizeBigger";
const std::string BookshelfActionCode::SIGNIN_RELOAD_GOOGLE_DRIVE = "signInReloadGoogleDrive";
const std::string BookshelfActionCode::SIGNOUT_GOOGLE_DRIVE = "signOutGoogleDrive";
const std::string BookshelfActionCode::SIGNIN_RELOAD_BOOKS_FBREADER_ORG = "signInReloadBooksFbreaderOrg";
const std::string BookshelfActionCode::SIGNOUT_BOOKS_FBREADER_ORG = "signOutBooksFbreaderOrg";
const std::string BookshelfActionCode::DOWNLOAD_BOOK = "downloadBook";

ModeDependentAction::ModeDependentAction(int visibleInModes) : myVisibleInModes(visibleInModes) {
}

bool ModeDependentAction::isVisible() const {
    return (Fbookshelf::Instance().mode() & myVisibleInModes) != 0;
}


SetGridViewAction::SetGridViewAction() : ModeDependentAction(Fbookshelf::BOOKSTACK_MODE | Fbookshelf::WEB_MODE) {
}

void SetGridViewAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    fbookshelf.setMode(Fbookshelf::GRID_MODE);

    fbookshelf.refreshWindow();
}

SetBookStackViewAction::SetBookStackViewAction() : ModeDependentAction(Fbookshelf::GRID_MODE | Fbookshelf::WEB_MODE) {
}

void SetBookStackViewAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    fbookshelf.setMode(Fbookshelf::BOOKSTACK_MODE);

    fbookshelf.refreshWindow();
}


SetWebViewAction::SetWebViewAction() : ModeDependentAction(Fbookshelf::BOOKSTACK_MODE | Fbookshelf::GRID_MODE) {
}

void SetWebViewAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    fbookshelf.setMode(Fbookshelf::WEB_MODE);

    fbookshelf.refreshWindow();
}


SignInReloadGoogleDrive::SignInReloadGoogleDrive() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignInReloadGoogleDrive::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE) {

        std::string url = "https://books.fbreader.org/opds/by_title";
        OPDSDownloader downloader;
        std::string content = downloader.download(url);
        OPDSSimpleParser parser(content);
        parser.parse();
        std::string baseString = "";
        std::map<std::string, shared_ptr<Book> > &booksmap = BookshelfModel::Instance().getLibrary();
        for (size_t i = 0; i < parser.OPDS_Title_nodes.size(); ++i){
            std::string title =  parser.OPDS_Title_nodes[i];
            // std::cout << title << " " << parser.OPDS_tree_href[i].size() << std::endl;
            std::string path = parser.OPDS_tree_href[i][2].second;
            std::string type = parser.get_book_type(i, 2);
            shared_ptr<Book> bookptr = Book::createBook(
                ZLFile(path), i,
                type,
                "English",
                title
            );
            bookptr->addAuthor("author");
            baseString += "a";
            booksmap.insert(std::make_pair(path,bookptr));
        }


        //load data in netLibrary


        static_cast<WebView&>(*view).setMode(WebView::GOOGLE_DRIVE);
    }
}

SignOutGoogleDrive::SignOutGoogleDrive() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignOutGoogleDrive::run() {

}


SignInReloadBooksFbreaderOrg::SignInReloadBooksFbreaderOrg() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignInReloadBooksFbreaderOrg::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE) {

        GoogleDriveLibrary lib;
        lib.login();

        //load data in netLibrary

        static_cast<WebView&>(*view).setMode(WebView::BOOKS_FBREADER_ORG);
    }
}

SignOutBooksFbreaderOrg::SignOutBooksFbreaderOrg() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignOutBooksFbreaderOrg::run() {

}



void AddToShelfDialogAction::run()
{
    AddToShelfDialog::run();
}

void RemoveFromShelfDialogAction::run()
{
    RemoveFromShelfDialog::run();
}


SortBooksAction::SortBooksAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {
}

void SortBooksAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_ID) {
        shared_ptr<ZLView> view = fbookshelf.currentView();

        if(fbookshelf.mode() == Fbookshelf::GRID_MODE)
            static_cast<GridView&>(*view).updateView(BookshelfModel::SORT_BY_ID);

        if(fbookshelf.mode() == Fbookshelf::BOOKSTACK_MODE)
            static_cast<BookStackView&>(*view).updateView(BookshelfModel::SORT_BY_ID);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_AUTHOR) {
        shared_ptr<ZLView> view = fbookshelf.currentView();

        if(fbookshelf.mode() == Fbookshelf::GRID_MODE)
            static_cast<GridView&>(*view).updateView(BookshelfModel::SORT_BY_AUTHOR);

        if(fbookshelf.mode() == Fbookshelf::BOOKSTACK_MODE)
            static_cast<BookStackView&>(*view).updateView(BookshelfModel::SORT_BY_AUTHOR);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_TITLE) {
        shared_ptr<ZLView> view = fbookshelf.currentView();

        if(fbookshelf.mode() == Fbookshelf::GRID_MODE)
            static_cast<GridView&>(*view).updateView(BookshelfModel::SORT_BY_TITLE);

        if(fbookshelf.mode() == Fbookshelf::BOOKSTACK_MODE)
            static_cast<BookStackView&>(*view).updateView(BookshelfModel::SORT_BY_TITLE);
    }

}

ShowTagMenuAction::ShowTagMenuAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {

}

void ShowTagMenuAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();
    if(view->isInstanceOf(GridView::TYPE_ID))
        static_cast<GridView&>(*view).invertMode();
}


ScrollingAction::ScrollingAction(bool forward) : myForward(forward) {
}

int ScrollingAction::scrollingDelay() const {
    return 0;
}

bool ScrollingAction::isEnabled() const {
    return true;
}

bool ScrollingAction::useKeyDelay() const {
    return false;
}

void ScrollingAction::run() {

    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();
    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        (static_cast<GridView&>(*view)).onMouseScroll(myForward);
    }

    if(fbookshelf.mode() == Fbookshelf::BOOKSTACK_MODE) {
        (static_cast<BookStackView&>(*view)).onMouseScroll(myForward);
    }
    fbookshelf.refreshWindow();

}



MouseWheelScrollingAction::MouseWheelScrollingAction(bool forward) : ScrollingAction(forward) {
}

size_t MouseWheelScrollingAction::textOptionValue() const {
    return 1;
}


void RunFBReaderAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
        system(("FBReader " + book->file().physicalFilePath() + "&").c_str());
    }
}

void DownloadBookAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE){
        if(static_cast<WebView&>(*view).mode() == WebView::BOOKS_FBREADER_ORG) {
            OPDSDownloader downloader;
            shared_ptr<Book> book = (*(static_cast<WebView&>(*view).getSelectedElement())).myBook;
            std::string book_name = book->title() + "." + book->encoding();
            std::string book_path = downloader.getHomeDir() + "/FBookshelfNet/";
            std::ofstream write_book((book_path + book_name).c_str());

            std::string url = mainDomain + book->file().physicalFilePath();
            std::string content = downloader.download(url);
            write_book << content;
        }
        else if(static_cast<WebView&>(*view).mode() == WebView::GOOGLE_DRIVE) {

            //download book from google drive here
        }
    }
}

ResizeSmallerAction::ResizeSmallerAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {

}

void ResizeSmallerAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        static_cast<GridView&>(*view).resizeElements(true);
    }
}

ResizeBiggerAction::ResizeBiggerAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {

}

void ResizeBiggerAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        static_cast<GridView&>(*view).resizeElements(false);
    }
}






