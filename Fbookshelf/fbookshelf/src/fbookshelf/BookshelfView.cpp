#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "BookshelfView.h"
#include "Fbookshelf.h"
#include "BookshelfActions.h"

#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"

const int ELEMENTS_ON_X = 4;
const int ELEMENTS_ON_Y = 4;
const ZLColor ELEMENT_COLOR = ZLColor(190,190,190);
const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BACKGROUND_COLOR = ZLColor(255,255,255);
const std::string CAPTION = "Bookshelf";

BookshelfView::BookshelfView(ZLPaintContext &context) : ZLView(context),
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
  //  init();
}

void BookshelfView::init()
{
    mBookshelfElements.clear();

    int x1 = 0;
    int y1 = 0;
    int x2 = mElementWidth;
    int y2 = mElementHeight;

    BookshelfElement element;

    BooksMap::const_iterator it = Fbookshelf::Instance().getLibrary().begin();
    BooksMap::const_iterator itEnd = Fbookshelf::Instance().getLibrary().end();

    for(; it != itEnd; ++it)
    {

        BookModel model((*it).second);

        element.mTitleImage.mImageData = ZLImageManager::Instance().imageData(*(model.imageMap().begin()->second));
        element.mTitleImage.mHWFactor = (float)element.mTitleImage.mImageData->height() / element.mTitleImage.mImageData->width();
        element.mBook = (*it).second;

        element.mTitleString = new StringRect(element.mBook->title(), context());
       // element.mTitleString->mStr = ;
      //  element.mTitleString.painter = ;

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


}

std::vector<BookshelfElement>::iterator BookshelfView::getSelectedElement()
{
    return mItSelectedElement;
}

/*
shared_ptr<Book> BookshelfView::getSelectedBook()
{
    return mSelectedBook;
}

void BookshelfView::setSelectedBook(shared_ptr<Book> book)
{
    mSelectedBook = book;
}
*/

bool BookshelfView::onStylusPress(int x, int y) {
    mStartPoint.x = x;
    mStartPoint.y = y;

    return true;
}


bool BookshelfView::onStylusMovePressed(int x, int y) {
    mEndPoint.x = x;
    mEndPoint.y = y;

    Fbookshelf::Instance().refreshWindow();

    return true;
}

bool BookshelfView::onStylusRelease(int x, int y)
{
    for(std::vector<BookshelfElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        if((*it).CheckBookOptions(x, y))
        {
            std::cout << "yes\n";
            Fbookshelf::Instance().doAction(SIMPLE_DIALOG);
            break;
        }
    }
}



bool BookshelfView::onStylusMove(int x, int y)
{
    for(std::vector<BookshelfElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        bool SelectedPrevState = (*it).mIsSelected;
        bool MenuSelectedPrevState = (*it).mIsMenuSelected;

        if((*it).CheckSelectedBook(x, y))
        {
            mItSelectedElement = it;

            if((*it).CheckBookOptions(x, y)){
                (*it).mIsMenuSelected = true;
                (*it).mIsSelected = false;
            }
            else
                (*it).mIsSelected = true;
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
void BookshelfView::onScrollbarStep(ZLView::Direction direction, int steps)
{
    std::cout << "onscrollstep\n";
}



void BookshelfView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to)
{

    if(from < mScrollBarPos)
    {
        UpdateScrollUp();
        std::cout << "update scroll up\n";
    }
    else
    {
        UpdateScrollDown();
        std::cout << "update scroll down\n";
    }

    mScrollBarPos = from;
}



void BookshelfView::onScrollbarPageStep(ZLView::Direction direction, int steps)
{

    if(steps < 0)
    {
        UpdateScrollUp();
        std::cout << "update scroll step up\n";
    }
    else
    {
        UpdateScrollDown();
        std::cout << "update scroll step down\n";
    }
}


//doesn't work
void BookshelfView::onMouseScroll(bool forward)
{
    std::cout << "mouse scroll \n";

    if(forward)
    {
        UpdateScrollUp();
        std::cout << "update scroll step up\n";
    }
    else
    {
        UpdateScrollDown();
        std::cout << "update scroll step down\n";
    }
}



void BookshelfView::UpdateBookshelfElements()
{
    mViewWidth = context().width();
    mViewHeight = context().height();

    mElementWidth = mViewWidth / mElementsOnX;
    mElementHeight = mViewHeight / mElementsOnY;

    int x1 = 0;
    int y1 = 0;
    int x2 = mElementWidth;
    int y2 = mElementHeight;

    for(std::vector<BookshelfElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        (*it).UpdatePosition(x1, y1, x2, y2);

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

;
}



void BookshelfView::UpdateScrollDown()
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

    UpdateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void BookshelfView::UpdateScrollUp()
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

    UpdateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}



void BookshelfView::DrawBookshelfElements()
{
    if(context().width() != mViewWidth || context().height() != mViewHeight)
    {
        UpdateBookshelfElements();
    }

    for(std::vector<BookshelfElement>::iterator it = mItFirstRendering; it != mItLastRendering; ++it)
    {
        (*it).DrawElement(context());
    }
}



void BookshelfView::DrawBackground()
{
    context().setFillColor(mBackgroundColor);
    context().fillRectangle(0,0,context().width(),context().height());
}



const std::string &BookshelfView::caption() const {
    return mCaption;
}



ZLColor BookshelfView::backgroundColor() const {
    return mBackgroundColor;
}



void BookshelfView::paint() {

    DrawBackground();
    DrawBookshelfElements();


    mStartPoint.x = mEndPoint.x;
    mStartPoint.y = mEndPoint.y;
}
