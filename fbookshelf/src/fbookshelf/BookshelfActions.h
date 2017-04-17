#ifndef BOOKSHELFACTIONS
#define BOOKSHELFACTIONS

#include <ZLApplication.h>

#include <string>
#include <shared_ptr.h>

#include "TagDialogs.h"


class BookshelfActionCode {

public:
    static const std::string SORT_BY_AUTHOR;
    static const std::string SORT_BY_ID;
    static const std::string SORT_BY_TITLE;
    static const std::string ADD_TAG;

private:
    BookshelfActionCode();
};


class ModeDependentAction : public ZLApplication::Action {

protected:
    ModeDependentAction(int visibleInModes);

public:
    bool isVisible() const;

private:
    int myVisibleInModes;
};


class SortBooksAction : public ModeDependentAction {

public:
    SortBooksAction();
    void run();
};


class OpenSimpleDialogAction : public ZLApplication::Action {

public:
    void run();
};



#endif // BOOKSHELFACTIONS

