#include "reader.h"

reader :: reader(std::string bfile) // записывает бинарник в вектор uint8
{
    std :: ifstream bdata;

    bdata.open(bfile, std::ios::binary);

    while (!bdata.eof())
    {
        char ch;
        bdata.get(ch);
        binfile.push_back(int(ch));
    }

    bdata.close();
}

void reader :: read_records(parser ddl) // парсит рекордсы
{
    std::vector<Elem*> rec = ddl.return_records();

    int rec_size = 0;
    for(int i=0;i!=rec.size();i++)
    {
        rec_size+=rec[i]->return_size();
    }

    std::vector<uint8_t> temp;

    for(int i=0;i<rec_size;i++)
    {
        temp.push_back(binfile[i]);
    }

    for(int i=0;i<rec_size;i++)
    {
        binfile.pop_front();
    }

    int i=0;
    unsigned int j=0;
    std :: vector <int> buff;

    while(i<rec.size())
    {
        if(rec[i]->return_size() == 1)
        {
            buff.push_back(int(temp[j]));
        }
        else if(rec[i]->return_size() == 2)
        {
            int res = paste_together(temp[j], temp[j+1]);
            buff.push_back(res);
        }
        else if(rec[i]->return_size() == 3)
        {
            int res = paste_together(temp[j], temp[j+1], temp[j+2]);
            buff.push_back(res);
        }
        else
            continue;
        j+=rec[i]->return_size();
        i++;
    }

    records.lng = buff[0];
    records.nulls = buff[1];
    records.station = buff[2];
    records.year = buff[3];
    records.month = buff[4];
    records.rec_type = buff[5];

}

void reader :: read_bin (parser ddl) // для считывания всего бинарника
{
    while(binfile.size()>11)
    {
        read_records(ddl);
        push_records();
        read_body(ddl);
    }
}

std::vector<std::string> reader :: get_parsed() // возвращает готовый вектор
{
    return parsed_file;
}

void reader :: push_records() // записывает рекордс в готовый вектор
{
    std::string lng;
    lng = "ДЛИНА ЗАПИСИ - ";
    lng+=std::to_string(records.lng);
    parsed_file.push_back(" ");
    parsed_file.push_back(lng);

    lng = "НУЛИ - ";
    lng+=std::to_string(records.nulls);
    parsed_file.push_back(lng);

    lng = "СТАНЦИЯ - ";
    lng+=std::to_string(records.station);
    parsed_file.push_back(lng);

    lng = "ГОД - ";
    lng+=std::to_string(records.year);
    parsed_file.push_back(lng);

    lng = "МЕСЯЦ - ";
    lng+=std::to_string(records.month);
    parsed_file.push_back(lng);

    lng = "ТИП ЗАПИСИ - ";
    lng+=std::to_string(records.rec_type);
    parsed_file.push_back(lng);
    parsed_file.push_back(" ");

}

void reader :: read_body(parser ddl) // считывает боди в бинарнике
{
    std::vector<Elem*> body = ddl.return_body(records.rec_type);

    int body_size = records.lng-11;
    std::vector<uint8_t> temp;

    for(int i=0;i<body_size;i++)
        temp.push_back(binfile[i]);

    for(int i=0;i!=body_size;i++)
        binfile.pop_front();

    int i=0;
    int j=0;
    while(i!=body.size())
    {
        if(body[i]->return_type() == "GRP")
        {
            read_group(body[i], temp, j);
            j+=body[i]->return_size();
            i++;
        }
        if(body[i]->return_type() == "GRK")
        {
            if(counter.quan==0)
            {
            }
            else
            {
                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();

                read_group(body[i], temp, j);
                j+=body[i]->return_size();
            }
        }
        if(body[i]->return_type() == "CNT")
        {
            counter.description = body[i]->return_grk_cnt();
            counter.quan = temp[j];
            read_elem(body[i], temp, j);
            j+=body[i]->return_size();
            i++;
        }
        else
        {
            read_elem(body[i], temp, j);
            j+=body[i]->return_size();
            i++;
        }
    }
}

void reader :: read_group(Elem *ex, std::vector<uint8_t> temp, int numb)// считывает группы в бинарнике и записывает в вектор
{
    std::string decl = "----------------------|";
    decl+=ex->return_name();
    decl+="|--------------------\n";
    parsed_file.push_back(decl);

    Elem* gr = ex;
    std::vector<Elem*> gr_elems = gr->return_gr_elems();
    int foo = numb;

    if(gr->return_type()=="GRP")
    {
        std::vector<int> indx = gr->return_grp_ind();
        if(indx[1]==0)
        {
            for(int i=0;i!=indx[0];i++)
            {
                int k=0;
                while(k!=gr_elems.size())
                {
                    read_elem(gr_elems[k], temp, foo);
                    foo+=gr_elems[k]->return_size();
                    k++;
                }
                parsed_file.push_back("|||||||||||||||||||||");
            }
        }
        else if(indx[1]!=0)
        {
            for(int i=0;i!=indx[0];i++)
            {
                parsed_file.push_back("///////////////////////");

                for(int j=0;j!=indx[1];j++)
                {
                    parsed_file.push_back("|||||||||||||||||||||\n");

                    int k=0;
                    while(k<gr_elems.size())
                    {
                        read_elem(gr_elems[k], temp, foo);
                        foo+=gr_elems[k]->return_size();
                        k++;
                    }
                    parsed_file.push_back("|||||||||||||||||||||");
                }
                parsed_file.push_back("///////////////////////");
            }
        }
    }
    else if(gr->return_type()=="GRK")
    {
        int k=0;
        while(k<gr_elems.size())
        {
            if(gr_elems[k]->return_type() == "GRP")
            {
                read_group(gr_elems[k], temp, foo);
            }
            else
            {
                read_elem(gr_elems[k], temp, foo);
            }

            foo+=gr_elems[k]->return_size();
            k++;
        }
        parsed_file.push_back("*************************************\n");
    }
    parsed_file.push_back("----------------------END-OF-GROUP------------------------");

}

void reader :: read_elem(Elem* ex, std::vector<uint8_t> temp, int j)// считывает обычный элемент в бинарнике и записывает в вектор
{
    std::string edited;
    Elem* per = ex;

    if(per->return_sav_type()=="B")
    {
        if(per->return_size() == 1)
        {
            if(int(temp[j]==128)||int(temp[j])==7)
            {
                edited = per->return_name();
                edited+="-";
                edited+="Данные отсутствуют.";
            }
            else
            {
                edited = per->return_name();
                edited+="-";
                edited+=std::to_string(int(temp[j]));
            }
        }
        else if(per->return_size() == 2)
        {
            int res = paste_together(temp[j], temp[j+1]);
            if(int(res==32768))
            {
                edited = per->return_name();
                edited+="-";
                edited+="Данные отсутствуют.";
            }
            else
            {
                edited = per->return_name();
                edited+="-";
                edited+=(std::to_string(res));
            }
        }
        else if(per->return_size() == 3)
        {
            int res = paste_together(temp[j], temp[j+1], temp[j+2]);
            if(int(res==8388608))
            {
                edited = per->return_name();
                edited+="-";
                edited+="Данные отсутствуют.";
            }
            else
            {
                edited = per->return_name();
                edited+="-";
                edited+=(std::to_string(res));
            }
        }
        else if(per->return_size() == 4)
        {
            int res = paste_together(temp[j], temp[j+1], temp[j+2], temp[j+3]);
            if(int(res==-2147483648))
            {
                edited = per->return_name();
                edited+="-";
                edited+="Данные отсутствуют.";
            }
            else
            {
                edited = per->return_name();
                edited+="-";
                edited+=(std::to_string(res));
            }
        }
    }
    else if (per->return_sav_type()=="A")
    {
        std::vector<int> line;

        std::copy(temp.begin()+j,temp.begin()+j+per->return_size(), std::back_inserter(line));
        edited = per->return_name();
        edited+=": ";
        std::string data = koi_to_ascii(line);
        edited+=data;
    }
    parsed_file.push_back(edited);
}

/*просто побайтовые сложения*/
int reader :: paste_together(uint8_t a, uint8_t b)
{
    int result = (a << 8) | b;
    return result;
}

int reader :: paste_together(uint8_t a, uint8_t b, uint8_t c)
{
    int result = (((a << 8) | b) << 8) | c;
    return result;
}

int reader :: paste_together(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    int result = (((((a << 8) | b) << 8) | c) <<8 ) | d;
    return result;
}
/*переводит посимвольно в аски и возвращает готовую строку*/
std::string reader :: koi_to_ascii(std::vector<int> dkoi)
{
    std::string converted;
    std::string temp;

    for(int i=0;i<dkoi.size();i++)
        switch (dkoi[i])
        {
        case 0xC1:
            temp = "А";
            converted+=temp;
            break;
        case 0x81:
            temp = "а";
            converted+=temp;
            break;
        case 0xBA:
            temp = "Б";
            converted+=temp;
            break;
        case 0x78:
            temp = "б";
            converted+=temp;
            break;
        case 0xC2:
            temp = "B";
            converted+=temp;
            break;
        case 0xAF:
            temp = "в";
            converted+=temp;
            break;
        case 0xBF:
            temp = "Г";
            converted+=temp;
            break;
        case 0x8D:
            temp = "г";
            converted+=temp;
            break;
        case 0xBC:
            temp = "Д";
            converted+=temp;
            break;
        case 0x8A:
            temp = "д";
            converted+=temp;
            break;
        case 0xC5:
            temp = "Е";
            converted+=temp;
            break;
        case 0x85:
            temp = "е";
            converted+=temp;
            break;
        case 0xEC:
            temp = "Ж";
            converted+=temp;
            break;
        case 0xAE:
            temp = "ж";
            converted+=temp;
            break;
        case 0xFA:
            temp = "З";
            converted+=temp;
            break;
        case 0xB2:
            temp = "з";
            converted+=temp;
            break;
        case 0xCB:
            temp = "И";
            converted+=temp;
            break;
        case 0x8F:
            temp = "и";
            converted+=temp;
            break;
        case 0xCC:
            temp = "Й";
            converted+=temp;
            break;
        case 0x90:
            temp = "й";
            converted+=temp;
            break;
        case 0xD2:
            temp = "К";
            converted+=temp;
            break;
        case 0x9A:
            temp = "к";
            converted+=temp;
            break;
        case 0xCE:
            temp = "Л";
            converted+=temp;
            break;
        case 0x9B:
            temp = "л";
            converted+=temp;
            break;
        case 0xD4:
            temp = "М";
            converted+=temp;
            break;
        case 0x9C:
            temp = "м";
            converted+=temp;
            break;
        case 0xC8:
            temp = "Н";
            converted+=temp;
            break;
        case 0x9D:
            temp = "н";
            converted+=temp;
            break;
        case 0xD6:
            temp = "О";
            converted+=temp;
            break;
        case 0x96:
            temp = "о";
            converted+=temp;
            break;
        case 0xDC:
            temp = "П";
            converted+=temp;
            break;
        case 0x9F:
            temp = "п";
            converted+=temp;
            break;
        case 0xD7:
            temp = "Р";
            converted+=temp;
            break;
        case 0x97:
            temp = "р";
            converted+=temp;
            break;
        case 0xC3:
            temp = "С";
            converted+=temp;
            break;
        case 0x83:
            temp = "с";
            converted+=temp;
            break;
        case 0xE3:
            temp = "Т";
            converted+=temp;
            break;
        case 0xAC:
            temp = "т";
            converted+=temp;
            break;
        case 0xEB:
            temp = "У";
            converted+=temp;
            break;
        case 0xA8:
            temp = "у";
            converted+=temp;
            break;
        case 0xBE:
            temp = "Ф";
            converted+=temp;
            break;
        case 0x8C:
            temp = "ф";
            converted+=temp;
            break;
        case 0xE7:
            temp = "Х";
            converted+=temp;
            break;
        case 0xA7:
            temp = "х";
            converted+=temp;
            break;
        case 0xBB:
            temp = "Ц";
            converted+=temp;
            break;
        case 0x80:
            temp = "ц";
            converted+=temp;
            break;
        case 0xFE:
            temp = "Ч";
            converted+=temp;
            break;
        case 0xB6:
            temp = "ч";
            converted+=temp;
            break;
        case 0xFB:
            temp = "Ш";
            converted+=temp;
            break;
        case 0xB3:
            temp = "ш";
            converted+=temp;
            break;
        case 0xFD:
            temp = "Щ";
            converted+=temp;
            break;
        case 0xB5:
            temp = "щ";
            converted+=temp;
            break;
        case 0xB7:
            temp = "ъ";
            converted+=temp;
            break;
        case 0xEF:
            temp = "Ы";
            converted+=temp;
            break;
        case 0xB1:
            temp = "ы";
            converted+=temp;
            break;
        case 0xEE:
            temp = "Ь";
            converted+=temp;
            break;
        case 0xB0:
            temp = "ь";
            converted+=temp;
            break;
        case 0xFC:
            temp = "Э";
            converted+=temp;
            break;
        case 0xB4:
            temp = "э";
            converted+=temp;
            break;
        case 0xB8:
            temp = "Ю";
            converted+=temp;
            break;
        case 0x76:
            temp = "ю";
            converted+=temp;
            break;
        case 0xDD:
            temp = "Я";
            converted+=temp;
            break;
        case 0xA0:
            temp = "я";
            converted+=temp;
            break;
        case 0x4B:
            temp = ".";
            converted+=temp;
            break;
        case 0x60:
            temp = "-";
            converted+=temp;
            break;
        case 0x40:
            temp = " ";
            converted+=temp;
            break;
        case 0xF1:
            temp = "1";
            converted+=temp;
            break;
        case 0xF2:
            temp = "2";
            converted+=temp;
            break;
        case 0xF3:
            temp = "3";
            converted+=temp;
            break;
        case 0xF4:
            temp = "4";
            converted+=temp;
            break;
        case 0xF5:
            temp = "5";
            converted+=temp;
            break;
        case 0xF6:
            temp = "6";
            converted+=temp;
            break;
        case 0xF7:
            temp = "7";
            converted+=temp;
            break;
        case 0xF8:
            temp = "8";
            converted+=temp;
            break;
        case 0xF9:
            temp = "9";
            converted+=temp;
            break;
        case 0xF0:
            temp = "0";
            converted+=temp;
            break;
        case 0x6B:
            temp = ",";
            converted+=temp;
            break;
        case 0xFF:
            temp = "EO";
            converted+=temp;
            break;
        default:
            continue;
            break;
        }
    return converted;
}

