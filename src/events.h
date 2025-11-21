#ifndef EVENTS_H
#define EVENTS_H

#include <concord/discord.h>

struct am_event {
  const char* name;
  void (*reg)(struct discord* client);
};

void am_events_register_all(struct discord* client);

#endif // EVENTS_H
