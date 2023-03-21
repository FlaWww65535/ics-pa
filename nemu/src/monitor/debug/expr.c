#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include<ctype.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,TK_NEQ,
  TK_INT,TK_HEX,TK_REG,
  TK_DEREF,TK_UNARY,

  /* TODO: Add more token types */

};


static struct rule {
  char *regex;
  int token_type;
  int level;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +",    TK_NOTYPE,0},       // spaces
  {"\\$[a-zA-Z]+",TK_REG,0},    //REGISTER
  {"\\b0(X|x)[0-9a-fA-F]+\\b",TK_HEX,0},//HEX NUMBER
  {"\\b[0-9]+\\b",TK_INT,0},    //INTEGER
  {"==",    TK_EQ,  4},         // equal
  {"!=",    TK_NEQ, 4},         // not-equal
  {"\\+",   '+',    3},         // plus
  {"-",     '-',    3},         // minus
  {"\\*",   '*',    2},         // multiple
  {"/",     '/',    2},         // divide
  {"\\(",   '(',    1},         // left-bracket
  {"\\)",   ')',    1},         // right-bracket


};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
  int level;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          default: 
          {
            assert(substr_len<32);
            assert(nr_token<32);
            strncpy(tokens[nr_token].str,substr_start,substr_len);
            tokens[nr_token].str[substr_len]='\0';
            tokens[nr_token].type = rules[i].token_type;
            tokens[nr_token].level = rules[i].level;
            nr_token++;
          } 
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p,int q){
  if(p>=q)return false;
  if(!(tokens[p].type=='('&&tokens[q].type==')'))return false;
  int stack=0;
  for(int i=p;i<=q;i++){
    if(tokens[i].type=='('){stack++;}
    if(tokens[i].type==')'){stack--;}
    if(stack<0)return false;
  }
  return stack == 0;
}

int eval(int p,int q){
  if(p>q)
  {
    return 0;
  }else if(p==q){
      //TODO register pointor
    switch(tokens[p].type){
    case TK_INT:
      return atoi(tokens[p].str);
    case TK_HEX:
    {
      int addr;
      sscanf(tokens[p].str,"%x",&addr );
      return addr;
    }
    case TK_REG:
      if(strcmp(tokens[p].str+1,"eip") == 0){
        return cpu.eip;
      }
      for(int i=0;i<8;i++){
        if(strcmp(tokens[p].str+1,reg_name(i,4)) == 0){
          return reg_l(i);
        }
      }
    }

    return atoi(tokens[p].str);

  }else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1);
  }else{

    int domain=p;
    int level =tokens[p].level;
    if(level>=2){
      switch(tokens[p].type){
        case TK_DEREF:
          //Log("Deref\n");
          return vaddr_read(eval(p+1,q),4);
        case TK_UNARY:
          return -eval(p+1,q);
      }
    }

    int stack=0;  //consider in-bracket condition
    for(int i=p;i<=q;i++){

      if(tokens[i].type=='('){stack++;}
      if(tokens[i].type==')'){stack--;}
      assert(stack>=0);//bracket should match
      if(stack>0)continue;//if in-bracket,ignore
      int lvl =tokens[i].level;
      if(lvl>=2&&lvl>=level){
        domain  = i;
        level   = lvl;
      }
    }

    switch(tokens[domain].type){
      case TK_EQ:
        return eval(p,domain-1) == eval(domain+1,q);
      case TK_NEQ:
        return eval(p,domain-1) != eval(domain+1,q);
      case '+':
        return eval(p,domain-1) +  eval(domain+1,q);
      case '-':
        return eval(p,domain-1) -  eval(domain+1,q);
      case '*':
        return eval(p,domain-1) *  eval(domain+1,q);
      case '/':
        return eval(p,domain-1) /  eval(domain+1,q);
      default:
        assert(0);
    }
  }
  assert(0);
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  for (int i = 0; i < nr_token; i ++) {
    if(tokens[i].type==TK_REG){
      for(int k=0;k<strlen(tokens[i].str);k++){
        tokens[i].str[k]=tolower(tokens[i].str[k]);
      }
    }

    if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].level>0) ) {
      tokens[i].type = TK_DEREF;
    }
    if (tokens[i].type == '-' && (i == 0 || tokens[i - 1].level>0) ) {
       tokens[i].type = TK_UNARY;
    }
    Log("token %d type %d\n",i,tokens[i].type);
  }
  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token-1);

}
