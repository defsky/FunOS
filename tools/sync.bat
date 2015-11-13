@echo off

title Synchronize files

pscp -r -pw 123456789 root@192.168.100.34:/root/FunOS ..\

echo\
echo Complete.
pause > nul

