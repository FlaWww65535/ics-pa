#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;



void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char* expr){
  WP* new = free_;
  if(new==NULL){
    Log("You can set %d Watchpoints at most\n",NR_WP);
    assert(0);
  }
  free_ = new->next;
  new->next = head;
  head = new;
}

void free_wp(WP* wp){
  for(WP* it = head; it; it= it->next){
    if(it->next == wp){
      it->next =wp->next;
      wp->next=free_;
      free_ =wp;
    }
  }
  Log("Failed to free watchpoint\n");
  assert(0);
}


