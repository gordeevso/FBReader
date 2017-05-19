#include <algorithm>
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

//const int ELEMENTS_ON_X = 3;
//const int ELEMENTS_ON_Y = 3;

//const ZLColor ELEMENT_COLOR = ZLColor(190,190,190);
//const ZLColor ELEMENT_FRAME_COLOR = ZLColor(250,250,250);
//const ZLColor ELEMENT_COLOR_ON_SELECT = ZLColor(210,210,210);
const ZLColor BOOKSTACK_BACKGROUND_COLOR = ZLColor(210, 180, 140);

const std::string BOOKSTACK_CAPTION = "BookStack";

BookStackView::BookStackView(ZLPaintContext &context) : ZLView(context),
                                              myViewMode(BookStackView::WITHOUT_TAGS_MENU),
                                              mySortType(BookshelfModel::SORT_BY_AUTHOR),
//                                              myTopLeftX(0),
//                                              myTopleftY(0),
                                              myCaption(BOOKSTACK_CAPTION),
                                              myViewWidth(context.width()),
                                              myViewHeight(context.height()),
                                              myBackgroundColor(BOOKSTACK_BACKGROUND_COLOR),
//                                              myElementsOnX(ELEMENTS_ON_X),
//                                              myElementsOnY(ELEMENTS_ON_Y),
//                                              myRenderingElementsCount(myElementsOnX * myElementsOnY),
                                              myScrollBarPos(0),
                                              myScrollBarMaxPos(1),
                                              myMouseScrollFrom(0),
                                              myMouseScrollTo(1),
//                                              myElementWidth(200),
//                                              myElementHeight(250),
                                              myVecBookshelfElements(),
                                              myVecShelfs(),
                                              myItSelectedElement(myVecBookshelfElements.end()),
                                              myItFirstRendering(myVecBookshelfElements.begin()),
                                              myItLastRendering(myVecBookshelfElements.end()),
                                              myElementMenu(context)
{
    std::vector<std::string> tags;
    Tag::collectTagNames(tags);
    myTagsMenu = new BookshelfMenu(context, tags);
}

const ZLTypeId BookStackView::TYPE_ID(ZLView::TYPE_ID);

const ZLTypeId &BookStackView::typeId() const {
    return TYPE_ID;
}

void BookStackView::updateView(BookshelfModel::SortType sort_type) {
    
    myVecShelfs.clear();
    myViewWidth = context().width();
    myViewHeight = context().height();

    int x1 = 50;
    int y1 = 50;
    int x2 = x1 + 200;
    int y2 = y1 + 250;
    
    BookOnShelf element;
    
    std::vector<shared_ptr<Book> > & library = BookshelfModel::Instance().getLibrary(sort_type);
    std::vector<shared_ptr<Book> >::const_iterator it = library.begin();
    std::vector<shared_ptr<Book> >::const_iterator itEnd = library.end();
    
        for(; it != itEnd; ++it) {
        x1 = x2 + 50;
        x2 = x1 + 200;
        
        if(x2 > myViewWidth)
        {
            x1 = 50;
            x2 = x1 + 200;
            y1 += 250;
            y2 += 250;
        }
    }
    if (y2 <= myViewHeight) {
        myScrollBarMaxPos = 1;
        myMouseScrollFrom = 0;
        myMouseScrollTo = 1;
    } else {
        
        myScrollBarMaxPos = y2 - 250;
        myMouseScrollTo = (myMouseScrollFrom + myViewHeight);
    }
    x1 = 50;
    y1 = 50 - myMouseScrollFrom;
    x2 = x1 + 200;
    y2 = y1 + 250;
    
    it = library.begin();
    itEnd = library.end();
    size_t i = 0;
    myVecShelfs.push_back(Shelf(y2 - 50, myViewWidth));
   
            
    myVecShelfs[i].myShelfName = new StringRect((*it)->title(), context());
    i++;
    for(; it != itEnd; ++it) {
        BookModel model(*it);
        element.myTitleImage.myImageData = ZLImageManager::Instance().imageData(*(model.imageMap().begin()->second));
        element.myTitleImage.myHWFactor = (float)element.myTitleImage.myImageData->height() / element.myTitleImage.myImageData->width();
        element.myBook = *it;
        
        myVecBookshelfElements.push_back(element);

        x1 = x2 + 50;
        x2 = x1 + 200;
        
        if(x2 > myViewWidth)
        {
            x1 = 50;
            x2 = x1 + 200;
            y1 += 250;
            y2 += 250;
            myVecShelfs.push_back(Shelf(y2 - 50, myViewWidth));
            myVecShelfs[i].myShelfName = new StringRect((*it)->title(), context());
            i++;
        }
    }
    myItSelectedElement = myVecBookshelfElements.begin();
    setScrollbarEnabled(VERTICAL, true);
    setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
    myItFirstRendering = myVecBookshelfElements.begin();
    myItLastRendering = myVecBookshelfElements.end();
    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void BookStackView::setMode(BookStackView::ViewMode mode) {
    if(mode != myViewMode) {
        if(mode == BookStackView::WITHOUT_TAGS_MENU) {
            if(!myTagsMenu.isNull())
                myTagsMenu->myIsVisible = false;
//            myTopLeftX = 0;
        }

        if(mode == BookStackView::WITH_TAGS_MENU) {
            if(!myTagsMenu.isNull()) {
                std::vector<std::string> tags;
                Tag::collectTagNames(tags);
                myTagsMenu->reloadTags(tags);
                myTagsMenu->myIsVisible = true;
//                myTopLeftX = myTagsMenu->myXOffset;
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

void BookStackView::invertMode()
{
    if(myViewMode == BookStackView::WITHOUT_TAGS_MENU)
        setMode(BookStackView::WITH_TAGS_MENU);
    else
        setMode(BookStackView::WITHOUT_TAGS_MENU);
}

std::vector<BookOnShelf>::iterator BookStackView::getSelectedElement() {
    return myItSelectedElement;
}


bool BookStackView::onStylusPress(int x, int y) {
    if(!myTagsMenu.isNull() && myTagsMenu->myIsVisible) {
        bool state = false;
        if(myTagsMenu->checkSelectedElementMenu(x, y, state)) {

            assert(myTagsMenu->myItSelectedActionCode >= myTagsMenu->myVecMenuStrings.begin() &&
                   myTagsMenu->myItSelectedActionCode < myTagsMenu->myVecMenuStrings.end());

           // myTagsMenu->myIsVisible = false;
            std::cout << (*(myTagsMenu->myItSelectedActionCode)).first << "\n";
        }
    }

    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
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

//        if((*it).checkBookOptions(x, y)) {
//            myElementMenu.myIsVisible = true;
//            myElementMenu.myTopLeft.x = (*it).myOptionsTopLeft.x -
//                    (myElementMenu.myXOffset - ((*it).myOptionsBottomRight.x - (*it).myOptionsTopLeft.x)) - 1;
//            myElementMenu.myTopLeft.y = (*it).myOptionsBottomRight.y;
//            Fbookshelf::Instance().refreshWindow();
//            break;
//        }
    }

    return true;
}


bool BookStackView::onStylusRelease(int x, int y) {
    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {

        if(it == myItSelectedElement &&
           (*it).checkSelectedBook(x, y) &&
//           !(*it).checkBookOptions(x, y) &&
           !myElementMenu.myIsVisible &&
           !myElementMenu.myIsSelected) {

            Fbookshelf::Instance().doAction(BookshelfActionCode::RUN_FBREADER);
            break;
        }
    }
    return true;
}



bool BookStackView::onStylusMove(int x, int y) {
    bool ElementMenuPrevState = myElementMenu.myIsSelected;
    bool ElementMenuStringStateChanged = false;

    if(myElementMenu.myIsVisible && myElementMenu.checkSelectedElementMenu(x, y, ElementMenuStringStateChanged)){
        myElementMenu.myIsSelected = true;
    }
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

    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        bool SelectedPrevState = (*it).myIsSelected;
//        bool MenuSelectedPrevState = (*it).myIsMenuSelected;

        if((*it).checkSelectedBook(x, y)) {
            myItSelectedElement = it;

//            if((*it).checkBookOptions(x, y)) {
//                (*it).myIsMenuSelected = true;
//                (*it).myIsSelected = false;
//            }
//            else {
//                (*it).myIsSelected = true;
//                (*it).myIsMenuSelected = false;
//            }

        }
        else {
            (*it).myIsSelected = false;
//            (*it).myIsMenuSelected = false;
        }

        if(myElementMenu.myIsVisible) {
            (*it).myIsSelected = false;
//            (*it).myIsMenuSelected = false;
        }

        if((*it).myIsSelected != SelectedPrevState/* || (*it).myIsMenuSelected != MenuSelectedPrevState*/)
            Fbookshelf::Instance().refreshWindow();
    }

    return true;
}





void BookStackView::onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to) {

    if(from < myScrollBarPos) {
        updateScrollUp();
    }
    else {
        updateScrollDown();
    }

    myScrollBarPos = from;
}



void BookStackView::onScrollbarPageStep(ZLView::Direction direction, int steps){
    size_t diff = myMouseScrollTo - myMouseScrollFrom;

    if(steps < 0) {
    myMouseScrollFrom -= diff;
    myMouseScrollTo -= diff;
        updateScrollUp();
    }
    else {
            myMouseScrollFrom += diff;
    myMouseScrollTo += diff;
        updateScrollDown();
    }
}


void BookStackView::onMouseScroll(bool forward) {
    size_t scrollSpeed = 30;
    if(myViewMode == BookStackView::WITHOUT_TAGS_MENU) {
        if(forward && myMouseScrollTo < myScrollBarMaxPos) {
            myMouseScrollFrom += scrollSpeed;
            myMouseScrollTo += scrollSpeed;
            onScrollbarMoved(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
            setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
        }

        if(!forward && myMouseScrollFrom > 0) {
            myMouseScrollFrom -= scrollSpeed;
            myMouseScrollTo -= scrollSpeed;
            onScrollbarMoved(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
            setScrollbarParameters(VERTICAL, myScrollBarMaxPos, myMouseScrollFrom, myMouseScrollTo);
        }
    }

    if(myViewMode == BookStackView::WITH_TAGS_MENU && !myTagsMenu.isNull()) {
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



void BookStackView::updateBookshelfElements() {
    myVecShelfs.clear();
    myViewWidth = context().width();
    myViewHeight = context().height();
    
    int x1 = 50;
    int y1 = 50;
    int x2 = x1 + 200;
    int y2 = y1 + 250;

    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        std::cout << "myTitleImage.myHeight: " << (*it).myTitleImage.myWidth << std::endl;
        std::cout << "myTitleImage.myX: " << (*it).myTitleImage.myX << std::endl;
        std::cout << "myTitleImage.myY: " << (*it).myTitleImage.myY << std::endl;
        x1 = x2 + 10;
        x2 = x1 + 200;
        if(x2 > myViewWidth) {
            x1 = 50;
            x2 = x1 + 200;
            y1 += 250;
            y2 += 250;
//            myVecShelfs.push_back(Shelf((i + 1) * 350, myViewWidth));
//            i++;
        }
    }
    if (y2 <= myViewHeight) {
        myScrollBarMaxPos = 1;
        myMouseScrollFrom = 0;
        myMouseScrollTo = 1;
    } else {
        
        myScrollBarMaxPos = y2 - 250;
        myMouseScrollTo = (myMouseScrollFrom + myViewHeight);
    }
     
    std::cout << "y2: " << y2 << std::endl;
    std::cout << "myViewHeight: " << myViewHeight << std::endl;
    std::cout << "myScrollBarMaxPos: " << myScrollBarMaxPos << std::endl;
    std::cout << "myMouseScrollFrom " << myMouseScrollFrom << std::endl;
    std::cout << "myMouseScrollTo " << myMouseScrollTo << std::endl;

    x1 = 50;
    y1 = 50 - myMouseScrollFrom;
    x2 = x1 + 200;
    y2 = y1 + 250;
    size_t i = 0;
    myVecShelfs.push_back(Shelf(y2 - 50, myViewWidth));
    std::vector<BookOnShelf>::iterator jt = myItFirstRendering;
    myVecShelfs[i].myShelfName = new StringRect((*jt).myBook->title(), context());
    i++;

//    element.myTitleString = new StringRect(element.myBook->title(), context());

    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).updatePosition(x1, y1, x2, y2);
        x1 = x2 + 10;
        x2 = x1 + 200;
//        std::cout << "x2 = " << x2 << std::endl;
//        std::cout << "myViewWidth = " << myViewWidth << std::endl << std::endl;
        if(x2 > myViewWidth) {
            x1 = 50;
            x2 = x1 + 200;
            y1 += 250;
            y2 += 250;
            myVecShelfs.push_back(Shelf(y2 - 50, myViewWidth));
            myVecShelfs[i].myShelfName = new StringRect((*it).myBook->title(), context());
            i++;

        }
//        std::cout << (*it).myTitleImage.myWidth << " " << (*it).myTitleImage.myHeight;
//        std::cout << std::endl;
    }
}



void BookStackView::updateScrollDown() {
//    if(myItLastRendering + myElementsOnX <= myVecBookshelfElements.end()){
//        myItFirstRendering += myElementsOnX;
//        myItLastRendering += myElementsOnX;
//    }
//    else {
//        myItLastRendering = myItFirstRendering = myVecBookshelfElements.end();
//        myItFirstRendering -= myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();
//    }
//
//    assert(myItFirstRendering <= myItLastRendering);
//    assert(myItFirstRendering >= myVecBookshelfElements.begin() && myItFirstRendering < myVecBookshelfElements.end());
//    assert(myItLastRendering > myVecBookshelfElements.begin() && myItLastRendering <= myVecBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}

void BookStackView::updateScrollUp()
{
//    if(myItFirstRendering - myElementsOnX >= myVecBookshelfElements.begin()){
//        myItFirstRendering -= myElementsOnX;
//        myItLastRendering -= myElementsOnX;
//    }
//    else {
//        myItFirstRendering = myItLastRendering = myVecBookshelfElements.begin();
//        myItLastRendering += myVecBookshelfElements.size() > myRenderingElementsCount ? myRenderingElementsCount : myVecBookshelfElements.size();
//    }
//
//    assert(myItFirstRendering <= myItLastRendering);
//    assert(myItFirstRendering >= myVecBookshelfElements.begin() && myItFirstRendering < myVecBookshelfElements.end());
//    assert(myItLastRendering > myVecBookshelfElements.begin() && myItLastRendering <= myVecBookshelfElements.end());

    updateBookshelfElements();
    Fbookshelf::Instance().refreshWindow();
}



void BookStackView::drawBookshelfElements() {
    if(myViewMode == BookStackView::WITHOUT_TAGS_MENU) {
        if(context().width() != myViewWidth || context().height() != myViewHeight) {
            updateBookshelfElements();
        }
    }

    if(myViewMode == BookStackView::WITH_TAGS_MENU) {
        if(context().width() != myViewWidth /*+ myTopLeftX */|| context().height() != myViewHeight) {
            updateBookshelfElements();
        }
    }


    for(std::vector<BookOnShelf>::iterator it = myItFirstRendering; it != myItLastRendering; ++it) {
        (*it).drawElement(context());
    }
}

void BookStackView::drawBookshelfs() {
    for(std::vector<Shelf>::iterator it = myVecShelfs.begin(); it != myVecShelfs.end(); ++it) {
        (*it).draw(context());
    }
}


void BookStackView::drawBackground() {
    context().setFillColor(myBackgroundColor);
    context().fillRectangle(0,0,context().width(),context().height());
    context().setFillColor(ZLColor(153, 131, 102));
    context().fillRectangle(0, 0, 70, myViewHeight);
    context().fillRectangle(myViewWidth, 0, myViewWidth - 70, myViewHeight);
    context().setColor(ZLColor(0, 0, 0));
    context().drawLine(70, 0, 70, myViewHeight);
    context().drawLine(myViewWidth - 70, 0, myViewWidth - 70, myViewHeight);
}



const std::string &BookStackView::caption() const {
    return myCaption;
}



ZLColor BookStackView::backgroundColor() const {
    return myBackgroundColor;
}



void BookStackView::paint() {
    drawBackground();

    if(!myTagsMenu.isNull()) {
        if(myTagsMenu->myIsVisible) {
            myTagsMenu->checkFont();
            myTagsMenu->draw();
        }
    }
    drawBookshelfs();
    drawBookshelfElements();
    if(myElementMenu.myIsVisible)
        myElementMenu.draw();

}
