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
    delete stream;
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
    createTable(nfa(weights,false),"nfa");
     createTable(nfa(weights,true),"nfa sorted");
    createTable(ffa(weights,false),"ffa");
     createTable(ffa(weights,true),"ffa sorted");
    createTable(wfa(weights,false),"wfa");
     createTable(wfa(weights,true),"wfa sorted");
    createTable(bfa(weights,false),"bfa");
     createTable(bfa(weights,true),"bfa sorted");

    close();
}


void MainWindow::on_pushButton_clicked()
{
     createVector(varAll);
    createVector(var1);
    createVector(var2);
    createVector(var3);
    qDebug()<<"vectorAll "<<varAll;


    file->close();

}
bool lessThan(const int &a,const int &b)
{
    return b<a;
}

QVector<Container> MainWindow::ffa(QVector<int> _weights, bool sort)
{

    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    qDebug()<<_weights;
    if(sort)
    {
        std::sort(_weights.begin(),_weights.end(),lessThan);
        complexity+=_weights.size()*log(_weights.size());
    }
      qDebug()<<_weights;
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

    return containers;

}

QVector<Container> MainWindow::nfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
       std::sort(_weights.begin(),_weights.end(),lessThan);
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
     return containers;
}

QVector<Container> MainWindow::wfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
       std::sort(_weights.begin(),_weights.end(),lessThan);
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
     return containers;

}

QVector<Container> MainWindow::bfa(QVector<int> _weights, bool sort)
{
    int complexity = 0;
    QVector<Container> containers;
    containers.append(Container());
    if(sort)
    {
       std::sort(_weights.begin(),_weights.end(),lessThan);
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
     return containers;
}

void MainWindow::printer()
{
     file = new QFile("result.html");
    if(!file->open(QIODevice::Text|QIODevice::WriteOnly))
        return;
        stream = new QTextStream(file);
        *stream<<"<html>\n<head><link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" integrity=\"sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7\" crossorigin=\"anonymous\"><style>td{width:20px;text-align:center;}.table>tbody>tr>td{padding:0;}</style><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script><script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script></head>\n<body><div class=\"container-fluid\" width=\"5000px\"\>";


}

void MainWindow::createTable(QVector<Container> containers, QString algorithm)
{   *stream<<"<div class=\"row-fluid\"><div class=\"col-lg-8\"><table border=\"1\" class=\"table table-responsive\" padding=\"0\"><h3>"+algorithm+"</h3>";
     QList<int>weightsForContainer;
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
       weightsForContainer<<Container::size -containers.at(i).empty;


    }
     *stream <<"</table></div><div class=\"col-lg-4\"><table border=\"1\" class=\"table table-responsive\">";
     for(int i =0;i<containers.size();++i)
    {
          *stream<<"<tr>";
           *stream <<QString("<td>%1 </td>").arg(weightsForContainer.at(i));
          *stream<<"</tr>";

     }
      *stream <<QString(tr("<h3>Containers Counts: %1</h3>")).arg(containers.size());
      *stream <<"</table>";

     *stream << "</div></div>";
}
