#include <iostream>

#include "BookshelfActions.h"
#include "FBookshelf.h"

#include "../library/BookshelfModel.h"

const std::string BookshelfActionCode::SORT_BY_AUTHOR = "sortByAuthor";
const std::string BookshelfActionCode::SORT_BY_ID = "sortById";
const std::string BookshelfActionCode::SORT_BY_TITLE = "sortByTitle";
const std::string BookshelfActionCode::ADD_TAG = "add tag";
const std::string BookshelfActionCode::REMOVE_TAG = "remove tag";
const std::string BookshelfActionCode::MOUSE_SCROLL_FORWARD = "mouseScrollForward";
const std::string BookshelfActionCode::MOUSE_SCROLL_BACKWARD = "mouseScrollBackward";

ModeDependentAction::ModeDependentAction(int visibleInModes) : myVisibleInModes(visibleInModes) {
}

bool ModeDependentAction::isVisible() const {
    return (Fbookshelf::Instance().mode() & myVisibleInModes) != 0;
}

void AddTagDialogAction::run()
{
    AddTagDialog::run();
}

void RemoveTagDialogAction::run()
{
    RemoveTagDialog::run();
}


SortBooksAction::SortBooksAction() : ModeDependentAction(Fbookshelf::BOOKSHELF_MODE | Fbookshelf::GRID_MODE) {
}

void SortBooksAction::run() {
    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_ID) {
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_ID);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_AUTHOR) {
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_AUTHOR);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_TITLE) {
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_TITLE);
    }

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
    (dynamic_cast<GridView&>(*view)).onMouseScroll(myForward);
    fbookshelf.refreshWindow();

}



MouseWheelScrollingAction::MouseWheelScrollingAction(bool forward) : ScrollingAction(forward) {
}

size_t MouseWheelScrollingAction::textOptionValue() const {
    return 1;
}

