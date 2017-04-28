#include "BookshelfMenu.h"
#include "BookshelfActions.h"

#include <cassert>
#include <iostream>

#include "../library/Tag.h"



BookshelfMenu::BookshelfMenu(ZLPaintContext &context,
                   std::vector<std::string> const & tags)
    : myRefPainter(context),
      myXOffset(200),
      myYOffset(20),
      myFontSize(7),
      myViewHeight(context.height()),
      myRenderingElementsCount(myViewHeight / myYOffset),
      myTopLeft(Point()),
      myIsVisible(false),
      myIsSelected(false),
      myVecMenuStrings()
{
    reloadTags(tags);
}

void BookshelfMenu::reloadTags(std::vector<std::string> const &tags) {
    myViewHeight = myRefPainter.height();
    myRenderingElementsCount = myViewHeight / myYOffset;

    myVecMenuStrings.clear();
    std::vector<std::string>::const_iterator it_tag_end = tags.end();

    for(std::vector<std::string>::const_iterator it = tags.begin(); it != it_tag_end; ++it) {
        myVecMenuStrings.push_back(std::make_pair(*it, false));
    }

    myItFirstRenderingMenu = myItLastRenderingMenu = myVecMenuStrings.begin();
    myItLastRenderingMenu += myVecMenuStrings.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecMenuStrings.size();

}

void BookshelfMenu::draw() {
    const FBTextStyle &style = FBTextStyle::Instance();
    myRefPainter.setFont(style.fontFamily(), myFontSize, style.bold(), style.italic());

    myCurTopLeft = myTopLeft;
    int cur_xr = myCurTopLeft.x + myXOffset;
    int cur_yr = myCurTopLeft.y + myYOffset;

    myCurTopLeft.x += 2;
    myCurTopLeft.y += 2;

    for(std::vector<std::pair<std::string, bool> >::iterator it = myItFirstRenderingMenu; it != myItLastRenderingMenu; ++it) {
        if(myIsSelected && (*it).second)
            myRefPainter.setFillColor(ZLColor(210,210,90));
        else
            myRefPainter.setFillColor(ZLColor(190,190,190));

        myRefPainter.fillRectangle(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);
        myRefPainter.setColor(FBOptions::Instance().RegularTextColorOption.value());

        if(myRefPainter.stringWidth((*it).first.c_str(), (*it).first.size(), true) < myXOffset) {

            myRefPainter.drawString(myCurTopLeft.x,
                                    cur_yr,
                                    (*it).first.c_str(),
                                    (*it).first.size(),
                                    true);
        }
        else {
            std::string new_str;
            divideStr((*it).first, new_str);

            myRefPainter.drawString(myCurTopLeft.x,
                                    cur_yr,
                                    new_str.c_str(),
                                    new_str.size(),
                                    true);
        }

        myRefPainter.setColor(ZLColor(255,255,255));
        myRefPainter.drawLine(myCurTopLeft.x-2, myCurTopLeft.y-2, cur_xr+1,myCurTopLeft.y-2);
        myRefPainter.drawLine(cur_xr+1, myCurTopLeft.y-2, cur_xr+1, cur_yr+2);
        myRefPainter.drawLine(cur_xr+1, cur_yr+2, myCurTopLeft.x-2, cur_yr+2);
        myRefPainter.drawLine(myCurTopLeft.x-2, cur_yr+2, myCurTopLeft.x-2, myCurTopLeft.y-2);

        myCurTopLeft.y += myYOffset + 2;
        cur_yr += myYOffset + 2;
    }
}

void BookshelfMenu::divideStr(std::string const & source, std::string & new_str) {
    new_str.reserve(source.size());
    new_str.clear();
    for(size_t i = 1; i != source.size() + 1; ++i) {
        if(myRefPainter.stringWidth(source.substr(0, i).c_str(), source.substr(0, i).size(), true) >= myXOffset) {
            new_str = source.substr(0, i-1);
            break;
        }
    }
}

void BookshelfMenu::checkFont() {
    const FBTextStyle &style = FBTextStyle::Instance();
    myRefPainter.setFont(style.fontFamily(), myFontSize, style.bold(), style.italic());
}


bool BookshelfMenu::checkSelectedElementMenu(int x, int y, bool &changed_state) {
    bool res = false;
    if((x > myTopLeft.x && x < myTopLeft.x + myXOffset)) {

        myCurTopLeft = myTopLeft;
        myCurTopLeft.y += 2;

        for(std::vector<std::pair<std::string, bool> >::iterator it = myItFirstRenderingMenu; it != myItLastRenderingMenu; ++it) {
            bool prev = (*it).second;

            if(y > myCurTopLeft.y && y < myCurTopLeft.y + myYOffset + 2) {
                myItSelectedActionCode = it;
                (*it).second = true;
                res = true;
            }
            else
                (*it).second = false;

            myCurTopLeft.y += myYOffset + 2;

            if(prev != (*it).second)
                changed_state = true;
        }
    }
    return res;
}

void BookshelfMenu::updateScrollDown() {

    if(myItLastRenderingMenu + 1 <= myVecMenuStrings.end()) {
        ++myItFirstRenderingMenu;
        ++myItLastRenderingMenu;
    }
    else {
        myItLastRenderingMenu = myItFirstRenderingMenu = myVecMenuStrings.end();
        myItFirstRenderingMenu -= myVecMenuStrings.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecMenuStrings.size();
    }

    assert(myItFirstRenderingMenu <= myItLastRenderingMenu);
    assert(myItFirstRenderingMenu >= myVecMenuStrings.begin() && myItFirstRenderingMenu < myVecMenuStrings.end());
    assert(myItLastRenderingMenu > myVecMenuStrings.begin() && myItLastRenderingMenu <= myVecMenuStrings.end());

}

void BookshelfMenu::updateScrollUp() {

    if(myItFirstRenderingMenu - 1 >= myVecMenuStrings.begin()) {
        --myItFirstRenderingMenu;
        --myItLastRenderingMenu;
    }
    else {
        myItFirstRenderingMenu = myItLastRenderingMenu = myVecMenuStrings.begin();
        myItLastRenderingMenu += myVecMenuStrings.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecMenuStrings.size();
    }

    assert(myItFirstRenderingMenu <= myItLastRenderingMenu);
    assert(myItFirstRenderingMenu >= myVecMenuStrings.begin() && myItFirstRenderingMenu < myVecMenuStrings.end());
    assert(myItLastRenderingMenu > myVecMenuStrings.begin() && myItLastRenderingMenu <= myVecMenuStrings.end());

}
