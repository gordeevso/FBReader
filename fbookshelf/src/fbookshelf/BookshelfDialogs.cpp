#include <ZLResource.h>
#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>


#include "BookshelfDialogs.h"
#include "FBookshelf.h"

#include "../database/booksdb/BooksDB.h"

class BookShelfEntry : public ZLComboOptionEntry {

public:
    BookShelfEntry(bool adding_book);

    const std::string &initialValue() const;
    const std::vector<std::string> &values() const;
    void onAccept(const std::string &value);
    void onValueSelected(int index);


private:
    std::string mySelectedValue;

    std::vector<std::string> myValues;
    bool myAddingBook;
};

BookShelfEntry::BookShelfEntry(bool adding_book) : ZLComboOptionEntry(true),
                                                   mySelectedValue(""),
                                                   myAddingBook(adding_book)
{

    myValues.push_back("");

    Fbookshelf &fbookshelf = Fbookshelf::Instance();

    shared_ptr<ZLView> view = fbookshelf.getGridView();
    shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
    ShelfList const & shelves = myAddingBook ? BookshelfModel::Instance().getShelves() : book->shelves();

    for(size_t i = 0; i != shelves.size(); ++i) {
        myValues.push_back(shelves[i]);
    }

}

const std::string &BookShelfEntry::initialValue() const {
    return mySelectedValue;
}

const std::vector<std::string> &BookShelfEntry::values() const {
    return myValues;
}

void BookShelfEntry::onAccept(const std::string &value) {
    if(myAddingBook)
        mySelectedValue = value;
}


void BookShelfEntry::onValueSelected(int index) {
    mySelectedValue = myValues[index];
}


AddToShelfDialog::AddToShelfDialog()
{}

bool AddToShelfDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("add to shelf"));

    BookShelfEntry* addToShelfEntry = new BookShelfEntry(true);
    dialog->addOption(ZLResourceKey("name"), addToShelfEntry);

    dialog->addButton(ZLDialogManager::OK_BUTTON, true);
    dialog->addButton(ZLDialogManager::CANCEL_BUTTON, false);

    if (dialog->run()) {
        dialog->acceptValues();
        Fbookshelf &fbookshelf = Fbookshelf::Instance();

        shared_ptr<ZLView> view = fbookshelf.getGridView();
        shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
        BookshelfModel::Instance().addBookToShelf(addToShelfEntry->initialValue(), book);

        return true;
    }
    return false;
}


RemoveFromShelfDialog::RemoveFromShelfDialog() {
}


bool RemoveFromShelfDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("remove from shelf"));

    BookShelfEntry * bookShelfEntry = new BookShelfEntry(false);
    dialog->addOption(ZLResourceKey("name"), bookShelfEntry);

    dialog->addButton(ZLDialogManager::OK_BUTTON, true);
    dialog->addButton(ZLDialogManager::CANCEL_BUTTON, false);


    if (dialog->run()) {
        dialog->acceptValues();

        Fbookshelf &fbookshelf = Fbookshelf::Instance();

        shared_ptr<ZLView> view = fbookshelf.getGridView();
        shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
        ShelfList const & shelves = book->shelves();

        for(size_t i = 0; i != shelves.size(); ++i) {
            if(bookShelfEntry->initialValue() == shelves[i]) {
                BookshelfModel::Instance().removeBookFromShelf(shelves[i], book);
                break;
            }
        }

        return true;
    }
    return false;
}

