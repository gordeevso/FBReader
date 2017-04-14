#ifndef BOOKSHELFACTIONS
#define BOOKSHELFACTIONS

#include <ZLApplication.h>

#include <string>
#include <shared_ptr.h>

#include "TagDialogs.h"

const std::string SIMPLE_DIALOG = "simpleDialog";

class OpenSimpleDialogAction : public ZLApplication::Action {

public:
    void run();
};



#endif // BOOKSHELFACTIONS

