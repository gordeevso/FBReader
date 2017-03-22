#include <iostream>
#include <algorithm>

#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>

#include "SimpleDialog.h"

SimpleDialog::SimpleDialog()
{
    std::cout << "SimpleDialog created \n";
    myDialog = ZLDialogManager::Instance().createDialog(ZLResourceKey("SimpleDialog"));
//	myNameEntry = new AuthorNameEntry(*this, jt - myAuthors.begin());
//	mySortKeyEntry = new AuthorSortKeyEntry(*this);

//	myDialog->addOption(ZLResourceKey("name"), myNameEntry);
//	myDialog->addOption(ZLResourceKey("sortKey"), mySortKeyEntry);

    myDialog->addButton(ZLDialogManager::OK_BUTTON, true);
    myDialog->addButton(ZLDialogManager::CANCEL_BUTTON, false);
    myDialog->addButton(ZLDialogManager::DIALOG_TITLE, true);

    std::cout << myDialog->resource(ZLResourceKey("2")).name() << "\n";
}


ZLDialog &SimpleDialog::dialog() {
    return *myDialog;
}


bool SimpleDialog::run()
{
    SimpleDialog dlg;
    if (dlg.dialog().run()) {
        std::cout << "simple dialog.run()\n";
        return true;
    }
    return false;
}




