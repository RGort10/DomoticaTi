# compile: 
gcc cron.c -o cron $(mariadb_config --libs) $(pkg-config --libs --cflags libcurl) -lm -L../cgi-bin/library -ldomoticati -Wall -Wextra -ansi -Wconversion

# cronjob:

 */5 * * * * /mnt/d/Repos/DomoticaTi/cronjob/cron