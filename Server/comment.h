#ifndef COMMENT_H
#define COMMENT_H
#include <stdio.h>
#include <string.h>

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


#endif // COMMENT_H
