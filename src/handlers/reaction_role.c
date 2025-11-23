/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include "reaction_role.h"
#include <concord/logmod.h>
#include "../db.h"
#include <string.h>
#include <stdlib.h>

static int check_fail_bind_param(int rc, sqlite3_stmt* stmt) {
  if(rc != SQLITE_OK) {
    logmod_log(ERROR, NULL, "Failed to bind params rc:%d, %s", rc, sqlite3_errmsg(am_db));
    sqlite3_finalize(stmt);
    return 1;
  }
  return 0;
}

static int check_fail_prepared_stmt(int rc, sqlite3_stmt* stmt) {
  if(rc != SQLITE_OK) {
    logmod_log(ERROR, NULL, "Failed to prepare stmt rc:%d, %s", rc, sqlite3_errmsg(am_db));
    sqlite3_finalize(stmt);
    return 1;
  }
  return 0;
}

static int check_fail_step(int rc, sqlite3_stmt* stmt) {
  if(rc != SQLITE_DONE) {
    _logmod_log_permissive((rc == SQLITE_MISUSE || rc == SQLITE_ERROR) ? LOGMOD_LEVEL_ERROR : LOGMOD_LEVEL_WARN, NULL, __LINE__, __FILE__, "Failed to step rc:%d, %s", rc, sqlite3_errmsg(am_db), "");
    sqlite3_finalize(stmt);
    return 1;
  }
  return 0;
}

static _Bool check_if_custom_emoji(const char* emoji, const u64snowflake emoji_id) {
  return emoji == NULL && emoji_id != 0;
}

struct am_reaction_role* am_reaction_role_get_from_message_and_emoji(const u64snowflake message_id, const char* emoji, const u64snowflake emoji_id) {
  _Bool is_custom_emoji = check_if_custom_emoji(emoji, emoji_id);

  char sql_query[512];
  is_custom_emoji ?
    strncpy(sql_query, "SELECT rowid, * FROM am_reaction_roles WHERE message_id = ? AND emoji_id = ?", sizeof(sql_query)) :
    strncpy(sql_query, "SELECT rowid, * FROM am_reaction_roles WHERE message_id = ? AND emoji = ?", sizeof(sql_query));

  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(am_db, sql_query, -1, &stmt, 0);
  if(check_fail_prepared_stmt(rc, stmt)) return NULL;

  rc = sqlite3_bind_int64(stmt, 1, message_id);
  if(check_fail_bind_param(rc, stmt)) return NULL;
  rc = is_custom_emoji ?
    sqlite3_bind_int64(stmt, 2, emoji_id) :
    sqlite3_bind_text(stmt, 2, emoji, -1, SQLITE_STATIC);
  if(check_fail_bind_param(rc, stmt)) return NULL;

  struct am_reaction_role* reaction_role = calloc(1, sizeof(struct am_reaction_role));

  rc = sqlite3_step(stmt);
  if(rc == SQLITE_ROW) {
    do {
      reaction_role->oid = sqlite3_column_int64(stmt, 0);
      reaction_role->emoji_id = sqlite3_column_int64(stmt, 1);
      const unsigned char *db_emoji = sqlite3_column_text(stmt, 2);
      if(db_emoji)
        snprintf(reaction_role->emoji, sizeof reaction_role->emoji, "%s", db_emoji);
      else
        reaction_role->emoji[0] = '\0';
      reaction_role->message_id = sqlite3_column_int64(stmt, 3);
      reaction_role->channel_id = sqlite3_column_int64(stmt, 4);
      reaction_role->guild_id = sqlite3_column_int64(stmt, 5);
      reaction_role->role_id = sqlite3_column_int64(stmt, 6);
    } while ((rc = sqlite3_step(stmt)) == SQLITE_ROW);
  } else if (rc == SQLITE_DONE) {
    free(reaction_role);
    sqlite3_finalize(stmt);
    return NULL;
  }
  if(check_fail_step(rc, stmt)) {
    free(reaction_role);
    return NULL;
  }

  sqlite3_finalize(stmt);
  return reaction_role;
}

int am_reaction_role_add_reaction_role(const struct am_reaction_role* reaction_role) {
  sqlite3_stmt* stmt;

  const char* sql_query = "INSERT INTO am_reaction_roles "
                          "(emoji_id,emoji,message_id,channel_id,guild_id,role_id) "
                          "VALUES (?,?,?,?,?,?)";

  int rc = sqlite3_prepare_v2(am_db, sql_query, -1, &stmt, 0);
  if(check_fail_prepared_stmt(rc, stmt)) return 1;

  rc = sqlite3_bind_int64(stmt, 1, reaction_role->emoji_id);
  if(check_fail_bind_param(rc, stmt)) return 1;
  rc = sqlite3_bind_text(stmt, 2, reaction_role->emoji, -1, SQLITE_STATIC);
  if(check_fail_bind_param(rc, stmt)) return 1;
  rc = sqlite3_bind_int64(stmt, 3, reaction_role->message_id);
  if(check_fail_bind_param(rc, stmt)) return 1;
  rc = sqlite3_bind_int64(stmt, 4, reaction_role->channel_id);
  if(check_fail_bind_param(rc, stmt)) return 1;
  rc = sqlite3_bind_int64(stmt, 5, reaction_role->guild_id);
  if(check_fail_bind_param(rc, stmt)) return 1;
  rc = sqlite3_bind_int64(stmt, 6, reaction_role->role_id);
  if(check_fail_bind_param(rc, stmt)) return 1;

  rc = sqlite3_step(stmt);
  if(check_fail_step(rc, stmt)) return 1;

  return 0;
}

int am_reaction_role_reaction_add_hook(struct discord* client, const struct discord_message_reaction_add *event) {
  if(!event->guild_id) return 1;
  _Bool is_custom_emoji = check_if_custom_emoji(event->emoji->name, event->emoji->id);
  struct am_reaction_role* reaction_role = am_reaction_role_get_from_message_and_emoji(event->message_id, event->emoji->name, event->emoji->id);
  if(reaction_role == NULL) return 1;

  struct discord_guild_member member;
  struct discord_ret_guild_member member_ret = { .sync = &member };
  CCORDcode code = discord_get_guild_member(client, event->guild_id, event->user_id, &member_ret);
  if(code != CCORD_OK) {
    logmod_log(ERROR, NULL, "Couldn't fetch member %"PRIu64" for %"PRIu64", code: %d", event->user_id, event->guild_id, code);
    free(reaction_role);
    return 1;
  }

  for(int i = 0; i < member.roles->size; i++) {
    if(member.roles->array[i] == reaction_role->role_id) return 0;
  }

  struct discord_add_guild_member_role role_params = {
    .reason = "Reaction role"
  };

  code = discord_add_guild_member_role(client, event->guild_id, event->user_id, reaction_role->role_id, &role_params, NULL);
  if(code != CCORD_OK) {
    logmod_log(ERROR, NULL, "Couldn't give role to member %"PRIu64" on %"PRIu64" (reaction roles), code: %d", event->user_id, event->guild_id, code);
    free(reaction_role);
    return 1;
  }

  free(reaction_role);
  return 0;
}