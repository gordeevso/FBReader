
#include <queue>
#include <iostream>

#include <ZLibrary.h>
// #include <ZLFile.h>
// #include <ZLDialogManager.h>
// #include <ZLOptionsDialog.h>
// #include <ZLDir.h>
// #include <ZLStringUtil.h>
// #include <ZLResource.h>
// #include <ZLMessage.h>
// #include <ZLTimeManager.h>
// #include <ZLLogger.h>
// #include <ZLNetworkManager.h>

// #include <ZLTextStyleCollection.h>
// #include <ZLTextHyphenator.h>

#include "ZLApp.h"
#include "ZLAppActions.h"

// #include "ZLAppActions.h"
// #include "ScrollingAction.h"
// #include "BookTextView.h"
// #include "FootnoteView.h"
// #include "ContentsView.h"
// #include "RecentBooksPopupData.h"
// #include "PreferencesPopupData.h"
// #include "TimeUpdater.h"

// #include "../libraryTree/LibraryView.h"
// #include "../network/NetworkLinkCollection.h"
// #include "../networkActions/NetworkOperationRunnable.h"
// #include "../networkTree/NetworkView.h"

// #include "../migration/migrate.h"

// #include "../options/FBCategoryKey.h"
// #include "../bookmodel/BookModel.h"
// #include "../formats/FormatPlugin.h"

// #include "../database/booksdb/BooksDB.h"
// #include "../database/booksdb/BooksDBUtil.h"
// #include "../library/Book.h"

// static const std::string OPTIONS = "Options";

// const std::string ZLApp::PageIndexParameter = "pageIndex";

// class OpenFileHandler : public ZLMessageHandler {

// public:
// 	void onMessageReceived(const std::vector<std::string> &arguments) {
// 		if (arguments.size() == 1) {
// 			ZLApp &fbreader = ZLApp::Instance();
// 			fbreader.myBookAlreadyOpen = true;
// 			fbreader.presentWindow();
// 			fbreader.openFile(ZLFile(arguments[0]));
// 		}
// 	}
// };

ZLApp &ZLApp::Instance() {
    return (ZLApp&)ZLApplication::Instance();
}

ZLApp::ZLApp(const std::string &bookToOpen) :
    ZLApplication("ZLApp")
    // ,
    // QuitOnCancelOption(ZLCategoryKey::CONFIG, OPTIONS, "QuitOnCancel", false),
    // KeyScrollingDelayOption(ZLCategoryKey::CONFIG, "Scrollings", "Delay", 0, 2000, 100),
    // LinesToScrollOption(ZLCategoryKey::CONFIG, "SmallScrolling", "LinesToScroll", 1, 20, 1),
    // LinesToKeepOption(ZLCategoryKey::CONFIG, "LargeScrolling", "LinesToKeepOption", 0, 20, 0),
    // EnableTapScrollingOption(ZLCategoryKey::CONFIG, "TapScrolling", "Enabled", true),
    // TapScrollingOnFingerOnlyOption(ZLCategoryKey::CONFIG, "TapScrolling", "FingerOnly", true),
    // UseSeparateBindingsOption(ZLCategoryKey::CONFIG, "KeysOptions", "UseSeparateBindings", false),
    // EnableSingleClickDictionaryOption(ZLCategoryKey::CONFIG, "Dictionary", "SingleClick", false),
    // LastOpenedPreferencesDialog(ZLCategoryKey::CONFIG, "PreferencesDialog", "LastOpened", ""),
    // myBindings0(new ZLKeyBindings("Keys")),
    // myBindings90(new ZLKeyBindings("Keys90")),
    // myBindings180(new ZLKeyBindings("Keys180")),
    // myBindings270(new ZLKeyBindings("Keys270")),
    // myBookToOpen(bookToOpen),
    // myBookAlreadyOpen(false),
    // myActionOnCancel(UNFULLSCREEN)
    {

    mZLAppView = new ZLAppView(*context());
    setView(mZLAppView);
  //  this->refreshWindow();
    // myBookTextView = new BookTextView(*context());
    // myFootnoteView = new FootnoteView(*context());
    // myContentsView = new ContentsView(*context());
    // myNetworkLibraryView = new NetworkView(*context());
    // myLibraryByAuthorView = new LibraryByAuthorView(*context());
    // myLibraryByTagView = new LibraryByTagView(*context());
    // myRecentBooksPopupData = new RecentBooksPopupData();
    // myPreferencesPopupData = new PreferencesPopupData();
    // myMode = UNDEFINED_MODE;
    // myPreviousMode = BOOK_TEXT_MODE;
    // setMode(BOOK_TEXT_MODE);

       addAction("SimpleDialog", new OpenSimpleDialogAction());
     //  doAction("SimpleDialog");
    // addAction(ActionCode::SHOW_READING, new UndoAction(ZLApp::ALL_MODES & ~ZLApp::BOOK_TEXT_MODE));
    // addAction(ActionCode::SHOW_LIBRARY, new SetModeAction(ZLApp::LIBRARY_MODE, ZLApp::BOOK_TEXT_MODE | ZLApp::CONTENTS_MODE));
    // addAction(ActionCode::SHOW_NETWORK_LIBRARY, new ShowNetworkLibraryAction());
    // addAction(ActionCode::SEARCH_ON_NETWORK, new SimpleSearchOnNetworkAction());
    // addAction(ActionCode::ADVANCED_SEARCH_ON_NETWORK, new AdvancedSearchOnNetworkAction());
    // registerPopupData(ActionCode::SHOW_LIBRARY, myRecentBooksPopupData);
    // addAction(ActionCode::SHOW_OPTIONS_DIALOG, new ShowOptionsDialogAction());
    // addAction(ActionCode::SHOW_TOC, new ShowContentsAction());
    // addAction(ActionCode::SHOW_BOOK_INFO_DIALOG, new ShowBookInfoAction());
    // addAction(ActionCode::SHOW_LIBRARY_OPTIONS_DIALOG, new ShowLibraryOptionsDialogAction());
    // addAction(ActionCode::SHOW_NETWORK_OPTIONS_DIALOG, new ShowNetworkOptionsDialogAction());
    // addAction(ActionCode::SHOW_SYSTEM_OPTIONS_DIALOG, new ShowSystemOptionsDialogAction());
    // addAction(ActionCode::SHOW_READING_OPTIONS_DIALOG, new ShowReadingOptionsDialogAction());
    // addAction(ActionCode::SHOW_LOOKANDFEEL_OPTIONS_DIALOG, new ShowLookAndFeelOptionsDialogAction());
    // addAction(ActionCode::ADD_BOOK, new AddBookAction(ZLApp::BOOK_TEXT_MODE | ZLApp::LIBRARY_MODE | ZLApp::CONTENTS_MODE));
    // addAction(ActionCode::UNDO, new UndoAction(ZLApp::BOOK_TEXT_MODE));
    // addAction(ActionCode::REDO, new RedoAction());
    // addAction(ActionCode::SEARCH, new SearchPatternAction());
    // addAction(ActionCode::FIND_NEXT, new FindNextAction());
    // addAction(ActionCode::FIND_PREVIOUS, new FindPreviousAction());
    // addAction(ActionCode::SCROLL_TO_HOME, new ScrollToHomeAction());
    // addAction(ActionCode::SCROLL_TO_START_OF_TEXT, new ScrollToStartOfTextAction());
    // addAction(ActionCode::SCROLL_TO_END_OF_TEXT, new ScrollToEndOfTextAction());
    // addAction(ActionCode::PAGE_SCROLL_FORWARD, new PageScrollingAction(true));
    // addAction(ActionCode::PAGE_SCROLL_BACKWARD, new PageScrollingAction(false));
    // addAction(ActionCode::LINE_SCROLL_FORWARD, new LineScrollingAction(true));
    // addAction(ActionCode::LINE_SCROLL_BACKWARD, new LineScrollingAction(false));
    // addAction(ActionCode::MOUSE_SCROLL_FORWARD, new MouseWheelScrollingAction(true));
    // addAction(ActionCode::MOUSE_SCROLL_BACKWARD, new MouseWheelScrollingAction(false));
    // addAction(ActionCode::TAP_SCROLL_FORWARD, new TapScrollingAction(true));
    // addAction(ActionCode::TAP_SCROLL_BACKWARD, new TapScrollingAction(false));
    // addAction(ActionCode::INCREASE_FONT, new ChangeFontSizeAction(2));
    // addAction(ActionCode::DECREASE_FONT, new ChangeFontSizeAction(-2));
    // addAction(ActionCode::ROTATE_SCREEN, new RotationAction());
    // addAction(ActionCode::TOGGLE_FULLSCREEN, new FBFullscreenAction());
    // addAction(ActionCode::FULLSCREEN_ON, new FBFullscreenAction());
    // addAction(ActionCode::CANCEL, new CancelAction());
    // addAction(ActionCode::SHOW_HIDE_POSITION_INDICATOR, new ToggleIndicatorAction());
    // addAction(ActionCode::QUIT, new QuitAction());
    // addAction(ActionCode::FORCE_QUIT, new ForceQuitAction());
    // addAction(ActionCode::OPEN_PREVIOUS_BOOK, new OpenPreviousBookAction());
    // addAction(ActionCode::SHOW_HELP, new ShowHelpAction());
    // addAction(ActionCode::GOTO_NEXT_TOC_SECTION, new GotoNextTOCSectionAction());
    // addAction(ActionCode::GOTO_PREVIOUS_TOC_SECTION, new GotoPreviousTOCSectionAction());
    // addAction(ActionCode::COPY_SELECTED_TEXT_TO_CLIPBOARD, new CopySelectedTextAction());
    // addAction(ActionCode::OPEN_SELECTED_TEXT_IN_DICTIONARY, new OpenSelectedTextInDictionaryAction());
    // addAction(ActionCode::CLEAR_SELECTION, new ClearSelectionAction());
    // addAction(ActionCode::GOTO_PAGE_NUMBER, new GotoPageNumberAction(std::string()));
    // addAction(ActionCode::GOTO_PAGE_NUMBER_WITH_PARAMETER, new GotoPageNumberAction(PageIndexParameter));
    // shared_ptr<Action> booksOrderAction = new BooksOrderAction();
    // addAction(ActionCode::ORGANIZE_BOOKS_BY_AUTHOR, booksOrderAction);
    // addAction(ActionCode::ORGANIZE_BOOKS_BY_TAG, booksOrderAction);
    // addAction(ActionCode::FILTER_LIBRARY, new FilterLibraryAction());

    // registerPopupData(ActionCode::SHOW_OPTIONS_DIALOG, myPreferencesPopupData);

    // myOpenFileHandler = new OpenFileHandler();
    // ZLCommunicationManager::Instance().registerHandler("openFile", myOpenFileHandler);

    // ZLNetworkManager::Instance().setUserAgent(std::string("ZLApp/") + VERSION);
    std::cout << "ZLApp::ZLApp()" << std::endl;
}

ZLApp::~ZLApp() {
    // ZLTextStyleCollection::deleteInstance();
    // PluginCollection::deleteInstance();
    // ZLTextHyphenator::deleteInstance();
}

void ZLApp::initWindow() {
    ZLApplication::initWindow();
    trackStylus(true);

    // MigrationRunnable migration;
    // if (migration.shouldMigrate()) {
    // 	ZLDialogManager::Instance().wait(ZLResourceKey("migrate"), migration);
    // }

    // if (!myBookAlreadyOpen) {
    // 	shared_ptr<Book> book;
    // 	if (!myBookToOpen.empty()) {
    // 		createBook(ZLFile(myBookToOpen), book);
    // 	}
    // 	if (book.isNull()) {
    // 		const BookList &books = Library::Instance().recentBooks();
    // 		if (!books.empty()) {
    // 			book = books[0];
    // 		}
    // 	}
    // 	if (book.isNull()) {
    // 		book = BooksDBUtil::getBook(helpFileName(ZLibrary::Language()));
    // 	}
    // 	if (book.isNull()) {
    // 		book = BooksDBUtil::getBook(helpFileName("en"));
    // 	}
    // 	openBook(book);
    // }
    std::cout << "ZLApp::initWindow():214" << std::endl;
    refreshWindow();
    std::cout << "ZLApp::initWindow():216" << std::endl;
    // ZLTimeManager::Instance().addTask(new TimeUpdater(), 1000);
}

void ZLApp::refreshWindow() {
    std::cout << "ZLApp::refreshWindow():221" << std::endl;
    ZLApplication::refreshWindow();
    // ((RecentBooksPopupData&)*myRecentBooksPopupData).updateId();
    // ((PreferencesPopupData&)*myPreferencesPopupData).updateId();
    std::cout << "ZLApp::refreshWindow():225" << std::endl;
}

// bool ZLApp::createBook(const ZLFile &bookFile, shared_ptr<Book> &book) {
// 	shared_ptr<FormatPlugin> plugin =
// 		PluginCollection::Instance().plugin(bookFile, false);
// 	if (!plugin.isNull()) {
// 		std::string error = plugin->tryOpen(bookFile);
// 		if (!error.empty()) {
// 			ZLResourceKey boxKey("openBookErrorBox");
// 			ZLDialogManager::Instance().errorBox(
// 				boxKey,
// 				ZLStringUtil::printf(ZLDialogManager::dialogMessage(boxKey), error)
// 			);
// 		} else {
// 			book = BooksDBUtil::getBook(bookFile.path());
// 			if (!book.isNull()) {
// 				BooksDB::Instance().insertIntoBookList(*book);
// 			}
// 		}
// 		return true;
// 	}

// 	if (!bookFile.isArchive()) {
// 		return false;
// 	}

// 	std::queue<std::string> archiveNames;
// 	archiveNames.push(bookFile.path());

// 	std::vector<std::string> items;

// 	while (!archiveNames.empty()) {
// 		shared_ptr<ZLDir> archiveDir = ZLFile(archiveNames.front()).directory();
// 		archiveNames.pop();
// 		if (archiveDir.isNull()) {
// 			continue;
// 		}
// 		archiveDir->collectFiles(items, true);
// 		for (std::vector<std::string>::const_iterator it = items.begin(); it != items.end(); ++it) {
// 			const std::string itemName = archiveDir->itemPath(*it);
// 			ZLFile subFile(itemName);
// 			if (subFile.isArchive()) {
// 				archiveNames.push(itemName);
// 			} else if (createBook(subFile, book)) {
// 				return true;
// 			}
// 		}
// 		items.clear();
// 	}

// 	return false;
// }

// class OpenBookRunnable : public ZLRunnable {

// public:
// 	OpenBookRunnable(shared_ptr<Book> book) : myBook(book) {}
// 	void run() { ZLApp::Instance().openBookInternal(myBook); }

// private:
// 	shared_ptr<Book> myBook;
// };

// void ZLApp::openBook(shared_ptr<Book> book) {
// 	OpenBookRunnable runnable(book);
// 	ZLDialogManager::Instance().wait(ZLResourceKey("loadingBook"), runnable);
// 	resetWindowCaption();
// }

// void ZLApp::openBookInternal(shared_ptr<Book> book) {
// 	if (!book.isNull()) {
// 		BookTextView &bookTextView = (BookTextView&)*myBookTextView;
// 		ContentsView &contentsView = (ContentsView&)*myContentsView;
// 		FootnoteView &footnoteView = (FootnoteView&)*myFootnoteView;

// 		bookTextView.saveState();
// 		bookTextView.setModel(0, 0);
// 		bookTextView.setContentsModel(0);
// 		contentsView.setModel(0);
// 		myModel.reset();
// 		myModel = new BookModel(book);
// 		ZLTextHyphenator::Instance().load(book->language());
// 		bookTextView.setModel(myModel->bookTextModel(), book);
// 		bookTextView.setCaption(book->title());
// 		bookTextView.setContentsModel(myModel->contentsModel());
// 		footnoteView.setModel(0);
// 		footnoteView.setCaption(book->title());
// 		contentsView.setModel(myModel->contentsModel());
// 		contentsView.setCaption(book->title());

// 		Library::Instance().addBook(book);
// 		Library::Instance().addBookToRecentList(book);
// 		((RecentBooksPopupData&)*myRecentBooksPopupData).updateId();
// 		showBookTextView();
// 	}
// }

// void ZLApp::openLinkInBrowser(const std::string &url) const {
// 	if (url.empty()) {
// 		return;
// 	}
// 	shared_ptr<ProgramCollection> collection = webBrowserCollection();
// 	if (collection.isNull()) {
// 		return;
// 	}
// 	shared_ptr<Program> program = collection->currentProgram();
// 	if (program.isNull()) {
// 		return;
// 	}
// 	std::string copy = url;
// 	NetworkLinkCollection::Instance().rewriteUrl(copy, true);
// 	ZLLogger::Instance().println("URL", copy);
// 	program->run("openLink", copy);
// }

// void ZLApp::tryShowFootnoteView(const std::string &id, const std::string &type) {
// 	if (type == "external") {
// 		openLinkInBrowser(id);
// 	} else if (type == "internal") {
// 		if (myMode == BOOK_TEXT_MODE && !myModel.isNull()) {
// 			BookModel::Label label = myModel->label(id);
// 			if (!label.Model.isNull()) {
// 				if (label.Model == myModel->bookTextModel()) {
// 					bookTextView().gotoParagraph(label.ParagraphNumber);
// 				} else {
// 					FootnoteView &view = ((FootnoteView&)*myFootnoteView);
// 					view.setModel(label.Model);
// 					setMode(FOOTNOTE_MODE);
// 					view.gotoParagraph(label.ParagraphNumber);
// 				}
// 				setHyperlinkCursor(false);
// 				refreshWindow();
// 			}
// 		}
// 	} else if (type == "book") {
// 		DownloadBookRunnable downloader(id);
// 		downloader.executeWithUI();
// 		if (downloader.hasErrors()) {
// 			downloader.showErrorMessage();
// 		} else {
// 			shared_ptr<Book> book;
// 			createBook(ZLFile(downloader.fileName()), book);
// 			if (!book.isNull()) {
// 				Library::Instance().addBook(book);
// 				openBook(book);
// 				refreshWindow();
// 			}
// 		}
// 	}
// }

// ZLApp::ViewMode ZLApp::mode() const {
// 	return myMode;
// }

// bool ZLApp::isViewFinal() const {
// 	return myMode == BOOK_TEXT_MODE;
// }

// void ZLApp::showLibraryView() {
// 	if (ZLStringOption(ZLCategoryKey::LOOK_AND_FEEL, "ToggleButtonGroup", "booksOrder", "").value() == ActionCode::ORGANIZE_BOOKS_BY_TAG) {
// 		setView(myLibraryByTagView);
// 	} else {
// 		setView(myLibraryByAuthorView);
// 	}
// }

// void ZLApp::setMode(ViewMode mode) {
// 	if (mode == myMode) {
// 		return;
// 	}

// 	if (mode != BOOK_TEXT_MODE) {
// 		myActionOnCancel = RETURN_TO_TEXT_MODE;
// 	}

// 	myPreviousMode = myMode;
// 	myMode = mode;

// 	switch (myMode) {
// 		case BOOK_TEXT_MODE:
// 			setHyperlinkCursor(false);
// 			((ZLTextView&)*myBookTextView).forceScrollbarUpdate();
// 			setView(myBookTextView);
// 			break;
// 		case CONTENTS_MODE:
// 			((ContentsView&)*myContentsView).gotoReference();
// 			setView(myContentsView);
// 			break;
// 		case FOOTNOTE_MODE:
// 			setView(myFootnoteView);
// 			break;
// 		case LIBRARY_MODE:
// 		{
// 			shared_ptr<Book> currentBook = myModel->book();
// 			((LibraryView&)*myLibraryByAuthorView).showBook(currentBook);
// 			((LibraryView&)*myLibraryByTagView).showBook(currentBook);
// 			showLibraryView();
// 			break;
// 		}
// 		case BOOKMARKS_MODE:
// 			break;
// 		case NETWORK_LIBRARY_MODE:
// 			setView(myNetworkLibraryView);
// 			break;
// 		case UNDEFINED_MODE:
// 		case ALL_MODES:
// 			break;
// 	}
// 	refreshWindow();
// }

// BookTextView &ZLApp::bookTextView() const {
// 	return (BookTextView&)*myBookTextView;
// }

// void ZLApp::showBookTextView() {
// 	setMode(BOOK_TEXT_MODE);
// }

// void ZLApp::restorePreviousMode() {
// 	setMode(myPreviousMode);
// 	myPreviousMode = BOOK_TEXT_MODE;
// }

// bool ZLApp::closeView() {
// 	if (myMode == BOOK_TEXT_MODE) {
// 		quit();
// 		return true;
// 	} else {
// 		restorePreviousMode();
// 		return false;
// 	}
// }

// std::string ZLApp::helpFileName(const std::string &language) const {
// 	return ZLibrary::ApplicationDirectory() + ZLibrary::FileNameDelimiter + "help" + ZLibrary::FileNameDelimiter + "MiniHelp." + language + ".fb2";
// }

// void ZLApp::openFile(const ZLFile &file) {
// 	shared_ptr<Book> book;
// 	createBook(file, book);
// 	if (!book.isNull()) {
// 		openBook(book);
// 		refreshWindow();
// 	}
// }

// bool ZLApp::canDragFiles(const std::vector<std::string> &filePaths) const {
// 	switch (myMode) {
// 		case BOOK_TEXT_MODE:
// 		case FOOTNOTE_MODE:
// 		case CONTENTS_MODE:
// 		case LIBRARY_MODE:
// 			return filePaths.size() > 0;
// 		default:
// 			return false;
// 	}
// }

// void ZLApp::dragFiles(const std::vector<std::string> &filePaths) {
// 	switch (myMode) {
// 		case BOOK_TEXT_MODE:
// 		case FOOTNOTE_MODE:
// 		case CONTENTS_MODE:
// 			if (filePaths.size() > 0) {
// 				openFile(ZLFile(filePaths[0]));
// 			}
// 			break;
// 		case LIBRARY_MODE:
// 			if (filePaths.size() > 0) {
// 				openFile(ZLFile(filePaths[0]));
// 			}
// 			break;
// 		default:
// 			break;
// 	}
// }

// void ZLApp::clearTextCaches() {
// 	((ZLTextView&)*myBookTextView).clearCaches();
// 	((ZLTextView&)*myFootnoteView).clearCaches();
// 	((ZLTextView&)*myContentsView).clearCaches();
// }

// shared_ptr<ZLKeyBindings> ZLApp::keyBindings() {
// 	return UseSeparateBindingsOption.value() ?
// 		keyBindings(rotation()) : myBindings0;
// }

// shared_ptr<ZLKeyBindings> ZLApp::keyBindings(ZLView::Angle angle) {
// 	switch (angle) {
// 		case ZLView::DEGREES0:
// 			return myBindings0;
// 		case ZLView::DEGREES90:
// 			return myBindings90;
// 		case ZLView::DEGREES180:
// 			return myBindings180;
// 		case ZLView::DEGREES270:
// 			return myBindings270;
// 	}
// 	return 0;
// }

// shared_ptr<ProgramCollection> ZLApp::dictionaryCollection() const {
// 	return myProgramCollectionMap.collection("Dictionary");
// }

// bool ZLApp::isDictionarySupported() const {
// 	shared_ptr<ProgramCollection> collection = dictionaryCollection();
// 	return !collection.isNull() && !collection->currentProgram().isNull();
// }

// void ZLApp::openInDictionary(const std::string &word) {
// 	shared_ptr<Program> dictionary = dictionaryCollection()->currentProgram();
// 	dictionary->run("present", ZLibrary::ApplicationName());
// 	dictionary->run("showWord", word);
// }

// shared_ptr<ProgramCollection> ZLApp::webBrowserCollection() const {
// 	return myProgramCollectionMap.collection("Web Browser");
// }

// shared_ptr<Book> ZLApp::currentBook() const {
// 	return myModel->book();
// }

// void ZLApp::invalidateNetworkView() {
// 	((NetworkView &) *myNetworkLibraryView).invalidate();
// }

// void ZLApp::invalidateAccountDependents() {
// 	((NetworkView &) *myNetworkLibraryView).invalidateAccountDependents();
// }
