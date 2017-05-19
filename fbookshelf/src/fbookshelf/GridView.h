#ifndef GRIDVIEW
#define GRIDVIEW

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "GridElements.h"
#include "BookshelfMenu.h"
#include "GridContextMenu.h"

#include "../library/BookshelfModel.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

const int ELEMENTS_ON_X = 3;
const int ELEMENTS_ON_Y = 3;

const ZLColor ELEMENT_COLOR = ZLColor(190,190,190);
const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BACKGROUND_COLOR = ZLColor(255,255,255);

const std::string CAPTION = "Grid";
const std::string ALL_SHELVES = "All shelves";

long const DELAY_ON_CLICK_BOOK_MS = 400;

class ZLImageData;
class ZLTime;

class GridView : public ZLView {

public:
    GridView(ZLPaintContext &context);
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

    std::vector<GridElement>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarStep(Direction direction, int steps);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

public:
    static const ZLTypeId TYPE_ID;

protected:
    void updateBookshelfElements();
    void updateScrollDown();
    void updateScrollUp();

    void drawBookshelfElements();
    void drawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;


protected:
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
    size_t myRenderingElementsCount;

    size_t myScrollBarPos;
    size_t myScrollBarMaxPos;
    size_t myMouseScrollFrom;
    size_t myMouseScrollTo;

    int myElementWidth;
    int myElementHeight;

    std::vector<GridElement> myVecBookshelfElements;
    std::vector<GridElement>::iterator myItSelectedElement;
    std::vector<GridElement>::iterator myItFirstRendering;
    std::vector<GridElement>::iterator myItLastRendering;

    GridContextMenu myContextMenu;
    shared_ptr<BookshelfMenu> myShelfvesMenu;
    std::string myCurrentShelf;
    std::string myLastShelf;

    shared_ptr<ZLTime> myTime;

};


#endif
