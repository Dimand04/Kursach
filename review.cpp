#include "review.h"
#include "ui_review.h"
#include <QTextLayout>

review::review(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::review)
{
    ui->setupUi(this);
}

review::~review()
{
    delete ui;
}

void review::loadReview(const QString nick, QString text, QPixmap image)
{
    ui->lb_review_name->setText(nick);
    ui->lb_review_avatar->setPixmap(image);
    ui->lb_review_text->setPlainText(text);
}
