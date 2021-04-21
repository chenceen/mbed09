#include "mbed.h"
#include "mbed_rpc.h"

RpcDigitalOut myled2(LED2,"myled2");
RpcDigitalOut myled3(LED3,"myled3");
BufferedSerial pc(USBTX, USBRX);
void LEDControl(Arguments *in, Reply *out);
RPCFunction rpcLED(&LEDControl, "LEDControl");
double x, y;

int main() {

    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");

    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}


void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;    
    x = in->getArg<double>();
    
    char buffer[200], outbuf[256];
    char strings[20];

    if(x==1){
        while(1){
        sprintf(strings, "/myled2/write 1");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);
        ThisThread::sleep_for(500ms);
        sprintf(strings, "/myled2/write 0");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);
        ThisThread::sleep_for(500ms);
        sprintf(strings, "/myled3/write 1");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);
        ThisThread::sleep_for(500ms);
        sprintf(strings, "/myled3/write 0");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);
        ThisThread::sleep_for(500ms);
        }
    }
    else {
        sprintf(strings, "/myled2/write 0");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);
        sprintf(strings, "/myled3/write 1");
        strcpy(buffer, strings);
        RPC::call(buffer, outbuf);
        out->putData(buffer);    
    }
}