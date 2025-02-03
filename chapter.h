#ifndef CHAPTER_H
#define CHAPTER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QByteArray>
#include <QLabel>

namespace Ui {
class chapter;
}

class chapter : public QWidget
{
    Q_OBJECT

public:
    explicit chapter(int chapterID, QWidget *parent = nullptr);
    ~chapter();

private:
    Ui::chapter *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "animanga");
    int chapterID;
    void loadImages();
};

#endif // CHAPTER_H
