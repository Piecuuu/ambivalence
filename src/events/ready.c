#include <concord/logmod.h>
#include <concord/discord.h>
#include "../events.h"
#include <stdio.h>

void am_event_ready_run(struct discord* client, const struct discord_ready* event) {
  logmod_log(INFO, NULL, "%s#%s %"PRIu64, event->user->username, event->user->discriminator, event->user->id);
}

void am_event_ready_register(struct discord* client) {
  discord_set_on_ready(client, &am_event_ready_run);
}

const struct am_event am_event_ready = {
  .name = "ready",
  .reg = &am_event_ready_register
};
