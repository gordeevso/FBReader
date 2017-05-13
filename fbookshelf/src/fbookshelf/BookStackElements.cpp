#include "BookStackElements.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"


Shelf::Shelf(int y, int width)
    : myY(y),
      myWidth(width)
{
}


void Shelf::draw(ZLPaintContext &painter)
{

    
    painter.setFillColor(ZLColor(150,75,0));
    painter.fillRectangle(10, myY, myWidth - 10, myY + 15);
    painter.setColor(ZLColor(150,75,0));
    for(int i = 0; i <= 60; ++i) {
        painter.drawLine(10 + i, myY, 10 + i, myY - i);
    }
    for(int i = 0; i <= 60; ++i) {
        painter.drawLine(myWidth - 10 - i, myY, myWidth - 10 - i, myY - i);
    }
    painter.fillRectangle(10 + 60, myY, myWidth - 10 - 60, myY - 60);
    painter.setColor(ZLColor(0,0,0));
    painter.drawLine(10, myY, 10 + 60, myY - 60);
    painter.drawLine(myWidth - 10, myY, myWidth - 10 - 60, myY - 60);
    painter.drawLine(10 + 60, myY - 60, myWidth - 10 - 60, myY - 60);
    painter.drawLine(10, myY, 10, myY + 15);
    painter.drawLine(myWidth - 10, myY, myWidth - 10, myY + 15);
    painter.drawLine(10 + 60, myY - 60, myWidth - 10 - 60, myY - 60);
    painter.drawLine(10, myY, myWidth - 10, myY);
    painter.drawLine(10, myY + 15, myWidth - 10, myY + 15);
    myShelfName->myFontSize = 40;
    myShelfName->updatePosition(60, myY - 260, myWidth - 60, myY + 40);
    myShelfName->draw();

}

BookOnShelf::BookOnShelf(bool selected)
    : myTitleImage(),
      myIsSelected(selected)
{}


void BookOnShelf::updatePosition(int x1, int y1, int x2, int y2)
{
    myTitleImage.updatePosition(x1, y1, x2, y2);
}

void BookOnShelf::drawElement(ZLPaintContext &painter)
{
    myTitleImage.draw(painter);
}

bool BookOnShelf::checkSelectedBook(int x, int y)
{
    return (x > myTitleImage.myX &&
            x < myTitleImage.myX + myTitleImage.myWidth &&
            y < myTitleImage.myY &&
            y > myTitleImage.myY - myTitleImage.myHeight);
}