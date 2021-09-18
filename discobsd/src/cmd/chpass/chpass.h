/*
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

struct entry {
	char *prompt;
	int (*func)(), restricted, len;
	char *except, *save;
};

extern uid_t uid;

char	*ttoa(time_t tval);
int	 atot(char *p, time_t *store);
void	 print(FILE *fp, struct passwd *pw);

int	 p_login(char *p, struct passwd *pw, struct entry *ep);
int	 p_passwd(char *p, struct passwd *pw, struct entry *ep);
int	 p_uid(char *p, struct passwd *pw, struct entry *ep);
int	 p_gid(char *p, struct passwd *pw, struct entry *ep);
int	 p_gecos(char *p, struct passwd *pw, struct entry *ep);
int	 p_hdir(char *p, struct passwd *pw, struct entry *ep);
int	 p_shell(char *p, struct passwd *pw, struct entry *ep);
