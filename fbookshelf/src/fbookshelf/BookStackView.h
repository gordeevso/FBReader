#ifndef BOOKSTACKVIEW
#define BOOKSTACKVIEW

#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "BookStackElements.h"
#include "BookshelfMenu.h"
#include "GridContextMenu.h"

#include "../library/BookshelfModel.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

class ZLImageData;

class BookStackView : public ZLView {

public:
    BookStackView(ZLPaintContext &context);
    const ZLTypeId &typeId() const;

    enum ViewMode {
        WITH_TAGS_MENU = 0,
        WITHOUT_TAGS_MENU = 1
    };

    void setCaption(const std::string &caption);
    void updateView(BookshelfModel::SortType);
    void setMode(ViewMode);
    void invertMode();

  std::vector<BookElement>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);
    bool onStylusMovePressed(int x, int y); 
    void onMouseScroll(bool forward); // what is it?
    
    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);
    void onScrollbarStep(ZLView::Direction direction, int steps);

public:
    static const ZLTypeId TYPE_ID;

private:
    void createBookElements();
    void updateBookElements();
    
    void updateBookSlots();
    
    void updateScrollDown();
    void updateScrollUp();

    void drawBookElements();
    void drawBookshelfs();
    void drawBookSlots();
    void drawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;

private:
    ViewMode myViewMode;
    BookshelfModel::SortType mySortType;

    std::string myCaption;
    int myViewWidth;
    int myViewHeight;
    ZLColor myBackgroundColor;
    
    int bookElementWidth;
    int bookElementHeight;
    int bookHorizontalDistance;
    int bookVerticalDistance;
    
    int prevX1;
    
    size_t myScrollBarPos;
    size_t myScrollBarMaxPos;
    size_t myMouseScrollFrom;
    size_t myMouseScrollTo;
    
    int leftBorder;
    int rightBorder;

    std::vector<BookElement> myBookElements;
    std::vector<BookElement>::iterator mySelectedBookElement;
    std::vector<BookElement>::iterator myCapturedBookElement;
    std::vector<BookSlot>::iterator mySelectedSlot;
    std::vector<Shelf> myShelfs;
    std::vector<BookSlot> myBookSlots;


};


#endif