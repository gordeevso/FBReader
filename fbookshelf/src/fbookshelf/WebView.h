#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include <ZLView.h>
#include <ZLPaintContext.h>
#include <ZLFileImage.h>

#include "GridView.h"
#include "GridElements.h"
#include "BookshelfMenu.h"
#include "GridContextMenu.h"

#include "../library/BookshelfModel.h"

#include "../library/Book.h"
#include "../options/FBOptions.h"
#include "../options/FBTextStyle.h"

class ZLImageData;

class WebView : public GridView {

public:
    WebView(ZLPaintContext &context);
    const ZLTypeId &typeId() const;

    enum ViewMode {
        BOOKS_FBREADER_ORG = 0,
        GOOGLE_DRIVE = 1
    };

    void setCaption(const std::string &caption);
    void updateView(BookshelfModel::SortType);
    void setMode(ViewMode);
    ViewMode mode() const;
    void invertMode();

    std::vector<GridElement>::iterator getSelectedElement();

    bool onStylusPress(int x, int y);
    bool onStylusMovePressed(int x, int y);
    bool onStylusRelease(int x, int y);
    bool onStylusMove(int x, int y);

    void onScrollbarStep(ZLView::Direction direction, int steps);
    void onScrollbarMoved(ZLView::Direction direction, size_t full, size_t from, size_t to);
    void onScrollbarPageStep(ZLView::Direction direction, int steps);

    void onMouseScroll(bool forward);

public:
    static const ZLTypeId TYPE_ID;

private:
    void updateBookshelfElements();
    void updateScrollDown();
    void updateScrollUp();

    void drawBookshelfElements();
    void drawBackground();

    const std::string &caption() const;
    void paint();
    ZLColor backgroundColor() const;


private:
    WebView::ViewMode myViewMode;

};


#endif // WEBVIEW_H
