/*
 * OffersPage.h
 *
 *  Created on: 11-Jan-2015
 *      Author: SuhairZain
 */

#ifndef OFFERSPAGE_H_
#define OFFERSPAGE_H_

#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Page>

using namespace bb::cascades;

class OffersPage: public Page
{
Q_OBJECT
private slots:
    void onSortingValueChanged(QVariant);
public:
    OffersPage(QList<QVariantMap>);
private:
    GroupDataModel *dataModel;
};

#endif /* OFFERSPAGE_H_ */
