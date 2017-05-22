#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>
#include <string>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "BookStackView.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"
#include "../library/BookshelfModel.h"
#include "../library/Tag.h"

BookStackView::BookStackView(ZLPaintContext &context) : ZLView(context),
                                              myViewMode(BookStackView::WITHOUT_TAGS_MENU),
                                              mySortType(BookshelfModel::SORT_BY_AUTHOR),
                                              myCaption(CAPTION),
                                              myViewWidth(context.width()),
                                              myViewHeight(context.height()),
                                              myBackgroundColor(BACKGROUND_COLOR),
                                              myScrollBarPos(0),
                                              myScrollBarMaxPos(1),
                                              myMouseScrollFrom(0),
                                              myMouseScrollTo(1),
                                              myBookElements(),
                                              myShelfs(),
                                              myBookSlots(),
                                              mySelectedBookElement(myBookElements.end()),
                                              myCapturedBookElement(myBookElements.end()),
                                              bookElementWidth(200),
                                              bookElementHeight(250),
                                              bookHorizontalDistance(250),
                                              bookVerticalDistance(300),
                                              leftBorder(80),
                                              rightBorder(context.width() - 80)
{
    
}

const ZLTypeId BookStackView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &BookStackView::typeId() const {
    return TYPE_ID;
}

void BookStackView::updateView(BookshelfModel::SortType sort_type) {
    createBookElements();
    Fbookshelf::Instance().refreshWindow();
}

void BookStackView::createBookElements() {
    myBookElements.clear();
    BooksByShelf booksByShelf = BookshelfModel::Instance().getBooksByShelf();
    BooksByShelf::const_iterator it = booksByShelf.begin();
    BooksByShelf::const_iterator itEnd = booksByShelf.end();
    if (it == itEnd) {
        return;
    }
    for(; it != itEnd; ++it) {
        BookList::const_iterator jt = (*it).second.begin();
        BookList::const_iterator jtEnd = (*it).second.end();
        for(; jt != jtEnd; ++jt) {
            
            BookElement bookElement;
            BookModel model(*jt);
            bookElement.myTitleImage.myImageData = ZLImageManager::Instance().imageData(*(model.imageMap().begin()->second));
            bookElement.myTitleImage.myHWFactor = (float)bookElement.myTitleImage.myImageData->height() / bookElement.myTitleImage.myImageData->width();
            bookElement.myBook = *jt;
            bookElement.myShelf = (*it).first;
            myBookElements.push_back(bookElement);
        }
    }
    updateBookElements();
    updateBookSlots();
}

void BookStackView::updateBookElements() {
    if (myBookElements.empty()) {
        createBookElements();
    }
    std::vector<BookElement>::iterator it = myBookElements.begin();
    std::vector<BookElement>::iterator itEnd = myBookElements.end();
    if (it == itEnd) {
        return;
    }
    int x1 = leftBorder;
    int y1 = 30 - myMouseScrollFrom;
    int x2 = x1 + bookElementWidth;
    int y2 = y1 + bookElementHeight;
 
    std::string prevShelf = (*it).myShelf;
    for(; it != itEnd; ++it) {       
        if (x2 > context().width() || (*it).myShelf != prevShelf) {
            x1 = leftBorder;
            y1 += bookVerticalDistance;
            x2 = x1 + bookElementWidth;
            y2 += bookVerticalDistance;
        }
        if (it != myCapturedBookElement) {
            (*it).updatePosition(x1, y1, x2, y2);
        }
        x1 += bookHorizontalDistance;
        x2 += bookHorizontalDistance;
        prevShelf = (*it).myShelf;
    }
    
    if (y2 <= context().height()) {
        myScrollBarMaxPos = 1;
        myMouseScrollFrom = 0;
        myMouseScrollTo = 1;
        setScrollbarEnabled(VERTICAL, false);
    } else {  
        myScrollBarMaxPos = y2;
        myMouseScrollTo = (myMouseScrollFrom + context().height());
    }
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    setScrollbarEnabled(VERTICAL, true);
    updateBookSlots();
}

void BookStackView::updateBookSlots() {
    myViewWidth = context().width();
    myViewHeight = context().height();
    myBookSlots.clear();
    myShelfs.clear();
    std::vector<BookElement>::const_iterator it = myBookElements.begin();
    std::vector<BookElement>::const_iterator itEnd = myBookElements.end();
    if(it == itEnd) {
        return;
    }
    BookSlot bookSlot;
    int x1 = 0;
    int y1 = 30 - myMouseScrollFrom;
    int x2 = x1 + (*it).myTitleImage.myWidth / 2 + leftBorder;
    int y2 = y1 + (*it).myTitleImage.myHeight;
    bookSlot.myShelf = (*it).myShelf;
    size_t index = 0;
    bookSlot.myIndex = index;
    bookSlot.updatePosition(x1, y1, x2, y2);
    myBookSlots.push_back(bookSlot);
    Shelf shelf(0, y1 + 270, context().width(), 500);
    myShelfs.push_back(shelf);
   
    

    std::string prevShelf = (*it).myShelf;
    it++;
    for(; it != itEnd; ++it) {
        x1 = x2;
        x2 = (*it).myTopLeft.x + (*it).myTitleImage.myWidth / 2;
        bookSlot.myShelf = (*it).myShelf;
        ++index;
        if (((*it).myTopLeft.x == leftBorder) || myCapturedBookElement == it && prevX1 == leftBorder) {
            bookSlot.myIndex = index;
            bookSlot.myShelf = prevShelf;
            bookSlot.updatePosition(x1, y1, context().width(), y2);
            myBookSlots.push_back(bookSlot);
            myShelfs.push_back(Shelf(0, y1 + bookVerticalDistance + 270, 10 * context().width(), 2000));
            if ((*it).myShelf != prevShelf) {
                index = 0;
            }
            x1 = 0;
            y1 += bookVerticalDistance;
            x2 = (*it).myTopLeft.x + (*it).myTitleImage.myWidth / 2;
            y2 += bookVerticalDistance;
        }
        prevShelf = (*it).myShelf;
        bookSlot.myIndex = index;
        bookSlot.myShelf = (*it).myShelf;
        bookSlot.updatePosition(x1, y1, x2, y2); 
        myBookSlots.push_back(bookSlot);
    }
    ++index;
    bookSlot.myIndex = index;
    bookSlot.updatePosition(x2, y1, context().width(), y2); 
    myBookSlots.push_back(bookSlot);
}

void BookStackView::setMode(BookStackView::ViewMode mode) {
        updateView(mySortType);
}

std::vector<BookElement>::iterator BookStackView::getSelectedElement() {
    return mySelectedBookElement;
}


bool BookStackView::onStylusPress(int x, int y) {
    std::vector<BookElement>::iterator it = myBookElements.begin();
    std::vector<BookElement>::iterator itEnd = myBookElements.end();
    for(; it != itEnd; ++it) {
        if((*it).checkSelectedBookElement(x, y)) {
            (*it).myIsSelected = true;
            myCapturedBookElement = it;
            mySelectedBookElement = it;
            prevX1 = (*it).myTopLeft.x;
            return true;
        }
    }
    
    myCapturedBookElement = myBookElements.end();
    return true;
}


bool BookStackView::onStylusRelease(int x, int y) {
    std::vector<BookElement>::iterator it = myBookElements.begin();
    std::vector<BookElement>::iterator itEnd = myBookElements.end();
    for(; it != itEnd; ++it) {
        if((*it).checkSelectedBookElement(x, y) && it == mySelectedBookElement && !(*it).myIsCaptured) {
            Fbookshelf::Instance().doAction(BookshelfActionCode::RUN_FBREADER);
            (*it).myIsSelected = false;
            mySelectedBookElement = myBookElements.end();
            myCapturedBookElement = myBookElements.end();
            createBookElements();
            Fbookshelf::Instance().refreshWindow();
            return true;
        }
    }
    std::vector<BookSlot>::iterator jt = myBookSlots.begin();
    std::vector<BookSlot>::iterator jtEnd = myBookSlots.end();
    for(; jt != jtEnd; ++jt) {
        if((*jt).checkSelectedSlot(x, y) && myCapturedBookElement != myBookElements.end()) {
            BookshelfModel::Instance().replaceBookToShelfSlot((*myCapturedBookElement).myShelf, (*jt).myShelf, (*jt).myIndex, (*myCapturedBookElement).myBook);
//            (*mySelectedBookElement).myIsSelected = false;
            mySelectedBookElement = myBookElements.end();
            (*myCapturedBookElement).myIsCaptured = true;
            myCapturedBookElement = myBookElements.end();
            createBookElements();
            Fbookshelf::Instance().refreshWindow();
            return true;
        }
    }
    myCapturedBookElement = myBookElements.end();
    Fbookshelf::Instance().refreshWindow();
    return true;
    
}

bool BookStackView::onStylusMovePressed(int x, int y) {
    if (myCapturedBookElement != myBookElements.end()) {
        (*myCapturedBookElement).myIsCaptured = true;
        int x1 = x - (*myCapturedBookElement).myTitleImage.myWidth / 2;
        int y1 = y - (*myCapturedBookElement).myTitleImage.myHeight / 2;
        int x2 = x1 + (*myCapturedBookElement).myTitleImage.myWidth;
        int y2 = y1 + (*myCapturedBookElement).myTitleImage.myHeight;
        if (y1 < 0) {
            onScrollbarPageStep(ZLView::HORIZONTAL, -1);
        } else if (y2 > context().height()) {
            onScrollbarPageStep(ZLView::HORIZONTAL, 1);
        }


        (*myCapturedBookElement).updatePosition(x1, y1, x2, y2);
        Fbookshelf::Instance().refreshWindow();
    }
    return true;
}



bool BookStackView::onStylusMove(int x, int y) {

    return true;
}

void BookStackView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {
    myMouseScrollFrom = from;
    myMouseScrollTo = to;
    if (to > myScrollBarMaxPos) {
        myMouseScrollTo = myScrollBarMaxPos;
        myMouseScrollFrom = myMouseScrollTo - context().height();
    } else if (from > myScrollBarMaxPos) {
        myMouseScrollFrom = 0;
        myMouseScrollTo += context().height();
    }
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    Fbookshelf::Instance().refreshWindow();
}



void BookStackView::onScrollbarPageStep(ZLView::Direction direction, int steps){
    (steps > 0) ? steps = 1 : steps = -1;
    myMouseScrollFrom += steps;
    myMouseScrollTo += steps;
    if (myMouseScrollTo > myScrollBarMaxPos) {
        myMouseScrollTo = myScrollBarMaxPos;
        myMouseScrollFrom = myMouseScrollTo - context().height();
    } else if (myMouseScrollFrom > myScrollBarMaxPos) {
        myMouseScrollFrom = 0;
        myMouseScrollTo += context().height();
    }
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    Fbookshelf::Instance().refreshWindow();
}

void BookStackView::onScrollbarStep(ZLView::Direction direction, int steps){
    (steps > 0) ? steps = 1 : steps = -1;
    myMouseScrollFrom += steps;
    myMouseScrollTo += steps;
    
    if (myMouseScrollTo > myScrollBarMaxPos) {
        myMouseScrollTo = myScrollBarMaxPos;
        myMouseScrollFrom = myMouseScrollTo - context().height();
    } else if (myMouseScrollFrom > myScrollBarMaxPos) {
        myMouseScrollFrom = 0;
        myMouseScrollTo += context().height();
    }
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    Fbookshelf::Instance().refreshWindow();
}

void BookStackView::onMouseScroll(bool forward) {
    if (forward) {
        onScrollbarStep(ZLView::VERTICAL, 1);
    } else {
        onScrollbarStep(ZLView::VERTICAL, -1);
    }
}

void BookStackView::drawBookSlots() {
    for(std::vector<BookSlot>::iterator it = myBookSlots.begin(); it != myBookSlots.end(); ++it) {
        (*it).draw(context());
    }
}

void BookStackView::drawBackground() {
    context().setFillColor(ZLColor((150, 75, 0)));
    context().fillRectangle(0,0,context().width(),context().height());
}

void BookStackView::drawBookshelfs() {
    updateBookElements();
    for(std::vector<Shelf>::iterator it = myShelfs.begin(); it != myShelfs.end(); ++it) {
        (*it).draw(context());
    }
}

void BookStackView::drawBookElements() {
    for(std::vector<BookElement>::iterator it = myBookElements.begin(); it != myBookElements.end(); ++it) {
        (*it).drawElement(context());
    }
}


const std::string &BookStackView::caption() const {
    return myCaption;
}

ZLColor BookStackView::backgroundColor() const {
    return myBackgroundColor;
}

void BookStackView::paint() {
    context().clear(ZLColor(150, 75, 0));
    drawBookshelfs();
    drawBookElements();
//    drawBookSlots();
}