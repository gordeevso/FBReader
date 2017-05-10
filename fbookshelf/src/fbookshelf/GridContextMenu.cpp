#include "GridElements.h"
#include "GridContextMenu.h"
#include "BookshelfActions.h"

#include <cassert>
#include <iostream>

#include <ZLPaintContext.h>

#include "../library/Tag.h"
#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

GridContextMenu::GridContextMenu(ZLPaintContext &context, Point topleft, int xoffset, int yoffset, int fontsize)
    : myRefPainter(context),
      myTopLeft(topleft),
      myXOffset(xoffset),
      myYOffset(yoffset),
      myFontSize(fontsize),
      myIsVisible(false),
      myIsSelected(false),
      myVecMenuStrings()
{
    myIsVisible = false;
    myVecMenuStrings.push_back(std::make_pair(BookshelfActionCode::ADD_TO_SHELF, false));
    myVecMenuStrings.push_back(std::make_pair(BookshelfActionCode::REMOVE_FROM_SHELF, false));
    myItMenuEnd = myVecMenuStrings.end();
}

void GridContextMenu::draw()
{
    myItMenu = myVecMenuStrings.begin();

    myCurTopLeft = myTopLeft;
    int cur_xr = myCurTopLeft.x + myXOffset;
    int cur_yr = myCurTopLeft.y + myYOffset;

    myCurTopLeft.x += 2;
    myCurTopLeft.y += 2;

    for(; myItMenu != myItMenuEnd; ++myItMenu)
    {
        if(myIsSelected && (*myItMenu).second)
            myRefPainter.setFillColor(ZLColor(210,210,90));
        else
            myRefPainter.setFillColor(ZLColor(160,160,160));

        myRefPainter.fillRectangle(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);
        myRefPainter.setColor(FBOptions::Instance().RegularTextColorOption.value());

        myRefPainter.drawString(myCurTopLeft.x,
                                cur_yr,
                                (*myItMenu).first.c_str(),
                                (*myItMenu).first.size(),
                                true);

        myRefPainter.setColor(ZLColor(0,0,0));
        myRefPainter.drawLine(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1,myCurTopLeft.y-2);
        myRefPainter.drawLine(cur_xr+1, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);
        myRefPainter.drawLine(cur_xr+1, cur_yr+2, myCurTopLeft.x-2, cur_yr+2);
        myRefPainter.drawLine(myCurTopLeft.x-2, cur_yr+2, myCurTopLeft.x-2, myCurTopLeft.y-2);

        myCurTopLeft.y += myYOffset + 2;
        cur_yr += myYOffset + 2;
    }
}

bool GridContextMenu::checkSelectedElementMenu(int x, int y, bool & changed_state) {
    bool res = false;
    if((x > myTopLeft.x && x < myTopLeft.x + myXOffset)) {
        myItMenu = myVecMenuStrings.begin();
        myCurTopLeft = myTopLeft;
        myCurTopLeft.y += 2;

        for(; myItMenu != myItMenuEnd; ++myItMenu) {
            bool prev = (*myItMenu).second;

            if(y > myCurTopLeft.y && y < myCurTopLeft.y + myYOffset + 2) {
                myItSelectedActionCode = myItMenu;
                (*myItMenu).second = true;
                res = true;
            }
            else
                (*myItMenu).second = false;

            myCurTopLeft.y += myYOffset + 2;

            if(prev != (*myItMenu).second)
                changed_state = true;
        }
    }
    return res;

}
