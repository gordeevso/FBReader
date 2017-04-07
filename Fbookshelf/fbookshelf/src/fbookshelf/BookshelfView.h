#ifndef SIMPLEBLACKVIEW
#define SIMPLEBLACKVIEW

#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "UIElements.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

class ZLImageData;

class BookshelfView : public ZLView {

public:
    BookshelfView(ZLPaintContext &context);

    void setCaption(const std::string &caption);
    void init();
    shared_ptr<Book> getSelectedBook();
    void setSelectedBook(shared_ptr<Book> book);

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarStep(ZLView::Direction direction, int steps);
    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

private:
    void UpdateBookshelfElements();
    void UpdateScrollDown();
    void UpdateScrollUp();

    void DrawBookshelfElements();
    void DrawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;

private:


    struct ElementMenu
    {
        //TODO
    };

private:
    Point mStartPoint;
    Point mEndPoint;

    std::string mCaption;
    int mViewWidth;
    int mViewHeight;
    ZLColor mBackgroundColor;

    int mElementsOnX;
    int mElementsOnY;
    int mRenderingElementsCount;

    size_t mScrollBarPos;

    int mElementWidth;
    int mElementHeight;

    shared_ptr<Book> mSelectedBook;

    std::vector<BookshelfElement> mBookshelfElements;
    std::vector<BookshelfElement>::iterator mItFirstRendering;
    std::vector<BookshelfElement>::iterator mItLastRendering;

};


#endif // ZLAPPVIEW

