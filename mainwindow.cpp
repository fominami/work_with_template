#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <iostream>
#include <QStandardItemModel>
#include<QString>
#include<QVector>
#include<QMessageBox>
using namespace std;
struct Table_Titel// исполюзую как часть класса для заглавия таблицы, вывод показан на примере вывода М1
{
    QString Name="Name";
    QString Field="Other Information";
    QString Section="Section";
};

struct Comp{
    QString Name;
    QString Section;
    int Price;
    friend QTextStream& operator<<(QDataStream&bin,Comp& person);
    friend QTextStream& operator>>(QTextStream&bin,Comp& person);
    friend ostream& operator<<(ostream&out,Comp&person);
    bool operator==(Comp& other) const {
        return Name == other.Name;
    }
};
QTextStream& operator<<(QTextStream&bin,Comp& person)
{
    bin<<person.Name<<" "<<person.Section<<" "<<person.Price;
    return bin;
}
QTextStream& operator>>(QTextStream&bin,Comp& person)
{
    bin>>person.Name>>person.Section>>person.Price;
    return bin;
}

ostream& operator<<(ostream&out,Comp&person)
{
    out<<person.Name.toStdString()<<" "<<person.Section.toStdString()<<" "<<person.Price<<endl;
    return out;
}
using namespace std;
struct Bag{
    QString Name;
    int Num;
    QString Section;
    friend QTextStream& operator<<(QDataStream&bin,Comp& person);
    friend QTextStream& operator>>(QTextStream&bin,Comp& person);
    friend ostream& operator<<(ostream&out,Comp&person);
    bool operator==(Bag& other) const {
        return Name == other.Name;
    }
};
QTextStream& operator<<(QTextStream&bin,Bag& person)
{
    bin<<person.Name<<" "<<person.Num<<" "<<person.Section;
    return bin;
}
QTextStream& operator>>(QTextStream&bin,Bag& person)
{
    bin>>person.Name>>person.Num>>person.Section;
    return bin;
}

ostream& operator<<(ostream&out,Bag&person)
{
    out<<person.Name.toStdString()<<" "<<person.Num<<" "<<person.Section.toStdString()<<endl;
    return out;
}
template<typename T>
class COMP
{public:
    Table_Titel t;
private:

    int size;
    T *arr;
public:
    COMP(){arr=nullptr; size=0;};
    COMP(int a){size=a; arr=new T[a];};
    ~COMP(){delete[]arr; size=0;};
    void ReadData(QFile& input1);//считка с текстового
    void WriteData(QFile& outBin);//запись в текстовый файл
    int GetSize(){return size;};
    void Sort();//вызов сортировки по именам
    void SortSec();//вызов сортировки по секциям
    template<typename U>
    friend COMP<U> operator +(COMP<U>& a, COMP<U>& b);//обьединение
    T operator[](int i){return arr[i];};
    COMP<T> & operator =(const COMP<T>&other)
    {
        this -> size =other.size;
        if (this-> arr!= nullptr)
        {     delete[] this->arr;}
        this->arr= new T[other.size];
        for (int i=0;i<other.size;i++)
            this->arr[i]=other.arr[i];
        return *this;
    }
};
template<typename T>
void COMP<T>::SortSec()
{
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j <size; j++)
            if (arr[i].Section < arr[j].Section)
                swap(arr[i], arr[j]);
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j <size; j++)
            if ((arr[i].Section== arr[j].Section)&&(arr[i].Name < arr[j].Name))
                swap(arr[i], arr[j]);

}
template<typename T>
void COMP<T>::Sort()
{

    for (int i = 0; i < size; i++)
        for (int j = i + 1; j <size; j++)
            if (arr[i].Name < arr[j].Name)
                swap(arr[i], arr[j]);
}
template<typename T>
void COMP<T>::WriteData(QFile& outBin)
{
    QTextStream out1(&outBin);
    if (!outBin.open(QIODevice::WriteOnly))
    {
        cout<<"problems with yout binary file 1.txt"<<endl;
    }
    for (int i=0;i<size;i++) {
        out1<<arr[i];
    }
    outBin.close();
}
template<typename T>
void COMP<T>::ReadData(QFile& input1)
{
    QTextStream in1(&input1);
    if(!input1.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        cout<<"problems with txt"<<endl;
    }
    cout<<"Information from txt"<<endl;
    int count(0);
    while (!input1.atEnd()) {
        input1.readLine();
        count++;
    }
    size=count;
    arr=new T[size];
    //cout<<lineCount;
    input1.close();
    input1.open(QIODevice::ReadOnly | QIODevice::Text);
    for(int i=0;i<size;i++)// каждый элемент структуры через пробел
    {
        in1>>arr[i];
    }
    for (int i=0;i<size;i++) {
        cout<<arr[i];
    }
    input1.close();
}
template<typename T>
COMP<T> operator +( COMP<T>&a, COMP<T>&b)
{
    int count=0;
    COMP<T> e;
    QVector<T> all;
    for(int i=0;i<a.GetSize();i++)
    {
        all.push_back(a[i]);
    }
        bool flag=false;
        for(int i=0;i<b.GetSize();i++)
        {
            flag=false;
            for(int j=0;j<all.size();j++)
            {
                if(all[j].Name==b[i].Name)
                {flag=true;};
            }
            if(!flag)
            {
                all.push_back(b[i]);
            }
        }
    e.size=all.size();
    e.arr=new T[e.size];
    for(int i=0;i<all.size();i++)
    {
        e.arr[i]=all[i];
    }
    return e;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
;
void MainWindow::on_pushButton_clicked()
{
    COMP<Comp> M1,M2,M3;
    COMP<Bag> K1, K2,K3;
    QFile input1("input1.txt");

    M1.ReadData(input1);

    QStandardItemModel *m_c = new QStandardItemModel(this);//выделяем память под модель
    ui->table1->setModel(m_c);//придали табличке модель
    m_c->setColumnCount(3);
    m_c->setRowCount(M1.GetSize()+1);
    m_c->setData(m_c->index(0,0), M1.t.Name);
    m_c->setData(m_c->index(0,1), M1.t.Section);
    m_c->setData(m_c->index(0,2), M1.t.Field);
    for(int i = 1; i <=M1.GetSize(); i++){
        QString str(M1[i-1].Name);
        QString str2(M1[i-1].Section);
        m_c->setData(m_c->index(i,0), str);
        m_c->setData(m_c->index(i,1), str2);
        m_c->setData(m_c->index(i,2), M1[i-1].Price);

    }
    QFile input2("input2.txt");
    M2.ReadData(input2);
    QStandardItemModel *m = new QStandardItemModel(this);//выделяем память под модель
    ui->table2->setModel(m);//придали табличке модель
    m->setColumnCount(3);
    m->setRowCount(M2.GetSize());//GETTER для класса MB1(не 3 короче)
    for(int i = 0; i < M2.GetSize(); i++){
        QString str(M2[i].Name);
        QString str2(M2[i].Section);
        m->setData(m->index(i,0), str);
        m->setData(m->index(i,1), str2);
        m->setData(m->index(i,2), M2[i].Price);

    }
    M3=M1+M2;
    QFile output1("OutM3.txt");
    M3.Sort();
    M3.WriteData(output1);
    QStandardItemModel *r = new QStandardItemModel(this);//выделяем память под модель
    ui->resm->setModel(r);//придали табличке модель
    r->setColumnCount(3);
    r->setRowCount(M3.GetSize());
    for(int i = 0; i < M3.GetSize(); i++){
        QString str(M3[i].Name);
        QString str2(M3[i].Section);
        r->setData(r->index(i,0), str);
        r->setData(r->index(i,1), str2);
        r->setData(r->index(i,2), M3[i].Price);

    }
    QFile input3("input3.txt");
    K1.ReadData(input3);
    QStandardItemModel *m_b = new QStandardItemModel(this);//выделяем память под модель, this - наш mainwindow.
    ui->table3->setModel(m_b);//придали табличке модель
    m_b->setColumnCount(3);
    m_b->setRowCount(K1.GetSize());
    for(int i = 0; i < K1.GetSize(); i++){
        QString str(K1[i].Name);
        QString str2(K1[i].Section);
        m_b->setData(m_b->index(i,0), str);
        m_b->setData(m_b->index(i,1), K1[i].Num);
        m_b->setData(m_b->index(i,2), str2);
    }
    QFile input4("input4.txt");
    K2.ReadData(input4);
    QStandardItemModel *b = new QStandardItemModel(this);//выделяем память под модель, this - наш mainwindow.
    ui->table4->setModel(b);//придали табличке модель
    b->setColumnCount(3);
    b->setRowCount(K2.GetSize());
    for(int i = 0; i < K2.GetSize(); i++){
        QString str(K2[i].Name);
        QString str2(K2[i].Section);
        b->setData(b->index(i,0), str);
        b->setData(b->index(i,1), K2[i].Num);
        b->setData(b->index(i,2), str2);
    }
    K3=K1+K2;
    QFile output2("Resk3.txt");
    K3.SortSec();
    K3.WriteData(output2);
    QStandardItemModel *rb = new QStandardItemModel(this);//выделяем память под модель, this - наш mainwindow.
    ui->tableres->setModel(rb);//придали табличке модель
    rb->setColumnCount(3);
    rb->setRowCount(K3.GetSize());
    for(int i = 0; i < K3.GetSize(); i++){
        QString str(K3[i].Name);
        QString str2(K3[i].Section);
        rb->setData(rb->index(i,0), str);
        rb->setData(rb->index(i,1), K3[i].Num);
        rb->setData(rb->index(i,2), str2);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    COMP<Comp> M1;
    QFile input1("input1.txt");
    M1.ReadData(input1);
    QString name=ui->lineEdit->text();
    bool flag;
    flag=false;
    for(int i=0;i<M1.GetSize();i++)
    {
        if(name==M1[i].Name)
        {
            flag=true;
            QString a=QString::number(M1[i].Price);
            QString answer="Name: "+M1[i].Name+" "+"Price: "+a+" "+"Type: "+M1[i].Section;
            QMessageBox::about(this,"Результат поиска",answer);
        }
    }
    if(!flag)
    {
        QMessageBox::warning(this,"Результат поиска","Таких устройство нет");
    }
}

