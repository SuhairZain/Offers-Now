/*
 * CustomTextStyle.cpp
 *
 *  Created on: 28-Dec-2014
 *      Author: SuhairZain
 */

#include <src/CustomTextStyle.h>

#include <bb/cascades/Container>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TextStyleDefinition>

TextStyle CustomTextStyle::getStyle()
{
    QmlDocument *qml = QmlDocument::create("asset:///TextStyle.qml");

    Container *root = qml->createRootObject<Container>();
    TextStyleDefinition *def = static_cast<TextStyleDefinition*>(
            root->children().at(0));

    TextStyle style = def->style();
    style.setFontFamily("MyCustomFont");

    root->deleteLater();
    qml->deleteLater();

    return style;
}
