/**
* keepalive.c -- 保持与服务器的连接
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

#include <stdio.h>
#include <string.h>

#include "include/basetype.h"
#include "include/debug.h"
#include "include/sys/timeal.h"
#include "include/param/term.h"
#include "uplink_pkt.h"
#include "svrcomm.h"
#include "keepalive.h"
#include "include/param/unique.h"
/**
* @brief 获取心跳周期
* @return 心跳周期(100ms)
*/

static int GetCycKeepAlive(void)
{
      int i      = 10*60*ParaUniG.keepalive_cyc   ;//keepalive_cyc单位为分钟,默认是15分钟
    return(i);
}

#define TIMEOUT_KEEPALIVE    600    // 1 minute
#define TIMEOUT_GNOWAIT    600    // 1 minute
typedef struct {
    int cnt;
    int cnt_max;
    int cnt_getcyc;
    int cnt_retry;
} keepalive_stat_t;
static keepalive_stat_t keepalive_stat;

/**
* @brief 清除状态
*/
void ClearKeepAlive(void)
{
     int cyc = GetCycKeepAlive();//crt屏蔽

     keepalive_stat.cnt = 0;
    keepalive_stat.cnt_retry = 0;
    keepalive_stat.cnt_getcyc = 0;
    keepalive_stat.cnt_max = cyc;
}

/**
* @brief 检测是否在可上线时段
* @return 可上线返回1, 不可上线返回0
*/
int KeepAliveInPeriod(void)
{
#if 0
#define TIMOUT_CHECK    600  // 1minute
    static int lastrtn = 1;
    static int count = TIMOUT_CHECK;

    if(ParaUniG.clientmode != 2) return 1;

    count++;
    if(count > TIMOUT_CHECK) {
        sysclock_t clock;
        unsigned int mask;

        count = 0;
        SysClockReadCurrent(&clock);
        mask = (unsigned int)1<<(clock.hour);
        //if(mask&ParaTerm.uplink.onlineflag) lastrtn = 1;
        //else lastrtn = 0;
    }

    return lastrtn;
#endif
    return 1;
}

/**
* @brief 保持连接处理
* @return 返回0-需要发送链路测试或者登陆, 1-不要测试链路
*/
#if 0
int KeepAliveProc(void)
{
    keepalive_stat.cnt++;
    if(keepalive_stat.cnt > keepalive_stat.cnt_max)
           return 0;
    if(LINESTAT_ON != SvrCommLineState)  //add by crt
        return 0;//
    return 1;
}
#else

/**
* @brief 保持连接处理
* @return 返回0-需要发送链路测试或者登陆, 1-不要测试链路
*/
int KeepAliveProc(void)
{
    int cnt_maxretry = (int)ParaUniG.keepalive_sndretry& 0xff;

    if(cnt_maxretry == 0) cnt_maxretry = 3;

    if((LINESTAT_OFF == SvrCommLineState) && (keepalive_stat.cnt_max > 1800)
        && (keepalive_stat.cnt_retry < cnt_maxretry))
    {
        int cnt_timedail = (int)ParaUniG.keepalive_dialtime&0xff;//

        cnt_timedail *= 10;
        if(cnt_timedail == 0) cnt_timedail = 600;
        keepalive_stat.cnt++;
        if(keepalive_stat.cnt > cnt_timedail) {
            keepalive_stat.cnt = 0;
            keepalive_stat.cnt_getcyc = 0;
            keepalive_stat.cnt_max = GetCycKeepAlive();
            return 0;
        }
    }
    else if(keepalive_stat.cnt_max >= 100) {
        keepalive_stat.cnt++;
        if(keepalive_stat.cnt > keepalive_stat.cnt_max) {
            keepalive_stat.cnt = 0;
            keepalive_stat.cnt_getcyc = 0;
            keepalive_stat.cnt_max = GetCycKeepAlive();
            return 0;
        }
    }
    else {
        keepalive_stat.cnt_getcyc++;
        if(keepalive_stat.cnt_getcyc > TIMEOUT_KEEPALIVE) {
            keepalive_stat.cnt_getcyc = 0;
            keepalive_stat.cnt_max = GetCycKeepAlive();
            return 0;
        }
    }

    return 1;
}

#endif
extern int svrcomm_havetask(void);

/**
* @brief 刷新链路处理
* @return 成功0, 否则失败
*/
int RefreshKeepAlive(void)
{

    if((LINESTAT_ON == SvrCommLineState) &&
        (keepalive_stat.cnt >= TIMEOUT_GNOWAIT)
        && (svrcomm_havetask()))
    {
        keepalive_stat.cnt = 0;
        keepalive_stat.cnt_getcyc = 0;
        keepalive_stat.cnt_max = GetCycKeepAlive();
        return 0;
    }

    return 1;
}

/**
* @brief 设置标志
* @param flag 标志
*/
void SetKeepAlive(unsigned char flag)
{
    switch(flag) {
    case KEEPALIVE_FLAG_LOGONFAIL:
        //keepalive_stat.cnt_retry++;
        break;

    case KEEPALIVE_FLAG_LOGONOK:
        keepalive_stat.cnt_retry = 0;
        break;
    /*by ydl add 2011-05-13*/
    case KEEPALIVE_FLAG_TIMEOUT:
        keepalive_stat.cnt = keepalive_stat.cnt_max + 1;
        break;
    /*end*/

    default: break;
    }
}

