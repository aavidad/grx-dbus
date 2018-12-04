#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QtDBus/QtDBus>
#include <QList>
#include "grx-dbus.h"


QString arp_scan(const QString &arg)
{
       QProcess process;
       process.start(arg);
       process.waitForFinished(-1);
       return process.readAllStandardOutput();
}

QList<QVariant> ipsNodos()
{
       QString consulta;
       QList<QVariant> vector;
       consulta.append("select ipLinea from NODO");
       QSqlQuery consultar;
       if (consultar.exec(consulta)){
           while (consultar.next()){
               vector.append(consultar.value(0).toString());
           }
       }
       else{
          vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
       }
       return vector;
}

QString ipsNodosString()
{
       QString vector;
       QSqlQuery consultar;
       if (consultar.exec("select ipLinea from NODO")){
           while (consultar.next()){
               vector.append(consultar.value(0).toString()+" ");
           }
       }
       else{
            vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
       }
       return vector;
}

QString GrxDbus::busca_router(const QString &ip)
{
       return arp_scan( ARP_SCAN + ip);
}

QString GrxDbus::crea_conexion (const QString &conexion)
{

   return conexion;
}

QString GrxDbus::busca_routers_muestra_nombres(const QString &ips)
{
       QString arp;
       arp = arp_scan(ARP_SCAN + ips);
       QStringList nodoList = arp.split('\n');
       nodoList.removeAll({}); //Quitamos vacios
       QString nombre_nodo;
       for (int i = 0; i < nodoList.size(); ++i){
            QString nombres, tmp;
            tmp = nodoList.value(i);
            nombres = tmp.remove(QRegExp("[\\t]"));
            if (i < nodoList.size()-1){
               nombre_nodo.append(consulta_sql_nombre(nombres)+"|");
            }
            else{
               nombre_nodo.append(consulta_sql_nombre(nombres));
            }

       }
return nombre_nodo;
}

QList<QVariant> GrxDbus::ip_nodos(){

       return ipsNodos();

}

QString GrxDbus::ip_nodos_string(){

    return ipsNodosString();

}

QString GrxDbus::busca_todos_routers_nombre()
{

    return consulta_sql_nombre(busca_todos_routers());

}

QString GrxDbus::busca_todos_routers()
{
        QString vector;
        QString arp , tmp;
        int i;
        vector=ipsNodosString();
        tmp = arp_scan(ARP_SCAN + vector);
        i = tmp.indexOf("\t"); //Quitamos \t y \n de la cadena
        arp = tmp.mid(0,i);
return arp;
}

//Realiza la consulta de una columna de una tabla
QList<QVariant> GrxDbus::consulta_sql_columna(const QString &tabla,int columna)
{
    QSqlQuery consultar;
    QList<QVariant> vector;

    consultar.prepare(QString("select * from "+tabla));

    if (consultar.exec()){
       while (consultar.next()){
          vector.append(consultar.value(columna).toString());
       }
    } else {
           vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }

return vector;
}

QString GrxDbus::consulta_sql_nombre (const QString &ip){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select nombre from NODO where ipLinea = :ip"));
    consultar.bindValue(":ip", ip);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString().toUtf8());

    } else {
           resultado.append("Nodo no encontrado en la DB "+consultar.lastError().text());
    }
return resultado;
}

QString GrxDbus::consulta_sql_ip (const QString &nombre){
    QSqlQuery consultar;
    QString resultado;
    consultar.prepare(QString("select ipLinea from NODO where nombre = :nombre"));
    consultar.bindValue(":nombre", nombre);
    if (consultar.exec() and consultar.first()){
            resultado.append(consultar.value(0).toString());

    } else {
          resultado.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }
return resultado;
}

QString GrxDbus::busca_nodo_por_ip(const QString &ip)
{
    QString arp, ips_encontradas;
    arp = arp_scan(ARP_SCAN + ip);
    QStringList nodoList = arp.split('\n');
    nodoList.removeAll({});
    for (int i = 0; i < nodoList.size(); ++i){
         QString nombre, tmp;
         tmp = nodoList.value(i);
         nombre = tmp.remove(QRegExp("[\\t]"));
         if (i < nodoList.size()-1){
            ips_encontradas.append(nombre+"|");
         }
         else{
            ips_encontradas.append(nombre);
         }
    }
    return ips_encontradas;
}

bool GrxDbus::esta_nodo_por_ip(const QString &ip)
{
    QString arp;
    arp = arp_scan(ARP_SCAN + ip);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"

    if (ip == nodoList.first()){
        return true;
    } else {
        return false;
    }

}

QString GrxDbus::busca_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_sql_ip(nodo);
    qDebug() << ip_nodo;
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t'); //arp devuelve algo parecido a esto "192.168.1.1\tdd:dd:dd:dd:dd:dd"
    return nodoList.first();//con esto cogemos solo la ip


}

void GrxDbus::mensaje(const QString &mensaje){
    qDebug()<< "Este es el mensaje"<< mensaje;
}

bool GrxDbus::esta_nodo_por_nombre(const QString &nodo)
{
    QString arp;
    QString ip_nodo;
    ip_nodo = consulta_sql_ip(nodo);
    arp = arp_scan(ARP_SCAN + ip_nodo);
    QStringList nodoList = arp.split('\n').first().split('\t');
    if (ip_nodo == nodoList.first()){
        return true;
    } else {
        return false;
    }
}


bool GrxDbus::esta_repositorio(){
    QProcess process;
    process.start("ping -c1 -w1 10.1.1.91");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    else
    {
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << false;
        QDBusConnection::systemBus().send(message);

    }
    return !process.exitCode();
}


bool GrxDbus::esta_veleta(){
    QProcess process;
    process.start("ping -c1 -w1 10.1.1.185");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    else
    {
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << false;
        QDBusConnection::systemBus().send(message);

    }
    return !process.exitCode();
}

bool GrxDbus::esta_mysqlDB(){
    QProcess process;
    process.start("ping -c1 -w1 10.7.15.193");
    process.waitForFinished(-1);
    if (!process.exitCode()){
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << true;
        QDBusConnection::systemBus().send(message);

    }
    else
    {
        QDBusMessage message = QDBusMessage::createSignal("/",SERVICE_NAME, "senal");
        message << false;
        QDBusConnection::systemBus().send(message);

    }
    return !process.exitCode();
}

void GrxDbus::temporizador(){
    QTimer *timer_estaveleta = new QTimer(this);
    connect(timer_estaveleta, SIGNAL(timeout()), this, SLOT(esta_veleta()));
    timer_estaveleta->start(60000);
    QTimer *timer_estarepositorio = new QTimer(this);
    connect(timer_estarepositorio, SIGNAL(timeout()), this, SLOT(esta_repositorio()));
    timer_estarepositorio->start(60000);
    QTimer *timer_estamysqlDB = new QTimer(this);
    connect(timer_estamysqlDB, SIGNAL(timeout()), this, SLOT(esta_mysqlDB()));
    timer_estamysqlDB->start(60000);
}

QList<QVariant> GrxDbus::consulta_sql_todo(const QString &sql)
{
    int col,i;
    QSqlQuery consultar;
    QList<QVariant> vector;
    if (consultar.exec(sql)){
       QSqlRecord record = consultar.record();
       col=record.count();
       while (consultar.next()){
          for (i=0; i< col;i++){
                vector.append(consultar.value(i).toString());
          }

       }
    }else{
          vector.append("Error No se ha podido realizar la consulta "+consultar.lastError().text());
    }

return vector;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if (!crea_conexionSQLite()){
        fprintf(stderr, "No puedo conectarme a la base de datos.\n");
        return 1;
        }
    if (!QDBusConnection::systemBus().isConnected()) {
            fprintf(stderr, "No puedo conectarme al bus del sistema.\n");
            return 1;
        }
    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
            fprintf(stderr, "%s\n",
                    qPrintable(QDBusConnection::systemBus().lastError().message()));
            return 1;
        }
    GrxDbus grx_dbus;
    grx_dbus.temporizador();
    QDBusConnection::systemBus().registerObject("/", &grx_dbus, QDBusConnection::ExportAllSlots);
    QDBusConnection::systemBus().registerObject("/senales", &grx_dbus,QDBusConnection::ExportAllSignals);
    return a.exec();
}
