#include "BookStackElements.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

BookElement::BookElement(int x, int y, int width, int height, shared_ptr<ZLImageData> imagedata)
    : myX(x),
      myY(y),
      myWidth(x + 200),
      myHeight(y + 250),
      myImageData(imagedata)
{}

void BookElement::updatePosition(int x1, int y1, int x2, int y2)
{
    float beFactor = (x2 - x1)/(y2 - y1);

    myX = x1 + (x2 - x1) * 0.05 ;
    myY = y2 - (y2 - y1) * 0.25 + beFactor * (y2 - y1) * 0.05;

    myHeight = 250;
    myWidth = myHeight/myHWFactor;

}

void BookElement::draw(ZLPaintContext &painter)
{


    painter.drawImage(myX,
                      myY,
                      *myImageData,
                      myWidth,
                      myHeight,
                      ZLPaintContext::SCALE_FIT_TO_SIZE);

    painter.setColor(ZLColor(0,0,0));
    painter.drawLine(myX, myY-myHeight, myX+myWidth, myY-myHeight);
    painter.drawLine(myX+myWidth, myY-myHeight, myX+myWidth, myY);
    painter.drawLine(myX+myWidth, myY, myX, myY);
    painter.drawLine(myX, myY, myX, myY-myHeight);


}

BookStackElement::BookStackElement(Point topLeft,
                         Point bottomRight,
                         shared_ptr<Book> book,
                         shared_ptr<ZLImageData> imagedata)
    : myTopLeft(topLeft),
      myBottomRight(bottomRight),
//      myOptionsTopLeft(opttopleft),
//      myOptionsBottomRight(optbottomright),


      myTitleImage()
{}


void BookStackElement::updatePosition(int x1, int y1, int x2, int y2)
{
    myTopLeft.x = x1;
    myTopLeft.y = y1;
    myBottomRight.x = x2;
    myBottomRight.y = y2;

//    float beFactor = (x2 - x1)/(y2 - y1);
//    int options_width = (x2 - x1) * 0.1;
//    int options_height = (y2 - y1) * 0.1;

//    myOptionsTopLeft.x = x2 - options_width - (x2 - x1) * 0.05;
//    myOptionsTopLeft.y = y1 + beFactor * (y2 - y1) * 0.05;
//    myOptionsBottomRight.x = myOptionsTopLeft.x + options_width;
//    myOptionsBottomRight.y = myOptionsTopLeft.y + options_height;

    myTitleImage.updatePosition(x1, y1, x2, y2);
//    myTitleString->updatePosition(x1, y1, x2, y2);
}


void BookStackElement::drawOptions(ZLPaintContext &painter)
{
//      painter.setFillColor(ZLColor(210,210,90));

//    painter.fillRectangle(myOptionsTopLeft.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsBottomRight.y);

//    painter.setColor(ZLColor(100,100,100));
//    p    if(!myIsMenuSelected)
//        painter.setFillColor(ZLColor(255,255,255));
//    else
//  ainter.drawLine(myOptionsTopLeft.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsTopLeft.y);
//    painter.drawLine(myOptionsBottomRight.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsBottomRight.y);
//    painter.drawLine(myOptionsBottomRight.x, myOptionsBottomRight.y, myOptionsTopLeft.x, myOptionsBottomRight.y);
//    painter.drawLine(myOptionsTopLeft.x, myOptionsBottomRight.y, myOptionsTopLeft.x, myOptionsTopLeft.y);

}

void BookStackElement::drawElement(ZLPaintContext &painter)
{

    myTitleImage.draw(painter);
}

bool BookStackElement::checkBookOptions(int x, int y)
{
//    return (x > myOptionsTopLeft.x &&
//            x < myOptionsBottomRight.x &&
//            y > myOptionsTopLeft.y &&
//            y < myOptionsBottomRight.y);
}

bool BookStackElement::checkSelectedBook(int x, int y)
{
    return (x > myTopLeft.x &&
            x < myBottomRight.x &&
            y > myTopLeft.y &&
            y < myBottomRight.y);
}

Shelf::Shelf(int y, int width)
    : myY(y),
      myWidth(width)
{}


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
    
}