CREATE TABLE am_reaction_roles (
  emoji_id INTEGER,
  emoji TEXT,
  message_id INTEGER NOT NULL,
  channel_id INTEGER NOT NULL,
  guild_id INTEGER NOT NULL,
  role_id INTEGER NOT NULL
);