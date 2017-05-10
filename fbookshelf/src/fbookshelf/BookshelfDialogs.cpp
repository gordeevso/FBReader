#include <ZLResource.h>
#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>


#include "BookshelfDialogs.h"
#include "FBookshelf.h"

#include "../database/booksdb/BooksDB.h"



class AddToShelfEntry : public ZLTextOptionEntry {

public:
    AddToShelfEntry(const std::string &initialValue);

    const std::string &initialValue() const;
    void onAccept(const std::string &value);
    ZLOptionKind kind() const;

private:
    std::string myValue;
};

AddToShelfEntry::AddToShelfEntry(const std::string &initialValue) : myValue(initialValue)
{}

const std::string &AddToShelfEntry::initialValue() const
{
    return myValue;
}

void AddToShelfEntry::onAccept(const std::string &value)
{
    myValue = value;
}

ZLOptionEntry::ZLOptionKind AddToShelfEntry::kind() const
{
    return STRING;
}




AddToShelfDialog::AddToShelfDialog()
{}

bool AddToShelfDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("add to shelf"));

    AddToShelfEntry * addToShelfEntry = new AddToShelfEntry("");
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


class BookShelfEntry : public ZLComboOptionEntry {

public:
    BookShelfEntry();

    const std::string &initialValue() const;
    const std::vector<std::string> &values() const;
    void onAccept(const std::string &value);
    void onValueSelected(int index);


private:
    std::string mySelectedValue;

    std::vector<std::string> myValues;
};

BookShelfEntry::BookShelfEntry() : ZLComboOptionEntry(true),
                               mySelectedValue("")
{

    myValues.push_back("");

    Fbookshelf &fbookshelf = Fbookshelf::Instance();

    shared_ptr<ZLView> view = fbookshelf.getGridView();
    shared_ptr<Book> book = (*(static_cast<GridView&>(*view).getSelectedElement())).myBook;
    ShelfList const & shelves = book->shelves();

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

}


void BookShelfEntry::onValueSelected(int index) {
    mySelectedValue = myValues[index];
}


RemoveFromShelfDialog::RemoveFromShelfDialog() {
}


bool RemoveFromShelfDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("remove from shelf"));

    BookShelfEntry * bookShelfEntry = new BookShelfEntry();
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

