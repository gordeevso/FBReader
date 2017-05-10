#ifndef BOOKSHELFACTIONS
#define BOOKSHELFACTIONS

#include <ZLApplication.h>

#include <string>
#include <shared_ptr.h>

#include "BookshelfDialogs.h"


class BookshelfActionCode {

public:
    static const std::string SORT_BY_AUTHOR;
    static const std::string SORT_BY_ID;
    static const std::string SORT_BY_TITLE;
    static const std::string ADD_TO_SHELF;
    static const std::string REMOVE_FROM_SHELF;
    static const std::string MOUSE_SCROLL_FORWARD;
    static const std::string MOUSE_SCROLL_BACKWARD;
    static const std::string SHOW_TAG_MENU;
    static const std::string SET_BOOKSTACKVIEW;
    static const std::string SET_GRIDVIEW;
    static const std::string SET_WEBVIEW;
    static const std::string RUN_FBREADER;
    static const std::string RESIZE_SMALLER;
    static const std::string RESIZE_BIGGER;
    static const std::string SIGNIN_RELOAD_GOOGLE_DRIVE;
    static const std::string SIGNOUT_GOOGLE_DRIVE;
    static const std::string SIGNIN_RELOAD_BOOKS_FBREADER_ORG;
    static const std::string SIGNOUT_BOOKS_FBREADER_ORG;
    static const std::string DOWNLOAD_BOOK;

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



class SetGridViewAction : public ModeDependentAction {

public:
    SetGridViewAction();
    void run();
};

class SetBookStackViewAction : public ModeDependentAction {

public:
    SetBookStackViewAction();
    void run();
};

class SetWebViewAction : public ModeDependentAction {

public:
    SetWebViewAction();
    void run();
};



class SignInReloadGoogleDrive : public ModeDependentAction {

public:
    SignInReloadGoogleDrive();
    void run();
};

class SignOutGoogleDrive : public ModeDependentAction {

public:
    SignOutGoogleDrive();
    void run();
};


class SignInReloadBooksFbreaderOrg : public ModeDependentAction {

public:
    SignInReloadBooksFbreaderOrg();
    void run();
};

class SignOutBooksFbreaderOrg : public ModeDependentAction {

public:
    SignOutBooksFbreaderOrg();
    void run();
};


class SortBooksAction : public ModeDependentAction {

public:
    SortBooksAction();
    void run();
};

class ShowTagMenuAction : public ModeDependentAction {

public:
    ShowTagMenuAction();
    void run();
};


class AddToShelfDialogAction : public ZLApplication::Action {

public:
    void run();
};

class RemoveFromShelfDialogAction : public ZLApplication::Action {

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

class RunFBReaderAction : public ZLApplication::Action {

public:
    void run();
};

class DownloadBookAction : public ZLApplication::Action {

public:
    void run();
};

class ResizeSmallerAction : public ModeDependentAction {

public:
    ResizeSmallerAction();
    void run();
};

class ResizeBiggerAction : public ModeDependentAction {

public:
    ResizeBiggerAction();
    void run();
};

#endif // BOOKSHELFACTIONS

