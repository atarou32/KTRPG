#include "log.h"

void DB_threadfunc(void*) {

	while(1) {
		
		
		
		
		if (g_dbthread_func_do == 0) {
			break;
		}
	}
	
	// �������s���Ă��Ȃ��L���[������ꍇ�͂��̎|���G���[���O�ɓf���o��
	
	
	
	
}

void DB_release() {
	
	g_dbthread_func_do = 1;
	sleep();// 0.0
}