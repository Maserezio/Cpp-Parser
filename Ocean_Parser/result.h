#ifndef RESULT_H
#define RESULT_H

#include <QWidget>

namespace Ui {
class result; // окно сохранения в файл
}

class result : public QWidget
{
    Q_OBJECT

public:
    explicit result(QWidget *parent = nullptr, std::vector<std::string> _parsed = {"ERROR", "\n"});
    ~result();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::result *ui;
    QString save_file; // путь файла
    std::vector<std::string> parsed; // готовый вектор
};

#endif // RESULT_H
