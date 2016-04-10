#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QtWidgets/qdialog.h>
#include "ActionStruct.h"
class QLabel;
class QPushButton;
class QLineEdit;
class QWidget;

class AddDlg: public QDialog
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

    QPushButton  *AddBtn;
    QPushButton  *CloseBtn;
    QPushButton  *SourceBtn;

    bool FindAction(QString str);
    QList <Resource *>  AddSource;

public:
//    AddDlg(QList <ActionAttribute *> *Action, QDialog *parent = 0);
    AddDlg(QDialog *parent = 0);

private slots:
    void SlotAdd();
    void SlotAddNextSource();

};

#endif // ADDDIALOG_H
