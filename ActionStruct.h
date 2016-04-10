#ifndef ACTIONSTRUCT_H
#define ACTIONSTRUCT_H
#include <QList>
class QLabel;
class QPushButton;
class QString;

typedef struct  ResourceStruct            // ÿ��action�����Ķ�����Դ
{
  QString  SourceName;
  int      SourceNum;
} Resource;

typedef struct Action_Attribute           // action����
{
    QString      ActionName;
    int          BeginTime;
    int          LastTime;
    QLabel       *label;
    QPushButton  *btn;
    QList <Resource *>  source;
}ActionAttribute;

typedef struct StructTimeElaped          // ʱ�侫��
{
    int ElapedTime;
    int ElapedTimes;
}ElapsedTimeStruct;

#endif // ACTIONSTRUCT_H
