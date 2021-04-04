#pragma once

extern struct _level_node;
extern struct _user;
extern enum e_errorCode;

void bookmarkOpening(struct _user*, e_errorCode* errorCode);

struct _level_node* bookmarkGenerator(struct _level_node*, int*, e_errorCode* errorCode);

void bookmarkMain(int*, struct _level_node*, struct _user*, e_errorCode* errorCode);

void gameOver(struct _level_node*);