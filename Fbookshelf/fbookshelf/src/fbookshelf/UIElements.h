#ifndef UIELEMENTS
#define UIELEMENTS

#include <string>

#include <ZLPaintContext.h>

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

struct Point{
    int x;
    int y;
    Point(int xx = 0, int yy = 0) : x(xx), y(yy)
    {}
};


struct ImageRect
{
    ImageRect(int x = 0, int y = 0,
              int width = 40, int height = 40,
              shared_ptr<ZLImageData>imagedata = 0)
        : mx(x),
          my(y),
          mWidth(width),
          mHeight(height),
          mImageData(imagedata)
    {}

    ~ImageRect()
    {}

    void UpdatePosition(int x1, int y1, int x2, int y2)
    {
        float beFactor = (x2 - x1)/(y2 - y1);

        mx = x1 + (x2 - x1) * 0.05 ;
        my = y2 - (y2 - y1) * 0.25 + beFactor * (y2 - y1) * 0.05;

        mHeight = (y2 - y1)*0.75;
        mWidth = mHeight/mHWFactor;

    }

    void Draw(ZLPaintContext & painter)
    {


        painter.drawImage(mx,
                          my,
                          *mImageData,
                          mWidth,
                          mHeight,
                          ZLPaintContext::SCALE_FIT_TO_SIZE);

        painter.setColor(ZLColor(0,0,0));
        painter.drawLine(mx, my-mHeight, mx+mWidth, my-mHeight);
        painter.drawLine(mx+mWidth, my-mHeight, mx+mWidth, my);
        painter.drawLine(mx+mWidth, my, mx, my);
        painter.drawLine(mx, my, mx, my-mHeight);


    }

public:
    int mx;
    int my;
    int mWidth;
    int mHeight;
    shared_ptr<ZLImageData> mImageData;
    float mHWFactor;
};

struct StringRect
{
    StringRect(const std::string & str,
               ZLPaintContext & paintcontext,
               int x1 = 0, int y1 = 0,
               int x2 = 0, int y2 = 0,
               int fontsize = 4)
        : mStr(str),
          painter(paintcontext),
          mx(x1),
          my(y1),
          mxr(x2),
          myr(y2),
          mFontSize(fontsize)
    {}

    ~StringRect()
    {}

    void UpdateFont(int x1, int y1, int x2, int y2)
    {
        mFontSize = (y2 - y1) * 0.04;

        const FBTextStyle &style = FBTextStyle::Instance();
        painter.setFont(style.fontFamily(), mFontSize, style.bold(), style.italic());
    }

    void UpdatePosition(int x1, int y1, int x2, int y2)
    {
        UpdateFont(x1, y1, x2, y2);

        mx = x1 + (x2 - x1) * 0.05;
        my = y2 - (y2 - y1) * 0.075 - mFontSize;

        mxr = x2 - (x2 - x1) * 0.05;
        myr = y2 - (y2 - y1) * 0.075;
    }

    void Draw()
    {
        painter.setFillColor(ZLColor(255,255,255));
        painter.fillRectangle(mx-1, my-2, mxr+1, myr+1);

        painter.setColor(FBOptions::Instance().RegularTextColorOption.value());

        painter.drawString(mx,
                           myr,
                           mStr.c_str(),
                           mStr.size(),
                           true);

        painter.setColor(ZLColor(0,0,0));
        painter.drawLine(mx-1, my-2, mxr+1, my-2);
        painter.drawLine(mxr+1, my-2, mxr+1, myr+1);
        painter.drawLine(mxr+1, myr+1, mx-1, myr+1);
        painter.drawLine(mx-1, myr+1, mx-1, my-2);
    }


    public:
        int mx;
        int my;
        int mxr;
        int myr;
        int mFontSize;
        std::string const & mStr;
        ZLPaintContext & painter;
        float mHWFactor;
};

struct BookshelfElement
{
    BookshelfElement(Point topLeft = Point(),
                     Point bottomRight = Point(),
                     Point opttopleft = Point(),
                     Point optbottomright = Point(),
                     ZLColor elementColor = ZLColor(),
                     ZLColor frameColor = ZLColor(),
                     shared_ptr<Book> book = 0,
                     shared_ptr<ZLImageData> imagedata = 0,
                     bool selected = false,
                     bool menuselected = false)
        : mTopLeft(topLeft),
          mBottomRight(bottomRight),
          mOptionsTopLeft(opttopleft),
          mOptionsBottomRight(optbottomright),
          mElementColor(elementColor),
          mFrameColor(frameColor),
          mTitleString(),
          mTitleImage(),
          mIsSelected(selected),
          mIsMenuSelected(menuselected)
    {}

    ~BookshelfElement()
    {}

    void UpdatePosition(int x1, int y1, int x2, int y2)
    {
        mTopLeft.x = x1;
        mTopLeft.y = y1;
        mBottomRight.x = x2;
        mBottomRight.y = y2;

        float beFactor = (x2 - x1)/(y2 - y1);
        int options_width = (x2 - x1) * 0.1;
        int options_height = (y2 - y1) * 0.1;

        mOptionsTopLeft.x = x2 - options_width - (x2 - x1) * 0.05;
        mOptionsTopLeft.y = y1 + beFactor * (y2 - y1) * 0.05;
        mOptionsBottomRight.x = mOptionsTopLeft.x + options_width;
        mOptionsBottomRight.y = mOptionsTopLeft.y + options_height;

        mTitleImage.UpdatePosition(x1, y1, x2, y2);
        mTitleString->UpdatePosition(x1, y1, x2, y2);
    }

    void DrawOptions(ZLPaintContext & painter)
    {
        if(!mIsMenuSelected)
            painter.setFillColor(ZLColor(255,255,255));
        else
            painter.setFillColor(ZLColor(210,210,90));

        painter.fillRectangle(mOptionsTopLeft.x, mOptionsTopLeft.y, mOptionsBottomRight.x, mOptionsBottomRight.y);

        painter.setColor(ZLColor(100,100,100));
        painter.drawLine(mOptionsTopLeft.x, mOptionsTopLeft.y, mOptionsBottomRight.x, mOptionsTopLeft.y);
        painter.drawLine(mOptionsBottomRight.x, mOptionsTopLeft.y, mOptionsBottomRight.x, mOptionsBottomRight.y);
        painter.drawLine(mOptionsBottomRight.x, mOptionsBottomRight.y, mOptionsTopLeft.x, mOptionsBottomRight.y);
        painter.drawLine(mOptionsTopLeft.x, mOptionsBottomRight.y, mOptionsTopLeft.x, mOptionsTopLeft.y);

    }

    void DrawElement(ZLPaintContext & painter)
    {
        if(!mIsSelected)
            painter.setFillColor(mElementColor);
        else
            painter.setFillColor(ZLColor(210,210,90));

        painter.fillRectangle(mTopLeft.x, mTopLeft.y, mBottomRight.x, mBottomRight.y);

        painter.setColor(mFrameColor);
        painter.drawLine(mTopLeft.x, mTopLeft.y, mBottomRight.x, mTopLeft.y);
        painter.drawLine(mBottomRight.x, mTopLeft.y, mBottomRight.x, mBottomRight.y);
        painter.drawLine(mBottomRight.x, mBottomRight.y, mTopLeft.x, mBottomRight.y);
        painter.drawLine(mTopLeft.x, mBottomRight.y, mTopLeft.x, mTopLeft.y);

        mTitleImage.Draw(painter);
        mTitleString->Draw();

        DrawOptions(painter);
    }

    bool CheckBookOptions(int x, int y)
    {
        return (x > mOptionsTopLeft.x &&
                x < mOptionsBottomRight.x &&
                y > mOptionsTopLeft.y &&
                y < mOptionsBottomRight.y);
    }

    bool CheckSelectedBook(int x, int y)
    {
        return (x > mTopLeft.x &&
                x < mBottomRight.x &&
                y > mTopLeft.y &&
                y < mBottomRight.y);
    }


public:
    Point mTopLeft;
    Point mBottomRight;
    Point mOptionsTopLeft;
    Point mOptionsBottomRight;
    ZLColor mElementColor;
    ZLColor mFrameColor;
    shared_ptr<Book> mBook;
    shared_ptr<StringRect> mTitleString;
    ImageRect mTitleImage;
    bool mIsSelected;
    bool mIsMenuSelected;
};

#endif // UIELEMENTS

