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
    static const std::string MOUSE_SCROLL_FORWARD;
    static const std::string MOUSE_SCROLL_BACKWARD;
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


class ScrollingAction : public ZLApplication::Action {

protected:
    ScrollingAction(bool forward);

public:
    virtual size_t textOptionValue() const = 0;
    virtual int scrollingDelay() const;
    bool isEnabled() const;
    bool useKeyDelay() const;
    void run();

private:
    const bool myForward;
};


class MouseWheelScrollingAction : public ScrollingAction {

public:
    MouseWheelScrollingAction(bool forward);

private:
    size_t textOptionValue() const;
};

#endif // BOOKSHELFACTIONS

