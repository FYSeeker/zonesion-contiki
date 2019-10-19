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
    
    if (memcmp(ptag, "TYPE", 4) == 0)
    {
        if (pval[0] == '?')
        {
            int radio_type = 0;
            int device_type = 0;
            switch(hwType_Get())
            {
                case 1:
                    radio_type = ZIGBEE_CONFIG_RADIO_TYPE;
                    device_type = zigbeeGat_type();
                    if (device_type < 0) device_type = 2;
                    break;
                    
                case 2:
                    radio_type = BLE_CONFIG_RADIO_TYPE;
                    device_type = BLE_CONFIG_DEVICE_TYPE;
                    break;
                    
                case 3:
                    radio_type = WIFI_CONFIG_RADIO_TYPE;
                    device_type = WIFI_CONFIG_DEVICE_TYPE;
                    break;
                    
                case 4:
                    radio_type = LORA_CONFIG_RADIO_TYPE;
                    device_type = LORA_CONFIG_DEVICE_TYPE;
                    break;
                    
                case 5:
                    radio_type = NB_CONFIG_RADIO_TYPE;
                    device_type = NB_CONFIG_DEVICE_TYPE;
                    break;
            }
            ret = sprintf(obuf, "TYPE=%d%d%s",radio_type, device_type, sensor_type());
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