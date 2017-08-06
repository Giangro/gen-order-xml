/*
 * File:   nelist.h
 * Author: Alex
 *
 * Created on 6 agosto 2017, 17.57
 */

 #ifndef NELIST_H
 #define	NELIST_H

 #include <string>
 #include <sqlite3.h>
 #include <boost/atomic.hpp>
 #include <boost/thread/mutex.hpp>

 using namespace std;

 class NeList {
 private:
     NeList(const NeList& orig);
     NeList();
 public:
     static NeList * instance();
     virtual ~NeList();
 public:
     int openNeList(const string &NeListname);
     int closeNeList();
 private:
     int executeSql(const string &sql, NeList_cb callback, void *data);
 private:
      // add private member here
 private:
     static boost::atomic<NeList *> instance_;
     static boost::mutex instantiation_mutex;
 };

 #endif	/* NELIST_H */
