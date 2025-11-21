/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include <concord/discord.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "events.h"

int main() {
  const char* token = getenv("BOT_TOKEN");
  if(!token || strlen(token) == 0) {
    fprintf(stderr, "BOT_TOKEN is missing.\n");
    return 1;
  }

  struct discord* client = discord_from_token(token);
  am_events_register_all(client);
  discord_run(client);
  return 0;
}
