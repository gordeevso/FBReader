#include <iostream>
#include <cstdlib>
#include <fstream>

#include "BookshelfActions.h"
#include "FBookshelf.h"
#include "BookStackView.h"
//#include "../OPDSExtractor/OPDSDownloader.h"
//#include "../OPDSExtractor/OPDSSimpleParser.h"
#include "../googleDriveLibrary/GoogleDriveLibrary.h"
#include "../library/BookshelfModel.h"
#include "../library/BookshelfNetFBReaderModel.h"
#include "../library/BookshelfNetGoogleModel.h"
#include "../netLib/NetworkFBReaderActions.h"

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
        shared_ptr<NetworkActions> net = new GoogleDriveLibrary();
	

        std::vector<BookModelFill> booksToPass = net->getNetworkLibrary();

        std::map<std::string, shared_ptr<Book> > &booksmap = BookshelfNetGoogleModel::Instance().getLibrary();
        for (size_t i = 0; i < booksToPass.size(); i++){
            booksmap.insert(booksToPass[i]);
        }

        static_cast<WebView&>(*view).setMode(WebView::GOOGLE_DRIVE);
    }
}

SignOutGoogleDrive::SignOutGoogleDrive() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignOutGoogleDrive::run() {
    GoogleDriveLibrary().logOut();
}


SignInReloadBooksFbreaderOrg::SignInReloadBooksFbreaderOrg() : ModeDependentAction(Fbookshelf::WEB_MODE) {

}

void SignInReloadBooksFbreaderOrg::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE) {
        NetworkFBReaderActions net;
        std::vector<BookModelFill> booksToPass = net.getNetworkLibrary();
        std::map<std::string, shared_ptr<Book> > &booksmap = BookshelfNetFBReaderModel::Instance().getLibrary();
        booksmap.clear();
        for (size_t i = 0; i < booksToPass.size(); i++){
            booksmap.insert(booksToPass[i]);
        }
        static_cast<WebView&>(*view).setMode(WebView::BOOKS_FBREADER_ORG);
    }
}

SignOutBooksFbreaderOrg::SignOutBooksFbreaderOrg() : ModeDependentAction(Fbookshelf::WEB_MODE) {
}

void SignOutBooksFbreaderOrg::run() {
    NetworkFBReaderActions net;
    net.logOut();
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

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE) {
        (static_cast<WebView&>(*view)).onMouseScroll(myForward);
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
    } else if (fbookshelf.mode() == Fbookshelf::BOOKSTACK_MODE) {
        shared_ptr<Book> book = (*(static_cast<BookStackView&>(*view).getSelectedElement())).myBook;
        system(("FBReader " + book->file().physicalFilePath() + "&").c_str());
    }
}

void DownloadBookAction::run() {
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::WEB_MODE){
        if(static_cast<WebView&>(*view).mode() == WebView::BOOKS_FBREADER_ORG) {
            shared_ptr<Book> book = (*(static_cast<WebView&>(*view).getSelectedElement())).myBook;
            NetworkFBReaderActions net;
            std::string filePath = net.downloadBook(book);
            system(("FBReader " + filePath + "&").c_str());
        }
        else if(static_cast<WebView&>(*view).mode() == WebView::GOOGLE_DRIVE) {

            shared_ptr<NetworkActions> net = new GoogleDriveLibrary();
            std::string filePath = net->downloadBook((*(static_cast<WebView&>(*view).getSelectedElement())).myBook);
            system(("FBReader " + filePath + "&").c_str());
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






