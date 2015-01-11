/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <src/LoadingDialog.h>

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <bb/cascades/Button>
#include <bb/cascades/DropDown>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/Validator>
using namespace bb::cascades;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI();
private slots:
    void onCircleChanged(QVariant);
    void onOperatorChanged(QVariant);

    void onNumberGoClicked();
    void onOperatorAndCircleGoClicked();

    void onCancelCurrentOperation();
    void onNumberChanging(QString);
    void onPopEnded(bb::cascades::Page*);

    void onOfferSearchFinished();
    void onOperatorSearchFinished();
private:
    bool operatorSelected, circleSelected;
    QList<QString> operators, circles;
    QNetworkReply *currOperation;

    LoadingDialog *loadingDialog;

    Button *numberGoButton, *operatorAndCircleGoButton;
    DropDown *operatorDropdown, *circleDropdown;
    NavigationPane *navPane;
    Validator *numberFieldValidator;

    QNetworkAccessManager manager;
    QString numberString, operatorString, circleString;
};

#endif /* ApplicationUI_HPP_ */
