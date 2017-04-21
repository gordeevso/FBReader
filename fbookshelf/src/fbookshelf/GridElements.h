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

    void updateFont(int x1, int y1, int x2, int y2);
    void updatePosition(int x1, int y1, int x2, int y2);
    void draw();


    public:

        int myX;
        int myY;
        int myXr;
        int myYr;
        int myFontSize;
        std::string const & myRefStr;
        ZLPaintContext & myRefPainter;
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



struct ElementMenu
{
    ElementMenu(ZLPaintContext & context,
                Point topleft = Point(),
                int xoffset = 80,
                int yoffset = 10,
                int fontsize = 5);

    ~ElementMenu()
    {}

    void draw();
    bool checkSelectedElementMenu(int x, int y, bool & changed_state);

    ZLPaintContext & myRefPainter;
    Point myTopLeft;
    Point myCurTopLeft;
    int myXOffset;
    int myYOffset;
    int myFontSize;
    bool myIsVisible;
    bool myIsSelected;

    std::vector<std::pair<std::string, bool> >::iterator myItSelectedActionCode;
    std::vector<std::pair<std::string, bool> >::iterator myItMenu;
    std::vector<std::pair<std::string, bool> >::iterator myItMenuEnd;
    std::vector<std::pair<std::string, bool> > myVecMenuStrings;
};


#endif

