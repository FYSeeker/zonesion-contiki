#include "zxbee.h"
#include "zigbee-net.h"
#include "rfUart.h"

extern char hwType_Get();
extern char *sensor_type(void);
extern int z_process_command_call(char* ptag, char* pval, char* obuf);

static char wbuf[96];

void zxbeeBegin(void)
{
    wbuf[0] = '{';
    wbuf[1] = '\0';
}
int zxbeeAdd(char* tag, char* val)
{
    int offset = strlen(wbuf);
    sprintf(&wbuf[offset],"%s=%s,", tag, val);
    return 0;
}
char* zxbeeEnd(void)
{
    int offset = strlen(wbuf);
    wbuf[offset-1] = '}';
    wbuf[offset] = '\0';
    if (offset > 2) return wbuf;
    return NULL;
}

static int _process_command_call(char* ptag, char* pval, char* obuf)
{
    int ret = -1;
    int temp=0;
    if (memcmp(ptag, "TYPE", 4) == 0)
    {
        if (pval[0] == '?')
        {
            switch(hwType_Get())
            {
            case 1:
                temp = zigbeeGat_type();
                if (temp < 0) temp = 2;
                ret = sprintf(obuf, "TYPE=1%d%s", temp, sensor_type());
                break;
            case 2:
                ret = sprintf(obuf, "TYPE=42%s", sensor_type());
                break;
            case 3:
                ret = sprintf(obuf, "TYPE=32%s", sensor_type());
                break;
            case 4:
                ret = sprintf(obuf, "TYPE=62%s", sensor_type());
                break;
            }
        }
    }
    return ret;
}

void _zxbee_onrecv_fun(char *pkg, int len)
{
    char *p;
    char *ptag = NULL;
    char *pval = NULL;
    //int len = strlen(pkg);
    char *pwbuf = wbuf+1;


    if (pkg[0] != '{' || pkg[len-1] != '}') return;

    pkg[len-1] = 0;
    p = pkg+1;
    do
    {
        ptag = p;
        p = strchr(p, '=');
        if (p != NULL)
        {
            *p++ = 0;
            pval = p;
            p = strchr(p, ',');
            if (p != NULL) *p++ = 0;
            /*if (process_command_call != NULL) */
            {
                int ret;
                ret = _process_command_call(ptag, pval, pwbuf);
                if (ret < 0)
                {
                    ret = z_process_command_call(ptag, pval, pwbuf);
                }
                if (ret > 0)
                {
                    pwbuf += ret;
                    *pwbuf++ = ',';
                }
            }
        }
    }
    while (p != NULL);
    if (pwbuf - wbuf > 1)
    {
        wbuf[0] = '{';
        pwbuf[0] = 0;
        pwbuf[-1] = '}';
        rfUartSendData(wbuf);
    }
}