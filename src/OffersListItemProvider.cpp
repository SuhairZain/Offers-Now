/*
 * OffersListItemProvider.cpp
 *
 *  Created on: 11-Jan-2015
 *      Author: SuhairZain
 */

#include <src/OffersListItemProvider.h>

#include <bb/cascades/Container>
#include <bb/cascades/Divider>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Header>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/TextStyle>

#include <src/CustomTextStyle.h>

OffersListItem::OffersListItem()
{
    TextStyle style = CustomTextStyle::getStyle();
    style.setColor(Color::DarkGray);
    style.setFontSize(FontSize::PointValue);

    valueLabel = Label::create().
            preferredWidth(210).
            textStyle(style);
    valueLabel->textStyle()->setTextAlign(TextAlign::Center);
    valueLabel->textStyle()->setFontSizeValue(13);

    Container *valueC = Container::create().
            horizontal(HorizontalAlignment::Center).
            add(valueLabel);

    style.setColor(Color::DarkGray);
    validityLabel = Label::create().
            textStyle(style);
    validityLabel->textStyle()->setFontSizeValue(8);

    Container *validityC = Container::create().
            horizontal(HorizontalAlignment::Center).
            add(validityLabel);

    Container *valueAndValidityC = Container::create().
            add(valueC).
            add(validityC);

    style.setColor(Color::Black);
    shortLabel = Label::create().
            textStyle(style);
    shortLabel->textStyle()->setFontSizeValue(8);

    Container *shortC = Container::create().
            add(shortLabel);

    longLabel = Label::create().
            textStyle(style).
            multiline(true);
    longLabel->textStyle()->setFontSizeValue(5);

    Container *longC = Container::create().
            add(longLabel);

    Container *descC = Container::create().
            layoutProperties(StackLayoutProperties::create().spaceQuota(1)).
            add(shortC).
            add(longC);

    Container *rootC = Container::create().
            layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight)).
            add(valueAndValidityC).
            add(descC);

    Container *finalRootC = Container::create().
            add(rootC).
            add(new Divider);

    setRoot(finalRootC);
}

void OffersListItem::activate(bool activate)
{
    select(activate);
}

void OffersListItem::reset(bool selected, bool activated)
{
    Q_UNUSED(activated);

    select(selected);
}

void OffersListItem::select(bool /*select*/)
{

}

void OffersListItem::updateItem(int validity, QString shortD, QString longD, int value)
{
    valueLabel->setText("Rs " + QString::number(value));
    validityLabel->setText(QString::number(validity));
    shortLabel->setText(shortD);
    longLabel->setText(longD);
}

VisualNode * OffersListItemProvider::createItem(ListView*/* list*/, const QString &type)
{
    //We only have one item type so we do not need to check the type variable.
    if(type==GroupDataModel::Header)
        return new Header;
    else
        return new OffersListItem;
}

void OffersListItemProvider::updateItem(ListView*, bb::cascades::VisualNode *listItem,
        const QString &type, const QVariantList &, const QVariant &data)
{
    if(type==GroupDataModel::Header)
    {
        static_cast<Header*>(listItem)->setTitle(data.toString());
    }
    else
    {
        QVariantMap map = data.toMap();

        QString validityStr = map["recharge_validity"].toString();
        validityStr = validityStr.left(validityStr.indexOf(' '));
        static_cast<OffersListItem*>(listItem)->updateItem(
                validityStr.toInt(),
                map["recharge_short_description"].toString(),
                map["recharge_description"].toString(),
                map["recharge_value"].toInt());
    }
}
