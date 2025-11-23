/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include <concord/discord.h>
#include <concord/logmod.h>
#include "../events.h"
#include <stdio.h>
#include "../handlers/reaction_role.h"

void am_event_message_reaction_add_run(struct discord* client, const struct discord_message_reaction_add *event) {
  am_reaction_role_reaction_add_hook(client, event);
  logmod_log(INFO, NULL, "We've got a reaction %s (%s) from %"PRIu64" on message %"PRIu64, event->emoji->id, event->emoji->name, event->user_id, event->message_id);
}

void am_event_message_reaction_add_register(struct discord* client) {
  discord_set_on_message_reaction_add(client, &am_event_message_reaction_add_run);
}

const struct am_event am_event_message_reaction_add = {
  .name = "message_reaction_add",
  .reg = &am_event_message_reaction_add_register
};
