#ifndef PARSER_H
#define PARSER_H

#include <elements.h>
#include <iostream>
#include <vector>
#include <deque>


class parser // этот класс просто считывает ddl и записывает в вектор векторов
{
private:
    std::vector<Elem*> records; // так удобнее
    struct rbody // такой же вектор, только с номером и названием
    {
        rbody(int _num, std::string _name, std::vector<Elem*> _elem) : num(_num), name(_name), elements(_elem) {}

        int num;
        std::string name;
        std::vector<Elem*> elements;
    };

    std::vector<rbody> ddl;  // финальный результат разбора ddl
    std::deque<std::string> ex; // вектор в который переписывается ddl
    std::vector<std::string>::iterator gr_end; // итератор, чтобы возвращаться от чтения групп к чтению body

    bool static rec(std::string str); // предикаты, чтобы убрать лишние строки
    bool static end(std::string str);

    std::string data_desc_lang_edit(std::string str); // убирает проблемы и ненужные знаки для регулярки

    /*функции чтения*/
    Elem* parse_str(std::string line);
    Elem* form_group(std::vector<std::string>::iterator i, std::string group_name, std::string group_type);

    std::vector<Elem*> read_body(std::deque<std::string> example);

public:
    parser(std::string path);

    void get_records(); // тоже для чтения
    void  get_body();

    /*для ридера*/
    std::vector<Elem*> return_records();
    std::vector<Elem*> return_body(int mrc);
    std::string  return_body_name(int mrc);
};

#endif // PARSER_H
