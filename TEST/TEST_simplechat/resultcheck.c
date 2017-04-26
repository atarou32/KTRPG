#include "resultcheck.h"
#include <string.h>
int result_check(char* buf, int buflen) {

	char* temp;
	temp = strstr(buf, "ready for accept");
	if (temp == NULL) {
		return 0;
	}
	
	temp = strstr(buf, "<<child count:0>>");
	if (temp == NULL) {
		return 0;
	}
	
	temp = strstr(buf, "select: Interrupted system call");
	if (temp == NULL) {
		return 0;
	}
	
	
	return 1;
}