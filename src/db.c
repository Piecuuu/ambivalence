/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include "db.h"
#include <stddef.h>
#include <concord/logmod.h>

sqlite3* am_db = NULL;

int am_db_init(const char* db_path) {
  int rc = sqlite3_open(db_path, &am_db);
  if(rc != SQLITE_OK) {
    logmod_log(ERROR, NULL, "Failed to open database: %s", sqlite3_errmsg(am_db));
    sqlite3_close(am_db);
    return 1;
  }
  logmod_log(INFO, NULL, "Succesfully opened database");
  return 0;
}
