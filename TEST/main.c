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
TEST_* というディレクトリを走査してmakefileを走らせて
main.exeができたあとに　main.exeを走らせて
結果ができたあとにok か dame　か判定して標準出力に出力する

*/

int main(int argc, char** argv) {
	
	int temp;
	DIR *dir;
	struct dirent *dp;
	struct stat st;
	
	
	// TEST以下にある　TEST_というなのつくディレクトリを走査する
	if ((dir = opendir(".")) == NULL) {
		
		perror("opendir");
		exit(-1);
	}
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
		
		if (strstr(dp->d_name, "TEST") != NULL) {
			printf("dir = %s\n", dp->d_name);
			// TEST_* のディレクトリに入って　 ./main.exe を走らせる
			
			
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
	
	
	// すべてのTEST_ディレクトリを走査し終えたら　ok.txt dame.txt の存在を確認して
	// 結果を標準出力に出力する
	// TEST以下にある　TEST_というなのつくディレクトリを走査する
	if ((dir = opendir(".")) == NULL) {
		
		perror("opendir");
		exit(-1);
	}
	
	for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
		
		if (strstr(dp->d_name, "TEST") != NULL) {
			printf("dir = %s\n", dp->d_name);
			// TEST_* のディレクトリに入って　make ./main.exe を走らせる
			
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