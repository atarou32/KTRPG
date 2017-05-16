#include "log.h"

void DB_threadfunc(void*) {

	while(1) {
		
		
		
		
		if (g_dbthread_func_do == 0) {
			break;
		}
	}
	
	// もし実行していないキューがある場合はその旨をエラーログに吐き出す
	
	
	
	
}

void DB_release() {
	
	g_dbthread_func_do = 1;
	sleep();// 0.0
}