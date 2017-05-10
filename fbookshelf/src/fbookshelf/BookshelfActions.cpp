#include <iostream>
#include <cstdlib>
#include <fstream>

#include "BookshelfActions.h"
#include "FBookshelf.h"
#include "BookStackView.h"
#include "../OPDSExtractor/OPDSDownloader.h"
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
const std::string BookshelfActionCode::RUN_FBREADER = "runFbreader";
const std::string BookshelfActionCode::RESIZE_SMALLER = "resizeSmaller";
const std::string BookshelfActionCode::RESIZE_BIGGER = "resizeBigger";

ModeDependentAction::ModeDependentAction(int visibleInModes) : myVisibleInModes(visibleInModes) {
}

bool ModeDependentAction::isVisible() const {
    return (Fbookshelf::Instance().mode() & myVisibleInModes) != 0;
}


SetGridViewAction::SetGridViewAction() : ModeDependentAction(Fbookshelf::BOOKSTACK_MODE) {
}

void SetGridViewAction::run()
{
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    fbookshelf.setMode(Fbookshelf::GRID_MODE);

    fbookshelf.refreshWindow();
}

SetBookStackViewAction::SetBookStackViewAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {
}

void SetBookStackViewAction::run()
{
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    fbookshelf.setMode(Fbookshelf::BOOKSTACK_MODE);

    fbookshelf.refreshWindow();
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

void ShowTagMenuAction::run()
{
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


void RunFBReaderAction::run()
{
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
        system(("FBReader " + book->file().physicalFilePath() + "&").c_str());
    }
    else if(fbookshelf.mode() == Fbookshelf::WEB_MODE){
        OPDSDownloader downloader;
        shared_ptr<Book> book = (*(static_cast<WebView&>(*view).getSelectedElement())).myBook;
        std::string book_name = book->title() + "." + book->encoding();
        std::string book_path = downloader.getHomeDir() + "/FBookshelfNet/";
        std::ofstream write_book((book_path + book_name).c_str());

        std::string url = mainDomain + book->file().physicalFilePath();
        std::string content = downloader.download(url);
        write_book << content;
    }
}

ResizeSmallerAction::ResizeSmallerAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {

}

void ResizeSmallerAction::run()
{
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        static_cast<GridView&>(*view).resizeElements(true);
    }
}

ResizeBiggerAction::ResizeBiggerAction() : ModeDependentAction(Fbookshelf::GRID_MODE) {

}

void ResizeBiggerAction::run()
{
    Fbookshelf &fbookshelf = Fbookshelf::Instance();
    shared_ptr<ZLView> view = fbookshelf.currentView();

    if(fbookshelf.mode() == Fbookshelf::GRID_MODE) {
        static_cast<GridView&>(*view).resizeElements(false);
    }
}
