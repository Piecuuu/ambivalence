/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#ifndef AM_EVENTS_H
#define AM_EVENTS_H

#include <concord/discord.h>

struct am_event {
  const char* name;
  void (*reg)(struct discord* client);
};

void am_events_register_all(struct discord* client);

#endif // AM_EVENTS_H
