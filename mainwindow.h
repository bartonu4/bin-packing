#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <qmath.h>
#include <QFile>
#include <QPair>
#define var1 "05 25 84 27 36 05 46 29 13 57 24 95 82 45 14 67 34 64 43 50 "
#define var2 "87 08 76 78 88 84 03 51 54 99 32 60 76 68 39 12 26 86 94 39 "
#define var3 "95 70 34 78 67 01 97 02 17 92 52 56 01 80 86 41 65 89 44 19 "
#define varAll var1 var2 var3
namespace Ui {

class MainWindow;
}
struct Element
{
    int id, weight;
        Element(){}
        Element(int _id,int _weight):id(_id),weight(_weight){}
};
struct Container
{
    static const int size= 100;
    QVector<Element> elements;
    int empty;

    Container(){
        empty = size;
    }
    bool addElement(Element element)
    {
        if(element.weight<=empty)
        {   empty-=element.weight;
            elements.append(element);
            return true;
        }
        else
        {
            return false;
        }
    }
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
void createVector(const char *input);
void calculateContainers();
private slots:
void on_pushButton_clicked();
QPair<QVector<Container>, int> ffa(QVector<int> _weights, bool sort);
QPair<QVector<Container>, int> nfa(QVector<int> _weights, bool sort);
QPair<QVector<Container>, int> wfa(QVector<int> _weights, bool sort);
QPair<QVector<Container>, int> bfa(QVector<int> _weights, bool sort);
void printer();
void createTable(QVector<Container> containers, QString algorithm, int complexity);
private:
    Ui::MainWindow *ui;
     QVector<int> weights,complex;
     QFile *file;
     QTextStream *stream;


};



#endif // MAINWINDOW_H
