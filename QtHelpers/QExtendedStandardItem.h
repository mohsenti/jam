//
// Created by mohsen on 6/14/16.
//

#include <QtGui/QStandardItem>

#ifndef JAM_QEXTENDEDSTANDARDITEM_H
#define JAM_QEXTENDEDSTANDARDITEM_H

class QExtendedStandardItem : public QStandardItem {
public:
    QExtendedStandardItem(QString text):QStandardItem(text) {}

    void *data() { return _data; }

    void setData(void *_data) { this->_data = _data; }

private:
    void *_data;
};

#endif //JAM_QEXTENDEDSTANDARDITEM_H
