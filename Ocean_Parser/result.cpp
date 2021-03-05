#include "result.h"
#include "ui_result.h"
#include "QFileDialog"
#include "fstream"

result::result(QWidget *parent, std::vector<std::string> _parsed) :
    QWidget(parent),
    ui(new Ui::result)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    parsed = _parsed;
}

result::~result()
{
    delete ui;

}

void result::on_pushButton_clicked() // выбор пути
{
    save_file = QFileDialog::getSaveFileName();
    ui->textEdit->setText(save_file);
    ui->pushButton_2->setEnabled(true);
}

void result::on_pushButton_2_clicked() // запись
{
  std::ofstream file;
  file.open(save_file.toStdString());
  for(int i =0;i<parsed.size();i++)
      file << parsed[i] << "\n";

  close();
}
