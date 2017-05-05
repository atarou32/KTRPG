#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <dirent.h>
#include <stdlib.h>

#define OKTXT "temp/ok.txt"
#define DAMETXT "temp/dame.txt"
#define ERRORTXT "error.txt"

int writeToErrorTxt(char* buf, int buflen) {
	FILE* fp;
	fp = fopen(ERRORTXT, "a");
	if (fp == 0) {
		//perror("fp");
		printf("fp error\n");
		return 0;
	}
	fwrite(buf,buflen,1,fp);
	fclose(fp);
	return 1;
}

/*
TEST_* �Ƃ����f�B���N�g���𑖍�����makefile�𑖂点��
main.exe���ł������ƂɁ@main.exe�𑖂点��
���ʂ��ł������Ƃ�ok �� dame�@�����肵�ĕW���o�͂ɏo�͂���

*/

int main(int argc, char** argv) {
	
	int temp;
	DIR *dir;
	struct dirent *dp;
	struct stat st;
	
	
	// TEST�ȉ��ɂ���@TEST_�Ƃ����Ȃ̂��f�B���N�g���𑖍�����
	if ((dir = opendir(".")) == NULL) {
		
		perror("opendir");
		exit(-1);
	}
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
		
		if (strstr(dp->d_name, "TEST") != NULL) {
			printf("dir = %s\n", dp->d_name);
			// TEST_* �̃f�B���N�g���ɓ����ā@ ./main.exe �𑖂点��
			
			
			if (chdir(dp->d_name) != 0) {
				writeToErrorTxt("error exec\n",11);
				perror("cd execl");
				closedir(dir);
				exit(-1);
			}
			
			if (execl("./main.exe","./main.exe",NULL) == -1) {
				writeToErrorTxt("error main\n",11);
				perror("main execl");
				closedir(dir);
				exit(-1);
			}
			
			if (chdir("../") != 0) {//execl("cd", "cd","../",NULL) == -1) {
				writeToErrorTxt("error exec\n",11);
				perror("cd modoru execl");
				closedir(dir);
				exit(-1);
			}
			
			
		}
	}
	closedir(dir);
	
	
	// ���ׂĂ�TEST_�f�B���N�g���𑖍����I������@ok.txt dame.txt �̑��݂��m�F����
	// ���ʂ�W���o�͂ɏo�͂���
	// TEST�ȉ��ɂ���@TEST_�Ƃ����Ȃ̂��f�B���N�g���𑖍�����
	if ((dir = opendir(".")) == NULL) {
		
		perror("opendir");
		exit(-1);
	}
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
		
		if (strstr(dp->d_name, "TEST") != NULL) {
			printf("dir = %s\n", dp->d_name);
			// TEST_* �̃f�B���N�g���ɓ����ā@make ./main.exe �𑖂点��
			
			if (chdir(dp->d_name) != 0) {//execl("cd", "cd",dp->d_name,NULL) == -1) {
				writeToErrorTxt("error exec\n",11);
				printf("%s ", dp->d_name);
				perror("cd execl");
				closedir(dir);
				exit(-1);
			}
			temp = stat(OKTXT, &st);
			if (0==temp) {
				printf("OK!!\n");
			} else {
				printf("DAME......\n");
			}
			
			if (chdir("../") != 0) {//execl("cd", "cd","../",NULL) == -1) {
				writeToErrorTxt("error exec\n",11);
				perror("modoru execl");
				closedir(dir);
				exit(-1);
			}
		}
	}
	closedir(dir);
		
		
	return 0;
}