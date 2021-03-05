#ifndef PARSER_H
#define PARSER_H

#include <elements.h>
#include <iostream>
#include <vector>
#include <deque>


class parser // ���� ����� ������ ��������� ddl � ���������� � ������ ��������
{
private:
    std::vector<Elem*> records; // ��� �������
    struct rbody // ����� �� ������, ������ � ������� � ���������
    {
        rbody(int _num, std::string _name, std::vector<Elem*> _elem) : num(_num), name(_name), elements(_elem) {}

        int num;
        std::string name;
        std::vector<Elem*> elements;
    };

    std::vector<rbody> ddl;  // ��������� ��������� ������� ddl
    std::deque<std::string> ex; // ������ � ������� �������������� ddl
    std::vector<std::string>::iterator gr_end; // ��������, ����� ������������ �� ������ ����� � ������ body

    bool static rec(std::string str); // ���������, ����� ������ ������ ������
    bool static end(std::string str);

    std::string data_desc_lang_edit(std::string str); // ������� �������� � �������� ����� ��� ���������

    /*������� ������*/
    Elem* parse_str(std::string line);
    Elem* form_group(std::vector<std::string>::iterator i, std::string group_name, std::string group_type);

    std::vector<Elem*> read_body(std::deque<std::string> example);

public:
    parser(std::string path);

    void get_records(); // ���� ��� ������
    void  get_body();

    /*��� ������*/
    std::vector<Elem*> return_records();
    std::vector<Elem*> return_body(int mrc);
    std::string  return_body_name(int mrc);
};

#endif // PARSER_H
