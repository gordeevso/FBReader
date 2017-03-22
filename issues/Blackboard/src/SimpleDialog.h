#ifndef SIMPLEDIALOG
#define SIMPLEDIALOG

#include <string>
#include <shared_ptr.h>

class ZLDialog;


class SimpleDialog {

public:
    static bool run();

private:
    SimpleDialog();

    ZLDialog & dialog();

public:




private:


    shared_ptr<ZLDialog> myDialog;

};

#endif // SIMPLEDIALOG

