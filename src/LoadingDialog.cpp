/*
 * LoadingDialog.cpp
 *
 *  Created on: 10-Jan-2015
 *      Author: SuhairZain
 */

#include <src/LoadingDialog.h>

#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>

#include <src/CustomTextStyle.h>

LoadingDialog::LoadingDialog()
{
    ImageView *bg = ImageView::create().
            horizontal(HorizontalAlignment::Center).
            vertical(VerticalAlignment::Center).
            image("asset:///images/fetchingBG.png");

    ActivityIndicator *indicator = ActivityIndicator::create().
            horizontal(HorizontalAlignment::Center);
    indicator->start();

    TextStyle style = CustomTextStyle::getStyle();

    messageLabel = Label::create().
            multiline(true).
            horizontal(HorizontalAlignment::Center).
            textStyle(style);
    messageLabel->textStyle()->setTextAlign(TextAlign::Center);
    messageLabel->setMaxWidth(560);

    Button *cancelButton = Button::create().
            horizontal(HorizontalAlignment::Center).
            text("Cancel").
            onClicked(this, SLOT(onCancelClicked()));

    Container *contentC = Container::create().
            horizontal(HorizontalAlignment::Center).
            vertical(VerticalAlignment::Center).
            add(indicator).
            add(messageLabel).
            add(cancelButton);

    Container *innerC = Container::create().
            horizontal(HorizontalAlignment::Center).
            vertical(VerticalAlignment::Center).
            layout(DockLayout::create()).
            add(bg).
            add(contentC);

    Container *rootC = Container::create().
            horizontal(HorizontalAlignment::Fill).
            vertical(VerticalAlignment::Fill).
            layout(DockLayout::create()).
            add(innerC);

    setContent(rootC);
}

void LoadingDialog::onCancelClicked()
{
    emit cancel();
    this->close();
}

void LoadingDialog::setText(QString message)
{
    messageLabel->setText(message);
}
