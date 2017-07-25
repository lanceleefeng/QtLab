//
// Created by Administrator on 2017-7-4.
//

#ifndef BASEMODEL_H
#define BASEMODEL_H

#include "db.h"


class BaseModel
{
public:

    //QString tableName = "settings";
    static QString tableName;
    static QString tableFullName;
    static QString tablePrefix;
    QString table;

    static int uid;

    //static QMap<QString, int> tableFields;
    static QMap<QString, QMap<QString, int>> tableFields;

    static QString getTableName();

    BaseModel();
    BaseModel(QString tableName);
    BaseModel(QString tableName, QString prefix);
    BaseModel(QString tableName, bool isFullName);

    //static QMap<QString, int> getTableFields();
    //static QMap<QString, QMap<QString, int>> getTableFields();
    //static QMap<QString, QMap<QString, int>> getTableFields();
    //static QMap<QString, QMap<QString, int>> getTableFields(QString tableName);
    //static QMap<QString, QMap<QString, int>> getTableFields(QString tableName, QString prefix);
    //static QMap<QString, QMap<QString, int>> getTableFields(QString tableName, bool isFullName);

    // 不必再区分表名用哪种情况了，最常用的是不带前缀的表名；
    // 不同的方法已经进行区分了，在各个方法中直接拼接完整的表名即可

    //static QMap<QString, int> getFields();
    static QMap<QString, int> getFields(QString tableName);
    static QMap<QString, int> getFields(QString tableName, QString prefix);
    static QMap<QString, int> getFields(QString tableName, bool isFullName);

    static QMap<QString, int> getTableFields(QString table);


    static void setTableFields(QString table);



    QVariantMap getOne(int id);
    QVariantMap getOne(QVariantMap where);
    QVariantMap getOne(QString where);
    QList<QVariantMap> getAll(QVariantMap where);
    QList<QVariantMap> getPage(QVariantMap where, int page, int pageSize);

    bool add(QVariantMap data);
    bool update(QVariantMap data, QVariantMap where);
    bool update(QVariantMap data, QString where);
    //bool delete(QVariant where);
    bool del(QVariantMap where); // delete是关键字！
    bool del(QString where);
    QString getWhere(QVariantMap where);



};




#endif //BASEMODEL_H
