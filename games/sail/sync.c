/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#include "externs.h"
#include <time.h>
#include <sys/file.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 4096

static char sync_buf[BUFSIZE];
static char *sync_bp = sync_buf;
static char sync_lock[25];
static char sync_file[25];
static long sync_seek;
static FILE *sync_fp;
#define SF "/tmp/#sailsink.%d"
#define LF "/tmp/#saillock.%d"

/*VARARGS3*/
void
makesignal(from, fmt, ship, a, b, c)
	struct ship *from;
	char *fmt;
	register struct ship *ship;
	int a, b, c;
{
	char message[80];

	if (ship == 0)
		(void) sprintf(message, fmt, a, b, c);
	else
		(void) sprintf(message, fmt,
			ship->shipname, colours(ship),
			sterncolour(ship), a, b, c);
	Write(W_SIGNAL, from, 1, (int)message, 0, 0, 0);
}

int
sync_exists(game)
	int game;
{
	char buf[sizeof sync_file];
	struct stat s;
	time_t t;

	(void) sprintf(buf, SF, game);
	(void) time(&t);
	if (stat(buf, &s) < 0)
		return 0;
	if (s.st_mtime < t - 60*60*2) {		/* 2 hours */
		(void) unlink(buf);
		(void) sprintf(buf, LF, game);
		(void) unlink(buf);
		return 0;
	} else
		return 1;
}

int
sync_open()
{
	if (sync_fp != NULL)
		(void) fclose(sync_fp);
	(void) sprintf(sync_lock, LF, game);
	(void) sprintf(sync_file, SF, game);
	if (access(sync_file, 0) < 0) {
		int omask = umask(issetuid ? 077 : 011);
		sync_fp = fopen(sync_file, "w+");
		(void) umask(omask);
	} else
		sync_fp = fopen(sync_file, "r+");
	if (sync_fp == NULL)
		return -1;
	sync_seek = 0;
	return 0;
}

void
sync_close(remove)
	int remove;
{
	if (sync_fp != 0)
		(void) fclose(sync_fp);
	if (remove)
		(void) unlink(sync_file);
}

static int
sync_update(type, ship, a, b, c, d)
	int type;
	register struct ship *ship;
	int a, b, c, d;
{
	switch (type) {
	case W_DBP: {
		register struct BP *p = &ship->file->DBP[a];
		p->turnsent = b;
		p->toship = SHIP(c);
		p->mensent = d;
		break;
		}
	case W_OBP: {
		register struct BP *p = &ship->file->OBP[a];
		p->turnsent = b;
		p->toship = SHIP(c);
		p->mensent = d;
		break;
		}
	case W_FOUL: {
		register struct snag *p = &ship->file->foul[a];
		if (SHIP(a)->file->dir == 0)
			break;
		if (p->sn_count++ == 0)
			p->sn_turn = turn;
		ship->file->nfoul++;
		break;
		}
	case W_GRAP: {
		register struct snag *p = &ship->file->grap[a];
		if (SHIP(a)->file->dir == 0)
			break;
		if (p->sn_count++ == 0)
			p->sn_turn = turn;
		ship->file->ngrap++;
		break;
		}
	case W_UNFOUL: {
		register struct snag *p = &ship->file->foul[a];
		if (p->sn_count > 0) {
			if (b) {
				ship->file->nfoul -= p->sn_count;
				p->sn_count = 0;
			} else {
				ship->file->nfoul--;
				p->sn_count--;
			}
                }
		break;
		}
	case W_UNGRAP: {
		register struct snag *p = &ship->file->grap[a];
		if (p->sn_count > 0) {
			if (b) {
				ship->file->ngrap -= p->sn_count;
				p->sn_count = 0;
			} else {
				ship->file->ngrap--;
				p->sn_count--;
			}
                }
		break;
		}
	case W_SIGNAL:
		if (mode == MODE_PLAYER) {
			if (nobells)
				Signal("%s (%c%c): %s", ship, a, 0, 0, 0);
			else
				Signal("\7%s (%c%c): %s", ship, a, 0, 0, 0);
                }
		break;
	case W_CREW: {
		register struct shipspecs *s = ship->specs;
		s->crew1 = a;
		s->crew2 = b;
		s->crew3 = c;
		break;
		}
	case W_CAPTAIN:
		(void) strncpy(ship->file->captain, (char *)a,
			sizeof ship->file->captain - 1);
		ship->file->captain[sizeof ship->file->captain - 1] = 0;
		break;
	case W_CAPTURED:
		if (a < 0)
			ship->file->captured = 0;
		else
			ship->file->captured = SHIP(a);
		break;
	case W_CLASS:
		ship->specs->class = a;
		break;
	case W_DRIFT:
		ship->file->drift = a;
		break;
	case W_EXPLODE:
		if ((ship->file->explode = a) == 2)
			ship->file->dir = 0;
		break;
	case W_FS:
		ship->file->FS = a;
		break;
	case W_GUNL: {
		register struct shipspecs *s = ship->specs;
		s->gunL = a;
		s->carL = b;
		break;
		}
	case W_GUNR: {
		register struct shipspecs *s = ship->specs;
		s->gunR = a;
		s->carR = b;
		break;
		}
	case W_HULL:
		ship->specs->hull = a;
		break;
	case W_MOVE:
		(void) strncpy(ship->file->movebuf, (char *)a,
			sizeof ship->file->movebuf - 1);
		ship->file->movebuf[sizeof ship->file->movebuf - 1] = 0;
		break;
	case W_PCREW:
		ship->file->pcrew = a;
		break;
	case W_POINTS:
		ship->file->points = a;
		break;
	case W_QUAL:
		ship->specs->qual = a;
		break;
	case W_RIGG: {
		register struct shipspecs *s = ship->specs;
		s->rig1 = a;
		s->rig2 = b;
		s->rig3 = c;
		s->rig4 = d;
		break;
		}
	case W_RIG1:
		ship->specs->rig1 = a;
		break;
	case W_RIG2:
		ship->specs->rig2 = a;
		break;
	case W_RIG3:
		ship->specs->rig3 = a;
		break;
	case W_RIG4:
		ship->specs->rig4 = a;
		break;
	case W_COL:
		ship->file->col = a;
		break;
	case W_DIR:
		ship->file->dir = a;
		break;
	case W_ROW:
		ship->file->row = a;
		break;
	case W_SINK:
		if ((ship->file->sink = a) == 2)
			ship->file->dir = 0;
		break;
	case W_STRUCK:
		ship->file->struck = a;
		break;
	case W_TA:
		ship->specs->ta = a;
		break;
	case W_ALIVE:
		alive = 1;
		break;
	case W_TURN:
		turn = a;
		break;
	case W_WIND:
		winddir = a;
		windspeed = b;
		break;
	case W_BEGIN:
		(void) strcpy(ship->file->captain, "begin");
		people++;
		break;
	case W_END:
		*ship->file->captain = 0;
		ship->file->points = 0;
		people--;
		break;
	case W_DDEAD:
		hasdriver = 0;
		break;
	default:
		fprintf(stderr, "sync_update: unknown type %d\r\n", type);
		return -1;
	}
	return 0;
}

void
Write(type, ship, isstr, a, b, c, d)
	int type;
	struct ship *ship;
	int isstr;
	int a, b, c, d;
{
	if (isstr)
		(void) sprintf(sync_bp, "%d %d %d %s\n",
			type, ship->file->index, isstr, (char*) a);
	else
		(void) sprintf(sync_bp, "%d %d %d %d %d %d %d\n",
			type, ship->file->index, isstr, a, b, c, d);
	while (*sync_bp++)
		;
	sync_bp--;
	if (sync_bp >= &sync_buf[sizeof sync_buf])
		abort();
	(void) sync_update(type, ship, a, b, c, d);
}

int
Sync()
{
	void (*sighup)(int), (*sigint)(int);
	register int n;
	int type, shipnum, isstr, a, b, c, d;
	char buf[80];
	int erred = 0;

	sighup = signal(SIGHUP, SIG_IGN);
	sigint = signal(SIGINT, SIG_IGN);
	for (n = TIMEOUT; --n >= 0;) {
#ifdef LOCK_EX
		if (flock(fileno(sync_fp), LOCK_EX|LOCK_NB) >= 0)
			break;
		if (errno != EWOULDBLOCK)
			return -1;
#else
		if (link(sync_file, sync_lock) >= 0)
			break;
		if (errno != EEXIST)
			return -1;
#endif
		sleep(1);
	}
	if (n <= 0)
		return -1;
	(void) fseek(sync_fp, sync_seek, 0);
	for (;;) {
		switch (fscanf(sync_fp, "%d%d%d", &type, &shipnum, &isstr)) {
		case 3:
			break;
		case EOF:
			goto out;
		default:
			goto bad;
		}
		if (shipnum < 0 || shipnum >= cc->vessels)
			goto bad;
		if (isstr != 0 && isstr != 1)
			goto bad;
		if (isstr) {
			register char *p;
			for (p = buf;;) {
				switch (*p++ = getc(sync_fp)) {
				case '\n':
					p--;
				case (char)EOF:
					break;
				default:
					if (p >= buf + sizeof buf)
						p--;
					continue;
				}
				break;
			}
			*p = 0;
			for (p = buf; *p == ' '; p++)
				;
			a = (int)p;
			b = c = d = 0;
		} else
			if (fscanf(sync_fp, "%d%d%d%d", &a, &b, &c, &d) != 4)
				goto bad;
		if (sync_update(type, SHIP(shipnum), a, b, c, d) < 0)
			goto bad;
	}
bad:
	erred++;
out:
	if (!erred && sync_bp != sync_buf) {
		(void) fseek(sync_fp, 0L, 2);
		(void) fwrite(sync_buf, sizeof *sync_buf, sync_bp - sync_buf,
			sync_fp);
		(void) fflush(sync_fp);
		sync_bp = sync_buf;
	}
	sync_seek = ftell(sync_fp);
#ifdef LOCK_EX
	(void) flock(fileno(sync_fp), LOCK_UN);
#else
	(void) unlink(sync_lock);
#endif
	(void) signal(SIGHUP, sighup);
	(void) signal(SIGINT, sigint);
	return erred ? -1 : 0;
}
