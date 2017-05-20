#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "WebView.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

#include "../library/BookshelfNetFBReaderModel.h"
#include "../library/BookshelfNetGoogleModel.h"
#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"
#include "../library/BookshelfModel.h"
#include "../library/Tag.h"

const ZLColor WEB_ELEMENT_COLOR = ZLColor(255,160,122);
const ZLColor WEB_ELEMENT_DOWNLOADED_COLOR = ZLColor(190,190,190);

const std::string WEB_CAPTION = "Web";

WebView::WebView(ZLPaintContext &context) : GridView(context),
                                            myViewMode(WebView::GOOGLE_DRIVE)
{}

const ZLTypeId WebView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &WebView::typeId() const {
    return TYPE_ID;
}

void WebView::updateView(BookshelfModel::SortType sort_type) {
    myVecBookshelfElements.clear();

    int x1 = myTopLeftX;
    int y1 = myTopleftY;
    int x2 = myElementWidth;
    int y2 = myElementHeight;

    GridElement element;

    bool books_fbreader_org = true;
    if(myViewMode == WebView::GOOGLE_DRIVE)
        books_fbreader_org = false;

    std::vector<shared_ptr<Book> > & library = books_fbreader_org ?
                                               BookshelfNetFBReaderModel::Instance().getLibrary(BookshelfNetFBReaderModel::SortType::SORT_BY_TITLE) :  //fbreader
                                               BookshelfNetGoogleModel::Instance().getLibrary(BookshelfNetGoogleModel::SortType::SORT_BY_TITLE);   //google

    if(!library.empty()) {
        std::vector<shared_ptr<Book> >::iterator it = library.begin();
        std::vector<shared_ptr<Book> >::iterator itEnd = library.end();
        int incr = 0;
        for(; it != itEnd; ++it) {
            BookModel model(*it);

            incr++;
            element.myTitleImage.myImageData = ZLImageManager::Instance().imageData(*((*it)->image()));
            element.myTitleImage.myHWFactor = (float)element.myTitleImage.myImageData->height() / element.myTitleImage.myImageData->width();
            element.myBook = *it;

            element.myTitleString = new StringRect(element.myBook->title(), context());

            element.myTopLeft.x = x1;
            element.myTopLeft.y = y1;
            element.myBottomRight.x = x2;
            element.myBottomRight.y = y2;

            element.myElementColor = element.myBook->isLocal() ? WEB_ELEMENT_DOWNLOADED_COLOR : WEB_ELEMENT_COLOR;
            element.myFrameColor = ELEMENT_FRAME_COLOR;

            myVecBookshelfElements.push_back(element);

            x1 += myElementWidth;
            x2 += myElementWidth;

            if(x2 > myViewWidth + myTopLeftX)
            {
                x1 = myTopLeftX;
                x2 = myElementWidth;
                y1 += myElementHeight;
                y2 += myElementHeight;
            }
        }

        myItSelectedElement = myVecBookshelfElements.begin();

        if(myVecBookshelfElements.size() > myRenderingElementsCount) {
            myScrollBarMaxPos = (myVecBookshelfElements.size() - myRenderingElementsCount) / myElementsOnX;
            ++myScrollBarMaxPos;
        }

        setScrollbarEnabled(VERTICAL, true);
        setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);

        myItFirstRendering = myItLastRendering = myVecBookshelfElements.begin();
        myItLastRendering += myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();

        mySortType = sort_type;
    }

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void WebView::setMode(WebView::ViewMode mode)
{
    if(mode != myViewMode)
        myViewMode = mode;

    updateView(mySortType);
}

WebView::ViewMode WebView::mode() const {
    return myViewMode;
}

void WebView::invertMode()
{
    if(myViewMode == WebView::GOOGLE_DRIVE)
        setMode(WebView::BOOKS_FBREADER_ORG);
    else
        setMode(WebView::GOOGLE_DRIVE);
}

std::vector<GridElement>::iterator WebView::getSelectedElement() {
    return myItSelectedElement;
}


bool WebView::onStylusPress(int x, int y) {

    return true;
}


bool WebView::onStylusMovePressed(int x, int y) {

    return true;
}

bool WebView::onStylusRelease(int x, int y) {
    shared_ptr<ZLTime> current_time = new ZLTime();
    long delta = current_time->millisecondsFrom(*myTime);
    myTime = current_time;

    if(delta > DELAY_ON_CLICK_BOOK_MS)
        return true;

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {

        if(it == myItSelectedElement &&
           (*it).checkSelectedBook(x, y)) {
            Fbookshelf::Instance().doAction(BookshelfActionCode::DOWNLOAD_BOOK);
            break;
        }
    }
    return true;
}


bool WebView::onStylusMove(int x, int y) {

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        bool SelectedPrevState = (*it).myIsSelected;

        if((*it).checkSelectedBook(x, y)) {
            myItSelectedElement = it;
            (*it).myIsSelected = true;
        }
        else {
            (*it).myIsSelected = false;
        }

        if((*it).myIsSelected != SelectedPrevState)
            Fbookshelf::Instance().refreshWindow();
    }

    return true;
}


void WebView::onScrollbarStep(ZLView::Direction direction, int steps) {
    GridView::onScrollbarStep(direction, steps);
}

void WebView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {
    GridView::onScrollbarMoved(direction, full, from, to);
}

void WebView::onScrollbarPageStep(ZLView::Direction direction, int steps){
    GridView::onScrollbarPageStep(direction, steps);
}

void WebView::onMouseScroll(bool forward) {
    GridView::onMouseScroll(forward);
}

void WebView::updateBookshelfElements() {
    GridView::updateBookshelfElements();
}

void WebView::updateScrollDown() {
    GridView::updateScrollDown();
}

void WebView::updateScrollUp() {
    GridView::updateScrollUp();
}

void WebView::drawBookshelfElements() {

    if(context().width() != myViewWidth || context().height() != myViewHeight) {
        updateBookshelfElements();
    }

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).drawElement(context());
    }
}

void WebView::drawBackground() {
    GridView::drawBackground();
}


const std::string &WebView::caption() const {
    return WEB_CAPTION;
}


ZLColor WebView::backgroundColor() const {
    return myBackgroundColor;
}


void WebView::paint() {
    GridView::drawBackground();
    WebView::drawBookshelfElements();
}
