#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>


#define NODE_HEADER_PATH "src/node.h" 
#define NODE_TYPE_MARKER "_M_"


int main(int argc, char ** argv) {
	int mode = 0;
	if (argc > 1) {
		sscanf(argv[1], "%d", &mode);
	}
    FILE * file = fopen(NODE_HEADER_PATH, "r");
	if (!file) {
		fprintf(stderr, "Le fichier \"%s\" n'existe pas!", NODE_HEADER_PATH);
		return -1;
	}
	char line[200];
    do fscanf(file, "%[^\n]\n", line);
	while (!strstr(line, "enum NodeType {") && !feof(file));
	char name[100];
	if (mode == 0)
		fprintf(stdout, "#define NODE_NAMES ");
	if (mode == 1)
		fprintf(stdout, "#define IS_NODE_TYPE_MARKER(x) (");
	int c = 0;
    do {
        fscanf(file, "%[^,],\n", name);
		if (mode == 0 && !strstr(name, "};")) fprintf(stdout, "\\\"%s\\\",\n", name);
		if (mode == 1 && strstr(name, NODE_TYPE_MARKER)) fprintf(stdout, "%s(x) == %s", c++ ? " || " : "", name);
    } while(!strstr(name, "};") && !feof(file));
	if (mode == 1)
		fprintf(stdout, ")");
	fclose(file);
	return 0;
}