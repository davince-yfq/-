#ifndef SQLITEOPERATOR_H
#define SQLITEOPERATOR_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class sqliteOperator
{
public:
    sqliteOperator();
    //打开数据库
    bool opendb(void);
    //关闭数据库
    void closedb(void);
    //执行函数
    void normalExec(QString sql);
    //查询函数
    void checkExec(QSqlQuery  &sqlQuery,QString sql);
private:
    QSqlDatabase database;//用于建立和数据库的链接
};

#endif // SQLITEOPERATOR_H
