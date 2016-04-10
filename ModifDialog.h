#ifndef MODIFDIALOG_H
#define MODIFDIALOG_H
#include <QDialog>
#include "ActionStruct.h"
class QLabel;
class QPushButton;
class QLineEdit;

class MDialog: public QDialog
{
    Q_OBJECT
private:
    QLabel       *ActionNameLabel;
    QLabel       *BeginTimeLabel;
    QLabel       *EndTimeLabel;
    QLabel       *SourceLabel;
    QLabel       *SourceNumLabel;

    QLineEdit    *ActionNameEdit;
    QLineEdit    *BeginTimeEdit;
    QLineEdit    *EndTimeEdit;
    QLineEdit    *SourceEdit;
    QLineEdit    *SourceNumEdit;

    QPushButton  *ModifBtn;
    QPushButton  *DeleteBtn;
    QPushButton  *CloseBtn;
    QPushButton  *SourceBtn;
    QList <Resource *>  AddSource;

    int FindAction(QString str);

public:
    MDialog(QDialog *parent = 0);

private slots:
    void SlotModif();
    void SlotDelete();
    void SlotNextSource();
};

#endif // MODIFDIALOG_H
