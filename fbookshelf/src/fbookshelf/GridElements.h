#ifndef GRIDELEMENTS
#define GRIDELEMENTS

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
              shared_ptr<ZLImageData>imagedata = 0);

    ~ImageRect()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
    void draw(ZLPaintContext & painter);

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
               int fontsize = 4);

    ~StringRect()
    {}

    void updateFont(int x1, int y1, int x2, int y2);
    void updatePosition(int x1, int y1, int x2, int y2);
    void draw();


    public:

        int mx;
        int my;
        int mxr;
        int myr;
        int mFontSize;
        std::string const & mStr;
        ZLPaintContext & painter;
};

struct GridElement
{
    GridElement(Point topLeft = Point(),
                     Point bottomRight = Point(),
                     Point opttopleft = Point(),
                     Point optbottomright = Point(),
                     ZLColor elementColor = ZLColor(),
                     ZLColor frameColor = ZLColor(),
                     shared_ptr<Book> book = 0,
                     shared_ptr<ZLImageData> imagedata = 0,
                     bool selected = false,
                     bool menuselected = false);

    ~GridElement()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
    void drawOptions(ZLPaintContext & painter);
    void drawElement(ZLPaintContext & painter);

    bool checkBookOptions(int x, int y);
    bool checkSelectedBook(int x, int y);


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

#endif

