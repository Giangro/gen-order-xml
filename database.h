/*
 * File:   database.h
 * Author: Alex
 *
 * Created on 28 novembre 2015, 15.49
 */

#ifndef DATABASE_H
#define	DATABASE_H

#include <string>
#include <sqlite3.h>
#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;

class DataBase {
private:
    DataBase(const DataBase& orig);
    DataBase();
public:
    typedef int (database_cb) (void *, int , char **, char **);
public:
    static DataBase * instance();
    virtual ~DataBase();
public:
    int openDatabase(const string &databasename);
    int closeDatabase();
    int executeSql(const string &sql, database_cb callback, void *data);
private:
    sqlite3 *db_;
    string dbname_;
private:
    static boost::atomic<DataBase *> instance_;
    static boost::mutex instantiation_mutex;
};

#endif	/* DATABASE_H */
