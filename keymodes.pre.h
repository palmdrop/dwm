/* See LICENSE file for copyright and license details. */
/* © 2010 joten <joten@freenet.de> */

#define COMMANDMODE             1
#define INSERTMODE              2

typedef struct {
	unsigned int mod[4];
	KeySym keysym[4];
	void (*func)(const Arg *);
	const Arg arg;
} Command;

/* function declarations */
static void writekeymode();
static void clearcmd(const Arg *arg);
static void defkeypress(XEvent *e);
static void grabdefkeys(void);
static void onlyclient(const Arg *arg);
static void setkeymode(const Arg *arg);

/* variables */
static unsigned int cmdmod[4];
static unsigned int keymode = COMMANDMODE;
static KeySym cmdkeysym[4];
