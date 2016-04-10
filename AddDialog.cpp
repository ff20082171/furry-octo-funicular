#include "AddDialog.h"
#include "Solving.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>
#include <QMouseEvent>

extern QList     <ActionAttribute *>   Action;
extern bool      LoadActionFlag;

AddDlg::AddDlg(QDialog *parent) : QDialog(parent)
{
    this->resize(300,150);
    this->setWindowTitle("Add Action Dialog");

    ActionNameLabel = new QLabel("ActionName", this);
    BeginTimeLabel  = new QLabel("BeginTime", this);
    EndTimeLabel    = new QLabel("LastTime", this);
    SourceLabel    = new QLabel("ResourceName", this);
    SourceNumLabel    = new QLabel("ResourceNum", this);

    ActionNameEdit  = new QLineEdit;
    BeginTimeEdit   = new QLineEdit;
    EndTimeEdit     = new QLineEdit;
    SourceEdit     = new QLineEdit;
    SourceNumEdit     = new QLineEdit;

    AddBtn   = new QPushButton("&Add", this);
    CloseBtn   = new QPushButton("&Close", this);
    SourceBtn   = new QPushButton("&NextSource", this);

    ActionNameLabel->setFont(QFont("Times",12,QFont::Bold));
    BeginTimeLabel->setFont(QFont("Times",12,QFont::Bold));
    SourceLabel->setFont(QFont("Times",12,QFont::Bold));
    EndTimeLabel->setFont(QFont("Times",12,QFont::Bold));
    SourceNumLabel->setFont(QFont("Times",12,QFont::Bold));
    ActionNameEdit->setFont(QFont("Times",12,QFont::Bold));
    BeginTimeEdit->setFont(QFont("Times",12,QFont::Bold));
    EndTimeEdit->setFont(QFont("Times",12,QFont::Bold));
    SourceNumEdit->setFont(QFont("Times",12,QFont::Bold));
    SourceEdit->setFont(QFont("Times",12,QFont::Bold));
    AddBtn->setFont(QFont("Times",12,QFont::Bold));
    CloseBtn->setFont(QFont("Times",12,QFont::Bold));
    SourceBtn->setFont(QFont("Times",12,QFont::Bold));

    QRegExpValidator  *regexpvalidator = new QRegExpValidator(QRegExp("[0-9]{0,4}"),this);
    EndTimeEdit->setValidator(regexpvalidator);
    BeginTimeEdit->setValidator(regexpvalidator);
    SourceNumEdit->setValidator(regexpvalidator);

    QGridLayout  *layout = new QGridLayout;
    layout->addWidget(ActionNameLabel,0,0);
    layout->addWidget(BeginTimeLabel,1,0);
    layout->addWidget(EndTimeLabel,2,0);
    layout->addWidget(SourceLabel,3,0);
    layout->addWidget(SourceNumLabel,4,0);
    layout->addWidget(ActionNameEdit,0,1);
    layout->addWidget(BeginTimeEdit,1,1);
    layout->addWidget(EndTimeEdit,2,1);
    layout->addWidget(SourceEdit,3,1);
    layout->addWidget(SourceNumEdit,4,1);
    layout->addWidget(AddBtn,0,3);
    layout->addWidget(CloseBtn,4,3);
    layout->addWidget(SourceBtn,3,3);

    this->setLayout(layout);
    connect(CloseBtn,SIGNAL(clicked()), this, SLOT(close()));
    connect(AddBtn,SIGNAL(clicked()), this, SLOT(SlotAdd()));
    connect(SourceBtn,SIGNAL(clicked()), this, SLOT(SlotAddNextSource()));
}

void AddDlg::SlotAdd()
{
    if((!ActionNameEdit->text().isEmpty())&&(!BeginTimeEdit->text().isEmpty())
        &&(!EndTimeEdit->text().isEmpty()))
    {
        if(AddSource.empty())
        {
          QMessageBox::question(NULL, "Remind: ", "Resource can't be empty!");
        }
        else
        {
        if(!FindAction(ActionNameEdit->text()))  // 不在，就增加；
        {
          ActionAttribute *TempAction = new ActionAttribute;
          TempAction->ActionName = ActionNameEdit->text();
          TempAction->BeginTime  = BeginTimeEdit->text().toInt();
          TempAction->LastTime   = EndTimeEdit->text().toInt();
          QLabel        *TempLabel = new QLabel(ActionNameEdit->text(), (QDialog *)this->parentWidget());  //设置为父窗口的子控件
          QPushButton   *TempBtn   = new QPushButton((QDialog *)this->parentWidget());
          TempAction->label = TempLabel;
          TempAction->btn   = TempBtn;
          TempAction->source = AddSource;

          Action.push_back(TempAction);   

          QMessageBox::question(NULL, "Notif: ",
                                tr("Number of actions is %1. \n The new Action is \"%2\"").arg(QString::number(Action.size())).arg(Action[Action.size()-1]->ActionName));

          QMessageBox message(QMessageBox::NoIcon, "Add Box",
              tr("Add message successfully!\n Do you want to exit?"),
              QMessageBox::Yes | QMessageBox::No, NULL);

           if(message.exec() == QMessageBox::Yes)
           {
               this->close();
           }
          }
          else
          {
            QMessageBox message(QMessageBox::NoIcon, "Add Box",
                  tr("The action \"%1\" is exist!\n Do you want to exit?").arg(ActionNameEdit->text()),
                  QMessageBox::Yes | QMessageBox::No, NULL);

            if(message.exec() == QMessageBox::Yes)
            {
               this->close();
            }
          }
      }
    }
    else
    {
        QMessageBox::question(NULL, "Remind: ", "Text can't be empty!");
    }
}

bool AddDlg::FindAction(QString str)
{
    int size = Action.size();
    for(int index = 0; index < size; index++)
    {
        if(Action[index]->ActionName == str)
        {
            return true;
        }
    }
    return false;
}

void AddDlg::SlotAddNextSource()
{
    if((!this->SourceEdit->text().isEmpty())&&(!this->SourceNumEdit->text().isEmpty()))
    {
        Resource *TmpSource = new Resource;
        TmpSource->SourceName = this->SourceEdit->text();
        TmpSource->SourceNum = this->SourceNumEdit->text().toInt();

       if(AddSource.size() > 0)
       {
           bool IfIn = false;

           for(int i = 0; i < AddSource.size(); i++)
           {
               if(AddSource[i]->SourceName == TmpSource->SourceName)
               {
                   IfIn = true;
               }

               if(IfIn)
               {
                   break;
               }
           }

           if(!IfIn)
           {
             AddSource.push_back(TmpSource);
           }
           else
           {
              QMessageBox::question(NULL, "Remind: ", "The resource has been added!\nPlease add other resource!");
           }
       }
       else
       {
           AddSource.push_back(TmpSource);
       }
    }
}

