#ifndef ADDNEWCHAPTER_H
#define ADDNEWCHAPTER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QByteArray>
#include <QLabel>

namespace Ui {
class addnewchapter;
}

class addnewchapter : public QWidget
{
    Q_OBJECT

public:
    explicit addnewchapter(int mangaID, QWidget *parent = nullptr);
    ~addnewchapter();

signals:
    void chapterAdded();
    void cancelAdding();

public slots:
    void cancelAddChapter();

private:
    Ui::addnewchapter *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "animanga");
    QPixmap getImageFromDB(QByteArray);

    void addNewChapter();
    void addNewPages();
    int mangaID;
    //int mangaID;
};

#endif // ADDNEWCHAPTER_H
