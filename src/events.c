/*
 * This code is licensed under the European Union Public Licence, Version 1.2
 * See LICENSE in the root of the repository for details.
 */

#include "events.h"

#define am_array_length(array) (sizeof(array) / sizeof(array[0]))

extern const struct am_event am_event_ready;
extern const struct am_event am_event_message_reaction_add;

const struct am_event* events_list[] = {
  &am_event_ready, &am_event_message_reaction_add,
};

void am_events_register_all(struct discord* client) {
  for(int i = 0; i < am_array_length(events_list); i++) {
    events_list[i]->reg(client);
  }
}

