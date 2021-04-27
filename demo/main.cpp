  #include "mbed.h"
#include "mbed_rpc.h"

DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
BufferedSerial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
double x;
Thread thread;

void blink() 
{
    while(1) 
    {
        if(x==1)
        {
            myled2 = !myled2;
            ThisThread::sleep_for(250ms);
            myled3 = !myled3;
            ThisThread::sleep_for(250ms);
        }
        else
        {
            myled2 = 0;
            myled3 = 1;
        }
    }
}

int main() 
{
    char buf[256], outbuf[256];
    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    thread.start(blink);

    while(1) 
    {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) 
        {
            char recv = fgetc(devin);
            if (recv == '\n') 
            {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }

}

void LEDControl (Arguments *in, Reply *out)   
{
    bool success = true;
    char buffer[200], outbuf[256];
    char strings[20];
    x = in->getArg<double>();
    if(x==1) sprintf(strings, "LED blinking");
    else sprintf(strings, "STOP");
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    if (success) out->putData(buffer);
    else out->putData("Failed to execute LED control.");

}

