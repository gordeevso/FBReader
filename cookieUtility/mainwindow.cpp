#include "mainwindow.h"
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>

CookieWidget::CookieWidget(const QNetworkCookie &cookie, QWidget *parent): QWidget(parent)
{
    setupUi(this);
    setAutoFillBackground(true);
    m_nameLabel->setText(cookie.name());
    m_domainLabel->setText(cookie.domain());
}

void CookieWidget::setHighlighted(bool enabled)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), enabled ? QColor(0xF0, 0xF8, 0xFF) : Qt::white);
    setPalette(p);
}

MainWindow::MainWindow(const QUrl &url) :
    QMainWindow(),
    m_store(nullptr),
    m_layout(new QVBoxLayout)
{
    setupUi(this);

    m_layout->addItem(new QSpacerItem(0,0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    QWidget *w = new QWidget();
    QPalette p = w->palette();
    p.setColor(widget->backgroundRole(), Qt::white);
    w->setPalette(p);
    w->setLayout(m_layout);

    connect(SaveAll, &QPushButton::clicked, this, &MainWindow::handleSaveAllClicked);
    connect(LogOut,  &QPushButton::clicked, this, &MainWindow::handleLogOutClicked);
    m_store = m_webview->page()->profile()->cookieStore();
    connect(m_store, &QWebEngineCookieStore::cookieAdded, this, &MainWindow::handleCookieAdded);
    m_store->loadAllCookies();
    m_webview->load(url);
}

bool MainWindow::containsCookie(const QNetworkCookie &cookie)
{
    for (auto c: m_cookies) {
        if (c.hasSameIdentifier(cookie))
            return true;
    }
    return false;
}

void MainWindow::handleCookieAdded(const QNetworkCookie &cookie)
{
    /*if (containsCookie(cookie))
        return;
*/
    CookieWidget *widget = new CookieWidget(cookie);
    widget->setHighlighted(m_cookies.count() % 2);
    m_cookies.append(cookie);
    m_layout->insertWidget(0,widget);
}

std::string getHomeDir(){
    int myuid;
    passwd *mypasswd;
    std::string TestFileName;
    myuid = getuid();
    mypasswd = getpwuid(myuid);
    TestFileName= mypasswd->pw_dir;
    return TestFileName;
}

void MainWindow::handleLogOutClicked()
{
    m_store->deleteAllCookies();
    for (int i = m_layout->count() - 1; i >= 0; i--)
        delete m_layout->itemAt(i)->widget();
    m_cookies.clear();
    handleSaveAllClicked();
    m_webview->reload();
}


void MainWindow::handleSaveAllClicked()
{
    std::string filepath = getHomeDir() + "/FBookshelfNet/cookie.txt";
    std::ofstream fout(filepath);
    for (auto c: m_cookies) {
        if (c.domain() == ".fbreader.org" || c.domain() == "books.fbreader.org")
            fout << c.name().constData() << "=" <<c.value().constData() << "; ";
    }
}
