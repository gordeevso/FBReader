#include <ZLResource.h>
#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>


#include "TagDialogs.h"
#include "FBookshelf.h"

#include "../database/booksdb/BooksDB.h"



const std::string ADD_TAG_DIALOG = "Add tag";



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

        shared_ptr<Book> book = (*(Fbookshelf::Instance().getGridView().getSelectedElement())).mBook;
        book->addTag(addTagEntry->initialValue());
        BooksDB::Instance().saveBook(book);

        return true;
    }
    return false;
}

