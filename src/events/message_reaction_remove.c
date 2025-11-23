/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include <concord/discord.h>
#include <concord/logmod.h>
#include "../events.h"
#include <stdio.h>
#include "../handlers/reaction_role.h"

void am_event_message_reaction_remove_run(struct discord* client, const struct discord_message_reaction_remove *event) {
  am_reaction_role_reaction_remove_hook(client, event);
  logmod_log(INFO, NULL, "%"PRIu64" has removed his reaction %s (%s) on message %"PRIu64, event->user_id, event->emoji->id, event->emoji->name, event->message_id);
}

void am_event_message_reaction_remove_register(struct discord* client) {
  discord_set_on_message_reaction_remove(client, &am_event_message_reaction_remove_run);
}

const struct am_event am_event_message_reaction_remove = {
  .name = "message_reaction_remove",
  .reg = &am_event_message_reaction_remove_register
};
