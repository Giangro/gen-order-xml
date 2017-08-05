/*
 * File:   database.cpp
 * Author: Alex
 *
 * Created on 28 novembre 2015, 15.49
 */

#include <cstdlib>
#include <stdio.h>

#include "simplelog.h"
#include "database.h"

using namespace std;

boost::atomic<DataBase *> DataBase::instance_(0);
boost::mutex DataBase::instantiation_mutex;

DataBase* DataBase::instance() {
    DataBase * tmp = instance_.load(boost::memory_order_consume);
    if (!tmp) {
        boost::mutex::scoped_lock guard(instantiation_mutex);
        tmp = instance_.load(boost::memory_order_consume);
        if (!tmp) {
            tmp = new DataBase();
            instance_.store(tmp, boost::memory_order_release);
        }
    }
    return tmp;
}

DataBase::DataBase()
: db_(0)
, dbname_() {
}

DataBase::DataBase(const DataBase& orig) {
    // nothing to do
}

DataBase::~DataBase() {
    if (this->db_)
        (void)closeDatabase();
}

int DataBase::openDatabase(const string &databasename) {

    if (this->db_)
        closeDatabase();

    int rc = sqlite3_open(databasename.c_str(), &this->db_);
    if (rc) {
        LOGWRITE( NULL, (_LOG_ERROR, "Can't open database %s: %s", databasename.c_str(), sqlite3_errmsg(this->db_)));
        return -1;
    } else {
        LOGWRITE ( NULL, (_LOG_INFO, "Database \"%s\" opened successfully", databasename.c_str()));
        dbname_ = databasename;
        return 0;
    }
}

int DataBase::closeDatabase() {
    int rc = sqlite3_close(this->db_);
    db_ = 0;
    if (rc) {
        LOGWRITE( NULL, (_LOG_ERROR, "Can't close database %s: %s", dbname_.c_str(), sqlite3_errmsg(this->db_)));
        return -1;
    } else {
        LOGWRITE ( NULL, (_LOG_INFO, "Database \"%s\" closed successfully", dbname_.c_str()));
        return 0;
    }
}

int DataBase::executeSql(const string &sql, DataBase::database_cb callback,void *data) {
    char *zErrMsg = 0;
    /* Execute SQL statement */
    LOGWRITE( NULL, (_LOG_DEBUG, "Executing sql:\n\t%s",sql.c_str()));
    int rc = sqlite3_exec(this->db_, sql.c_str(), callback, (void *) data, &zErrMsg);
    if (rc != SQLITE_OK) {
        LOGWRITE( NULL, (_LOG_ERROR, "SQL error: %s", zErrMsg));
        sqlite3_free(zErrMsg);
        return -1;
    } else {
        LOGWRITE ( NULL, (_LOG_DEBUG, "Operation done successfully."));
        return 0;
    }
}
