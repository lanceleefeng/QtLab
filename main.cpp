
#include <QApplication>
#include <QMessageBox>


#include "init.h";
//#include "mainwindow.h"
#include "db.h"
#include "tomato.h"
#include "settingmodel.h"
//#include "testmodel.h"

int uid = 1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //qDebug() << "使用静态属性的name: " << Tomato::name;
    Init init;
    if(!init.succeed()){
        QMessageBox::critical(0, Tomato::name, QString("初始化失败"));
        //QMessageBox::critical(0, Tomato.name, QString("初始化失败"));
        //QMessageBox::critical(0, init.name, QString("初始化失败"));
        //QMessageBox::critical(0, name , QString("初始化失败"));
        return -1;
        //this->quit();

        //return;
    }

    Tomato w;
    if(!w.initiated){
        a.quit();
        return 10;
    }
    w.show();


    if(w.autoStart){
        w.startCountDown();
    }

    //qDebug() << "\n" << __FUNCTION__;


    SettingModel settingModel;

    //RingModel ringModel;

    //settingModel.getSettings();
    //settingModel.getSetting();

    //settingModel.save();
    //settingModel.saveSetting();
    //settingModel.add();
    //settingModel.update();
    //settingModel.del();

    //settingModel.saveCountMode();
    //settingModel.saveAutoStart();
    //settingModel.saveRing();
    //settingModel.saveBlink();

    // 数据验证、过滤，暂不考虑

    // 默认值
    // 默认值使用数据库中的并不好，可能需要修改，改数据库结构不方便
    // 默认值单独作为属性配置，
    // 新增时数据与默认值合并，就像JQ那样，然后保存合并之后的
    // 对于更新就只更新指定的数据。

    // 另外，用sqlite保存数据并不好，容易保修改，但用xml、配置文件一样可能被修改
    // 毕竟是客户端，对于正常使用来说OK即可

    BaseModel::uid = uid;

    QVariantMap data;
    data["countdown"] = 1;
    //settingModel.saveCountDown(data);
    //settingModel.saveCountMode(1);

    //settingModel.add(data);
    //settingModel.update(data, where);

    //甚至都不需要模型类，用基类进行操作！
    //baseModel.setTableName(tableName);
    //baseModel.setTableName(tableName);
    //baseModel.setFullTableName
    //baseModel.getOne(id);
    //baseModel.getOne(where);
    //baseModel.add(data);
    //baseModel.update(data, where);


    //BaseModel baseModel("tableName", false);

    //模型类应该是与具体业务逻辑有关的，共同的部分应该在基类中
    //甚至做成全静态属性？

    //做成静态不合适：
    //每次调用会相互影响，需要实例化

    //另外，每个人的设置在settings表只有一条记录，属于业务逻辑部分，基类不需要关心
    //保存时判断是否已有记录，应该在设置的模型类中判断，而不是基类

    //正是因为把每人只有一个setting记录的业务逻辑带入到模型设计中，才导致没有设计出通用的模型！

    BaseModel model("settings");
    BaseModel ring("rings");


    QMap<QString, int> fields =  BaseModel::getFields("settings");
    QMap<QString, int>::const_iterator i = fields.constBegin();
    while(i != fields.constEnd()){
        //qDebug() << i.key() << ": " << i.value();
        ++i;
    }


    QMap<QString, int> fields2 =  BaseModel::getFields("rings");
    QMap<QString, int>::const_iterator i2 = fields2.constBegin();
    while(i2 != fields2.constEnd()){
        //qDebug() << i2.key() << ": " << i2.value();
        ++i2;
    }

    // 查询

    //QVariantMap setting = model.getOne(QVariantMap w);
    //QVariantMap setting = model.getOne(QMap({{"uid", uid}}));
    QVariantMap setting = model.getOne(QMap<QString, QVariant>({{"uid", uid}}));

    // 写一个完善的模型类还是比较复杂的，甚至一个类搞不定。
    // 只写基本的增删改查，最复杂的还是查询

    //qDebug() << "查询到的数据：";
    QMap<QString, QVariant>::const_iterator s = setting.constBegin();
    while(s != setting.constEnd()){
        //qDebug() << s.key() << ": " << s.value().toString();
        ++s;
    }

    // 插入：2017-7-19 2:25:12

    QVariantMap newData;
    newData["uid"] = uid;
    newData["auto_start"] = 1;
    newData["countdown"] = 1;

    model.add(newData);


    // 修改

    QVariantMap updatedData;
    updatedData["auto_start"] = 0;
    //QString where = "id=" + setting["id"].toString();
    //model.update(updatedData, where);
    //QVariantMap where{{"id", 1}};

    //model.update(updatedData, QMap<QString, QVariant>({{"uid", uid}, {"id", setting["id"]}}));
    model.update(updatedData, QMap<QString, QVariant>({{"uid", uid}}));


    // 删除
    //QString delWhere = "uid=" + uid + " AND id!=" + setting["id"].toString();
    //QString delWhere = QString("uid=%1 AND id!=%2").arg(uid).arg(setting["id"].toInt());
    QString delWhere = QString("uid=%1 AND id>=%2").arg(uid).arg(70);
    //model.del(QMap<QString, QVariant>({{"uid",uid}}));
    model.del(delWhere);

    return a.exec();
}
