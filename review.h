#ifndef REVIEW_H
#define REVIEW_H

#include <QWidget>

namespace Ui {
class review;
}

class review : public QWidget
{
    Q_OBJECT

public:
    explicit review(QWidget *parent = nullptr);
    ~review();
    void loadReview(const QString nick, QString text, QPixmap image);

private:
    Ui::review *ui;
};

#endif // REVIEW_H
