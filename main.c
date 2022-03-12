#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>


// parse args for input typemap file, input typemap definitions, and numeric/string prefix settings
// then parse for the actual input file

int processline(char* line, int len, int lc, FILE* fout);

struct ln {
	int line;
	int num;
	struct ln* next;
};

struct ln* head = NULL;

int main (int argc, char **argv)
{
	if (argc == 1) {
		perror("error: enter a file to be processed\n");
	}
	if (argc > 2) {
		perror("error: unknown arguments\n");
		exit(0);
	}

	char* outfile = (char*) malloc(256), *fname = (char*) malloc(256);
	strcpy(fname, argv[1]);
	sprintf(outfile, "%s.apd.c",  strtok(fname, ".c"));
	FILE* fin = fopen(argv[1], "r");
	FILE* fout = fopen(outfile, "w");
	char* linepnt = NULL;
	size_t n = 0;
	for (int lc = 1; getline(&linepnt, &n, fin) != -1; ++lc) {
		processline(linepnt, n, lc, fout);
	}
	
	if(head == NULL) {
		printf("no NPRINTs found\n");
		exit(0);
	}
	struct ln* pnt = head;
	while(pnt != NULL) {
		printf("line %d: %d\n", pnt->line, pnt->num);
		pnt = pnt->next;
	}
	
}

int d = 0;
int processline(char* line, int len, int lc, FILE* fout) {
	char* pos = strstr(line, "NPRINT");
	if(pos) {
		*pos = '\0';
		fprintf(fout, "%sprintf(\"%d\\n\"); fflush(stdout);%s", line, d, (pos + 6));

		if(head == NULL) {
			head = (struct ln*) malloc(sizeof(struct ln));
			head->next = NULL;
			head->line = lc;
			head->num = d;
		}
		else {
			struct ln* pnt = head;
			while(pnt->next != NULL) {
				pnt = pnt->next;
			}
			pnt->next = (struct ln*) malloc(sizeof(struct ln));
			pnt = pnt->next;
			pnt->next = NULL;
			pnt->line = lc;
			pnt->num = d;
		}
		d++;
	}
	else {
		fprintf(fout, "%s", line);
	}
	return 0;
}