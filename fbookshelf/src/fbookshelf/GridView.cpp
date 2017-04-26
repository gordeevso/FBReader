#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "GridView.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

#include "../bookmodel/BookModel.h"
#include "../options/FBTextStyle.h"
#include "../library/BookshelfModel.h"
#include "../library/Tag.h"

const int ELEMENTS_ON_X = 3;
const int ELEMENTS_ON_Y = 3;

const ZLColor ELEMENT_COLOR = ZLColor(190,190,190);
const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BACKGROUND_COLOR = ZLColor(255,255,255);

const std::string CAPTION = "Grid";

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
                                              myElementMenu(context)
{
    std::vector<std::string> tags;
    Tag::collectTagNames(tags);
    myTagsMenu = new TagsMenu(context, tags);
}

const ZLTypeId GridView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &GridView::typeId() const {
    return TYPE_ID;
}

void GridView::updateView(BookshelfModel::SortType sort_type) {

    if(mySortType != sort_type || myVecBookshelfElements.empty()) {
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

        for(; it != itEnd; ++it) {

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

        myElementMenu.myIsVisible = false;

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

void GridView::setMode(GridView::ViewMode mode)
{
    if(mode != myViewMode) {
        if(mode == GridView::WITHOUT_TAGS_MENU) {
            if(!myTagsMenu.isNull())
                myTagsMenu->myIsVisible = false;
            myTopLeftX = 0;
        }

        if(mode == GridView::WITH_TAGS_MENU) {
            if(!myTagsMenu.isNull()) {
                std::vector<std::string> tags;
                Tag::collectTagNames(tags);
                myTagsMenu->reloadTags(tags);
                myTagsMenu->myIsVisible = true;
                myTopLeftX = myTagsMenu->myXOffset;
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
    if(!myTagsMenu.isNull() && myTagsMenu->myIsVisible) {
        bool state = false;
        if(myTagsMenu->checkSelectedElementMenu(x, y, state)) {

            assert(myTagsMenu->myItSelectedActionCode >= myTagsMenu->myVecMenuStrings.begin() &&
                   myTagsMenu->myItSelectedActionCode < myTagsMenu->myVecMenuStrings.end());

           // myTagsMenu->myIsVisible = false;
            std::cout << (*(myTagsMenu->myItSelectedActionCode)).first << "\n";
        }
    }

    for(std::vector<GridElement>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        if(myElementMenu.myIsVisible) {
            bool state = false;
            if(myElementMenu.checkSelectedElementMenu(x, y, state)) {

                assert(myElementMenu.myItSelectedActionCode >= myElementMenu.myVecMenuStrings.begin() &&
                       myElementMenu.myItSelectedActionCode < myElementMenu.myVecMenuStrings.end());

                myElementMenu.myIsVisible = false;
                Fbookshelf::Instance().refreshWindow();
                Fbookshelf::Instance().doAction((*(myElementMenu.myItSelectedActionCode)).first);
            }
            myElementMenu.myIsVisible = false;
            Fbookshelf::Instance().refreshWindow();
        }

        if((*it).checkBookOptions(x, y)) {
            myElementMenu.myIsVisible = true;
            myElementMenu.myTopLeft.x = (*it).myOptionsTopLeft.x -
                    (myElementMenu.myXOffset - ((*it).myOptionsBottomRight.x - (*it).myOptionsTopLeft.x)) - 1;
            myElementMenu.myTopLeft.y = (*it).myOptionsBottomRight.y;
            Fbookshelf::Instance().refreshWindow();
            break;
        }
    }

    return true;
}


bool GridView::onStylusMovePressed(int x, int y) {

    return true;
}

bool GridView::onStylusRelease(int x, int y)
{
    if((*myItSelectedElement).checkSelectedBook(x, y)) {
        system(("FBReader " + (*myItSelectedElement).myBook->file().physicalFilePath() + "&").c_str());
    }
    return true;
}



bool GridView::onStylusMove(int x, int y) {
    bool ElementMenuPrevState = myElementMenu.myIsSelected;
    bool ElementMenuStringStateChanged = false;

    if(myElementMenu.myIsVisible && myElementMenu.checkSelectedElementMenu(x, y, ElementMenuStringStateChanged))
        myElementMenu.myIsSelected = true;
    else
        myElementMenu.myIsSelected = false;

    if(myElementMenu.myIsSelected != ElementMenuPrevState || ElementMenuStringStateChanged)
        Fbookshelf::Instance().refreshWindow();

    if(!myTagsMenu.isNull()) {
        bool TagsMenuPrevState = myTagsMenu->myIsSelected;
        bool TagsMenuStateChanged = false;

        if(myTagsMenu->myIsVisible && myTagsMenu->checkSelectedElementMenu(x, y, TagsMenuStateChanged))
            myTagsMenu->myIsSelected = true;
        else
            myTagsMenu->myIsSelected = false;

        if(myTagsMenu->myIsSelected != TagsMenuPrevState || TagsMenuStateChanged)
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

        if(myElementMenu.myIsVisible) {
            (*it).myIsSelected = false;
            (*it).myIsMenuSelected = false;
        }

        if((*it).myIsSelected != SelectedPrevState || (*it).myIsMenuSelected != MenuSelectedPrevState)
            Fbookshelf::Instance().refreshWindow();
    }
}


//What is it?
void GridView::onScrollbarStep(ZLView::Direction direction, int steps) {
    std::cout << "onscrollstep\n";
}



void GridView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {
    if(myElementMenu.myIsVisible) {
        myElementMenu.myIsVisible = false;
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



void GridView::onScrollbarPageStep(ZLView::Direction direction, int steps){
    if(myElementMenu.myIsVisible) {
        myElementMenu.myIsVisible = false;
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

    if(myViewMode == GridView::WITH_TAGS_MENU && !myTagsMenu.isNull()) {
        ;
        if(forward) {
            myTagsMenu->updateScrollDown();
        }
        else {
            myTagsMenu->updateScrollUp();
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

    if(!myTagsMenu.isNull()) {
        if(myTagsMenu->myIsVisible) {
            myTagsMenu->checkFont();
            myTagsMenu->draw();
        }
    }

    drawBookshelfElements();
    if(myElementMenu.myIsVisible)
        myElementMenu.draw();

}
