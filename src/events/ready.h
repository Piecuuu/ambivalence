#ifndef EVENT_READY_H
#define EVENT_READY_H

#include <concord/discord.h>
#include "../events.h"

void am_event_ready_register(struct discord* client);

extern const struct am_event am_event_ready;

#endif // EVENT_READY_H
