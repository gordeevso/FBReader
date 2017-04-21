#include <ZLResource.h>
#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>


#include "TagDialogs.h"
#include "FBookshelf.h"

#include "../database/booksdb/BooksDB.h"



class AddTagEntry : public ZLTextOptionEntry {

public:
    AddTagEntry(const std::string &initialValue);

    const std::string &initialValue() const;
    void onAccept(const std::string &value);
    ZLOptionKind kind() const;

private:
    std::string myValue;
};

AddTagEntry::AddTagEntry(const std::string &initialValue) : myValue(initialValue)
{}

const std::string &AddTagEntry::initialValue() const
{
    return myValue;
}

void AddTagEntry::onAccept(const std::string &value)
{
    myValue = value;
}

ZLOptionEntry::ZLOptionKind AddTagEntry::kind() const
{
    return STRING;
}




AddTagDialog::AddTagDialog()
{}

bool AddTagDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("add tag"));

    AddTagEntry * addTagEntry = new AddTagEntry("");
    dialog->addOption(ZLResourceKey("name"), addTagEntry);

    dialog->addButton(ZLDialogManager::OK_BUTTON, true);
    dialog->addButton(ZLDialogManager::CANCEL_BUTTON, false);

    if (dialog->run()) {
        dialog->acceptValues();

        shared_ptr<Book> book = (*(Fbookshelf::Instance().getGridView().getSelectedElement())).myBook;
        book->addTag(addTagEntry->initialValue());
        BooksDB::Instance().saveBook(book);

        return true;
    }
    return false;
}


class BookTagEntry : public ZLComboOptionEntry {

public:
    BookTagEntry();

    const std::string &initialValue() const;
    const std::vector<std::string> &values() const;
    void onAccept(const std::string &value);
    void onValueSelected(int index);


private:
    std::string mySelectedValue;

    std::vector<std::string> myValues;
};

BookTagEntry::BookTagEntry() : ZLComboOptionEntry(true),
                               mySelectedValue("")
{

    myValues.push_back("");

    shared_ptr<Book> book = (*(Fbookshelf::Instance().getGridView().getSelectedElement())).myBook;
    TagList const & tags = book->tags();

    for(size_t i = 0; i != tags.size(); ++i) {
        myValues.push_back(tags[i]->name());
    }

}

const std::string &BookTagEntry::initialValue() const {
    return mySelectedValue;
}

const std::vector<std::string> &BookTagEntry::values() const {
    return myValues;
}

void BookTagEntry::onAccept(const std::string &value) {

}


void BookTagEntry::onValueSelected(int index) {
    mySelectedValue = myValues[index];
}


RemoveTagDialog::RemoveTagDialog() {
}


bool RemoveTagDialog::run()
{
    shared_ptr<ZLDialog> dialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("remove tag"));

    BookTagEntry * bookTagEntry = new BookTagEntry();
    dialog->addOption(ZLResourceKey("name"), bookTagEntry);

    dialog->addButton(ZLDialogManager::OK_BUTTON, true);
    dialog->addButton(ZLDialogManager::CANCEL_BUTTON, false);


    if (dialog->run()) {
        dialog->acceptValues();

        shared_ptr<Book> book = (*(Fbookshelf::Instance().getGridView().getSelectedElement())).myBook;
        TagList const & tags = book->tags();

        for(size_t i = 0; i != tags.size(); ++i) {
            if(bookTagEntry->initialValue() == tags[i]->name()) {
                book->removeTag(tags[i], false);
                BooksDB::Instance().saveBook(book);
                break;
            }
        }

        return true;
    }
    return false;
}

