#include "BookshelfActions.h"
#include "FBookshelf.h"

#include "../library/BookshelfModel.h"

const std::string BookshelfActionCode::SORT_BY_AUTHOR = "sortByAuthor";
const std::string BookshelfActionCode::SORT_BY_ID = "sortById";
const std::string BookshelfActionCode::SORT_BY_TITLE = "sortByTitle";
const std::string BookshelfActionCode::ADD_TAG = "addTag";

ModeDependentAction::ModeDependentAction(int visibleInModes) : myVisibleInModes(visibleInModes) {
}

bool ModeDependentAction::isVisible() const {
    return (Fbookshelf::Instance().mode() & myVisibleInModes) != 0;
}

void OpenSimpleDialogAction::run()
{
    AddTagDialog::run();
}

#include <iostream>
SortBooksAction::SortBooksAction() : ModeDependentAction(Fbookshelf::BOOKSHELF_MODE | Fbookshelf::GRID_MODE) {
}

void SortBooksAction::run() {
    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_ID) {
        std::cout << "sortby id\n";
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_ID);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_AUTHOR) {
        std::cout << "sortby author\n";
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_AUTHOR);
    }

    if(ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == BookshelfActionCode::SORT_BY_TITLE) {
        std::cout << "sortby title\n";
        Fbookshelf::Instance().getGridView().updateView(BookshelfModel::SORT_BY_TITLE);
    }

}
