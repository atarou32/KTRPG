#include "my_mysql.h"
//#include "mysql/mysql.h"
#include "stdio.h"
#include "stdlib.h"

#define DBHOST "192.168.11.101"
#define DBUSER "root"
#define DBPASS "rootdesuyo"
#define DBNAME "KTRPG"


void test_connect() {
/*
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn,DBHOST, DBUSER,DBPASS, DBNAME, 3306, NULL,0)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return -1;
  }
  mysql_close(conn);
*/	
  return 0;
}
