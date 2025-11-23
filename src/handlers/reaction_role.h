/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#ifndef AM_REACTION_ROLE_H
#define AM_REACTION_ROLE_H

#include <sqlite3.h>
#include <concord/discord.h>

struct am_reaction_role {
  sqlite3_int64 oid; // set to -1 if null
  u64snowflake emoji_id;
  unsigned char emoji[8];
  u64snowflake message_id;
  u64snowflake channel_id;
  u64snowflake guild_id;
  u64snowflake role_id;
};

int am_reaction_role_reaction_add_hook(struct discord* client, const struct discord_message_reaction_add *event);
int am_reaction_role_reaction_remove_hook(struct discord* client, const struct discord_message_reaction_remove *event);
struct am_reaction_role* am_reaction_role_get_from_message_and_emoji(const u64snowflake message_id, const char* emoji, const u64snowflake emoji_id);
int am_reaction_role_add_reaction_role(const struct am_reaction_role* reaction_role);
int am_reaction_role_remove_reaction_role(const struct am_reaction_role* reaction_role);

#endif // AM_REACTION_ROLE_H