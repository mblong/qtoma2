#include <QtSql>
#include <QtWidgets>

#include "oma2UiIndependent/image.h"
#include "oma2UiIndependent/ImageBitmap.h"
#include "oma2UiIndependent/UI.h"
#include "qtoma2.h"

QSqlDatabase db;

int sqlAdd_q(int n,char* args)
{

    if(!db.isOpen()){
        beep();
        printf("No SQL database is currently open.\n");
        return FILE_ERR;
    }


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

    if(wPointer->fillInDataFromPixmap(db)){
        db.close();

        return -1;
    }

    QSqlQuery query = QSqlQuery( db );
    for(int i=0; i< num_variables; i++){
        if( strncmp("sql", user_variables[i].vname, 3) == 0){
            if(user_variables[i].is_float == -1){
                query.exec("UPDATE imgTable SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                        QString(user_variables[i].estring) +"' WHERE imageId = last_insert_rowid()");
            } else if(user_variables[i].is_float){
                query.exec("UPDATE imgTable SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                        QString::number(user_variables[i].fvalue) +"' WHERE imageId = last_insert_rowid()");

            }else {
                query.exec("UPDATE imgTable SET "+QString(QString(&user_variables[i].vname[3])) +" = '" +
                        QString::number(user_variables[i].ivalue) +"' WHERE imageId = last_insert_rowid()");
            }
        }
    }

    return NO_ERR;

}

int sqlNew_q(int n,char* args)
{
    // Set up database
    extern int 	num_variables;
    extern Variable user_variables[];


    QString dbName( QString(fullname(args,SAVE_DATA_NO_SUFFIX)) + ".db");
    QFile::remove( dbName ); // delete sqlite file if it exists from a previous run
    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbName );
    db.open();

    QString queryText = QString("CREATE TABLE IF NOT EXISTS imgTable (imageId integer primary key autoincrement, imageData BLOB");
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
    QSqlRecord columns = db.record("imgTable");
    printf("Opening %s.db with the following columns: \n",args);
    for(int i=0; i<columns.count();i++){
        printf("%s\n", columns.fieldName(i).toUtf8().constData());
    }

    return NO_ERR;
}

int sqlOpen_q(int n,char* args){

    // Set up database
    extern int 	num_variables;
    extern Variable user_variables[];

    QString dbName( QString(fullname(args,GET_DATA_NO_SUFFIX)) + ".db");
    if(!QFile::exists( dbName )){
        beep();
        printf("No such file: %s.\n",args);
        return FILE_ERR;
    }

    db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( dbName );
    if(!db.open()){
        beep();
        printf("Error opening database.\n");
        return FILE_ERR;
    }

    /*
        QString queryText = QString("CREATE TABLE IF NOT EXISTS imgTable (imageId integer primary key autoincrement, imageData BLOB");
        for(int i=NUM_COMMAND_RETURN_VARIABLES; i< num_variables; i++){
            if( strncmp("sql", user_variables[i].vname, 3) == 0){
                queryText.append(", "+ QString(&user_variables[i].vname[3])+" TEXT");
            }
        }
        queryText.append(")");

        QSqlQuery query = QSqlQuery( db );
        if(!query.exec( queryText )){
            beep();
            printf("Error initializing SQL image database.\n");
            return -1;
        }
        */
    QSqlRecord columns = db.record("imgTable");
    printf("Opening %s.db with the following columns: \n",args);
    for(int i=0; i<columns.count();i++){
        printf("%s\n", columns.fieldName(i).toUtf8().constData());
    }

    return NO_ERR;

}

int sqlClose_q(int n,char* args){
    db.close();
    return NO_ERR;
}
