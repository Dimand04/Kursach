#include "deleteuser.h"
#include "ui_deleteuser.h"
#include "globals.h"

deleteuser::deleteuser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::deleteuser)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setGeometry(440, 210, 400, 300);
    connect (ui->btn_delete,&QPushButton::clicked,this,&deleteuser::delusr);
    connect (ui->btn_delete_cancel,&QPushButton::clicked,this,&deleteuser::deleteCancel);
}

deleteuser::~deleteuser()
{
    delete ui;
}

void deleteuser::delusr()
{
    QString userLocalLogin = GlobalVars::userLogin;
    if (ui->le_password_del->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста заполните поле 'Пароль'.");
        return;
    }
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT id FROM user INNER JOIN authorization ON user.id = authorization.user_id WHERE user.user_nickname = ? AND authorization.user_password = ?;");
        query.addBindValue(userLocalLogin);
        query.addBindValue(ui->le_password_del->text());
        query.exec();
        if(query.next())
        {
            QList<QMessageBox::Button> buttons;
            buttons.append(QMessageBox::Ok);
            buttons.append(QMessageBox::Cancel);
            if(showMessage("Вы уверены что хотите удалить аккаунт?", "Подтвердите", buttons, QMessageBox::Question) == QMessageBox::Ok)
            {
                QString userID = query.value(0).toString();

                // Удаляем записи из таблицы ratings
                query.prepare("DELETE FROM ratings WHERE user_id = ?");
                query.addBindValue(userID);
                query.exec();

                // Удаляем записи из таблицы reviews
                query.prepare("DELETE FROM reviews WHERE user_id = ?");
                query.addBindValue(userID);
                query.exec();

                // Удаляем записи из таблицы user_fav_manga
                query.prepare("DELETE FROM user_fav_manga WHERE user_id = ?");
                query.addBindValue(userID);
                query.exec();

                // Удаляем запись из таблицы authorization
                query.prepare("DELETE FROM authorization WHERE user_id = ?");
                query.addBindValue(userID);
                query.exec();

                // Удаляем запись из таблицы user
                query.prepare("DELETE FROM user WHERE id = ?");
                query.addBindValue(userID);
                query.exec();

                if(query.exec())
                {
                    ui->le_password_del->clear();
                    emit userDeleted();
                }
            }
            else
            {
                ui->le_password_del->clear();
            }
        }
        else
        {
            ui->le_password_del->clear();
            QMessageBox::warning(this, "Ошибка", "В процессе удаления аккаунта произошла ошибка. Пользователя с такими данными не существует.");
        }
    }
}

void deleteuser::deleteCancel()
{
    emit userDeleteCancel();
}
