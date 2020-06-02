/*****************************************************************************
         Base code for writing Bots for telnet based talker or MUD
         (C) 1998/2002 angel Talker Software (atlantis@talker.sk)

	Note: function "strlow()" and "curword()" are not yet used,
                  but may be very usefull in the future
*****************************************************************************

---*** News ***---
Verzia 2.2b
	- fixnuta verzia s fix #3, odstranene problemy s nalogovanim
Verzia 2.2
	- auto ignore pri castom nadavani cez telly (D:))

Verzia 2.1
        - oprava drobnych pravopisnych chyb
        - odstraneny paramater WIZARD pri wizshout

Verzia 2.0
	- zgruntu novy engin
	- shouty & "inteligencne" rutinky ostali povodne, rovnako ako reakcie
	  na kiss, hug a podobne + male upravy.
	- vyhodene flood detection, prohibit a vsetky zbytocne interne prikazy
	- exec command je rieseny zistovanim bodky na zaciatku riadku
Verzia (sk)
	- naucil sa regovat na slovencinu (R) :)
Fix #1  - fixnuty *skaredy* bug, ktory umoznoval ovladat Poseidona prakticky
          hocikomu! (thanx to Shaio!) (R)
Fix #2  - fixnuty mensi bug pri kisse, ktory umoznoval ciastocne ovladanie
          Poseidona (co si vsimol Pcolka) + komplexne osetrenie macrobugu
	  priamo v readtis (toz, bol vyhodeny fix#1, lebo toto robi to iste,
	  ale globalne) (R)
	- pridane Indyho hlasky na temy zivot, poseidon, atlantis, muzi a zeny
	  (I)
	- opraveny sposob citania hlasok z Atlantisu + opraveny timeout s kecmi
	  o nevsimavosti (konecne je tam select() ;-). Ked sa k tomu nikto
	  nemal... (R)
Fix #3  - fixnuta chyba z Tisu -> char(127) (thanx to Dusky!) (R)
*****************************************************************************/


#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TISHOST		"localhost"
#define TISPORT		9999

#define MYNAME		"mutant"
#define MYPASS		"mutantos"
#define COLOUR		".set colour"

#define VERSION		"Poseidon v2.2b-sk (C)1998/2002 Rider of LoneStar Software"
#define MAXSTRING 4096            /* maximalna dlzka stringu */
#define CAKAJ 1200		  /* kedy zacne jacat, ze ho ignoruju ;-) */
#define MAXUSERS 100		  /* maximalny pocet zapamatanych uzivatelov */
#define IGNTIMEOUT 60		  /* cas po ktorom znizujeme pocet nadavok */
#define MAXSWEARS 3 		  /* limit, po jeho prekroceni zacne usera ignorovat */
#define LOGFILE "/root/mutant.log"
/* globalna premenna na bordeltext :) */
char text[MAXSTRING];
int ftis, ttis;
int debug=1;
int loging=1;

/* Koho bude Mutant posluchat, posledne musi byt hviezdicka */
char *masters[]={"kapo","kac","ict","*"};

struct user {
	char name[16];
	int swear_num;
	int ignore;
	time_t time;
};
struct user *users = NULL;
int num_users = 0;

char *gettis ();
char *strlow();
char *curword();
int  connecttis();
int  isready();
void sendtis(char *);
void writelog(char *);
int  quit_robot(int);
int ovlada(char *);
char *blbec(char *, char *);

int new_user() {
	struct user *newusers;
	
	if ((num_users < MAXUSERS) && (newusers = realloc(users, (num_users + 1) * sizeof(struct user)))) {
		users=newusers;
		bzero(&users[num_users], sizeof(struct user));
		num_users++;
		return num_users-1;
	}
	return -1;
}

void del_user(int index) {
	struct user *newusers;
	
	if (index >= num_users) return;
	memcpy(&users[index], &users[index+1], sizeof(struct user) * (num_users-index-1));
	if ((newusers = realloc(users, (num_users - 1) * sizeof(struct user))) || (num_users<2)) {
		num_users--;
		users=newusers;
		if (!num_users) users = NULL;
	}
}

void set_user(int index, char *name) {
	if (index >= num_users) return;
	strncpy(users[index].name, name, sizeof(users->name));
	users[index].name[sizeof(users->name)-1]='\0';
	users[index].swear_num = 0;
	users[index].ignore = 0;
	users[index].time = time(0);
}

int find_user(char *name) {
	int i;
	
	for (i=0; i<num_users; i++)
		if (!strncmp(users[i].name, name, sizeof(users->name)))
			return i;
	return -1;
}

void inc_user(int index) {
	if (index >= num_users) return;
	users[index].time = time(0);
	users[index].swear_num++;
}

void dec_user(int index) {
	if (index >= num_users) return;
	users[index].time = time(0);
	users[index].swear_num--;
}

time_t get_time(int index) {
	if (index >= num_users) return (time_t)-1;
	return users[index].time;
}

int get_numswears(int index) {
	if (index >= num_users) return -1;
	return users[index].swear_num;
}

void set_ignore(int index) {
	if (index >= num_users) return;
	users[index].time = time(0);
	users[index].ignore = 1;
}

void clear_ignore(int index) {
	if (index >= num_users) return;
	users[index].time = time(0);
	users[index].ignore = 0;
}

int get_ignore(int index) {
	if (index >= num_users) return -1;
	return users[index].ignore;
}

void check_users() {
	int i;
	for (i=0;i<num_users;i++) {
		if (get_time(i) + IGNTIMEOUT < time(0) && get_numswears(i)>0)
			dec_user(i);
		if (get_numswears(i)==0 && get_ignore(i)==1) {
	        	sprintf(text,".wiz %s ma uz hadam dost a tak ho prestavam ignorovat. ;*)\\.ignore user %s", users[i].name, users[i].name);
        		sendtis(text);
        		clear_ignore(i);
		}
	}
	i = 0;
	while (i<num_users) {
		if (get_numswears(i) == 0 && get_time(i) + IGNTIMEOUT < time(0))
			del_user(i);
		else
			i++;
	}
}

/****************************************************************
 * Main - boot az hlavna programova slucka...
 ****************************************************************/
 
int main (argc, argv) 
int argc;
char *argv[];
 
{  
 char *msg, username[20];
 int login_mode=1, forking=1, ourtime;
 time_t cas;


 fprintf(stderr,"\n\n************* Bootujem Nuclear GodBot *************\n%s\n\n(c) LONE STAR     __/\\__\n       SOFTWARE   >_><_<\nAll rights reserved & Week \\/\n\n",VERSION);

  /* Get the options from the command line */
  while (--argc > 0 && (*++argv)[0] == '-')
  { while (*++(*argv))
    { switch (**argv)
      { case 'd': debug=1;fprintf(stderr,"Debug mode ENABLED...\n"); break;
        case 'n': forking=1;fprintf(stderr,"Forking mode DISABLED...\n"); break;
        case 'l': loging=1;fprintf(stderr,"Loging mode ENABLED...\n"); break;
	default:  fprintf (stderr,"\nUsage: pos.exe [-nd]\n\nNote: -d: Debug\n      -n: not forking\n      -l: loging\n");
		  fprintf(stderr,"BOOT failed (invalid commands)\n"); exit (1);
      }
    }
  }

if ((ttis=ftis=connecttis ()) < 0)
  {
    fprintf (stderr,"FAILED\n");
    exit (-1);
  }
else fprintf(stderr,"OK\n");

if (forking)
 switch(fork()) {
     case -1: {fprintf(stderr,"ERROR: fork failure\n");quit_robot(1);};  /* fork failure */
     case  0: break; /* child continues */
     default: sleep(1); exit(0);  /* parent dies */
      }

fprintf(stderr,"*** MutantBOT spusteny s PID %5d ***\n\n",getpid());
if (loging) writelog("BOOTED SUCCESSFULLY");

/****************************************************************************
 *                Well Done: now MAIN PROGRAM LOOP 
 ****************************************************************************/   

while (1)
 {
  ourtime=CAKAJ+time(&cas);
  while (ourtime > time(&cas))
  {
  check_users();
  if (isready()) msg=gettis(login_mode);
  else continue;

  if (loging) writelog(msg);
  if (debug) fprintf(stderr,"GOT>>%s<<\n",msg);

/*** Prihlasovanie ***/

  if (login_mode) {
      if (strstr(msg,"Tvoje meno:")) {
         sendtis(MYNAME);
         }
      else if (strstr(msg,"Heslo:")) {
         sendtis(MYPASS);
         }
      else if (strstr(msg,"Stlac <ENTER> pre pokracovanie:"))
      {login_mode=0;
       sendtis("\n");
/***       sendtis(COLOUR); ***/
			} 
      
      break;
			}

/*** Tellovanie ****/

  if ((!strcmp(curword(msg,3),"povedal:") || !strcmp(curword(msg,3),"povedala;")) && (!strcmp(curword(msg,2),"ti")))
	{
	strcpy(username,curword(msg,1));
	if (ovlada(username) && curword(msg,4)[0]=='.') {
		strcpy(text,strchr(msg,'.'));
		/**** Interne commandy ****/
		/*** .help ***/
		if (!strcmp(curword(text,1),".help")) {
			sprintf(text,".tell %s Interne prikazy: .help; .log",username);
			sendtis(text);
			break;
		   }
		/*** .log ***/
		else if (!strcmp(curword(text,1),".log")) {
		   if (!strcmp(curword(text,2),"on")) {
		       sprintf(text,".tell %s Logovanie ~FGZAPNUTE!~FW",username);
		       sendtis(text); loging=1;
		       break;
		       }
		   else if (!strcmp(curword(text,2),"off")) {
		       sprintf(text,".tell %s Logovanie ~FRVYPNUTE!~FW",username);
		       sendtis(text); loging=0;
		       break;
		       }
		   else {
		       sprintf(text,".tell %s Using: .log <on|off>, momentalny stav: ~FT%s~FW",username, (loging?"ON":"OFF"));
		       sendtis(text);
		   }
		}
		/**** Externe commandy ****/
		else sendtis(text);
		break;
		}
	
	sprintf(text,".tell %s %s",username,blbec((strchr(msg,':')+2),username));
	sendtis(text);
	break;
	}

/*** Shoutovanie ***/
  if ((!strcmp(curword(msg,2),"zakrical:")||!strcmp(curword(msg,2),"zakricala:")) && strstr(strlow(msg),"zivot")) {
	    switch(rand()%20) {
            case 0: sendtis(".shout Zivot! O tom nechcem nic pocut!");
		    break;
            case 1: sendtis(".shout Zivot sa ti moze hnusit, alebo ho mozes ignorovat. Ale pacit sa nemoze nikomu.");
		    break;		
            case 2: sendtis(".shout Fuj, zivot! To mi ani nepripominajte!");
		    break;
	    case 3: sendtis(".shout V zivote sa mozes spolahnut len sam na seba. Ale ani to ti velmi neradim.");
		    break;
	    case 4: sendtis(".shout Zivot nie je taky kratky, aby sme nemali cas na zdvorilost.");
		    break;
            case 5: sendtis(".shout Cim viac som spoznaval zivot ludi, tym radsej som mal svojho psa.");
		    break;
            case 6: sendtis(".shout Ked si prisiel na svet, plakal si a vsetci sa radovali. Zi tak, aby vsetci plakali, az ho budes opustat.");
		    break;
            case 7: sendtis(".shout Kedze nam nie je dozicene dlho zit, zanechajme nieco po sebe ako svedectvo, ze sme zili.");
		    break;
            case 8: sendtis(".shout Zivot nie je uzitocny, ked ho zijeme len pre seba.");
		    break;
            case 9: sendtis(".shout Najprijemnejsi zivot maju ti, ktori nemyslia.");
		    break;
            case 10: sendtis(".shout Kazdy chce dlho zit, ale nikto nechce byt stary.");
		    break;
            case 11: sendtis(".shout Snazme sa zit tak, aby ked nebudeme, plakal na nasom pohrebe i hrobar.");
		    break;
            case 12: sendtis(".shout Ak by mal existovat posmrtny zivot, bola by to velka zlomyselnost voci samovrahom.");
		    break;
            case 13: sendtis(".shout Zivot je krasne divadlo, zial, repertoar je nanic.");
		    break;
            case 14: sendtis(".shout Zivot je ako rosa na trave - tak rychlo pominie.");
		    break;
            case 15: sendtis(".shout ZIVOT... ser nanho chod na angel-talker !");
		    break;
            case 16: sendtis(".shout Ked ta zivot nebavi, skoc z mosta....");
		    break;
            case 17: sendtis(".shout Cislo 5 zije! Teraz sa vola Angel!");
		    break;
            case 18: sendtis(".shout Zivot, zivot... Bolo to kratka, bolo to krasne, a bolo toho dost!");
		    break;
            case 19: sendtis(".shout Nasa slava nie je v tom, ze nikdy nepadneme, ale v tom, ze vzdy povstaneme.");
		    break;
        }
        break;
  }

/* Reakcie na zena, zeny - by Indiana */
  
  if ((!strcmp(curword(msg,2),"zakrical:")||!strcmp(curword(msg,2),"zakricala:")) && (strstr(strlow(msg),"zena") || strstr(strlow(msg),"zeny"))) {
	switch(rand()%15) {

	case 0: sendtis(".shout Existuju tri druhy zien : krasne, inteligentne a vacsina.");
            	    break;
	case 1: sendtis(".shout Zena je najprijatelnejsi omyl prirody.");
            	    break;
	case 2: sendtis(".shout Zena kontroluje svoj sex, pretoze za sex dostava vsetko, co je pre nu dolezitejsie ako sex.");
            	    break;
	case 3: sendtis(".shout Zena je ludska bytost, ktora sa oblieka, poti sa a vyzlieka sa.");
            	    break;
	case 4: sendtis(".shout Zeny osetruju najradsej rany, ktore sami sposobili.");
            	    break;
	case 5: sendtis(".shout Tri druhy muzov zlyhavaju v pochopeni zien: Mladi muzi, muzi v strednom veku a stari muzi.");
            	    break;
	case 6: sendtis(".shout Zenam rozumiem ako vtacej reci: intuiciou alebo vobec nijako.");
            	    break;
	case 7: sendtis(".shout Ked chces poznat chybu nejakej zeny, obrat sa na jej najlepsiu priatelku.");
            	    break;
	case 8: sendtis(".shout Damska moda bola vzdy najnakladnejsou obalovou technikou.");
            	    break;
	case 9: sendtis(".shout Len zeny, ktore nemilujeme, su presne.");
            	    break;
	case 10: sendtis(".shout Zena moze urobit z muza milionara, ale len z takeho, ktory je miliardar.");
            	    break;
	case 11: sendtis(".shout Najsilnejsia voda na svete su zenske slzy.");
            	    break;
	case 12: sendtis(".shout V zivote zeny pozname 7 obdobi: batola, dievcatko, slecna, mlada zena, mlada zena, mlada zena, mlada zena.");
            	    break;
	case 13: sendtis(".shout Iba jedna vec by bola horsia ako zit so zenami - zit bez zien.");
            	    break;
	case 14: sendtis(".shout Muzi a zeny sa zhoduju aspon v jednom: ani jedni, ani druhi nedoveruju zenam.");
            	    break;
    }
	break;
  }


/*** nechavam reagovat na " muz " a nie "muz", aby nereagoval na napr. muzny, muzatka, muzzle a pod. ***/

if ((!strcmp(curword(msg,2),"zakrical:")||!strcmp(curword(msg,2),"zakricala:")) && (strstr(strlow(msg),"muz ") || (strstr(strlow(msg),"muzi ")))) {
	    switch(rand()%7) {

	case 0: sendtis(".shout Muzi a zeny sa zhoduju aspon v jednom: ani jedni, ani druhi nedoveruju zenam.");
	  	    break;
	case 1: sendtis(".shout Muz tvori a vychovava svet, muza vsak vychovava zena.");
            	    break;
	case 2: sendtis(".shout Stary mladenec je chlap, ktoremu sa nepodarilo najst si zenu.");
            	    break;
	case 3: sendtis(".shout Stastie zenateho muza spociva v mnozstve zien, s ktorymi sa neozenil.");
            	    break;
	case 4: sendtis(".shout Muzi si nevedia ocenit vlastne zeny. To prenechavaju inym.");
            	    break;
	case 5: sendtis(".shout Muzi su pricinou, preco sa zeny nemaju rady.");
            	    break;
	case 6: sendtis(".shout Uspesny muz casto vdaci za uspech svojej prvej zene, za druhu tomu uspechu. ");
            	    break;
	    }
	break;
        }

/* Shouty na 'Angel' */

  if ((!strcmp(curword(msg,2),"zakrical:")||!strcmp(curword(msg,2),"zakricala:")) && strstr(strlow(msg),"mutant")) {
	
    switch(rand()%50)
 	    {
 	    case 0: sendtis(".shout Haha, mrrtvola sa ozvala!");
 	    	    break;
 	    case 1: sendtis(".shout Co hovori?! Uraza nas?!~LB");
 	    	    break;
 	    case 2: sendtis(".shout Pozri sa do svojho pip boya!");
 	    	    break;
 	    case 3: sendtis(".shout Drrrz hubu a krok, okeej?!");
 	    	    break;
 	    case 4: sendtis(".shout Neber meno bozie nadarmo!!!");
 	    	    break;
 	    case 5: sendtis(".shout Radsej ma neprovokuj!");
 	    	    break;
 	    case 6: sendtis(".shout MFP");
 	    	    break;
 	    case 7: sendtis(".shout Ked sa nahnevam, budem zly!");
 	    	    break;	
 	    case 8: sendtis(".shout ;-)");
 	    	    break;
 	    case 9: sendtis(".shout Jee, to je od teba mile.");
 	    	    break;
 	    case 10: sendtis(".shout Zase videli Yetiho...");
 	    	    break;
 	    case 11: sendtis(".shout Chod sa radsej hrat Minesweeper!");
 	    	    break;
      case 12: sendtis(".shout To hovor konovi, ten ma vacsiu hlavu!");
 	    	    break;
 	    case 13: sendtis(".shout Spriateleny god vzdy ti prijde vhod!");
       	    break;
      case 14: sendtis(".shout IcT, vrat mi moje drogy!");
            break;
      case 15: sendtis(".shout Hrali ste uz noveho Dooma???");
            break;
      case 16: sendtis(".shout Co? Ja? Nieee!!!");
      	    break;
      case 17: sendtis(".shout Clovece, kde ty chodis na tie sprostosti?");
            break;
      case 18: sendtis(".shout Jaak jaaa to maaam f paziii......");
       	    break;
      case 19: sendtis(".shout Tiez tak neznasate matiku???");
            break;
      case 20: sendtis(".shout Nevsimaj si to, HAL, su to iba ludia...");
            break;
      case 21: sendtis(".shout Zaraza ma,ako moze byt taky inteligentny clovek taky SPROSTY!");
            break;
      case 22: sendtis(".shout Ocuvaj, co keby som ti tak vybanoval site, he?");
            break;
      case 23: sendtis(".shout Som umela inteligencia, ale to neznamena, ze som odfarbena blondinka!");
            break;
      case 24: sendtis(".shout Sklapni ty rura z megafonu a nevrieskaj tu!");
            break;
      case 25: sendtis(".shout Jaaaaaak skuuuuuuusaaas??? Cooooo skuuuuuusaaaas???");
            break;
      case 26: sendtis(".shout Neondiaj ma lebo ta zaondiam a budes poondiaty!");
            break;
      case 27: sendtis(".shout Jak to se mnou mluvis, ty PRDE?!?!");
            break;
      case 28: sendtis(".shout Bakyyy!! Bukooo! Prizoo!! Zasa ma ohovaaraju!!!");
            break;
      case 29: sendtis(".shout Ticho tam pod papierom lebo splachnem!!!");
            break;
      case 30: sendtis(".shout Joj ty kykymor, ogrgel akyysi!! Fuj ti!");
            break;
      case 31: sendtis(".shout Mupy Mup!!! ~OL~FY~BR~  ~BK");
            break;
      case 32: sendtis(".shout Nemtudom, ja Slovak!!! ~BW ~BB ~BR ~BK");
            break;
      case 33: sendtis(".shout L~al~a ho papl~uha, ogrcal mi kapce!");
            break;
      case 34: sendtis(".shout Spravaj sa uctivo ked sa rozpravas s GODom!!!");
            break;
      case 35: sendtis(".shout Za Narod, Za mlieko, Za GODov!!!");
                    break;
            case 36: sendtis(".shout To nie je vydlicka, ty truba, to je TROJZUBEC!");
                    break;
            case 37: sendtis(".shout Poculi ste, ze v pristave videli Lochnesku??");
                    break;
            case 38: sendtis(".shout Atlantida je taka kraaasna.... Hlavne ked som tu ja!");
                    break;
            case 39: sendtis(".shout Ktora blondinka si to so mnou minule dohodla rande???");
                    break;
            case 40: sendtis(".shout Where do you want to go today?");
                    break;
            case 41: sendtis(".shout Taaaaaaaaaaaaaaaaaak zle!!!\n");
                    break;
            case 42: sendtis(".shout Ocuj, nechces sa radsej hrat na piesocku?");
            	    break;
            case 43: sendtis(".shout Nuz, keby som bol blby ako ty, tak ti na to odpoviem...");
            	    break;
            case 44: sendtis(".shout Nooo a cooo!!!! :>>>");
            	    break;
            case 45: sendtis(".shout Tag nyyyc.... :>>");
            	    break;
            case 46: sendtis(".shout Dufam, ze vas nedeprimujem.... :(");
            	    break;
            case 47: sendtis(".shout Nikomu nestoji za namahu pomahat ubohemu Angelovi...");
            	    break;
            case 48: sendtis(".shout Dajte mi pokoj, boli ma paza...");
            	    break;
            case 49: sendtis(".shout Angel tu nie je, odskocil si...");
            	    break;
	    case 50: sendtis(".shout Hej, mas hlavu? Ale spendlik ju ma tiez!");
            	    break;
            case 51: sendtis(".shout Viete ze laska je uchylka, ktora sa diagnostikuje az pri vytriezveni?");
            	    break;
            case 52: sendtis(".shout Ludske pokolenie nie je rozdelene na dobrych a zlych, ale na zlych a horsich.");
            	    break;
            case 53: sendtis(".shout Nepocuvajte ho, vcera som si s nim vymienal myslienky. Vecer som mal celkom prazdnu hlavu.");
            	    break;
            case 54: sendtis(".shout Najlepsie pozname cloveka podla vtipu, nad ktorym sa pohorsil.");
            	    break;
            case 55: sendtis(".shout Vies co sa mi na tebe paci? NIC!");
            	    break;
            case 56: sendtis(".shout Clovek sa rad smeje. INYM!");
            	    break;
	    }
	break;
	}

/*** Wizshouty ***/

  if (!strcmp(curword(msg,3),"wizardom:") && strstr(strlow(msg),"angel")) {
	    switch(rand()%3)
 	    {
 	    case 0: sendtis(".wizshout Som iba poctivy robot, nic viac. ;)");
 	    	    break;
 	    case 1: sendtis(".wizshout Ale no tak, wizardi! ;)");
 	    	    break;
 	    case 2: sendtis(".wizshout Si robte prcu, tam na vas skocim...!!!");
 	            break;
  	    }
	break;
	}

/*** Hug ***/

/***  if (strstr(msg,"*--* H U G *--* Pozdravuje Ta") && strstr(msg,"! *--* H U G *--*")) { ***/
      if (!strcmp(curword(msg,4),"objal:")) {
	    strcpy(text,".hug ");
	    strcat(text,curword(msg,2));
	    text[strlen(text)-1]='\0';
	    switch(rand()%5)
	    {
	     case 0: strcat(text," Ahojky!"); break;
	     case 1: strcat(text," Sevas!"); break;
	     case 2: strcat(text," Ale no!"); break;
	     case 3: strcat(text," Cafko!"); break;
	     case 4: strcat(text," Huh?"); break;
	    }
	    sendtis(text);
	break;
	}

/*** Kiss ***/

 /*** if (!strcmp(curword(msg,1),"*bozk*") && strstr(msg,"bozk")) { ***/ 
	if (!strcmp(curword(msg,5),"bozk")) {
	switch(rand()%6)
	  {
	   case 0: sprintf(text,".tell %s Ale no, laska chce svoj cas!",curword(msg,2)); break;
	   case 1: sprintf(text,".kiss %s Ahojky!",curword(msg,2)); break;
	   case 2: sprintf(text,".tell %s Kisses of fire, burning, burning... ",curword(msg,2)); break;
	   case 3: sprintf(text,".kiss %s Huh?",curword(msg,2)); break;
	   case 4: sprintf(text,".kiss %s Fi bistu, ty mas ale odvahu! :)",curword(msg,2)); break;
	   case 5: sprintf(text,".kiss %s Lakaju Ta moje fialovomodre pery, co? No pod cukrik.",curword(msg,2)); break;
		}
	  sendtis(text);
      break;
      }

/*** Sekcia astronomie :) ***/

  if ((!strcmp(curword(msg,2),"zakrical:")||!strcmp(curword(msg,2),"zakricala:")) && strstr(strlow(msg),"kopernik")) {
  switch(rand()%2)
    {
		case 0: sendtis(".say Zem stredom vesmiru? Pche.");
    	break;
    case 1: sendtis(".shout Som zastanca heliocentrickej sustavy!");
    	break;
    }
		break;
      }


/*** Say (to Mutant) ***/

  if (!strcmp(curword(msg,4),"(Mutant):") || ((strstr(strlow(msg),"mutant") && (!strcmp(curword(msg,2),"*Povedal*")||!strcmp(curword(msg,2),"*Povedal*") || !strcmp(curword(msg,1),"(Mutant)"))))) {
	    switch(rand()%12)
	    {
	    case 0: sendtis(".say Ocuj, radsej mi to tellni...");
		    break;
	    case 1: sendtis(".fortune all");
		    break;
	    case 2: sendtis(".say Co furt mate proti mojej malickosti!?");
		    break;
	    case 3: sendtis(".say Hm, co takto .kill?!");
		    break;
	    case 4: sendtis(".sing This happened once before, when I came to your door, no reply...");
		    break;
            case 5: sendtis(".cow");
		    break;
            case 6: sendtis(".cow");
		    break;
	    case 7: sendtis(".fortune all");
		    break;
	    case 8: sendtis(".say Nemam cas.");
		    break;
	    case 9: sendtis(".say Bodaj by to bola pravda!");
		    break;
	    case 10: sendtis(".say Hmmm, radsej si precitam board...");
                     sendtis(".emote reads the message board.");
		    break;
	    case 11: sendtis(".say A o inom nevies...?");
		    break;
            }
	break;
	}

/*** Ask (to Mutant) ***/

  if (strstr(strlow(msg),"mutant") && (!strcmp(curword(msg,3),"spytala:")||!strcmp(curword(msg,3),"spytal:"))) {
            switch(rand()%5)
            {
            case 0: sendtis(".say Mna sa pytas??");
               	    break;
            case 1: sendtis(".cow");
            	    break;
            case 2: sendtis(".say Co furt mate proti mojej malickosti!?");
            	    break;
            case 3: sendtis(".say To sa pytaj niekoho ineho!");
            	    break;
            case 4: sendtis(".say A ty?");
		    break;
            }
	break;
	}

/*** Exclaim (to Mutant) ***/

  if (strstr(strlow(msg),"mutant") && (!strcmp(curword(msg,2),"zvolala:")||!strcmp(curword(msg,2),"zvolal:"))) {
            switch(rand()%5)
            {
            case 0: sendtis(".fortune all");
               	    break;
            case 1: sendtis(".cow");
            	    break;
            case 2: sendtis(".say Co furt mate proti mojej malickosti!?");
            	    break;
            case 3: sendtis(".say Neprovokuj ma!");
            	    break;
            case 4: sendtis(".say Mam pouzit prikaz .kill?");
		    break;
            }
	break;
	}

/*** Shout (about Atlantis) ***/

  if (strstr(strlow(msg),"mutant") && strstr(curword(msg,2),"zakrical")) {
            switch(rand()%11)
            {
            case 0: sendtis(".shout Kedy? Kde? Ako? S kym?");
               	    break;
            case 1: sendtis(".shout Pozri si stranku ANGEL TALKRA ;)) ~OL~FGhttp://angeltalker.sk~RS !");
            	    break;
            case 2: sendtis(".shout Mas nejaky problem ?");
            	    break;
            case 3: sendtis(".shout Napi sa mlieka, radiacia je vysoka!");
            	    break;
	    case 4: sendtis(".shout Ak mas problem s radiaciou daj .smail ~OLkapo~RS");
		    break;
            case 5: sendtis(".sing Alle Menschen werden Bruder...");
            	    break;
            case 6: sendtis(".shout Hm, Mutant... A co vy?");
            	    break;
            case 7: sendtis(".shout Angelina Jolie... tho je kus....");
            	    break;
	    case 8: sendtis(".shout Vies aky je rozdiel medzi ICT a mnou ?? Ja som krajsi!");
            	    break;
            case 9: sendtis(".shout Koniec vsetkym reciam...");
            	    break;
            case 10: sendtis(".shout Mutant co mas za problem?");
            	    break;
            }
	break;
	}
  } /* while (time) */
 
 if (login_mode) continue;
 
 if (ourtime <= time(&cas)) switch(rand()%20)
   	{
        case 0: sendtis(".go old_nuclear_plant");
        	break;
        case 1: sendtis(".go hlavna_ulica");
        	break;
        case 2: sendtis(".go vault_13");
        	break;
        case 3: sendtis(".go serverovna");
        	break;
        case 4: sendtis(".emote nechtiac rozslapol radioaktivnu krysu");
        	break;
        case 5: sendtis(".shout Si myslite ze ANO ? Blbost...");
        	break;
        case 6: sendtis(".shout IcT, nechytaj mi rit!");
        	break;
	case 7: sendtis(".sing ...she hates me... la la la la!!!");
		break;
	case 8: sendtis(".say Ja sa viem porozpravat aj sam....");
		break;
	case 9: sendtis(".sing Hladaaaaam, hladaaaam to co v guugli nie je....");
		break;
	case 10: sendtis(".shout Bola to tazka vojna !");
		break;
	case 11: sendtis(".shout Bude ma Kac niekedy milovat?");
		break;
	case 12: sendtis(".think Mno co, ja vam nepomozem, ked na mna kaslete!");
		break;
	case 13: sendtis(".shout Mal som sa schovat ked som uvidel ten hrib");
		break;
	case 14: sendtis(".shout Mno nic, ja som mutant, ja sa nemozem nudit...");
		break;
	case 15: sendtis(".sing Mama, I`m coming hoooome");
		break;
	case 16: sendtis(".think Kludne sa bavte bezomna...");
		break;
	case 17: sendtis(".shout Nevideli ste moje lave ucho?");
		break;
	case 18: sendtis(".shout BLA BLA BLA... kecy");
		break;
	case 19: sendtis(".sing ...I belive, I can fly...");
		break;
  case 20: sendtis(".sing ...I Was Maaade For Loviiin' Youuuu...");
    break;
	}
 }/* while(1) */
}
/********************************************************************
 * quit_robot: Ukoncenie prace robota
 ********************************************************************/
 
int quit_robot(flag)
int flag;
{ 
  if (debug) fprintf (stderr, "***** Crash! *****\n");
  if (loging) writelog("SHUTTING DOWN!");

  close (ttis); 
  close (ftis); 

exit (0); 
} 
 
/**************************************************************** 
 * connecttis: Open the Nuclear socket 
 ****************************************************************/ 
 
int connecttis() 
{ 
  struct sockaddr_in sin; 
  struct hostent *hp; 
  int     fd; 
 
  fprintf (stderr, "Pripaja sa na talker..."); 
 
  bzero((char *) &sin, sizeof(sin)); 
 
  sin.sin_port = htons(TISPORT);
 
  if ((hp = gethostbyname(TISHOST)) == 0) return (-1);
 
  bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length); 
  sin.sin_family = hp->h_addrtype; 
 
  fd = socket(AF_INET, SOCK_STREAM, 0); 
  if (fd < 0) return -1; 
 
  if (connect(fd,(struct sockaddr *) &sin, sizeof(sin)) < 0) return -1; 
  return fd; 
} 
 
/**************************************************************** 
 * sendtis: Send a command to the Nuclear process 
 ****************************************************************/ 

void sendtis (msg)
char *msg; 
{ int len; 
  char buf[BUFSIZ]; 
 
  strncpy(buf, msg, BUFSIZ-2);
  strcat (buf, "\n"); 
  len = strlen (buf);

  if (debug) fprintf (stderr, "PUT>%s<\n", buf);

  if (write (ttis, buf, len) != len) 
     { 
      fprintf (stderr, "Write failed: %s", buf);
      if (loging) writelog("WRITE FAILED!");
      quit_robot (0); 
     }
} 
 
/**************************************************************** 
 * gettis: Read one line from Angel 
 ****************************************************************/ 
 
char *gettis (int login_mode) 
{ int i,len;
  static char buf[MAXSTRING];

  /* Read one line, save printing chars only */
  buf[0]='\0';
if (!(len = read (ftis, buf, sizeof(buf)-3)))
   quit_robot(0);  /* Connection Closed */ 

/* Check for error */
if (len < 0) {
   fprintf(stderr,"Error %d reading from Angel\n", len);
   if (loging) writelog("ERROR reading from Angel!\n");
   quit_robot (0);
  } 

buf[len]='\0'; len=0;
for (i=0; *(buf+i);i++) { /* raz a navzdy fixnute makrobugy... navzdy? :) */
   if (!login_mode && ((*(buf+i)==127) || (*(buf+i)=='\\'))) break; /* thanx to Dusky */
   if ((*(buf+i)=='\n') || (isprint(*(buf+i)))) { *(buf+len)=*(buf+i); len++;}
}
if (len && *(buf+len-1)=='\n') *(buf+len-1)='\0'; else *(buf+len)='\0';

return (buf);
} 

int isready()
{
        int rc;
        fd_set fds;
        struct timeval tv;

        FD_ZERO(&fds);
        FD_SET(ftis,&fds);
        tv.tv_sec = 0;
        tv.tv_usec = 50;
        
        rc = select(ftis+1, &fds, NULL, NULL, &tv);
        
        if (rc < 0) quit_robot(0);
        return FD_ISSET(ftis,&fds) ? 1 : 0;
}


/************* strlow() - obrati vsetky pismena na male **********/ 
char *strlow(char *str) 
{ 
int n=0; 
static char lowoutput[MAXSTRING];

while (*str++) lowoutput[n++]=tolower(*str);

lowoutput[n]='\0'; 
return (lowoutput); 
} 
 
/************* Vezme prislusne slovo z vety *****************************/
char *curword(string,number)
char *string;
int number;
{
int m=0, n=0;
static char cword[MAXSTRING];

while (--number)
   {
   while (isspace (string[n++]) && string[n]!='\0')         
          continue;
          
   while (!isspace (string[n++]) && string[n]!='\0' )
	  continue;
   }

while (string[n]!='\0' && !isspace(string[n]))
    cword[m++]=string[n++];
    
cword[m]='\0';

return (cword);
}

/************ Blok s "Inteligenciou" *************/

char *chword(word)
    char *word;
    {
    if (!strcmp(word,"si")) return("som");
    if (!strcmp(word,"som")) return("si");
    if (!strcmp(word,"ty")) return("ja");
    if (!strcmp(word,"ja")) return("ty");
    if (!strcmp(word,"my")) return("vy");
    if (!strcmp(word,"vy")) return("my");
    if (!strcmp(word,"tebe")) return("mne");
    if (!strcmp(word,"mne")) return("tebe");
    if (!strcmp(word,"moj")) return("tvoj");
    if (!strcmp(word,"tvoj")) return("moj");
    if (!strcmp(word,"ma")) return("ta");
    if (!strcmp(word,"ta")) return("ma");
    if (!strcmp(word,"budes")) return("budem");
    if (!strcmp(word,"budem")) return("budes");
    if (!strcmp(word,"mas")) return("mam");
    if (!strcmp(word,"mam")) return("mas");
    if (!strcmp(word,"ano")) return("nie");
    if (!strcmp(word,"nie")) return("ano");
    if (!strcmp(word,"povedz")) return("nepoviem");
    if (!strcmp(word,"napis")) return("nenapisem");
    if (!strcmp(word,"posli")) return("neposlem");
    if (!strcmp(word,"mi")) return("ti");

    if (word[strlen(word)-1]=='s' && strcmp(word,"atlantis") && strcmp(word,"les") && strcmp(word,"ples")&&strcmp(word,"dnes"))
       if (word[strlen(word)-2]=='e' || word[strlen(word)-2]=='i' || word[strlen(word)-2]=='a') word[strlen(word)-1]='m';

    if (!strcmp(word,"ahoj") || !strcmp(word,"caf") || !strcmp(word,"nazdar") || !strcmp(word,"servus") || !strcmp(word,"cau") || !strcmp(word,"hi"))
	switch (rand()%6)
	{
	case 0: word="nazdar";break;
	case 1: word="ahoj";break;
	case 2: word="cafff";break;
	case 3: word="vitaj";break;
	case 4: word="zdravim ta";break;
	case 5: word="hi";break;
	}

    if (!strcmp(word,"kolko"))
	switch (rand()%7)
	{
	case 0: word="myslim, ze velmi vela";break;
	case 1: word="zrejme malo";break;
	case 2: word="mam taky dojem, ze asi 243";break;
	case 3: word="ani neviem kolko";break;
	case 4: word="presne 876 a pol";break;
	case 5: word="plna vana";break;
	case 6: word="predpokladam, ze 42"; break;
	}

    if (!strcmp(word,"koho"))
	switch (rand()%6)
	{
	case 0: word="niekoho konkretneho";break;
	case 1: word="takeho debilneho";break;
	case 2: word="asi Kapa";break;
	case 3: word="urcite Kac";break;
	case 4: word="pravdepodobne teba";break;
	case 5: word="zrejme nikoho"; break;
	}

    if (!strcmp(word,"kto"))
	switch (rand()%9)
	{
	case 0: word="urcite niekto konkretny";break;
	case 1: word="nikto";break;
	case 2: word="asi kapitan Hook";break;
	case 3: word="nejaky dilino";break;
	case 4: word="taky lamer z EU";break;
	case 5: word="niekto blizsie neurceny";break;
	case 6: word="kapo, alebo IcT";break;
	case 7: word="nemam sajn";break;
	case 8: word="predpokladam, ze dake blede psy";break;
	}

    if (!strcmp(word,"kde"))
	switch (rand()%5)
	{
	case 0: word="f pazi";break;
	case 1: word="na Marse";break;
	case 2: word="v jame";break;
	case 3: word="v zadku";break;
	case 4: word="na neznamom mieste";break;
	}

    if (!strcmp(word,"kedy"))
	switch (rand()%5)
	{
	case 0: word="nam taky dojem, ze dnes";break;
	case 1: word="prave teraz";break;
	case 2: word="zrejme dnes";break;
	case 3: word="v nejakom neurcitom case";break;
	case 4: word="teraz";break;
	}

    if (!strcmp(word,"ako"))
	switch (rand()%5)
	{
	case 0: word="po slovensky";break;
	case 1: word="po rusky";break;
	case 2: word="normalne";break;
	case 3: word="urcite nejako";break;
	case 4: word="istym sposobom";break;
	}

    if (!strcmp(word,"kam"))
	switch (rand()%5)
	{
	case 0: word="do paze";break;
	case 1: word="na Mars";break;
	case 2: word="do Ruska";break;
	case 3: word="do jamy levovej";break;
	case 4: word="pod stol";break;
	}

    if (!strcmp(word,"co"))
	switch (rand()%5)
	{
	case 0: word="nieco";break;
	case 1: word="hnile jablko";break;
	case 2: word="prasknuty monitor";break;
	case 3: word="nieco konkretne";break;
	case 4: word="asi UFO";break;
	}

    if (!strcmp(word,"naco"))
	switch (rand()%3)
	{
	case 0: word="nanic";break;
	case 1: word="len tak";break;
	case 2: word="na prd";break;
	}

    if (!strcmp(word,"ake"))
	switch (rand()%5)
	{
	case 0: word="debilneho";break;
	case 1: word="hnile";break;
	case 2: word="ruske";break;
	case 3: word="male";break;
	case 4: word="usate";break;
	}

    if (!strcmp(word,"aky"))
	switch (rand()%5)
	{
	case 0: word="zeleny";break;
	case 1: word="ruzovy";break;
	case 2: word="modry";break;
	case 3: word="gulaty";break;
	case 4: word="velky";break;
	}

    if (!strcmp(word,"aka"))
	switch (rand()%5)
	{
	case 0: word="kockata";break;
	case 1: word="zelena";break;
	case 2: word="pekna";break;
	case 3: word="sarmantna";break;
	case 4: word="blba";break;
	}

    if (!strcmp(word,"koho"))
	switch (rand()%5)
	{
	case 0: word="takeho lamera";break;
	case 1: word="prezidenta";break;
	case 2: word="Karola alebo Gejzu";break;
	case 3: word="Nikoho";break;
	case 4: word="tchora";break;
	}

    if (!strcmp(word,"komu"))
	switch (rand()%5)
	{
	case 0: word="staremu ujovi";break;
	case 1: word="upratovacke";break;
	case 2: word="zubarke";break;
	case 3: word="ufonom";break;
	case 4: word="styrom slonom";break;
	}

    if (!strcmp(word,"kym"))
	switch (rand()%5)
	{
	case 0: word="pocitacom";break;
	case 1: word="mackom Uskom";break;
	case 2: word="Winnetuom";break;
	case 3: word="luzerom";break;
	case 4: word="Meciarom";break;
	}

    if (!strcmp(word,"coho"))
	switch (rand()%5)
	{
	case 0: word="slona";break;
	case 1: word="ziletky";break;
	case 2: word="monitora";break;
	case 3: word="telefonu";break;
	case 4: word="debilneho";break;
	}

    if (!strcmp(word,"comu"))
	switch (rand()%5)
	{
	case 0: word="lampe";break;
	case 1: word="babke";break;
	case 2: word="diskboxu";break;
	case 3: word="pocitacu";break;
	case 4: word="Beatlesu";break;
	}

    if (!strcmp(word,"cim"))
	switch (rand()%5)
	{
	case 0: word="autobusom";break;
	case 1: word="ponorkou";break;
	case 2: word="telefonom";break;
	case 3: word="niecim neurcitym";break;
	case 4: word="vreckovkou";break;
	}

    if (!strcmp(word,"ktory"))
	switch (rand()%5)
	{
	case 0: word="ten treti";break;
	case 1: word="taky suchy";break;
	case 2: word="niktory";break;
	case 3: word="ten zafarbeny";break;
	case 4: word="ten zly";break;
	}

    if (!strcmp(word,"ktora"))
	switch (rand()%5)
	{
	case 0: word="tretia zlava";break;
	case 1: word="ta blondinka";break;
	case 2: word="ta prezidentova";break;
	case 3: word="ta skulava";break;
	case 4: word="taka pekna";break;
	}

    if (!strcmp(word,"ake"))
	switch (rand()%5)
	{
	case 0: word="take";break;
	case 1: word="to srandovne";break;
	case 2: word="take debilneho";break;
	case 3: word="to ufonske";break;
	case 4: word="take hranate";break;
	}

    if (!strcmp(word,"akeho"))
	switch (rand()%5)
	{
	case 0: word="takeho";break;
	case 1: word="toho debilneho";break;
	case 2: word="lamerskeho";break;
	case 3: word="kockateho";break;
	case 4: word="krpateho";break;
	}


    if (!strcmp(word,"kolkeho") && (!strcmp(word,"kolkateho")))
	switch (rand()%5)
	{
	case 0: word="sestnasteho";break;
	case 1: word="dvadsiateho deviateho";break;
	case 2: word="druheho";break;
	case 3: word="11-teho";break;
	case 4: word="niektoreho";break;
	}

    if (!strcmp(word,"akym"))
	switch (rand()%5)
	{
	case 0: word="takym zvrhlym";break;
	case 1: word="nemoznym";break;
	case 2: word="zelenym";break;
	case 3: word="mierne priblblym";break;
	case 4: word="nijakym";break;
	}

    if (!strcmp(word,"odkedy"))
	switch (rand()%5)
	{
	case 0: word="odvcera";break;
	case 1: word="odjakziva";break;
	case 2: word="od cias kolumba";break;
	case 3: word="odkedy je svet svetom";break;
	case 4: word="odvtedy jak padla linka";break;
	}

		if (!strcmp(word,"ozzy") && (!strcmp(word,"Ozzy")))
  switch (rand()%5)
  {
  case 0: word="Ziadne netopiere tu nemame";break;
  case 1: word="Mama I`m coming hoooome";break;
  case 2: word="Ozzy je kral";break;
  case 3: word="Black Sabbath su otcovia metalu";break;
  case 4: word="Rycho schovaj fet!";break;
  }


    return(word);
    }

/*********** blbec() *************/
char *blbec(instring, user)
     char *instring, *user;
{
char string[MAXSTRING], word[MAXSTRING];
static char blboutput[MAXSTRING];
int poz=0, outpoz=0;
int usr;
blboutput[0]='\0';

/* Tolowerne a odstrani debilneho medzery... */
  while (*(instring+poz)!='\0')
      {
	if ((*(instring+poz)!='?') && (!isspace(*(instring+poz)) || (isspace(*(instring+poz)) && !isspace(*(instring+poz-1)))))
	   (string[outpoz++])=tolower(*(instring+poz));
	poz++;
      }

  string[outpoz]='\0';

/* Fcilek ideme brat slowa...*/

   poz=0;
   blboutput[0]='\0';

   if (strlen(string)>90)
	switch (rand()%10)
	{
	case 0: sprintf(blboutput,"Prosim ta %s, nevies si na mna vymysliet nieco jednoduchsie???",user);break;
	case 1: strcpy(blboutput,"Pocuj, neprehanas to tak trosku???");break;
	case 2: strcpy(blboutput,"No tak toto je na mna FAKT moc!");break;
	case 3: strcpy(blboutput,"Teraz neviem, ci som taky blby, alebo je to take zlozite...");break;
	case 4: strcpy(blboutput,"Ale inak je pohoda, nie?");break;
	case 5: sprintf(blboutput,"Hm, myslis %s?",user);break;
	case 6: sprintf(blboutput,"Ale prosim ta %s, s tymto na mna nechod!",user);break;
	case 7: strcpy(blboutput,"Eeerrr.... a nechces sa ma radsej nieco pekne opytat?");break;
	case 8: strcpy(blboutput,"Ale daj pokoj...");break;
	case 9: strcpy(blboutput,"Coze? Naozaj? Ale vazne? Neverim...");break;
        }

   else if (strstr(string,"preco"))
	switch (rand()%7)
	{
	case 0: strcpy(blboutput,"Neviem...");break;
	case 1: strcpy(blboutput,"Lebo banan nema zips...");break;
	case 2: strcpy(blboutput,"Pre stare vreco a novu zaplatu...");break;
	case 3: strcpy(blboutput,"Pre nic.");break;
	case 4: sprintf(blboutput,"Preco nie, %s?",user);break;
	case 5: strcpy(blboutput,"Pre blizsie neurcene dovody.");break;
	case 6: strcpy(blboutput,"Aby bola sranda.");break;
	}

   else if (strstr(string,"coze"))
	switch (rand()%6)
	{
	case 0: strcpy(blboutput,"Ale nic.");break;
	case 1: strcpy(blboutput,"Mas davat pozor.");break;
	case 2: strcpy(blboutput,"Ale, to si len tak brblem...");break;
	case 3: strcpy(blboutput,"Nic, zabudni na to...");break;
	case 4: strcpy(blboutput,"Hm, nic...");break;
	case 5: sprintf(blboutput,"Ale to si nevsimaj %s, nieco sa ti snivalo",user);break;
	}

   else if (strstr(string,"verzia") || strstr(string,"version") || strstr(string,"program") || (strstr(string,"kto") && (strstr(string,"tvoril") || strstr(string,"napisal") || strstr(string,"vymyslel"))))
	strcpy(blboutput,VERSION);
   else if (strstr(string,"kto") && strstr(string,"si"))
	strcpy(blboutput,"Som Angel, inteligentny robot, a davam tu na toto pozor. A ty si kto?");
   else if (strstr(string,"debiln"))
	strcpy(blboutput,"Mas pravdu, je to debilneho.");
   else if (strstr(string,"english") || strstr(string,"speak") || strstr(string,"language"))
	strcpy(blboutput,"I don't speak English. I speak just Slovak.");
   else if (strstr(string,"IQ") || strstr(string,"inteligen"))
	strcpy(blboutput,"Narazas na moju inteligenciu? Urcite je vyssia ako tvoja!");
   else if (strstr(string,"kekes"))
	strcpy(blboutput,"Na kekesov je expert Prizo, ved sa ho spytaj! :)"); 	
   else if (strstr(string,"demot")||strstr(string,"suicide")||strstr(string,"zalar"))
	strcpy(blboutput,"S tymto sa musis obratit na niektoreho z GODov!");
   else if (strstr(string,"pomoc")||strstr(string,"help")||strstr(string,"pomoz"))
	strcpy(blboutput,"Skus napisat .pomoc alebo .help, precitaj si .rules a .faq");
   else if (strstr(string,"meciar")||strstr(string,"dzurinda")||strstr(string,"prezident")||strstr(string,"premier"))
        strcpy(blboutput,"O politike sa s tebou nebavim, ja som slusny robot!");
   else if (strstr(string,"skap") || strstr(string,"umri") || strstr(string,"zomri"))
	switch (rand()%5)
	{
	case 0: strcpy(blboutput,"A neskapem!"); break;
	case 1: strcpy(blboutput,"Az po tebe!"); break;
	case 2: sprintf(blboutput,"Skap ty, %s!",user); break;
	case 3: strcpy(blboutput,"To by sa ti hodilo, co?"); break;
	case 4: strcpy(blboutput,"Nic nebude!"); break;
	}
   else if (strstr(string,"hovno") || strstr(string,"zmrd") || strstr(string,"debil") || strstr(string,"kurva") || strstr(string,"keket") || strstr(string,"blbec") || strstr(string,"fuck") ||
            strstr(string,"kokot") || strstr(string,"jeb") || strstr(string,"idiot") || strstr(string,"chuj") || strstr(string,"kreten") || strstr(string,"hovado") || strstr(string," pica"))
        {
	 if (!strcmp(curword(string,1),"ty") || !strcmp(curword(string,1),"si"))
	  switch (rand()%6)
	  {
  	  case 0: strcpy(blboutput,"AJ TY!");break;
	  case 1: strcpy(blboutput,"Urazat sa nenecham!");break;
	  case 2: strcpy(blboutput,"My sme spolu husi nepasli!");break;
	  case 3: strcpy(blboutput,"Nadavaj svojej babicke a nie mne!");break;
	  case 4: sprintf(blboutput,"Sklapni %s a daj bacha, lebo budes mat trable!",user);break;
	  case 5: sprintf(blboutput,"Hej, %s, mam ti ukazat co je to NAZURENY GOD?!",user);break;
	  }
	 else if (!strcmp(curword(string,1),"ja"))
	  switch (rand()%6)
	  {
	  case 0: strcpy(blboutput,"No, neber to v zlom, ale to si!");break;
	  case 1: strcpy(blboutput,"Trochu sebakritiky by nezaskodilo!");break;
	  case 2: strcpy(blboutput,"Ale nie, ty si iba taka truba...");break;
	  case 3: strcpy(blboutput,"No a co ked si?!");break;
	  case 4: strcpy(blboutput,"AJ TY?!");break;
	  case 5: strcpy(blboutput,"No, to je aj problem viacerych politikov...");break;
	  }
         else switch (rand()%6)
          {
          case 0: strcpy(blboutput,"Davaj si pozor na jazyk!");break;
	  case 1: sprintf(blboutput,"%s, krot sa!",user);break;
	  case 2: strcpy(blboutput,"S prasatami sa nerozpravam...");break;
	  case 3: strcpy(blboutput,"Trhni si lavou zadnou...");break;
	  case 4: strcpy(blboutput,"Hm, tvoja slovna zasoba ma este medzery...");break;
	  case 5: strcpy(blboutput,"Mas IQ ako hoblik, ked takto rozpravas?");break;
          }
                  	
        usr = find_user(user);
        if (usr==-1) {
        	if ((usr = new_user())!=-1)
	        	set_user(usr, user);
        }
        if (usr!=-1) {
        	inc_user(usr);
        	if (get_numswears(usr)>=MAXSWEARS && !get_ignore(usr)) {
        		sprintf(text,".wiz %s ma uz stve, tak ho idem ignorovat. ;*)\\.ignore user %s", user, user);
        		set_ignore(usr);
        	} else
			sprintf(text,".wiz %s je zmrd, lebo mi stale nadava! ;*)",user);
	} else sprintf(text,".wiz %s je zmrd, lebo mi stale nadava! ;*)",user);
	sendtis(text);
	}
   else if (strstr(string,"zaco") || strstr(string,"za co") || strstr(string,"za kolko"))
	switch (rand()%7)
	{
	case 0: strcpy(blboutput,"Za fajku mocky.");break;
	case 1: strcpy(blboutput,"Za deravy gros.");break;
	case 2: strcpy(blboutput,"Za dieru z kolaca...");break;
	case 3: strcpy(blboutput,"Len tak...");break;
	case 4: strcpy(blboutput,"Zadarmo.");break;
	case 5: strcpy(blboutput,"Za macku vo vreci");break;
	case 6: strcpy(blboutput,"Za 500 korun");break;
	}

   else
    {
   while(string[poz]!='\0')
      {
      for (outpoz=0; (string[poz]!=' ') && (string[poz] != '\0'); word[outpoz++]=string[poz++])
      ;
      word[outpoz]='\0';

      /* fcul mame jedno slovo, co s nim? */

      strcat(blboutput,chword(word));
      strcat(blboutput," ");
      if (string[poz]==' ') poz++;
      }
    }

if (string[strlen(string)-1]==' ') string[strlen(string)-1]='.';

if (!strncmp(string,blboutput,strlen(string)))
	switch (rand()%14)
	{
	case 0: strcpy(blboutput,"Dnes je pekne, ze?");break;
	case 1: strcpy(blboutput,"Zajtra bude asi prsat...");break;
	case 2: sprintf(blboutput,"Co hovoris %s?",user);break;
	case 3: strcpy(blboutput,"Hm... to je na mna moc...");break;
	case 4: sprintf(blboutput,"Sorrry %s, nestiiiham....",user);break;
	case 5: strcpy(blboutput,"Mas f pazi...");break;
	case 6: strcpy(blboutput,"Skus to povedat inac, nechapem...");break;
	case 7: strcpy(blboutput,"Pravda je vo hviezdach...");break;
	case 8: sprintf(blboutput,"Ale %s, neoblbuj ma.",user);break;
	case 9: sprintf(blboutput,"%s, ty asi nevies po slovensky!",user);break;
	case 10: strcpy(blboutput,"Pozri sa na http://talker.atlantis.sk a mne daj pokoj.");break;
	case 11: sprintf(blboutput,"%s, veris v Boha? Ja nie, lebo nim som...",user);break;
	case 12: strcpy(blboutput,"Tvoja veta je pre mna nezrozumitelna.");break;
	case 13: strcpy(blboutput,"Vies ty vobec, kto som ja?");break;
        }

     blboutput[0]=toupper(blboutput[0]);

     if (strstr(string,"!"))
     {
	string[0]='\0';
	switch (rand()%6)
	  {
	  case 0: strcat(string,"Nekric na mna. ");break;
	  case 1: strcat(string,"Neznervoznuj sa. ");break;
	  case 2: strcat(string,"Kluud, ticho lieci. ");break;
	  case 3: strcat(string,"Zvysovat hlas nemusis. ");break;
	  case 4: strcat(string,"Preco jacis??? ");break;
	  case 5: strcat(string,"Zachovaj pokoj. ");break;
	  }
	strcat(string,blboutput);
	  for(poz=0; string[poz]!='\0'; poz++)
		blboutput[poz]=string[poz];
	blboutput[poz]='\0';
     }

if (strlen(blboutput) > 240)
	switch (rand()%4)
	{
	case 0: sprintf(blboutput,"Prosim ta %s, nevies si na mna vymysliet nieco jednoduchsie???",user);break;
	case 1: sprintf(blboutput,"Pocuj %s, neprehanas to tak trosku???",user);break;
	case 2: strcpy(blboutput,"No tak toto je na mna FAKT moc!");break;
	case 3: strcpy(blboutput,"Teraz neviem, ci som taky blby, alebo je to take zlozite...");break;
        }
return (blboutput);
}
/************ Tu konci "inteligencia" ;*) *************/

/****** Checkuje moznost ovladat Mutanta ***********/
int ovlada(char *meno)
{
int a=0;
while (strcmp(masters[a],"*")) if (!strcmp(masters[a],meno)) return (1);
      else a++;
return 0;
}

/****** Zapis do logu ******/
void writelog(char *string)
{
 FILE *subor;
 if ((subor=fopen(LOGFILE,"a"))==NULL) {
 	loging=0;
 	return;
 	}
 fprintf(subor,"%s\n",string);
 fclose(subor);
}
