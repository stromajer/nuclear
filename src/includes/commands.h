/****************************************************************************
         Amnuts version 2.3.0 - Copyright (C) Andrew Collington, 2001
                      Last update: 1st September, 2001

                              amnuts@talker.com
                          http://amnuts.talker.com/

                                   based on

   NUTS version 3.3.3 (Triple Three :) - Copyright (C) Neil Robertson 1996
 ***************************************************************************/


/* This is the command general function type list */
#define CT_LIST \
  ML_ENTRY((GENERAL, "General")) \
  ML_ENTRY((SOCIAL,  "Social")) \
  ML_ENTRY((MSG,     "Messages")) \
  ML_ENTRY((USER,    "User")) \
  ML_ENTRY((IGNORE,  "Ignores")) \
  ML_ENTRY((MOVE,    "Movement")) \
  ML_ENTRY((CLONE,   "Clones")) \
  ML_ENTRY((ADMIN,   "Admin")) \
  ML_NLINK((NETLINK, "Netlink")) \
  ML_ENTRY((COUNT,   NULL))


/* This is the command list */
#define CMD_LIST \
  ML_ENTRY((QUIT,         "quit",       "",    JAILED,  GENERAL)) \
  ML_ENTRY((SAY,          "say",        "",    JAILED,  SOCIAL )) \
  ML_ENTRY((LOOK,         "look",       "",    JAILED,  GENERAL)) \
  ML_ENTRY((WHO,          "who",        "",    JAILED,  GENERAL)) \
  ML_ENTRY((DESC,         "desc",       "",    JAILED,  USER   )) \
  ML_ENTRY((PROMPT,       "prompt",     "",    JAILED,  USER   )) \
  ML_ENTRY((HELP,         "help",       "",    JAILED,  GENERAL)) \
  ML_ENTRY((RANKS,        "ranks",      "",    JAILED,  GENERAL)) \
  ML_ENTRY((WIZLIST,      "wizlist",    "",    JAILED,  GENERAL)) \
  ML_ENTRY((RULES,        "rules",      "",    JAILED,  GENERAL)) \
  ML_ENTRY((SOS,          "sos",        "",    JAILED,  SOCIAL )) \
  ML_ENTRY((VER,          "version",    "",    JAILED,  GENERAL)) \
  ML_ENTRY((CHARECHO,     "charecho",   "",    JAILED,  USER   )) \
  ML_ENTRY((CLS,          "cls",        "",    JAILED,  GENERAL)) \
  ML_ENTRY((SUICIDE,      "suicide",    "",    JAILED,  USER   )) \
  ML_ENTRY((SHOUT,        "shout",      "!",   NEW,     SOCIAL )) \
  ML_ENTRY((TELL,         "tell",       "",    NEW,     SOCIAL )) \
  ML_ENTRY((GO,           "go",         "",    NEW,     MOVE   )) \
  ML_ENTRY((NEWS,         "news",       "",    NEW,     MSG    )) \
  ML_ENTRY((READ,         "read",       "",    NEW,     MSG    )) \
  ML_ENTRY((WRITE,        "write",      "",    NEW,     MSG    )) \
  ML_ENTRY((WIPE,         "wipe",       "",    NEW,     MSG    )) \
  ML_ENTRY((REVIEW,       "review",     "",    NEW,     GENERAL)) \
  ML_ENTRY((REVTELL,      "revtell",    "",    NEW,     SOCIAL )) \
  ML_ENTRY((REVSHOUT,     "revshout",   "",    NEW,     SOCIAL )) \
  ML_ENTRY((REVAFK,       "revafk",     "",    NEW,     SOCIAL )) \
  ML_ENTRY((MAP,          "map",        "",    NEW,     GENERAL)) \
  ML_ENTRY((LETMEIN,      "knock",      "",    NEW,     GENERAL)) \
  ML_ENTRY((AFK,          "afk",        "",    NEW,     USER   )) \
  ML_ENTRY((COLOUR,       "colour",     "",    NEW,     GENERAL)) \
  ML_ENTRY((SAYTO,        "sayto",      "-",   NEW,     SOCIAL )) \
  ML_ENTRY((FRIENDS,      "friends",    "",    NEW,     SOCIAL )) \
  ML_ENTRY((IGNUSER,      "ignuser",    "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNALL,       "ignall",     "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNLIST,      "ignlist",    "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNSHOUTS,    "ignshout",   "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNTELLS,     "igntell",    "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNLOGONS,    "ignlogons",  "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNPICS,      "ignpics",    "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNGREETS,    "igngreets",  "",    NEW,     IGNORE )) \
  ML_ENTRY((IGNBEEPS,     "ignbeeps",   "",    NEW,     IGNORE )) \
  ML_ENTRY((VERIFY,       "verify",     "",    NEW,     MSG    )) \
  ML_ENTRY((COPYTO,       "copyto",     "",    NEW,     MSG    )) \
  ML_ENTRY((NOCOPIES,     "nocopys",    "",    NEW,     MSG    )) \
  ML_ENTRY((SET,          "set",        "",    NEW,     USER   )) \
  ML_ENTRY((TIME,         "time",       "",    NEW,     GENERAL)) \
  ML_ENTRY((HANGMAN,      "hangman",    "",    NEW,     GENERAL)) \
  ML_ENTRY((GUESS,        "guess",      "",    NEW,     GENERAL)) \
  ML_GAMES((CASH,         "cash",       "",    NEW,     GENERAL)) \
  ML_GAMES((MONEY,        "money",      "",    NEW,     GENERAL)) \
  ML_GAMES((BANK,         "bank",       "",    NEW,     GENERAL)) \
  ML_ENTRY((CALENDAR,     "calendar",   "",    NEW,     GENERAL)) \
  ML_ENTRY((FWHO,         "fwho",       "",    NEW,     GENERAL)) \
  ML_ENTRY((SUG,          "suggest",    "",    NEW,     MSG    )) \
  ML_ENTRY((MODE,         "mode",       "",    NEW,     USER   )) \
  ML_ENTRY((ACCREQ,       "accreq",     "",    NEW,     USER   )) \
  ML_ENTRY((EXAMINE,      "examine",    "",    USER,    USER   )) \
  ML_ENTRY((KISS,         "kiss",       "",    USER,    SOCIAL)) \
  ML_ENTRY((HUG,          "hug",        "",    USER,    SOCIAL)) \
  ML_ENTRY((WAKE,         "wake",       "",    USER,    SOCIAL )) \
  ML_ENTRY((EMOTE,        "emote",      ":",   USER,    SOCIAL )) \
  ML_ENTRY((STATUS,       "ustat",      "",    USER,    USER   )) \
  ML_ENTRY((RMAIL,        "rmail",      "",    USER,    MSG    )) \
  ML_ENTRY((SMAIL,        "smail",      "",    USER,    MSG    )) \
  ML_ENTRY((DMAIL,        "dmail",      "",    USER,    MSG    )) \
  ML_ENTRY((FMAIL,        "fmail",      "",    USER,    MSG    )) \
  ML_ENTRY((REMINDER,     "reminder",   "",    USER,    MSG    )) \
  ML_ENTRY((QCALL,        "call",       ",",   USER,    SOCIAL )) \
  ML_ENTRY((UNQCALL,      "uncall",     "",    USER,    SOCIAL )) \
  ML_ENTRY((GREET,        "greet",      "",    USER,    SOCIAL )) \
  ML_ENTRY((THINKIT,      "think",      "",    USER,    SOCIAL )) \
  ML_ENTRY((SINGIT,       "sing",       "",    USER,    SOCIAL )) \
  ML_ENTRY((LAST,         "last",       "",    USER,    USER   )) \
  ML_ENTRY((MACROS,       "macros",     "",    USER,    USER   )) \
  ML_ENTRY((FROM,         "from",       "",    USER,    MSG    )) \
  ML_ENTRY((ENTPRO,       "entpro",     "",    USER,    USER   )) \
  ML_ENTRY((RMST,         "rooms",      "",    USER,    GENERAL)) \
  ML_ENTRY((DISPLAY,      "files",      "",    USER,    GENERAL)) \
  ML_GAMES((DONATE,       "donate",     "",    USER,    GENERAL)) \
  ML_ENTRY((PASSWD,       "passwd",     "",    USER,    USER   )) \
  ML_ENTRY((JOIN,         "join",       "",    SUPER,   MOVE   )) \
  ML_ENTRY((REVCLR,       "cbuff",      "*",   SUPER,   SOCIAL )) \
  ML_ENTRY((ECHO,         "echo",       "+",   SUPER,   SOCIAL )) \
  ML_ENTRY((INPHRASE,     "inmsg",      "",    SUPER,   USER   )) \
  ML_ENTRY((OUTPHRASE,    "outmsg",     "",    SUPER,   USER   )) \
  ML_ENTRY((PUBCOM,       "public",     "",    SUPER,   GENERAL)) \
  ML_ENTRY((PRIVCOM,      "private",    "",    SUPER,   GENERAL)) \
  ML_ENTRY((INVITE,       "invite",     "",    SUPER,   GENERAL)) \
  ML_ENTRY((TOPIC,        "topic",      "",    SUPER,   SOCIAL )) \
  ML_ENTRY((SEMOTE,       "semote",     "&",   SUPER,   SOCIAL )) \
  ML_ENTRY((PEMOTE,       "pemote",     "</",  SUPER,   SOCIAL )) \
  ML_ENTRY((PTELL,        "ptell",      "",    SUPER,   SOCIAL )) \
  ML_ENTRY((PREVIEW,      "preview",    "",    SUPER,   SOCIAL )) \
  ML_ENTRY((PICTURE,      "picture",    "",    SUPER,   SOCIAL )) \
  ML_ENTRY((FSAY,         "fsay",       "",    SUPER,   SOCIAL )) \
  ML_ENTRY((FSMAIL,       "fsmail",     "",    SUPER,   MSG    )) \
  ML_ENTRY((FEMOTE,       "femote",     "",    SUPER,   SOCIAL )) \
  ML_ENTRY((CTELLS,       "ctells",     "",    SUPER,   SOCIAL )) \
  ML_ENTRY((SPODLIST,     "spodlist",   "",    SUPER,    GENERAL)) \
  ML_ENTRY((BRING,        "bring",      "",    KUZ,     MOVE   )) \
  ML_ENTRY((CTOPIC,       "ctopic",     "",    KUZ,     SOCIAL )) \
  ML_ENTRY((VIS,          "vis",        "",    KUZ,     USER   )) \
  ML_ENTRY((INVIS,        "invis",      "",    KUZ,     USER   )) \
  ML_ENTRY((UNINVITE,     "uninvite",   "",    KUZ,     GENERAL)) \
  ML_ENTRY((BCAST,        "bcast",      "",    WIZ,     SOCIAL )) \
  ML_ENTRY((WIZSHOUT,     "twiz",       ">",   WIZ,     SOCIAL )) \
  ML_ENTRY((WIZEMOTE,     "ewiz",       "",    WIZ,     SOCIAL )) \
  ML_ENTRY((CSHOUT,       "cshout",     "",    WIZ,     SOCIAL )) \
  ML_ENTRY((MYROOM,       "myroom",     "",    WIZ,     MOVE   )) \
  ML_ENTRY((MYLOCK,       "mylock",     "",    WIZ,     GENERAL)) \
  ML_ENTRY((VISIT,        "visit",      "",    WIZ,     MOVE   )) \
  ML_ENTRY((MYPAINT,      "mypaint",    "",    WIZ,     GENERAL)) \
  ML_ENTRY((MYNAME,       "myname",     "",    WIZ,     GENERAL)) \
  ML_ENTRY((MYKEY,        "mykey",      "",    WIZ,     GENERAL)) \
  ML_ENTRY((MYBGONE,      "mybgone",    "",    WIZ,     GENERAL)) \
  ML_ENTRY((MOVE,         "move",       "",    ARCH,    MOVE   )) \
  ML_ENTRY((PEOPLE,       "people",     "",    ARCH,    GENERAL)) \
  ML_ENTRY((KILL,         "kill",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((PROMOTE,      "promote",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((DEMOTE,       "demote",     "",    ARCH,    ADMIN  )) \
  ML_ENTRY((LISTBANS,     "lban",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((BAN,          "ban",        "",    ARCH,    ADMIN  )) \
  ML_ENTRY((UNBAN,        "unban",      "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SITE,         "site",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((MUZZLE,       "muzzle",     "",    ARCH,    ADMIN  )) \
  ML_ENTRY((UNMUZZLE,     "unmuzzle",   "",    ARCH,    ADMIN  )) \
  ML_ENTRY((LOGGING,      "logging",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SYSTEM,       "system",     "",    ARCH,    ADMIN  )) \
  ML_ENTRY((FIX,          "fix",        "",    ARCH,    GENERAL)) \
  ML_ENTRY((UNFIX,        "unfix",      "",    ARCH,    GENERAL)) \
  ML_ENTRY((FORCE,        "force",      "",    ARCH,    ADMIN  )) \
  ML_ENTRY((VIEWLOG,      "viewlog",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((CHEAR,        "chear",      "",    ARCH,    CLONE  )) \
  ML_ENTRY((HISTORY,      "history",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((MKVIS,        "makevis",    "",    ARCH,    USER   )) \
  ML_ENTRY((MKINVIS,      "makeinvis",  "",    ARCH,    USER   )) \
  ML_ENTRY((RSUG,         "rsug",       "",    ARCH,    MSG    )) \
  ML_ENTRY((DSUG,         "dsug",       "",    ARCH,    MSG    )) \
  ML_ENTRY((ARREST,       "arrest",     "",    ARCH,    ADMIN  )) \
  ML_ENTRY((UNARREST,     "unarrest",   "",    ARCH,    ADMIN  )) \
  ML_ENTRY((ADDHISTORY,   "addhistory", "",    ARCH,    ADMIN  )) \
  ML_ENTRY((MONITOR,      "monitor",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((IGNWIZ,       "ignwiz",     "",    ARCH,    IGNORE )) \
  ML_ENTRY((ACCOUNT,      "create",     "",    ARCH,    USER   )) \
  ML_ENTRY((SAMESITE,     "samesite",   "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SAVEALL,      "save",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SHACKLE,      "shackle",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((UNSHACKLE,    "unshackle",  "",    ARCH,    ADMIN  )) \
  ML_ENTRY((CMDCOUNT,     "cmdcount",   "",    ARCH,    ADMIN  )) \
  ML_ENTRY((RCOUNTU,      "rcountu",    "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SETCMDLEV,    "setcmdlev",  "",    ARCH,    ADMIN  )) \
  ML_ENTRY((XCOM,         "xcom",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((GCOM,         "gcom",       "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SFROM,        "sfrom",      "",    ARCH,    MSG    )) \
  ML_ENTRY((WIZRULES,     "wrules",     "",    ARCH,    GENERAL)) \
  ML_ENTRY((DISPLAYADMIN, "adminfiles", "",    ARCH,    ADMIN  )) \
  ML_ENTRY((MORPH,        "cname",      "",    ARCH,    ADMIN  )) \
  ML_ENTRY((SHUTDOWN,     "shutdown" ,  "",    GOD,     ADMIN  )) \
  ML_ENTRY((SEARCH,       "search",     "",    GOD,     GENERAL)) \
  ML_NLINK((HOME,         "home",       "",    GOD,     MOVE   )) \
  ML_NLINK((RMSN,         "rnet",       "",    GOD,     NETLINK)) \
  ML_NLINK((NETSTAT,      "netstat",    "",    GOD,     NETLINK)) \
  ML_NLINK((NETDATA,      "netdata",    "",    GOD,     NETLINK)) \
  ML_NLINK((CONN,         "connect",    "",    GOD,     NETLINK)) \
  ML_NLINK((DISCONN,      "disconnect", "",    GOD,     NETLINK)) \
  ML_ENTRY((MINLOGIN,     "minlogin",   "",    GOD,     ADMIN  )) \
  ML_ENTRY((CLEARLINE,    "clearline",  "",    GOD,     ADMIN  )) \
  ML_ENTRY((CREATE,       "clone",      "",    GOD,     CLONE  )) \
  ML_ENTRY((DESTROY,      "destroy",    "",    GOD,     CLONE  )) \
  ML_ENTRY((MYCLONES,     "myclones",   "",    GOD,     CLONE  )) \
  ML_ENTRY((ALLCLONES,    "allclones",  "",    GOD,     CLONE  )) \
  ML_ENTRY((SWITCH,       "switch",     "",    GOD,     CLONE  )) \
  ML_ENTRY((CSAY,         "csay",       "",    GOD,     CLONE  )) \
  ML_NLINK((RSTAT,        "rstat",      "",    GOD,     NETLINK)) \
  ML_ENTRY((SWBAN,        "swban",      "",    GOD,     ADMIN  )) \
  ML_ENTRY((DELETE,       "nuke",       "",    GOD,     ADMIN  )) \
  ML_ENTRY((REBOOT,       "reboot",     "",    GOD,     ADMIN  )) \
  ML_ENTRY((RECOUNT,      "recount",    "",    GOD,     MSG    )) \
  ML_ENTRY((PURGE,        "purge",      "",    GOD,     ADMIN  )) \
  ML_ENTRY((EXPIRE,       "expire",     "",    GOD,     ADMIN  )) \
  ML_ENTRY((BBCAST,       "bbcast",     "",    GOD,     SOCIAL )) \
  ML_ENTRY((SHOW,         "show",       "'",   GOD,     SOCIAL )) \
  ML_ENTRY((MUTTER,       "mutter",     "",    GOD,     SOCIAL )) \
  ML_ENTRY((LMAIL,        "lmail",      "",    GOD,     MSG    )) \
  ML_ENTRY((FORWARDING,   "forwarding", "",    GOD,     ADMIN  )) \
  ML_ENTRY((BFROM,        "bfrom",      "",    GOD,     MSG    )) \
  ML_ENTRY((CEMOTE,       "cemote",     "",    GOD,     CLONE  )) \
  ML_ENTRY((CAFK,         "cafk",       "",    GOD,     SOCIAL )) \
  ML_ENTRY((REVEDIT,      "revedit",    "",    GOD,     SOCIAL )) \
  ML_ENTRY((CEDIT,        "cedit",      "",    GOD,     SOCIAL )) \
  ML_ENTRY((LISTEN,       "listen",     "",    GOD,     IGNORE )) \
  ML_ENTRY((RETIRE,       "retire",     "",    GOD,     ADMIN  )) \
  ML_ENTRY((UNRETIRE,     "unretire",   "",    GOD,     ADMIN  )) \
  ML_ENTRY((RECAPS,       "recaps",     "",    GOD,     ADMIN  )) \
  ML_ENTRY((GREPUSER,     "grepu",      "",    GOD,     GENERAL)) \
  ML_ENTRY((RLOADRM,      "rloadrm",    "",    GOD,     ADMIN  )) \
  ML_ENTRY((SETAUTOPROMO, "autopromo",  "",    GOD,     ADMIN  )) \
  ML_ENTRY((BEEP,         "beep",       "",    GOD,     SOCIAL )) \
  ML_ENTRY((RMADMIN,      "rmadmin",    "",    GOD,     ADMIN  )) \
  ML_ENTRY((DUMPCMD,      "dump",       "",    GOD,     ADMIN  )) \
  ML_ENTRY((TEMPRO,       "tpromote",   "",    GOD,     ADMIN  )) \
  ML_ENTRY((SREBOOT,      "sreboot",    "",    GOD,     ADMIN  )) \
  ML_IDENT((RESITE,       "resite",     "",    GOD,     ADMIN  )) \
  ML_GAMES((SHOOT,        "shoot",      "",    GOD,     GENERAL)) \
  ML_GAMES((RELOAD,       "reload",     "",    GOD,     GENERAL)) \
  ML_ENTRY((FLAGGED,      "flagged",    "",    GOD,     GENERAL)) \
  ML_ENTRY((SKLONUJ,      "sklonuj",    "",    GOD,    ADMIN)) \
  ML_ENTRY((COUNT,        NULL,         NULL,  GOD+1,   COUNT  ))


/* This is the 'set' command attribute list */
#define SET_LIST \
  ML_ENTRY((SHOW,      "show",     "show the current attributes setting"                                   )) \
  ML_ENTRY((GEND,      "gender",   "sets your gender (male, female)"                            )) \
  ML_ENTRY((AGE,       "age",      "set your age for people to see"                                        )) \
  ML_ENTRY((EMAIL,     "email",    "enter your email address"                                              )) \
  ML_ENTRY((HOMEP,     "www",      "enter your homepage address"                                           )) \
  ML_ENTRY((HIDEEMAIL, "hide",     "makes your email visible to only you and the law, or everyone (toggle)")) \
  ML_ENTRY((WRAP,      "wrap",     "sets screen wrap to be on or off (toggle)"                             )) \
  ML_ENTRY((PAGER,     "pager",    "sets how many lines per page of the pager you get"                     )) \
  ML_ENTRY((COLOUR,    "colour",   "display in colour or not (toggle)"                                     )) \
  ML_ENTRY((ROOM,      "room",     "lets you log back into the room you left from, if public (toggle)"     )) \
  ML_ENTRY((FWD,       "autofwd",  "lets you receive smails via your email address."                       )) \
  ML_ENTRY((PASSWD,    "password", "lets you see your password when entering it at the login (toggle)"     )) \
  ML_ENTRY((RDESC,     "rdesc",    "lets you ignore room descriptions (toggle)"                            )) \
  ML_ENTRY((COMMAND,   "command",  "displays the command lisiting differently (toggle)"                    )) \
  ML_ENTRY((RECAP,     "recap",    "allows you to have caps in your name"                                  )) \
  ML_ENTRY((ICQ,       "icq",      "allows you to put in your ICQ number"                                  )) \
  ML_ENTRY((ALERT,     "alert",    "lets you know when someone in your friends list logs on (toggle)"      )) \
  ML_ENTRY((REVBUF,    "revbuf",   "lets you reverse the viewing of your review buffers (toggle)"          )) \
  ML_ENTRY((COUNT,     NULL,       NULL                                                                    ))


/* Macro List Magic */
#define ML_ENTRY(a) ML_EXPAND a
#ifndef GAMES
#define ML_GAMES(a)
#else
#define ML_GAMES(a) ML_EXPAND a
#endif
#ifndef IDENTD
#define ML_IDENT(a)
#else
#define ML_IDENT(a) ML_EXPAND a
#endif
#ifndef NETLINKS
#define ML_NLINK(a)
#else
#define ML_NLINK(a) ML_EXPAND a
#endif

/* XXX: Maybe find a better solution than enums? Pointers are unique */
enum ct_value
{
#define ML_EXPAND(value,name) CT_ ## value,
  CT_LIST
#undef ML_EXPAND
};

enum cmd_value
{
#define ML_EXPAND(value,name,alias,level,type) value,
  CMD_LIST
#undef ML_EXPAND
};

enum set_value
{
#define ML_EXPAND(value,name,desc) SET ## value,
  SET_LIST
#undef ML_EXPAND
};

struct cmd_entry
{
  const char *name;
  const char *alias;
  int level;                    /* FIXME: Should be enum lvl_value */
  enum ct_value function;
};

struct set_entry
{
  const char *type;
  const char *desc;
};

/* XXX: Ug! Kill the globals */
extern enum cmd_value com_num;
extern const char *const command_types[];
extern const struct cmd_entry command_table[];
extern const struct set_entry setstr[];

/* XXX: These REALLY need to be some place else */
#ifdef __MAIN_FILE__

enum cmd_value com_num = COUNT;

const char *const command_types[] = {
#define ML_EXPAND(value,name) name,
  CT_LIST
#undef ML_EXPAND
};

const struct cmd_entry command_table[] = {
#define ML_EXPAND(value,name,alias,level,type) { name, alias, level, CT_ ## type },
  CMD_LIST
#undef ML_EXPAND
};

const struct set_entry setstr[] = {
#define ML_EXPAND(value,name,desc) { name, desc },
  SET_LIST
#undef ML_EXPAND
};

#endif
