#ifndef SOLVING_H
#define SOLVING_H

#include  <QtWidgets/qdialog.h>
#include  <QTime>

#include <QMouseEvent>
#include "ActionStruct.h"

class   QLabel;
class   QPushButton;
class   QMenu;
class   QMenuBar;
class   QLineEdit;
////////////////////////////// ����������
#define SEGMENT  10          // ʱ���Ϊ 10��
#define MAXSOURCENUM  100    // ������Դ�������

class MyTimeSolveProject : public QDialog
{
    Q_OBJECT

public:
    MyTimeSolveProject(QDialog *parent = 0);

private:
    int           CountTime;
    int           ActionNum;
    int           BeginTime;
    int           EndTime;
    int           Times;

    int           CountMouseClicked;
    int           LastY;
    int           LastX;

    QPushButton   *FileOpenBtn;
    QPushButton   *AddBtn;
    QPushButton   *ModifBtn;
    QPushButton   *DeleteBtn;
    QPushButton   *SaveBtn;

    QList <Resource >  SourceArray;

    QLabel        *timelabel;
    QLabel        *SetTimelabel;
    QLineEdit     *SetTimeEdit;
    QPushButton   *SetTimeBtn;
    QTime         ChangingTime;
    QLabel        *label;
    QLabel        *ShowText;
    QPushButton   *ShowTime;
    QList          <ElapsedTimeStruct *> TimeElaped;

private:
    void  CreateModifActionMenu();
    void  ReadFile();         // ��ȡ÷�����滮�õ��ļ���
    void  paintEvent(QPaintEvent *);
    void  mousePressEvent(QMouseEvent *);


private
    slots:
        void  LoadAction();
        void  ModifAction();             //�޸�action��
        void  DeleteAction();            //ɾ��action��
        void  AddAction();               //����action��
        void  SetTimes();                //����ʱ�䱶�ʡ�
        void  SaveAction();              // �����޸ĺ���ļ�����Ϊ���滮�ļ�������÷������֤�ý��Ƿ����
};

#endif // SOLVING_H
