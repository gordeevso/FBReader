#ifndef TAGDIALOGS
#define TAGDIALOGS


#include <ZLDialogManager.h>
#include <ZLDialog.h>
#include <ZLOptionEntry.h>

class ZLDialog;
class ZLOptionsDialog;

class AddToShelfDialog {

public:
    static bool run();

private:
    AddToShelfDialog();

};

class RemoveFromShelfDialog {

public:
    static bool run();

private:
    RemoveFromShelfDialog();

};




#endif // TAGDIALOGS

