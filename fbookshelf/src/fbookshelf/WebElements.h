#ifndef WEBELEMENTS_H
#define WEBELEMENTS_H


#include <string>

#include <ZLPaintContext.h>

#include "GridElements.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"



struct WebElement
{
    WebElement(Point topLeft = Point(),
                     Point bottomRight = Point(),
                     Point opttopleft = Point(),
                     Point optbottomright = Point(),
                     ZLColor elementColor = ZLColor(),
                     ZLColor frameColor = ZLColor(),
                     shared_ptr<Book> book = 0,
                     bool selected = false
            );

    ~WebElement()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
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
};





#endif // WEBELEMENTS_H
