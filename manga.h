#ifndef MANGA_H
#define MANGA_H

#include <QWidget>

namespace Ui {
class manga;
}

class manga : public QWidget
{
    Q_OBJECT

public:
    explicit manga(QWidget *parent = nullptr);
    ~manga();
    QString getID() { return ID; }
    void setMangaText(const QString text, QString id, QPixmap image);

public slots:
    void getClicked();

signals:
    void clicked(QString);

private:
    Ui::manga *ui;
    QString ID;
};

#endif // MANGA_H
