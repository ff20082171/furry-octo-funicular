#ifndef ACTIONSTRUCT_H
#define ACTIONSTRUCT_H
#include <QList>
class QLabel;
class QPushButton;
class QString;

typedef struct  ResourceStruct            // 每个action，消耗多种资源
{
  QString  SourceName;
  int      SourceNum;
} Resource;

typedef struct Action_Attribute           // action属性
{
    QString      ActionName;
    int          BeginTime;
    int          LastTime;
    QLabel       *label;
    QPushButton  *btn;
    QList <Resource *>  source;
}ActionAttribute;

typedef struct StructTimeElaped          // 时间精度
{
    int ElapedTime;
    int ElapedTimes;
}ElapsedTimeStruct;

#endif // ACTIONSTRUCT_H
