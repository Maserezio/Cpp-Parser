#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <iostream>
#include <vector>

class Elem // родительский класс любого элемента
{
protected:
    std::string type;
    std::string name;
public:
    Elem(std::string _type,std::string _name):type(_type),name(_name) {}
    virtual void see() const = 0;
    /* методы для возвращения инфы об элементе */
    virtual int return_size() const = 0; // размер
    virtual std::string return_sav_type() const = 0; //тип хранения
    virtual std::string return_type() // просто тип
    {
        return type;
    }
    virtual std::string return_name() //название
    {
        return name;
    }
    /* инфа о разных группах */
    virtual std::vector<int> return_grp_ind() const = 0; // индексы grp
    virtual std::string return_grk_cnt() const = 0; // счётчик cnt
    virtual std::vector<Elem*> return_gr_elems() const = 0; // просто возвращает элементы


};

class SimElem : public Elem // все элементы, кроме групп
{
protected:
    std::string sav_type;
    int sav_bytes;
public:
    SimElem(std::string _type,std::string _name,std::string _sav_type, int _sav_bytes):Elem(_type,_name)
    {
        sav_type = _sav_type;
        sav_bytes = _sav_bytes;
    }

    void see() const
    {
        std::cout << type << "\t" << name << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }
    virtual std::string return_sav_type() const
    {
        return sav_type;
    }
    virtual std::vector<int> return_grp_ind() const {}
    virtual std::string return_grk_cnt() const {}

    virtual std::vector<Elem*> return_gr_elems() const {}
};
/*просто одни и те же классы только одно поле разное*/
class MIT : public SimElem
{
public:
    MIT (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes) : SimElem(_type, _name, _sav_type, _sav_bytes) {}

    void see() const
    {
        std::cout << type << "\t\t" << name << "\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }

};

class CHA : public SimElem
{
public:
    CHA (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes) : SimElem(_type, _name, _sav_type, _sav_bytes) {}

    void see() const
    {
        std::cout << type << "\t\t"  << "\t\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }
    virtual std::string return_name()
    {
        std::string cha;
        cha = "Характеристика ";
        cha+=this->name;
        return cha;
    }
    int return_size() const
    {
        return sav_bytes;
    }
};

class KEY : public SimElem
{
    std::string ord_method;
public:
    KEY (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes ,std::string _ord_method) : SimElem(_type, _name, _sav_type, _sav_bytes)
    {
        ord_method = _ord_method;
    }

    void see() const
    {
        std::cout << type << "\t" << ord_method <<"\t" << name << "\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }
};

class LNG : public SimElem
{
public:
    LNG (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes) : SimElem(_type, _name, _sav_type, _sav_bytes) {}

    void see() const
    {
        std::cout << type << "\t\t" << name << "\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }
};

class MRC : public SimElem
{
    std::string ord_method;
public:
    MRC (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes ,std::string _ord_method) : SimElem(_type, _name, _sav_type, _sav_bytes)
    {
        ord_method = _ord_method;
    }

    void see() const
    {
        std::cout << type << "\t" << ord_method <<"\t" << name << "\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }
};

class CNT : public SimElem
{
public:
    CNT (std::string _type,std::string _name,std::string _sav_type, int _sav_bytes) : SimElem(_type, _name, _sav_type, _sav_bytes) {}

    void see() const
    {
        std::cout << type << "\t" << name << "\t\t" << sav_type << "-" << sav_bytes << std::endl;
    }

    int return_size() const
    {
        return sav_bytes;
    }
};
/*классы групп*/
class Group : public Elem
{
public:
    Group(std::string _type,std::string _name):Elem(_type,_name) {}

    void see() const
    {
        std::cout << type << "\t" << name << "\t" << std::endl;
    }
    virtual std::string return_sav_type() const
    {
        return "Group";
    }

    virtual std::vector<int> return_grp_ind() const {}
    virtual std::string return_grk_cnt() const {}


};

class GRK : public Group //обычная группа,тольео может повторяться
{
private:
    std::vector <Elem*> elems;
    std::string cnt;

public:
    GRK(std::string _type,std::string _name, std::string _cnt, std::vector <Elem*> _elems):Group(_type,_name)
    {
        cnt = _cnt;
        std::copy(_elems.begin(),_elems.end(), std::back_inserter(elems));

    }

    void see() const
    {
        std::cout  << "\n\t|\t" << type << "\t" << cnt << "\t" << name << "\t" << std::endl;

        for(int i = 0; i!=elems.size(); i++)
        {
            std::cout << "\t|\t";
            elems[i]->see();
        }
        std::cout << std::endl;
    }

    int return_size() const
    {
        int size=0;

        for(int i=0;i<elems.size();i++)
            size+=elems[i]->return_size();

        return size;
    }
    virtual std::vector<Elem*> return_gr_elems() const
    {
        return elems;
    }
    virtual std::string return_grk_cnt() const
    {
        return cnt;
    }

};

class GRP : public Group// группа как массив. может быть одномерной и двумерной
{
private:
    std::vector <Elem*> elems;
    int ind1; // т.к. инды не имеют байтов для хранения, будут индексами
    std::string desc1;
    int ind2;
    std::string desc2;

public:
    GRP(std::string _type,std::string _name, std::vector <Elem*> _elems, int _ind1, std::string _desc):Group(_type,_name)
    {
        std::copy(_elems.begin(),_elems.end(), std::back_inserter(elems));
        ind1 = _ind1;
        desc1 = _desc;
        ind2 = 0;
        desc2 = "";
    }
    GRP(std::string _type,std::string _name, std::vector <Elem*> _elems, int _ind1, std::string _desc,int _ind2, std::string _desc2):Group(_type,_name)
    {
        std::copy(_elems.begin(),_elems.end(), std::back_inserter(elems));
        ind1 = _ind1;
        desc1 = _desc;
        ind2 = _ind2;
        desc2 = _desc2;
    }

    void see() const
    {
        if(ind2==0)
            std::cout << "\n\t|\t" << type << "\t" << name << "\n\t|\t" <<"["<<ind1 << "-" << desc1 <<"]" << std::endl;
        else
            std::cout  << "\n\t|\t" << type << "\t" << name << "\n\t|\t" <<"["<<ind1 << "-" << desc1 <<"]" << "[" << ind2 << "-" << desc2 << "]" << std::endl;

        for(int i = 0; i!=elems.size(); i++)
        {
            std::cout << "\t|\t";
            elems[i]->see();
        }
        std::cout << std::endl;
    }

    int return_size() const
    {
        if(ind2==0)
        {
            int size=0;

            for(int i=0;i<elems.size();i++)
                size+=elems[i]->return_size();

            return size*ind1;
        }
        else
        {
            int size=0;

            for(int i=0;i<elems.size();i++)
                size+=elems[i]->return_size();

            return size*ind1*ind2;
        }
    }
    virtual std::vector<int> return_grp_ind() const
    {
        std::vector<int> inds;
        inds.push_back(ind1);
        inds.push_back(ind2);

        return inds;
    }
    virtual std::vector<Elem*> return_gr_elems() const
    {
        return elems;
    }

};


#endif // ELEMENTS_H
