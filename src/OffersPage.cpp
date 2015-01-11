/*
 * OffersPage.cpp
 *
 *  Created on: 11-Jan-2015
 *      Author: SuhairZain
 */

#include <src/OffersPage.h>

#include <src/OffersListItemProvider.h>

#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>

OffersPage::OffersPage(QList<QVariantMap> rechargeData)
{
    dataModel = new GroupDataModel;
    dataModel->setGrouping(ItemGrouping::ByFullValue);
    dataModel->setSortingKeys(QStringList()<<"recharge_short_description"<<"recharge_value"
            <<"recharge_validity");
    dataModel->insertList(rechargeData);

    DropDown *sortingSelector = DropDown::create().
            title("Sort by").
            add("Recharge type", "recharge_short_description", true).
            add("Recharge value", "recharge_value").
            add("Recharge validity", "recharge_validity").
            connect(SIGNAL(selectedValueChanged(QVariant)), this, SLOT(onSortingValueChanged(QVariant)));

    ListView *listView = ListView::create().
            listItemProvider(new OffersListItemProvider).
            leadingVisual(sortingSelector).
            dataModel(dataModel);

    Container *rootC = Container::create().
            add(listView);
    setContent(rootC);
}

void OffersPage::onSortingValueChanged(QVariant value)
{
    QStringList sortingKeys = dataModel->sortingKeys();
    sortingKeys.swap(0, sortingKeys.indexOf(value.toString()));
    dataModel->setSortingKeys(sortingKeys);
}
