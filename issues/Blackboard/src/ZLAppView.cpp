#include <cmath>
#include <algorithm>

#include <iostream>

#include <ZLUnicodeUtil.h>
#include <ZLTimeManager.h>
#include <ZLTextSelectionModel.h>

#include "ZLAppView.h"
#include "ZLApp.h"

ZLAppView::ZLAppView(ZLPaintContext &context) : ZLView(context)
{
}


bool ZLAppView::onStylusPress(int x, int y){
    std::cout << "onStylusPress " << x <<" " << y <<"\n";

    mStartPoint.x = x;
    mStartPoint.y = y;

    return true;
}


bool ZLAppView::onStylusMovePressed(int x, int y)
{
    std::cout << "onStylusMovePressed " << x <<" " << y <<"\n";

    mEndPoint.x = x;
    mEndPoint.y = y;

    ZLApp::Instance().refreshWindow();

    return true;
}

const std::string &ZLAppView::caption() const {
    static const std::string cap = "PaintView";
    return cap;
}

ZLColor ZLAppView::backgroundColor() const {
    return ZLColor(255, 255, 255);
}

void ZLAppView::paint() {

    context().setColor(ZLColor(255, 255, 255));
    context().drawLine(mStartPoint.x,mStartPoint.y,mEndPoint.x,mEndPoint.y);

    mStartPoint.x = mEndPoint.x;
    mStartPoint.y = mEndPoint.y;
}
