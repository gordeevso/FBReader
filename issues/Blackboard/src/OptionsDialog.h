#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <string>

#include <ZLOptionEntry.h>


class ZLOptionsDialog;
class ZLDialogContent;


class BookInfoDialog {

public:
    BookInfoDialog(shared_ptr<Book> book);

    ZLOptionsDialog &dialog();

private:
    void initAuthorEntries();
    void initTagEntries();

private:
    shared_ptr<ZLOptionsDialog> myDialog;
    shared_ptr<Book> myBook;
    shared_ptr<FormatInfoPage> myFormatInfoPage;

    ZLComboOptionEntry *myEncodingSetEntry;
    ZLComboOptionEntry *myEncodingEntry;
    ZLComboOptionEntry *myLanguageEntry;
    SeriesTitleEntry *mySeriesTitleEntry;
    BookIndexEntry *myBookIndexEntry;

    ZLDialogContent *myTagsTab;
    std::vector<BookTagEntry *> myTagEntries;
    bool myTagsDone;

    std::vector<std::string> myNewTags;

    ZLDialogContent *myAuthorsTab;
    std::vector<AuthorDisplayNameEntry *> myAuthorEntries;
    bool myAuthorsDone;

friend class AuthorDisplayNameEntry;
friend class SeriesTitleEntry;
friend class BookIndexEntry;
friend class BookTitleEntry;
friend class BookEncodingEntry;
friend class BookLanguageEntry;
friend class BookTagEntry;
friend class BookInfoApplyAction;
};

inline ZLOptionsDialog &BookInfoDialog::dialog() { return *myDialog; }

#endif // OPTIONSDIALOG_H

