#include "db.h"
#include <concord/logmod.h>

sqlite3* am_db = NULL;

int am_db_init(const char* db_path) {
  int rc = sqlite3_open(db_path, &am_db);
  if(rc != SQLITE_OK) {
    logmod_log(ERROR, NULL, "Failed to open database: %s\n", sqlite3_errmsg(am_db));
    sqlite3_close(am_db);
    return 1;
  }
  return 0;
}
