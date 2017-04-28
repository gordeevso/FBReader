#ifndef TAGDIALOGS
#define TAGDIALOGS


#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>

class ZLDialog;
class ZLOptionsDialog;

class AddTagDialog {

public:
    static bool run();

private:
    AddTagDialog();

};

class RemoveTagDialog {

public:
    static bool run();

private:
    RemoveTagDialog();

};




#endif // TAGDIALOGS

