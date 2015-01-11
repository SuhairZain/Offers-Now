/*
 * OffersListItemProvider.h
 *
 *  Created on: 11-Jan-2015
 *      Author: SuhairZain
 */

#ifndef OFFERSLISTITEMPROVIDER_H_
#define OFFERSLISTITEMPROVIDER_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Label>
#include <bb/cascades/ListItemProvider>

using namespace bb::cascades;

class OffersListItem: public CustomControl/*, public ListItemListener*/
{
Q_OBJECT
public:
    OffersListItem();

    void updateItem(int, QString, QString, int);
    void select(bool select);
    void reset(bool selected, bool activated);
    void activate(bool activate);

private:
    Label *valueLabel, *validityLabel, *shortLabel, *longLabel;
};

class OffersListItemProvider: public ListItemProvider
{
public:
    VisualNode *createItem(ListView*, const QString &);
    void updateItem(ListView*, VisualNode*, const QString&,
            const QVariantList&, const QVariant&);
};

#endif /* OFFERSLISTITEMPROVIDER_H_ */
