#include <cmath>
#include <algorithm>

#include <iostream>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "SimpleBlackView.h"
#include "Fbookshelf.h"

SimpleBlackView::SimpleBlackView(ZLPaintContext &context) : ZLView(context)
{
}


bool SimpleBlackView::onStylusPress(int x, int y) {
    mStartPoint.x = x;
    mStartPoint.y = y;

    return true;
}


bool SimpleBlackView::onStylusMovePressed(int x, int y) {
    mEndPoint.x = x;
    mEndPoint.y = y;

    Fbookshelf::Instance().refreshWindow();

    return true;
}

const std::string &SimpleBlackView::caption() const {
    static const std::string cap = "PaintView";
    return cap;
}

ZLColor SimpleBlackView::backgroundColor() const {
    return ZLColor(255, 255, 255);
}

void SimpleBlackView::paint() {

    context().setColor(ZLColor(255, 255, 255));
    context().drawLine(mStartPoint.x,mStartPoint.y,mEndPoint.x,mEndPoint.y);

    mStartPoint.x = mEndPoint.x;
    mStartPoint.y = mEndPoint.y;
}
