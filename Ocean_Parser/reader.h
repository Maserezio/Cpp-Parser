#ifndef READER_H
#define READER_H

#include<iostream>
#include<fstream>
#include <vector>

#include<parser.h>

class reader // считывает бинарник и переводит его
{
private:
    std :: deque<uint8_t> binfile;  // сам бинарник

    struct desc // рекордс
    {
        int lng;
        int nulls;
        int station;
        int year;
        int month;
        int rec_type;
    };
    desc records;

    struct grk_cnt // счётчик grk
    {
        std::string description;
        int quan;
    };
    grk_cnt counter;

    std::vector<std::string> parsed_file;// готовый файл, который запишется в файл
public:
    reader(std::string bfile);

    /*функции чтения*/
    void read_bin(parser ddl);
    void read_records(parser ddl);
    void push_records();
    void read_body(parser ddl);
    void read_elem(Elem* ex, std::vector<uint8_t> temp, int j);
    void read_group(Elem* ex, std::vector<uint8_t> temp, int numb);
    void read_group(Elem* ex, std::vector<uint8_t> temp, int numb, int quan);

    /*функции перевода*/
    int paste_together (uint8_t a, uint8_t b);
    int paste_together (uint8_t a, uint8_t b, uint8_t c);
    int paste_together (uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    std::string koi_to_ascii(std::vector<int> dkoi);

    /*возвращение вектора и запись в файл*/
    std::vector<std::string> get_parsed();
    void save_to_file();

};

#endif // READER_H
