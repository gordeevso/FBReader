#ifndef GRIDVIEW
#define GRIDVIEW

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "GridElements.h"
#include "../library/BookshelfModel.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

class ZLImageData;

class GridView : public ZLView {

public:
    GridView(ZLPaintContext &context);

    void setCaption(const std::string &caption);
    void updateView(BookshelfModel::SortType);

    std::vector<GridElement>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarStep(ZLView::Direction direction, int steps);
    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

private:
    void updateBookshelfElements();
    void updateScrollDown();
    void updateScrollUp();

    void drawBookshelfElements();
    void drawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;

private:








private:
    Point myStartPoint;
    Point myEndPoint;

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

    std::vector<GridElement> myVecBookshelfElements;
    std::vector<GridElement>::iterator myItSelectedElement;
    std::vector<GridElement>::iterator myItFirstRendering;
    std::vector<GridElement>::iterator myItLastRendering;

    ElementMenu myElementMenu;

};


#endif // ZLAPPVIEW

