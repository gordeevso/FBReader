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


    struct ElementMenu
    {
        ElementMenu(ZLPaintContext & context,
                    Point topleft = Point(),
                    int xoffset = 80,
                    int yoffset = 10,
                    int fontsize = 5)
            : myRefPainter(context),
              myTopLeft(topleft),
              myXOffset(xoffset),
              myYOffset(yoffset),
              myFontSize(fontsize),
              myIsVisible(false),
              myVecMenuStrings()
        {
            myVecMenuStrings.push_back(ADD_TAG_ITEM);
            myVecMenuStrings.push_back(REMOVE_TAG_ITEM);
            myItMenuEnd = myVecMenuStrings.end();
        }

        ~ElementMenu()
        {}


        void updatePosition(int x1, int y1, int x2, int y2)
        {

        }

        void draw()
        {
            myItMenu = myVecMenuStrings.begin();

            int cur_xl = myTopLeft.x;
            int cur_yl = myTopLeft.y;
            int cur_xr = cur_xl + myXOffset;
            int cur_yr = cur_yl + myYOffset;

            cur_xl += 2;
            cur_yl += 2;

            for(; myItMenu != myItMenuEnd; ++myItMenu)
            {
                myRefPainter.setFillColor(ZLColor(34,230,135));
                myRefPainter.fillRectangle(cur_xl-2, cur_yl-2, cur_xr+1, cur_yr+2);

                myRefPainter.setColor(FBOptions::Instance().RegularTextColorOption.value());

                myRefPainter.drawString(cur_xl,
                                        cur_yr,
                                        (*myItMenu).c_str(),
                                        (*myItMenu).size(),
                                        true);

                myRefPainter.setColor(ZLColor(0,0,0));
                myRefPainter.drawLine(cur_xl-2, cur_yl-2, cur_xr+1, cur_yl-2);
                myRefPainter.drawLine(cur_xr+1, cur_yl-2, cur_xr+1, cur_yr+2);
                myRefPainter.drawLine(cur_xr+1, cur_yr+2, cur_xl-2, cur_yr+2);
                myRefPainter.drawLine(cur_xl-2, cur_yr+2, cur_xl-2, cur_yl-2);

                cur_yl += myYOffset + 2;
                cur_yr += myYOffset + 2;
            }
        }

        const std::string ADD_TAG_ITEM = "add tag";
        const std::string REMOVE_TAG_ITEM = "remove tag";

        ZLPaintContext & myRefPainter;
        Point myTopLeft;
        int myXOffset;
        int myYOffset;
        int myFontSize;
        bool myIsVisible;

        std::vector<std::string>::iterator myItMenu;
        std::vector<std::string>::iterator myItMenuEnd;
        std::vector<std::string> myVecMenuStrings;
    };



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

