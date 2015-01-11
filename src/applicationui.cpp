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

#include "applicationui.hpp"

#include <bb/cascades/Application>

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/data/JsonDataAccess>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/system/SystemToast>
#include <bb/cascades/TextField>

#include <src/OffersPage.h>
#include <src/CustomTextStyle.h>

ApplicationUI::ApplicationUI() : QObject()
{
    currOperation = NULL;
    loadingDialog = new LoadingDialog;
    loadingDialog->setParent(this);
    connect(loadingDialog, SIGNAL(cancel()), this, SLOT(onCancelCurrentOperation()));

    operatorSelected = circleSelected = false;

    TextStyle style = CustomTextStyle::getStyle();

    Label *operatorAndCircleLabel = Label::create().
            horizontal(HorizontalAlignment::Center).
            text("Select your Operator and Circle").
            textStyle(style);
    operatorAndCircleLabel->textStyle()->setColor(Color::White);

    operators<<"Aircel"<<"Airtel"<<"BSNL"<<"Idea"<<"Reliance CDMA"<<"Reliance GSM"
            <<"Tata DOCOMO"<<"Uninor"<<"Vodafone";
    operatorDropdown = DropDown::create().
            title("Operator").
            connect(SIGNAL(selectedValueChanged(QVariant)), this, SLOT(onOperatorChanged(QVariant)));
    for(int i=0; i<operators.size(); ++i)
        operatorDropdown->add(Option::create().text(operators.at(i)).value(operators.at(i)));


    circles<<"Andhra Pradesh"<<"Bihar"<<"Chennai"<<"Gujarat"<<"Haryana"<<"J&K"<<"Karnataka"
            <<"Kerala"<<"Kolkata"<<"Madhya Pradesh"<<"Maharashtra"<<"North East"<<"Orissa"
            <<"Punjab"<<"Rajasthan"<<"Tamil Nadu"<<"Uttar Pradesh (E)"
            <<"Uttar Pradesh (W)"<<"West Bengal";
    circleDropdown = DropDown::create().
            title("Circle").
            connect(SIGNAL(selectedValueChanged(QVariant)), this, SLOT(onCircleChanged(QVariant)));
    for(int i=0; i<circles.size(); ++i)
        circleDropdown->add(Option::create().text(circles.at(i)).value(circles.at(i)));

    operatorAndCircleGoButton = Button::create().
            enabled(false).
            horizontal(HorizontalAlignment::Center).
            text("Go").
            onClicked(this, SLOT(onOperatorAndCircleGoClicked()));

    Container *operatorAndCircleC = Container::create().
            horizontal(HorizontalAlignment::Center).
            vertical(VerticalAlignment::Center).
            add(operatorAndCircleLabel).
            add(operatorDropdown).
            add(circleDropdown).
            add(operatorAndCircleGoButton);

    Container *operatorAndCircleRootC = Container::create().
            layoutProperties(StackLayoutProperties::create().spaceQuota(1)).
            layout(DockLayout::create()).
            add(operatorAndCircleC);

    Label *orLabel = Label::create().
            horizontal(HorizontalAlignment::Center).
            multiline(true).
            text("OR").
            textStyle(style);
    orLabel->textStyle()->setFontSize(FontSize::PointValue);
    orLabel->textStyle()->setFontSizeValue(13);
    orLabel->textStyle()->setFontWeight(FontWeight::W500);

    numberFieldValidator = Validator::create().
            state(ValidationState::Unknown);

    TextField *numberField = TextField::create().
            inputMode(TextFieldInputMode::PhoneNumber).
            connect(SIGNAL(textChanging(QString)), this, SLOT(onNumberChanging(QString))).
            maximumLength(10).
            validator(numberFieldValidator).
            hintText("Enter your 10 digit number");

    numberGoButton = Button::create().
            preferredWidth(200).
            enabled(false).
            onClicked(this, SLOT(onNumberGoClicked())).
            text("Go");

    Container *numberAndGoC = Container::create().
            horizontal(HorizontalAlignment::Center).
            vertical(VerticalAlignment::Center).
            layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight)).
            add(numberField).
            add(numberGoButton);

    Container *phoneNumberRootC = Container::create().
            left(10).
            right(10).
            layoutProperties(StackLayoutProperties::create().spaceQuota(1)).
            layout(DockLayout::create()).
            add(numberAndGoC);

    Container *rootC = Container::create().
            background(Color::fromARGB(0xfffc4546)).
            add(phoneNumberRootC).
            add(orLabel).
            add(operatorAndCircleRootC);

    Page *rootPage = Page::create().
            content(rootC);

    navPane = NavigationPane::create().
            add(rootPage);
    connect(navPane, SIGNAL(popTransitionEnded(bb::cascades::Page*)),
            this, SLOT(onPopEnded(bb::cascades::Page*)));

    Application::instance()->setScene(navPane);
}

void ApplicationUI::onCancelCurrentOperation()
{
    currOperation->abort();
    currOperation = NULL;
}

void ApplicationUI::onCircleChanged(QVariant value)
{
    circleString = value.toString();
    circleSelected = true;
    if(operatorSelected)
        operatorAndCircleGoButton->setEnabled(true);
}

void ApplicationUI::onNumberChanging(QString numString)
{
    TextField *field = static_cast<TextField*>(sender());

    QRegExp exp("[0-9]*");
    if(exp.exactMatch(numString))
    {
        if(numString.length()<10)
        {
            numberGoButton->setEnabled(false);
            field->validator()->setState(ValidationState::Invalid);
            field->validator()->setErrorMessage("Number must be at least 10 digits long");
        }
        else
        {
            numberGoButton->setEnabled(true);
            field->validator()->setState(ValidationState::Valid);
            numberString = numString;
        }
    }
    else
    {
        numberGoButton->setEnabled(false);
        field->validator()->setState(ValidationState::Invalid);
        field->validator()->setErrorMessage("Number can contain only 0-9");
    }
}

void ApplicationUI::onNumberGoClicked()
{
    numberGoButton->setEnabled(false);

    QNetworkRequest request;
    request.setUrl(QUrl("https://sphirelabs-indian-caller-info-with-name-v1.p.mashape.com/caller/v1/"
            "get/callerinfo.php?number=" + numberString));
    request.setRawHeader("X-Mashape-Key", "2XKHPwdLqKmshAQmAcOqGljzvhPup1PAkAvjsnUzIF05mHlhcK");

    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onOperatorSearchFinished()));
    currOperation = reply;

    loadingDialog->setText("Trying to find your operator and circle.. Please wait..");
    loadingDialog->open();
}

void ApplicationUI::onOperatorAndCircleGoClicked()
{
    operatorAndCircleGoButton->setEnabled(false);

    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://community-telecom-data.p.mashape.com/telecom_data/listByCircle/?"
            "circle=%1&operator=%2&page=1&per_page=1000").arg(circleString).arg(operatorString)));
    request.setRawHeader("X-Mashape-Key", "2XKHPwdLqKmshAQmAcOqGljzvhPup1PAkAvjsnUzIF05mHlhcK");

    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onOfferSearchFinished()));
    currOperation = reply;

    loadingDialog->setText(QString("Searching offers for %1 in %2.. Please wait..").
            arg(operatorString).arg(circleString));
    loadingDialog->open();
}

void ApplicationUI::onOperatorChanged(QVariant value)
{
    operatorString = value.toString();
    operatorSelected = true;
    if(circleSelected)
        operatorAndCircleGoButton->setEnabled(true);
}

void ApplicationUI::onOfferSearchFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());

    if(reply->error()==QNetworkReply::NoError)
    {
        bb::data::JsonDataAccess jda;
        QByteArray data = reply->readAll();
        QVariantMap dataMap = jda.loadFromBuffer(data).toMap();
        if(dataMap["count"].toInt()>0)
        {
            QVariantList dataList = dataMap["data"].toList();
            QList<QVariantMap> mapList;
            const int count = dataList.size();
            for(int i=0; i<count; ++i)
                mapList.insert(i, dataList.at(i).toMap());
            navPane->push(new OffersPage(mapList));
            QFile logFile;
            logFile.setFileName(QDir::home().absoluteFilePath("log.txt"));
            qDebug()<<logFile.open(QFile::WriteOnly);
            logFile.write(data);
            logFile.close();
        }
        else
        {
            bb::system::SystemToast *toast = new bb::system::SystemToast();
            toast->setBody("Sorry.. We were unable to find any offers..");
            connect(toast, SIGNAL(finished(bb::system::SystemUiResult::Type)), toast, SLOT(deleteLater()));
            toast->show();
        }
    }
    else
    {
        qDebug()<<"Error"<<reply->errorString();
        bb::system::SystemToast *toast = new bb::system::SystemToast();
        toast->setBody("Sorry.. A connection error occurred..");
        connect(toast, SIGNAL(finished(bb::system::SystemUiResult::Type)), toast, SLOT(deleteLater()));
        toast->show();
    }

    operatorAndCircleGoButton->setEnabled(true);

    loadingDialog->close();
}

void ApplicationUI::onOperatorSearchFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());

    if(reply->error()==QNetworkReply::NoError)
    {
        bb::data::JsonDataAccess jda;
        QVariantMap map = jda.loadFromBuffer(reply->readAll()).toMap();

        int op = operators.indexOf(map["Operator"].toString());
        if(op==-1)
            operatorDropdown->setSelectedIndex(DropDown::SelectedIndexNone);
        else
            operatorDropdown->setSelectedIndex(op);

        int circ = circles.indexOf(map["Telecom circle"].toString());
        if(circ==-1)
            circleDropdown->setSelectedIndex(DropDown::SelectedIndexNone);
        else
            circleDropdown->setSelectedIndex(circ);

        onOperatorAndCircleGoClicked();
    }
    else
    {
        qDebug()<<"Error"<<reply->errorString();

        bb::system::SystemToast *toast = new bb::system::SystemToast();
        toast->setBody("Sorry.. A connection error occurred..");
        connect(toast, SIGNAL(finished(bb::system::SystemUiResult::Type)), toast, SLOT(deleteLater()));
        toast->show();

        loadingDialog->close();
    }

    if(numberFieldValidator->isValid())
        numberGoButton->setEnabled(true);
}

void ApplicationUI::onPopEnded(bb::cascades::Page *poppedPage)
{
    poppedPage->deleteLater();
}
