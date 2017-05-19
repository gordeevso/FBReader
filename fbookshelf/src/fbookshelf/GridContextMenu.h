#ifndef GRIDCONTEXTMENU
#define GRIDCONTEXTMENU

#include <string>
#include <vector>

#include <ZLPaintContext.h>

#include "GridElements.h"


struct GridContextMenu
{
    GridContextMenu(ZLPaintContext & context,
                Point topleft = Point(),
                int xoffset = 100,
                int yoffset = 10,
                int fontsize = 5);

    ~GridContextMenu()
    {}

    void draw();
    bool checkSelectedElementMenu(int x, int y, bool & changed_state);

    ZLPaintContext & myRefPainter;
    Point myTopLeft;
    Point myCurTopLeft;
    int myXOffset;
    int myYOffset;
    int myFontSize;
    bool myIsVisible;
    bool myIsSelected;

    std::vector<std::pair<std::string, bool> >::iterator myItSelectedActionCode;
    std::vector<std::pair<std::string, bool> >::iterator myItMenu;
    std::vector<std::pair<std::string, bool> >::iterator myItMenuEnd;
    std::vector<std::pair<std::string, bool> > myVecMenuStrings;
};

#endif // GRIDCONTEXTMENU

