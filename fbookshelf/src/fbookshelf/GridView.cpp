#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>
#include <ZLTimeManager.h>
#include <ZLTime.h>

#include "GridView.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"
#include "../library/BookshelfModel.h"
#include "../library/Tag.h"



GridView::GridView(ZLPaintContext &context) : ZLView(context),
                                              myViewMode(GridView::WITHOUT_TAGS_MENU),
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
                                              myItLastRendering(myVecBookshelfElements.end()),
                                              myContextMenu(context),
                                              myShelfvesMenu(0),
                                              myCurrentShelf(ALL_SHELVES),
                                              myLastShelf(""),
                                              myTime(new ZLTime())
{
    std::vector<std::string> tags;
    myShelfvesMenu = new BookshelfMenu(context, tags);
}

const ZLTypeId GridView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &GridView::typeId() const {
    return TYPE_ID;
}

void GridView::updateView(BookshelfModel::SortType sort_type) {
    myLastShelf = myCurrentShelf;
    myVecBookshelfElements.clear();

    if(myViewMode == GridView::WITHOUT_TAGS_MENU)
        myViewWidth = context().width();

    if(myViewMode == GridView::WITH_TAGS_MENU)
        myViewWidth = context().width() - myTopLeftX;

    myElementWidth = myViewWidth / myElementsOnX;

    int x1 = myTopLeftX;
    int y1 = myTopleftY;
    int x2 = myTopLeftX + myElementWidth;
    int y2 = myElementHeight;

    GridElement element;

    std::vector<shared_ptr<Book> > & library = BookshelfModel::Instance().getLibrary(sort_type);

    std::vector<shared_ptr<Book> >::const_iterator it = library.begin();
    std::vector<shared_ptr<Book> >::const_iterator itEnd = library.end();

    std::vector<std::string> shelves = BookshelfModel::Instance().getShelves();
    shelves.insert(shelves.begin(), ALL_SHELVES);
    myShelfvesMenu->reloadTags(shelves);

    std::vector<std::string> shelves_curr_book;

    bool show_book = false;
    for(; it != itEnd; ++it) {

        show_book = false;
        shelves_curr_book = (*it)->shelves();

        for(auto & x: shelves_curr_book) {
            if(myCurrentShelf == x)
                show_book = true;
        }

        if(myCurrentShelf == ALL_SHELVES)
            show_book = true;

        if(!show_book)
            continue;

        BookModel model(*it);

        element.myTitleImage.myImageData = ZLImageManager::Instance().imageData(*(model.imageMap().begin()->second));
        element.myTitleImage.myHWFactor = (float)element.myTitleImage.myImageData->height() / element.myTitleImage.myImageData->width();
        element.myBook = *it;

        element.myTitleString = new StringRect(element.myBook->title(), context());

        element.myTopLeft.x = x1;
        element.myTopLeft.y = y1;
        element.myBottomRight.x = x2;
        element.myBottomRight.y = y2;

        element.myElementColor = ELEMENT_COLOR;
        element.myFrameColor = ELEMENT_FRAME_COLOR;

        myVecBookshelfElements.push_back(element);

        x1 += myElementWidth;
        x2 += myElementWidth;

        if(x2 > myViewWidth + myTopLeftX)
        {
            x1 = myTopLeftX;
            x2 = myTopLeftX + myElementWidth;
            y1 += myElementHeight;
            y2 += myElementHeight;
        }
    }

    myItSelectedElement = myVecBookshelfElements.begin();
    myContextMenu.myIsVisible = false;

    if(myVecBookshelfElements.size() > myRenderingElementsCount) {
        myScrollBarMaxPos = (myVecBookshelfElements.size() - myRenderingElementsCount) / myElementsOnX;
        ++myScrollBarMaxPos;
    }

    setScrollbarEnabled(VERTICAL, true);
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);

    myItFirstRendering = myItLastRendering = myVecBookshelfElements.begin();
    myItLastRendering += myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();

    mySortType = sort_type;

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void GridView::resizeElements(bool smaller) {
    if(smaller) {
        if(myElementsOnX < 10 && myElementsOnY < 10) {
            ++myElementsOnX;
            ++myElementsOnY;
        }
        else
            return;
    }
    else {
        if(myElementsOnX > 2 && myElementsOnY > 2) {
            --myElementsOnX;
            --myElementsOnY;
        }
        else
            return;
    }

    myRenderingElementsCount = myElementsOnX * myElementsOnY;
    myScrollBarPos = 0;
    myScrollBarMaxPos = 1;
    myMouseScrollFrom = 0;
    myMouseScrollTo = 1;
    myElementWidth = myViewWidth / myElementsOnX;
    myElementHeight = myViewHeight / myElementsOnY;

    int x1 = myTopLeftX;
    int y1 = myTopleftY;
    int x2 = myTopLeftX + myElementWidth;
    int y2 = myElementHeight;

    std::vector<GridElement>::iterator cur = myVecBookshelfElements.begin();
    std::vector<GridElement>::iterator end = myVecBookshelfElements.end();

    for(; cur != end; ++cur) {
        (*cur).myTopLeft.x = x1;
        (*cur).myTopLeft.y = y1;
        (*cur).myBottomRight.x = x2;
        (*cur).myBottomRight.y = y2;

        x1 += myElementWidth;
        x2 += myElementWidth;

        if(x2 > myViewWidth + myTopLeftX)
        {
            x1 = myTopLeftX;
            x2 = myTopLeftX + myElementWidth;
            y1 += myElementHeight;
            y2 += myElementHeight;
        }
    }

    if(myVecBookshelfElements.size() > myRenderingElementsCount) {
        myScrollBarMaxPos = (myVecBookshelfElements.size() - myRenderingElementsCount) / myElementsOnX;
        ++myScrollBarMaxPos;
    }

    setScrollbarEnabled(VERTICAL, true);
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);

    myItFirstRendering = myItLastRendering = myVecBookshelfElements.begin();
    myItLastRendering += myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void GridView::setMode(GridView::ViewMode mode) {
    if(mode != myViewMode) {
        if(mode == GridView::WITHOUT_TAGS_MENU) {
            if(!myShelfvesMenu.isNull())
                myShelfvesMenu->myIsVisible = false;
            myTopLeftX = 0;
        }

        if(mode == GridView::WITH_TAGS_MENU) {
            if(!myShelfvesMenu.isNull()) {
                myShelfvesMenu->myIsVisible = true;
                myTopLeftX = myShelfvesMenu->myXOffset;
            }
        }

        myViewMode = mode;
    }

    if(myVecBookshelfElements.empty())
        updateView(mySortType);
    else {
        updateBookshelfElements();
        Fbookshelf::Instance().refreshWindow();
    }


}

void GridView::invertMode()
{
    if(myViewMode == GridView::WITHOUT_TAGS_MENU)
        setMode(GridView::WITH_TAGS_MENU);
    else
        setMode(GridView::WITHOUT_TAGS_MENU);
}

std::vector<GridElement>::iterator GridView::getSelectedElement() {
    return myItSelectedElement;
}


bool GridView::onStylusPress(int x, int y) {
    if(!myShelfvesMenu.isNull() && myShelfvesMenu->myIsVisible) {
        bool state = false;
        if(myShelfvesMenu->checkSelectedElementMenu(x, y, state)) {

            assert(myShelfvesMenu->myItSelectedActionCode >= myShelfvesMenu->myVecMenuStrings.begin() &&
                   myShelfvesMenu->myItSelectedActionCode < myShelfvesMenu->myVecMenuStrings.end());

            myCurrentShelf = (*(myShelfvesMenu->myItSelectedActionCode)).first;
            updateView(mySortType);
        }
    }

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        if(myContextMenu.myIsVisible) {
            bool state = false;
            if(myContextMenu.checkSelectedElementMenu(x, y, state)) {

                assert(myContextMenu.myItSelectedActionCode >= myContextMenu.myVecMenuStrings.begin() &&
                       myContextMenu.myItSelectedActionCode < myContextMenu.myVecMenuStrings.end());

                myContextMenu.myIsVisible = false;
                Fbookshelf::Instance().refreshWindow();
                Fbookshelf::Instance().doAction((*(myContextMenu.myItSelectedActionCode)).first);
                updateView(mySortType);
            }
            myContextMenu.myIsVisible = false;
            Fbookshelf::Instance().refreshWindow();
        }

        if((*it).checkBookOptions(x, y)) {
            myContextMenu.myIsVisible = true;
            myContextMenu.myTopLeft.x = (*it).myOptionsTopLeft.x -
                    (myContextMenu.myXOffset - ((*it).myOptionsBottomRight.x - (*it).myOptionsTopLeft.x)) - 1;
            myContextMenu.myTopLeft.y = (*it).myOptionsBottomRight.y;
            Fbookshelf::Instance().refreshWindow();
            break;
        }
    }

    return true;
}


bool GridView::onStylusRelease(int x, int y) {
    shared_ptr<ZLTime> current_time = new ZLTime();
    long delta = current_time->millisecondsFrom(*myTime);
    myTime = current_time;

    if(delta > DELAY_ON_CLICK_BOOK_MS)
        return true;

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {

        if(it == myItSelectedElement &&
           (*it).checkSelectedBook(x, y) &&
           !(*it).checkBookOptions(x, y) &&
           !myContextMenu.myIsVisible &&
           !myContextMenu.myIsSelected) {

            Fbookshelf::Instance().doAction(BookshelfActionCode::RUN_FBREADER);
            break;
        }
    }
    return true;

}



bool GridView::onStylusMove(int x, int y) {
    bool ElementMenuPrevState = myContextMenu.myIsSelected;
    bool ElementMenuStringStateChanged = false;

    if(myContextMenu.myIsVisible && myContextMenu.checkSelectedElementMenu(x, y, ElementMenuStringStateChanged)){
        myContextMenu.myIsSelected = true;
    }
    else
        myContextMenu.myIsSelected = false;

    if(myContextMenu.myIsSelected != ElementMenuPrevState || ElementMenuStringStateChanged)
        Fbookshelf::Instance().refreshWindow();

    if(!myShelfvesMenu.isNull()) {
        bool TagsMenuPrevState = myShelfvesMenu->myIsSelected;
        bool TagsMenuStateChanged = false;

        if(myShelfvesMenu->myIsVisible && myShelfvesMenu->checkSelectedElementMenu(x, y, TagsMenuStateChanged))
            myShelfvesMenu->myIsSelected = true;
        else
            myShelfvesMenu->myIsSelected = false;

        if(myShelfvesMenu->myIsSelected != TagsMenuPrevState || TagsMenuStateChanged)
            Fbookshelf::Instance().refreshWindow();
    }

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        bool SelectedPrevState = (*it).myIsSelected;
        bool MenuSelectedPrevState = (*it).myIsMenuSelected;

        if((*it).checkSelectedBook(x, y)) {
            myItSelectedElement = it;

            if((*it).checkBookOptions(x, y)) {
                (*it).myIsMenuSelected = true;
                (*it).myIsSelected = false;
            }
            else {
                (*it).myIsSelected = true;
                (*it).myIsMenuSelected = false;
            }

        }
        else {
            (*it).myIsSelected = false;
            (*it).myIsMenuSelected = false;
        }

        if(myContextMenu.myIsVisible) {
            (*it).myIsSelected = false;
            (*it).myIsMenuSelected = false;
        }

        if((*it).myIsSelected != SelectedPrevState || (*it).myIsMenuSelected != MenuSelectedPrevState)
            Fbookshelf::Instance().refreshWindow();
    }

    return true;
}



void GridView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {
    if(myContextMenu.myIsVisible) {
        myContextMenu.myIsVisible = false;
        Fbookshelf::Instance().refreshWindow();
    }

    if(from < myScrollBarPos) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }

    myScrollBarPos = from;
}

void GridView::onScrollbarStep(ZLView::Direction direction, int steps){
    if(myContextMenu.myIsVisible) {
        myContextMenu.myIsVisible = false;
        Fbookshelf::Instance().refreshWindow();
    }

    if(steps < 0) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }
}

void GridView::onScrollbarPageStep(ZLView::Direction direction, int steps){
    if(myContextMenu.myIsVisible) {
        myContextMenu.myIsVisible = false;
        Fbookshelf::Instance().refreshWindow();
    }

    if(steps < 0) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }
}


void GridView::onMouseScroll(bool forward) {

    if(myViewMode == GridView::WITHOUT_TAGS_MENU) {
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

    if(myViewMode == GridView::WITH_TAGS_MENU && !myShelfvesMenu.isNull()) {
        if(forward) {
            myShelfvesMenu->updateScrollDown();
        }
        else {
            myShelfvesMenu->updateScrollUp();
        }

        Fbookshelf::Instance().refreshWindow();
    }

}



void GridView::updateBookshelfElements() {
    if(myViewMode == GridView::WITHOUT_TAGS_MENU)
        myViewWidth = context().width();

    if(myViewMode == GridView::WITH_TAGS_MENU)
        myViewWidth = context().width() - myTopLeftX;

    myViewHeight = context().height();

    myElementWidth = myViewWidth / myElementsOnX;
    myElementHeight = myViewHeight / myElementsOnY;

    int x1 = myTopLeftX;
    int y1 = myTopleftY;
    int x2 = myTopLeftX + myElementWidth;
    int y2 = myElementHeight;

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
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



void GridView::updateScrollDown() {
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

void GridView::updateScrollUp()
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



void GridView::drawBookshelfElements() {
    if(myViewMode == GridView::WITHOUT_TAGS_MENU) {
        if(context().width() != myViewWidth || context().height() != myViewHeight) {
            updateBookshelfElements();
        }
    }

    if(myViewMode == GridView::WITH_TAGS_MENU) {
        if(context().width() != myViewWidth + myTopLeftX || context().height() != myViewHeight) {
            updateBookshelfElements();
        }
    }


    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).drawElement(context());
        (*it).drawOptions(context());
    }
}



void GridView::drawBackground() {
    context().setFillColor(myBackgroundColor);
    context().fillRectangle(0,0,context().width(),context().height());
}



const std::string &GridView::caption() const {
    return myCaption;
}



ZLColor GridView::backgroundColor() const {
    return myBackgroundColor;
}



void GridView::paint() {
    drawBackground();

    if(!myShelfvesMenu.isNull()) {
        if(myShelfvesMenu->myIsVisible) {
            myShelfvesMenu->checkFont();
            myShelfvesMenu->draw();
        }
    }

    drawBookshelfElements();
    if(myContextMenu.myIsVisible)
        myContextMenu.draw();

}
