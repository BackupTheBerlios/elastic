
/* A structure for holding user data */

typedef struct {
  char  login[16];            /* Login ID  */
  int   uid;                  /* User ID   */
  int   gid;                  /* Group ID  */
  char  name[32];             /* User name */
  char  home[256];            /* Home directory */
} User;

/* A global variable */
extern float myv;


