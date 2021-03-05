#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <parser.h>
#include <reader.h>
#include <result.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


private:
    Ui::MainWindow *ui;

protected:
    QString ddlFilename; //  пути к файлам
    QString binFilename;

    parser* ddl;
    reader* bin;
    result* res; // окно для сохранения файла
};
#endif // MAINWINDOW_H
