#include "gsm.h"



//local
unsigned char queue_sms;
char data_sms[smsLen];
char Selection[Selec];
char sms_reply[smsLen];
char sms_time[20];
char userCall[phoneLen];
//global

char phoneDrive[phoneLen];
char time_gps[20];
char time_gps_send[20];

char imei[imeiLen];

char rx_buffer0[RX_BUFFER_SIZE0];
unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
unsigned char rx_wr_index1;
unsigned char rx_rd_index1,rx_counter1;

//Function
void gsm_on(){	
	GPIOSetDir(GSM_POWER, OUTPUT);
}
unsigned char init_gsm()
{
        unsigned char i; 
        flag_modem.modem = not_connect;
        for (;;)
        { 
                UART0_PrintString("AT+CPIN?\r");            // test xem co sim trong chip hay chua
                //led_status = ! led_status;
                delay_ms(1000);
                if (flag_modem.sim == 1) 
                {        
                        UART0_PrintString("AT+CMGF=1\r"); 
                        delay_ms(1000);    
                       // led_status=1;
                       break;
                } 
                
                if (++i>=20)    // check sim 30 lan ma ko nhan sim thi tu dong reset lai at64 de khoi dong lai con sim900
                {            
                    i=0;
                   // watchdog_on();
                    
                    UART0_PrintString("AT1\r");  
                    while(1);
                }
        } 
        
                
        UART0_PrintString("AT+CLIP=1\r");       
          
        delay_ms(1000); 
       i=0;
        while (check_imei()!=0);    // check imei o day
        //for(i=0 ;i<imeiLen; i++)imei_save[i]=imei[i];
        UART0_PrintString("AT+CMGDA=\"DEL ALL\"\r");  
        delay_ms(3000);

        flag_modem.modem = connected;   

           UART0_PrintString("AT+COPS?\r"); 
           delay_ms(1000);
            //flag_modem.modem = not_connect;   // chua ket noi hoac chua biet da ket noi hay chua
            i=0;    // khi lat trang thai roi phai kiem tra lai 1 lan nua de xem co hoat dong hay chua
            delay_ms(2000);  
              
       return 1;
}
void start_up_gsm()
{
	   unsigned char retry_sim900=0; 
    unsigned char retry_update=0;
	 for(;;)
	{
			UART0_PrintString("AT\r");
			delay_ms(500);
			if (flag_modem.ok)  break;
		else if (++retry_sim900 > 10)
		{
			retry_sim900 =0;
			GPIOSetValue(GSM_POWER, HIGH);
			delay_ms(500);
			GPIOSetValue(GSM_POWER, LOW);
			flag_modem.modem = not_connect;
			break;
		}
	}
}
void del_buffer(unsigned char number)
{
         unsigned char i;
         if (number == 0)
         {
                rx_wr_index0=0;
                rx_counter0=0; 
                for (i=0;i<RX_BUFFER_SIZE0;i++)  rx_buffer0[i]=0;
                
         }         
         else 
         {
                rx_wr_index1=0;
                rx_counter1=0; 
                for (i=0;i<RX_BUFFER_SIZE1;i++)  rx_buffer1[i]=0;
                   
                
         }
}


void upperCase ()
{
    unsigned char len, i;
        len = strlen(data_sms);
        for (i=0;i<len;i++) 
        {
            if (data_sms[i] >= 0x61 && data_sms[i] <= 0x7A) data_sms[i] = data_sms[i] - 0x20;   // chuyen sang dang chu hoa
        } 
} 
void process_SMS(void)
{
        unsigned char i,k;        // +CMGL: 1,"REC UNREAD","+841689625170",,"08/01/23,09:20:25+00"   Testat
        for (i=0;; i++)
        {
                if (rx_buffer0[i] == '"') break;    // rec unread
        }    
         k=i+1;       
         for (i=k;;i++)
         {
                if (rx_buffer0[i] == '"' ) break;     // ,
         }  
         k=i+1;
         k=k+5; ///k=k+2;
         for (i=k;;i++)
         {
                if (rx_buffer0[i] == '"') break;
                userCall[i-k] = rx_buffer0[i];        // 168...// +84168...
         }   
         userCall[i-k] = NULL;
                                            
         k=i+1;              // bo dau "
      
         k=k+1;              // bo dau ,
         for (i=k;;i++)
         {
              if (rx_buffer0[i] == ',') break;
         }                                    
         k=i+2; 
         for (i=k;;i++)
         {
                if (rx_buffer0[i] == '"') break;
                sms_time[i-k] = rx_buffer0[i];  // lay thoi gian cua tin nhan
         }               
         sms_time[i-k] = NULL;
         k=i+1; // bo wa dau "      den phan noi dung tin nhan
         for (i=k;i<RX_BUFFER_SIZE0;i++)    // tach du lieu cua noi dung tin  nhan tu vi tri cho den het toan bo tin nhan
         {
                if (rx_buffer0[i] == NULL) break;
                data_sms[i-k] = rx_buffer0[i];         // chuoi nay chua den 160 ky tu
         } 
         data_sms[i-k] = NULL;
         upperCase();   // chuyen sang chu hoa het
         
}
void process_gsm_data(void) 
{
	char chuoitam[20];
	unsigned char i,j;
	if (flag_system.imei_check)
  {
   flag_system.imei_check=0;
   for(i=0;i<imeiLen;i++)
          {  
            if (rx_buffer0[i] == NULL) break;
            imei[i] = rx_buffer0[i];  
          } 
          imei[i]=NULL;
  }
	for (i=0; i<20; i++) 
  {
     if (rx_buffer0[i] == ':' || rx_buffer0[i] == ',' || rx_buffer0[i] == NULL) break;
     chuoitam[i] = rx_buffer0[i];   // cat lay 5 ky tu dau tien de xac dinh loai nao
  }
  chuoitam[i] = NULL;
	if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'P' && chuoitam[3] == 'I' && chuoitam[4] == 'N') 
  {
    flag_modem.sim = 1;            // test sim     // sim ok ready
  }
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'M' && chuoitam[3] == 'G' && chuoitam[4] == 'R')
	{
		flag_modem.cmgr = 1;
		i=i+3;
		for (j=i; j<20; j++) 
    {
     if (rx_buffer0[j] == '"') break;
     chuoitam[j-i] = rx_buffer0[j];
    }                                                                
    chuoitam[j-i]=NULL; 
    process_SMS();
	}	
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'U' && chuoitam[3] == 'S' && chuoitam[4] == 'D')
	{
		i++;
		for (j=i; j<RX_BUFFER_SIZE0; j++) 
		{
			if (rx_buffer0[j] == NULL || rx_buffer0[j] == '"') break;
		}       
			i=j+1;
		for (j=i; j<RX_BUFFER_SIZE0; j++) 
		{
			if (rx_buffer0[j] == NULL || rx_buffer0[j] == '"') break;
			sms_reply[j-i] = rx_buffer0[j];
		}                                                                
		sms_reply[j-i]=NULL;
                            
		flag_modem.cusd = 1;
	}
		//+CCLK
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'C' && chuoitam[3] == 'L' && chuoitam[4] == 'K')
	{
		i=i+3;  // bo qua : va khoang trang va dau "   
    for (j=i; j<RX_BUFFER_SIZE0; j++) 
    {
      if (rx_buffer0[j] == NULL || rx_buffer0[j] == '+') break;
      time_gps[j-i] = rx_buffer0[j];
      time_gps_send[j-i] = rx_buffer0 [j]; 
                                
    }                                                                
      time_gps[j-i]=NULL;
      time_gps_send[j-i] = NULL; 
		
	}
	
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'L' && chuoitam[3] == 'I' && chuoitam[4] == 'P')
	{
		i=i+3;
    for (j=i;j<RX_BUFFER_SIZE0; j++)
    {
      if (rx_buffer0[j] == NULL || rx_buffer0[j] == '"') break;
      userCall[j-i] = rx_buffer0[j];
    }
     userCall[j-i]=NULL;
     flag_modem.ring = ON;
	}
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'O' && chuoitam[3] == 'P' && chuoitam[4] == 'S')
	{
		buzzer_on();
		
		//Delay(50);
		//GPIOSetValue(BUZZER, LOW); 
    i=i+7;
    for (j=i;j<RX_BUFFER_SIZE0; j++)
    {
       if (rx_buffer0[j] == NULL || rx_buffer0[j] == '"') break;
       Selection[j-i] = rx_buffer0[j];
    }
    Selection[j-i]=NULL; 
                   // printf("Selection2: %s\r",Selection); 
                               
    flag_modem.op_selec = ON; 
		//GPIOSetValue(BUZZER, HIGH);
		//Delay(50);
		buzzer_off();
	}
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'M' && chuoitam[3] == 'G' && chuoitam[4] == 'S') 
	{
    flag_modem.send_sms = complete; 
    flag_system.send_data_flag = no_send;     
	}
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'M' && chuoitam[3] == 'T' && chuoitam[4] == 'I') 
	{
		if (rx_buffer0[13] == 0) queue_sms = rx_buffer0[12] - 0x30;
    else queue_sms = (rx_buffer0[12] - 0x30)*10 + (rx_buffer0[13] - 0x30); 
	}
	else if (chuoitam[0] == 'O' && chuoitam[1] == 'K')
	{
		flag_modem.ok = 1;
	}
	else if (chuoitam[0] == 'E' && chuoitam[1] == 'R' && chuoitam[2] == 'R' && chuoitam[3] == 'O' && chuoitam[4] == 'R')flag_modem.error = 1; 
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'M' && chuoitam[3] == 'E' && chuoitam[4] == ' ' && chuoitam[5] == 'E' && chuoitam[6] == 'R' && chuoitam[7] == 'R' && chuoitam[8] == 'O' && chuoitam[9] == 'R')flag_modem.error = 1;
	else if (chuoitam[0] == '+' && chuoitam[1] == 'C' && chuoitam[2] == 'M' && chuoitam[3] == 'S' && chuoitam[4] == ' ' && chuoitam[5] == 'E' && chuoitam[6] == 'R' && chuoitam[7] == 'R' && chuoitam[8] == 'O' && chuoitam[9] == 'R')flag_modem.error = 1;
	else if (chuoitam[0] == 'B' && chuoitam[1] == 'U' && chuoitam[2] == 'S' && chuoitam[3] == 'Y') flag_modem.busy_call = 1;
	else if (chuoitam[0] == 'N' && chuoitam[1] == 'O' && chuoitam[2] == ' ' && chuoitam[3] == 'A' && chuoitam[4] == 'N' && chuoitam[5] == 'S' && chuoitam[6] == 'W' && chuoitam[7] == 'E' && chuoitam[8] == 'R')flag_modem.no_answer = 1;
	else if (chuoitam[0] == 'N' && chuoitam[1] == 'O' && chuoitam[2] == ' ' && chuoitam[3] == 'D' && chuoitam[4] == 'I' && chuoitam[5] == 'A' && chuoitam[6] == 'L' && chuoitam[7] == ' ' && chuoitam[8] == 'T' && chuoitam[9] == 'O' && chuoitam[10] == 'N' && chuoitam[11] == 'E')flag_modem.no_dial_tone = 1;
		
	else if (chuoitam[0] == 'N' && chuoitam[1] == 'O' && chuoitam[2] == ' ' && chuoitam[3] == 'C' && chuoitam[4] == 'A' && chuoitam[5] == 'R' && chuoitam[6] == 'R' && chuoitam[7] == 'I' && chuoitam[8] == 'E' && chuoitam[9] == 'R')       flag_modem.no_carrier = 1;                                            
  else if (chuoitam[0] == 'C' && chuoitam[1] == 'O' && chuoitam[2] == 'N' && chuoitam[3] == 'N' && chuoitam[4] == 'E' && chuoitam[5] == 'C' && chuoitam[6] == 'T' && chuoitam[7] == ' ' && chuoitam[8] == 'O' && chuoitam[9] == 'K')    flag_gprs.connect_ok = 1;
	else if (chuoitam[0] == 'C' && chuoitam[1] == 'O' && chuoitam[2] == 'N' && chuoitam[3] == 'N' && chuoitam[4] == 'E' && chuoitam[5] == 'C' && chuoitam[6] == 'T' && chuoitam[7] == ' ' && chuoitam[8] == 'F' && chuoitam[9] == 'A' && chuoitam[8] == 'I' && chuoitam[9] == 'L') flag_gprs.connect_ok = 0;
  else if (chuoitam[0] == 'S' && chuoitam[1] == 'E' && chuoitam[2] == 'N' && chuoitam[3] == 'D' && chuoitam[4] == ' ' && chuoitam[5] == 'O' && chuoitam[6] == 'K') flag_gprs.send_gprs_ok = 1;
 else if (chuoitam[0] == 'C' && chuoitam[1] == 'L' && chuoitam[2] == 'O' && chuoitam[3] == 'S' && chuoitam[4] == 'E') flag_gprs.connect_ok = 0;     // dong ket noi server
								
 else if (chuoitam[0] == 'A' && chuoitam[1] == 'L' && chuoitam[2] == 'R' && chuoitam[3] == 'E' && chuoitam[4] == 'A' && chuoitam[5] == 'D' && chuoitam[6] == 'Y' && chuoitam[7] == ' ' && chuoitam[8] == 'C') flag_gprs.connect_ok = 1;
 else if (chuoitam[0] == 'T' && chuoitam[1] == 'C' && chuoitam[2] == 'P' && chuoitam[3] == ' ' && chuoitam[4] == 'E' && chuoitam[5] == 'R' && chuoitam[6] == 'R' && chuoitam[7] == 'O' && chuoitam[8] == 'R')  flag_gprs.deactive_pdp = 1;
 else if (chuoitam[0] == '+' && chuoitam[1] == 'P' && chuoitam[2] == 'D' && chuoitam[3] == 'P' && chuoitam[4] == ':' && chuoitam[5] == ' ' && chuoitam[6] == 'D' && chuoitam[7] == 'E' && chuoitam[8] == 'A' && chuoitam[9] == 'C' && chuoitam[10] == 'T')  flag_gprs.deactive_pdp = 1;
 else if (chuoitam[0] == 'S' && chuoitam[1] == 'T' && chuoitam[2] == 'A' && chuoitam[3] == 'T' && chuoitam[4] == 'E')
 
 {
	 i=i+2;  // bo qua : va khoang trang  
   for (j=i; j<20; j++) 
   {
     if (rx_buffer0[j] == NULL) break;
     chuoitam[j-i] = rx_buffer0[j];
   }                                                                
   chuoitam[j-i]=NULL; 
	 if (chuoitam[0] == 'P' && chuoitam[1] == 'D' && chuoitam[2] == 'P' && chuoitam[3] == ' ' && chuoitam[4] == 'D' && chuoitam[5] == 'E' && chuoitam[6] == 'A' && chuoitam[7] == 'C' && chuoitam[8] == 'T') flag_gprs.deactive_pdp = 1;
   else if (chuoitam[0] == 'T' && chuoitam[1] == 'C' && chuoitam[2] == 'P' && chuoitam[3] == ' ' && chuoitam[4] == 'C' && chuoitam[5] == 'L' && chuoitam[6] == 'O' && chuoitam[7] == 'S' && chuoitam[8] == 'E')  flag_gprs.deactive_pdp = 1;
 }	
 

	
	
}

unsigned char check_imei()
{
    unsigned char i=0;
   // for(;;)
   // {
        UART0_PrintString("AT+GSN\r");    // kiem tra imei
        flag_system.imei_check = 1;
        delay_ms(1000);
        flag_system.imei_check = 0; 
        if (strlen(imei) > 10) return 0; // neu imei da co thi tra ve 0
        if (++i >= 10) return 1;
   /// }
}
void ring_func()
{
        //led_status=0;
         flag_modem.ring=0; 
         UART0_PrintString("ATH\r");
         delay_ms(1000);

          
}