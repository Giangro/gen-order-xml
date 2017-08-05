/*
 * File:   utils.h
 * Author: Alex
 *
 * Created on 04 agosto 2017, 15.13
 */

#ifndef UTILS_H
#define UTILS_H

#include <boost/lexical_cast.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <string>
#include <vector>

using namespace boost;
using namespace boost::accumulators;
using namespace std;

/*
 * Defines
 */

#define MAX_LOG_SIZE 100000000
#define MAX_NUM_LOG 5

/*
 *  Global Vars
 */

extern const char *logFilename;
extern const char *databaseFilename;
extern const char *xmlFilename;
extern char line[LINE_MAX + 1];

/*
 *  Data Types
 */


/*
 *  Prototypes
 */

#endif  /* UTILS_H */
