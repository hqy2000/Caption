#ifndef STRINGLIST_H
#define STRINGLIST_H
#include<QString>
#include<QStringListModel>
#include<QStringList>

class StringList : public QStringListModel
{
public:
    StringList();
    QStringList* list;
    void append (const QString& string);
    void push(const QList<QString> strings);
};

#endif // STRINGLIST_H
