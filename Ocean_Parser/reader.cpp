#include "reader.h"

reader :: reader(std::string bfile) // ���������� �������� � ������ uint8
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

void reader :: read_records(parser ddl) // ������ ��������
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

void reader :: read_bin (parser ddl) // ��� ���������� ����� ���������
{
    while(binfile.size()>11)
    {
        read_records(ddl);
        push_records();
        read_body(ddl);
    }
}

std::vector<std::string> reader :: get_parsed() // ���������� ������� ������
{
    return parsed_file;
}

void reader :: push_records() // ���������� ������� � ������� ������
{
    std::string lng;
    lng = "����� ������ - ";
    lng+=std::to_string(records.lng);
    parsed_file.push_back(" ");
    parsed_file.push_back(lng);

    lng = "���� - ";
    lng+=std::to_string(records.nulls);
    parsed_file.push_back(lng);

    lng = "������� - ";
    lng+=std::to_string(records.station);
    parsed_file.push_back(lng);

    lng = "��� - ";
    lng+=std::to_string(records.year);
    parsed_file.push_back(lng);

    lng = "����� - ";
    lng+=std::to_string(records.month);
    parsed_file.push_back(lng);

    lng = "��� ������ - ";
    lng+=std::to_string(records.rec_type);
    parsed_file.push_back(lng);
    parsed_file.push_back(" ");

}

void reader :: read_body(parser ddl) // ��������� ���� � ���������
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

void reader :: read_group(Elem *ex, std::vector<uint8_t> temp, int numb)// ��������� ������ � ��������� � ���������� � ������
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

void reader :: read_elem(Elem* ex, std::vector<uint8_t> temp, int j)// ��������� ������� ������� � ��������� � ���������� � ������
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
                edited+="������ �����������.";
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
                edited+="������ �����������.";
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
                edited+="������ �����������.";
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
                edited+="������ �����������.";
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

/*������ ���������� ��������*/
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
/*��������� ����������� � ���� � ���������� ������� ������*/
std::string reader :: koi_to_ascii(std::vector<int> dkoi)
{
    std::string converted;
    std::string temp;

    for(int i=0;i<dkoi.size();i++)
        switch (dkoi[i])
        {
        case 0xC1:
            temp = "�";
            converted+=temp;
            break;
        case 0x81:
            temp = "�";
            converted+=temp;
            break;
        case 0xBA:
            temp = "�";
            converted+=temp;
            break;
        case 0x78:
            temp = "�";
            converted+=temp;
            break;
        case 0xC2:
            temp = "B";
            converted+=temp;
            break;
        case 0xAF:
            temp = "�";
            converted+=temp;
            break;
        case 0xBF:
            temp = "�";
            converted+=temp;
            break;
        case 0x8D:
            temp = "�";
            converted+=temp;
            break;
        case 0xBC:
            temp = "�";
            converted+=temp;
            break;
        case 0x8A:
            temp = "�";
            converted+=temp;
            break;
        case 0xC5:
            temp = "�";
            converted+=temp;
            break;
        case 0x85:
            temp = "�";
            converted+=temp;
            break;
        case 0xEC:
            temp = "�";
            converted+=temp;
            break;
        case 0xAE:
            temp = "�";
            converted+=temp;
            break;
        case 0xFA:
            temp = "�";
            converted+=temp;
            break;
        case 0xB2:
            temp = "�";
            converted+=temp;
            break;
        case 0xCB:
            temp = "�";
            converted+=temp;
            break;
        case 0x8F:
            temp = "�";
            converted+=temp;
            break;
        case 0xCC:
            temp = "�";
            converted+=temp;
            break;
        case 0x90:
            temp = "�";
            converted+=temp;
            break;
        case 0xD2:
            temp = "�";
            converted+=temp;
            break;
        case 0x9A:
            temp = "�";
            converted+=temp;
            break;
        case 0xCE:
            temp = "�";
            converted+=temp;
            break;
        case 0x9B:
            temp = "�";
            converted+=temp;
            break;
        case 0xD4:
            temp = "�";
            converted+=temp;
            break;
        case 0x9C:
            temp = "�";
            converted+=temp;
            break;
        case 0xC8:
            temp = "�";
            converted+=temp;
            break;
        case 0x9D:
            temp = "�";
            converted+=temp;
            break;
        case 0xD6:
            temp = "�";
            converted+=temp;
            break;
        case 0x96:
            temp = "�";
            converted+=temp;
            break;
        case 0xDC:
            temp = "�";
            converted+=temp;
            break;
        case 0x9F:
            temp = "�";
            converted+=temp;
            break;
        case 0xD7:
            temp = "�";
            converted+=temp;
            break;
        case 0x97:
            temp = "�";
            converted+=temp;
            break;
        case 0xC3:
            temp = "�";
            converted+=temp;
            break;
        case 0x83:
            temp = "�";
            converted+=temp;
            break;
        case 0xE3:
            temp = "�";
            converted+=temp;
            break;
        case 0xAC:
            temp = "�";
            converted+=temp;
            break;
        case 0xEB:
            temp = "�";
            converted+=temp;
            break;
        case 0xA8:
            temp = "�";
            converted+=temp;
            break;
        case 0xBE:
            temp = "�";
            converted+=temp;
            break;
        case 0x8C:
            temp = "�";
            converted+=temp;
            break;
        case 0xE7:
            temp = "�";
            converted+=temp;
            break;
        case 0xA7:
            temp = "�";
            converted+=temp;
            break;
        case 0xBB:
            temp = "�";
            converted+=temp;
            break;
        case 0x80:
            temp = "�";
            converted+=temp;
            break;
        case 0xFE:
            temp = "�";
            converted+=temp;
            break;
        case 0xB6:
            temp = "�";
            converted+=temp;
            break;
        case 0xFB:
            temp = "�";
            converted+=temp;
            break;
        case 0xB3:
            temp = "�";
            converted+=temp;
            break;
        case 0xFD:
            temp = "�";
            converted+=temp;
            break;
        case 0xB5:
            temp = "�";
            converted+=temp;
            break;
        case 0xB7:
            temp = "�";
            converted+=temp;
            break;
        case 0xEF:
            temp = "�";
            converted+=temp;
            break;
        case 0xB1:
            temp = "�";
            converted+=temp;
            break;
        case 0xEE:
            temp = "�";
            converted+=temp;
            break;
        case 0xB0:
            temp = "�";
            converted+=temp;
            break;
        case 0xFC:
            temp = "�";
            converted+=temp;
            break;
        case 0xB4:
            temp = "�";
            converted+=temp;
            break;
        case 0xB8:
            temp = "�";
            converted+=temp;
            break;
        case 0x76:
            temp = "�";
            converted+=temp;
            break;
        case 0xDD:
            temp = "�";
            converted+=temp;
            break;
        case 0xA0:
            temp = "�";
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

