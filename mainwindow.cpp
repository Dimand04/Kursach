#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>
#include "globals.h"
#include <QProcess>
#include <QDebug>
#include "deleteuser.h"
#include "manga.h"
#include "review.h"
#include <QCheckBox>
#include "addnewreview.h"
#include "addnewchapter.h"
#include "chapter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    MPlayer->setAudioOutput(audioOutput);

    ui->tableWidget->setColumnWidth(0,214);
    ui->tableWidget->setColumnWidth(1,214);
    ui->tableWidget->setColumnWidth(2,214);
    ui->tableWidget->setColumnWidth(3,214);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(294);
    ui->tableWidget1->setColumnWidth(0,214);
    ui->tableWidget1->setColumnWidth(1,214);
    ui->tableWidget1->setColumnWidth(2,214);
    ui->tableWidget1->setColumnWidth(3,214);
    ui->tableWidget1->setColumnWidth(4,214);
    ui->tableWidget1->verticalHeader()->setDefaultSectionSize(294);
    ui->tableWidget_3->setColumnWidth(0,214);
    ui->tableWidget_3->verticalHeader()->setDefaultSectionSize(100);

    ui->tableWidget_4->setColumnWidth(1,50);
    ui->tableWidget_4->setColumnWidth(2,30);
    ui->tableWidget_4->setColumnWidth(3,985);
    ui->tableWidget_4->verticalHeader()->setDefaultSectionSize(30);

    lightColor = QColor(QColor(255, 255, 255, 230));
    darkColor = QColor(QColor(33, 33, 33, 230));
    lightTextColor = QColor(Qt::black);
    darkTextColor = QColor(QColor(245, 245, 245));
    isDarkMode = false;

    MPlayer->setSource(QUrl("qrc:/new/prefix1/Pic/01 - Genshin Impact Main Theme.mp3"));
    MPlayer->play();

    connect (ui->BTN_login,&QPushButton::clicked,this,&MainWindow::tryAuthorization);
    connect (ui->BTN_Autho,&QPushButton::clicked,this,&MainWindow::showAutoMenu);
    connect (ui->qlb_profile,&QPushLabel::clicked,this,&MainWindow::showProfileMenu);
    connect (ui->qlb_manga,&QPushLabel::clicked,this,&MainWindow::showMangaMenu);
    connect(ui->lb_profile_avatar, &QPushLabel::clicked, [this]{ selectUserAvatar(userID); });
    connect(ui->lb_manga_add_icon, &QPushLabel::clicked, this, &MainWindow::selectMangaIcon);
    connect (ui->btn_manga_add,&QPushButton::clicked,this,&MainWindow::addNewManga);
    connect (ui->btn_add_new_manga,&QPushButton::clicked,this,&MainWindow::showMangaAddMenu);
    connect (ui->btn_delete_user,&QPushButton::clicked,this,&MainWindow::deleteUser);
    connect (ui->btn_themechange,&QPushButton::clicked,this,&MainWindow::toggleColor);
    connect(ui->btn_search, &QPushButton::clicked, this, &MainWindow::searchManga);
    connect(ui->btn_select_audio, &QPushButton::clicked, this, &MainWindow::selectAudioFile);
    connect(ui->PB_Play, &QPushButton::clicked, this, &MainWindow::audioPlay);
    connect(ui->PB_Stop, &QPushButton::clicked, this, &MainWindow::audioStop);
    connect(ui->btn_manga_edit, &QPushButton::clicked, [this]{showEditManga(currentMangaID);});
    connect(ui->btn_add_fav, &QPushButton::clicked, [this]{addToFavorites(currentMangaID);});
    connect(ui->btn_manga_delete, &QPushButton::clicked, [this]{deleteManga(currentMangaID);});
    connect(ui->btn_manga_editc, &QPushButton::clicked, [this]{editManga(currentMangaID);});
    connect(ui->btn_del_fav, &QPushButton::clicked, [this]{delFromFavorites(currentMangaID);});
    connect(ui->qpb_genre_sort, &QPushButton::clicked, this, &MainWindow::filterMangaByGenre);
    connect(ui->qpb_rating_add, &QPushButton::clicked, this, &MainWindow::addRating);
    connect (ui->qpb_add_new_review,&QPushButton::clicked,this,&MainWindow::showNewReview);
    connect (ui->qpb_add_chapter,&QPushButton::clicked,this,&MainWindow::showNewChapter);
    connect(ui->tableWidget_4, &QTableWidget::cellClicked, this, &MainWindow::TableWidgetRowClicked);
    connect (ui->qpb_del_review,&QPushButton::clicked,this,&MainWindow::delReview);
    connect (ui->qlb_anime,&QPushLabel::clicked,this,&MainWindow::showAnimeMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// функция назначения роли
bool MainWindow::hasAccess(const QString &requiredRole)
{
    return userRole == requiredRole;
    qDebug() << userRole;
}

// запуск аудио
void MainWindow::audioPlay()
{
    MPlayer->play();
}

//остановка аудио
void MainWindow::audioStop()
{
    MPlayer->stop();
}

// показать меню авторизации
void MainWindow::showAutoMenu()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение выхода", "Вы действительно хотите выйти из аккаунта?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Выполнение выхода из аккаунта
        MPlayer->setSource(QUrl("qrc:/new/prefix1/Pic/01 - Genshin Impact Main Theme.mp3"));
        MPlayer->play();
        ui->main_frame->setGeometry(ui->main_frame->x(), 1000, ui->main_frame->width(), ui->main_frame->height());
        ui->authorization_frame->setGeometry(ui->authorization_frame->x(), 0, ui->authorization_frame->width(), ui->authorization_frame->height());
        ui->manga_frame->setGeometry(ui->manga_frame->x(), 1000, ui->manga_frame->width(), ui->manga_frame->height());
        ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(), 1000, ui->manga_add_frame->width(), ui->manga_add_frame->height());
        ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(), 1000, ui->current_manga_frame->width(), ui->current_manga_frame->height());
        ui->profile_frame->setGeometry(ui->profile_frame->x(), 1000, ui->profile_frame->width(), ui->profile_frame->height());
        ui->anime_frame->setGeometry(ui->anime_frame->x(),1000,ui->anime_frame->width(),ui->anime_frame->height());
        ui->main_frame->setEnabled(false);
        ui->authorization_frame->setEnabled(true);
        ui->manga_frame->setEnabled(false);
        ui->profile_frame->setEnabled(false);
        ui->anime_frame->setEnabled(false);
        clearRowsInTable(ui->tableWidget);
        clearRowsInTable(ui->tableWidget1);
        clearRowsInTable(ui->tableWidget3);
        clearRowsInTable(ui->tableWidget_3);

        userID.clear();

        // if(!m_currentFrame.isEmpty())
        // {
        //     toggleFrame(false, m_currentFrame);
        // }

        if (!ui->authorization_frame->isVisible())
        {
            ui->authorization_frame->setVisible(true);
            ui->le_login->setFocus();
        }
        connect(ui->BTN_login, &QPushButton::clicked, this, &MainWindow::tryAuthorization);
    }
    else
    {

    }
}

void MainWindow::showAnimeMenu()
{
    ui->manga_frame->setGeometry(ui->manga_frame->x(),1000,ui->manga_frame->width(),ui->manga_frame->height());
    ui->anime_frame->setGeometry(ui->anime_frame->x(),0,ui->anime_frame->width(),ui->anime_frame->height());
    ui->profile_frame->setGeometry(ui->profile_frame->x(),1000,ui->profile_frame->width(),ui->profile_frame->height());
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),1000,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),1000,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->manga_frame->setEnabled(false);
    ui->anime_frame->setEnabled(true);
    ui->profile_frame->setEnabled(false);
    ui->current_manga_frame->setEnabled(false);
    ui->manga_add_frame->setEnabled(false);
}

// показать главное меню
void MainWindow::showMainMenu()
{
    ui->authorization_frame->setGeometry(ui-> authorization_frame->x(),1000,ui->authorization_frame->width(),ui->authorization_frame->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),0,ui->main_frame->width(),ui->main_frame->height());
    ui->profile_frame->setGeometry(ui-> profile_frame->x(),1000,ui->profile_frame->width(),ui->profile_frame->height());
    ui->manga_frame->setGeometry(ui->manga_frame->x(),0,ui->manga_frame->width(),ui->manga_frame->height());
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),1000,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->anime_frame->setGeometry(ui->anime_frame->x(),1000,ui->anime_frame->width(),ui->anime_frame->height());
    ui->main_frame->setEnabled(true);
    ui->authorization_frame->setEnabled(false);
    ui->profile_frame->setEnabled(false);
    ui->manga_frame->setEnabled(true);
    ui->manga_add_frame->setEnabled(false);
    ui->current_manga_frame->setEnabled(false);
    ui->anime_frame->setEnabled(false);
    clearRowsInTable(ui->tableWidget);
    getUserInfo(userID);
    mangatable();
    showMangaMenu();
}

// показать меню профиля
void MainWindow::showProfileMenu()
{
    ui->profile_frame->setGeometry(ui->profile_frame->x(),0,ui->profile_frame->width(),ui->profile_frame->height());
    ui->manga_frame->setGeometry(ui->manga_frame->x(),1000,ui->manga_frame->width(),ui->manga_frame->height());
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),1000,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),1000,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->anime_frame->setGeometry(ui->anime_frame->x(),1000,ui->anime_frame->width(),ui->anime_frame->height());
    ui->profile_frame->setEnabled(true);
    ui->manga_frame->setEnabled(false);
    ui->manga_add_frame->setEnabled(false);
    ui->anime_frame->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
    clearRowsInTable(ui->tableWidget_3);
}

// показать меню манги
void MainWindow::showMangaMenu()
{
    clearRowsInTable(ui->tableWidget);
    ui->manga_frame->setGeometry(ui->manga_frame->x(),0,ui->manga_frame->width(),ui->manga_frame->height());
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),1000,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),1000,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->profile_frame->setGeometry(ui->profile_frame->x(),1000,ui->profile_frame->width(),ui->profile_frame->height());
    ui->anime_frame->setGeometry(ui->anime_frame->x(),1000,ui->anime_frame->width(),ui->anime_frame->height());
    ui->main_frame->setEnabled(true);
    ui->manga_frame->setEnabled(true);
    ui->manga_add_frame->setEnabled(false);
    ui->current_manga_frame->setEnabled(false);
    ui->profile_frame->setEnabled(false);
    ui->anime_frame->setEnabled(false);
    clearRowsInTable(ui->tableWidget_2);
    clearRowsInTable(ui->tableWidget3);
    clearRowsInTable(ui->tableWidget_3);
    mangatable();
    getUserInfo(userID);
    ui->tableWidget3->setColumnHidden(1, true);
    ui->tableWidget3->setColumnWidth(0, 10);
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.exec("select * from manga_genre;");
        while(query.next())
        {
            ui->tableWidget3->insertRow(ui->tableWidget3->rowCount());
            QCheckBox *chkb = new QCheckBox();
            chkb->setCheckState(Qt::Unchecked);
            ui->tableWidget3->setCellWidget(ui->tableWidget3->rowCount()-1, 0, chkb);
            ui->tableWidget3->setItem(ui->tableWidget3->rowCount()-1, 1, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget3->setItem(ui->tableWidget3->rowCount()-1, 2, new QTableWidgetItem(query.value(1).toString()));
        }
        ui->tableWidget3->resizeRowsToContents();
    }
}

// показать меню добавления новой манги
void MainWindow::showMangaAddMenu()
{
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),0,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->manga_frame->setGeometry(ui->manga_frame->x(),1000,ui->manga_frame->width(),ui->manga_frame->height());
    ui->manga_add_frame->setEnabled(true);
    ui->manga_frame->setEnabled(false);
    ui->btn_manga_editc->setVisible(false);
    ui->btn_manga_add->setVisible(true);
    clearRowsInTable(ui->tableWidget);
    ui->tableWidget_2->setColumnHidden(1, true);
    ui->tableWidget_2->setColumnWidth(0, 10);
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.exec("select * from manga_genre;");
        while(query.next())
        {
            ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
            QCheckBox *chkb = new QCheckBox();
            chkb->setCheckState(Qt::Unchecked);
            ui->tableWidget_2->setCellWidget(ui->tableWidget_2->rowCount()-1, 0, chkb);
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 1, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1, 2, new QTableWidgetItem(query.value(1).toString()));
        }
        ui->tableWidget_2->resizeRowsToContents();
    }
}

// показать авторизацию (скорее всего удалить)
void MainWindow::showAuthorization()
{
    userID.clear();
    //userRole.clear();

    // if(!m_currentFrame.isEmpty())
    // {
    //     toggleFrame(false, m_currentFrame);
    // }

    if(!ui->authorization_frame->isVisible())
    {
        ui->authorization_frame->setVisible(true);
        ui->le_login->setFocus();
    }
}

// удаление пользователя
void MainWindow::deleteUser()
{
    deleteuser *del = new deleteuser;
    del->show();
    ui->profile_frame->setGeometry(ui-> profile_frame->x(),1000,ui->profile_frame->width(),ui->profile_frame->height());
    ui->profile_frame->setEnabled(false);
    connect(del, &deleteuser::userDeleted, [=]()
        {
            del->hide();
            QList<QMessageBox::Button>buttons={QMessageBox::Ok};
            showMessage("Аккаунт успешно удалён!","",QMessageBox::Information,buttons);
            showAutoMenu();
        });
    connect(del, &deleteuser::userDeleteCancel, [=]()
        {
            del->hide();
            ui->profile_frame->setGeometry(ui-> profile_frame->x(),0,ui->profile_frame->width(),ui->profile_frame->height());
            ui->profile_frame->setEnabled(true);
        });
}

// авторизация
void MainWindow::tryAuthorization()
{
    QString login = ui->le_login->text().remove(" ");
    QString password = ui->le_password->text().remove(" ");

    if (login.isEmpty() && password.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните поля 'Логин' и 'Пароль'.");
        return;
    }
    else if (login.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните поле 'Логин'.");
        return;
    }
    else if (password.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните поле 'Пароль'.");
        return;
    }

    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT id, user_role FROM user INNER JOIN authorization ON user.id = authorization.user_id WHERE user.user_nickname = ? AND authorization.user_password = ?;");
        query.addBindValue(login);
        QString userLoginValue = ui->le_login->text();
        GlobalVars::userLogin = userLoginValue;
        query.addBindValue(password);
        query.exec();
        if(query.next())
        {
            userID = query.value(0).toString();
            userRole = query.value(1).toString();
            ui->le_login->clear();
            ui->le_password->clear();
            disconnect(ui->BTN_login, &QPushButton::clicked, this, &MainWindow::tryAuthorization);
            ui->authorization_frame->setVisible(false);
            ui->main_frame->setEnabled(true);
            db = QSqlDatabase::database("animanga");
            if(getDBConnection(db))
            {
                QList<QMessageBox::Button> buttons = {QMessageBox::Ok};
                showMessage("Успешная авторизация!", "", QMessageBox::Information, buttons);
            }
            else
            {
                QList<QMessageBox::Button> buttons = {QMessageBox::Ok};
                showMessage("Ошибка авторизации!", "", QMessageBox::Critical, buttons);
            }
            showMainMenu();
            adminFunction();
            MPlayer->stop();
        }
        else
        {
            QList<QMessageBox::Button> buttons;
            buttons.append(QMessageBox::Ok);
            buttons.append(QMessageBox::Cancel);
            if(showMessage("Нет пользователей с такими данными.\nВы хотите создать новый аккаунт?", "Confirm choice", buttons, QMessageBox::Question) == QMessageBox::Ok)
            {
                query.prepare("INSERT INTO user (user_nickname, user_registration, user_role) VALUES (?, ?, ?);");
                query.addBindValue(ui->le_login->text());
                query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
                query.addBindValue("user");
                if(query.exec())
                {
                    query.exec("SELECT MAX(id) FROM user;");
                    if(query.next())
                    {
                        userID = query.value(0).toString();
                        query.prepare("INSERT INTO authorization (user_id, user_password) VALUES (?, ?);");
                        query.addBindValue(userID);
                        query.addBindValue(ui->le_password->text());
                        if(query.exec())
                        {
                            ui->le_login->clear();
                            ui->le_password->clear();
                            disconnect(ui->BTN_login, &QPushButton::clicked, this, &MainWindow::tryAuthorization);
                            ui->authorization_frame->setVisible(false);
                            ui->main_frame->setEnabled(true);
                            showMainMenu();
                            adminFunction();
                            MPlayer->stop();
                        }
                        else
                        {
                            showMessage("Блин, у вас тут ошибка.", "Unexpected error", QMessageBox::Button::Ok, QMessageBox::Question);
                            ui->le_login->clear();
                            ui->le_password->clear();
                            ui->le_login->setFocus();
                        }
                    }
                    else
                    {
                        showMessage("Блин, у вас тут ошибка.", "Unexpected error", QMessageBox::Button::Ok, QMessageBox::Question);
                        ui->le_login->clear();
                        ui->le_password->clear();
                        ui->le_login->setFocus();
                    }
                }
                else
                {
                    showMessage("Блин, у вас тут ошибка.", "Unexpected error", QMessageBox::Button::Ok, QMessageBox::Question);
                    ui->le_login->clear();
                    ui->le_password->clear();
                    ui->le_login->setFocus();
                }
            }
            else
            {
                ui->le_password->clear();
                ui->le_password->setFocus();
            }
        }
    }
}

// определение прав пользователя
void MainWindow::adminFunction()
{
    if(hasAccess("user"))
    {
        ui->btn_add_new_manga->setVisible(false);
    }
    else
    {
        ui->btn_add_new_manga->setVisible(true);
    }
}

// определение темы программы
void MainWindow::toggleColor()
{
    if (isDarkMode) {
        ui->label_6->setPalette(QPalette(lightColor));
        ui->lb_profile->setPalette(QPalette(lightColor));
        ui->qlb_manga->setPalette(QPalette(lightColor));
        ui->btn_themechange->setPalette(QPalette(lightColor));
        ui->BTN_Autho->setPalette(QPalette(lightColor));
        ui->label_9->setPalette(QPalette(lightColor));
        //ui->label_6->setStyleSheet("QLabel { color : " + (isDarkMode ? lightTextColor.name() : darkTextColor.name()) + "; }");
        ui->lineEdit->setPalette(QPalette(lightColor));
        ui->btn_search->setPalette(QPalette(lightColor));
        ui->btn_add_new_manga->setPalette(QPalette(lightColor));
        ui->tableWidget->setPalette(QPalette(lightColor));
        ui->label_28->setPalette(QPalette(lightColor));
        //ui->label_3->setPalette(QPalette(lightColor));
        ui->tabWidget->setPalette(QPalette(lightColor));
        ui->lb_username->setPalette(QPalette(lightColor));
        ui->tableWidget3->setPalette(QPalette(lightColor));
        ui->tabWidget_2->setPalette(QPalette(lightColor));
        ui->qpb_genre_sort->setPalette(QPalette(lightColor));
        ui->btn_manga_add->setPalette(QPalette(lightColor));
        ui->btn_manga_editc->setPalette(QPalette(lightColor));
        ui->lb_manga_add_name->setPalette(QPalette(lightColor));
        ui->lb_manga_add_description->setPalette(QPalette(lightColor));
        ui->le_audio_path->setPalette(QPalette(lightColor));
        ui->btn_select_audio->setPalette(QPalette(lightColor));
        ui->tableWidget_2->setPalette(QPalette(lightColor));
        ui->qlb_anime->setPalette(QPalette(lightColor));
        ui->label_8->setPalette(QPalette(lightColor));
        } else {
        ui->label_6->setPalette(QPalette(darkColor));
        ui->lb_profile->setPalette(QPalette(darkColor));
        ui->qlb_manga->setPalette(QPalette(darkColor));
        ui->btn_themechange->setPalette(QPalette(darkColor));
        ui->BTN_Autho->setPalette(QPalette(darkColor));
        ui->label_9->setPalette(QPalette(darkColor));
        ui->lineEdit->setPalette(QPalette(darkColor));
        ui->btn_search->setPalette(QPalette(darkColor));
        ui->btn_add_new_manga->setPalette(QPalette(darkColor));
        ui->tableWidget->setPalette(QPalette(darkColor));
        ui->label_28->setPalette(QPalette(darkColor));
        ui->tabWidget->setPalette(QPalette(darkColor));
        ui->lb_username->setPalette(QPalette(darkColor));
        ui->tableWidget3->setPalette(QPalette(darkColor));
        ui->tabWidget_2->setPalette(QPalette(darkColor));
        ui->qpb_genre_sort->setPalette(QPalette(darkColor));
        ui->btn_manga_add->setPalette(QPalette(darkColor));
        ui->btn_manga_editc->setPalette(QPalette(darkColor));
        ui->lb_manga_add_name->setPalette(QPalette(darkColor));
        ui->lb_manga_add_description->setPalette(QPalette(darkColor));
        ui->le_audio_path->setPalette(QPalette(darkColor));
        ui->btn_select_audio->setPalette(QPalette(darkColor));
        ui->tableWidget_2->setPalette(QPalette(darkColor));
        ui->qlb_anime->setPalette(QPalette(darkColor));
        ui->label_8->setPalette(QPalette(darkColor));
        }
    isDarkMode = !isDarkMode;
}

// загрузить информацию пользователя
void MainWindow::getUserInfo(QString ID)
{
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("select user.user_nickname, user.user_avatar from user where user.id = ?;");
        query.addBindValue(ID);
        query.exec();
        if(query.next())
        {
            ui->lb_username->setText(query.value(0).toString());
            ui->lb_profile_name->setText(query.value(0).toString());
            QByteArray imageData = query.value(1).toByteArray();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            ui->lb_profile_avatar->setPixmap(pixmap);
            ui->qlb_profile->setPixmap(pixmap);
        }
        query.prepare("SELECT manga.id, manga.manga_name, manga.manga_icon FROM manga "
                      "JOIN user_fav_manga ON manga.id = user_fav_manga.manga_id "
                      "WHERE user_fav_manga.user_id = ?;");
        query.addBindValue(ID);
        query.exec();
        ui->tableWidget1->clearContents();
        ui->tableWidget1->setRowCount(0);
        int j = 0;
        while (query.next())
        {
            if(ui->tableWidget1->rowCount() == 0)
            {
                ui->tableWidget1->insertRow(ui->tableWidget1->rowCount());
            }
            manga* Manga = new manga();
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            Manga->setMangaText(query.value(1).toString(), query.value(0).toString(), image);
            ui->tableWidget1->setItem(ui->tableWidget1->rowCount()-1, j, new QTableWidgetItem());
            ui->tableWidget1->setCellWidget(ui->tableWidget1->rowCount()-1, j, Manga);
            //connect(Manga, &manga::clicked, this, &MainWindow::loadManga);
            connect(Manga, &manga::clicked, this, [this](QString id) { loadManga(id, MangaSource::FromUserInfo); });
            if(j + 1 == 5)
            {
                j = 0;
                ui->tableWidget1->insertRow(ui->tableWidget1->rowCount());
            }
            else
            {
                j++;
            }
        }
    }
}

// установить аватар пользователя
void MainWindow::selectUserAvatar(QString ID)
{
    QStringList imageName;
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setViewMode(QFileDialog::Detail);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle("Выберите аватар игрока");
    fd.setNameFilter("Image files (*.png *.jpeg *.jpg)");
    if(fd.exec())
    {
        imageName = fd.selectedFiles();
    }
    if(!imageName.isEmpty())
    {
        QPixmap image;
        image.load(imageName.at(0));
        image.scaled(QSize(60,60), Qt::KeepAspectRatioByExpanding);
        db = QSqlDatabase::database("animanga");
        if(getDBConnection(db))
        {
            QByteArray arr;
            QBuffer buffer(&arr);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            QSqlQuery query(db);
            query.prepare("update user set user_avatar = ? where id = ?;");
            query.addBindValue(arr);
            query.addBindValue(ID);
            if(query.exec())
            {
                ui->lb_profile_avatar->setPixmap(image);
                ui->qlb_profile->setPixmap(image);
            }
        }
    }
}

// получить изображение из БД
QPixmap MainWindow::getImageFromDB(QByteArray arr)
{
    QPixmap image;
    image.loadFromData(arr);
    return image;
}

// добавить новую мангу
void MainWindow::addNewManga()
{
    QList<QString> ID = getSelectedIDGenre();
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QPixmap image = ui->lb_manga_add_icon->pixmap();
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите добавить новую мангу?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QSqlQuery query(db);
            query.prepare("INSERT INTO manga (manga_name, manga_icon, manga_description, manga_audio) VALUES (?, ?, ?, ?);");
            query.addBindValue(ui->lb_manga_add_name->text());
            query.addBindValue(arr);
            query.addBindValue(ui->lb_manga_add_description->toPlainText());
            query.addBindValue(ui->le_audio_path->text());
            if(query.exec())
            {
                QString currentIDManga;
                query.exec("SELECT max(id) FROM manga;");
                if(query.next())
                {
                    currentIDManga = query.value(0).toString();
                    for(QString id : ID)
                    {
                        query.prepare("INSERT INTO all_manga_genre (manga_id, genre_id) VALUES (?, ?);");
                        query.addBindValue(currentIDManga);
                        query.addBindValue(id);
                        query.exec();
                    }
                    showMessage("Манга успешно добавлена!", "", QMessageBox::Ok, QMessageBox::Information);
                    ui->lb_manga_add_name->clear();
                    ui->lb_manga_add_icon->clear();
                    ui->lb_manga_add_description->clear();
                    ui->le_audio_path->clear();
                    showMangaMenu();
                }
            }
        }
    }
}

// получить жанры манги
QList<QString> MainWindow::getSelectedIDGenre()
{
    QList<QString> founded;
    for(int i = 0; i < ui->tableWidget_2->rowCount(); i++)
    {
        if(qobject_cast<QCheckBox*>(ui->tableWidget_2->cellWidget(i, 0))->checkState() == Qt::Checked)
        {
            founded.append(ui->tableWidget_2->item(i, 1)->text());
        }
    }
    return founded;
}

// сформировать запрос фильтра по жанрам
QString MainWindow::generateGenreQuery(const QStringList &genres)
{
    QString queryStr = "SELECT DISTINCT manga.manga_name, manga.id, manga.manga_icon "
                       "FROM manga "
                       "JOIN all_manga_genre ON manga.id = all_manga_genre.manga_id "
                       "WHERE all_manga_genre.genre_id IN (";
    for (int i = 0; i < genres.size(); ++i)
    {
        queryStr += (i > 0 ? ", " : "") + genres[i];
    }
    queryStr += ")";
    return queryStr;
}

// получить отмеченные жанры
QStringList MainWindow::getSelectedGenres()
{
    QStringList selectedGenres;
    for (int row = 0; row < ui->tableWidget3->rowCount(); ++row)
    {
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(ui->tableWidget3->cellWidget(row, 0));
        if (checkbox && checkbox->isChecked())
        {
            QString genreId = ui->tableWidget3->item(row, 1)->text();
            selectedGenres.append(genreId);
        }
    }
    return selectedGenres;
}

// отфильтровать жанры манги
void MainWindow::filterMangaByGenre()
{
    QStringList selectedGenres = getSelectedGenres();
    if (selectedGenres.isEmpty())
    {
        clearRowsInTable(ui->tableWidget);
        mangatable();
        return;
    }
    QString queryStr = "SELECT manga.manga_name, manga.id, manga.manga_icon "
                       "FROM manga "
                       "JOIN all_manga_genre ON manga.id = all_manga_genre.manga_id "
                       "WHERE all_manga_genre.genre_id IN (";

    for (int i = 0; i < selectedGenres.size(); ++i)
    {
        queryStr += (i > 0 ? ", " : "") + selectedGenres[i];
    }
    queryStr += ") GROUP BY manga.id "
                "HAVING COUNT(DISTINCT all_manga_genre.genre_id) = " + QString::number(selectedGenres.size());
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare(queryStr);
        query.exec();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        int j = 0;
        while (query.next())
        {
            if (ui->tableWidget->rowCount() == 0)
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            manga* Manga = new manga();
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            Manga->setMangaText(query.value(0).toString(), query.value(1).toString(), image);
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, j, new QTableWidgetItem());
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1, j, Manga);
            connect(Manga, &manga::clicked, this, [this](QString id) { loadManga(id, MangaSource::FromGenreFilter); });

            if (j + 1 == 4)
            {
                j = 0;
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            else
            {
                j++;
            }
        }
    }
}

// удалить мангу
void MainWindow::deleteManga(QString ID)
{
    if(getDBConnection(db))
    {
        qDebug() << "Deleting manga with ID:" << ID;
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить мангу?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QSqlQuery query(db);
            query.prepare("DELETE FROM pages WHERE chapter_id IN (SELECT id FROM chapters WHERE manga_id = :id)");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM chapters WHERE manga_id = :id");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM user_fav_manga WHERE manga_id = :id");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM all_manga_genre WHERE manga_id = :id");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM ratings WHERE manga_id = :id");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM reviews WHERE manga_id = :id");
            query.bindValue(":id", ID);
            query.exec();
            query.prepare("DELETE FROM manga WHERE id = :id");
            query.bindValue(":id", ID);
            query.exec();

            if (query.exec())
            {
                showMessage("Манга успешно удалена!", "", QMessageBox::Ok, QMessageBox::Information);
                showMangaMenu();
            }
        }
    }
}

// установить иконку манги
void MainWindow::selectMangaIcon()
{
    QStringList imageName;
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setViewMode(QFileDialog::Detail);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle("Выберите иконку аниме");
    fd.setNameFilter("Image files (*.png *.jpeg *.jpg)");
    if(fd.exec())
    {
        imageName = fd.selectedFiles();
    }
    if(!imageName.isEmpty())
    {
        QPixmap image;
        image.load(imageName.at(0));
        ui->lb_manga_add_icon->setPixmap(image);
    }
}

// добавить рейтинг манги
void MainWindow::addRating()
{
    bool ok;
    double rating = ui->le_rating->text().toDouble(&ok);
    if (!ok || rating < 0 || rating > 10)
    {
        showMessage("Введённое значение некорректно!", "", QMessageBox::Ok, QMessageBox::Information);
        return;
    }
    if (getDBConnection(db))
    {
        QSqlQuery checkQuery(db);
        checkQuery.prepare("SELECT COUNT(*) FROM ratings WHERE manga_id = ? AND user_id = ?");
        checkQuery.addBindValue(currentMangaID);
        checkQuery.addBindValue(userID);
        checkQuery.exec();
        if (checkQuery.next() && checkQuery.value(0).toInt() > 0)
        {
            QSqlQuery updateQuery(db);
            updateQuery.prepare("UPDATE ratings SET rating = ? WHERE manga_id = ? AND user_id = ?");
            updateQuery.addBindValue(rating);
            updateQuery.addBindValue(currentMangaID);
            updateQuery.addBindValue(userID);
            updateQuery.exec();
        }
        else
        {
            QSqlQuery insertQuery(db);
            insertQuery.prepare("INSERT INTO ratings (manga_id, user_id, rating) VALUES (?, ?, ?)");
            insertQuery.addBindValue(currentMangaID);
            insertQuery.addBindValue(userID);
            insertQuery.addBindValue(rating);
            insertQuery.exec();
        }
        QSqlQuery avgQuery(db);
        avgQuery.prepare("SELECT AVG(rating) FROM ratings WHERE manga_id = ?");
        avgQuery.addBindValue(currentMangaID);
        avgQuery.exec();
        if (avgQuery.next())
        {
            double averageRating = avgQuery.value(0).toDouble();
            ui->lb_manga_rating->setText(QString::number(averageRating, 'f', 1));
        }
    }
}

// загрузить мангу
void MainWindow::loadManga(QString ID, MangaSource source)
{
    ui->tabWidget_2->setCurrentIndex(0);
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),0,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->manga_frame->setGeometry(ui->manga_frame->x(),1000,ui->manga_frame->width(),ui->manga_frame->height());
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),1000,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->profile_frame->setGeometry(ui->profile_frame->x(),1000,ui->profile_frame->width(),ui->profile_frame->height());
    ui->manga_frame->setEnabled(false);
    ui->current_manga_frame->setEnabled(true);
    ui->manga_add_frame->setEnabled(false);
    ui->profile_frame->setEnabled(false);
    if(hasAccess("user"))
    {
        ui->btn_add_new_manga->setVisible(false);

        if (source == MangaSource::FromMangaTable) {
            ui->btn_del_fav->setVisible(false);
            ui->btn_add_fav->setVisible(true);
            ui->btn_manga_edit->setVisible(false);
            ui->btn_manga_delete->setVisible(false);
            ui->qpb_add_chapter->setVisible(false);
        } else if (source == MangaSource::FromUserInfo) {
            ui->btn_del_fav->setVisible(true);
            ui->btn_add_fav->setVisible(false);
            ui->btn_manga_edit->setVisible(false);
            ui->btn_manga_delete->setVisible(false);
            ui->qpb_add_chapter->setVisible(false);
        }
    }
    else
    {
        ui->btn_add_new_manga->setVisible(true);

        if (source == MangaSource::FromMangaTable) {
            ui->btn_del_fav->setVisible(false);
            ui->btn_add_fav->setVisible(true);
            ui->btn_manga_edit->setVisible(true);
            ui->btn_manga_delete->setVisible(true);
            ui->qpb_add_chapter->setVisible(true);
        } else if (source == MangaSource::FromUserInfo) {
            ui->btn_del_fav->setVisible(true);
            ui->btn_add_fav->setVisible(false);
            ui->btn_manga_edit->setVisible(false);
            ui->btn_manga_delete->setVisible(false);
            ui->qpb_add_chapter->setVisible(false);
        }
    }

    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT id, manga_name, manga_icon, manga_description, manga_audio FROM manga WHERE id = ?;");
        query.addBindValue(ID);
        query.exec();
        if(query.next())
        {
            currentMangaID = query.value(0).toString();
            ui->lb_current_manga_name->setText(query.value(1).toString());
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            ui->lb_current_manga_icon->setPixmap(image);
            ui->lb_current_manga_description->setPlainText(query.value(3).toString());
            QString audioPath = query.value(4).toString();
            MPlayer->setSource(QUrl::fromLocalFile(audioPath));
        }
        ui->le_rating->clear();
        QSqlQuery avgQuery(db);
        avgQuery.prepare("SELECT AVG(rating) FROM ratings WHERE manga_id = ?");
        avgQuery.addBindValue(currentMangaID);
        avgQuery.exec();
        if (avgQuery.next())
        {
            double averageRating = avgQuery.value(0).toDouble();
            ui->lb_manga_rating->setText(QString::number(averageRating, 'f', 1));
        }
        else
        {
            ui->lb_manga_rating->setText("Оценок нет");
        }
        query.prepare("select genre_value from manga_genre inner join all_manga_genre on all_manga_genre.genre_id = manga_genre.id where all_manga_genre.manga_id = ?;");
        query.addBindValue(currentMangaID);
        query.exec();
        QString roles;
        while(query.next())
        {
            roles += query.value(0).toString() + ", ";
        }
        if(roles.length() > 2)
        {
            roles.removeAt(roles.length()-2);
            ui->lb_manga_genre->setText(roles);
        }
    }
    reviewtable(ID);
    chaptertable(ID);
}

// таблица манги
void MainWindow::mangatable()
{
    manga* Manga;
    int j = 0;
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT manga_name, id, manga_icon FROM manga");
        query.exec();
        while (query.next())
        {
            if(ui->tableWidget->rowCount() == 0)
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            Manga = new manga();
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            Manga->setMangaText(query.value(0).toString(), query.value(1).toString(), image);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, j, new QTableWidgetItem());
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, j, Manga);
            //connect(Manga, &manga::clicked, this, &MainWindow::loadManga);
            connect(Manga, &manga::clicked, this, [this](QString id) { loadManga(id, MangaSource::FromMangaTable); });
            if(j + 1 == 4)
            {
                j = 0;
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            else
            {
                j++;
            }
        }
    }
}

// отчистить таблицу манги
void MainWindow::clearRowsInTable(QTableWidget *table)
{
    int rowCount = table->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        table->removeRow(0);
    }
}

// поиск
void MainWindow::searchManga()
{
    QString searchText = ui->lineEdit->text().trimmed();
    int j = 0;
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT manga_name, id, manga_icon FROM manga WHERE manga_name LIKE ?");
        query.addBindValue("%" + searchText + "%");
        query.exec();
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        while (query.next())
        {
            if(ui->tableWidget->rowCount() == 0)
            {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            manga* Manga = new manga();
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            Manga->setMangaText(query.value(0).toString(), query.value(1).toString(), image);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, j, new QTableWidgetItem());
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, j, Manga);
            connect(Manga, &manga::clicked, this, [this](QString id) { loadManga(id, MangaSource::FromSearchManga); });
            if(j + 1 == 4)
            {
                j = 0;
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }
            else
            {
                j++;
            }
        }
    }
}

// показать редактирование манги
void MainWindow::showEditManga(QString ID)
{
    ui->btn_manga_add->setVisible(false);
    ui->btn_manga_editc->setVisible(true);
    ui->manga_add_frame->setGeometry(ui->manga_add_frame->x(),0,ui->manga_add_frame->width(),ui->manga_add_frame->height());
    ui->current_manga_frame->setGeometry(ui->current_manga_frame->x(),1000,ui->current_manga_frame->width(),ui->current_manga_frame->height());
    ui->manga_add_frame->setEnabled(true);
    ui->current_manga_frame->setEnabled(false);
    clearRowsInTable(ui->tableWidget);
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT id, manga_name, manga_icon, manga_description, manga_audio FROM manga WHERE id = ?;");
        query.addBindValue(ID);
        query.exec();
        if(query.next())
        {
            ui->lb_manga_add_name->setText(query.value(1).toString());
            QPixmap image;
            image.loadFromData(query.value(2).toByteArray());
            ui->lb_manga_add_icon->setPixmap(image);
            ui->lb_manga_add_description->setPlainText(query.value(3).toString());
            ui->le_audio_path->setText(query.value(4).toString());
        }
    }
}

// изменить мангу
void MainWindow::editManga(QString ID)
{
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QPixmap image = ui->lb_manga_add_icon->pixmap();
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");
        buffer.close();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите обновить мангу?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            QSqlQuery query(db);
            query.prepare("UPDATE manga SET manga_name = ?, manga_icon = ?, manga_description = ?, manga_audio = ? WHERE id = ?;");
            query.addBindValue(ui->lb_manga_add_name->text());
            query.addBindValue(arr);
            query.addBindValue(ui->lb_manga_add_description->toPlainText());
            query.addBindValue(ui->le_audio_path->text());
            query.addBindValue(ID);
            if(query.exec())
            {
                if(query.numRowsAffected() > 0)
                {
                    showMessage("Манга успешно обновлена!", "", QMessageBox::Ok, QMessageBox::Information);
                    ui->lb_manga_add_name->clear();
                    ui->lb_manga_add_icon->clear();
                    ui->lb_manga_add_description->clear();
                    loadManga(ID, MangaSource::FromMangaTable);
                }
            }
        }
    }
}

// выбрать аудио файл
void MainWindow::selectAudioFile()
{
    QString audioPath = QFileDialog::getOpenFileName(this, tr("Select Audio File"), "", tr("Audio Files (*.mp3 *.wav)"));
    if(!audioPath.isEmpty())
    {
        ui->le_audio_path->setText(audioPath);
    }
}

// добавить в избранное
void MainWindow::addToFavorites(QString ID)
{
    QString currentUserID = userID;
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) FROM user_fav_manga WHERE user_id = ? AND manga_id = ?");
        query.addBindValue(currentUserID);
        query.addBindValue(ID);
        if (!query.exec() || !query.next())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось проверить наличие манги в избранных");
            query.clear();
            return;
        }
        if (query.value(0).toInt() > 0)
        {
            QMessageBox::information(this, "Информация", "Манга уже добавлена в избранные");
            query.clear();
            return;
        }
        query.prepare("INSERT INTO user_fav_manga (user_id, manga_id) VALUES (?, ?);");
        query.addBindValue(userID);
        query.addBindValue(ID);
        if (query.exec())
        {
            QMessageBox::information(this, "Успех", "Манга добавлена в избранные");
            getUserInfo(currentUserID);
        }
        query.clear();
        currentMangaID.clear();
    }
}

// удалить мангу из списка избранных
void MainWindow::delFromFavorites(QString ID)
{
    if(getDBConnection(db))
    {
        qDebug() << "Deleting manga from favorites with ID:" << ID;
        QSqlQuery query(db);
        query.prepare("DELETE FROM user_fav_manga WHERE manga_id = :id");
        query.bindValue(":id", ID);
        query.exec();
        if (query.exec())
        {
            showMessage("Манга успешно удалена из избранных!", "", QMessageBox::Ok, QMessageBox::Information);
            showMangaMenu();
        }
    }
}

// добавить новый отзыв
void MainWindow::showNewReview()
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM reviews WHERE manga_id = :manga_id AND user_id = :user_id");
    query.bindValue(":manga_id", currentMangaID);
    query.bindValue(":user_id", userID);
    query.exec();
    if(query.next()) {
        int reviewCount = query.value(0).toInt();
        if(reviewCount > 0) {
            QMessageBox::warning(this, "Предупреждение", "Вы уже оставили отзыв для этой манги.");
            return;
        }
    }

    addnewreview *rvw = new addnewreview;
    QString ID = currentMangaID;
    rvw->setUserID(userID);
    rvw->setMangaID(currentMangaID);
    rvw->show();

    connect(rvw, &addnewreview::reviewAdded, [=]()
            {
                rvw->hide();
                QList<QMessageBox::Button> buttons = {QMessageBox::Ok};
                showMessage("Отзыв успешно оставлен!", "", QMessageBox::Information, buttons);
                //loadManga(ID);
                //loadManga(ID, MangaSource::FromMangaTable);
                reviewtable(ID);
            });

    connect(rvw, &addnewreview::cancelAdding, [=]()
            {
                rvw->hide();
            });
}

// таблица отзывов
void MainWindow::reviewtable(QString ID)
{
    clearRowsInTable(ui->tableWidget_3);
    review* Review;
    int j = 0;
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT user.user_avatar, user.user_nickname, reviews.review_text "
                      "FROM reviews "
                      "INNER JOIN user ON reviews.user_id = user.id "
                      "WHERE reviews.manga_id = ?");
        query.addBindValue(ID);
        query.exec();

        while (query.next())
        {
            if (ui->tableWidget_3->rowCount() == 0)
            {
                ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
            }

            Review = new review();
            QPixmap image;
            image.loadFromData(query.value(0).toByteArray());
            Review->loadReview(query.value(1).toString(), query.value(2).toString(), image);
            ui->tableWidget_3->setItem(ui->tableWidget_3->rowCount() - 1, j, new QTableWidgetItem());
            ui->tableWidget_3->setCellWidget(ui->tableWidget_3->rowCount() - 1, j, Review);

            if (j + 1 == 1)
            {
                j = 0;
                ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
            }
            else
            {
                j++;
            }
        }
    }
}

void MainWindow::delReview()
{
    QSqlQuery query(db);
    query.prepare("SELECT id FROM reviews WHERE manga_id = :manga_id AND user_id = :user_id");
    query.bindValue(":manga_id", currentMangaID);
    query.bindValue(":user_id", userID);
    query.exec();
    if(query.next()) {
        int reviewID = query.value(0).toInt();
        QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, "Удаление отзыва", "Вы уверены, что хотите удалить свой отзыв?", QMessageBox::Yes | QMessageBox::No);
        if(confirmDelete == QMessageBox::Yes) {
            query.prepare("DELETE FROM reviews WHERE id = :review_id");
            query.bindValue(":review_id", reviewID);
            if(query.exec()) {
                QMessageBox::information(this, "Успех", "Отзыв успешно удален.");
                reviewtable(currentMangaID);
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось удалить отзыв.");
            }
        }
    } else {
        QMessageBox::warning(this, "Предупреждение", "У вас нет отзыва для этой манги.");
    }
}

// добавить новую главу
void MainWindow::showNewChapter()
{
    int mangaID = currentMangaID.toInt();
    addnewchapter *chapter = new addnewchapter(mangaID);
    chapter->show();

    connect(chapter, &addnewchapter::chapterAdded, [=]()
            {
                chapter->hide();
                QList<QMessageBox::Button> buttons = {QMessageBox::Ok};
                showMessage("Глава успешно добавлена!", "", QMessageBox::Information, buttons);
                // loadChapterData(currentChapterID); // Если необходимо
                chaptertable(currentMangaID);
            });

    connect(chapter, &addnewchapter::cancelAdding, [=]()
            {
                chapter->hide();
            });
}

// таблица глав
void MainWindow::chaptertable(QString ID)
{
    db = QSqlDatabase::database("animanga");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        ui->tableWidget_4->clearContents();
        ui->tableWidget_4->setRowCount(0);
        query.prepare("SELECT id, number, name FROM chapters WHERE manga_id = ?");
        query.addBindValue(ID);
        query.exec();
        int row = 0;
        while(query.next())
        {
            QString chapterID = query.value(0).toString();
            QString chapterNumber = query.value(1).toString();
            QString chapterName = query.value(2).toString();
            ui->tableWidget_4->insertRow(row);
            QTableWidgetItem *chapterIDItem = new QTableWidgetItem(chapterID);
            QTableWidgetItem *chapterItem = new QTableWidgetItem("Глава");
            QTableWidgetItem *numberItem = new QTableWidgetItem(chapterNumber);
            QTableWidgetItem *nameItem = new QTableWidgetItem(chapterName);
            ui->tableWidget_4->setItem(row, 0, chapterIDItem);
            ui->tableWidget_4->setItem(row, 1, chapterItem);
            ui->tableWidget_4->setItem(row, 2, numberItem);
            ui->tableWidget_4->setItem(row, 3, nameItem);
            ++row;
        }
        ui->tableWidget_4->hideColumn(0);
    }
}

// клик на таблицу
void MainWindow::TableWidgetRowClicked(int row, int column)
{

    QTableWidgetItem *item = ui->tableWidget_4->item(row, 0);
    if (item)
    {
        int chapterID = item->text().toInt();
        loadChapter(chapterID);
    }
}

// загрузить главу
void MainWindow::loadChapter(int chapterID)
{
    chapter *chp = new chapter(chapterID);
    chp->show();
}
