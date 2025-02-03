#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QProcess>
#include <QSqlQuery>
#include <QDate>
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QBuffer>
#include <QListWidget>
#include <QTableWidget>
#include <QtMultimedia>
#include <QtCore>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum class MangaSource {
        FromMangaTable,
        FromUserInfo,
        FromSearchManga,
        FromGenreFilter
    };

public slots:
    void showAutoMenu();
    void showMainMenu();
    void showProfileMenu();
    void showMangaMenu();
    void showAuthorization();
    void tryAuthorization();
    void selectUserAvatar(QString);
    void addNewManga();
    void selectMangaIcon();
    void showMangaAddMenu();
    void loadManga(QString ID, MangaSource source);
    void deleteUser();
    void deleteManga(QString);
    void showEditManga(QString);
    void editManga(QString);
    void selectAudioFile();
    void adminFunction();
    void addRating();
    void showNewReview();
    void TableWidgetRowClicked(int row, int column);
    void delReview();
    void showAnimeMenu();

private slots:
    void toggleColor();
    void searchManga();
    void audioPlay();
    void audioStop();
    void addToFavorites(QString);
    void delFromFavorites(QString);
    void showNewChapter();

private:
    Ui::MainWindow *ui;
    QString currentMangaID;
    QProcess *test;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "animanga");
    QString userID,
        m_currentFrame,
        currentIDPlayer;
    QString userRole;
    void mangatable();
    QColor lightColor;
    QColor darkColor;
    QColor lightTextColor;
    QColor darkTextColor;
    bool isDarkMode;
    QMediaPlayer *MPlayer;
    QAudioOutput *audioOutput;
    QIODevice *audioOutputIODevice;
    bool hasAccess(const QString &requiredRole);
    void getUserInfo(QString);
    QPixmap getImageFromDB(QByteArray);
    void clearRowsInTable(QTableWidget*);
    void reviewtable(QString ID);
    void chaptertable(QString ID);
    void loadChapter(int ID);

private:
    QList<QString> getSelectedIDGenre();
    QList<QString> getSelectedGenres();
    QString generateGenreQuery(const QStringList &genres);
    void filterMangaByGenre();

};
#endif // MAINWINDOW_H
