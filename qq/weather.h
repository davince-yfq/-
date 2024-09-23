#ifndef WEATHER_H
#define WEATHER_H

#include <QMainWindow>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QVariant>
#include <QFile>
#include <QTextStream>
#include <comment.h>
#include <QLabel>
#include <QPainter>
#include <QRegularExpression>
#include <QMoveEvent>
#include <QCursor>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaPlaylist>
#include <QCompleter>
#include <QStringListModel>
#include <QMovie>

namespace Ui {
class Weather;
}

class Weather : public QMainWindow
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();
    QDateTime currentTime;
    QDate currentDate;
    QString week(int day);
    void parseJson(QByteArray jsonByte);
    //根据输入文本查询城市号
    QString searchCityID(QString searchtext);
    WtData data;
    Forecast fordata;
    QList<Forecast> forlist;

    Forecast farr[6];
    //int sum=0;

    QLabel *la[6];   //预测图片标签集

    QList<QPoint> Phigh;
    QList<QPoint> Plow;
    QPainter painter;

    //事件过滤器
    bool eventFilter(QObject *watched, QEvent *event);
    //画高低温函数
    void drawTempHigh();
    void drawTempLow();

    //天气界面退出按键事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool btn;
    QIcon btn_true;
    QIcon btn_false;

    QString cityname;//保存当前搜索城市用于刷新

    QCompleter mycom;//匹配模型
    //搜索框匹配函数
    void Mycompleter();
    QStringList mylist;
private:
    Ui::Weather *ui;
    QNetworkAccessManager *manner;
    QMenu *menuQuit;
    QPoint mOffset;//窗口移动偏移值

    QMediaPlayer *player;
    QMediaPlaylist *plist;
private slots:
    void updatetime();
    void ReplyFinshed(QNetworkReply *reply);
    void on_search_clicked();
    void setpicture();

    void on_pushButton_clicked();
    void positionChanged(qint64 position);
    void on_line_Search_textChanged(const QString &arg1);
    void on_pushButton_2_clicked();
};

#endif // WEATHER_H
