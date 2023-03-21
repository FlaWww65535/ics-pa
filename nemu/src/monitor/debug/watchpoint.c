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

WP* new_wp(char* e){
  
  WP* new = free_;
  if(new==NULL){
    Log("You can set %d Watchpoints at most\n",NR_WP);
    assert(0);
  }
  Log("%s",e);
  memcpy(new->expr,e,3);
  
  return NULL;
  bool success = true;
  new->val = expr(e,&success);
  assert(success==true);
  Log("new_wp\n");
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

void print_wp(WP *wp,bool isupdate){
  bool success = true;
  int val = expr(wp->expr,&success);
  assert(success==true);
  printf("Watchpoint %d: %s\nvalue = %d\n",wp->NO,wp->expr,val);
}
bool check_wp(){
  bool flag=false;
  for(WP* it = head; it; it= it->next){
    bool success = true;
    int val = expr(it->expr,&success);
    assert(success==true);
    if(val!=it->val){
      printf("Watchpoint %d: %s\nOld value = %d\nNew value = %d\n",it->NO,it->expr,it->val,val);
      it->val=val;
      flag=true;
    }
  }
  return flag;
}

