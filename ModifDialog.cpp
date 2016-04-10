#include "ModifDialog.h"
#include "Solving.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QRegExp>
#include <QValidator>
#include <QMessageBox>

extern QList    <ActionAttribute *>   Action;
extern bool     LoadActionFlag;

MDialog::MDialog(QDialog *parent) : QDialog(parent)
{
    this->resize(300,150);
 //   this->setParent(parent);            // 把主窗口的this指针传进来，设置add窗口为子窗口
    this->setWindowTitle("Modif Action Dialog");

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

    ModifBtn   = new QPushButton("&Modif", this);
    DeleteBtn  = new QPushButton("&Delete",this);
    CloseBtn   = new QPushButton("&Close", this);
    SourceBtn   = new QPushButton("&NextResource", this);

    ActionNameLabel->setFont(QFont("Times",12,QFont::Bold));
    BeginTimeLabel->setFont(QFont("Times",12,QFont::Bold));
    EndTimeLabel->setFont(QFont("Times",12,QFont::Bold));
    SourceLabel->setFont(QFont("Times",12,QFont::Bold));
    SourceNumLabel->setFont(QFont("Times",12,QFont::Bold));
    ActionNameEdit->setFont(QFont("Times",12,QFont::Bold));
    BeginTimeEdit->setFont(QFont("Times",12,QFont::Bold));
    EndTimeEdit->setFont(QFont("Times",12,QFont::Bold));
    SourceEdit->setFont(QFont("Times",12,QFont::Bold));
    SourceNumEdit->setFont(QFont("Times",12,QFont::Bold));
    ModifBtn->setFont(QFont("Times",12,QFont::Bold));
    DeleteBtn->setFont(QFont("Times",12,QFont::Bold));
    CloseBtn->setFont(QFont("Times",12,QFont::Bold));
    SourceBtn->setFont(QFont("Times",12,QFont::Bold));

    QRegExpValidator  *regexpvalidator = new QRegExpValidator(QRegExp("[0-9]{0,4}"),this);
    EndTimeEdit->setValidator(regexpvalidator);
    BeginTimeEdit->setValidator(regexpvalidator);

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
    layout->addWidget(ModifBtn,0,3);
    layout->addWidget(DeleteBtn,1,3);
    layout->addWidget(SourceBtn,3,3);
    layout->addWidget(CloseBtn,4,3);

    this->setLayout(layout);
    connect(CloseBtn,SIGNAL(clicked()), this, SLOT(close()));
    connect(ModifBtn,SIGNAL(clicked()), this, SLOT(SlotModif()));
    connect(DeleteBtn,SIGNAL(clicked()), this, SLOT(SlotDelete()));
    connect(SourceBtn,SIGNAL(clicked()), this, SLOT(SlotNextSource()));

}

void MDialog::SlotModif()
{
    if(!ActionNameEdit->text().isEmpty())
    {
        int index = FindAction(ActionNameEdit->text());
        if(index != -1)
        {
            if(!BeginTimeEdit->text().isEmpty()&&!EndTimeEdit->text().isEmpty())
            {
                Action[index]->ActionName = ActionNameEdit->text();
                Action[index]->BeginTime  = BeginTimeEdit->text().toInt();
                Action[index]->LastTime   = EndTimeEdit->text().toInt();
                Action[index]->source = AddSource;

                QMessageBox message(QMessageBox::NoIcon, "Modif Box",
                    tr("Modif message successfully!\n Do you want to exit?"),
                    QMessageBox::Yes | QMessageBox::No, NULL);

                 if(message.exec() == QMessageBox::Yes)
                 {
                     this->close();
                 }
            }
        }
        else
        {
            QMessageBox message(QMessageBox::NoIcon, "Modif Box",
                tr("Action named of \"%1\" is not here!\n Do you want to exit?").arg(ActionNameEdit->text()),
                QMessageBox::Yes | QMessageBox::No, NULL);

             if(message.exec() == QMessageBox::Yes)
             {
                 this->close();
             }
        }
    }
    else
    {
        QMessageBox::question(NULL, "Remind: ", "Text can't be empty!");
    }
}

void MDialog::SlotDelete()
{
    if(!ActionNameEdit->text().isEmpty())
    {
        int index = FindAction(ActionNameEdit->text());    // 找到索引
        if(index != -1)
        {
            delete Action[index]->label;
            delete Action[index]->btn;

            Action.removeAt(index);

            QMessageBox message(QMessageBox::NoIcon, "Delete Box",
                tr("Add message successfully!\n Do you want to exit?"),
                QMessageBox::Yes | QMessageBox::No, NULL);

             if(message.exec() == QMessageBox::Yes)
             {
                 this->close();
             }
        }
        else
        {
            QMessageBox message(QMessageBox::NoIcon, "Delete Box",
                tr("Action named of \"%1\" is not here!\n Do you want to exit?").arg(ActionNameEdit->text()),
                QMessageBox::Yes | QMessageBox::No, NULL);

             if(message.exec() == QMessageBox::Yes)
             {
                 this->close();
             }
        }
    }
    else
    {
        QMessageBox::question(NULL, "Remind: ", "Text can't be empty!");
    }
}

int MDialog::FindAction(QString str)
{
   int size = Action.size();
   for(int index = 0; index < size; index++)
   {
       if(Action[index]->ActionName == str)
       {
           return index;
       }
   }
   return -1;
}

void MDialog::SlotNextSource()
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

