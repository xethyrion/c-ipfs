#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(int longHelp, FILE* outStream) {

}

/**
 * command line interface to IPFS.
 * Steps:
 * 	1) parse the command line
 * 	2) if user requests help, display then exit
 * 	3) run the command
 * 	4) output the response
 * 	5) if anything fails, print the error, maybe with help
 */
int main(int argc, char** argv) {

	if (argc == 2) {
		if (strncmp(argv[1], "help", 4) == 0) {
			print_help(1, stdout);
			exit(0);
		} else if (strncmp(argv[1], "--version", 9) == 0) {
			argv[1] = "version";
		}
	} // end of help

	// parse command line into an invocation
	// exit
	exit(0);
}
