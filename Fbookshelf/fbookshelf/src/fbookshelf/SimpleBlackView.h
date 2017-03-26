#ifndef SIMPLEBLACKVIEW
#define SIMPLEBLACKVIEW

#include <ZLView.h>

class SimpleBlackView : public ZLView {

public:
    SimpleBlackView(ZLPaintContext &context);

    void setCaption(const std::string &caption);

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);

private:
    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;

    struct Point{
        int x;
        int y;
        Point(int xx = 0, int yy = 0) : x(xx), y(yy)
        {}
    } mStartPoint, mEndPoint;
};


#endif // ZLAPPVIEW

