#include "BookStackElements.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"


Shelf::Shelf(int x, int y, int width, int height) :
    myX(x),
    myY(y),
    myWidth(width),
    myHeight(height)
{
    ZLFile shelfImageFile("/usr/share/pixmaps/FBookshelf/shelf.png");
    shared_ptr<ZLFileImage> shelfFileImage = new ZLFileImage(shelfImageFile, 0);
    shelfImage = ZLImageManager::Instance().imageData(*shelfFileImage);

}

void Shelf::draw(ZLPaintContext &painter)
{
    painter.drawImage(0, myY, *shelfImage, 3975, 500, ZLPaintContext::SCALE_FIT_TO_SIZE);
//    myShelfName->myFontSize = 40;
//    myShelfName->updatePosition(x, myY - 300, myWidth - 60, myY);
//    this->myShelfName->draw();
}

TitleImage::TitleImage(int x, int y, int width, int height, shared_ptr<ZLImageData> imagedata)
    : myX(x),
      myY(y),
      myWidth(width),
      myHeight(height),
      myImageData(imagedata)
{}

void TitleImage::updatePosition(int x1, int y1, int x2, int y2)
{
    float beFactor = (x2 - x1)/(y2 - y1);

    myX = x1 ;
    myY = y2;

    myHeight = (y2 - y1);
    myWidth = myHeight/myHWFactor;

}

void TitleImage::draw(ZLPaintContext &painter)
{
    painter.drawImage(myX,
                      myY,
                      *myImageData,
                      myWidth,
                      myHeight,
                      ZLPaintContext::SCALE_FIT_TO_SIZE);
}

BookSlot::BookSlot(Point topLeft, Point bottomRight, std::string shelf, size_t index, bool selected) :
        myTopLeft(topLeft),
        myBottomRight(bottomRight),
        myShelf(shelf),
        myIndex(index),
        myIsSelected(selected)
{}
  
void BookSlot::updatePosition(int x1, int y1, int x2, int y2) {
        myTopLeft.x = x1;
        myTopLeft.y = y1;
        myBottomRight.x = x2;
        myBottomRight.y = y2;
}

void BookSlot::draw(ZLPaintContext & painter) {
    if (myIsSelected) {
        painter.setFillColor(ZLColor(133, 133, 250));
    } else {
        painter.setFillColor(ZLColor(133, 133, 0), ZLPaintContext::HALF_FILL);
    }
    painter.fillRectangle(myTopLeft.x, myTopLeft.y, myBottomRight.x, myBottomRight.y);
    painter.setColor(ZLColor(123, 255, 145));
    painter.drawLine(myTopLeft.x, myTopLeft.y, myTopLeft.x, myBottomRight.y);
    painter.drawLine(myTopLeft.x, myTopLeft.y, myBottomRight.x, myTopLeft.y);
    painter.drawLine(myBottomRight.x, myTopLeft.y, myBottomRight.x, myBottomRight.y);
    painter.drawLine(myBottomRight.x, myBottomRight.y, myTopLeft.x, myBottomRight.y);
}

bool BookSlot::checkSelectedSlot(int x, int y) {
    return (x > myTopLeft.x &&
            x < myBottomRight.x &&
            y < myBottomRight.y &&
            y > myTopLeft.y);
}
    


BookElement::BookElement(Point topLeft,
                         Point bottomRight,
                         bool selected,
                         bool captured)
    : myTopLeft(topLeft),
      myBottomRight(bottomRight),
      myTitleImage(),
      myIsSelected(selected),
      myIsCaptured(captured)
{}


void BookElement::updatePosition(int x1, int y1, int x2, int y2)
{
    myTopLeft.x = x1;
    myTopLeft.y = y1;
    myBottomRight.x = x2;
    myBottomRight.y = y2;
    myTitleImage.updatePosition(x1, y1, x2, y2);
}


void BookElement::drawElement(ZLPaintContext &painter)
{
    myTitleImage.draw(painter);
    if(myIsSelected) {
        painter.setFillColor(ZLColor(255, 255, 0), ZLPaintContext::HALF_FILL);
        painter.fillRectangle(myTopLeft.x, myTopLeft.y, myTopLeft.x + myTitleImage.myWidth, myTopLeft.y + myTitleImage.myHeight);
    }
}

bool BookElement::checkSelectedBookElement(int x, int y)
{   
    return (x > myTitleImage.myX &&
            x < myTitleImage.myX + myTitleImage.myWidth &&
            y < myTitleImage.myY &&
            y > myTitleImage.myY - myTitleImage.myHeight);
}