#include "resultcheck.h"
#include <string.h>
#include <stdio.h>
#define ERRORTXT "temp/error.txt"

int result_check(char* buf, int buflen) {

	char* temp;
	FILE* fp;
	int sz;
	
	fp = fopen(ERRORTXT,"r");
	if (fp ==0) {
		return 0;
	}
	fseek( fp, 0, SEEK_END );
	sz = ftell( fp );
	if (sz != 0) {
		fclose(fp);
		return 0;
	}
	fclose(fp);
	
	temp = strstr(buf, "ready for accept");
	if (temp == NULL) {
		fprintf(stderr, "no ready for accept");
		return 0;
	}
	
	temp = strstr(buf, "<<child count:0>>");
	if (temp == NULL) {
		fprintf(stderr, "<<child count:0>>");
		return 0;
	}
	
	temp = strstr(buf, "<<child count:1>>");
	if (temp == NULL) {
		fprintf(stderr, "<<child count:1>>");
		return 0;
	}
	
	temp = strstr(buf, "<<child count:2>>");
	if (temp == NULL) {
		fprintf(stderr, "<<child count:2>>");
		return 0;
	}
	
	/*
	temp = strstr(buf, "never give up");
	if (temp == NULL) {
		return 0;
	}
	*/
	temp = strstr(buf, "select: Interrupted system call");
	if (temp == NULL) {
		return 0;
	}
	
	
	return 1;
}