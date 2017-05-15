#include "WebElements.h"
#include "FBookshelf.h"
#include "BookshelfActions.h"



WebElement::WebElement(Point topLeft,
                         Point bottomRight,
                         Point opttopleft,
                         Point optbottomright,
                         ZLColor elementColor,
                         ZLColor frameColor,
                         shared_ptr<Book> book,
                         bool selected
                       )
    : myTopLeft(topLeft),
      myBottomRight(bottomRight),
      myOptionsTopLeft(opttopleft),
      myOptionsBottomRight(optbottomright),
      myElementColor(elementColor),
      myFrameColor(frameColor),
      myBook(book),
      myTitleString(),
      myTitleImage(),
      myIsSelected(selected)
{}


void WebElement::updatePosition(int x1, int y1, int x2, int y2)
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



void WebElement::drawElement(ZLPaintContext &painter)
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

}

bool WebElement::checkBookOptions(int x, int y)
{
    return (x > myOptionsTopLeft.x &&
            x < myOptionsBottomRight.x &&
            y > myOptionsTopLeft.y &&
            y < myOptionsBottomRight.y);
}

bool WebElement::checkSelectedBook(int x, int y)
{
    return (x > myTopLeft.x &&
            x < myBottomRight.x &&
            y > myTopLeft.y &&
            y < myBottomRight.y);
}
