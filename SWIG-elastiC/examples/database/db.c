#include "db.h"
#include <stdlib.h>
#include <string.h>

static int  nusers = 0;
static int  maxitems = 256;
static User *user_data = 0;

float myv = 0;

void add_user(User u) {
  if (!user_data) {
    user_data = (User *) malloc(maxitems*sizeof(User));
  }
  if (nusers == maxitems) {
    maxitems = maxitems*2;
    user_data = (User *) realloc(user_data,maxitems*sizeof(User));
  }
  user_data[nusers] = u;
  nusers++;
}

static int usercmp(const void *u1, const void *u2) {
  User *user1 = (User *) u1;
  User *user2 = (User *) u2;
  return (strcmp(user1->login,user2->login));
}

void sort_users(void) {
  qsort(user_data,nusers,sizeof(User),usercmp);
}

static int usersearch(const void *k, const void *u) {
  char *key = (char *) k;
  User *user = (User *) u;
  return (strcmp(key,user->login));
}

User *lookup_user(char *name) {
  User *u;
  u = (User *) bsearch(name,user_data, nusers, sizeof(User), usersearch);
  return u;
}

User *get_user(int i) {
  if (i >= nusers) return 0;
  else return &user_data[i];
}

int num_users(void) {
  return nusers;
}
