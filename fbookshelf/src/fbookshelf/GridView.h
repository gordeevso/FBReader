#ifndef GRIDVIEW
#define GRIDVIEW

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
/*

    struct ElementMenu
    {
        ElementMenu(ZLPaintContext & context,
                    Point topleft = Point(),
                    Point bottomright = Point(),
                    int fontsize = 5)
            : painter(context),
              mTopLeft(topleft),
              mBottomRight(bottomright),
              mFontSize(fontsize),
              mMenuStrings()
        {}

        ~ElementMenu()
        {}

        void UpdatePosition(int x1, int y1, int x2, int y2)
        {

        }

        void Draw()
        {
            mItMenu = mMenuStrings.begin();
            mItMenuEnd = mMenuStrings.end();
            for(; mItMenu != mItMenuEnd; ++mItMenu)
            {
                painter.setFillColor(ZLColor(255,255,255));
                painter.fillRectangle(mx-1, my-2, mxr+1, myr+1);

                painter.setColor(FBOptions::Instance().RegularTextColorOption.value());

                painter.drawString(mx,
                                   myr,
                                   (*mItMenu).c_str(),
                                   (*mItMenu).size(),
                                   true);

                painter.setColor(ZLColor(0,0,0));
                painter.drawLine(mx-1, my-2, mxr+1, my-2);
                painter.drawLine(mxr+1, my-2, mxr+1, myr+1);
                painter.drawLine(mxr+1, myr+1, mx-1, myr+1);
                painter.drawLine(mx-1, myr+1, mx-1, my-2);
            }
        }

        ZLPaintContext & painter;
        Point mTopLeft;
        Point mBottomRight;
        int mFontSize;

        std::vector<std::string>::iterator mItMenu;
        std::vector<std::string>::iterator mItMenuEnd;
        std::vector<std::string> mMenuStrings;
    };
*/
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

    std::vector<GridElement>::iterator myItSelectedElement;

    std::vector<GridElement> myBookshelfElements;
    std::vector<GridElement>::iterator myItFirstRendering;
    std::vector<GridElement>::iterator myItLastRendering;

};


#endif // ZLAPPVIEW

