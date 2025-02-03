#include "addanime.h"
#include "ui_addanime.h"

addanime::addanime(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addanime)
{
    ui->setupUi(this);
}

addanime::~addanime()
{
    delete ui;
}
