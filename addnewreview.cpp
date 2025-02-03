#include "addnewreview.h"
#include "ui_addnewreview.h"
#include "globals.h"

addnewreview::addnewreview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addnewreview)
{
    ui->setupUi(this);
    connect (ui->qpb_add_review,&QPushButton::clicked,this,&addnewreview::addNewReview);
    connect (ui->qpb_cancel_review,&QPushButton::clicked,this,&addnewreview::cancelReview);
}

addnewreview::~addnewreview()
{
    delete ui;
}

void addnewreview::addNewReview()
{
    //db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QString reviewText = ui->lb_add_review_text->toPlainText();
        QSqlQuery query(db);
        query.prepare("INSERT INTO reviews (manga_id, user_id, review_text) VALUES (?, ?, ?)");
        query.addBindValue(mangaID);
        query.addBindValue(userID);
        query.addBindValue(reviewText);
        if (query.exec())
        {
            emit reviewAdded();
        }
    }
}

void addnewreview::cancelReview()
{
    emit cancelAdding();
}
