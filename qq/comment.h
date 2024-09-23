#ifndef COMMENT_H
#define COMMENT_H
#include <stdio.h>
#include <string.h>
#include <QString>
    struct Qmsg
    {
        char buf[1022];
        short type;
    };

    struct rigister_node
    {
        rigister_node(const char *name,const char *pwd) {
            strcpy(this->name,name);
            strcpy(this->pwd,pwd);
            //this->sex=sex;
        }
        char name[128];
        char pwd[48];
        //bool sex;
    };

    struct WtData
    {
        QString city;
        QString shidu;
        QString pm25;
        QString quality;
        QString wendu;
        QString date;
        QString high;
        QString low;
        QString ymd;
        QString week;
        QString fx;
        QString fl;
        QString type;
        QString notice;

    };
    struct Forecast
    {
        QString date;
        QString high;
        QString low;
        QString ymd;
        QString week;
        QString fx;
        QString fl;
        QString type;
    };

#endif // COMMENT_H
