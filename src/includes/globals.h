/****************************************************************************
         Amnuts version 2.3.0 - Copyright (C) Andrew Collington, 2001
                      Last update: 1st September, 2001

                              amnuts@talker.com
                          http://amnuts.talker.com/

                                   based on

   NUTS version 3.3.3 (Triple Three :) - Copyright (C) Neil Robertson 1996
 ***************************************************************************/


#define ML_ENTRY(a) ML_EXPAND a

/* Levels */
#define LVL_LIST \
  ML_ENTRY((JAILED,     "VYVRHEL", "0" )) \
  ML_ENTRY((NEW,        "PUTNIK",    "1" )) \
  ML_ENTRY((USER,       "USADLIK",   "2"  )) \
  ML_ENTRY((SUPER,      "KLERIK",  "3"  )) \
  ML_ENTRY((KUZ,        "KUZELNIK",    "4" )) \
  ML_ENTRY((WIZ,        "DRUID",    "5" )) \
  ML_ENTRY((ARCH,       "ANJEL",   "6" )) \
  ML_ENTRY((GOD,        "ARCHANJEL",    "7" )) \
  ML_ENTRY((NUM_LEVELS, NULL,     NULL))

/*
 * Object types
 */
typedef struct user_struct *UR_OBJECT;
typedef struct room_struct *RM_OBJECT;
typedef struct netlink_struct *NL_OBJECT;
typedef struct list_users_struct *UD_OBJECT;
typedef struct command_struct *CMD_OBJECT;
typedef struct system_struct *SYS_OBJECT;
typedef struct flagged_user *FU_OBJECT;
typedef struct review_buffer *RB_OBJECT;
typedef struct buf_mesg *PM_OBJECT;


/*
 * Enumerations
 */
enum lvl_value
{
#define ML_EXPAND(value,name,alias) value,
  LVL_LIST
#undef ML_EXPAND
};


/*
 * user variables - some are saved in the user file, and some are not
 */
struct user_struct
{
  char name[USER_NAME_LEN + 1], desc[USER_DESC_LEN + 1], pass[PASS_LEN + 1];
  char in_phrase[PHRASE_LEN + 1], out_phrase[PHRASE_LEN + 1], buff[BUFSIZE];
  char site[81], ipsite[81], last_site[81];     /* XXX: Use NI_MAXHOST */
  char site_port[32];           /* NI_MAXSERV */
  char page_file[81], mail_to[WORD_LEN + 1];
  char afk_mesg[AFK_MESG_LEN + 1], inpstr_old[REVIEW_LEN + 1];
  char logout_room[ROOM_NAME_LEN + 1], version[10];
  char copyto[MAX_COPIES][USER_NAME_LEN + 1], invite_by[USER_NAME_LEN + 1],
    date[80];
  char email[81], homepage[81], recap[RECAP_NAME_LEN],
    samesite_check_store[ARR_SIZE];
  char bw_recap[USER_NAME_LEN + 1], call[USER_NAME_LEN + 1],
    macros[10][MACRO_LEN], verify_code[80];
  char hang_word[WORD_LEN + 1], hang_word_show[WORD_LEN + 1],
    hang_guess[WORD_LEN + 1], nomin[USER_NAME_LEN + 4];
  char genit[USER_NAME_LEN + 4], dativ[USER_NAME_LEN + 4],
       akuza[USER_NAME_LEN + 4], lokal[USER_NAME_LEN + 4];
  char instr[USER_NAME_LEN + 4];       
  char *malloc_start, *malloc_end, icq[ICQ_LEN + 1];
  int type, login, attempts, vis, ignall, prompt, command_mode, charmode_echo;
  int gender, hideemail, edit_line, warned, accreq, ignall_store, igntells;
  int afk, clone_hear, colour, ignshouts, unarrest, pager, expire, lroom,
    monitor;
  int show_rdesc, wrap, alert, mail_verified, autofwd, show_pass, pagecnt,
    pages[MAX_PAGES];
  int ignpics, ignlogons, ignwiz, igngreets, ignbeeps, hang_stage,
    samesite_all_store;
#ifdef WIZPORT
  int wizport;
#endif
  int socket, buffpos, filepos, remote_com, charcnt, misc_op, last_login_len;
  int edit_op, wipe_from, wipe_to, logons, cmd_type, user_page_pos, lmail_all;
  enum lvl_value user_page_lev;
  enum lvl_value lmail_lev;
  enum lvl_value muzzled;
  enum lvl_value arrestby;
  enum lvl_value real_level;
  enum lvl_value level;
  int age, misses, hits, kills, deaths, bullets, hps, login_prompt, retired;
  int gcoms[MAX_GCOMS], xcoms[MAX_XCOMS], reverse_buffer;
  RM_OBJECT room, invite_room, wrap_room;
  UR_OBJECT prev, next, owner;
  struct
  {
    long date;
    char msg[REMINDER_LEN];
  }
  reminder[MAX_REMINDERS];
  int reminder_pos;
  time_t last_input, last_login, total_login, read_mail, t_expire;
#ifdef NETLINKS
  NL_OBJECT netlink, pot_netlink;
#endif
  int pm_count, pm_currcount, universal_pager;
  PM_OBJECT pm_current, pm_first, pm_last;
  FU_OBJECT fu_first, fu_last;
  RB_OBJECT rb_first, rb_last;
  int money, bank, inctime;
};


/*
 * structure to see who last logged on
 */
struct last_login_struct
{
  char name[USER_NAME_LEN + 1], time[80];
  int on;
};


/*
 * room informaytion structure
 */
struct room_struct
{
  char name[ROOM_NAME_LEN + 1], label[ROOM_LABEL_LEN + 1],
    desc[ROOM_DESC_LEN + 1], topic[TOPIC_LEN + 1];
  char revbuff[REVIEW_LINES][REVIEW_LEN + 2], map[ROOM_NAME_LEN + 1],
    show_name[PERSONAL_ROOMNAME_LEN];
  int access;                   /* public , private etc */
  int revline;                  /* line number for review */
  int mesg_cnt;
  char link_label[MAX_LINKS][ROOM_LABEL_LEN + 1];       /* temp store for parse */
  RM_OBJECT link[MAX_LINKS];
  RM_OBJECT next, prev;
#ifdef NETLINKS
  int inlink;                   /* 1 if room accepts incoming net links */
  char netlink_name[SERV_NAME_LEN + 1]; /* temp store for config parse */
  NL_OBJECT netlink;            /* for net links, 1 per room */
#endif
};


#ifdef NETLINKS
/*
 * Structure for net links, ie server initiates them
 */
struct netlink_struct
{
  char service[SERV_NAME_LEN + 1];
  char site[SITE_NAME_LEN + 1]; /* XXX: Use NI_MAXHOST */
  char port[32];                /* NI_MAXSERV */
  char verification[VERIFY_LEN + 1];
  char buffer[ARR_SIZE * 2];
  char mail_to[WORD_LEN + 1];
  char mail_from[WORD_LEN + 1];
  FILE *mailfile;
  time_t last_recvd;
  int socket, type, connected;
  int stage, lastcom, allow, warned, keepalive_cnt;
  int ver_major, ver_minor, ver_patch;
  UR_OBJECT mesg_user;
  RM_OBJECT connect_room;
  NL_OBJECT prev, next;
};
#endif


/*
 * main user list structure
 */
struct list_users_struct
{
  char name[USER_NAME_LEN + 1], date[80];
  enum lvl_value level;
  int retired;
  UD_OBJECT next, prev;
};


/*
 * command list
 */
struct command_struct
{
  char name[15], alias[5];      /* 15 and 5 characters should be long enough */
  int id;                       /* FIXME: Should be enum cmd_value */
  enum lvl_value min_lev;
  int function;                 /* FIXME: Should be enum ct_value */
  int count;
  CMD_OBJECT next, prev;
};


/*
 * system structure
 */
struct system_struct
{
  int heartbeat, keepalive_interval, ignore_sigterm;
  int mesg_life;
  int mesg_check_hour;
  int mesg_check_min;
  time_t mesg_check_done;
  time_t auto_purge_date;
  int net_idle_time, login_idle_time, user_idle_time, time_out_afks,
    crash_action;
  int num_of_users, num_of_logins, logons_old, logons_new, purge_count,
    purge_skip, users_purged;
  int user_count, max_users, max_clones, min_private_users, colour_def,
    charecho_def, prompt_def;
  enum lvl_value time_out_maxlevel;
  enum lvl_value wizport_level;
  enum lvl_value minlogin_level;
  enum lvl_value gatecrash_level;
  enum lvl_value ignore_mp_level;
  enum lvl_value rem_user_maxlevel;
  enum lvl_value rem_user_deflevel;
  int password_echo, auto_promote, ban_swearing, personal_rooms,
    startup_room_parse, auto_connect;
  int allow_recaps, suggestion_count, random_motds, motd1_cnt, motd2_cnt,
    forwarding, sbuffline;
  int resolve_ip, rs_countdown, level_count[GOD + 1], last_cmd_cnt,
    flood_protect;
  int mport_socket;
  char mport_port[32];          /* NI_MAXSERV */
#ifdef WIZPORT
  int wport_socket;
  char wport_port[32];          /* NI_MAXSERV */
#endif
#ifdef NETLINKS
  int nlink_socket;
  char nlink_port[32];          /* NI_MAXSERV */
  char verification[SERV_NAME_LEN + 1];
#endif
#ifdef IDENTD
  int ident_pid;
  int ident_socket, ident_state;
#endif
  int boot_off_min;
  int stop_logins, is_pager;
  unsigned logging;
  struct utsname uts;
  char shoutbuff[REVIEW_LINES][REVIEW_LEN + 2];
  time_t boot_time, rs_announce, rs_which, sreb_time;
  UR_OBJECT rs_user;
};


/*
 * pager buffer structure
 */
struct buf_mesg
{
  char *data;
  PM_OBJECT next, prev;
};


/*
 * flagged users
 */
struct flagged_user
{
  char *name;
  unsigned int flags;
  FU_OBJECT prev, next;
};


/*
 * review buffer structure
 */
struct review_buffer
{
  char *buffer, *name;
  unsigned int flags;
  RB_OBJECT next, prev;
};


/*
 * for seamless reboot
 */
struct talker_system_info
{
  int mport_socket;
#ifdef WIZPORT
  int wport_socket;
#endif
#ifdef NETLINKS
  int nlink_socket;
#endif
  int reduce;
  char mr_name[ROOM_NAME_LEN + 1];
  char rebooter[USER_NAME_LEN + 1];
  time_t boot_time;
  int last_com;
  int num_of_logins, new_user_logins, old_user_logins;
  int ban_swearing;
};


/*
 * Constant structures
 */
struct user_level_struct
{
  const char *name;
  const char *alias;
};

struct priv_room_struct
{
  const char *name;
  enum lvl_value level;
};

struct colour_codes_struct
{
  const char *txt_code;
  const char *esc_code;
};

struct kill_mesgs_struct
{
  const char *victim_msg;
  const char *room_msg;
};


/* XXX: Ug! Kill the globals */
extern const struct user_level_struct user_level[];
extern const struct priv_room_struct priv_room[];
extern const struct colour_codes_struct colour_codes[];
extern const struct kill_mesgs_struct kill_mesgs[];
extern const char *const hanged[8];
extern const char *const swear_words[];
extern const char *const noyes[];
extern const char *const offon[], *const minmax[], *const sex[], *const koncovka[];
extern const int biglet[26][5][5], bigsym[32][5][5];
extern const char default_jail[], default_warp[], default_shoot[],
  default_bank[];
extern const char syserror[], nosuchroom[], nosuchuser[], notloggedon[],
  invisenter[], invisleave[], invisname[], crypt_salt[];
extern const char swear_censor[], noswearing[];
extern const char default_personal_room_desc[];
extern const char talker_signature[];

extern struct last_login_struct last_login_info[LASTLOGON_NUM + 1];
extern UR_OBJECT user_first, user_last;
extern RM_OBJECT room_first, room_last;
#ifdef NETLINKS
extern NL_OBJECT nl_first, nl_last;
#endif
extern UD_OBJECT first_user_entry, last_user_entry;
extern CMD_OBJECT first_command, last_command;
extern SYS_OBJECT amsys;
extern char word[MAX_WORDS][WORD_LEN + 1], wrd[8][81];
extern char cmd_history[16][128], text[ARR_SIZE * 2], vtext[ARR_SIZE * 2];
extern char progname[40], confile[40];
extern int destructed, force_listen, no_prompt, logon_flag;
extern int config_line, word_count;
extern unsigned retrieve_user_type;

extern char rebooter[USER_NAME_LEN + 1];
extern char mrname[ROOM_NAME_LEN + 1];
extern int reduce_wiz, nbwiz, nbuser, num_of_users, last_com;


/* XXX: These REALLY need to be some place else */
#ifdef __MAIN_FILE__


/*
 * levels used on the talker
 */
const struct user_level_struct user_level[] = {
#define ML_EXPAND(value,name,alias) { name, alias },
  LVL_LIST
#undef ML_EXPAND
};

/*
 * The rooms listed here are just examples of what can be added
 * You may add more or remove as many as you like, but you MUST
 * keep the stopping clause in
 */
const struct priv_room_struct priv_room[] = {
  {"oblacik", WIZ},             /* a room for wizzes+ only */
  {"nebesa", GOD},      /* only top people can get in this place! */
  {NULL, NUM_LEVELS}            /* stopping clause */
};

/*
 * colour code values
 */
const struct colour_codes_struct colour_codes[] = {
  /* Standard stuff */
  {"RS", "\033[0m"},            /* reset */
  {"OL", "\033[1m"},            /* bold */
  {"UL", "\033[4m"},            /* underline */
  {"LI", "\033[5m"},            /* blink */
  {"RV", "\033[7m"},            /* reverse */
  /* Foreground colour */
  {"FK", "\033[30m"},           /* black */
  {"FR", "\033[31m"},           /* red */
  {"FG", "\033[32m"},           /* green */
  {"FY", "\033[33m"},           /* yellow */
  {"FB", "\033[34m"},           /* blue */
  {"FM", "\033[35m"},           /* magenta */
  {"FC", "\033[36m"},           /* cyan */
  {"FW", "\033[37m"},           /* white */
  /* Background colour */
  {"BK", "\033[40m"},           /* black */
  {"BR", "\033[41m"},           /* red */
  {"BG", "\033[42m"},           /* green */
  {"BY", "\033[43m"},           /* yellow */
  {"BB", "\033[44m"},           /* blue */
  {"BM", "\033[45m"},           /* magenta */
  {"BC", "\033[46m"},           /* cyan */
  {"BW", "\033[47m"},           /* white */
  /* Some compatibility names */
  {"FT", "\033[36m"},           /* cyan AKA turquoise */
  {"BT", "\033[46m"},           /* cyan AKA turquoise */
  {NULL, NULL},                 /* end */
};


/*
 * random kill messages
 */
const struct kill_mesgs_struct kill_mesgs[] = {
  {"You are killed\n", "%s is killed\n"},
  {"You have been totally splatted\n", "A hammer splats %s\n"},
  {"The Hedgehog Of Doom runs over you with a car.\n",
   "The Hedgehog Of Doom runs over %s with a car.\n"},
  {"The Inquisitor deletes the worthless, prunes away the wastrels... ie, YOU!", "The Inquisitor prunes away %s.\n"},
  {NULL, NULL},
};

#ifdef GAMES
/*
 * hangman picture for the hangman game
 */
const char *const hanged[8] = {
  "~FY~OL+~RS~FY---~OL+  \n~FY|      \n~FY|~RS           ~OLWord:~RS %s\n~FY|~RS           ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS           ~OLWord:~RS %s\n~FY|~RS           ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS           ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS   |       ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS  /|       ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS  /|\\      ~OLLetters guessed:~RS %s\n~FY|~RS      \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS  /|\\      ~OLLetters guessed:~RS %s\n~FY|~RS  /   \n~FY|______\n",
  "~FY~OL+~RS~FY---~OL+  \n~FY|   |  \n~FY|~RS   O       ~OLWord:~RS %s\n~FY|~RS  /|\\      ~OLLetters guessed:~RS %s\n~FY|~RS  / \\ \n~FY|______\n"
};
#endif

/*
 * swear words array.  These must all be lowercase.  NULL is the stopping
 * clause and must remain in the array even if you have no words listed.
 */
const char *const swear_words[] = {
  "pici","picha","pica","kokot","kurva","kurwa","buzerant","jeb","fuck", "shit", "cunt", "cock", "bastard", "dyke", "fag", "pussy", "bitch",
    NULL
};

/*
 * some general arrays being defined
 */
const char *const noyes[] = { "NIE", "ANO" };
const char *const offon[] = { "OFF", "ON" };
const char *const minmax[] = { "OFF", "MIN", "MAX" };
const char *const sex[] = {"ziadne", "Muz","Zena" };
const char *const koncovka[] = {"","","a"};

/*
 * Letter array map - for greet()
 */
const int biglet[26][5][5] = {
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1},
   {1, 0, 0, 0, 1}},
  {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 1},
   {1, 1, 1, 1, 0}},
  {{0, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0},
   {0, 1, 1, 1, 1}},
  {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
   {1, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 0},
   {1, 1, 1, 1, 1}},
  {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 0},
   {1, 0, 0, 0, 0}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 0, 1, 1, 0}, {1, 0, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1},
   {1, 0, 0, 0, 1}},
  {{0, 1, 1, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
   {0, 1, 1, 1, 0}},
  {{0, 0, 0, 0, 1}, {0, 0, 0, 0, 1}, {0, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{1, 0, 0, 0, 1}, {1, 0, 0, 1, 0}, {1, 0, 1, 0, 0}, {1, 0, 0, 1, 0},
   {1, 0, 0, 0, 1}},
  {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0},
   {1, 1, 1, 1, 1}},
  {{1, 0, 0, 0, 1}, {1, 1, 0, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 0, 1},
   {1, 0, 0, 0, 1}},
  {{1, 0, 0, 0, 1}, {1, 1, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1},
   {1, 0, 0, 0, 1}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 0},
   {1, 0, 0, 0, 0}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1},
   {0, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 0}, {1, 0, 0, 1, 0},
   {1, 0, 0, 0, 1}},
  {{0, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 1},
   {1, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
   {0, 0, 1, 0, 0}},
  {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 0, 0, 1},
   {1, 1, 1, 1, 1}},
  {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0},
   {0, 0, 1, 0, 0}},
  {{1, 0, 0, 0, 1}, {1, 0, 0, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 1, 1},
   {1, 0, 0, 0, 1}},
  {{1, 0, 0, 0, 1}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 1, 0},
   {1, 0, 0, 0, 1}},
  {{1, 0, 0, 0, 1}, {0, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
   {0, 0, 1, 0, 0}},
  {{1, 1, 1, 1, 1}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 0, 0},
   {1, 1, 1, 1, 1}}
};


/*
 * Symbol array map - for greet()
 */
const int bigsym[32][5][5] = {
  {{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0},
   {0, 0, 1, 0, 0}},
  {{0, 1, 0, 1, 0}, {0, 1, 0, 1, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0}},
  {{0, 1, 0, 1, 0}, {1, 1, 1, 1, 1}, {0, 1, 0, 1, 0}, {1, 1, 1, 1, 1},
   {0, 1, 0, 1, 0}},
  {{0, 1, 1, 1, 1}, {1, 0, 1, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 1, 0, 1},
   {1, 1, 1, 1, 0}},
  {{1, 1, 0, 0, 1}, {1, 1, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 1, 1},
   {1, 0, 0, 1, 1}},
  {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0}},
  {{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0}},
  {{0, 0, 1, 1, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0},
   {0, 0, 1, 1, 0}},
  {{0, 1, 1, 0, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0},
   {0, 1, 1, 0, 0}},
  {{1, 0, 1, 0, 1}, {0, 1, 1, 1, 0}, {1, 1, 1, 1, 1}, {0, 1, 1, 1, 0},
   {1, 0, 1, 0, 1}},
  {{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 1, 0, 0},
   {0, 0, 1, 0, 0}},
  {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 0, 0, 0},
   {1, 1, 0, 0, 0}},
  {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0}},
  {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 0, 0, 0},
   {1, 1, 0, 0, 0}},
  {{0, 0, 0, 0, 1}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 0, 0},
   {1, 0, 0, 0, 0}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{0, 0, 1, 0, 0}, {0, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0},
   {0, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 0}, {0, 0, 0, 0, 1}, {0, 1, 1, 1, 0}, {1, 0, 0, 0, 0},
   {1, 1, 1, 1, 1}},
  {{1, 1, 1, 1, 0}, {0, 0, 0, 0, 1}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 1},
   {1, 1, 1, 1, 0}},
  {{0, 0, 1, 1, 0}, {0, 1, 0, 0, 0}, {1, 0, 0, 1, 0}, {1, 1, 1, 1, 1},
   {0, 0, 0, 1, 0}},
  {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {0, 0, 0, 0, 1},
   {1, 1, 1, 1, 0}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {1, 0, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0},
   {0, 1, 0, 0, 0}},
  {{0, 1, 1, 1, 0}, {1, 0, 0, 0, 1}, {0, 1, 1, 1, 0}, {1, 0, 0, 0, 1},
   {0, 1, 1, 1, 0}},
  {{1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1},
   {0, 0, 0, 0, 1}},
  {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0},
   {0, 0, 0, 0, 0}},
  {{0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0},
   {0, 1, 0, 0, 0}},
  {{0, 0, 0, 1, 0}, {0, 0, 1, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 1, 0, 0},
   {0, 0, 0, 1, 0}},
  {{0, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 1},
   {0, 0, 0, 0, 0}},
  {{0, 1, 0, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 1, 0}, {0, 0, 1, 0, 0},
   {0, 1, 0, 0, 0}},
  {{0, 1, 1, 1, 1}, {0, 0, 0, 0, 1}, {0, 0, 1, 1, 1}, {0, 0, 0, 0, 0},
   {0, 0, 1, 0, 0}},
  {{0, 1, 0, 0, 0}, {1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1},
   {0, 1, 1, 1, 0}}
};

/*
 * default rooms
 */
const char default_jail[] = "zalar";
const char default_warp[] = "mesto_anjelov";
const char default_shoot[] = "dracia_hora";
const char default_bank[] = "serverovna";

/*
 * other strings used on the talker
 */
const char syserror[] = "Sorry, a system error has occured";
const char nosuchroom[] = "Taqe miesto tu nie je.\n";
const char nosuchuser[] = "O tom som este nepocul.\n";
const char notloggedon[] = "There is no one of that name logged on.\n";
const char invisenter[] = "Do miestnosti niekto vstupil...\n";
const char invisleave[] = "Niekto opustil miestnost...\n";
const char invisname[] = "Neviditelny";
const char crypt_salt[] = "NU";

/*
 * This is what replaces any swear words in a string if the swearban is set to MIN.
 */
const char swear_censor[] = "&#@%";     /* za co budu nahradene nadavky */
const char noswearing[] = "...skus tho povedat slusnejsie...\n";

/*
 * you can set a standard room desc for those people who are creating a new
 * personal room
 */
const char default_personal_room_desc[] =
  "Pevne mury a hole steny, tato ruumka zatial nie je zariadena, lenivy\n"
  "user, alebo len nevie ze thu je prikaz .mypaint ... ;))\n";

/*
 * you can change this for whatever sig you want - of just "" if you don't want
 * to have a sig file attached at the end of emails
 */
const char talker_signature[] =
  "+--------------------------------------------------------------------------+\n"
  "|              Tento mail ti bol poslany z Nucler-talkra ako tvoj          |\n"
  "|        auto-forward. Ak nie... kludne ho ignoruj, niekto sa pomylil.     |\n"
  "|                                                                          |\n"
  "|                  Nuclear talker - angeltalker.ath.cx 9999                |\n"
  "|                v pripade otazok napis na info@nucleartalker.eu           |\n"
  "+--------------------------------------------------------------------------+\n";


struct last_login_struct last_login_info[LASTLOGON_NUM + 1] = { {"", "", 0} };

UR_OBJECT user_first = NULL, user_last = NULL;
RM_OBJECT room_first = NULL, room_last = NULL;
#ifdef NETLINKS
NL_OBJECT nl_first = NULL, nl_last = NULL;
#endif
UD_OBJECT first_user_entry = NULL, last_user_entry = NULL;
CMD_OBJECT first_command = NULL, last_command = NULL;
SYS_OBJECT amsys = NULL;


/*
 * Other global variables
 */
char word[MAX_WORDS][WORD_LEN + 1] = { "" };
char wrd[8][81] = { "" };
char cmd_history[16][128] = { "" };
char text[ARR_SIZE * 2] = "", vtext[ARR_SIZE * 2] = "";
char progname[40] = "", confile[40] = "";
int destructed = 0, force_listen = 0, no_prompt = 0, logon_flag = 0;
int config_line = 0, word_count = 0;
unsigned retrieve_user_type = 0;

/*
 * seemless reboot
 */
char rebooter[USER_NAME_LEN + 1] = "";  /* use a global for nicer looking code */
char mrname[ROOM_NAME_LEN + 1] = "";
int reduce_wiz = 0, nbwiz = 0, nbuser = 0, num_of_users = 0;
int last_com = 0;


#endif /* ifdef __MAIN_FILE__ */
