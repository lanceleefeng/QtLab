

#include <QtSql>
#include <QSqlTableModel>

#include "db.h"
#include "settingmodel.h"

QString SettingModel::tableName = "settings";
QMap<QString, int> SettingModel::tableFields;

SettingModel::SettingModel()
    //: tableName("settings222")
{


}

SettingModel::~SettingModel()
{

}

bool SettingModel::add(QVariantMap data)
{

    qDebug() << "SettingModel::add";

    QMap<QString, int> index;
    index["id"] = 0;
    index["uid"] = 1;
    index["auto_start"] = 2;
    index["countdown"] = 3;
    index["created_at"] = 4;
    index["updated_at"] = 5;



    // 这是表字段顺序，能通过sql查询搞定吗？


    QSqlTableModel *model = new QSqlTableModel(Q_NULLPTR, DB::instance().conn);
    model->setTable("t_settings");

    int row = 0;
    bool res = model->insertRows(row, 1);

    /*QMap<QString, int>::const_iterator i = index.constBegin();
    while(i != index.constEnd()){
        int dataIndex = i.value();
        QString dataKey = i.key();

        ++i;
    }
    */

    QMap<QString, QVariant>::const_iterator i = data.constBegin();
    while(i != data.constEnd()){

        QString dataKey = i.key();
        QString dataVal = i.value().toString();
        int dataIndex = index[dataKey];
        QString dataIndex2 = dataKey;

        qDebug() << "index: " << dataIndex << "; " << dataKey << ": " << dataVal ;
        model->setData(model->index(row, dataIndex), dataVal);
        //model->setData(model->index(row, dataIndex2), dataVal);

        ++i;

    }

    model->submitAll();


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
