#ifndef MENUELEMENTS
#define MENUELEMENTS

#include <string>
#include <vector>

#include <ZLPaintContext.h>

#include "GridElements.h"


struct BookshelfMenu
{
    BookshelfMenu(ZLPaintContext & context,
             std::vector<std::string> const & tags);

    ~BookshelfMenu()
    {}

    void reloadTags(std::vector<std::string> const & tags);
    void draw();
    void divideStr(std::string const & source, std::string & new_str);
    void checkFont();
    bool checkSelectedElementMenu(int x, int y, bool & changed_state);
    void updateScrollDown();
    void updateScrollUp();

public:
    ZLPaintContext & myRefPainter;
    int myXOffset;
    int myYOffset;
    int myFontSize;
    int myViewHeight;
    size_t myRenderingElementsCount;

    Point myTopLeft;
    Point myCurTopLeft;

    bool myIsVisible;
    bool myIsSelected;

    std::vector<std::pair<std::string, bool> >::iterator myItSelectedActionCode;
    std::vector<std::pair<std::string, bool> >::iterator myItFirstRenderingMenu;
    std::vector<std::pair<std::string, bool> >::iterator myItLastRenderingMenu;
    std::vector<std::pair<std::string, bool> > myVecMenuStrings;
};



#endif // MENUELEMENTS

