/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#ifndef AM_DB_H
#define AM_DB_H

#include <sqlite3.h>

extern sqlite3* am_db;

int am_db_init(const char* db_path);

#endif // AM_DB_H
