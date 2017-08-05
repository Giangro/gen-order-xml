/*
 * File:   main.cpp
 * Author: Alex
 *
 * Created on 04 agosto 2017, 15.10
 */

#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <time.h>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp> // for 'list_of()'

#include "database.h"
#include "simplelog.h"
#include "utils.h"

using namespace boost::assign; // bring 'list_of()' into scope

/*
 * Prototypes
 */

int parseArgs(int argc, char **argv);
void usage(const char *message);
void showAll();

/*
 * Global
 */

const char *logFilename       = NULL;
const char *databaseFilename  = NULL;
const char *xmlFilename       = NULL;

/*
 *  Main
 */
int main(int argc, char **argv) {

  if((parseArgs(argc, argv) != 0) || (logFilename == NULL) || (xmlFilename == NULL)) {
    usage("genordxml [-l log file name] [-d database file name] [-f xml template file]\n");
    return EXIT_FAILURE;
  }

  LOGINIT(logFilename, MAX_LOG_SIZE, MAX_NUM_LOG, (_LOG_DEBUG | _LOG_INFO | _LOG_WARNING | _LOG_ERROR));

  LOGWRITE(NULL, (_LOG_INFO, "database file name = <%s>", databaseFilename));
  LOGWRITE(NULL, (_LOG_INFO, "xml file name      = <%s>", xmlFilename));

  if(DataBase::instance()->openDatabase(databaseFilename) == -1) {
    LOGWRITE(NULL, (_LOG_ERROR, "Error while opening database %s. Exiting.", databaseFilename));
    return EXIT_FAILURE;
  }

  unsigned int count = 0;

  DataBase::instance()->executeSql("SELECT COUNT(*) FROM NELIST"
         , [] (void *data, int argc, char **argv, char **azColName) {
           if (argv[0]) {
             unsigned int *count = reinterpret_cast<unsigned int*> (data);
             *count = lexical_cast<int>(argv[0]);
           }
          return 0;
  }, (void*) &count);

  LOGWRITE(NULL, (_LOG_INFO, "ne list count is = %d",count));

  DataBase::instance()->closeDatabase();

  LOGWRITE(NULL, (_LOG_INFO, "All done. Exiting."));
  return EXIT_SUCCESS;
}

int parseArgs(int argc, char **argv) {
  int c;
  while((c = getopt(argc, argv, "l:d:f:")) != -1) {
    switch(c) {
    case 'l':
      logFilename = optarg;
      break;
    case 'd':
      databaseFilename = optarg;
      break;
    case 'f':
      xmlFilename = optarg;
      break;
    case '?':
      if((optopt == 'l') || (optopt == 'd') || (optopt == 'f')) {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      } else if(isprint(optopt)) {
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf(stderr,
                "Unknown option character `\\x%x'.\n",
                optopt);
      }
      return 1;
    default:
      abort();
    }
  }
  opterr = 0;
  return 0;
}

void usage(const char *message) {
  fprintf(stderr, "usage: %s", message);
}

void showAll() {
  DataBase::instance()->executeSql("SELECT * FROM NELIST"
         , [] (void *data, int argc, char **argv, char **azColName) {

          return 0;
  }, (void*) 0l);
}
