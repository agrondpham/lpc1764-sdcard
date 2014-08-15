#include "tcp.h"

uint32_t timer_gprs;
unsigned char init_tcp (char *apnString, char * userName_a,  char *passWord_a, unsigned char pos)
{
	uint32_t buffer_tam[50];
    if (pos==cstt_error) goto cstt_label;
    else if (pos==ciicr_error) goto ciicr_label;     
cstt_label: 
   //start
UART0_PrintString("AT\r");
delay_ms(500);
    sprintf(buffer_tam,"AT+CSTT=\"%s\",\"%s\",\"%s\"\r", apnString, userName_a, passWord_a);
    UART0_PrintString(buffer_tam);
			//print_data(buffer_tam);
// end
	//	UART0_PrintString("AT+CSTT=\"v-internet\",\"\",\"\"\r");
        flag_system.timeout_gprs = 0;
           flag_modem.ok=0;
           flag_modem.error=0;
           timer_gprs=0;
            while (!flag_modem.ok)     // waiting ok flag
            {
                if (flag_system.timeout_gprs || flag_modem.error) return cstt_error;
                else if (flag_modem.ring) ring_func();
            }
ciicr_label:      
         delay_ms(1000);
    UART0_PrintString ("AT+CIICR\r");
        flag_system.timeout_gprs = 0;
        flag_modem.ok=0;
        flag_modem.error=0;
        timer_gprs=0;
            while (!flag_modem.ok)     // waiting ok flag
            {
                if (flag_system.timeout_gprs || flag_modem.error) return ciicr_error;
                 else if (flag_modem.ring) ring_func();
            }
      delay_ms(1000);  
    UART0_PrintString ("AT+CIFSR\r");
    delay_ms(1000);         
    return ok_status;
}
unsigned char connectServer (char *ipString, char *port)
{
	uint32_t buffer_tam[20];
    if (flag_gprs.connect_ok) return ok_status;
    sprintf(buffer_tam,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r", ipString, port);
    //print_data(buffer_tam);
	//UART0_PrintString("AT+CIPSTART=\"TCP\",\"112.213.84.16\",\"11511\"\r");
	UART0_PrintString(buffer_tam);
     flag_gprs.connect_ok=0;
     flag_modem.error = 0;
     flag_modem.ok = 0;
     timer_gprs=0;
     flag_system.timeout_gprs = 0;
            while (!flag_gprs.connect_ok)     // waiting ok flag
            {
                if (flag_system.timeout_gprs || flag_modem.error) return connect_error;
                 else if (flag_modem.ring) ring_func();
            }
    return ok_status;    
}
unsigned char send_data_gprs (char *dataString)
{
	uint8_t buffer_tam[200];
    delay_ms(1000);
    UART0_PrintString("AT+CIPSEND\r");
    delay_ms(300);
    flag_modem.error = 0;
    if (flag_modem.error) return send_error;
		sprintf(buffer_tam,"%s%c", dataString, ctrl_z);
    UART0_PrintString(buffer_tam);
    
    flag_system.timeout_gprs = 0;
    flag_modem.error=0;
    flag_gprs.send_gprs_ok=0;
    timer_gprs=0;
		//delay_ms(1000);
            while (!flag_gprs.send_gprs_ok)     // waiting ok flag
            {
            	if (flag_system.timeout_gprs || flag_modem.error)
                {

									memset(buffer_tam,gpsLen,NULL);
								sprintf(buffer_tam, "%c\r",ESC);
								UART0_PrintString(buffer_tam);//diachi,ctrl_z
									delay_ms(300);
                   return send_error;
                } 
                else if (flag_modem.ring) ring_func();
            }
            flag_system.send_data=0;
            timer_send_gps=0;  // reset lai thoi gian check
    return ok_status;
}
