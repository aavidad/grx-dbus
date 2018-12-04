#ifndef GRX_DBUS
#define GRX_DBUS
#define SERVICE_NAME    "grx.dbus"
#define VELETA "10.1.1.185"
#define BASE_DATOS "/var/lib/grx/grx.sqlite"
#define ARP_SCAN "/usr/local/bin/arp-scan -onliip -G 187 -t50 2> /dev/null "
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QObject>
#include "QtDebug"
#include <QDBusVariant>
#include <QSqlRecord>
#include <QDBusMessage>

class GrxDbus: public QObject
{
    Q_OBJECT
public slots:
    Q_SCRIPTABLE QString busca_router(const QString &ip);
    Q_SCRIPTABLE QString busca_routers_muestra_nombres(const QString &ips);
    Q_SCRIPTABLE QList<QVariant> ip_nodos();
    Q_SCRIPTABLE QString ip_nodos_string();
    Q_SCRIPTABLE QList<QVariant> consulta_sql_columna(const QString &tabla,int columna);
    Q_SCRIPTABLE QList<QVariant> consulta_sql_todo(const QString &arg);
    Q_SCRIPTABLE QString busca_todos_routers();
    Q_SCRIPTABLE QString consulta_sql_nombre(const QString &ip);
    Q_SCRIPTABLE QString consulta_sql_ip (const QString &nombre);
    Q_SCRIPTABLE QString busca_nodo_por_ip(const QString &ip);
    Q_SCRIPTABLE QString busca_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_nombre(const QString &nodo);
    Q_SCRIPTABLE bool esta_nodo_por_ip(const QString &ip);
    Q_SCRIPTABLE QString crea_conexion (const QString &conexion);
    Q_SCRIPTABLE QString busca_todos_routers_nombre();
    Q_SCRIPTABLE bool esta_veleta();
    Q_SCRIPTABLE bool esta_repositorio();
    Q_SCRIPTABLE bool esta_mysqlDB();
    Q_SCRIPTABLE void temporizador();
    Q_SCRIPTABLE void mensaje(const QString &mensaje);

signals:

   void senal(const QString &);

private:
    QSqlDatabase db;
    QSqlQuery consultar;
};

static bool crea_conexionSQLite()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(BASE_DATOS);
    if (!db.open()) {
       qDebug() << ("No puedo abrir la base de datos\n");
       return false;
    }

   return true;
}



#endif
