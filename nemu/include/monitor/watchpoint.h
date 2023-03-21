#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#include "monitor/expr.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char* expr;
  int32_t val;


} WP;

WP* new_wp(char* e);
void free_wp(WP *wp);
void print_wp(WP *wp,bool isupdate);
bool check_wp();

#endif
