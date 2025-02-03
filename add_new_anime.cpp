#include "add_new_anime.h"
#include "ui_add_new_anime.h"

add_new_anime::add_new_anime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::add_new_anime)
{
    ui->setupUi(this);
}

add_new_anime::~add_new_anime()
{
    delete ui;
}
