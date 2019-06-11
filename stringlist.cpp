#include "stringlist.h"
#include <QDebug>

StringList::StringList()
{
    this->list = new QStringList();
    this->setStringList(*this->list);
}

void StringList::append (const QString& string){
    this->list->append(string);
    this->setStringList(*this->list);
}

void StringList::push (QList<QString> strings) {
    for(int i=0; i< strings.count(); i++) {
        this->append(strings[i]);
    }
}
