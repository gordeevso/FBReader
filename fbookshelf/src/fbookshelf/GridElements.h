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

    int myX;
    int myY;
    int myWidth;
    int myHeight;
    shared_ptr<ZLImageData> myImageData;
    float myHWFactor;
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

    void updateFont(int y1, int y2);
    void updatePosition(int x1, int y1, int x2, int y2);
    void draw();
    void divideStr(std::string & s1, std::string & s2);

    public:
        std::string const & myRefStr;
        ZLPaintContext & myRefPainter;
        int myX;
        int myY;
        int myXr;
        int myYr;
        int myFontSize;

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
    Point myTopLeft;
    Point myBottomRight;
    Point myOptionsTopLeft;
    Point myOptionsBottomRight;
    ZLColor myElementColor;
    ZLColor myFrameColor;
    shared_ptr<Book> myBook;
    shared_ptr<StringRect> myTitleString;
    ImageRect myTitleImage;
    bool myIsSelected;
    bool myIsMenuSelected;
};






#endif

