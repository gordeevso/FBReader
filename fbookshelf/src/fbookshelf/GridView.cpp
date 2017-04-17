#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "GridView.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"
#include "../library/BookshelfModel.h"

const int ELEMENTS_ON_X = 3;
const int ELEMENTS_ON_Y = 3;
const ZLColor ELEMENT_COLOR = ZLColor(190,190,190);
const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BACKGROUND_COLOR = ZLColor(255,255,255);

const std::string CAPTION = "Grid";

GridView::GridView(ZLPaintContext &context) : ZLView(context),
                                                        mCaption(CAPTION),
                                                        mViewWidth(context.width()),
                                                        mViewHeight(context.height()),
                                                        mBackgroundColor(BACKGROUND_COLOR),
                                                        mElementsOnX(ELEMENTS_ON_X),
                                                        mElementsOnY(ELEMENTS_ON_Y),
                                                        mRenderingElementsCount(mElementsOnX * mElementsOnY),
                                                        mElementWidth(mViewWidth / mElementsOnX),
                                                        mElementHeight(mViewHeight / mElementsOnY)
{
}

void GridView::updateView(BookshelfModel::SortType sort_type)
{
    mBookshelfElements.clear();

    int x1 = 0;
    int y1 = 0;
    int x2 = mElementWidth;
    int y2 = mElementHeight;

    GridElement element;

    std::vector<shared_ptr<Book> > & library = BookshelfModel::Instance().getLibrary(sort_type);

    std::vector<shared_ptr<Book> >::const_iterator it = library.begin();
    std::vector<shared_ptr<Book> >::const_iterator itEnd = library.end();

    for(; it != itEnd; ++it)
    {

        BookModel model(*it);

        element.mTitleImage.mImageData = ZLImageManager::Instance().imageData(*(model.imageMap().begin()->second));
        element.mTitleImage.mHWFactor = (float)element.mTitleImage.mImageData->height() / element.mTitleImage.mImageData->width();
        element.mBook = *it;

        element.mTitleString = new StringRect(element.mBook->title(), context());

        element.mTopLeft.x = x1;
        element.mTopLeft.y = y1;
        element.mBottomRight.x = x2;
        element.mBottomRight.y = y2;

        element.mElementColor = ELEMENT_COLOR;
        element.mFrameColor = ELEMENT_FRAME_COLOR;

        mBookshelfElements.push_back(element);

        x1 += mElementWidth;
        x2 += mElementWidth;

        if(x2 > mViewWidth)
        {
            x1 = 0;
            x2 = mElementWidth;
            y1 += mElementHeight;
            y2 += mElementHeight;
        }
    }


    setScrollbarEnabled(VERTICAL, true);
    setScrollbarParameters(VERTICAL, mBookshelfElements.size()/mElementsOnY, 0, 1);

    mScrollBarPos = 0;

    mItFirstRendering = mItLastRendering = mBookshelfElements.begin();
    mItLastRendering += mBookshelfElements.size() > mRenderingElementsCount ? mRenderingElementsCount : mBookshelfElements.size();

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

std::vector<GridElement>::iterator GridView::getSelectedElement()
{
    return mItSelectedElement;
}


bool GridView::onStylusPress(int x, int y) {


    return true;
}


bool GridView::onStylusMovePressed(int x, int y) {


    return true;
}

bool GridView::onStylusRelease(int x, int y)
{
    for(std::vector<GridElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        if((*it).checkBookOptions(x, y))
        {
            Fbookshelf::Instance().doAction(BookshelfActionCode::ADD_TAG);
            break;
        }
    }
}



bool GridView::onStylusMove(int x, int y)
{
    for(std::vector<GridElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        bool SelectedPrevState = (*it).mIsSelected;
        bool MenuSelectedPrevState = (*it).mIsMenuSelected;

        if((*it).checkSelectedBook(x, y))
        {
            mItSelectedElement = it;

            if((*it).checkBookOptions(x, y)){
                (*it).mIsMenuSelected = true;
                (*it).mIsSelected = false;
            }
            else{
                (*it).mIsSelected = true;
                (*it).mIsMenuSelected = false;
            }
        }
        else
        {
            (*it).mIsSelected = false;
            (*it).mIsMenuSelected = false;
        }

        if((*it).mIsSelected != SelectedPrevState || (*it).mIsMenuSelected != MenuSelectedPrevState)
            Fbookshelf::Instance().refreshWindow();

    }
}


//What is it?
void GridView::onScrollbarStep(ZLView::Direction direction, int steps)
{
    std::cout << "onscrollstep\n";
}



void GridView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to)
{

    if(from < mScrollBarPos)
    {
        updateScrollUp();
        std::cout << "update scroll up\n";
    }
    else
    {
        updateScrollDown();
        std::cout << "update scroll down\n";
    }

    mScrollBarPos = from;
}



void GridView::onScrollbarPageStep(ZLView::Direction direction, int steps)
{

    if(steps < 0)
    {
        updateScrollUp();
        std::cout << "update scroll step up\n";
    }
    else
    {
        updateScrollDown();
        std::cout << "update scroll step down\n";
    }
}


//doesn't work
void GridView::onMouseScroll(bool forward)
{
    std::cout << "mouse scroll \n";

    if(forward)
    {
        updateScrollUp();
        std::cout << "update scroll step up\n";
    }
    else
    {
        updateScrollDown();
        std::cout << "update scroll step down\n";
    }
}



void GridView::updateBookshelfElements()
{
    mViewWidth = context().width();
    mViewHeight = context().height();

    mElementWidth = mViewWidth / mElementsOnX;
    mElementHeight = mViewHeight / mElementsOnY;

    int x1 = 0;
    int y1 = 0;
    int x2 = mElementWidth;
    int y2 = mElementHeight;

    for(std::vector<GridElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        (*it).updatePosition(x1, y1, x2, y2);

        x1 += mElementWidth;
        x2 += mElementWidth;

        if(x2 > mViewWidth)
        {
            x1 = 0;
            x2 = mElementWidth;
            y1 += mElementHeight;
            y2 += mElementHeight;
        }
    }


}



void GridView::updateScrollDown()
{
    if(mItLastRendering + mElementsOnX <= mBookshelfElements.end()){
        mItFirstRendering += mElementsOnX;
        mItLastRendering += mElementsOnX;
    }
    else
    {
        mItLastRendering = mItFirstRendering = mBookshelfElements.end();
        mItFirstRendering -= mBookshelfElements.size() > mRenderingElementsCount ? mRenderingElementsCount : mBookshelfElements.size();
    }

    assert(mItFirstRendering <= mItLastRendering);
    assert(mItFirstRendering >= mBookshelfElements.begin() && mItFirstRendering < mBookshelfElements.end());
    assert(mItLastRendering > mBookshelfElements.begin() && mItLastRendering <= mBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void GridView::updateScrollUp()
{
    if(mItFirstRendering - mElementsOnX >= mBookshelfElements.begin()){
        mItFirstRendering -= mElementsOnX;
        mItLastRendering -= mElementsOnX;
    }
    else
    {
        mItFirstRendering = mItLastRendering = mBookshelfElements.begin();
        mItLastRendering += mBookshelfElements.size() > mRenderingElementsCount ? mRenderingElementsCount : mBookshelfElements.size();
    }

    assert(mItFirstRendering <= mItLastRendering);
    assert(mItFirstRendering >= mBookshelfElements.begin() && mItFirstRendering < mBookshelfElements.end());
    assert(mItLastRendering > mBookshelfElements.begin() && mItLastRendering <= mBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}



void GridView::drawBookshelfElements()
{
    if(context().width() != mViewWidth || context().height() != mViewHeight)
    {
        updateBookshelfElements();
    }

    for(std::vector<GridElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        (*it).drawElement(context());
    }
}



void GridView::drawBackground()
{
    context().setFillColor(mBackgroundColor);
    context().fillRectangle(0,0,context().width(),context().height());
}



const std::string &GridView::caption() const {
    return mCaption;
}



ZLColor GridView::backgroundColor() const {
    return mBackgroundColor;
}



void GridView::paint() {

    drawBackground();
    drawBookshelfElements();

}
