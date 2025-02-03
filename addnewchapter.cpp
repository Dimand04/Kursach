#include "addnewchapter.h"
#include "ui_addnewchapter.h"
#include <QFileDialog>
#include <QSqlQuery>
#include <QImage>
#include <QBuffer>
#include "globals.h"

addnewchapter::addnewchapter(int mangaID, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addnewchapter)
    , mangaID(mangaID)

{
    ui->setupUi(this);

    connect (ui->add_new_list,&QPushButton::clicked,this,&addnewchapter::addNewPages);
    connect(ui->qpb_add_new_chapter, &QPushButton::clicked, this, &addnewchapter::addNewChapter);
    connect(ui->qpb_cancel, &QPushButton::clicked, this, &addnewchapter::cancelAddChapter);
}

addnewchapter::~addnewchapter()
{
    delete ui;
}

QPixmap addnewchapter::getImageFromDB(QByteArray arr)
{
    QPixmap image;
    image.loadFromData(arr);
    return image;
}

void addnewchapter::addNewPages()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Выберите страницу манги"), "", tr("Images (*.png *.xpm *.jpg)"));

    if (fileNames.isEmpty())
        return;

    foreach (const QString &fileName, fileNames)
    {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::warning(this, "Ошибка загрузки изображения", "Не удалось загрузить: " + fileName);
            continue;
        }

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        QTableWidgetItem *item = new QTableWidgetItem(fileName);
        ui->tableWidget->setItem(row, 0, item);

        QLabel *label = new QLabel;
        label->setPixmap(QPixmap::fromImage(image).scaled(100, 100, Qt::KeepAspectRatio));
        ui->tableWidget->setCellWidget(row, 1, label);
    }
}

void addnewchapter::addNewChapter()
{
    QString chapterName = ui->lineEdit->text();
    QString chapterNumber = ui->lineEdit_2->text();

    if (chapterName.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Название главы не может быть пустым.");
        return;
    }

    if (getDBConnection(db))
    {
        QSqlQuery query(db);

        // Начало транзакции
        db.transaction();

        query.prepare("INSERT INTO chapters (name, number, manga_id) VALUES (:name, :number, :manga_id)");
        query.bindValue(":name", chapterName);
        query.bindValue(":number", chapterNumber);
        query.bindValue(":manga_id", mangaID); // Использование переданного значения mangaID
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить новую главу.");
            return;
        }

        int chapterID = query.lastInsertId().toInt();

        for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
        {
            QString fileName = ui->tableWidget->item(row, 0)->text();
            QImage image(fileName);

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            image.save(&buffer, "PNG");

            query.prepare("INSERT INTO pages (chapter_id, image) VALUES (:chapter_id, :image)");
            query.bindValue(":chapter_id", chapterID);
            query.bindValue(":image", byteArray);
            if (!query.exec()) {
                db.rollback();
                QMessageBox::critical(this, "Ошибка", "Не удалось добавить страницы в новую главу.");
                return;
            }
        }

        // Завершение транзакции
        if (db.commit()) {
            emit chapterAdded();
        } else {
            db.rollback();
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить изменения в базе данных.");
        }
    }
}

void addnewchapter::cancelAddChapter()
{
    emit cancelAdding();
}
