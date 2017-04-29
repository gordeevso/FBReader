#ifndef BOOKSTACKVIEW
#define BOOKSTACKVIEW

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "GridElements.h"
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
    void setMode(ViewMode);
    void invertMode();

    std::vector<BookElement>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarStep(ZLView::Direction direction, int steps);
    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

public:
    static const ZLTypeId TYPE_ID;

private:
    void updateBookStackElements();
    void updateScrollDown();
    void updateScrollUp();

    void drawBookshelfElements();
    void drawBackground();
    
    void updateBookStack();
    void drawBookStack();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;

private:
    ViewMode myViewMode;
    BookshelfModel::SortType mySortType;

    int myTopLeftX;
    int myTopleftY;

    std::string myCaption;
    int myViewWidth;
    int myViewHeight;
    ZLColor myBackgroundColor;

    int myElementsOnX;
    int myElementsOnY;
    int myRenderingElementsCount;

    size_t myScrollBarPos;
    size_t myScrollBarMaxPos;
    size_t myMouseScrollFrom;
    size_t myMouseScrollTo;

    int myElementWidth;
    int myElementHeight;

    std::vector<BookElement> myVecBookshelfElements;
    std::vector<BookElement>::iterator myItSelectedElement;
    std::vector<BookElement>::iterator myItFirstRendering;
    std::vector<BookElement>::iterator myItLastRendering;
    
    std::vector<Shelf> myVecShelf;

    GridContextMenu myElementMenu;
    shared_ptr<BookshelfMenu> myTagsMenu;

};



#endif // BOOKSTACKVIEW

