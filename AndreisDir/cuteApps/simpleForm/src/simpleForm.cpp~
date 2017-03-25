 #include "simpleForm.h" 
 #include <QtGui>
 
 SimpleForm::SimpleForm(QWidget *parent)
     : QWidget(parent)
 {
     QLabel *firstNameLable = new QLabel(tr("First name"));
     firstNameLine = new QLineEdit;

     QLabel *secondNameLable = new QLabel(tr("Second name"));
     secondNameLine = new QLineEdit;

     QLabel *phoneLable = new QLabel(tr("Phone number"));
     phoneLine = new QLineEdit;

     QLabel *emailLable = new QLabel(tr("Email"));
     emailLine = new QLineEdit;

     QLabel *backgroundLable = new QLabel(tr("Tell us about your programming experience"));
     backgroundText = new QTextEdit;

     QLabel *motivationLable = new QLabel(tr("Why do you want to study at the CS center?"));
     motivationText = new QTextEdit;


     QGroupBox *citiesBox = new QGroupBox(tr("City")); 
     QRadioButton *Novosibirsk = new QRadioButton(tr("Novosibirsk")); 
     QRadioButton *SaintPetersburg = new QRadioButton(tr("Saint Petersburg")); 
     
     QGridLayout *mainLayout = new QGridLayout;
     QVBoxLayout *vbox = new QVBoxLayout;

     mainLayout->addWidget(firstNameLable, 0, 0);
     mainLayout->addWidget(firstNameLine, 0, 1);
     mainLayout->addWidget(secondNameLable, 1, 0);
     mainLayout->addWidget(secondNameLine, 1, 1);
     mainLayout->addWidget(phoneLable, 2, 0);
     mainLayout->addWidget(phoneLine, 2, 1);
     mainLayout->addWidget(emailLable, 3, 0);
     mainLayout->addWidget(emailLine, 3, 1);
     mainLayout->addWidget(backgroundLable, 4, 0);
     mainLayout->addWidget(backgroundText, 4, 1);
     mainLayout->addWidget(motivationLable, 5, 0);
     mainLayout->addWidget(motivationText, 5, 1);
     mainLayout->addWidget(motivationText, 5, 1);
     mainLayout->addWidget(citiesBox, 6, 1, Qt::AlignHCenter);
     vbox->addWidget(Novosibirsk);
     vbox->addWidget(SaintPetersburg);
     vbox->addStretch(1);
     citiesBox->setLayout(vbox);
     
     setLayout(mainLayout);
     setWindowTitle(tr("Simple form"));
     setLayout(vbox);

     
     
}
