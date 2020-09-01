@echo off

set server="localhost"
set db="tpointer"
set user="tpointer"
set password="password"

start TPointer.exe -h %server% -d %db% -u %user% -p %password%
exit
