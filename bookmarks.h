#pragma once
#include "mob.h"

extern struct _user;
extern struct _error;
extern enum e_errorCode;
extern struct _settings;

void bookmarkOpening(struct _user*, struct _error*, struct _settings* gameSet);
bool bookmarkSaveSelect(struct _user*, struct _error*, const struct _settings* gameSet);
void bookmarkSetting(struct _error*, struct _settings* gameSet);
void bookmarkMain(struct _user*, struct _error*, struct _settings* gameSet);
bool nextLevelMain(const struct _settings* gameSet);