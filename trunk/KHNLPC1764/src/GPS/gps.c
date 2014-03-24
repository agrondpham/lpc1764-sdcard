#include "gps.h"
char latitude[latLen];   //    ="1049.3361"
char longitude[longLen];   // ="10641.8130"
char speed_gps[speedLen];
void process_gps_data(void)
{
    char chuoitam[10];
    unsigned char k,m;
   int check;
            // $GPVTG,244.66,T,,M,0.14,N,0.3,K,N*06     : chuoi toc do :0.3 kmh
            // $GPGGA,093720.102,1049.3363,N,10641.7996,E,1,03,2.3,-2.4,M,2.4,M,,0000*45   // chuoi toa do
            // $GPRMC,080156.000,A,1049.3361,N,10641.8129,E,0.14,232.44,210711,,,A*6D
                flag_system.request_data=0;
							
                check=0;
                for (k=0; k<RX_BUFFER_SIZE1; k++)
                {
                    if (rx_buffer1[k] == ',') break;
                    chuoitam[k] = rx_buffer1[k];
                }
                chuoitam[k]=NULL;
                m=k+1;
								for (k=m; k<RX_BUFFER_SIZE1; k++)  {if (rx_buffer1[k] == ',') break; } // bo qua : 080156.000
                     m=k+1;
                     for (k=m; k<RX_BUFFER_SIZE1; k++) { if (rx_buffer1[k] == ',') break; } // bo qua: A
                     m=k+1;
										 for (k=m; k<RX_BUFFER_SIZE1; k++)   
										 {
												if (rx_buffer1[k] == ',') break;
											 latitude[k-m] = rx_buffer1[k];
										}
											latitude[k-m] = NULL;
                        // 1049.3361      // lay lat
                     if ((k-m) < 5) 
                     {
                        flag_system.gps_detect=0;   // neu toa do bat thuong hoac toa do < 5
										}
                     else 
                     {
                        flag_system.gps_detect =1;
												
                     }
                     m=k+1;
                     for (k=m; k<RX_BUFFER_SIZE1; k++) {if (rx_buffer1[k] == ',') break;}
                     
                     m=k+1;
                     for (k=m; k<RX_BUFFER_SIZE1; k++) 
										 {
											 if (rx_buffer1[k] == ',') break;
											  longitude[k-m] = rx_buffer1[k];
										 }
										  longitude[k-m] = NULL;
                     
                     if ((k-m) < 5) 
                     {
                        flag_system.gps_detect=0;    // neu toa do bat thuong hoac toa do < 5
												//flag_system.led_check = detect_gps_status;
											}
                     else 
                     {
                        flag_system.gps_detect =1;
                     }
                      m=k+1;
                     for (k=m; k<RX_BUFFER_SIZE1; k++) {if (rx_buffer1[k] == ',') break;}
                     
                     m=k+1;
                     for (k=m; k<RX_BUFFER_SIZE1; k++) 
										 {
											 if (rx_buffer1[k] == ',') break;
											  speed_gps[k-m] = rx_buffer1[k];
										 }
										  speed_gps[k-m] = NULL;  
										
								
								
              timer_gps = 0;
								
 
}