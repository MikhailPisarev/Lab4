#include "doglistsql.h"
#include "QObject"

doglistsql::doglistsql(QObject *parent) :
    QSqlQueryModel(parent)
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./DogDB.db");


     _isConnectionOpen = true;

    if(!db.open())
    {
        qDebug() << db.lastError().text();
        _isConnectionOpen = false;
    }


    QSqlQuery qry(db);
    qry.prepare("CREATE TABLE IF NOT EXISTS DogList (Id INTEGER UNIQUE PRIMARY KEY AUTOINCREMENT, nameOfDog text, breedOfDog text, ageOfDog text,fioOfDog text, Id_dog integer)");
    if( !qry.exec() )
    {
        qDebug() << db.lastError().text();
        _isConnectionOpen = false;
    }

    refresh();
}

QSqlQueryModel* doglistsql::getModel(){
    return this;
}
bool doglistsql::isConnectionOpen(){
    return _isConnectionOpen;
}
QHash<int, QByteArray> doglistsql::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "nameOfDog";
    roles[Qt::UserRole + 2] = "breedOfDog";
    roles[Qt::UserRole + 3] = "ageOfDog";
    roles[Qt::UserRole + 4] = "fioOfDog";
    roles[Qt::UserRole + 6] = "Id";
    return roles;
}


QVariant doglistsql::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole-1)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

const char* doglistsql::SQL_SELECT =
        "SELECT nameOfDog,breedOfDog,ageOfDog,fioOfDog, Id"
        " FROM DogList";

void doglistsql::refresh()
{
    this->setQuery(doglistsql::SQL_SELECT,db);
}

void doglistsql::add(const QString& nameRiv, const QString& breedRiv, const QString& ageRiv, const QString& fioRiv){

    QSqlQuery query(db);
    QString phQuery= QString("INSERT INTO DogList (nameOfDog,breedOfDog,ageOfDog,fioOfDog) VALUES ('%1', '%2', '%3', '%4')")
            .arg(nameRiv)
            .arg(breedRiv)
            .arg(ageRiv)
            .arg(fioRiv);
    query.exec(phQuery);

    refresh();

}

void doglistsql::edit(const QString& nameRiv, const QString& breedRiv, const QString& ageRiv, const QString& fioRiv, const int index){

    QSqlQuery query(db);
    QString phQuery= QString("UPDATE DogList SET nameOfDog = '%1',breedOfDog = '%2',ageOfDog = %3, fioOfDog = %4, WHERE Id = %5")
            .arg(nameRiv)
            .arg(breedRiv)
            .arg(ageRiv)
            .arg(fioRiv)
            .arg(index);
    query.exec(phQuery);

    refresh();

}
void doglistsql::del(const int Id){


    QSqlQuery query(db);
    QString strQuery= QString("DELETE FROM DogList WHERE Id = %1")
            .arg(Id);
    query.exec(strQuery);

    refresh();
}

QString doglistsql::count(const QString& textCount){

    QSqlQuery query(db);
    QString phQuery= QString("SELECT COUNT (Id) FROM DogList WHERE circulation <= %1")
            .arg(textCount.toInt());
    query.exec(phQuery);

    QString info;
    while(query.next())
    {
        info = query.value(0).toString();

    }
    return(info);

}
