#ifndef SUN_H
#define SUN_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class Sun;
}

class Sun : public QWidget
{
    Q_OBJECT

public:
    explicit Sun(QWidget *parent = 0);
    ~Sun();

protected:

    void paintEvent(QPaintEvent *event);

private:
    Ui::Sun *ui;

};

#endif // SUN_H
