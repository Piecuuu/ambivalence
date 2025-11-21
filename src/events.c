#include "events.h"

#include "events/ready.h"

#define am_array_length(array) (sizeof(array) / sizeof(array[0]))

const struct am_event* events_list[] = {
  &am_event_ready,
};

void am_events_register_all(struct discord* client) {
  for(int i = 0; i < am_array_length(events_list); i++) {
    events_list[i]->reg(client);
  }
}

