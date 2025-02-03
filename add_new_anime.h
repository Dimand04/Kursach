#ifndef ADD_NEW_ANIME_H
#define ADD_NEW_ANIME_H

#include <QWidget>

namespace Ui {
class add_new_anime;
}

class add_new_anime : public QWidget
{
    Q_OBJECT

public:
    explicit add_new_anime(QWidget *parent = nullptr);
    ~add_new_anime();

private:
    Ui::add_new_anime *ui;
};

#endif // ADD_NEW_ANIME_H
