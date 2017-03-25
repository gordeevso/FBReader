 #ifndef SIMPLEFORM_H
 #define SIMPLEFORM_H

 #include <QGroupBox>
 #include <QPushButton>
 #include <QRadioButton>
 #include <QWidget>

 class QLabel;
 class QLineEdit;
 class QTextEdit;

 class SimpleForm : public QWidget
 {
     Q_OBJECT

 public:
     SimpleForm(QWidget *parent = 0);

 private:
     QLineEdit *firstNameLine;
     QLineEdit *secondNameLine;
     QLineEdit *phoneLine;
     QLineEdit *emailLine;
     QTextEdit *backgroundText;
     QTextEdit *motivationText; 
     QPushButton *applyButton;
     QGroupBox *citiesBox();
     QRadioButton *radio1();
     QRadioButton *radio2();

 };

#endif
