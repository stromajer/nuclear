/****************************************************************************
*******************************ANGEL TALKER**********************************
               Angel version 1.0.0 - Copyright (C) Week,  2005

                            zalozeny na zaklade

         Amnuts version 2.3.0 - Copyright (C) Andrew Collington, 2001
				   
   NUTS version 3.3.3 (Triple Three :) - Copyright (C) Neil Robertson 1996
 ***************************************************************************/

#include "defines.h"
#include "globals.h"
#include "commands.h"
#include "prototypes.h"

/***************************************************************************/

/*
 * Say user speech.
 */
void
say(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: say <text>\n";
  const char *type;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes hovorit.\n");
    return;
  }
  if (!strlen(inpstr)) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
#ifdef NETLINKS
  if (!user->room) {
    sprintf(text, "ACT %s say %s\n", user->name, inpstr);
    write_sock(user->netlink->socket, text);
    no_prompt = 1;
    return;
  }
#endif
 //toto som zatial vyhodil, lebo sa mi to nehodilo do slovenciny...//
 // type = smiley_type(inpstr); //
  //if (!type) { //
    type = "Povedal";
 // } //
  if (user->type == CLONE_TYPE) {
    sprintf(text, "Clone - ~FG%s %s:~RS %s~RS\n", user->recap, type,
            inpstr);
    record(user->room, text);
    write_room(user->room, text);
    return;
  }
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  sprintf(text, "~FG%s~RS~FG povedal%s:~RS %s~RS\n", name, koncovka[user->gender], inpstr);
  record(user->room, text);
  write_room_except(user->room, text, user);
  vwrite_user(user, "~FG%s%s si~RS:~RS %s~RS\n", type, koncovka[user->gender], inpstr);
}


/*
 * Direct a say to someone, even though the whole room can hear it
 */
void
say_to(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: sayto <uzivatel> <text>\n";
  const char *type;
  const char *name, *n;
  UR_OBJECT u;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes hovorit.\n");
    return;
  }
  if (word_count < 3 && *inpstr != '-') {
    write_user(user, usage);
    return;
  }
  if (word_count < 2 && *inpstr == '-') {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    vwrite_user(user, "Chces sa rozpravat sam%s so sebou ?!?!?\n",koncovka[user->gender]);
    return;
  }
  if (user->room != u->room || (!u->vis && user->level < u->level)) {
    vwrite_user(user,
                "Nevidis %s~RS, takze mu nemozes nic povedat!\n",
                u->akuza);
    return;
  }
  inpstr = remove_first(inpstr);
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
#ifdef NETLINKS
  /* can send over netlink as normal say */
  if (!user->room) {
    sprintf(text, "ACT %s say %s\n", user->name, inpstr);
    write_sock(user->netlink->socket, text);
    no_prompt = 1;
    return;
  }
#endif
 // type = smiley_type(inpstr); //
 // if (!type) { //
    type = "povedal";
 // } //
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  n = u->vis ? u->recap : invisname;
  sprintf(text, "~FG %s~RS ~FG%s%s ~RS~FG- ~RS(%s~RS)~FG:~RS %s~RS\n", name, type, koncovka[user->gender], n, inpstr);
  record(user->room, text);
  write_room_except(user->room, text, user);
  vwrite_user(user, "~FGPovedal%s si ~RS(%s~RS)~RS~FG:~RS %s~RS\n",koncovka[user->gender], u->dativ, inpstr);
}


/*
 * takze podme na .shout
 */
void
shout(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: shout <text>\n";
  const char *type;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes hovorit.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
       write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  // type = smiley_type(inpstr); //
  // if (!type) { //
    type = "zakrical";
  // } //
  if (!user->vis) {
    write_monitor(user, NULL, 0);
  }
  name = user->vis ? user->recap : invisname;
  sprintf(text, "~OL~FG%s~RS ~OL~FG%s%s:~RS %s~RS\n", name, type, koncovka[user->gender], inpstr);
  record_shout(text);
  write_room_except(NULL, text, user);
  vwrite_user(user, "~OL~FGZakrical%s si:~RS %s~RS\n", koncovka[user->gender], inpstr);
}
/*
* HUG USER-a
*/
void
hug(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: hug <uzivatel>\n";
  UR_OBJECT u;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes nikoho hugnut.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    vwrite_user(user,
               "Objat sam%s seba ??? Daj si ~OL~FRPAUZU~RS.\n",koncovka[user->gender]);
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ta ignoruje.\n", u->recap);
    return;
  }
  if (u->afk) {
    write_user(user, "Uzivatel je ~OL~FRmimo klavesnice~RS.\n");
    return;
  }
  if (u->malloc_start) {
    write_user(user, "Uzivatel prave pracuje na nejakom texte.\n");
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    write_user(user, "Uzivatel prave vsetko ignoruje.\n");
    return;
  }
  name = user->vis ? user->recap : invisname;
  vwrite_user(u,
              "\n%s~BR***~RS %s~RS ~BRta objal%s~RS: ~OL~LI~BRobrazok..!!!~RS ~BR***~RS\n\n",
              u->ignbeeps ? "" : "\007", name, koncovka[user->gender]);
  vwrite_user(user, "~RSObjal%s si %s.\n",koncovka[user->gender],u->akuza);
}

/*
* funkcia kiss
*/

void
kiss(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: kiss <uzivatel>\n";
  UR_OBJECT u;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes nikoho pobozkat.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    vwrite_user(user,
               "Pobozkat sam%s seba ??? Si v ~OL~FR pohode~RS?\n", koncovka[user->gender]);
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ta ignoruje.\n", u->recap);
    return;
  }
  if (u->afk) {
    write_user(user, "Uzivatel je ~OL~FRmimo klavesnice~RS.\n");
    return;
  }
  if (u->malloc_start) {
    write_user(user, "Uzivatel prave pracuje na nejakom texte.\n");
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    write_user(user, "Uzivatel prave vsetko ignoruje.\n");
    return;
  }
  name = user->vis ? user->recap : invisname;
  vwrite_user(u,"%s~BR***~RS~OL %s~RS ~BRti posiela bozk ***~RS\n\n",
              u->ignbeeps ? "" : "\007", name);
  vwrite_user(user, "Pobozkal%s si %s\n",koncovka[user->gender], u->akuza);
}



/*
 * Tell another user something
 */


void
tell_user(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: tell <uzivatel> <text>\n";
  static const char qcusage[] = "Pouzitie: ,<text>\n";
  const char *type;
  const char *name;
  UR_OBJECT u;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes hovorit.\n");
    return;
  }
  /* determine whether this is a quick call */
  if (*inpstr == ',') {
    if (!*user->call) {
      write_user(user, "Quick call este nemas nastaveny.\n");
      return;
    }
    u = get_user_name(user, user->call);
    /* if quick call with no message */
    if (word_count < 2) {
      write_user(user, qcusage);
      return;
    }
    inpstr = remove_first(inpstr);
  } else {
    /* if tell by itself, review tells */
    if (word_count < 2) {
      revtell(user);
      return;
    }
    u = get_user_name(user, word[1]);
    /* if tell <user> with no message */
    if (word_count < 3) {
      write_user(user, usage);
      return;
    }
    inpstr = remove_first(inpstr);
  }
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    vwrite_user(user, "Chces sa rozpravat sam%s so sebou ?!?!?\n", koncovka[user->gender]);
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ignoruje tvoje .tell-y.\n", u->recap);
    return;
  }
  if (u->igntells && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS momntalne ignoruje vsetky .tell-y.\n", u->recap);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  // type = smiley_type(inpstr); //
  // if (!type) { //
     type = "povedal";
  //}  //
  name = user->vis || u->level >= user->level ? user->recap : invisname;
  sprintf(text, "~OL%s~RS~OL ti povedal%s~RS: %s~RS\n", name, koncovka[u->gender], inpstr);
  if (u->afk) {
    record_afk(user, u, text);
    if (*u->afk_mesg) {
      vwrite_user(user, "%s~RS ma ~FRAFK~RS spravu : %s\n", u->recap,
                  u->afk_mesg);
    } else {
      vwrite_user(user, "%s~RS je prave ~FRAFK~RS.\n", u->recap);
    }
    write_user(user, "Sprava ulozena do afk review buffer-a.\n");
    return;
  }
  if (u->malloc_start) {
    record_edit(user, u, text);
    vwrite_user(user,
                "%s~RS je momentlne v ~FCEDIT~RS mode (pouziva line editor).\n",
                u->recap);
    write_user(user, "Sprava ulozena do edit review buffer-a.\n");
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS prave kazdeho ignoruje.\n",
                u->recap);
    return;
  }
#ifdef NETLINKS
  if (!u->room) {
    vwrite_user(user,
                "%s~RS is offsite and would not be able to reply to you.\n",
                u->recap);
    return;
  }
#endif
  record_tell(user, u, text);
  write_user(u, text);
  sprintf(text, "~RS~OLPovedal%s si %s~RS~OL:~RS %s~RS\n", koncovka[user->gender], u->dativ,
          inpstr);
  record_tell(user, user, text);
  write_user(user, text);
}


/*
 * Emote something
 */
void
emote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: emote <text>\n";
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzivat .emote.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  if (user->type == CLONE_TYPE) {
    sprintf(text, "Clone of %s~RS%s%s~RS\n", name, *inpstr != '\'' ? " " : "",
            inpstr);
    record(user->room, text);
    write_room(user->room, text);
    return;
  }
  sprintf(text, "%s~RS%s%s~RS\n", name, *inpstr != '\'' ? " " : "", inpstr);
  record(user->room, text);
  write_room_ignore(user->room, user, text);
}


/*
 * Do a shout emote
 */
void
semote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: semote <text>\n";
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .semote.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  if (!user->vis) {
    write_monitor(user, NULL, 0);
  }
  name = user->vis ? user->recap : invisname;
  sprintf(text, "~OL!~RS %s~RS%s%s~RS\n", name, *inpstr != '\'' ? " " : "",
          inpstr);
  record_shout(text);
  write_room_ignore(NULL, user, text);
}


/*
 * Do a private emote
 */
void
pemote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: pemote <uzivatel> <text>\n";
  const char *name;
  UR_OBJECT u;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .pemote.\n");
    return;
  }
  if (word_count < 3 && !strchr("</", *inpstr)) {
    write_user(user, usage);
    return;
  }
  if (word_count < 2 && strchr("</", *inpstr)) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    vwrite_user(user, "Sam%s sebe ?? Schiiiiza ??.\n", koncovka[user->gender]);
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ignoruje od teba vsetky .pemote.\n",
                u->recap);
    return;
  }
  if (u->igntells && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS mmntalne ignoruje vsetky .pemote.\n",
                u->recap);
    return;
  }
  inpstr = remove_first(inpstr);
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  name = user->vis || u->level >= user->level ? user->recap : invisname;
  sprintf(text, "~OL~FG>~RS %s~RS%s%s~RS\n", name, *inpstr != '\'' ? " " : "",
          inpstr);
  if (u->afk) {
    record_afk(user, u, text);
    if (*u->afk_mesg) {
      vwrite_user(user, "%s~RS~FRAFK~RS, sprava je: %s\n", u->recap,
                  u->afk_mesg);
    } else {
      vwrite_user(user, "%s~RS je prave ~FRAFK~RS.\n", u->recap);
    }
    write_user(user, "Posielam spravu do afk rev.bufferu.\n");
    return;
  }
  if (u->malloc_start) {
    record_edit(user, u, text);
    vwrite_user(user,
                "%s~RS mometalne nieco ~FCPISE~RS .\n",
                u->recap);
    write_user(user, "Sprava ulozena do jeho edit review buffer-a.\n");
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS prave vsetkych ignoruje.\n",
                u->recap);
    return;
  }
#ifdef NETLINKS
  if (!u->room) {
    vwrite_user(user,
                "%s~RS is offsite and would not be able to reply to you.\n",
                u->recap);
    return;
  }
#endif
  record_tell(user, u, text);
  write_user(u, text);
  sprintf(text, "~FG~OL>~RS (%s~RS) %s~RS%s%s~RS\n", u->recap, name,
          *inpstr != '\'' ? " " : "", inpstr);
  record_tell(user, user, text);
  write_user(user, text);
}


/*
 * Echo something to screen
 */
void
echo(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: echo <text>\n";

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .echo.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  write_monitor(user, user->room, 0);
  sprintf(text, "+ %s~RS\n", inpstr);
  record(user->room, text);
  write_room(user->room, text);
}


/*
 * Tell something to everyone but one person
 */
void
mutter(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: mutter <uzivatel> <text>\n";
  const char *type;
  const char *name, *n;
  UR_OBJECT u;

  if (word_count < 3) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user, "To myslis vazne ?\n");
    return;
  }
  if (user->room != u->room || (!u->vis && user->level < u->level)) {
    vwrite_user(user, "%s~RS tu nie, takze o nom mozes kludne hovorit...\n",
                u->recap);
    return;
  }
  inpstr = remove_first(inpstr);
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  type = smiley_type(inpstr);
  if (!type) {
    type = "mutter";
  }
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  n = u->vis ? u->recap : invisname;
  sprintf(text, "(NOT %s~RS) %s~RS ~FC%ss~RS: %s~RS\n", n, name, type,
          inpstr);
#if !!0
  record(user->room, text);
#endif
  write_room_except_both(user->room, text, user, u);
  vwrite_user(user, "(NOT %s~RS) You ~FC%s~RS: %s~RS\n", u->recap, type,
              inpstr);
}


/*
 * ask all the law, (sos), no muzzle restrictions
 */
void
plead(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: sos <text>\n";
  UR_OBJECT u;
  const char *type;

  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  if (user->level >= WIZ) {
    write_user(user, "Sam sebe .sos ? Ved si wizzard!\n");
    return;
  }
  for (u = user_first; u; u = u->next) {
    if (u->level >= WIZ && !u->login) {
      break;
    }
  }
  if (!u) {
    write_user(user, "Je mi luto, ale ziadni wizzi nie su online.\n");
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  type = smiley_type(inpstr);
  if (!type) {
    type = "plead";
  }
  vwrite_level(WIZ, 1, RECORD, user,
               "~OL~FG>~RS [~FRSOS~RS] %s~RS ~OL%ss~RS: %s~RS\n", user->recap,
               type, inpstr);
  sprintf(text, "~OL~FG>~RS [~FRSOS~RS] You ~OL%s~RS: %s~RS\n", type, inpstr);
  record_tell(user, user, text);
  write_user(user, text);
}


/*
 * Shout something to other wizes and gods. If the level isnt given it
 * defaults to WIZ level.
 */
void
wizshout(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: twiz [<level>] <text>\n";
  //const char *type;    //DAL SOM HET... NEPOTREBNE ZATIAL... ;))
  enum lvl_value lev;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .twiz.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  strtoupper(word[1]);
  lev = get_level(word[1]);
  if (lev == NUM_LEVELS) {
    lev = WIZ;
  } else {
    if (lev < WIZ || word_count < 3) {
      write_user(user, usage);
      return;
    }
    if (lev > user->level) {
      write_user(user,
                 "Nemozes kricat specialne len pre userof s vyssim levelom ako ty...\n");
      return;
    }
    inpstr = remove_first(inpstr);
  }
  /* Even wizzes cannot escapde the swear ban! MWHAHahaha.... ahem. */
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  /*type = smiley_type(inpstr);
  if (!type) {
    type = "Wizujes";
  }*/
  vwrite_level(lev, 1, RECORD, user,
               "~OL~FY>~RS|~FY%s~RS|~OL~FY %s wizuje:~RS~OL %s~RS\n",
               user_level[lev].name, user->name, inpstr);
  sprintf(text, "~OL~FY>~RS|~FY%s~RS| ~OL~FYWizujes:~RS~OL %s~RS\n",
          user_level[lev].name, inpstr);
  record_tell(user, user, text);
  write_user(user, text);
}


/*
 * Emote something to other wizes and gods. If the level isnt given it
 * defaults to WIZ level.
 */
void
wizemote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: ewiz [<level>] <text>\n";
  enum lvl_value lev;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .ewiz.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  strtoupper(word[1]);
  lev = get_level(word[1]);
  if (lev == NUM_LEVELS) {
    lev = WIZ;
  } else {
    if (lev < WIZ || word_count < 3) {
      write_user(user, usage);
      return;
    }
    if (lev > user->level) {
      write_user(user,
                 "Nemozes pozit specialne emote cisto len pre levely vyssie ako tvoj...\n");
      return;
    }
    inpstr = remove_first(inpstr);
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  vwrite_level(lev, 1, RECORD, NULL, "~OL~FG>~RS [~FY%s~RS] %s~RS%s%s~RS\n",
               user_level[lev].name, user->recap, *inpstr != '\'' ? " " : "",
               inpstr);
}


/*
 * Displays a picture to a person
 */
void
picture_tell(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: ptell <user> <picture>\n";
  char filename[80];
  const char *name;
  UR_OBJECT u;
  FILE *fp;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes posielat obrazky.\n");
    return;
  }
  if (word_count < 3) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user, "Existuje aj lahsia cesta ako si prezriet obrazok...\n");
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ignoruje obrazky od teba.\n", u->recap);
    return;
  }
  if (u->ignpics && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS prave ignoruje vsetky obrazky.\n",
                u->recap);
    return;
  }
  if (u->afk) {
    if (*u->afk_mesg) {
      vwrite_user(user, "%s~RS je ~FRAFK~RS: %s\n", u->recap,
                  u->afk_mesg);
    } else {
      vwrite_user(user, "%s~RS je prave ~FRAFK~RS.\n", u->recap);
    }
    return;
  }
  if (u->malloc_start) {
    vwrite_user(user, "%s~RS prave pise nejaku spravu.\n",
                u->recap);
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS prave nepocuva.\n", u->recap);
    return;
  }
#ifdef NETLINKS
  if (!u->room) {
    vwrite_user(user,
                "%s~RS is offsite and would not be able to reply to you.\n",
                u->recap);
    return;
  }
#endif
  if (strpbrk(word[2], "./")) {
    write_user(user, "Sorac, obrazok s takym nazvom neexistuje.\n");
    return;
  }
  sprintf(filename, "%s/%s", PICTFILES, word[2]);
  fp = fopen(filename, "r");
  if (!fp) {
    write_user(user, "Sorac, obrazok s takym nazvom nefunguje.\n");
    return;
  }
  fclose(fp);
  name = user->vis || u->level >= user->level ? user->recap : invisname;
  vwrite_user(u, "%s~RS ~OL~FG ti ukazuje obrazok...\n\n", name);
  switch (more(u, u->socket, filename)) {
  case 0:
    break;
  case 1:
    u->misc_op = 2;
    break;
  }
  vwrite_user(user, "~OL~FGUkazal si %s obrazok.~RS\n\n",
              u->dativ);
  switch (more(user, user->socket, filename)) {
  case 0:
    break;
  case 1:
    user->misc_op = 2;
    break;
  }
}


/*
 * see list of pictures availiable--file dictated in "go" script
 */
void
preview(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: preview [<picture>]\n";
#endif
  char filename[80], line[100];
  FILE *fp;
  DIR *dirp;
  struct dirent *dp;
  int cnt, total;

  if (word_count < 2) {
    /* open the directory file up */
    dirp = opendir(PICTFILES);
    if (!dirp) {
      write_user(user, "Neexistuje list dostupnych obrazkof....\n");
      return;
    }
    *line = '\0';
    cnt = total = 0;
    /* go through directory and list files */
    for (dp = readdir(dirp); dp; dp = readdir(dirp)) {
      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
        continue;
      }
      if (!total++) {
        write_user(user,
                   "+----------------------------------------------------------------------------+\n");
        write_user(user,
                   "| ~OL~FCDostupne ASCII obrazky~RS                                                 |\n");
        write_user(user,
                   "+----------------------------------------------------------------------------+\n");
      }
      sprintf(text, "%-12.12s   ", dp->d_name);
      strcat(line, text);
      if (++cnt == 5) {
        write_user(user, align_string(0, 78, 1, "|", "  %s", line));
        *line = '\0';
        cnt = 0;
      }
    }
    closedir(dirp);
    if (total) {
      if (cnt) {
        write_user(user, align_string(0, 78, 1, "|", "  %s", line));
      }
      write_user(user,
                 "+----------------------------------------------------------------------------+\n");
      write_user(user,
                 align_string(0, 78, 1, "|", "  Mame %d obrazkof  ", 
		 total));
      write_user(user,
                 "+----------------------------------------------------------------------------+\n\n");
    } else {
      write_user(user, "Ziadne dostupne obrazky.\n");
    }
    return;
  }
  if (strpbrk(word[1], "./")) {
    write_user(user, "Sorac, obrazok s tymto nazvom neexistuje.\n");
    return;
  }
  sprintf(filename, "%s/%s", PICTFILES, word[1]);
  fp = fopen(filename, "r");
  if (!fp) {
    write_user(user, "Sorac, obrazok s tymto nazvom neexistuje.\n");
    return;
  }
  fclose(fp);
  write_user(user, "~OL~FGPrezeras si obrazok...\n\n");
  switch (more(user, user->socket, filename)) {
  case 0:
    break;
  case 1:
    user->misc_op = 2;
    break;
  }
}


/*
 * Show a picture to the whole room that the user is in
 */
void
picture_all(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: picture <obrazok>\n";
  char filename[80];
  const char *name;
  UR_OBJECT u;
  FILE *fp;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes poslat obrazok.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  if (strpbrk(word[1], "./")) {
    write_user(user, "Sorac, obrazok s tymto menom thu nie je.\n");
    return;
  }
  sprintf(filename, "%s/%s", PICTFILES, word[1]);
  fp = fopen(filename, "r");
  if (!fp) {
    write_user(user, "Sorac obrazok s takymto menom thu nie je.\n");
    return;
  }
  fclose(fp);
  for (u = user_first; u; u = u->next) {
    if (u->login || !u->room || (u->room != user->room && user->room)
        || (u->ignall && !force_listen)
        || u->ignpics || u == user) {
      continue;
    }
    if (check_igusers(u, user) && user->level < GOD) {
      continue;
    }
    name = user->vis || u->level > user->level ? user->recap : invisname;
    if (u->type == CLONE_TYPE) {
      if (u->clone_hear == CLONE_HEAR_NOTHING || u->owner->ignall
          || u->clone_hear == CLONE_HEAR_SWEARS) {
        continue;
      }
      /*
       * Ignore anything not in clones room, eg shouts, system messages
       * and shemotes since the clones owner will hear them anyway.
       */
      if (user->room != u->room) {
        continue;
      }
      vwrite_user(u->owner,
                  "~FC[ %s ]:~RS %s~RS ~OL~FGukazuje obrazok...\n\n",
                  u->room->name, name);
      switch (more(u, u->socket, filename)) {
      case 0:
        break;
      case 1:
        u->misc_op = 2;
        break;
      }
    } else {
      vwrite_user(u, "%s~RS ~OL~FGukazuje obrazok...\n\n", name);
      switch (more(u, u->socket, filename)) {
      case 0:
        break;
      case 1:
        u->misc_op = 2;
        break;
      }
    }
  }
  write_user(user, "~OL~FGUkazujes obrazok vsetkym v tejto ruumke...\n\n");
  switch (more(user, user->socket, filename)) {
  case 0:
    break;
  case 1:
    user->misc_op = 2;
    break;
  }
}


/*
 * print out greeting in large letters
 */
void
greet(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: greet <text>\n";
  static const char *const clr[] =
    { "~OL~FR", "~OL~FY", "~OL~FG", "~OL~FC", "~OL~FB", "~OL~FM" };
  char pbuff[ARR_SIZE], temp[8];
  int slen, lc, c, i, j;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .greet.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  slen = strlen(inpstr);
  if (slen > 11) {
    write_user(user, "Mozes pouzit greet o maximalne dlzke 11 znakov.\n");
    return;
  }
  write_monitor(user, user->room, 0);
  write_room(user->room, "\n");
  for (i = 0; i < 5; ++i) {
    *pbuff = '\0';
    *temp = '\0';
    for (c = 0; c < slen; ++c) {
      /* check to see if it is a character a-z */
      if (isalpha(inpstr[c])) {
        lc = tolower(inpstr[c]) - 'a';
        if (lc >= 0 && lc < 26) {
          for (j = 0; j < 5; ++j) {
            if (biglet[lc][i][j]) {
              sprintf(temp, "%s#", clr[rand() % SIZEOF(clr)]);
              strcat(pbuff, temp);
            } else {
              strcat(pbuff, " ");
            }
          }
          strcat(pbuff, "  ");
        }
      }
      /* check if it is a character from ! to @ */
      if (ispunct(inpstr[c]) || isdigit(inpstr[c])) {
        lc = inpstr[c] - '!';
        if (lc >= 0 && lc < 32) {
          for (j = 0; j < 5; ++j) {
            if (bigsym[lc][i][j]) {
              sprintf(temp, "%s#", clr[rand() % SIZEOF(clr)]);
              strcat(pbuff, temp);
            } else {
              strcat(pbuff, " ");
            }
          }
          strcat(pbuff, "  ");
        }
      }
    }
    vwrite_room(user->room, "%s\n", pbuff);
  }
}


/*
 * put speech in a think bubbles
 */
void
think_it(UR_OBJECT user, char *inpstr)
{
#if !!0
  static const char usage[] = "Pouzitie: think [<text>]\n";
#endif
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, ani rozmyslat nemozes nahlas.\n");
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  if (word_count < 2) {
    sprintf(text, "%s~RS si nemysli nic... TYPICKE!\n", name);
  } else {
    sprintf(text, "%s~RS si mysli . o O ( %s~RS )\n", name, inpstr);
  }
  record(user->room, text);
  write_room(user->room, text);
}


/*
 * put speech in a music notes
 */
void
sing_it(UR_OBJECT user, char *inpstr)
{
#if !!0
  static const char usage[] = "Pouzitie: sing [<text>]\n";
#endif
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, takze ani spievat nemozes.\n");
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  if (!user->vis) {
    write_monitor(user, user->room, 0);
  }
  name = user->vis ? user->recap : invisname;
  if (word_count < 2) {
    sprintf(text, "%s~RS sa snazi zo seba vyludit ton ... ~OL~FRMARNA SNAHA!\n", name);
  } else {
    sprintf(text, "%s~RS ~OL~FGspieva ~RSo/~ %s~RS o/~\n", name, inpstr);
  }
  record(user->room, text);
  write_room(user->room, text);
}


/*
 * Broadcast an important message
 */
void
bcast(UR_OBJECT user, char *inpstr, int beeps)
{
  static const char usage[] = "Pouzitie: bcast <text>\n";
  static const char busage[] = "Pouzitie: bbcast <text>\n";

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .bcast.\n");
    return;
  }
  if (word_count < 2) {
    switch (beeps) {
    case 0:
      write_user(user, usage);
      return;
    case 1:
      write_user(user, busage);
      return;
    }
  }
  /* wizzes should be trusted...But they are not! */
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  force_listen = 1;
  write_monitor(user, NULL, 0);
  vwrite_room(NULL, "%s~OL~FR--==<~RS %s~RS ~OL~FR>==--~RS\n",
              beeps ? "\007" : "", inpstr);
}


/*
 * Wake up some idle sod
 */
void
wake(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: wake <uzivatel>\n";
  UR_OBJECT u;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes nikoho budit.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user,
               "Zobudit sam seba ??? Si daj radsej ~OL~FRREDBULL~RS.\n");
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ignoruje tvoje budicky.\n", u->recap);
    return;
  }
  if (u->ignbeeps && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS momentalne ignoruje vsetky budicky.\n", u->recap);
    return;
  }
  if (u->afk) {
    write_user(user, "Uzivatel je ~OL~FRmimo klavesnice~RS.\n");
    return;
  }
  if (u->malloc_start) {
    write_user(user, "Uzivatel prave pracuje na nejakom texte.\n");
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    write_user(user, "Uzivatel prave vsetko ignoruje.\n");
    return;
  }
#ifdef NETLINKS
  if (!u->room) {
    vwrite_user(user,
                "%s~RS is offsite and would not be able to reply to you.\n",
                u->recap);
    return;
  }
#endif
  name = user->vis ? user->recap : invisname;
  vwrite_user(u,
              "\n%s~BR***~RS %s~RS ~BRti huuuka do ucha~RS: ~OL~LI~BRHEY TY! ZOBUD SA!!!~RS ~BR***~RS\n\n",
              u->ignbeeps ? "" : "\007", name);
  write_user(user, "Budicek poslany...\n");
}


/*
 * Beep a user - as tell but with audio warning
 */
void
beep(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: beep <uzivatel> [<text>]\n";
  const char *name;
  UR_OBJECT u;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes tu len taq pipat.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user, "Pipat sam sebe? To ti je naco?\n");
    return;
  }
  if (check_igusers(u, user) && user->level < GOD) {
    vwrite_user(user, "%s~RS ignoruje tvoje pipacky.\n", u->recap);
    return;
  }
  if (u->ignbeeps && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS ignoruje pipanie.\n", u->recap);
    return;
  }
  if (u->afk) {
    write_user(user, "Nemozes pipat nekomu kto je mimo klavesnice.\n");
    return;
  }
  if (u->malloc_start) {
    vwrite_user(user, "%s~RS prave nieco pise.\n",
                u->recap);
    return;
  }
  if (u->ignall && (user->level < WIZ || u->level > user->level)) {
    vwrite_user(user, "%s~RS prave nepocuva.\n", u->recap);
    return;
  }
#ifdef NETLINKS
  if (!u->room) {
    vwrite_user(user,
                "%s~RS is offsite and would not be able to reply to you.\n",
                u->recap);
    return;
  }
#endif
  name = user->vis || u->level >= user->level ? user->recap : invisname;
  if (word_count < 3) {
    vwrite_user(u, "\007%s~RS ~OL~FRti peepa~RS: ~FR-=[*] BEEP [*]=-\n",
                name);
    vwrite_user(user, "\007~OL~FRPeepol si userovi >~RS %s~RS: ~FR-=[*] BEEP [*]=-\n",
                u->recap);
    return;
  }
  inpstr = remove_first(inpstr);
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  vwrite_user(u, "\007%s~RS ~OL~FRti peepa~RS: %s~RS\n", name, inpstr);
  vwrite_user(user, "\007~OL~FRPeepas userovi~RS %s~RS: %s~RS\n", u->recap,
              inpstr);
}


/*
 * set a name for a quick call
 */
void
quick_call(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: call [<user>]\n";
#endif
  UR_OBJECT u;

  if (word_count < 2) {
    if (!*user->call) {
      write_user(user, "Quick call nenastaveny.\n");
      return;
    }
    vwrite_user(user, "Quick call nastaveny na usera: %s.\n", user->call);
    return;
  }
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user, "Sam sebe? Quick call tho myslis vazne ?.\n");
    return;
  }
  strcpy(user->call, u->name);
  *user->call = toupper(*user->call);
  vwrite_user(user, "Nastavil si si Quick call na: %s.\n", user->call);
}


/*
 * Show recorded tells and pemotes
 */
void
revtell(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: revtell\n";
#endif

  start_pager(user);
  write_user(user, "\n~BB~FG*** Tvoj tell buffer ***\n\n");
  if (!review_buffer(user, rbfTELL)) {
    write_user(user, "Revtell buffer je prazdny.\n");
  }
  write_user(user, "\n~BB~FG*** Koniec ***\n\n");
  stop_pager(user);
}


/*
 * Show recorded tells and pemotes
 */
void
revedit(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: revedit\n";
#endif

  start_pager(user);
  write_user(user, "\n~BB~FG*** Tvoj EDIT review buffer ***\n\n");
  if (!review_buffer(user, rbfEDIT)) {
    write_user(user, "EDIT buffer je prazdny.\n");
  }
  write_user(user, "\n~BB~FG*** Koniec ***\n\n");
  stop_pager(user);
}


/*
 * Show recorded tells and pemotes
 */
void
revafk(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: revafk\n";
#endif

  start_pager(user);
  write_user(user, "\n~BB~FG*** Tvoj AFK review buffer ***\n\n");
  if (!review_buffer(user, rbfAFK)) {
    write_user(user, "AFK buffer je prazdny.\n");
  }
  write_user(user, "\n~BB~FG*** Koniec ***\n\n");
  stop_pager(user);
}


/*
 * See review of shouts
 */
void
revshout(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: revshout\n";
#endif
  int i, line, cnt;

  cnt = 0;
  start_pager(user);
  if (user->reverse_buffer) {
    for (i = REVIEW_LINES; i >= 0; --i) {
      line = (amsys->sbuffline + i) % REVIEW_LINES;
      if (*amsys->shoutbuff[line]) {
        if (!cnt++) {
          write_user(user, "~BB~FG*** Shout review buffer ***\n\n");
        }
        write_user(user, amsys->shoutbuff[line]);
      }
    }
  } else {
    for (i = 0; i < REVIEW_LINES; ++i) {
      line = (amsys->sbuffline + i) % REVIEW_LINES;
      if (*amsys->shoutbuff[line]) {
        if (!cnt++) {
          write_user(user, "~BB~FG*** Review buffer vykrikov ***\n\n");
        }
        write_user(user, amsys->shoutbuff[line]);
      }
    }
  }
  if (!cnt) {
    write_user(user, "Review buffer vykrikov je prazdny.\n");
  } else {
    write_user(user, "\n~BB~FG*** Koniec ***\n\n");
  }
  stop_pager(user);
}


/*
 * See review of conversation
 */
void
review(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: review\n";
#endif
  RM_OBJECT rm;
  int i, line, cnt;

  if (word_count < 2 || user->level < GOD) {
    rm = user->room;
  } else {
    rm = get_room(word[1]);
    if (!rm) {
      write_user(user, nosuchroom);
      return;
    }
    if (!has_room_access(user, rm)) {
      write_user(user,
                 "Ruumka je privatna, nemozes pozriet konverzaciu v tejto miestnosti.\n");
      return;
    }
    vwrite_user(user, "~FC(Review miestnosti > %s)\n", rm->name);
  }
  cnt = 0;
  start_pager(user);
  if (user->reverse_buffer) {
    for (i = REVIEW_LINES; i >= 0; --i) {
      line = (rm->revline + i) % REVIEW_LINES;
      if (*rm->revbuff[line]) {
        if (!cnt++) {
          write_user(user, "\n~BB~FG*** Buffer konverzacie v miestnosti ***\n\n");
        }
        write_user(user, rm->revbuff[line]);
      }
    }
  } else {
    for (i = 0; i < REVIEW_LINES; ++i) {
      line = (rm->revline + i) % REVIEW_LINES;
      if (*rm->revbuff[line]) {
        if (!cnt++) {
          write_user(user, "\n~BB~FG*** Buffer konverzacie v miestnosti ***\n\n");
        }
        write_user(user, rm->revbuff[line]);
      }
    }
  }
  if (!cnt) {
    write_user(user, "Review buffer je prazdny.\n");
  } else {
    write_user(user, "\n~BB~FG*** Koniec ***\n\n");
  }
  stop_pager(user);
}


/*
 * review from the user review buffer
 */
int
review_buffer(UR_OBJECT user, unsigned flags)
{
  int count = 0;
  RB_OBJECT rb, next;

  if (user->reverse_buffer) {
    for (rb = user->rb_last; rb; rb = next) {
      next = rb->prev;
      if (rb->flags & flags) {
        write_user(user, rb->buffer);
        ++count;
      }
    }
  } else {
    for (rb = user->rb_first; rb; rb = next) {
      next = rb->next;
      if (rb->flags & flags) {
        write_user(user, rb->buffer);
        ++count;
      }
    }
  }
  return count;
}


/*
 * Record speech and emotes in the room.
 */
void
record(RM_OBJECT rm, char *str)
{
  *rm->revbuff[rm->revline] = '\0';
  strncat(rm->revbuff[rm->revline], str, REVIEW_LEN);
  rm->revbuff[rm->revline][REVIEW_LEN] = '\n';
  rm->revbuff[rm->revline][REVIEW_LEN + 1] = '\0';
  rm->revline = (rm->revline + 1) % REVIEW_LINES;
}


/*
 * Records shouts and shemotes sent over the talker.
 */
void
record_shout(const char *str)
{
  *amsys->shoutbuff[amsys->sbuffline] = '\0';
  strncat(amsys->shoutbuff[amsys->sbuffline], str, REVIEW_LEN);
  amsys->shoutbuff[amsys->sbuffline][REVIEW_LEN] = '\n';
  amsys->shoutbuff[amsys->sbuffline][REVIEW_LEN + 1] = '\0';
  amsys->sbuffline = (amsys->sbuffline + 1) % REVIEW_LINES;
}


/*
 * Records tells and pemotes sent to the user.
 */
void
record_tell(UR_OBJECT from, UR_OBJECT to, const char *str)
{
  int count;

  if (!create_review_buffer_entry(to, !from ? "?" : from->name, str, rbfTELL)) {
    write_syslog(ERRLOG, 1,
                 "Neda sa vytvorit tell review buffer vstup pre %s.\n",
                 to->name);
    return;
  }
  /* check to see if we need to prune */
  count = has_review(to, rbfTELL);
  if (count > REVTELL_LINES) {
    destruct_review_buffer_type(to, rbfTELL, 1);
  }
}


/*
 * Records tells and pemotes sent to the user when afk.
 */
void
record_afk(UR_OBJECT from, UR_OBJECT to, const char *str)
{
  int count;

  if (!create_review_buffer_entry(to, !from ? "?" : from->name, str, rbfAFK)) {
    write_syslog(ERRLOG, 1,
                 "Neda sa vytvorit afk review buffer vstup pre %s.\n",
                 to->name);
    return;
  }
  /* check to see if we need to prune */
  count = has_review(to, rbfAFK);
  if (count > REVTELL_LINES) {
    destruct_review_buffer_type(to, rbfAFK, 1);
  }
}


/*
 * Records tells and pemotes sent to the user when in the line editor.
 */
void
record_edit(UR_OBJECT from, UR_OBJECT to, const char *str)
{
  int count;

  if (!create_review_buffer_entry(to, !from ? "?" : from->name, str, rbfEDIT)) {
    write_syslog(ERRLOG, 1,
                 "Neda sa vytvorit edit review buffer vstup pre %s.\n",
                 to->name);
    return;
  }
  /* check to see if we need to prune */
  count = has_review(to, rbfEDIT);
  if (count > REVTELL_LINES) {
    destruct_review_buffer_type(to, rbfEDIT, 1);
  }
}


/*
 * Clear the review buffer
 */
void
revclr(UR_OBJECT user)
{
#if !!0
  static const char usage[] = "Pouzitie: cbuff\n";
#endif
  const char *name;

  clear_revbuff(user->room);
  name = user->vis ? user->recap : invisname;
  vwrite_room_except(user->room, user, "%s~RS vymazal%s review buffer.\n",
                     name, koncovka[user->gender]);
  write_user(user, "Vymazal si review buffer.\n");
}


/*
* Clear the tell buffer of the user
*/
void
clear_afk(UR_OBJECT user)
{
  destruct_review_buffer_type(user, rbfAFK, 0);
}


/*
 * Clear the review buffer in the room
 */
void
clear_revbuff(RM_OBJECT rm)
{
  int i;

  for (i = 0; i < REVIEW_LINES; ++i) {
    *rm->revbuff[i] = '\0';
  }
  rm->revline = 0;
}


/*
 * Clear the tell buffer of the user
 */
void
clear_tells(UR_OBJECT user)
{
  destruct_review_buffer_type(user, rbfTELL, 0);
}


/*
 * Clear the shout buffer of the talker
 */
void
clear_shouts(void)
{
  int i;

  for (i = 0; i < REVIEW_LINES; ++i) {
    *amsys->shoutbuff[i] = '\0';
  }
  amsys->sbuffline = 0;
}


/*
 * Clear the tell buffer of the user
 */
void
clear_edit(UR_OBJECT user)
{
  destruct_review_buffer_type(user, rbfEDIT, 0);
}

/*
 * count up how many review buffers of a certain type
 */
int
has_review(UR_OBJECT user, unsigned flags)
{
  int count = 0;
  RB_OBJECT rb;

  for (rb = user->rb_first; rb; rb = rb->next) {
    if (rb->flags & flags) {
      ++count;
    }
  }
  return count;
}


/*
 * Clear the screen
 */
void
cls(UR_OBJECT user)
{
  int i;

  for (i = 0; i < 5; ++i) {
    write_user(user, "\n\n\n\n\n\n\n\n\n\n");
  }
}


/*
 * Make a clone speak
 */
void
clone_say(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: csay <room> <text>\n";
  RM_OBJECT rm;
  UR_OBJECT u;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pouzit .csay.\n");
    return;
  }
  if (word_count < 3) {
    write_user(user, usage);
    return;
  }
  rm = get_room(word[1]);
  if (!rm) {
    write_user(user, nosuchroom);
    return;
  }
  for (u = user_first; u; u = u->next) {
    if (u->type == CLONE_TYPE && u->room == rm && u->owner == user) {
      break;
    }
  }
  if (!u) {
    write_user(user, "Nemas na tom mieste ziadneho klona.\n");
    return;
  }
  say(u, remove_first(inpstr));
}


/*
 * Make a clone emote
 */
void
clone_emote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: cemote <room> <text>\n";
  RM_OBJECT rm;
  UR_OBJECT u;

  if (user->muzzled > 1) {
    write_user(user, "Si umlcany, nemozes pouzit .cemote.\n");
    return;
  }
  if (word_count < 3) {
    write_user(user, usage);
    return;
  }
  rm = get_room(word[1]);
  if (!rm) {
    write_user(user, nosuchroom);
    return;
  }
  for (u = user_first; u; u = u->next) {
    if (u->type == CLONE_TYPE && u->room == rm && u->owner == user) {
      break;
    }
  }
  if (!u) {
    write_user(user, "Na tom mieste nemas klona.\n");
    return;
  }
  emote(u, remove_first(inpstr));
}


/*
 * Set what a clone will hear, either all speach , just bad language
 * or nothing.
 */
void
clone_hear(UR_OBJECT user)
{
  static const char usage[] = "Pouzitie: chear <room> all|swears|nothing\n";
  RM_OBJECT rm;
  UR_OBJECT u;

  if (word_count < 3) {
    write_user(user, usage);
    return;
  }
  rm = get_room(word[1]);
  if (!rm) {
    write_user(user, nosuchroom);
    return;
  }
  for (u = user_first; u; u = u->next) {
    if (u->type == CLONE_TYPE && u->room == rm && u->owner == user) {
      break;
    }
  }
  if (!u) {
    write_user(user, "Na tom mieste nemas ziadneho klona.\n");
    return;
  }
  strtolower(word[2]);
  if (!strcmp(word[2], "all")) {
    u->clone_hear = CLONE_HEAR_ALL;
    write_user(user, "Klon bude odteraz pocuvat vsetko.\n");
    return;
  }
  if (!strcmp(word[2], "swears")) {
    u->clone_hear = CLONE_HEAR_SWEARS;
    write_user(user, "Klon bude odteraz pocuvat len nadavky.\n");
    return;
  }
  if (!strcmp(word[2], "nothing")) {
    u->clone_hear = CLONE_HEAR_NOTHING;
    write_user(user, "Klon nebude pocuvat nic.\n");
    return;
  }
  write_user(user, usage);
}


/*
 * Show command, i.e., "Type --> <text>"
 */
void
show(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: show <text>\n";

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes pozit .show.\n");
    return;
  }
  if (word_count < 2 && strlen(inpstr) < 2) {
    write_user(user, usage);
    return;
  }
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    if (!is_private_room(user->room)) {
      inpstr = censor_swear_words(inpstr);
    }
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  write_monitor(user, user->room, 0);
  vwrite_room(user->room, "~OL~FCType -->~RS %s~RS\n", inpstr);
}


/*
 * Say user speech to all people listed on users friends list
 */
void
friend_say(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: fsay <text>\n";
  const char *type;
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes rozpravat.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  /* check to see if use has friends listed */
  if (!count_friends(user)) {
    write_user(user, "Nemas userov vo friends liste.\n");
    return;
  }
  /* sort our swearing */
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  type = smiley_type(inpstr);
  if (!type) {
    type = "say";
  }
  name = user->vis ? user->recap : invisname;
  sprintf(text, "~OL~FG>~RS [~FGKaaamos~RS] %s~RS ~FG%ss~RS: %s~RS\n", name,
          type, inpstr);
  write_friends(user, text, 1);
  sprintf(text, "~OL~FG>~RS [~FGKaaamos~RS] You ~FG%s~RS: %s~RS\n", type,
          inpstr);
  record_tell(user, user, text);
  write_user(user, text);
}


/*
 * Emote something to all the people on the suers friends list
 */
void
friend_emote(UR_OBJECT user, char *inpstr)
{
  static const char usage[] = "Pouzitie: femote <text>\n";
  const char *name;

  if (user->muzzled) {
    write_user(user, "Si umlcany, nemozes emotovat.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, usage);
    return;
  }
  /* check to see if use has friends listed */
  if (!count_friends(user)) {
    write_user(user, "Ziadny ludia vo friends liste.\n");
    return;
  }
  /* sort out swearing */
  switch (amsys->ban_swearing) {
  case SBMAX:
    if (contains_swearing(inpstr)) {
      write_user(user, noswearing);
      return;
    }
    break;
  case SBMIN:
    inpstr = censor_swear_words(inpstr);
    break;
  case SBOFF:
  default:
    /* do nothing as ban_swearing is off */
    break;
  }
  name = user->vis ? user->recap : invisname;
  sprintf(text, "~OL~FG>~RS [~FGKaaamos~RS] %s~RS%s%s~RS\n", name,
          *inpstr != '\'' ? " " : "", inpstr);
  write_friends(user, text, 1);
  record_tell(user, user, text);
  write_user(user, text);
}
