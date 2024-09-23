#include "weather.h"
#include "ui_weather.h"

Weather::Weather(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);

    //创建定时器
    auto *time=new QTimer(this);
    connect(time,&QTimer::timeout,this,&Weather::updatetime);
    connect(time,&QTimer::timeout,this,&Weather::setpicture);
    time->start(1000);//每秒更新一次

    manner=new QNetworkAccessManager(this);
    connect(manner,SIGNAL(finished(QNetworkReply*)),this,SLOT(ReplyFinshed(QNetworkReply *)));

    manner->get(QNetworkRequest(QUrl("http://t.weather.sojson.com/api/weather/city/101010100")));

    ui->label_feng_data->setStyleSheet("background-image:url(:/天气风级.png);border:0px solid black;");
    ui->lable_shi->setStyleSheet("background-image:url(:/湿度.png);border:0px solid black;");
    ui->lable_pm->setStyleSheet("background-image:url(:/PM2.5.png);border:0px solid black;");
    ui->lable_qurlity->setStyleSheet("background-image:url(:/空气质量.png);border:0px solid black;");

    //设置widget边框弧度
    //ui->widget_3->setStyleSheet("QWidget { border: 2px solid black; border-radius: 150px; }");

    for(int i=0;i<6;i++)
    {
        QString numName="label_type"+QString::number(i+1);
        la[i]=QMainWindow::findChild<QLabel *>(numName);
    }

    //设置事件过滤器
    ui->widget_10->installEventFilter(this);
    ui->widget_11->installEventFilter(this);

    //去掉窗口边框
    setWindowFlag(Qt::FramelessWindowHint);
    //初始化退出最小化菜单
    menuQuit=new QMenu(this);
    QAction *openact=new QAction(QIcon(":/quit.png"),tr("退出"),this);
    QAction *suoxiao=new QAction(QIcon(":/最小化.png"),tr("最小化"),this);
    menuQuit->addAction(openact);
    menuQuit->addAction(suoxiao);
    //连接右键信号槽
    connect(openact,&QAction::triggered,this,[=]{
        this->close();
    });
    connect(suoxiao,&QAction::triggered,this,[=]{
        this->showMinimized();
    });

    btn=false;
    btn_true.addFile(":/背景音乐 .png");
    btn_false.addFile(":/背景音乐静音.png");
    ui->pushButton->setIcon(btn_false);

    //背景音乐播
    plist = new QMediaPlaylist;
    player = new QMediaPlayer;


    //构建歌曲播放链接
    QString play_url = QString("http://music.163.com/song/media/outer/url?id=30352477.mp3");
    //QString play_url1 = QString("http://music.163.com/song/media/outer/url?id=20744788.mp3");
    //30352477  20744788
    plist->addMedia(QUrl(play_url));//把歌曲加入到播放列表
    //plist->addMedia(QUrl(play_url1));
    plist->setCurrentIndex(0);
    player->setPlaylist(plist);

    //搜索模型函数
    Mycompleter();
    QStringListModel *model=new QStringListModel(mylist);
    QCompleter *com=new QCompleter(model);
    ui->line_Search->setCompleter(com);

}

Weather::~Weather()
{
    delete ui;
}

QString Weather::week(int day)
{
    QString week;
    switch (day) {
       case 7:
        week="星期日";
           return week;
       case 1:
        week="星期一";
           return week;
       case 2:
        week="星期二";
           return week;
       case 3:
        week="星期三";
           return week;
       case 4:
        week="星期四";
           return week;
       case 5:
        week="星期五";
           return week;
       case 6:
        week="星期六";
           return week;
       default:
        week="未知";
           return week;
    }
}

void Weather::parseJson(QByteArray jsonByte)
{
    QJsonParseError json_error;
    QJsonDocument parse_document=QJsonDocument::fromJson(jsonByte,&json_error);

    if(json_error.error==QJsonParseError::NoError)
    {
        if(parse_document.isObject())
        {
            QJsonObject rootobj=parse_document.object();

            if(rootobj.contains("cityInfo"))
            {
                QJsonValue valuecity=rootobj.value("cityInfo");
                if(valuecity.isObject())
                {
                    QJsonObject cityobj=valuecity.toObject();
                    if(cityobj.contains("city"))
                    {
                        QJsonValue city_value=cityobj.value("city");
                        if(city_value.isString())
                        {
                            data.city=city_value.toString();
                        }
                    }
                }
            }
            if(rootobj.contains("data"))
            {
                QJsonValue valuedata=rootobj.value("data");
                if(valuedata.isObject())
                {
                    QJsonObject valueobj=valuedata.toObject();
                    if(valueobj.contains("shidu"))
                    {
                        QJsonValue shidu_value=valueobj.value("shidu");
                        if(shidu_value.isString())
                        {
                            data.shidu=shidu_value.toString();
                        }
                    }
                    if(valueobj.contains("pm25"))
                    {
                        QJsonValue pm25_value=valueobj.value("pm25");
                        if (pm25_value.isString())
                            {
                                // 如果 pm25_value 是字符串类型，直接转换
                                data.pm25 = pm25_value.toString();
                            }
                            else if (pm25_value.isDouble()) // 假设 PM25 也可能是浮点数
                            {
                                // 如果 pm25_value 是浮点数，转换为字符串
                                data.pm25 = QString::number(pm25_value.toDouble(), 'f', 2); // 保留两位小数
                                //qDebug() << data.pm25 << " (converted from double)";
                            }
                            else
                            {
                                // 如果 pm25_value 不是字符串或浮点数，你可能需要处理这种情况
                                qDebug() << "pm25_value is not a string or double, but of type" << pm25_value.type();
                            }
                    }
                    if(valueobj.contains("quality"))
                    {
                        QJsonValue quality_value=valueobj.value("quality");
                        if(quality_value.isString())
                        {
                            data.quality=quality_value.toString();
                        }
                    }
                    if(valueobj.contains("wendu"))
                    {
                        QJsonValue wendu_value=valueobj.value("wendu");
                        if(wendu_value.isString())
                        {
                            data.wendu=wendu_value.toString();
                        }
                    }
                    if(valueobj.contains("forecast"))
                    {
                        QJsonValue valuearr=valueobj.value("forecast");
                        if(valuearr.isArray())
                        {
                            QJsonArray arr=valuearr.toArray();
                            int size=arr.size();
                            for(int i=0;i<6;i++)
                            {
                                QJsonValue value=arr.at(i);
                                if(value.isObject())
                                {
                                    QJsonObject for_obj=value.toObject();
                                    if(for_obj.contains("date"))
                                    {
                                        QJsonValue date_value=for_obj.value("date");
                                        if(date_value.isString())
                                        {
                                            data.date=date_value.toString();
                                            fordata.date=date_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("high"))
                                    {
                                        QJsonValue high_value=for_obj.value("high");
                                        if(high_value.isString())
                                        {
                                            data.high=high_value.toString();
                                            fordata.high=high_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("low"))
                                    {
                                        QJsonValue low_value=for_obj.value("low");
                                        if(low_value.isString())
                                        {
                                            data.low=low_value.toString();
                                            fordata.low=low_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("ymd"))
                                    {
                                        QJsonValue ymd_value=for_obj.value("ymd");
                                        if(ymd_value.isString())
                                        {
                                            data.ymd=ymd_value.toString();
                                            fordata.ymd=ymd_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("week"))
                                    {
                                        QJsonValue week_value=for_obj.value("week");
                                        if(week_value.isString())
                                        {
                                            data.week=week_value.toString();
                                            fordata.week=week_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("fx"))
                                    {
                                        QJsonValue fx_value=for_obj.value("fx");
                                        if(fx_value.isString())
                                        {
                                            data.fx=fx_value.toString();
                                            fordata.fx=fx_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("fl"))
                                    {
                                        QJsonValue fl_value=for_obj.value("fl");
                                        if(fl_value.isString())
                                        {
                                            data.fl=fl_value.toString();
                                            fordata.fl=data.fl=fl_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("type"))
                                    {
                                        QJsonValue type_value=for_obj.value("type");
                                        if(type_value.isString())
                                        {
                                            data.type=type_value.toString();
                                            fordata.type=type_value.toString();
                                        }
                                    }
                                    if(for_obj.contains("notice"))
                                    {
                                        QJsonValue notice_value=for_obj.value("notice");
                                        if(notice_value.isString())
                                        {
                                            data.notice=notice_value.toString();
                                        }
                                    }
                                    forlist.push_back(fordata);
                                    //qDebug()<<"for"<<fordata.high<<endl;
                                    farr[i]=fordata;
                                }

                            }
                        }

                    }
                }
            }
        }
    }

    this->update();

}

QString Weather::searchCityID(QString searchtext)
{
    QFile file(":/city.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line;
    QStringList list;
    while(!in.atEnd())
    {
        line=in.readLine();
        if(line.contains(searchtext))
        {
            list=line.split("\t");
            qDebug()<<list.at(0);
            file.close();
            return list.at(0);
        }
    }
    file.close();
    return "Not";
}

bool Weather::eventFilter(QObject *watched, QEvent *event)
{

    if(watched==ui->widget_10 && event->type()==QEvent::Paint)
    {
        drawTempHigh();
        return true;//表示事件以被处理
    }
    else if(watched==ui->widget_11 && event->type()==QEvent::Paint)
    {
        drawTempLow();
        return true;
    }
    return QMainWindow::eventFilter(watched,event);
}

void Weather::drawTempHigh()
{
    QPainter painter(ui->widget_10);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QColor(208,107,39));
    painter.setPen(Qt::red);
    int ave;
    int sum=0;
    int arr[6];
    int offset=0;
    int middle=ui->widget_10->height()/2;
    for(int i=0;i<6;i++)
    {
        QRegularExpression re("\\d+"); // 匹配一个或多个数字
        QRegularExpressionMatchIterator iy = re.globalMatch(farr[i].high);
        QRegularExpressionMatch match = iy.next();
        arr[i]=match.captured(0).toInt();
        sum+=match.captured(0).toInt();
    }
    ave=sum/6;
    QPoint points[6];
    for(int i=0;i<6;i++)
    {
         points[i].setX(la[i]->x()+la[i]->width()/2);
         offset=(arr[i]-ave)*3;
         points[i].setY(middle-offset);
         painter.drawEllipse(QPoint(points[i]),3,3);
         painter.drawText(points[i].x()-15,points[i].y()-15,farr[i].high);
    }
    for(int i=0;i<5;i++)
    {
        painter.drawLine(points[i],points[i+1]);
    }
}

void Weather::drawTempLow()
{
    QPainter painter(ui->widget_11);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(QColor(208,107,39));
    painter.setPen(Qt::blue);
    int ave;
    int sum=0;
    int arr[6];
    int offset=0;
    int middle=ui->widget_11->height()/2;
    for(int i=0;i<6;i++)
    {
        QRegularExpression re("\\d+"); // 匹配一个或多个数字
        QRegularExpressionMatchIterator iy = re.globalMatch(farr[i].low);
        QRegularExpressionMatch match = iy.next();
        arr[i]=match.captured(0).toInt();
        sum+=match.captured(0).toInt();
    }
    ave=sum/6;
    QPoint points[6];
    for(int i=0;i<6;i++)
    {
         points[i].setX(la[i]->x()+la[i]->width()/2);
         offset=(arr[i]-ave)*3;
         points[i].setY(middle-offset);
         painter.drawEllipse(QPoint(points[i]),3,3);
         painter.drawText(points[i].x()-15,points[i].y()-15,farr[i].low);
    }
    for(int i=0;i<5;i++)
    {
        painter.drawLine(points[i],points[i+1]);
    }
}

void Weather::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        menuQuit->exec(QCursor::pos());
    }
    else if(event->button()==Qt::LeftButton)
    {
        mOffset=event->globalPos()-this->pos();
    }
}

void Weather::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}

void Weather::Mycompleter()//填充模型
{
    QFile file(":/city.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString line;
    QStringList list;
    while(!in.atEnd())
    {
        line=in.readLine();
        list=line.split("\t");
        //qDebug()<<list.at(1);
        mylist.append(list.at(1));
    }
    file.close();
}


void Weather::updatetime()
{
    //当前日期获取
    currentTime=QDateTime::currentDateTime();
    currentDate=QDate::currentDate();
    int day=currentDate.dayOfWeek();
    QString dayofweek=week(day);
    QStringList ql=currentTime.toString("yyyy-MM-dd HH:mm:ss").split(" ");
    QString time=ql.at(0);
    QString date=time+" "+dayofweek;
    ui->la_date->setText(date);
}

void Weather::ReplyFinshed(QNetworkReply *reply)
{
    qDebug()<<"网页响应"<<endl;
    QVariant ret=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int st=ret.toInt();
    qDebug()<<"网页返回状态为"<<st<<endl;
    if(ret==200)
    {
        QByteArray data=reply->readAll();
        parseJson(data);
    }
}

void Weather::on_search_clicked()
{
    cityname=ui->line_Search->text();
    QString data1=ui->line_Search->text();
    QString Id=searchCityID(data1);
    if(Id=="Not")
    {
        qDebug()<<"not found"<<endl;
        return ;
    }
    QString url=QString("http://t.weather.sojson.com/api/weather/city/%1").arg(Id);
    manner->get(QNetworkRequest(QUrl(url)));
}

void Weather::setpicture()
{
    if(farr[0].type=="晴")
    {
        ui->picture->setStyleSheet("background-image:url(:/晴.png);border:0px solid black;");
    }
    else if(farr[0].type=="小雨")
    {
         ui->picture->setStyleSheet("background-image:url(:/小雨.png);border:0px solid black;");
    }
    else if(farr[0].type=="中雨")
    {
         ui->picture->setStyleSheet("background-image:url(:/中雨.png);border:0px solid black;");
    }
    else if(farr[0].type=="大雨")
    {
         ui->picture->setStyleSheet("background-image:url(:/大雨.png);border:0px solid black;");
    }
    else if(farr[0].type=="多云")
    {
         ui->picture->setStyleSheet("background-image:url(:/多云.png);border:0px solid black;");
    }
    else if(farr[0].type=="阴")
    {
         ui->picture->setStyleSheet("background-image:url(:/阴天40.png);border:0px solid black;");
    }
    else if(farr[0].type=="霾")
    {
         ui->picture->setStyleSheet("background-image:url(:/雾霾.png);border:0px solid black;");
    }
    QString str1=data.wendu+"℃";
    ui->wendu->setText(str1 );
    QString str=data.high+"~"+data.low;
    ui->wendufw->setText(str);
    ui->city->setText(data.city);

    ui->label_feng->setText(farr[0].fx);
    ui->lable_feng_data1->setText(farr[0].fl);
    ui->lable_shi_data->setText("湿度");
    ui->lable_shi_data2->setText(data.shidu);
    ui->lable_pm_data->setText("PM2.5");
    ui->lable_pm_data2->setText(data.pm25);
    ui->lable_quality_data->setText("空气质量");
    ui->lable_quality_data2->setText(data.quality);

    ui->notice->setText(data.notice);




    ui->label_day1->setText(farr[0].week);
    ui->label_day2->setText(farr[1].week);
    ui->label_day3->setText(farr[2].week);
    ui->label_day4->setText(farr[3].week);
    ui->label_day5->setText(farr[4].week);
    ui->label_day6->setText(farr[5].week);

    ui->label_date1->setText(farr[0].date);
    ui->label_date2->setText(farr[1].date);
    ui->label_date3->setText(farr[2].date);
    ui->label_date4->setText(farr[3].date);
    ui->label_date5->setText(farr[4].date);
    ui->label_date6->setText(farr[5].date);

    ui->label_type7->setText(farr[0].type);
    ui->label_type8->setText(farr[1].type);
    ui->label_type9->setText(farr[2].type);
    ui->label_type10->setText(farr[3].type);
    ui->label_type11->setText(farr[4].type);
    ui->label_type12->setText(farr[5].type);

    ui->label_fx1->setText(farr[0].fx);
    ui->label_fx2->setText(farr[1].fx);
    ui->label_fx3->setText(farr[2].fx);
    ui->label_fx4->setText(farr[3].fx);
    ui->label_fx5->setText(farr[4].fx);
    ui->label_fx6->setText(farr[5].fx);

    ui->label_fl1->setText(farr[0].fl);
    ui->label_fl2->setText(farr[1].fl);
    ui->label_fl3->setText(farr[2].fl);
    ui->label_fl4->setText(farr[3].fl);
    ui->label_fl5->setText(farr[4].fl);
    ui->label_fl6->setText(farr[5].fl);


    for(int i=0;i<6;i++)
    {
        if(farr[i].type=="晴")
        {
            la[i]->setStyleSheet("background-image:url(:/晴40.png);border:0px solid black;");
            la[i]->hasScaledContents();
        }
        else if(farr[i].type=="小雨")
        {
            la[i]->setStyleSheet("background-image:url(:/小雨40.png);border:0px solid black;");
        }
        else if(farr[i].type=="中雨")
        {
            la[i]->setStyleSheet("background-image:url(:/中雨40.png);border:0px solid black;");
        }
        else if(farr[i].type=="大雨")
        {
            la[i]->setStyleSheet("background-image:url(:/大雨40.png);border:0px solid black;");
        }
        else if(farr[i].type=="阴")
        {
            la[i]->setStyleSheet("background-image:url(:/阴天400.png);border:0px solid black;");
        }
        else if(farr[i].type=="多云")
        {
            la[i]->setStyleSheet("background-image:url(:/多云40.png);border:0px solid black;");
        }
        else if(farr[i].type=="霾")
        {
            la[i]->setStyleSheet("background-image:url(:/雾霾40.png);border:0px solid black;");
        }
    }

}

//背景音乐按键槽
void Weather::on_pushButton_clicked()
{
    if(btn==true)
    {
        ui->pushButton->setIcon(btn_true);
        btn=false;
        player->play();
    }
    else if(btn==false)
    {
        ui->pushButton->setIcon(btn_false);
        btn=true;
        player->pause();
    }
}

void Weather::positionChanged(qint64 position)
{
    qDebug()<<position<<endl;
}

void Weather::on_pushButton_2_clicked()
{
    QString Id=searchCityID(cityname);
    if(Id=="Not")
    {
        qDebug()<<"not found"<<endl;
        return ;
    }
    QString url=QString("http://t.weather.sojson.com/api/weather/city/%1").arg(Id);
    manner->get(QNetworkRequest(QUrl(url)));
}
