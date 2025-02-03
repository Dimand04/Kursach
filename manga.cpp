#include "manga.h"
#include "ui_manga.h"
#include "qpushlabel.h"
#include <QTextLayout>

manga::manga(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::manga)
{
    ui->setupUi(this);
    connect(ui->lb_manga_form_avatar, &QPushLabel::clicked, this, &manga::getClicked);
}

manga::~manga()
{
    delete ui;
}

void manga::setMangaText(const QString text, QString id, QPixmap image)
{
    ID = id;
    ui->lb_manga_form_name->setWordWrap(true);
    ui->lb_manga_form_name->setText(text);
    ui->lb_manga_form_avatar->setPixmap(image);
}

void manga::getClicked()
{
    emit clicked(ID);
}
