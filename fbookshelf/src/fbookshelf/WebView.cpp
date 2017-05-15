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

const int ELEMENTS_ON_X = 3;
const int ELEMENTS_ON_Y = 3;

const ZLColor ELEMENT_COLOR = ZLColor(255,160,122);
const ZLColor ELEMENT_DOWNLOADED_COLOR = ZLColor(190,190,190);
const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BACKGROUND_COLOR = ZLColor(255,255,255);

const std::string CAPTION = "Web";

WebView::WebView(ZLPaintContext &context) : ZLView(context),
                                              myViewMode(WebView::GOOGLE_DRIVE),
                                              mySortType(BookshelfModel::SORT_BY_AUTHOR),
                                              myTopLeftX(0),
                                              myTopleftY(0),
                                              myCaption(CAPTION),
                                              myViewWidth(context.width()),
                                              myViewHeight(context.height()),
                                              myBackgroundColor(BACKGROUND_COLOR),
                                              myElementsOnX(ELEMENTS_ON_X),
                                              myElementsOnY(ELEMENTS_ON_Y),
                                              myRenderingElementsCount(myElementsOnX * myElementsOnY),
                                              myScrollBarPos(0),
                                              myScrollBarMaxPos(1),
                                              myMouseScrollFrom(0),
                                              myMouseScrollTo(1),
                                              myElementWidth(myViewWidth / myElementsOnX),
                                              myElementHeight(myViewHeight / myElementsOnY),
                                              myVecBookshelfElements(),
                                              myItSelectedElement(myVecBookshelfElements.end()),
                                              myItFirstRendering(myVecBookshelfElements.end()),
                                              myItLastRendering(myVecBookshelfElements.end())
{
}

const ZLTypeId WebView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &WebView::typeId() const {
    return TYPE_ID;
}

void WebView::updateView(BookshelfModel::SortType sort_type) {
    std::cout << "Here Update";
    //if(mySortType != sort_type || myVecBookshelfElements.empty()) {
        myVecBookshelfElements.clear();

        int x1 = myTopLeftX;
        int y1 = myTopleftY;
        int x2 = myElementWidth;
        int y2 = myElementHeight;

        WebElement element;

        bool books_fbreader_org = true;
        if(myViewMode == WebView::GOOGLE_DRIVE)
            books_fbreader_org = false;

        std::vector<shared_ptr<Book> > & library = books_fbreader_org ?
                                                   BookshelfNetFBReaderModel::Instance().getLibrary(BookshelfNetFBReaderModel::SortType::SORT_BY_TITLE) :  //fbreader
                                                   BookshelfNetGoogleModel::Instance().getLibrary(BookshelfNetGoogleModel::SortType::SORT_BY_TITLE);   //google
        std::cout << library.size() << std::endl;
        if(library.empty()) {
            //just after first start library is empty
            //show message about authorization
            //"push button to authorize"
            //or nothing
        }
        else {
            std::vector<shared_ptr<Book> >::iterator it = library.begin();
            std::vector<shared_ptr<Book> >::iterator itEnd = library.end();
            int incr = 0;
            for(; it != itEnd; ++it) {
                BookModel model(*it);
                
                incr++;
                //change myTitleImage to (*it)->image()
                element.myTitleImage.myImageData = ZLImageManager::Instance().imageData(*((*it)->image()));
                element.myTitleImage.myHWFactor = (float)element.myTitleImage.myImageData->height() / element.myTitleImage.myImageData->width();
                element.myBook = *it;
		
                element.myTitleString = new StringRect(element.myBook->title(), context());

                element.myTopLeft.x = x1;
                element.myTopLeft.y = y1;
                element.myBottomRight.x = x2;
                element.myBottomRight.y = y2;

                element.myElementColor = element.myBook->isLocal() ? ELEMENT_DOWNLOADED_COLOR : ELEMENT_COLOR;
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
                std::cout << "Incr: " << incr << std::endl;
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
    //}
    std::cout << "update begin\n";
    updateBookshelfElements();
    std::cout << "update end\n";
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

std::vector<WebElement>::iterator WebView::getSelectedElement() {
    return myItSelectedElement;
}


bool WebView::onStylusPress(int x, int y) {

    return true;
}


bool WebView::onStylusMovePressed(int x, int y) {

    return true;
}

bool WebView::onStylusRelease(int x, int y) {
    for(std::vector<WebElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {

        if(it == myItSelectedElement &&
           (*it).checkSelectedBook(x, y)) {
            std::cout << "do action release\n";
            Fbookshelf::Instance().doAction(BookshelfActionCode::DOWNLOAD_BOOK);
            break;
        }
    }
    return true;
}


bool WebView::onStylusMove(int x, int y) {

    for(std::vector<WebElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
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
    std::cout << "onscrollstep\n";
}



void WebView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {

    if(from < myScrollBarPos) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }

    myScrollBarPos = from;
}



void WebView::onScrollbarPageStep(ZLView::Direction direction, int steps){

    if(steps < 0) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }
}


void WebView::onMouseScroll(bool forward) {
    if(forward && myMouseScrollTo < myScrollBarMaxPos) {
        ++myMouseScrollFrom;
        ++myMouseScrollTo;
        onScrollbarMoved(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
        setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    }

    if(!forward && myMouseScrollFrom > 0) {
        --myMouseScrollFrom;
        --myMouseScrollTo;
        onScrollbarMoved(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
        setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    }
}



void WebView::updateBookshelfElements() {
    myViewWidth = context().width();
    myViewHeight = context().height();

    std::cout << "works\n";

    myElementWidth = myViewWidth / myElementsOnX;
    myElementHeight = myViewHeight / myElementsOnY;

    int x1 = myTopLeftX;
    int y1 = myTopleftY;
    int x2 = myTopLeftX + myElementWidth;
    int y2 = myElementHeight;

    for(std::vector<WebElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).updatePosition(x1, y1, x2, y2);

        x1 += myElementWidth;
        x2 += myElementWidth;

        if(x2 > myViewWidth + myTopLeftX) {
            x1 = myTopLeftX;
            x2 = myTopLeftX + myElementWidth;
            y1 += myElementHeight;
            y2 += myElementHeight;
        }
    }

}


void WebView::updateScrollDown() {
    if(myItLastRendering + myElementsOnX <= myVecBookshelfElements.end()){
        myItFirstRendering += myElementsOnX;
        myItLastRendering += myElementsOnX;
    }
    else {
        myItLastRendering = myItFirstRendering = myVecBookshelfElements.end();
        myItFirstRendering -= myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();
    }

    assert(myItFirstRendering <= myItLastRendering);
    assert(myItFirstRendering >= myVecBookshelfElements.begin() && myItFirstRendering < myVecBookshelfElements.end());
    assert(myItLastRendering > myVecBookshelfElements.begin() && myItLastRendering <= myVecBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void WebView::updateScrollUp()
{
    if(myItFirstRendering - myElementsOnX >= myVecBookshelfElements.begin()){
        myItFirstRendering -= myElementsOnX;
        myItLastRendering -= myElementsOnX;
    }
    else {
        myItFirstRendering = myItLastRendering = myVecBookshelfElements.begin();
        myItLastRendering += myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();
    }

    assert(myItFirstRendering <= myItLastRendering);
    assert(myItFirstRendering >= myVecBookshelfElements.begin() && myItFirstRendering < myVecBookshelfElements.end());
    assert(myItLastRendering > myVecBookshelfElements.begin() && myItLastRendering <= myVecBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}



void WebView::drawBookshelfElements() {

    if(context().width() != myViewWidth || context().height() != myViewHeight) {
        updateBookshelfElements();
    }

    for(std::vector<WebElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).drawElement(context());
    }
}



void WebView::drawBackground() {
    context().setFillColor(myBackgroundColor);
    context().fillRectangle(0,0,context().width(),context().height());
}



const std::string &WebView::caption() const {
    return myCaption;
}



ZLColor WebView::backgroundColor() const {
    return myBackgroundColor;
}



void WebView::paint() {
    drawBackground();
    drawBookshelfElements();

}
