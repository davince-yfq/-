#include "sqliteoperator.h"

sqliteOperator::sqliteOperator()
{
    if(QSqlDatabase::contains("qt_sql_default_connection")==true)
    {
        database=QSqlDatabase::database("qt_sql_default_connection");
    }else
    {
        //建立链接
        database=QSqlDatabase::addDatabase("QSQLITE");
        //设置数据库的名字
        database.setDatabaseName("MyDataBase.db");
    }
}

bool sqliteOperator::opendb()
{
    if(!database.open())
    {
        qDebug()<<"ErrorLfail to connect database"<<database.lastError()<<endl;
    }else
    {

    }
    return true;
}

void sqliteOperator::closedb()
{
    database.close();
}

void sqliteOperator::normalExec(QString sql)
{
    QSqlQuery sqlqurey;
    if(!sqlqurey.exec(sql))
    {
        qDebug()<<sqlqurey.lastError()<<endl;
    }
    else {
        qDebug()<<"ok"<<endl;
    }
}

void sqliteOperator::checkExec(QSqlQuery &sqlQuery, QString sql)
{
    if(!sqlQuery.exec(sql))
    {
        qDebug()<<sqlQuery.lastError()<<endl;
    }
    else {
        qDebug()<<"ok"<<endl;
    }
}
