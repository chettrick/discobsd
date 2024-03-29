# include "ldefs.h"

uchar *
getl(uchar *p)	/* return next line of input, throw away trailing '\n' */
	/* returns 0 if eof is had immediately */
{
	int c;
	uchar *s, *t;

	t = s = p;
	while(((c = gch()) != 0) && c != '\n')
		*t++ = c;
	*t = 0;
	if(c == 0 && s == t) return((uchar *)0);
	prev = '\n';
	pres = '\n';
	return(s);
}

void
printerr(char *type, char *fmt, va_list argl)
{
	char buf[1024];

	if(!eof)fprintf(errorf,"%s:%d  ", yyfile, yyline);
	fprintf(errorf,"(%s) ", type);
	vsprintf(buf, fmt, argl);
	fprintf(errorf, "%s\n", buf);
}


void
error(char *s,...)
{
	va_list argl;

	va_start(argl, s);
	printerr("Error", s, argl);
	va_end(argl);
# ifdef DEBUG
	if(debug && sect != ENDSECTION) {
		sect1dump();
		sect2dump();
	}
# endif
	if(
# ifdef DEBUG
		debug ||
# endif
		report == 1) statistics();
	exit(1);	/* error return code */
}

void
warning(char *s,...)
{
	va_list argl;

	va_start(argl, s);
	printerr("Warning", s, argl);
	va_end(argl);

	fflush(errorf);
	fflush(fout);
	fflush(stdout);
}

void
lgate(void)
{
	if (lgatflg) return;
	lgatflg=1;
	if (fout == NULL)
		fout = fopen("lex.yy.c", "w");
	if (fout == NULL)
		error("Can't open lex.yy.c");
	phead1();
}

void
cclinter(int sw)
{
		/* sw = 1 ==> ccl */
	int i, j, k;
	int m;
	if(!sw){		/* is NCCL */
		for(i=1;i<NCH;i++)
			symbol[i] ^= 1;			/* reverse value */
	}
	for(i=1;i<NCH;i++)
		if(symbol[i]) break;
	if(i >= NCH) return;
	i = cindex[i];
	/* see if ccl is already in our table */
	j = 0;
	if(i){
		for(j=1;j<NCH;j++){
			if((symbol[j] && cindex[j] != i) ||
			   (!symbol[j] && cindex[j] == i)) break;
		}
	}
	if(j >= NCH) return;		/* already in */
	m = 0;
	k = 0;
	for(i=1;i<NCH;i++)
		if(symbol[i]){
			if(!cindex[i]){
				cindex[i] = ccount;
				symbol[i] = 0;
				m = 1;
			} else k = 1;
		}
			/* m == 1 implies last value of ccount has been used */
	if(m)ccount++;
	if(k == 0) return;	/* is now in as ccount wholly */
	/* intersection must be computed */
	for(i=1;i<NCH;i++){
		if(symbol[i]){
			m = 0;
			j = cindex[i];	/* will be non-zero */
			for(k=1;k<NCH;k++){
				if(cindex[k] == j){
					if(symbol[k]) symbol[k] = 0;
					else {
						cindex[k] = ccount;
						m = 1;
					}
				}
			}
			if(m)ccount++;
		}
	}
}

int
usescape(int c)
{
	int d;
	switch(c){
	case 'n': c = '\n'; break;
	case 'r': c = '\r'; break;
	case 't': c = '\t'; break;
	case 'b': c = '\b'; break;
	case 'f': c = 014; break;		/* form feed for ascii */
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
		c -= '0';
		while('0' <= (d=gch()) && d <= '7'){
			c = c * 8 + (d-'0');
			if(!('0' <= peek && peek <= '7')) break;
			}
		break;
	}
	return(c);
}

int
lookup(uchar *s, uchar **t)
{
	int i;
	i = 0;
	while(*t){
		if(strcmp((char *)s, *(char **)t) == 0)
			return(i);
		i++;
		t++;
	}
	return(-1);
}

int
cpyact(void)
{ /* copy C action to the next ; or closing } */
	int brac, c, mth;
	int savline, sw;
	char *savfile;

	brac = 0;
	sw = TRUE;
	savline = 0;
	savfile = "?";

while(!eof){
	c = gch();
swt:
	switch( c ){

case '|':	if(brac == 0 && sw == TRUE){
			if(peek == '|')gch();		/* eat up an extra '|' */
			return(0);
		}
		break;

case ';':
		if( brac == 0 ){
			putc(c,fout);
			putc('\n',fout);
			return(1);
		}
		break;

case '{':
		brac++;
		savline=yyline;
		savfile=yyfile;
		break;

case '}':
		brac--;
		if( brac == 0 ){
			putc(c,fout);
			putc('\n',fout);
			return(1);
		}
		break;

case '/':	/* look for comments */
		putc(c,fout);
		c = gch();
		if( c != '*' ) goto swt;

		/* it really is a comment */

		putc(c,fout);
		savline=yyline;
		savfile=yyfile;
		while ((c = gch()) != 0) {
			if( c=='*' ){
				putc(c,fout);
				if( (c=gch()) == '/' ) goto loop;
			}
			putc(c,fout);
		}
		yyline=savline;
		yyfile=savfile;
		error( "EOF inside comment" );

case '\'':	/* character constant */
		mth = '\'';
		goto string;

case '"':	/* character string */
		mth = '"';

	string:

		putc(c,fout);
		while ((c = gch()) != 0) {
			if( c=='\\' ){
				putc(c,fout);
				c=gch();
			}
			else if( c==mth ) goto loop;
			putc(c,fout);
			if (c == '\n') {
				yyline--;
				error( "Non-terminated string or character constant");
			}
		}
		error( "EOF in string or character constant" );

case '\0':
		yyline = savline;
		yyfile = savfile;
		error("Action does not terminate");
default:
		break;		/* usual character */
		}
loop:
	if(c != ' ' && c != '\t' && c != '\n') sw = FALSE;
	putc(c,fout);
	}
	error("Premature EOF");
	return(0);
}

int
gch(void)
{
	int c;
	static int hadeof;

	if (hadeof) {
		hadeof = 0;
		yyline = 0;
	}
	prev = pres;
	c = pres = peek;
	peek = pushptr > pushc ? *--pushptr : getc(fin);
	if(peek == EOF && sargc > 1){
		hadeof = 1;
		fclose(fin);
		yyfile = sargv[fptr++];
		fin = fopen(yyfile,"r");
		if(fin == NULL) {
			yyline = 0;
			error("Cannot open file %s",yyfile);
		}
		peek = getc(fin);
		sargc--;
		sargv++;
	}
	if(c == EOF) {
		eof = TRUE;
		fclose(fin);
		fin = NULL;
		return(0);
	}
	if(c == '\n')yyline++;
	return(c);
}

int
mn2(int a, int d, uintptr c)
{
	name[tptr] = a;
	left[tptr] = d;
	right[tptr] = c;
	parent[tptr] = 0;
	nullstr[tptr] = 0;
	switch(a){
	case RSTR:
		parent[d] = tptr;
		break;
	case BAR:
	case RNEWE:
		if(nullstr[d] || nullstr[c]) nullstr[tptr] = TRUE;
		parent[d] = parent[c] = tptr;
		break;
	case RCAT:
	case DIV:
		if(nullstr[d] && nullstr[c])nullstr[tptr] = TRUE;
		parent[d] = parent[c] = tptr;
		break;
	case RSCON:
		parent[d] = tptr;
		nullstr[tptr] = nullstr[d];
		break;
# ifdef DEBUG
	default:
		warning("bad switch mn2 %d %d",a,d);
		break;
# endif
		}
	if(tptr > treesize)
		error("Parse tree too big %s",(treesize == TREESIZE?"\nTry using %e num":""));
	return(tptr++);
}

int
mnp(int a, void *p)
{
	name[tptr] = a;
	left[tptr] = 0;
	parent[tptr] = 0;
	nullstr[tptr] = 0;
	ptr[tptr] = p;
	switch(a){
	case RCCL:
	case RNCCL:
		if(strlen(p) == 0) nullstr[tptr] = TRUE;
		break;
	default:
		error("bad switch mnp %d %P", a, p);
		break;
	}
	if(tptr > treesize)
		error("Parse tree too big %s",(treesize == TREESIZE?"\nTry using %e num":""));
	return(tptr++);
}

int
mn1(int a, int d)
{
	name[tptr] = a;
	left[tptr] = d;
	parent[tptr] = 0;
	nullstr[tptr] = 0;
	switch(a){
	case STAR:
	case QUEST:
		nullstr[tptr] = TRUE;
		parent[d] = tptr;
		break;
	case PLUS:
	case CARAT:
		nullstr[tptr] = nullstr[d];
		parent[d] = tptr;
		break;
	case S2FINAL:
		nullstr[tptr] = TRUE;
		break;
# ifdef DEBUG
	case FINAL:
	case S1FINAL:
		break;
	default:
		warning("bad switch mn1 %d %d",a,d);
		break;
# endif
	}
	if(tptr > treesize)
		error("Parse tree too big %s",(treesize == TREESIZE?"\nTry using %e num":""));
	return(tptr++);
}

int
mn0(int a)
{
	name[tptr] = a;
	parent[tptr] = 0;
	nullstr[tptr] = 0;
	if(a >= NCH) switch(a){
	case RNULLS: nullstr[tptr] = TRUE; break;
# ifdef DEBUG
	default:
		warning("bad switch mn0 %d",a);
		break;
# endif
	}
	if(tptr > treesize)
		error("Parse tree too big %s",(treesize == TREESIZE?"\nTry using %e num":""));
	return(tptr++);
}

void
munputc(int p)
{
	*pushptr++ = peek;		/* watch out for this */
	peek = p;
	if(pushptr >= pushc+TOKENSIZE)
		error("Too many characters pushed");
}

void
munputs(uchar *p)
{
	int i,j;
	*pushptr++ = peek;
	peek = p[0];
	i = strlen((char*)p);
	for(j = i-1; j>=1; j--)
		*pushptr++ = p[j];
	if(pushptr >= pushc+TOKENSIZE)
		error("Too many characters pushed");
}

int
dupl(int n)
{
	/* duplicate the subtree whose root is n, return ptr to it */
	int i;

	i = name[n];
	if(i < NCH) return(mn0(i));
	switch(i){
	case RNULLS:
		return(mn0(i));
	case RCCL: case RNCCL:
		return(mnp(i,ptr[n]));
	case FINAL: case S1FINAL: case S2FINAL:
		return(mn1(i,left[n]));
	case STAR: case QUEST: case PLUS: case CARAT:
		return(mn1(i,dupl(left[n])));
	case RSTR: case RSCON:
		return(mn2(i,dupl(left[n]),right[n]));
	case BAR: case RNEWE: case RCAT: case DIV:
		return(mn2(i,dupl(left[n]),dupl(right[n])));
# ifdef DEBUG
	default:
		warning("bad switch dupl %d",n);
# endif
	}
	return(0);
}

# ifdef DEBUG
void
allprint(int c)
{
	if(c < 0)
		c += 256;	/* signed char */
	switch(c){
		case 014:
			printf("\\f");
			charc++;
			break;
		case '\n':
			printf("\\n");
			charc++;
			break;
		case '\t':
			printf("\\t");
			charc++;
			break;
		case '\b':
			printf("\\b");
			charc++;
			break;
		case ' ':
			printf("\\\bb");
			break;
		default:
			if(!isprint(c)){
				printf("\\%-3o",c);
				charc += 3;
			} else
				putchar(c);
			break;
	}
	charc++;
}

void
strpt(uchar *s)
{
	charc = 0;
	while(*s){
		allprint(*s++);
		if(charc > LINESIZE){
			charc = 0;
			printf("\n\t");
		}
	}
}

void
sect1dump(void)
{
	int i;

	printf("Sect 1:\n");
	if(def[0]){
		printf("str	trans\n");
		i = -1;
		while(def[++i])
			printf("%s\t%s\n",def[i],subs[i]);
	}
	if(sname[0]){
		printf("start names\n");
		i = -1;
		while(sname[++i])
			printf("%s\n",sname[i]);
	}
}

void
sect2dump(void)
{
	printf("Sect 2:\n");
	treedump();
}

void
treedump(void)
{
	int t;
	uchar *p;
	printf("treedump %d nodes:\n",tptr);
	for(t=0;t<tptr;t++){
		printf("%4d ",t);
		parent[t] ? printf("p=%4d",parent[t]) : printf("      ");
		printf("  ");
		if(name[t] < NCH)
			allprint(name[t]);
		else switch(name[t]){
			case RSTR:
				printf("%d ",left[t]);
				allprint(right[t]);
				break;
			case RCCL:
				printf("ccl ");
				allprint(ptr[t]);
				break;
			case RNCCL:
				printf("nccl ");
				allprint(ptr[t]);
				break;
			case DIV:
				printf("/ %d %d",left[t],right[t]);
				break;
			case BAR:
				printf("| %d %d",left[t],right[t]);
				break;
			case RCAT:
				printf("cat %d %d",left[t],right[t]);
				break;
			case PLUS:
				printf("+ %d",left[t]);
				break;
			case STAR:
				printf("* %d",left[t]);
				break;
			case CARAT:
				printf("^ %d",left[t]);
				break;
			case QUEST:
				printf("? %d",left[t]);
				break;
			case RNULLS:
				printf("nullstring");
				break;
			case FINAL:
				printf("final %d",left[t]);
				break;
			case S1FINAL:
				printf("s1final %d",left[t]);
				break;
			case S2FINAL:
				printf("s2final %d",left[t]);
				break;
			case RNEWE:
				printf("new %d %d",left[t],right[t]);
				break;
			case RSCON:
				p = (uchar *)right[t];
				printf("start %s",sname[*p++-1]);
				while(*p)
					printf(", %s",sname[*p++-1]);
				printf(" %d",left[t]);
				break;
			default:
				printf("unknown %d %d %d",name[t],left[t],right[t]);
				break;
		}
		if(nullstr[t])printf("\t(null poss.)");
		putchar('\n');
	}
}
# endif
