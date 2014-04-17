#include "gps.h"

char latitude[latLen];   //    ="1049.3361"
char longitude[longLen];   // ="10641.8130"
char speed_gps[speedLen];
char gps_date_string[gps_date_len];

char gps_time_string[gps_time_len];

void gps_init(void) {
	GPIOSetDir(RES_GPS, OUTPUT);
}
void flash_gps() {
	GPIOSetValue(RES_GPS, HIGH);
	delay_ms(1000);
	GPIOSetValue(RES_GPS, LOW);
}

void process_gps_data(void) {
	char chuoitam[10];
	unsigned char k, m;
	int check;
//	$GPGGA,015449.000,1052.0497,N,10641.1773,E,1,9,0.86,-24.1,M,2.3,M,,*71
//	$GPRMC,015449.000,A,1052.0497,N,10641.1773,E,0.00,274.61,050414,,,A*6D
//	$GPVTG,274.61,T,,M,0.00,N,0.00,K,A*3B
	// 9139
//	$GPVTG,0.00,T,,M,2.013,N,3.728,K,A*33
//	$GPGGA,104036.00,1052.03468,N,10641.17855,E,1,07,1.53,-5.9,M,2.3,M,,*4F
//	$GPGLL,1052.03468,N,10641.17855,E,104036.00,A,A*6A
//	$GPGSA,A,3,31,27,03,193,07,16,19,,,,,,1.75,1.53,0.86*3F
//	$GPGSV,3,1,11,19,64,014,32,11,57,165,25,13,55,281,20,03,46,016,23*7E
//	$GPGSV,3,2,11,193,31,126,29,27,30,028,21,07,22,333,28,16,16,045,28*45
//	$GPGSV,3,3,11,31,07,126,40,32,07,159,17,41,,,0*45
//	$GPRMC,104036.00,A,1052.03468,N,10641.17855,E,2.212,77.64,150414,,,A*59
//	///end
	////start

	flag_system.request_data = 0;

	check = 0;
	for (k = 0; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		chuoitam[k] = rx_buffer1[k];
	}
	chuoitam[k] = NULL;
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		if (rx_buffer1[k] == '.') {
			check = 1;
			gps_time_string[k - m] = NULL;
			//break;
		}
		if (!check)
			gps_time_string[k - m] = rx_buffer1[k];


	} // bo qua : 080156.000
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
	} // bo qua: A
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		latitude[k - m] = rx_buffer1[k];
	}
	latitude[k - m] = NULL;
	// 1049.3361      // lay lat
	if ((k - m) < 5) {
		flag_system.gps_detect = 0;   // neu toa do bat thuong hoac toa do < 5
	} else {
		flag_system.gps_detect = 1;

	}
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
	}

	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		longitude[k - m] = rx_buffer1[k];
	}
	longitude[k - m] = NULL;

	if ((k - m) < 5) {
		flag_system.gps_detect = 0;    // neu toa do bat thuong hoac toa do < 5
		//flag_system.led_check = detect_gps_status;
	} else {
		flag_system.gps_detect = 1;
	}
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
	}		// bo qua E

	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		speed_gps[k - m] = rx_buffer1[k];
	}
	speed_gps[k - m] = NULL;
///// test
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
	}		// bo qua 274.61
	m = k + 1;
	for (k = m; k < RX_BUFFER_SIZE1; k++) {
		if (rx_buffer1[k] == ',')
			break;
		gps_date_string[k - m] = rx_buffer1[k];
	}
	gps_date_string[k - m] = NULL;
	////end
	timer_gps = 0;

	////and


}
