#include "parser.h"

#include <regex>
#include <fstream>

parser::parser(std::string path) // запись в вектор и удаление сторок до рекордс и после end
{
    std::ifstream data_desc_lang;
    data_desc_lang.open(path);
    std::string str;

    while(std::getline(data_desc_lang,str))
    {
        ex.push_back(str);
        str = "";
    }

    for(std::deque<std::string>::iterator i = ex.begin();*i!="RECORDS;";i++)
        ex.pop_front();

    std::regex end("(\\s+)(\\bEND;)(\\s+)");
    std::string temp;

    for(int i = ex.size()-1;i!=0;i--)
    {
        temp = ex[i];
        if(std::regex_match(temp, end))
            break;
        else
            ex.pop_back();
    }
    data_desc_lang.close();

}

bool parser :: rec(std::string str) // предикаст
{
    std::regex rb("(\\s+)(\\bRBODY)(.+)");
    if(std::regex_match(str,rb))
        return true;
    else
        return false;
}

std::string parser :: data_desc_lang_edit(std::string str) // убирает все пробелы, скобки, запятые
{
    for(auto i = 0; i < str.length(); i++)
    {
        if(str[i] == ',')
        {
            if(str.find_first_of(')',str.find_first_of(','))-str.find_first_of(',')==2)
            {
                str.erase(i,1);
                i--;
            }
            else
                str.erase(i,str.find(')')-i);
        }
        if(str[i] == '(' || str[i] == ')')
        {
            if(str.find("GRK")!=std::string::npos)
                continue;
            else
                str.erase(i,1);
            i--;
        }
        if(str[i] == ' '|| str[i] == ',')
        {
            str.erase(i,1);
            i--;
        }
        else if(str[i] == ';')
        {
            str.erase(i,str.size()-i);
        }
    }
    return str;
}

Elem* parser :: parse_str(std::string line) // с помощью регулярки парсится обычный элемент
{
    line = data_desc_lang_edit(line);

    std::cmatch result;

    std::regex MIT("(\\bMIT)(.+)(\\bA|B)([0-9]+)(PC||PA)([0-9]+)");
    std::regex MIT2("(\\bMIT)(.+)(\\bA|B)([0-9]+)(.+)");
    std::regex LNG("(\\bLNG)(.+)([A-B])([0-9]+)(PC||PA)([0-9]+)");
    std::regex CHA("(\\bCHA)(.+)(Q|ПРЮ)([A-B])([0-9]+)(PC)([0-9]+)");
    std::regex IND("(\\bIND)([0-9]+)(.+)(PC)([0-9]+)");
    std::regex KEY("(\\bKEY)(I|L)([А-Я]+)([A-B])([0-9]+)(PC)([0-9]+)");
    std::regex MRC("(\\bMRC)(I)([А-Я]+)([A-B])([0-9]+)(PC)([0-9]+)");
    std::regex CNT("(\\bCNT)(.+)([B])([0-9]+)(PC)([0-9]+)");

    if(regex_match(line.c_str(),result,MIT))
    {
        SimElem* elem = new class MIT(result[1],result[2],result[3],stoi(result[4]));
        return elem;
    }
    else if(regex_match(line.c_str(),result,MIT2))
    {
        SimElem* elem = new class MIT(result[1],result[2],result[3],stoi(result[4]));
        return elem;
    }
    else if(regex_match(line.c_str(),result,CHA))
    {
        SimElem* elem = new class CHA(result[1],result[2],result[4],stoi(result[5]));
        return elem;
    }
    else if(regex_match(line.c_str(),result,KEY))
    {
        SimElem* elem = new class KEY(result[1],result[3],result[4],stoi(result[5]),result[2]);
        return elem;
    }
    else if(regex_match(line.c_str(),result,MRC))
    {
        SimElem* elem = new class MRC(result[1],result[3],result[4],stoi(result[5]),result[2]);
        return elem;
    }
    else if(regex_match(line.c_str(),result,CNT))
    {
        SimElem* elem = new class CNT(result[1],result[2],result[3],stoi(result[4]));
        return elem;
    }
    else if(regex_match(line.c_str(),result,LNG))
    {
        SimElem* elem = new class LNG(result[1],result[2],result[3],stoi(result[4]));
        return elem;
    }
    else
    {
        std::cout << "ОШИБКА ПАРСИНГА\n";
        return nullptr;
    }
}

void parser :: get_records() // считывает рекодрс
{
    std::deque<std::string>::iterator j = std::find_if(ex.begin(),ex.end(), rec);
    for(std::deque<std::string>::iterator i = std::find(ex.begin(),ex.end(),"RECORDS;")+1;i<j;i++)
    {
        Elem* eda = parse_str(*i);
        records.push_back(eda);
        ex.pop_front();
    }
    ex.pop_front();
}

Elem* parser :: form_group(std::vector<std::string>::iterator i, std::string group_name, std::string group_type) // рекурсивно парсит группы
{
    std::regex GRP("(\\s+)(\\bGRP)(\\s)(.+)(;)(\\s)(.+)");
    std::regex GRK("(\\s+)(\\bGRK)(\\()(.+)(\\))(\\s)(.+)(;)(.+)");
    if(group_type=="GRK")
    {
        std::vector<Elem*> group_els;
        std::cmatch result;
        std::string grk_cnt;
        std::string temp="";
        temp = *i;

        if(std::regex_match(temp.c_str(),result,GRK))
            grk_cnt = result[4];
        else
            std::cout << "CNT ERROR";

        group_name = data_desc_lang_edit(group_name);
        std::regex group_end("(\\s+)(END)(\\s)"+group_name+"(.+)");

        for(i++;!std::regex_match(*i,group_end);i++)
        {
            temp = *i;
            if(std::regex_match(temp.c_str(),result,GRP))
            {
                Elem* gr =  form_group(i, result[4], result[2]);
                group_els.push_back(gr);
                i = gr_end;
            }
            else if(std::regex_match(temp.c_str(),result,GRK))
            {
                Elem* gr =  form_group(i, result[7], result[2]);
                group_els.push_back(gr);
                i = gr_end;
            }
            else
            {
                Elem* sel =  parse_str(temp);
                group_els.push_back(sel);
            }
        }
        gr_end = i;

        Elem* group = new  class GRK(group_type,group_name,grk_cnt,group_els);
        return group;
    }
    else if(group_type=="GRP")
    {
        std::vector<Elem*> group_els;

        std::regex IND("(\\s+)(IND)(\\()(.+)(\\))(\\s+)(.+)(\\s+)(PC)(.+)");
        std::vector<int> ind;
        std::vector<std::string> desc;
        std::cmatch result;
        std::string temp ="";
        temp = *i;

        std::regex group_end("(\\s+)(END)(\\s)"+group_name+"(.+)");

        for(i++;!std::regex_match(*i,group_end);i++)
        {
            temp = *i;
            if(std::regex_match(temp.c_str(),result,GRP))
            {
                group_els.push_back(form_group(i, result[4], result[2]));
                i = gr_end;
            }
            else if(std::regex_match(temp.c_str(),result,IND))
            {
                ind.push_back(stoi(result[4]));
                desc.push_back(data_desc_lang_edit(result[7]));
            }
            else if(std::regex_match(temp.c_str(),result,GRK))
            {
                group_els.push_back(form_group(i, result[7], result[2]));
                i = gr_end;
            }
            else
            {
                group_els.push_back(parse_str(temp));
            }
        }
        gr_end = i;
        if(ind.size()==1)
        {
            Elem* group = new  class GRP(group_type,group_name,group_els,ind[0],desc[0]);
            return group;
        }
        else
        {
            Elem* group = new  class GRP(group_type,group_name,group_els,ind[0],desc[0],ind[1],desc[1]);
            return group;
        }


    }
    else
    {
        std::cout << "Group type error" << std::endl;
        return nullptr;
    }
}

void parser :: get_body() // формирует вектор строк боди, чтобы передать считывающей функции
{
    std::deque<std::string>::iterator i = std::find_if(ex.begin(),ex.end(), rec);
    std::string rb="";
    rb = *i;

    std::string name;
    int number = 0;

    std::cmatch result;
    std::regex Rbody("(\\s+)(\\bRBODY)(\\()(\\d)(\\))(\\s)(.+)(;)(.+)");


    if(std::regex_match(rb.c_str(),result,Rbody))
    {
        number = stoi(result[4]);
        name = result[7];
    }
    else
        std::cout << "Error with RBODY";

    std::regex end("(\\s+)(END)(\\s)"+data_desc_lang_edit(name)+"(;)(.+)");

    std::deque<std::string> rbody_str;
    for(std::deque<std::string>::iterator j=i++;j!=rbody_str.end();j++)
    {
        std::string temp="";
        temp = *j;
        if(std::regex_match(temp.c_str(),end))
            break;
        else
        {
            rbody_str.push_back(temp);
            ex.pop_front();
        }
    }
    ex.pop_front();

    rbody foo(number,name,read_body(rbody_str));
    ddl.push_back(foo);

    if(ex.size()!=1)
        get_body();
}

std::vector<Elem*> parser :: read_body(std::deque<std::string> ex) // та самая считывающая, парсит элементы и записывает в вектор
{
    std::vector<std::string> example;

    for(int i =1;i!=ex.size();i++)
        example.push_back(ex[i]);

    std::vector<Elem*> temporary;
    std::cmatch result;
    std::regex GRP("(\\s+)(\\bGRP)(\\s)(.+)(;)(\\s)(.+)");
    std::regex GRK("(\\s+)(\\bGRK)(\\()(.+)(\\))(\\s)(.+)(;)(.+)");

    for(std::vector<std::string>::iterator i = example.begin()++;i!=example.end();i++)
    {
        std::string tempo = "";
        tempo = *i;
        if(std::regex_match(tempo.c_str(),result,GRP))
        {
            Elem* gr =  form_group(i, result[4], result[2]);
            temporary.push_back(gr);
            i = gr_end++;
        }
        else if(std::regex_match(tempo.c_str(),result,GRK))
        {
            Elem* gr =  form_group(i, result[7], result[2]);
            temporary.push_back(gr);
            i = gr_end++;
        }
        else
        {
            Elem* sel =  parse_str(tempo);
            temporary.push_back(sel);
        }
    }
    return temporary;
}

/*возвращает элементы для ридера*/

std::vector<Elem*> parser :: return_records()
{
    return  this->records;
}

std::vector<Elem*> parser :: return_body(int mrc)
{
    return this->ddl[mrc-1].elements;
}



