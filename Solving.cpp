#include "Solving.h"
#include "ModifDialog.h"
#include "AddDialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QTime>
#include <QApplication>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QValidator>

QList  <ActionAttribute *>   Action;
bool   LoadActionFlag;

MyTimeSolveProject::MyTimeSolveProject(QDialog *parent):QDialog(parent)    // 构造函数
{
    CountMouseClicked = 0;
    LastY = -1;

    resize(800,550);
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint);
//        this->setGeometry((QApplication::desktop()->width() - width())/2,
//           (QApplication::desktop()->height() - this->height())/2, 600, 400); //设置窗口居中

    this->setFocus();                  // 设置主窗口为焦点。

    CreateModifActionMenu();           // 创建增加，修改，删除栏。

    ShowText = new QLabel("Simple Gantt Chart Demo", this);
    ShowText->setStyleSheet("color:white; background-color:rgba(255,125,255,255)");
    ShowText->setFont(QFont("Times",20,QFont::Bold));
    ShowText->setAlignment(Qt::AlignCenter);
    LoadActionFlag = false;
    this->setUpdatesEnabled(true);
}

void MyTimeSolveProject::ReadFile()         // 读文件
{
    QFile   fRead("solved.txt");
    if(fRead.open(QIODevice::ReadOnly | QIODevice::Text))  // 读文件是否成功
    {
     QTextStream  tRead(&fRead);
     ActionAttribute *Temp;

     tRead.readLine();                      // <begin_plan>
     tRead.readLine();                      // <begin_num_actions>
     ActionNum = tRead.readLine().toInt();  // 记录Action的个数。
     tRead.readLine();                      // <end_num_actions>

     while(!tRead.atEnd())
     {
        tRead.readLine();                   // <begin_action>
        Temp = new ActionAttribute;
        Temp->ActionName = tRead.readLine();          // #动作名
        Temp->BeginTime  = tRead.readLine().toInt();  // #动作开始时刻
        Temp->LastTime   = tRead.readLine().toInt();  // #动作的持续时长

        label = new QLabel(this);                     // 标签
        label->setText(Temp->ActionName);

        ShowTime = new QPushButton(this);     // 显示时间推移控件

        Temp->btn   = ShowTime;
        Temp->label = label;

        // 读取资源情况。
        tRead.readLine();          //  <resource_num>
        int RecondSourceNum = tRead.readLine().toInt();          //  num
        tRead.readLine();          //  <end_resource_num>

        Resource *tmp_source;

        for(int i = 0; i < RecondSourceNum; i++)
        {
          tmp_source = new Resource;
          tmp_source->SourceName = tRead.readLine();
          tmp_source->SourceNum  = tRead.readLine().toInt();
          Temp->source.push_back(tmp_source);
        }
        tRead.readLine();         // <all_resource>

        if(!tRead.atEnd())
        {           
          Action.push_back(Temp);
        }       
        tRead.readLine();                  // <end_action>
     }
    }
    else
    {
        QMessageBox::warning(NULL, "Open File Fail!",
            tr("Open File %1 Fail! \n%2").arg(fRead.fileName()).arg(fRead.errorString()));
    }

    fRead.close();
}

void MyTimeSolveProject::CreateModifActionMenu()
{
    FileOpenBtn = new QPushButton(tr("&Load File"),this);
    AddBtn      = new QPushButton(tr("&Add Action"),this);
    ModifBtn    = new QPushButton("&Modif Action", this);
    DeleteBtn   = new QPushButton("&Delete Action", this);
    SaveBtn     = new QPushButton("&FileSave", this);

    FileOpenBtn->setStyleSheet("color: white;background-color:rgba(100,200,255,255)");
    FileOpenBtn->setFont(QFont("Times",12, QFont::Bold));
    AddBtn->setStyleSheet("color: white;background-color:rgba(100,200,255,255)");
    AddBtn->setFont(QFont("Times",12, QFont::Bold));
    ModifBtn->setStyleSheet("color: white;background-color:rgba(100,200,255,255)");
    ModifBtn->setFont(QFont("Times",12, QFont::Bold));
    DeleteBtn->setStyleSheet("color: white;background-color:rgba(100,200,255,255)");
    DeleteBtn->setFont(QFont("Times",12, QFont::Bold));
    SaveBtn->setStyleSheet("color: white;background-color:rgba(100,200,255,255)");
    SaveBtn->setFont(QFont("Times",12, QFont::Bold));

    connect(FileOpenBtn,SIGNAL(clicked()),this, SLOT(LoadAction()));
    connect(AddBtn,   SIGNAL(clicked()),this, SLOT(AddAction()));
    connect(ModifBtn, SIGNAL(clicked()),this, SLOT(ModifAction()));
    connect(DeleteBtn,SIGNAL(clicked()),this, SLOT(DeleteAction()));
    connect(SaveBtn,SIGNAL(clicked()),this, SLOT(SaveAction()));
}

void MyTimeSolveProject::SaveAction()
{
    QString FileName("ToBeSolving.txt");
    QFile   fWrite(FileName);
    if(fWrite.open(QIODevice::WriteOnly | QIODevice::Text))  // 文件是否成功
    {
     QTextStream  tWrite(&fWrite);

     int len = Action.size();

     tWrite<<"<begin_plan>"<<"\n";             // <begin_plan>
     tWrite<<"<begin_num_actions>\n";          // <begin_num_actions>
     tWrite<<QString::number(len, 10)<<endl;   // 记录Action的个数。
     tWrite<<"<end_num_actions>"<<endl;        // <end_num_actions>

     for(int i = 0; i < len; i++)
     {
         tWrite<<"<begin_action>"<<endl;
         tWrite<<Action[i]->ActionName<<endl;
         tWrite<<QString::number(Action[i]->BeginTime)<<endl;
         tWrite<<QString::number(Action[i]->LastTime)<<endl;

         tWrite<<"resource_num>"<<endl;
         tWrite<<QString::number(Action[i]->source.size())<<endl;
         tWrite<<"<end_resource_num>"<<endl;

         for(int index = 0; index < Action[i]->source.size(); index++)
         {
             tWrite<<Action[i]->source[index]->SourceName<<endl;
             tWrite<<QString::number(Action[i]->source[index]->SourceNum)<<endl;
         }
         tWrite<<"<all_resource>"<<endl;
         tWrite<<"<end_action>"<<endl;
     }

     tWrite<<"<end_plan>";

    fWrite.close();
    QMessageBox message(QMessageBox::NoIcon, "Save File Box",
        tr("Save File in \"%1\" successfully! \n Do you want to exit?").arg(FileName),
        QMessageBox::Yes | QMessageBox::No, NULL);

     if(message.exec() == QMessageBox::Yes)
     {
         this->close();
     }

}
    ActionAttribute *temp;                   // 保存文件后释放Action分配的内存。
    for(int i = 0; i < Action.size(); i++)
    {
        temp = Action[i];
        delete temp;
    }
}

void MyTimeSolveProject::paintEvent(QPaintEvent *) // 界面。
{
   int  i = 0;
   int  WindowWidth  = width();
   int  MenuWidth  = WindowWidth / 5;

   if(Action.size() > 0)
   {
       BeginTime = Action[0]->BeginTime;
       EndTime   = Action[0]->BeginTime + Action[0]->LastTime;

       for(i=1; i < Action.size(); i++)
       {
         int endtime = Action[i]->BeginTime + Action[i]->LastTime;
         if(BeginTime > Action[i]->BeginTime)
         {
           BeginTime = Action[i]->BeginTime;
         }

         if(EndTime < endtime)
         {
            EndTime =  endtime;
         }

       }

       int  TimeElapedSize = TimeElaped.size()-1;
       int  StartX  =  WindowWidth>>3;
       int  EndX    = (WindowWidth>>6)*55;
       int  EndSourceX    = (WindowWidth>>3)*5;
       int  XActionLength = (EndX - StartX)/(EndTime - BeginTime);
       int  SegmentTime = (EndTime - BeginTime)/SEGMENT;
       int  XLength = SegmentTime * XActionLength;

       //  设置时间精度
       CountTime = 0;
       if(TimeElaped.isEmpty())   // 调整时间倍率后，逝去时间的记录方法。T = t1*last1 + t2*last2...
       {
         CountTime = ChangingTime.elapsed()/1000 + BeginTime;
       }
       else
       {
           for(i = 0; i < TimeElapedSize; i++)
           {
              if(i==0)
              {
                CountTime += TimeElaped[i]->ElapedTime*TimeElaped[i]->ElapedTimes;
              }
              else
              {
                 CountTime += (TimeElaped[i]->ElapedTime - TimeElaped[i-1]->ElapedTime)*TimeElaped[i]->ElapedTimes;
              }
           }

           CountTime += (ChangingTime.elapsed()/1000 - TimeElaped[i-1]->ElapedTime)*TimeElaped[i]->ElapedTimes+BeginTime;
        }

       timelabel->setText(QString::number(CountTime,10)+=" s");

       // 图形化  坐标轴和时态Action图
       QPainter  pa(this);
       setStyleSheet("color:blue");
       setFont(QFont("Times",12,QFont::Bold));

       pa.drawText(StartX,65,80,15, Qt::AlignLeft, QString::number(BeginTime));

       for(i = 0; i <= SEGMENT; i++)
       {
           pa.drawLine(StartX + i*XLength, 88, StartX + (i+1)*XLength, 88);
           pa.drawLine(StartX + (i+1)*XLength, 88, StartX + (i+1)*XLength, 85);
           pa.drawText(StartX + (i+1)*XLength, 65, XLength, 15, Qt::AlignLeft,
                       QString::number(BeginTime + (i+1)*SegmentTime));
       }
       pa.drawLine(StartX + i*XLength, 88, StartX + (i+1)*XLength, 88);

       const int Htriangle[3][2] =
       {
         {StartX + (i+1)*XLength, 90},
         {StartX + (i+1)*XLength, 86},
         {StartX + (i+1)*XLength+2, 88}
       };
       QPen thickPen(palette().foreground(), 1.5);
       QPen thinPen(palette().foreground(), 0.5);
       pa.setPen(thinPen);
       pa.setBrush(palette().foreground());
       pa.drawPolygon(QPolygon(3, &Htriangle[0][0]));//绘制一个小的倒三角形

       for(i = 0; i < Action.size(); i++)            // 绘制
       {
           pa.drawLine(StartX, 88+i*30, StartX, 88+(i+1)*30);
           pa.drawLine(StartX, 88+(i+1)*30, StartX-3, 88+(i+1)*30);
           Action[i]->label->setFont(QFont("Times",12,QFont::Bold));
           Action[i]->label->setAlignment(Qt::AlignRight);
           Action[i]->label->show();             //  不加show，新增的Action不能显示。
           Action[i]->btn->setFocusPolicy(Qt::NoFocus);

           switch(i%3)
           {
           case 0:
              Action[i]->label->setStyleSheet("color:red");
              Action[i]->btn->setStyleSheet("background-color:red");
              break;

           case 1:
              Action[i]->label->setStyleSheet("color:green");
              Action[i]->btn->setStyleSheet("background-color:green");
              break;

           case 2:
              Action[i]->label->setStyleSheet("color:blue");
              Action[i]->btn->setStyleSheet("background-color:blue");
              break;
          }

           if(i!=0)
           {
               Action[i]->label->setGeometry(0,90+i*30,StartX-10,20);
           }
           else
           {
               Action[0]->label->setGeometry(0,90,StartX-6,20);
           }

           if(CountTime < Action[i]->BeginTime+Action[i]->LastTime)
           {
               Action[i]->btn->setGeometry(StartX+XActionLength*(Action[i]->BeginTime-BeginTime),90+i*30,
              XActionLength*(CountTime-Action[i]->BeginTime),29);
           }
           else
           {
               Action[i]->btn->setGeometry(StartX+XActionLength*(Action[i]->BeginTime-BeginTime),90+i*30,
              XActionLength*Action[i]->LastTime,29);
           }
           Action[i]->btn->show();   // 不加show，新增的Action不能显示。
       }
       pa.drawLine(StartX, 88+i*30, StartX, 100+i*30);

       const int Vtriangle[3][2] =
       {
         {StartX-2, 100+i*30},
         {StartX+2, 100+i*30},
         {StartX, 102+i*30}
       };

       pa.drawPolygon(QPolygon(3, &Vtriangle[0][0]));//绘制一个小的倒三角形


       /////////// 绘制资源图 ///////////////////////////////////////////////////////////////
       //    pa.drawRect(0,400,10+CountTime,20);
       ////////////////////////////////////////////////////////////////////////////////////

       QList <Resource >  SourceArray;
       Resource  TempSource;    

       int ActionIndex = 0, SourceIndex = 0;
       // 先把所有不同的资源都加入 SourceArray中. 如果预先就知道要消耗的所有资源，则可以略去这一步。

       TempSource.SourceName = Action[0]->source[0]->SourceName;  //bug：每个action必须耗费资源。如果action[0]不耗费资源则出现段错误
       TempSource.SourceNum  = 0;

       SourceArray.push_back(TempSource);

       for(ActionIndex = 0; ActionIndex < Action.size(); ActionIndex++)
       {
         for(SourceIndex=0; SourceIndex < Action[ActionIndex]->source.size(); SourceIndex++)
         {
           bool IfInArray = false;
           for(int j = 0; j < SourceArray.size(); j++)
           {
               if(SourceArray[j].SourceName == Action[ActionIndex]->source[SourceIndex]->SourceName)
               {
                   IfInArray = true;
                   break;
               }
           }

           if(!IfInArray)
           {
           TempSource.SourceName = Action[ActionIndex]->source[SourceIndex]->SourceName;
           TempSource.SourceNum  = 0;
           SourceArray.push_back(TempSource);
           }
         }
       }

       // 根据逝去的时间，判断某个action是否执行完了。是，则把耗费的资源加入SourceArray中。
       QList <int> RecordEndAction;
       for(ActionIndex=0; ActionIndex < Action.size(); ActionIndex++)
       {  
           if(CountTime > Action[ActionIndex]->BeginTime + Action[ActionIndex]->LastTime)
           {
              // 过滤掉已经执行完的Action
               bool  ifEnd = false;
               for(int j = 0; j < RecordEndAction.size(); j++)
               {
                 if(RecordEndAction[j] == ActionIndex)
                 {
                     ifEnd = true;
                     break;
                 }
               }

               if(ifEnd)             // 如果为真，即该标志Action的资源消耗已经记录过了，执行下个循环。
               {
                   continue;
               }

               //  记录消耗的资源。
               for(SourceIndex=0; SourceIndex < Action[ActionIndex]->source.size(); SourceIndex++)
               {
                  for(int k = 0; k < SourceArray.size(); k++)
                  {
                    if(SourceArray[k].SourceName == Action[ActionIndex]->source[SourceIndex]->SourceName)
                    {
                      SourceArray[k].SourceNum += Action[ActionIndex]->source[SourceIndex]->SourceNum;
                      RecordEndAction.push_back(ActionIndex);
                    }
                  }
               }
           }
       }

//        测试加入资源不同的个数。
//        pa.drawText(400,400,50,20,Qt::AlignCenter,QString::number(SourceArray.size(),10));

       ///// 图形化 消耗的资源 以消耗资源的num作为控件显示长度的标准。     
       int  XSourceLength  = (EndSourceX -StartX) / MAXSOURCENUM;  // 单位num长度
       int  XSegmentSourceLength = XSourceLength * SEGMENT;        // 段长度

       int j = 0;         // index
       for(j = 0; j < SEGMENT; j++)
       {
          pa.drawLine(StartX + XSegmentSourceLength * j, 148+(i+1)*30, StartX+ XSegmentSourceLength * (j+1), 148+(i+1)*30);
          pa.drawLine(StartX+ XSegmentSourceLength * (j+1), 148+(i+1)*30, StartX+ XSegmentSourceLength * (j+1), 146+(i+1)*30);
          pa.drawText(StartX + XSegmentSourceLength * j, 126+(i+1)*30,XSegmentSourceLength,15,Qt::AlignLeft,QString::number(j*SEGMENT));
       }
       pa.drawText(StartX + XSegmentSourceLength * j, 126+(i+1)*30,XSegmentSourceLength,15,Qt::AlignLeft,QString::number(j*SEGMENT));

       const int HSourceTriangle[3][2] =
       {
         {StartX + XSegmentSourceLength * j, 150+(i+1)*30},
         {StartX + XSegmentSourceLength * j, 146+(i+1)*30},
         {StartX + XSegmentSourceLength * j+2, 148+(i+1)*30}
       };
       pa.drawPolygon(QPolygon(3, &HSourceTriangle[0][0]));//绘制一个小的倒三角形

       for(j =0; j < SourceArray.size(); j++)
       {
        pa.drawLine(StartX, 148+(i+1)*30 + j*30, StartX, 148+(i+1)*30 + (j+1)*30);
        pa.drawLine(StartX, 148+(i+1)*30 + (j+1)*30, StartX-3, 148+(i+1)*30 + (j+1)*30);
       }
       pa.drawLine(StartX, 148+(i+1)*30 + j*30, StartX, 160+(i+1)*30 + j*30);

       const int VSourceTriangle[3][2] =
       {
         {StartX-2, 160+(i+1)*30 + j*30},
         {StartX+2, 160+(i+1)*30 + j*30},
         {StartX, 162+(i+1)*30 + j*30}
       };

       pa.drawPolygon(QPolygon(3, &VSourceTriangle[0][0]));//绘制一个小的倒三角形

       for(j =0; j < SourceArray.size(); j++)
       {
           switch(j % 3 )
           {
           case 0:
               pa.setBrush(Qt::red);
               break;
           case 1:
               pa.setBrush(Qt::blue);
               break;
           case 2:
               pa.setBrush(Qt::green);
               break;
           }

        pa.drawRect(StartX, 148+(i+1)*30 + j*30,SourceArray[j].SourceNum* XSourceLength,30);

        switch(j % 3 )
        {
        case 0:
            pa.setPen(Qt::red);
            break;
        case 1:
            pa.setPen(Qt::blue);
            break;
        case 2:pa.setPen(Qt::green);
            break;
        }

        pa.drawText(0,148+(i+1)*30 + j*30, StartX-6,30,Qt::AlignRight,SourceArray[j].SourceName);
        pa.setPen(Qt::blue);   
       }

       pa.drawText(EndSourceX + 70, 340, 120, 30, Qt::AlignLeft,"Current Time:");
       timelabel->setGeometry(EndSourceX + 182, 330, 120, 50);
       timelabel->setFont(QFont("Times",20,QFont::Bold));
       timelabel->show();

       SetTimelabel->setGeometry(EndSourceX + 70, 382,80,25);           // 设置时间倍率
       SetTimelabel->show();
       SetTimeEdit->setGeometry(EndSourceX + 150, 378,40,25);
       SetTimeEdit->show();
       pa.drawText(EndSourceX + 190,382,70,15,Qt::AlignLeft,"s");
       SetTimeBtn->setGeometry(EndSourceX + 210, 378,70,25);
       SetTimeBtn->show();
   }

   ShowText->setGeometry(0, 20, width(), 32);   // Simple Gantt Chart Demo
   FileOpenBtn->setGeometry(0,0, MenuWidth,20);
   AddBtn->setGeometry(MenuWidth,0, MenuWidth,20);
   ModifBtn->setGeometry(MenuWidth<<1,0, MenuWidth,20);
   DeleteBtn->setGeometry(MenuWidth*3,0, MenuWidth,20);
   SaveBtn->setGeometry(MenuWidth<<2,0, MenuWidth,20);
   SaveBtn->show();

}

void MyTimeSolveProject::mousePressEvent(QMouseEvent *event)
{

    int  x = event->x();
    int  y = event->y();
    int  WindowWidth = width();
    int  XLength = ((WindowWidth>>6)*55 - (WindowWidth>>3))/(EndTime - BeginTime);
    int  startX = WindowWidth>>3;

    // y 值 90+i*30
    if(x >= startX)
    {
      if((y <= (90 + Action.size()*30))&& (y >= 90))
      {
        if(LastY == -1)                                 // 第一次有效事件记录
        {
            LastY = y;
            LastX = x;
        }

        CountMouseClicked++;
      }
    }

    if((CountMouseClicked != 0)&&(CountMouseClicked % 2 == 0))  // 第二次有效事件  模拟双击事件
    {
       int  index = (y - 90) / 30;
       int  LastIndex = (LastY - 90) / 30;

       if((LastX != x)&&(LastIndex == index))           // 两次事件，鼠标按下点不同。
       {
         int  TempStartTime = (x - startX)/ XLength;
         Action[index]->BeginTime = TempStartTime + BeginTime;
         this->repaint();
       }

       LastY = -1;
    }
}


void MyTimeSolveProject::LoadAction()
{
    if(!LoadActionFlag)
    {
        ReadFile();

        if(Action.empty())
        {
           QMessageBox::question(NULL, "Remind: ", "There is no action!");
        }
        else
        {
            ChangingTime.start();              // 计时开始。
            this->setFocus();                  // 设置主窗口为焦点。

            timelabel = new QLabel(this);            // 显示时间
            timelabel->setAlignment(Qt::AlignLeft);

            Times = 1;
            SetTimelabel = new QLabel("Set Times:",this);          // 设置时间倍率  1秒*倍率。
            SetTimeEdit = new QLineEdit("1",this);
            SetTimeEdit->setAlignment(Qt::AlignRight);
            SetTimeBtn = new QPushButton("&Set",this);
            connect(SetTimeBtn,SIGNAL(clicked()),this,SLOT(SetTimes()));  //加入时间
            QRegExpValidator  *regexpvalidator = new QRegExpValidator(QRegExp("[0-9]{0,4}"),this);
            SetTimeEdit->setValidator(regexpvalidator);
            SetTimelabel->setFont(QFont("Times",12, QFont::Bold));
            SetTimeEdit->setFont(QFont("Times",12, QFont::Bold));
            SetTimeBtn->setFont(QFont("Times",12, QFont::Bold));
        }

        LoadActionFlag = true;
    }
    this->setFocus();
    this->update();
}

void MyTimeSolveProject::AddAction()
{
    if(LoadActionFlag)
    {
        AddDlg  *Dlg = new AddDlg(this);
        Dlg->exec();
        this->setFocus();
        this->update();
    }
    else
    {
      QMessageBox::warning(NULL, "Add Box", tr("Please load file first!"),QMessageBox::Yes);
    }
}

void MyTimeSolveProject::DeleteAction()
{
    if(LoadActionFlag)
    {
        MDialog *ModifDlg = new MDialog(this);
        ModifDlg->exec();
        this->setFocus();
        this->update();
    }
    else
    {
      QMessageBox::warning(NULL, "Delete Box", tr("Please load file first!"),QMessageBox::Yes);
    }
}

void MyTimeSolveProject::ModifAction()
{
    if(LoadActionFlag)
    {
        MDialog *ModifDlg = new MDialog(this);
        ModifDlg->exec();
        this->setFocus();
        this->update();
    }
    else
    {
        QMessageBox::warning(NULL, "Modif Box", tr("Please load file first!"),QMessageBox::Yes);
    }
}

void MyTimeSolveProject::SetTimes()
{
    if(!SetTimeEdit->text().isEmpty())
    {
      int TempTimes = SetTimeEdit->text().toInt();
      if(Times != TempTimes)   // 如果设置的时间倍率与之前不一样
      {
          ElapsedTimeStruct *temp = new ElapsedTimeStruct;
          temp->ElapedTimes = Times;          // 倍率

          temp->ElapedTime  = ChangingTime.elapsed()/1000;

          TimeElaped.push_back(temp);

          Times = TempTimes;
          temp = new ElapsedTimeStruct;
          temp->ElapedTimes = Times;
          TimeElaped.push_back(temp);

          SetTimeEdit->setText(QString::number(Times,10));
     }
    }
}


