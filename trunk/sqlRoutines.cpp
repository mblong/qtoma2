#include <QtSql>
#include <QtWidgets>

#include "oma2UiIndependent/image.h"
#include "oma2UiIndependent/ImageBitmap.h"
#include "oma2UiIndependent/UI.h"
#include "qtoma2.h"

QSqlDatabase db;
int isMySql = 0;
char databaseName[128],tableName[128],server[64],user[64],password[64];

/*
SQLADD
    Adds the current displayed image to the database table previously opened. This
    could be a SQLite or mySQL database. [qtoma command]
*/

int sqlAdd_q(int n,char* args)
{
    /*if(!isMySql){
        if(!db.isOpen()){
            beep();
            printf("No SQL database is currently open.\n");
            return FILE_ERR;
        }
    }*/
    extern QtOma2* wPointer;
    extern int 	num_variables;
    extern Variable user_variables[];

    int windowNumber = wPointer->activeWindow();

    if(windowNumber<0){
        //can't do this
        beep();
        printf("No active data window.\n");
        return -1;
    }

    if(wPointer->fillInDataFromPixmap(db, tableName)){
        db.close();
        qDebug( "Can't insert Image!" );
        return -1;
    }

    QSqlQuery query = QSqlQuery( db );

    for(int i=0; i< num_variables; i++){
        if( strncmp("sql", user_variables[i].vname, 3) == 0){
            if(user_variables[i].is_float == -1){
                if(!isMySql){
                    query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                            QString(user_variables[i].estring) +"' WHERE imageId = last_insert_rowid()");
                } else {
                    query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                            QString(user_variables[i].estring) +"' WHERE imageId = LAST_INSERT_ID()");
                }
            } else if(user_variables[i].is_float){
                if(!isMySql){
                    query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                            QString::number(user_variables[i].fvalue) +"' WHERE imageId = last_insert_rowid()");
                } else { query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                            QString::number(user_variables[i].fvalue) +"' WHERE imageId = LAST_INSERT_ID()");
                }
            }else if(!isMySql){
                query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                        QString::number(user_variables[i].ivalue) +"' WHERE imageId = last_insert_rowid()");
            } else { query.exec("UPDATE "+QString(tableName)+" SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                        QString::number(user_variables[i].ivalue) +"' WHERE imageId = LAST_INSERT_ID()");
            }

        }
    }

    return NO_ERR;

}

/*
MYSQSERVER server userName password databaseName
    Opens a new connection to the named MYSQL database on the specified server (e.g., cld3.eng.yale.edu)
    with the userName and password specified. [qtoma command]
*/

int mySqServer_q(int n,char* args)
{
    int narg = sscanf(args, "%s %s %s %s", server,user,password,databaseName);
    if(narg !=4){
        beep();
        printf("Need four arguments: server user password database\n");
        return CMND_ERR;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase( "QMYSQL" );
    db.setHostName( QString(server) );
    db.setDatabaseName( QString(databaseName) );
    db.setUserName( QString(user) );
    db.setPassword( QString(password)  );
    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=10");

    if( !db.open() ){
        printf("Problem to connect to database %s at server %s\n",databaseName,server);
        qDebug() << "Error opening:\n" << db.lastError();
        return CMND_ERR;
    }
    isMySql = 1;
    printf("Connected to database %s at server %s\n",databaseName,server);
    return NO_ERR;

}

/*
MYSQTABLE tableName
    Creates a table named tableName on the open MYSQL server if it doesn't already exist. The table will contain a numbered
    imageID and imageData containing the image displayed in the current data window. In addition, any variable name
    beginning with "sql" will be added as a column to the table (minus the "sql"). [qtoma command]
*/
int mySqTable_q(int n,char* args){
    // Set up mySqlTable
    extern int 	num_variables;
    extern Variable user_variables[];
    QString queryText;

    if(!isMySql){
        beep();
        printf("No MySqlServer is open. Use the MYSQLSERVER command first.\n");
        return CMND_ERR;
    }

    sscanf(args, "%s", tableName);
    queryText = QString("CREATE TABLE IF NOT EXISTS "+QString(tableName)+" (imageId INTEGER PRIMARY KEY AUTO_INCREMENT, imageData BLOB");
    for(int i=NUM_COMMAND_RETURN_VARIABLES; i< num_variables; i++){
        if( strncmp("sql", user_variables[i].vname, 3) == 0){
            if(user_variables[i].is_float == -1 )
                queryText.append(", "+ QString(&user_variables[i].vname[3])+" VARCHAR(45)");
            else
                queryText.append(", "+ QString(&user_variables[i].vname[3])+" FLOAT");
        }
    }
    queryText.append(")");
    QSqlQuery query = QSqlQuery( db );
    if(!query.exec( queryText )){
        beep();
        printf("Error initializing SQL image table.\n");
        return -1;
    }


    query.exec( "SELECT * FROM "+QString(tableName));
    QSqlRecord columns = query.record();
    printf("Opening Table %s with the following columns: \n",tableName);  // what's below dosn't return anything for mysql
    for(int i=0; i<columns.count();i++){
        printf("%s\n", columns.fieldName(i).toUtf8().constData());
    }
    return NO_ERR;
}

/*
SQLNEW databaseName tableName
    Opens a new SQLite database file named databaseName.db and creates a table named tableName
    if it doesn't already exist. If databaseName.db already exists, it is deleted and a new
    database started. The table will contain a numbered imageID and imageData containing the image
    displayed in the current data window. In addition, any variable name
    beginning with "sql" will be added as a column to the table (minus the "sql"). [qtoma command]
*/

int sqlNew_q(int n,char* args)
{
    // Set up database
    extern int 	num_variables;
    extern Variable user_variables[];
    QString queryText;

    if(isMySql){
        beep();
        printf("Close the open mySQL database before defining a new SQLite database\n");
        return CMND_ERR;
    }
    int nargs = sscanf(args, "%s %s ", databaseName, tableName);
    if(nargs <2){
        beep();
        printf("Two arguments needed: SQLNEW databaseName tableName\n");
        return CMND_ERR;
    }
    QString dbName( QString(fullname(databaseName,SAVE_DATA_NO_SUFFIX)) + ".db");
    QFile::remove( dbName ); // delete sqlite file if it exists from a previous run
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbName );
    db.open();

    queryText = QString("CREATE TABLE IF NOT EXISTS "+QString(tableName)+
                        " (imageId integer primary key autoincrement, imageData BLOB");
    for(int i=NUM_COMMAND_RETURN_VARIABLES; i< num_variables; i++){
        if( strncmp("sql", user_variables[i].vname, 3) == 0){
            queryText.append(", "+ QString(&user_variables[i].vname[3])+" FLOAT");
        }
    }
    queryText.append(")");

    QSqlQuery query = QSqlQuery( db );
    if(!query.exec( queryText )){
        beep();
        printf("Error initializing SQL image database.\n");
        return -1;
    }

    QSqlRecord columns = db.record(QString(tableName));
    printf("Opening %s.db with the following added columns: \n",args);
    for(int i=0; i<columns.count();i++){
        printf("%s\n", columns.fieldName(i).toUtf8().constData());
    }

    return NO_ERR;
}

/*
SQLOPEN databaseName tableName
    Opens an existing SQLite database file named databaseName.db and creates a table named tableName
    if it doesn't already exist. The table will contain a numbered imageID and imageData containing the image
    displayed in the current data window. In addition, any variable name
    beginning with "sql" will be added as a column to the table (minus the "sql"). [qtoma command]
*/
int sqlOpen_q(int n,char* args){

    // Set up database
    extern int 	num_variables;
    extern Variable user_variables[];

    int nargs = sscanf(args, "%s %s ", databaseName, tableName);
    if(nargs <2){
        beep();
        printf("Two arguments needed: SQLOPEN databaseName tableName\n");
        return CMND_ERR;
    }

    QString dbName( QString(fullname(databaseName,GET_DATA_NO_SUFFIX)) + ".db");
    if(!QFile::exists( dbName )){
        beep();
        printf("No such file: %s.\n",args);
        return FILE_ERR;
    }

    db.close();
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbName );
    if(!db.open()){
        beep();
        printf("Error opening database.\n");
        return FILE_ERR;
    }

    QString queryText = QString("CREATE TABLE IF NOT EXISTS "+QString(tableName)+
                                " (imageId integer primary key autoincrement, imageData BLOB");
    for(int i=NUM_COMMAND_RETURN_VARIABLES; i< num_variables; i++){
        if( strncmp("sql", user_variables[i].vname, 3) == 0){
            queryText.append(", "+ QString(&user_variables[i].vname[3])+" TEXT");
        }
    }
    queryText.append(")");

    QSqlQuery query = QSqlQuery( db );
    if(!query.exec( queryText )){
        beep();
        printf("Error initializing SQL image table.\n");
        return -1;
    }

    QSqlRecord columns = db.record(QString(tableName));
    printf("Opening %s.db with the following columns: \n",args);
    for(int i=0; i<columns.count();i++){
        printf("%s\n", columns.fieldName(i).toUtf8().constData());
    }
    isMySql = 0;
    return NO_ERR;

}

/*

SQLCLOSE
    Closes any open SQL database. [qtoma command]
 */

int sqlClose_q(int n,char* args){
    db.close();
    isMySql = 0;
    return NO_ERR;
}
