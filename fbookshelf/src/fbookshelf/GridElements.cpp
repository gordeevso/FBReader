#include "GridElements.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"

ImageRect::ImageRect(int x, int y, int width, int height, shared_ptr<ZLImageData> imagedata)
    : myX(x),
      myY(y),
      myWidth(width),
      myHeight(height),
      myImageData(imagedata)
{}

void ImageRect::updatePosition(int x1, int y1, int x2, int y2)
{
    float beFactor = (x2 - x1)/(y2 - y1);

    myX = x1 + (x2 - x1) * 0.05 ;
    myY = y2 - (y2 - y1) * 0.25 + beFactor * (y2 - y1) * 0.05;

    myHeight = (y2 - y1)*0.75;
    myWidth = myHeight/myHWFactor;

}

void ImageRect::draw(ZLPaintContext &painter)
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





StringRect::StringRect(const std::string &str,
                       ZLPaintContext &paintcontext,
                       int x1,
                       int y1,
                       int x2,
                       int y2,
                       int fontsize)
    : myRefStr(str),
      myRefPainter(paintcontext),
      myX(x1),
      myY(y1),
      myXr(x2),
      myYr(y2),
      myFontSize(fontsize)
{}

void StringRect::updateFont(int x1, int y1, int x2, int y2)
{
    myFontSize = (y2 - y1) * 0.04;

    const FBTextStyle &style = FBTextStyle::Instance();
    myRefPainter.setFont(style.fontFamily(), myFontSize, style.bold(), style.italic());
}

void StringRect::updatePosition(int x1, int y1, int x2, int y2)
{
    updateFont(x1, y1, x2, y2);

    myX = x1 + (x2 - x1) * 0.05;
    myY = y2 - (y2 - y1) * 0.075 - myFontSize;

    myXr = x2 - (x2 - x1) * 0.05;
    myYr = y2 - (y2 - y1) * 0.075;
}

void StringRect::draw()
{
    myRefPainter.setFillColor(ZLColor(255,255,255));
    myRefPainter.fillRectangle(myX-1, myY-2, myXr+1, myYr+1);

    myRefPainter.setColor(FBOptions::Instance().RegularTextColorOption.value());

    myRefPainter.drawString(myX,
                            myYr,
                            myRefStr.c_str(),
                            myRefStr.size(),
                            true);

    myRefPainter.setColor(ZLColor(0,0,0));
    myRefPainter.drawLine(myX-1, myY-2, myXr+1, myY-2);
    myRefPainter.drawLine(myXr+1, myY-2, myXr+1, myYr+1);
    myRefPainter.drawLine(myXr+1, myYr+1, myX-1, myYr+1);
    myRefPainter.drawLine(myX-1, myYr+1, myX-1, myY-2);
}





GridElement::GridElement(Point topLeft,
                         Point bottomRight,
                         Point opttopleft,
                         Point optbottomright,
                         ZLColor elementColor,
                         ZLColor frameColor,
                         shared_ptr<Book> book,
                         shared_ptr<ZLImageData> imagedata,
                         bool selected,
                         bool menuselected)
    : myTopLeft(topLeft),
      myBottomRight(bottomRight),
      myOptionsTopLeft(opttopleft),
      myOptionsBottomRight(optbottomright),
      myElementColor(elementColor),
      myFrameColor(frameColor),
      myTitleString(),
      myTitleImage(),
      myIsSelected(selected),
      myIsMenuSelected(menuselected)
{}


void GridElement::updatePosition(int x1, int y1, int x2, int y2)
{
    myTopLeft.x = x1;
    myTopLeft.y = y1;
    myBottomRight.x = x2;
    myBottomRight.y = y2;

    float beFactor = (x2 - x1)/(y2 - y1);
    int options_width = (x2 - x1) * 0.1;
    int options_height = (y2 - y1) * 0.1;

    myOptionsTopLeft.x = x2 - options_width - (x2 - x1) * 0.05;
    myOptionsTopLeft.y = y1 + beFactor * (y2 - y1) * 0.05;
    myOptionsBottomRight.x = myOptionsTopLeft.x + options_width;
    myOptionsBottomRight.y = myOptionsTopLeft.y + options_height;

    myTitleImage.updatePosition(x1, y1, x2, y2);
    myTitleString->updatePosition(x1, y1, x2, y2);
}


void GridElement::drawOptions(ZLPaintContext &painter)
{
    if(!myIsMenuSelected)
        painter.setFillColor(ZLColor(255,255,255));
    else
        painter.setFillColor(ZLColor(210,210,90));

    painter.fillRectangle(myOptionsTopLeft.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsBottomRight.y);

    painter.setColor(ZLColor(100,100,100));
    painter.drawLine(myOptionsTopLeft.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsTopLeft.y);
    painter.drawLine(myOptionsBottomRight.x, myOptionsTopLeft.y, myOptionsBottomRight.x, myOptionsBottomRight.y);
    painter.drawLine(myOptionsBottomRight.x, myOptionsBottomRight.y, myOptionsTopLeft.x, myOptionsBottomRight.y);
    painter.drawLine(myOptionsTopLeft.x, myOptionsBottomRight.y, myOptionsTopLeft.x, myOptionsTopLeft.y);

}

void GridElement::drawElement(ZLPaintContext &painter)
{
    if(!myIsSelected)
        painter.setFillColor(myElementColor);
    else
        painter.setFillColor(ZLColor(210,210,90));

    painter.fillRectangle(myTopLeft.x, myTopLeft.y, myBottomRight.x, myBottomRight.y);

    painter.setColor(myFrameColor);
    painter.drawLine(myTopLeft.x, myTopLeft.y, myBottomRight.x, myTopLeft.y);
    painter.drawLine(myBottomRight.x, myTopLeft.y, myBottomRight.x, myBottomRight.y);
    painter.drawLine(myBottomRight.x, myBottomRight.y, myTopLeft.x, myBottomRight.y);
    painter.drawLine(myTopLeft.x, myBottomRight.y, myTopLeft.x, myTopLeft.y);

    myTitleImage.draw(painter);
    myTitleString->draw();

    drawOptions(painter);
}

bool GridElement::checkBookOptions(int x, int y)
{
    return (x > myOptionsTopLeft.x &&
            x < myOptionsBottomRight.x &&
            y > myOptionsTopLeft.y &&
            y < myOptionsBottomRight.y);
}

bool GridElement::checkSelectedBook(int x, int y)
{
    return (x > myTopLeft.x &&
            x < myBottomRight.x &&
            y > myTopLeft.y &&
            y < myBottomRight.y);
}




