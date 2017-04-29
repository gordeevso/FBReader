#ifndef BOOKSTACKELEMENTS_H
#define BOOKSTACKELEMENTS_H

#include <string>

#include <ZLPaintContext.h>

#include "GridElements.h"
#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

struct BookElement
{
    BookElement(int x = 0, int y = 0,
              int width = 40, int height = 40,
              shared_ptr<ZLImageData>imagedata = 0);

    ~BookElement()
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

struct BookStackElement
{
    BookStackElement(Point topLeft = Point(),
                     Point bottomRight = Point(),
                     shared_ptr<Book> book = 0,
                     shared_ptr<ZLImageData> imagedata = 0);

    ~BookStackElement()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
    void drawOptions(ZLPaintContext & painter);
    void drawElement(ZLPaintContext & painter);

    bool checkBookOptions(int x, int y);
    bool checkSelectedBook(int x, int y);


public:
    Point myTopLeft;
    Point myBottomRight;
    shared_ptr<Book> myBook;
    ImageRect myTitleImage;
    
};


struct Shelf
{
    Shelf(int y = 0,
              int width = 40);

    ~Shelf()
    {}

    void updatePosition(int y);
    void draw(ZLPaintContext & painter);

public:

    int myX;
    int myY;
    int myWidth;
    shared_ptr<ZLImageData> myImageData;
    float myHWFactor;
};

#endif /* BOOKSTACKELEMENTS_H */

