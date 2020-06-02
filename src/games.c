/****************************************************************************
*******************************ANGEL TALKER**********************************
               Angel version 1.0.0 - Copyright (C) Week,  2005

                            zalozeny na zaklade

         Amnuts version 2.3.0 - Copyright (C) Andrew Collington, 2001
				   
   NUTS version 3.3.3 (Triple Three :) - Copyright (C) Neil Robertson 1996
 ***************************************************************************/

#ifndef GAMES
#define NO_GAMES
#else

#include "defines.h"
#include "globals.h"
#include "commands.h"
#include "prototypes.h"

/*****************************************************************************/

/*
 * lets a user start, stop or check out their status of a game of hangman
 */
void
play_hangman(UR_OBJECT user)
{
  int i;

  if (word_count < 2) {
    write_user(user, "Pozitie: hangman start|stop|status\n");
    return;
  }
  strtolower(word[1]);
  if (!strcmp("status", word[1])) {
    if (user->hang_stage == -1) {
      write_user(user, "Este nehras hru hangman.\n");
      return;
    }
    write_user(user, "Tvoj hangman status je:\n");
    if (strlen(user->hang_guess) < 1) {
      vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
                  "Ziadne!");
    } else {
      vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
                  user->hang_guess);
    }
    write_user(user, "\n");
    return;
  }
  if (!strcmp("stop", word[1])) {
    if (user->hang_stage == -1) {
      write_user(user, "Este nehras hru hangman.\n");
      return;
    }
    user->hang_stage = -1;
    *user->hang_word = '\0';
    *user->hang_word_show = '\0';
    *user->hang_guess = '\0';
    write_user(user, "Prestal si hrat hangman-a.\n");
    return;
  }
  if (!strcmp("start", word[1])) {
    if (user->hang_stage != -1) {
      write_user(user, "Uz hras hangman-a.\n");
      return;
    }
    get_hang_word(user->hang_word);
    strcpy(user->hang_word_show, user->hang_word);
    for (i = 0; user->hang_word_show[i]; ++i) {
      user->hang_word_show[i] = '-';
    }
    user->hang_stage = 0;
    write_user(user, "Tvoj hangman status:\n\n");
    vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
                "Ziadne!");
    return;
  }
  write_user(user, "Usage: hangman start|stop|status\n");
}


/*
 * returns a word from a list for hangman.
 * this will save loading words into memory, and the list could be updated as and when
 * you feel like it
 */
void
get_hang_word(char *aword)
{
  char filename[80];
  FILE *fp;
  int cnt;

  /* if no word is found, just return a generic word */
  strcpy(aword, "hangman");
  sprintf(filename, "%s/%s", MISCFILES, HANGDICT);
  cnt = count_lines(filename);
  if (!cnt) {
    return;
  }
  fp = fopen(filename, "r");
  if (!fp) {
    return;
  }
  cnt = rand() % cnt;
  for (fscanf(fp, "%s\n", aword); !feof(fp); fscanf(fp, "%s\n", aword)) {
    if (!cnt--) {
      break;
    }
  }
  fclose(fp);
}


/*
 * Lets a user guess a letter for hangman
 */
void
guess_hangman(UR_OBJECT user)
{
  int count, i, blanks;

  if (user->hang_stage == -1) {
    write_user(user, "Este si nedal .hangman start.\n");
    return;
  }
  if (word_count < 2) {
    write_user(user, "Pozitie: guess <pismeno>\n");
    return;
  }
  if (strlen(word[1]) != 1) {
    write_user(user, "Mozes hadat len 1 pismeno!\n");
    return;
  }
  *word[1] = tolower(*word[1]);
  if (strchr(user->hang_guess, *word[1])) {
    ++user->hang_stage;
    write_user(user,
               "Toto pismeno uz bolo hadane... ;)) .\n\n");
    vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
                user->hang_guess);
    if (user->hang_stage >= 7) {
      write_user(user,
                 "~FR~OLBlaaaah!~RS Tvoje telo sa hopala na snure!\n");
      user->hang_stage = -1;
      *user->hang_word = '\0';
      *user->hang_word_show = '\0';
      *user->hang_guess = '\0';
    }
    write_user(user, "\n");
    return;
  }
  count = blanks = i = 0;
  for (i = 0; user->hang_word[i]; ++i) {
    if (user->hang_word[i] == *word[1]) {
      user->hang_word_show[i] = user->hang_word[i];
      ++count;
    }
    if (user->hang_word_show[i] == '-') {
      ++blanks;
    }
  }
  strcat(user->hang_guess, word[1]);
  if (!count) {
    ++user->hang_stage;
    write_user(user,
               "Toto pismeno sa v slove nenachadza...\n");
    vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
                user->hang_guess);
    if (user->hang_stage >= 7) {
      write_user(user,
                 "~FR~OLBlaaaah!~RS Tvoje telo sa hompala na snure!\n");
      user->hang_stage = -1;
      *user->hang_word = '\0';
      *user->hang_word_show = '\0';
      *user->hang_guess = '\0';
    }
    write_user(user, "\n");
    return;
  }
  if (count == 1) {
    vwrite_user(user, "Dobre! Vyskyt v slove > 'jeden' ;)) %s\n",
                word[1]);
  } else {
    vwrite_user(user,
                "Vyborne! V slove su %d taqeto pismena %s\n",
                count, word[1]);
  }
  vwrite_user(user, hanged[user->hang_stage], user->hang_word_show,
              user->hang_guess);
  if (!blanks) {
    write_user(user,
               "~FY~OLBlahozelam!~RS Tvoje telo neodvislo na sibenici!\n");
    user->hang_stage = -1;
    *user->hang_word = '\0';
    *user->hang_word_show = '\0';
    *user->hang_guess = '\0';
  }
}


/*
 * Shoot another user... Fun! Fun! Fun! ;)
 */
void
shoot_user(UR_OBJECT user)
{
  UR_OBJECT user2;
  RM_OBJECT rm;
  int prob1, prob2;

  rm = get_room(default_shoot);
  if (!rm) {
    write_user(user, "There is nowhere that you can shoot.\n");
    return;
  }
  if (user->room != rm) {
    vwrite_user(user,
                "Do not be shooting in a public place. Go to the ~OL%s~RS to play.\n",
                rm->name);
    return;
  }
  if (word_count < 2) {
    if (!user->bullets) {
      vwrite_room_except(rm, user,
                         "%s~RS's gun goes *click* as they pull the trigger.\n",
                         user->recap);
      write_user(user, "Your gun goes *click* as you pull the trigger.\n");
      return;
    }
    vwrite_room_except(rm, user, "%s~RS fires their gun off into the air.\n",
                       user->recap);
    write_user(user, "You fire your gun off into the air.\n");
    --user->bullets;
    return;
  }
  prob1 = rand() % 100;
  prob2 = rand() % 100;
  user2 = get_user_name(user, word[1]);
  if (!user2) {
    write_user(user, notloggedon);
    return;
  }
  if (!user->vis) {
    write_user(user,
               "Be fair! At least make a decent target--do not be invisible!\n");
    return;
  }
  if ((!user2->vis && user2->level < user->level) || user2->room != rm) {
    write_user(user, "You cannot see that person around here.\n");
    return;
  }
  if (user == user2) {
    write_user(user, "Watch it! You might shoot yourself in the foot!\n");
    return;
  }
  if (!user->bullets) {
    vwrite_room_except(rm, user,
                       "%s~RS's gun goes *click* as they pull the trigger.\n",
                       user->recap);
    write_user(user, "Your gun goes *click* as you pull the trigger.\n");
    return;
  }
  if (prob1 > prob2) {
    vwrite_room(rm, "A bullet flies from %s~RS's gun and ~FR~OLhits~RS %s.\n",
                user->recap, user2->recap);
    --user->bullets;
    ++user->hits;
    --user2->hps;
    write_user(user2, "~FR~OLYou have been hit!\n");
    write_user(user, "~FG~OLGood shot!\n");
    if (user2->hps < 1) {
      ++user2->deaths;
      vwrite_user(user,
                  "\nYou have won the shoot out, %s~RS is dead!  You may now rejoice!\n",
                  user2->recap);
      write_user(user2,
                 "\nYou have received a fatal wound, and you feel your warm ~FRblood ~OLooze~RS out of you.\n");
      write_user(user2, "The room starts to fade and grow grey...\n");
      write_user(user2,
                 "In the bleak mist of Death's shroud you see a man walk towards you.\n");
      write_user(user2,
                 "The man is wearing a tall black hat, and a wide grin...\n\n");
      user2->hps = 5 * user2->level;
      write_syslog(SYSLOG, 1, "%s shot dead by %s\n", user2->name,
                   user->name);
      disconnect_user(user2);
      ++user->kills;
      user->hps = user->hps + 5;
      return;
    }
    return;
  }
  vwrite_room(rm,
              "A bullet flies from %s~RS's gun and ~FG~OLmisses~RS %s~RS.\n",
              user->recap, user2->recap);
  --user->bullets;
  ++user->misses;
  write_user(user2, "~FGThat was a close shave!\n");
  write_user(user, "~FRYou could not hit the side of a barn!\n");
}


/*
 * well...Duh! Reload the gun
 */
void
reload_gun(UR_OBJECT user)
{
  RM_OBJECT rm;

  rm = get_room(default_shoot);
  if (!rm) {
    write_user(user, "There is nowhere that you can shoot.\n");
    return;
  }
  if (user->room != rm) {
    vwrite_user(user,
                "Do not be shooting in a public place. Go to the ~OL%s~RS to play.\n",
                rm->name);
    return;
  }
  if (user->bullets > 0) {
    vwrite_user(user, "You have ~OL%d~RS bullets left.\n", user->bullets);
    return;
  }
  vwrite_room_except(user->room, user, "~FY%s reloads their gun.\n",
                     user->bw_recap);
  write_user(user, "~FYYou reload your gun.\n");
  user->bullets = 6;
}


/*
 * allows a user to store some of the money they have in their hand
 */
void
bank_money(UR_OBJECT user)
{
  static const char *const what[] = { ".", ", Pan.", ", Pani." };
  int money;

  if (word_count < 2) {
    write_user(user, "Pouzitie: bank deposit/withdraw/balance [<suma>]\n");
    return;
  }
  if (strcmp(user->room->name, default_bank)) {
    vwrite_user(user,
                "Musis byt v miestnosti ~OL%s~RS aby si mohol narabat so zatkami.\n",
                default_bank);
    return;
  }
  if (!strcasecmp("balance", word[1])) {
    vwrite_user(user, "~FCThe teller says:~RS V banke mas presne %d zatok%s\n",
                user->bank, what[user->gender]);
    return;
  }
  if (!strcasecmp("deposit", word[1])) {
    if (word_count < 3) {
      write_user(user, "Pozitie: bank deposit/withdraw/balance [<suma>]\n");
      return;
    }
    money = atoi(word[2]);
    if (money > user->money) {
      write_user(user, "Nemas u seba tolko zatok.\n");
      return;
    }
    if (!money) {
      write_user(user,
                 "Zabudol som napisat kolko zatok si prajes vlozit.\n");
      return;
    }
    if (money < 0) {
      write_user(user,
                 "Ak chces z banky vytiahnut svoje zatky pouzi moznost \"withdraw\".\n");
      return;
    }
    user->money -= money;
    user->bank += money;
    vwrite_user(user,
                "Vlozil si $%d zatok na svoje konto, vo vrecku ti zostava $%d zatok.\n",
                money, user->money);
    return;
  }
  if (!strcasecmp("withdraw", word[1])) {
    if (word_count < 3) {
      write_user(user, "Pouzitie: bank deposit/withdraw/balance [<suma>]\n");
      return;
    }
    money = atoi(word[2]);
    if (money > user->bank) {
      write_user(user, "Nemas tolko zatok na svojom ucte.\n");
      return;
    }
    if (!money) {
      write_user(user,
                 "Zabudol si napisat kolko zatok si chces vybrat.\n");
      return;
    }
    if (money < 0) {
      write_user(user,
                 "Ak chces vlozit zatky na svoj ucet, pouzi moznost \"deposit\" \n");
      return;
    }
    user->money += money;
    user->bank -= money;
    vwrite_user(user,
                "Vybral si si %d zatok z tvojho uctu. Zostalo ti ich tam presne %d \n",
                money, user->bank);
    return;
  }
  write_user(user, "Pouzitie: bank deposit/withdraw/balance [<suma>]\n");
}


/*
 * give some cash to another user
 */
void
donate_cash(UR_OBJECT user)
{
  UR_OBJECT u;
  const char *name;
  int cash;

  if (word_count < 3) {
    write_user(user, "Pouzitie: donate <user> <suma>\n");
    return;
  }
  *word[1] = toupper(*word[1]);
  u = get_user_name(user, word[1]);
  if (!u) {
    write_user(user, notloggedon);
    return;
  }
  if (u == user) {
    write_user(user, "You cannot donate money to yourself.\n");
    return;
  }
  cash = atoi(word[2]);
  if (cash > MAX_DONATION) {
    vwrite_user(user, "You cannot donate more than $%d.\n", MAX_DONATION);
    return;
  }
  if (cash < 0) {
    write_user(user, "Now do not be trying to steal money from them!\n");
    return;
  }
  if (!cash) {
    write_user(user,
               "If you are going to donate money, at least donate something!\n");
    return;
  }
  if (user->money < cash) {
    write_user(user, "You have not got that much money on you right now.\n");
    return;
  }
  u->money += cash;
  user->money -= cash;
  name = user->vis || u->level < WIZ ? user->recap : invisname;
  vwrite_user(user, "You donate $%d out of your own pocket to %s~RS.\n", cash,
              u->recap);
  vwrite_user(u, "%s~RS donates $%d to you out of their own pocket.\n", name,
              cash);
  sprintf(text, "%s donates $%d.\n", user->name, cash);
  add_history(u->name, 1, text);
}


/*
 * show the user how much money they have
 */
void
show_money(UR_OBJECT user)
{
  if (!user->money) {
    write_user(user, "Nemas u seba ziadne zatky.\n");
    return;
  }
  vwrite_user(user, "U seba mas presne ~OL~FC%d~RS zatok.\n", user->money);
}


/*
 * Add in the credits system
 */
void
check_credit_updates(void)
{
  UR_OBJECT u, next;

  for (u = user_first; u; u = next) {
    next = u->next;
    /* only update credits for users who qualify */
    if (u->level >= MIN_CREDIT_UPDATE_LEVEL && !u->afk && !u->login
        && (int) (time(0) - u->last_input) < amsys->user_idle_time) {
      u->inctime += amsys->heartbeat;
      /* work out how many credits per hour */
      if (!(u->inctime % (int) (3600 / CREDITS_PER_HOUR))) {
        u->inctime = 0;
        ++u->money;
      }
    }
  }
}


/*
 * give, take and view money of users currently logged on
 */
void
global_money(UR_OBJECT user)
{
  UR_OBJECT u;
  const char *name;
  int cash;

  if (word_count < 2) {
    write_user(user, "Pouzitie: money -l/-g/-t [<user> <amount>]\n");
    return;
  }
  /* list all users online and the amount of cash they have */
  if (!strcasecmp(word[1], "-l")) {
    char text2[ARR_SIZE];
    int x, cnt, user_cnt;

    write_user(user,
               "\n+----------------------------------------------------------------------------+\n");
    write_user(user,
               "| ~FC~OLKto ma kolko zatok u seba~RS                                           |\n");
    write_user(user,
               "+----------------------------------------------------------------------------+\n");
    x = user_cnt = 0;
    *text2 = '\0';
    for (u = user_first; u; u = u->next) {
      ++user_cnt;
      cnt = 13 + teslen(u->recap, 13);
      if (!x) {
	/* build up first half of the string */
        sprintf(text, "| %-*.*s $%6d ", cnt, cnt, u->recap, u->money);
        ++x;
      } else if (x == 1) {
	/* build up full line and print to user */
        sprintf(text2, "   %-*.*s $%6d   ", cnt, cnt, u->recap, u->money);
        strcat(text, text2);
        write_user(user, text);
        *text = '\0';
        *text2 = '\0';
        ++x;
      } else {
        sprintf(text2, "   %-*.*s $%6d  |\n", cnt, cnt, u->recap, u->money);
        strcat(text, text2);
        write_user(user, text);
        *text = '\0';
        *text2 = '\0';
        x = 0;
      }
    }
    /* If you have only printed first half of the string */
    if (x == 1) {
      strcat(text,
             "                                                     |\n");
      write_user(user, text);
    }
    if (x == 2) {
      strcat(text, "                          |\n");
      write_user(user, text);
    }
    write_user(user,
               "+----------------------------------------------------------------------------+\n");
    sprintf(text, "Total of ~OL%d~RS user%s", user_cnt, PLTEXT_S(user_cnt));
    vwrite_user(user, "| %-80s |\n", text);
    write_user(user,
               "+----------------------------------------------------------------------------+\n\n");
    return;
  }
  /* give money to users */
  if (!strcasecmp(word[1], "-g")) {
    if (word_count < 4) {
      write_user(user, "Pouzitie: money -l/-g/-t [<user> <amount>]\n");
      return;
    }
    u = get_user_name(user, word[2]);
    if (!u) {
      write_user(user, notloggedon);
      return;
    }
    if (u == user && user->level < GOD) {
      write_user(user, "Nemozes dat zatky sam sebe...\n");
      return;
    }
    cash = atoi(word[3]);
    if (cash < 1) {
      write_user(user, "Sak nemas taky velky CASH...\n");
      return;
    }
    u->money += cash;
    name = user->vis || u->level < WIZ ? user->recap : invisname;
    vwrite_user(user, "Dal si $%d userovi %s~RS.\n", cash, u->recap);
    vwrite_user(u, "%s~RS ti dal(a) $%d.\n", name, cash);
    sprintf(text, "%s dostal $%d.\n", user->name, cash);
    add_history(u->name, 1, text);
    return;
  }
  /* take money from users */
  if (!strcasecmp(word[1], "-t")) {
    if (word_count < 4) {
      write_user(user, "Pouzitie: money -l/-g/-t [<user> <amount>]\n");
      return;
    }
    u = get_user_name(user, word[2]);
    if (!u) {
      write_user(user, notloggedon);
      return;
    }
    if (u == user) {
      write_user(user, "You cannot take money away from yourself.\n");
      return;
    }
    cash = atoi(word[3]);
    if (cash < 1) {
      write_user(user, "You must supply an amount to take.\n");
      return;
    }
    if (u->money < cash) {
      vwrite_user(user, "%s~RS has not got that much money.\n", u->recap);
      return;
    }
    u->money -= cash;
    name = user->vis || u->level < WIZ ? user->recap : invisname;
    vwrite_user(user, "You take $%d from %s~RS.\n", cash, u->recap);
    vwrite_user(u, "%s~RS takes $%d from you.\n", name, cash);
    sprintf(text, "%s takes $%d.\n", user->name, cash);
    add_history(u->name, 1, text);
    return;
  }
  write_user(user, "Usage: money -l/-g/-t [<user> <amount>]\n");
}


#endif /* ifndef GAMES */
