

#include <QtSql>
#include <QSqlTableModel>

#include "db.h"
#include "settingmodel.h"

QString SettingModel::tableName = "settings";
//bool SettingModel::isFullName = true;
QMap<QString, int> SettingModel::tableFields;

SettingModel::SettingModel()
    //: tableName("settings222")
{
    getFields(tableName);
    table = getTable(tableName, isFullName);

}

SettingModel::~SettingModel()
{
}

bool SettingModel::add(QVariantMap data)
{

    return true;
}


bool SettingModel::saveCountMode(int countDown)
{
    //qDebug() << "in " << __FUNCTION__ << "保存到数据库";

    QVariantMap data;
    data["countdown"] = countDown > 0 ? 1 : 0;

    return saveSetting(data);
}

bool SettingModel::saveSetting(QVariantMap data)
{
    qDebug() << "in " << __FUNCTION__;

    static DB& db = DB::instance();

    //db.exec(table, data, fields);

    //if()
    //add(data);
    //add(data, table, fields);

    //update(data, condition);
    //update(data, condition, table, fields)

    //

    return true;


}

bool SettingModel::save(QVariantMap data)
{
    QVariantMap where;
    where["uid"] = uid;

    QVariantMap row = getOne(where);

    if(row.isEmpty()){
        qDebug() << "没有数据";
        add(data);
    }else{
        qDebug() << "已有数据";
        //where["id"] = row["id"];
        update(data, where);
    }

    qDebug() << "fk" << row["id"];
}

