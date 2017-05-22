#ifndef BOOKSTACKELEMENTS_H
#define BOOKSTACKELEMENTS_H

#include <string>

#include <ZLPaintContext.h>

#include "GridElements.h"
#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

struct Shelf
{
    Shelf(int x, int y, int width, int height);

    ~Shelf()
    {}

    void updatePosition(int y);
    void draw(ZLPaintContext & painter);

public:

    shared_ptr<StringRect> myShelfName;
    std::string pathToShelfImage = "/usr/share/pixmaps/FBookshelf/shelf.png";
    shared_ptr<ZLImageData> shelfImage;
    int myX;
    int myY;
    int myWidth;
    int myHeight;
    
};

struct TitleImage
{
    TitleImage(int x = 0, int y = 0,
              int width = 40, int height = 40,
              shared_ptr<ZLImageData>imagedata = 0);

    ~TitleImage()
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

struct BookSlot {
    BookSlot(Point topLeft = Point(), Point bottomRight = Point(),
             std::string shelf = "", size_t index = 0, bool selected = false);
  
    ~BookSlot()
    {}
    
    void updatePosition(int x1, int y1, int x2, int y2);
    void draw(ZLPaintContext & painter);
    bool checkSelectedSlot(int x, int y);
    
public:
    
    Point myTopLeft;
    Point myBottomRight;
    std::string myShelf;
    int myIndex;
    bool myIsSelected;
};

struct BookElement
{
    BookElement(Point topLeft = Point(),
                     Point bottomRight = Point(),
                     bool selected = false,
                     bool captured = false);
    
    ~BookElement()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
    void drawElement(ZLPaintContext & painter);
    bool checkSelectedBookElement(int x, int y);


public:
    shared_ptr<Book> myBook;
    TitleImage myTitleImage;
    Point myTopLeft;
    Point myBottomRight;
    std::string myShelf; 
    bool myIsSelected;
    bool myIsCaptured;
};

#endif /* BOOKSTACKELEMENTS_H */