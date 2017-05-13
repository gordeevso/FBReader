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
    Shelf(int y = 0, int width = 40);

    ~Shelf()
    {}

    void updatePosition(int y);
    void draw(ZLPaintContext & painter);

public:

    int myX;
    int myY;
    int myWidth;
    shared_ptr<StringRect> myShelfName;
};

struct BookOnShelf
{
    BookOnShelf(bool selected = false);

    ~BookOnShelf()
    {}

    void updatePosition(int x1, int y1, int x2, int y2);
    void drawElement(ZLPaintContext & painter);

    bool checkSelectedBook(int x, int y);


public:
    shared_ptr<Book> myBook;
    ImageRect myTitleImage;
    bool myIsSelected;
    bool myIsCaptured;
};

#endif /* BOOKSTACKELEMENTS_H */
