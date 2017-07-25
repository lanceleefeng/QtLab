
#include "db.h"
#include "basemodel.h"

QString BaseModel::tableName;
QString BaseModel::tableFullName;
QString BaseModel::tablePrefix = "ts_";
int BaseModel::uid;

//QMap<QString, int> BaseModel::tableFields;
QMap<QString, QMap<QString, int>> BaseModel::tableFields;


BaseModel::BaseModel()
{
    //setTableInfo();
}
BaseModel::BaseModel(QString tableName){
    //qDebug() << "table name: " << tableName;

    //this->table = tableName;
    table = tablePrefix + tableName;
    //qDebug() << "table name: " << table;

    setTableFields(table);

}

QVariantMap BaseModel::getOne(QString where)
{

    //qDebug() << __FUNCTION__;
    //qDebug() << "table: " << table;

    QVariantMap row;

    QSqlTableModel model(Q_NULLPTR, DB::instance().conn);
    model.setTable(table);
    if(!where.isEmpty()){
        model.setFilter(where);
    }
    bool res = model.select();
    if(!res){
        return row;
    }
    int count = model.rowCount();
    //qDebug() << "条数：" << count;

    for(int i=0; i<count; i++){
        QSqlRecord record = model.record(i);
        int id = record.value("id").toInt();
        int autoStart = record.value("auto_start").toInt();
        int countDown = record.value("countdown").toInt();
        //qDebug() << "record: " << i;
        //qDebug() << "id: " << id << ", autoStart: " << autoStart << ", countdown: " << countDown;

        int fieldsNum = record.count();
        //qDebug() << "fields num: " << fieldsNum;

        for(int j=0; j<fieldsNum; j++){
            //qDebug() << record.fieldName(j) << ": \t" << record.value(j).toString();
            row[record.fieldName(j)] = record.value(j);
        }
        break;

    }

    return row;
}
QVariantMap BaseModel::getOne(QVariantMap where)
{
    //qDebug() << __FUNCTION__;
    //qDebug() << "table: " << table;

    if(where.isEmpty()){
        return getOne("");
    }

    QString wh = getWhere(where);
    return getOne(wh);

}


bool BaseModel::add(QVariantMap data)
{

    //qDebug() << __FUNCTION__ << "用模型插入数据";
    //
    //qDebug() << "表：" << table;

    if(data.isEmpty()){
        return false;
    }

    //qDebug() << "没有数据？";

    QSqlTableModel model(Q_NULLPTR, DB::instance().conn);
    model.setTable(table);
    //QVariantMap fields = getFields(table);
    //QMap<QString, int> fields = getFields(table);
    QMap<QString, int> fields = getFields(table, true);

    if(fields.isEmpty()){
        qDebug() << "shit, fields not obtained";
    }

    QMap<QString, int>::const_iterator k = fields.constBegin();
    while(k != fields.constEnd()){
        //qDebug() << k.key() << ": " << k.value();
        ++k;
    }


    // 先插入数据再说，后面再考虑自动添加时间戳的问题

    int row = 0;
    model.insertRows(row, 1);

    QMap<QString, QVariant>::const_iterator i = data.constBegin();
    while(i != data.constEnd()){
        QString key = i.key();
        int index = fields[key];
        QString val = i.value().toString();

        //qDebug() << "index: " << index << ", key: " << key << ", val: " << val;
        model.setData(model.index(row, index), val);
        ++i;
    }

    bool res = model.submitAll();
    //qDebug() << model.lastError();
    return res;

}


bool BaseModel::update(QVariantMap data, QVariantMap where)
{
    QString w = getWhere(where);
    return update(data, w);
}

bool BaseModel::update(QVariantMap data, QString where)
{
    QSqlTableModel model(Q_NULLPTR, DB::instance().conn);
    model.setTable(table);
    model.setFilter(where);

    model.select();
    int num = model.rowCount();

    qDebug() << "num: " << num;

    for(int i=0; i<num; i++){

    }

}

bool BaseModel::del(QVariantMap where)
{
    QString w = getWhere(where);
    return del(w);
}

bool BaseModel::del(QString where)
{
    if(where.isEmpty()){
        return false;
    }
    QSqlTableModel model(Q_NULLPTR, DB::instance().conn);
    model.setTable(table);
    model.setFilter(where);
    model.select();

    int num = model.rowCount();

    if(num == 0){
        return true;
    }


    //bool res = model.removeRows(0, num);
    //model.submitAll();
    //return res;

    while(num > 0){
        model.setFilter(where);
        model.select();
        model.removeRows(0, 1);
        model.submitAll();
        num = model.rowCount();
    }

    return true;
}

QString BaseModel::getWhere(QVariantMap where)
{
    if(where.isEmpty()){
        return "";
    }

    //QMap<QString, int> fields = getFields(table);
    QMap<QString, int> fields = getFields(table, true);

    QString w;
    int num = where.size();
    bool first = true;
    QMap<QString, QVariant>::const_iterator con = where.constBegin();

    qDebug() << "条件：";
    while(con != where.constEnd()){
        QString key = con.key();
        QString val = con.value().toString();

        qDebug() << key << ": \t" << val;

        if(!fields.contains(key)){
            ++con;
            continue;
        }

        if(!first){
            w += " AND " + key + "=" + val;
        }else{
            w += key + "=" + val;
        }

        first = false;
        ++con;
    }

    qDebug() << "where: " << w;

    return w;
}

//void BaseModel::setTableInfo()
//QMap<QString, int> BaseModel::setTableInfo()
//QMap<QString, int> BaseModel::getTableFields()

QMap<QString, QMap<QString, int>> getTableFields()
{
    /*QString table = getTableName();
    qDebug() << "table got in BaseModel: " << table;

    QString sql = "PRAGMA table_info("+table+")";
    QStringList keys;
    keys << "cid" << "name";

    static DB& db = DB::instance();
    QList<QVariantMap> tableInfoList = db.query(sql, keys); // QList<QMap<QString, QVariant>>

    QMap<QString, int> tableInfo;
    QVariantMap field;
    foreach(field, tableInfoList){
            tableInfo[field["name"].toString()] = field["cid"].toInt();
        }
    return tableInfo;*/
}


/**
 * 获取表字段列表，使用默认前缀
 *
 * @param QString tableName
 * @return
 */
QMap<QString, int> BaseModel::getFields(QString tableName)
{
    //QString table = getTableName();
    //QString table = BaseModel::getTableName();
    QString table = BaseModel::tablePrefix + tableName;
    //qDebug() << "table got in BaseModel: " << table;

    return BaseModel::getTableFields(table);
}


QMap<QString, int> BaseModel::getFields(QString tableName, QString prefix)
{
    QString table = prefix + tableName;
    return BaseModel::getTableFields(table);
};

//QMap<QString, int> BaseModel::getFields(QString tableName, bool isFullName = true)
QMap<QString, int> BaseModel::getFields(QString tableName, bool isFullName)
{
    if(isFullName){
        return BaseModel::getTableFields(tableName);
    }else{
        return BaseModel::getFields(tableName);
    }
};
/**
 * 获取表的字段列表
 * @param QString table 不再处理前缀问题，直接使用表名
 * @return
 */
QMap<QString, int> BaseModel::getTableFields(QString table)
{
    if(BaseModel::tableFields[table].isEmpty()){
        BaseModel::setTableFields(table);
    }
    return BaseModel::tableFields[table];
}

void BaseModel::setTableFields(QString table)
{
    if(!tableFields[table].isEmpty()){
        return;
    }

    //qDebug() << "table got in BaseModel: " << table;

    QString sql = "PRAGMA table_info("+table+")";
    QStringList keys;
    keys << "cid" << "name";

    static DB& db = DB::instance();
    QList<QVariantMap> tableInfoList = db.query(sql, keys); // QList<QMap<QString, QVariant>>

    QMap<QString, int> tableInfo;
    QVariantMap field;
            foreach(field, tableInfoList){
            tableInfo[field["name"].toString()] = field["cid"].toInt();
        }

    //return tableInfo;
    //QMap<QString, QMap<QString, int>> fields;
    //fields[table] = tableInfo;
    //return fields;
    tableFields[table] = tableInfo;
}


