#ifndef BOOKSTACKVIEW
#define BOOKSTACKVIEW

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "BookStackElements.h"
#include "BookshelfMenu.h"
#include "GridContextMenu.h"

#include "../library/BookshelfModel.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

class ZLImageData;

class BookStackView : public ZLView {

public:
    BookStackView(ZLPaintContext &context);
    const ZLTypeId &typeId() const;

    enum ViewMode {
        WITH_TAGS_MENU = 0,
        WITHOUT_TAGS_MENU = 1
    };

    void setCaption(const std::string &caption);
    void updateView(BookshelfModel::SortType);
    void resizeElements(bool smaller);
    void setMode(ViewMode);
    void invertMode();

    std::vector<BookOnShelf>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

public:
    static const ZLTypeId TYPE_ID;

private:
    void updateBookshelfElements();
    void updateScrollDown();
    void updateScrollUp();

    void drawBookshelfElements();
    void drawBookshelfs();
    void drawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;


private:
    ViewMode myViewMode;
    BookshelfModel::SortType mySortType;

//    int myTopLeftX;
//    int myTopleftY;

    std::string myCaption;
    int myViewWidth;
    int myViewHeight;
    ZLColor myBackgroundColor;

    size_t myScrollBarPos;
    size_t myScrollBarMaxPos;
    size_t myMouseScrollFrom;
    size_t myMouseScrollTo;

    std::vector<BookOnShelf> myVecBookshelfElements;
    std::vector<BookOnShelf>::iterator myItSelectedElement;
    std::vector<BookOnShelf>::iterator myItFirstRendering;
    std::vector<BookOnShelf>::iterator myItLastRendering;
    
    std::vector<Shelf> myVecShelfs;

    GridContextMenu myElementMenu;
    shared_ptr<BookshelfMenu> myTagsMenu;

};


#endif