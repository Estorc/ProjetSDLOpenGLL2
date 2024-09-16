#include <stdio.h>
#include <string.h>


int check_str_occurence(char *src, char *occ) {
	int start = 0;
	for (char * str = src; *str; str++) {
		if (*str == *occ) occ++, start++;
		else if (start) return 0;
		if (!*occ) return 1;
	}
	return 0;
}

int main(int argc, char ** argv) {
	int mode = 0;
	if (argc > 1) {
		sscanf(argv[1], "%d", &mode);
	}
    FILE * file = fopen("src/node.h", "r");
	if (!file) {
		fprintf(stderr, "Le fichier \"src/node.h\" n'existe pas!");
		return -1;
	}
	char line[200];
    do fscanf(file, "%[^\n]\n", line);
	while (!check_str_occurence(line, "enum NodeType {") && !feof(file));
	char name[100];
	if (mode == 0)
		fprintf(stdout, "#define NODE_NAMES ");
	if (mode == 1)
		fprintf(stdout, "#define IS_NODE_TYPE_MARKER(x) (");
	int c = 0;
    do {
        fscanf(file, "%[^,],\n", name);
		if (mode == 0 && !check_str_occurence(name, "};")) fprintf(stdout, "\\\"%s\\\",\n", name);
		if (mode == 1 && check_str_occurence(name, "_M_")) fprintf(stdout, "%s(x) == %s", c++ ? " || " : "", name);
    } while(!check_str_occurence(name, "};") && !feof(file));
	if (mode == 1)
		fprintf(stdout, ")");
	fclose(file);
	return 0;
}