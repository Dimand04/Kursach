#ifndef ADDNEWREVIEW_H
#define ADDNEWREVIEW_H

#include <QWidget>
# include "mainwindow.h"

namespace Ui {
class addnewreview;
}

class addnewreview : public QWidget
{
    Q_OBJECT

signals:
    void reviewAdded();
    void cancelAdding();

public:
    explicit addnewreview(QWidget *parent = nullptr);
    ~addnewreview();

public slots:
    void addNewReview();
    void cancelReview();
    void setUserID(const QString &id) { userID = id; }
    void setMangaID(const QString &id) { mangaID = id; }

private:
    Ui::addnewreview *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "animanga");
    QString userID;
    QString mangaID;
};

#endif // ADDNEWREVIEW_H
