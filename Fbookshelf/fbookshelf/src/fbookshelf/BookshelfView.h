#ifndef SIMPLEBLACKVIEW
#define SIMPLEBLACKVIEW

#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>

#include "../library/Book.h"
#include "../options/FBOptions.h"

class ZLImageData;

class BookshelfView : public ZLView {

public:
    BookshelfView(ZLPaintContext &context);

    void setCaption(const std::string &caption);
    void init();

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);
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
    struct Point{
        int x;
        int y;
        Point(int xx = 0, int yy = 0) : x(xx), y(yy)
        {}
    } mStartPoint, mEndPoint;

    struct BookshelfElement
    {
        BookshelfElement(Point topLeft = Point(),
                         Point bottomRight = Point(),
                         ZLColor elementColor = ZLColor(),
                         ZLColor frameColor = ZLColor(),
                         shared_ptr<Book> book = 0,
                         const shared_ptr<ZLImageData> imagedata = 0)
            : mTopLeft(topLeft),
              mBottomRight(bottomRight),
              mElementColor(elementColor),
              mFrameColor(frameColor),
              mBook(book),
              mImageData(imagedata)
        {}

        ~BookshelfElement()
        {}

        void UpdatePosition(int x1, int y1, int x2, int y2)
        {
            mTopLeft.x = x1;
            mTopLeft.y = y1;
            mBottomRight.x = x2;
            mBottomRight.y = y2;
        }

        void DrawElement(ZLPaintContext & painter, int fontSize) const
        {
            painter.setFillColor(mElementColor);
            painter.fillRectangle(mTopLeft.x, mTopLeft.y, mBottomRight.x, mBottomRight.y);

            painter.setColor(mFrameColor);
            painter.drawLine(mTopLeft.x, mTopLeft.y, mBottomRight.x, mTopLeft.y);
            painter.drawLine(mBottomRight.x, mTopLeft.y, mBottomRight.x, mBottomRight.y);
            painter.drawLine(mBottomRight.x, mBottomRight.y, mTopLeft.x, mBottomRight.y);
            painter.drawLine(mTopLeft.x, mBottomRight.y, mTopLeft.x, mTopLeft.y);

            painter.drawImage(mTopLeft.x +  (mBottomRight.x - mTopLeft.x) / 4,
                              mBottomRight.y - fontSize,
                              *mImageData,
                              mBottomRight.x - mTopLeft.x,
                              mBottomRight.y - mTopLeft.y - fontSize,
                              ZLPaintContext::SCALE_FIT_TO_SIZE);

            painter.setColor(
                        //highlighted() ?
                //FBOptions::Instance().colorOption(ZLTextStyle::HIGHLIGHTED_TEXT).value() :
                FBOptions::Instance().RegularTextColorOption.value());


            painter.drawString(mTopLeft.x +  (mBottomRight.x - mTopLeft.x) / 4,
                               mBottomRight.y,
                               mBook->title().c_str(),
                               mBook->title().size(),
                               true);

        }


    public:
        Point mTopLeft;
        Point mBottomRight;
        ZLColor mElementColor;
        ZLColor mFrameColor;
        shared_ptr<Book> mBook;
        shared_ptr<ZLImageData> mImageData;

    };

private:

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
    int mFontSize;

    std::vector<std::pair<std::string, BookshelfElement> > mBookshelfElements;
    std::vector<std::pair<std::string, BookshelfElement> >::iterator mItFirstRendering;
    std::vector<std::pair<std::string, BookshelfElement> >::iterator mItLastRendering;

};


#endif // ZLAPPVIEW

