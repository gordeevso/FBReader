#include "MenuElements.h"
#include "BookshelfActions.h"

#include "../library/Tag.h"

ElementMenu::ElementMenu(ZLPaintContext &context, Point topleft, int xoffset, int yoffset, int fontsize)
    : myRefPainter(context),
      myTopLeft(topleft),
      myXOffset(xoffset),
      myYOffset(yoffset),
      myFontSize(fontsize),
      myIsVisible(false),
      myIsSelected(false),
      myVecMenuStrings()
{
    myVecMenuStrings.push_back(std::make_pair(BookshelfActionCode::ADD_TAG, false));
    myVecMenuStrings.push_back(std::make_pair(BookshelfActionCode::REMOVE_TAG, false));
    myItMenuEnd = myVecMenuStrings.end();
}

void ElementMenu::draw()
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

bool ElementMenu::checkSelectedElementMenu(int x, int y, bool & changed_state) {
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





TagsMenu::TagsMenu(ZLPaintContext &context,
                   std::vector<std::string> const & tags,
                   Point topleft,
                   int xoffset,
                   int yoffset,
                   int fontsize)
    : myRefPainter(context),
      myTopLeft(topleft),
      myXOffset(xoffset),
      myYOffset(yoffset),
      myFontSize(fontsize),
      myIsVisible(false),
      myIsSelected(false),
      myVecMenuStrings()
{
    reloadTags(tags);
}

void TagsMenu::reloadTags(std::vector<std::string> const &tags)
{
    myVecMenuStrings.clear();
    std::vector<std::string>::const_iterator it_tag_end = tags.end();
    for(std::vector<std::string>::const_iterator it = tags.begin(); it != it_tag_end; ++it) {
        myVecMenuStrings.push_back(std::make_pair(*it, false));
    }
    myItMenuEnd = myVecMenuStrings.end();
}

void TagsMenu::draw() {
    myItMenu = myVecMenuStrings.begin();

    myCurTopLeft = myTopLeft;
    int cur_xr = myCurTopLeft.x + myXOffset;
    int cur_yr = myCurTopLeft.y + myYOffset;

    myCurTopLeft.x += 2;
    myCurTopLeft.y += 2;

    for(; myItMenu != myItMenuEnd; ++myItMenu) {
        if(myIsSelected && (*myItMenu).second)
            myRefPainter.setFillColor(ZLColor(210,210,90));
        else
            myRefPainter.setFillColor(ZLColor(190,190,190));

        myRefPainter.fillRectangle(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);

        myRefPainter.setColor(FBOptions::Instance().RegularTextColorOption.value());

        myRefPainter.drawString(myCurTopLeft.x,
                                cur_yr,
                                (*myItMenu).first.c_str(),
                                (*myItMenu).first.size(),
                                true);

        myRefPainter.setColor(ZLColor(255,255,255));
        myRefPainter.drawLine(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1,myCurTopLeft.y-2);
        myRefPainter.drawLine(cur_xr+1, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);
        myRefPainter.drawLine(cur_xr+1, cur_yr+2, myCurTopLeft.x-2, cur_yr+2);
        myRefPainter.drawLine(myCurTopLeft.x-2, cur_yr+2, myCurTopLeft.x-2, myCurTopLeft.y-2);

        myCurTopLeft.y += myYOffset + 2;
        cur_yr += myYOffset + 2;
    }
}

bool TagsMenu::checkSelectedElementMenu(int x, int y, bool &changed_state) {
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
