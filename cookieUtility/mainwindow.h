#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "ui_cookiewidget.h"
#include <QNetworkCookie>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QWebEngineCookieStore;
QT_END_NAMESPACE

class CookieWidget : public QWidget,  public Ui_CookieWidget
{
    Q_OBJECT
public:
    CookieWidget(const QNetworkCookie &cookie, QWidget *parent = nullptr);
    void setHighlighted(bool enabled);
signals:
    void deleteClicked();
    void viewClicked();
};

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QUrl &url);

private:
    bool containsCookie(const QNetworkCookie &cookie);

private slots:
    void handleCookieAdded(const QNetworkCookie &cookie);
    void handleSaveAllClicked();
    void handleLogOutClicked();

private:
    QWebEngineCookieStore *m_store;
    QVector<QNetworkCookie> m_cookies;
    QVBoxLayout *m_layout;
};

#endif // MAINWINDOW_H
