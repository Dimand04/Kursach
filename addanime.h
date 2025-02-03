#ifndef ADDANIME_H
#define ADDANIME_H

#include <QWidget>

namespace Ui {
class addanime;
}

class addanime : public QWidget
{
    Q_OBJECT

public:
    explicit addanime(QWidget *parent = nullptr);
    ~addanime();

private:
    Ui::addanime *ui;
};

#endif // ADDANIME_H
