#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFileDialog"
#include "QString"
#include "iostream"
#include "parser.h"
#include "reader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(955,255);
    ui->textEdit_2->setReadOnly(true); // ����� ��������������� �������� ������
    ui->textEdit->setReadOnly(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ddlFilename = QFileDialog::getOpenFileName(this, "Choose DDL", "D://");// ����� ����� ddl
    ui->textEdit_2->setText(ddlFilename);  // ���������������� ������� �������
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    ddl = new parser(ddlFilename.toStdString()); // ������ ddl
    ddl->get_records();
    ddl->get_body();
    ui->pushButton_2->setEnabled(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    binFilename = QFileDialog::getOpenFileName(this, "Choose DDL", "D://"); // ����� ���������
    ui->textEdit->setText(binFilename);
    ui->pushButton_4->setEnabled(true);
}

void MainWindow::on_pushButton_4_clicked()
{
    bin = new reader(binFilename.toStdString()); // ������ ��������� � ������
    ui->pushButton_5->setEnabled(true);
}

void MainWindow::on_pushButton_5_clicked()
{
    bin->read_bin(*ddl); // ������ ������ �������� � ���������� � ����
    res  = new result(nullptr, bin->get_parsed());
    res->show();
}
