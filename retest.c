/*
	This file is part of retest.

	 retest is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 retest is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with it.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/* ANSI tty codes */

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"
#define VERSION "1.0"

/* prompt */

static const char *hs0 = YEL "retest" RESET ": ";

/* intro text */

static const char *hs1 = "Test POSIX regular expressions\n"
	"2019 Manolis Christodoulou (mchris@mobi-doc.com)\n";

/* help text */

static const char *hs = YEL "Syntax" RESET ": retest options\n"
	YEL "options" RESET ":-e: POSIX Extended regular expressions\n"
	"\t-l: (any character) operators don't match newlines\n"
	"\t-i: ignore case\n"
	"\t--version\n"
	"\t--help\n";

/* intro 2 text */

static const char *hs2 = "Enter string/regexp to test\n"
	"CTRL-D to exit.\n";

/* help */

static void help(int x) {
	printf("%s%s%s\n", hs0, hs1, hs);
	exit(x);
}

/* Main */

void main(int a, char *as[]) {
	int cflags = 0;
	char *s;

	for (int n = 1; n < a; n++) {
		if (!strcmp(as[n], "-e")) cflags |= REG_EXTENDED;
		else if (!strcmp(as[n], "-i")) cflags |= REG_ICASE;
		else if (!strcmp(as[n], "-l")) cflags |= REG_NEWLINE;
		else if (!strcmp(as[n], "--version")) {
			puts(VERSION);
			exit(0);
		}
		else if (!strcmp(as[n], "--help")) help(0);
		else help(1);
		}

	printf("%s\n%s", hs1, hs2);
	while (s = readline("\n\1" CYN "\2test/regexp: \1" RESET "\2")) {
		add_history(s);
		char *r = strtok(s, "/");
		char *p = strtok(NULL, "/");

		/* test if one or both strings arrounf / are empty */

		if (!r || !p) {
			free(s);
			continue;
		}

		/* test if the regex compiles */

		regex_t rt;
		if (regcomp(&rt, p, cflags)) {
			printf(RED "%s: compiler error.\n", p);
			free(s);
			continue;
		}

		/* mail loop */

		size_t nmatch = 32;
		regmatch_t pmatch[nmatch];
		printf("/" YEL "%s" RESET "/" YEL "%s" RESET"/ ", r, p);
		if (regexec(&rt, r, nmatch, pmatch, 0) != 0) {
			puts(RED "NO MATCH" RESET);
		}
		else {
			puts(GRN "MATCH!" RESET);
			for (short n = 0; pmatch[n].rm_so != -1; n++)
				printf(GRN "Group %d: " RESET "%.*s" RED "%.*s" RESET "%.*s\n",
				n, pmatch[n].rm_so,s, pmatch[n].rm_eo-pmatch[n].rm_so,
				&s[pmatch[n].rm_so], (int)(strlen(s)-pmatch[n].rm_eo),
				&s[pmatch[n].rm_eo]);
		}
		regfree(&rt);
		free(s);
	}
	puts("\n");
}
