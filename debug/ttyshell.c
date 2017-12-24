/**
* ttyshell.c -- 串口命令行调试
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-5
* 最后修改时间: 2009-6-5
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "include/debug.h"
#include "include/sys/schedule.h"
#include "include/sys/task.h"
#include "shell.h"

static void ReadLine(char *buf, int maxlen)
{
    while(1) {
        if(fgets(buf, maxlen-1, stdin) != NULL) {
        //if(gets(buf) != NULL) {
            buf[maxlen-1] = 0;
            return;
        }

        Sleep(10);
    }
}

#define SHELLARG_NUM    12
static char CmdLineArgBuf[SHELLARG_NUM][128];
static char *CmdLineArgV[SHELLARG_NUM];

/**
* @brief 串口命令行任务
*/
static void *TtyShellTask(void *arg)
{
    static char command[256];

    shell_func pfunc;
    int argc;

    PrintLog(0,"tty shell start\n");

    while(1) {
        PrintLog(0,"$:");
        ReadLine(command, 256);

        argc = ShellParseArg(command, CmdLineArgV, SHELLARG_NUM);
        if(argc > 0) {
            pfunc = FindShellFunc(CmdLineArgV[0]);
            if(NULL != pfunc) {
                SetLogInterface(0);
                (*pfunc)(argc, CmdLineArgV);
            }
        }
        if(exitflag)
            break;
    }

    return 0;
}

int TtyShellStart(void)
{
    int argc;

    for(argc=0; argc<SHELLARG_NUM; argc++) {
        CmdLineArgV[argc] = CmdLineArgBuf[argc];
    }

    SysCreateTask(TtyShellTask, NULL);

    return 0;
}

