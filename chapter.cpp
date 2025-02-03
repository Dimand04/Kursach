#include "chapter.h"
#include "ui_chapter.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QImage>
#include <QBuffer>
#include "globals.h"
#include <QDebug>

chapter::chapter(int chapterID, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chapter),
    chapterID(chapterID)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,684);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(1000);
    loadImages();
}

chapter::~chapter()
{
    delete ui;
}

void chapter::loadImages()
{
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT image FROM pages WHERE chapter_id = ?");
        query.addBindValue(chapterID);
        query.exec();

        int row = 0;

        while (query.next())
        {
            QByteArray imageData = query.value(0).toByteArray();
            QImage image;
            image.loadFromData(imageData);

            // Создаем элемент для отображения изображения в ячейке
            QLabel *imageLabel = new QLabel;
            //imageLabel->setPixmap(QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatio));
            imageLabel->setPixmap(QPixmap::fromImage(image).scaled(684, 1000, Qt::KeepAspectRatio));

            // Добавляем новую строку в таблицу
            ui->tableWidget->insertRow(row);

            // Вставляем элемент в таблицу
            ui->tableWidget->setCellWidget(row, 0, imageLabel);

            // Увеличиваем номер строки
            ++row;
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", "Нет соединения с базой данных");
    }
}
