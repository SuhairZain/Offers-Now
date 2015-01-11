/*
 * LoadingDialog.h
 *
 *  Created on: 10-Jan-2015
 *      Author: SuhairZain
 */

#ifndef LOADINGDIALOG_H_
#define LOADINGDIALOG_H_

#include <bb/cascades/Dialog>
#include <bb/cascades/Label>

using namespace bb::cascades;

class LoadingDialog: public Dialog
{
Q_OBJECT
signals:
    void cancel();
private slots:
    void onCancelClicked();
private:
    Label *messageLabel;
public:
    LoadingDialog();

    void setText(QString);
};

#endif /* LOADINGDIALOG_H_ */
