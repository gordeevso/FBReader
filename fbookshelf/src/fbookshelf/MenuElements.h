#ifndef MENUELEMENTS
#define MENUELEMENTS

#include <string>
#include <vector>

#include <ZLPaintContext.h>

#include "GridElements.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

struct ElementMenu
{
    ElementMenu(ZLPaintContext & context,
                Point topleft = Point(),
                int xoffset = 80,
                int yoffset = 10,
                int fontsize = 5);

    ~ElementMenu()
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


struct TagsMenu
{
    TagsMenu(ZLPaintContext & context,
             std::vector<std::string> const & tags);

    ~TagsMenu()
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
    int myRenderingElementsCount;

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

