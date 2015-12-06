#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     printer();
    on_pushButton_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createVector(const char *input)
{
    QString inp(input);
    QStringList list =inp.split(" ");
    weights.clear();


    foreach(QString line,list)
    {
        weights.append(line.toInt());
    }
        weights.pop_back();
    qDebug()<<weights;
    calculateContainers();
}

void MainWindow::calculateContainers()
{
    // QVector<Element> elements;
    QVector<Container> containers;
    nfa(weights,false);
    ffa(weights,false);
    wfa(weights,false);
    bfa(weights,false);
    close();
}


void MainWindow::on_pushButton_clicked()
{
    createVector(var1);
    createVector(var2);
    createVector(var3);
    qDebug()<<"vectorAll "<<varAll;
    createVector(varAll);
    file->close();

}
bool lessThan(const int &a,const int &b)
{
    return a<b;
}

void MainWindow::ffa(QVector<int> _weights, bool sort)
{

    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
        qSort(_weights.begin(),_weights.end());
        complexity+=_weights.size()*log(_weights.size());
    }
    for(int i=0;i<_weights.size();++i)
    {
        complexity++;
        Element e= Element(i,_weights.at(i));
        bool check;
        if(!containers.last().addElement(e))
        {
            for(int j=0;j<containers.size();++j)
            {
                check = containers[j].addElement(e);
                complexity++;
                if(check)
                {
                    j = containers.size();
                }
            }

            if(!check )
            {
                complexity++;
                containers.append(Container());
                containers.last().addElement(Element(i,_weights.at(i)));
            }
        }
    }
    qDebug()<<"ffa"<< containers.size();
    createTable(containers);


}

void MainWindow::nfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
        qSort(_weights.begin(),_weights.end());
        complexity+=_weights.size()*log(_weights.size());
    }
    for(int i=0;i<_weights.size();++i)
    {
        complexity++;
        if(!containers.last().addElement(Element(i,_weights.at(i))))
        {
            containers.append(Container());
            containers.last().addElement(Element(i,_weights.at(i)));
        }
    }
    qDebug()<<"nfa"<< containers.size();
}

void MainWindow::wfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
        qSort(_weights.begin(),_weights.end());
        complexity+=_weights.size()*log(_weights.size());
    }


    for(int i=0;i<_weights.size();++i)
    {
        complexity++;
        Element e= Element(i,_weights.at(i));

        if(!containers.last().addElement(e))
        {
            int min = 0;
            int minInd = -1;
            for(int j=0;j<containers.size()-1;++j)
            {
                if(min < containers.at(j).empty)
                {
                    min = containers.at(j).empty;
                    minInd = j;
                }
                complexity++;

            }
            complexity++;

            if(minInd!=-1 && containers.at(minInd).empty>=e.weight )
            {



                containers[minInd].addElement(e);

            }
            else
            {

                containers.append(Container());
                containers.last().addElement(Element(i,_weights.at(i)));
            }
        }


    }
    qDebug()<<"wfa"<< containers.size();

}

void MainWindow::bfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
        qSort(_weights.begin(),_weights.end());
        complexity+=_weights.size()*log(_weights.size());
    }


    for(int i=0;i<_weights.size();++i)
    {
        complexity++;
        Element e= Element(i,_weights.at(i));

        if(!containers.last().addElement(e))
        {
            int max = 100;
            int maxInd = -1;
            for(int j=0;j<containers.size();++j)
            {
                if(max > containers.at(j).empty&&containers.at(j).empty>=e.weight)
                {
                    max = containers.at(j).empty;
                    maxInd = j;
                }
                complexity++;

            }
            complexity++;

            if(maxInd!=-1)
            {



                containers[maxInd].addElement(e);

            }
            else
            {

                containers.append(Container());
                containers.last().addElement(Element(i,_weights.at(i)));
            }
        }


    }
    qDebug()<<"bfa"<< containers.size();
}

void MainWindow::printer()
{
     file = new QFile("result.html");
    if(!file->open(QIODevice::Text|QIODevice::WriteOnly))
        return;
        stream = new QTextStream(file);
        *stream<<"<html>\n<head><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\"><style>td{width:20px;text-align:center;}</style></head>\n<body>"  ;

}

void MainWindow::createTable(QVector<Container> &containers)
{   *stream<<"<table border=\"1\" class=\"table\">";
     for(int i =0;i<containers.size();++i)
    {
        *stream<<"<tr>";
        QVector<Element> e =containers.at(i).elements;
        int huy = 0;
        for(int j=0;j<e.size();++j)
        {
            for(int k =0;k<e.at(j).id - huy;)
            {
                ++huy;
                *stream <<"<td> </td>";
            }
            ++huy;
            *stream<<"<td bgcolor = \"red\">"<<e.at(j).weight<<"</td>";
        }
        for(int k =0;k<weights.size() - huy;)
        {
            ++huy;
            *stream <<"<td> </td>";
        }
        *stream<<"</tr>";

    }
     *stream <<"</table>";
}
