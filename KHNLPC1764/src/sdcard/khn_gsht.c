/****************************************Copyright (c)**************************************************
 **                               			Khanhhoi Co.,LTD.
 **
 **--------------File Info-------------------------------------------------------------------------------
 ** File Name: 			Sd.c
 ** Last modified Date: 	21-03-2014
 ** Last Version: 		1.0
 ** Descriptions: 		sdcard function using for KHN
 **
 **------------------------------------------------------------------------------------------------------
 ** Created 	 By: 		Agrond
 ** Created date: 		21-03-2014
 ** Version: 				1.0
 ** Descriptions: 		First version
 **
 **------------------------------------------------------------------------------------------------------
 ** Modified by:
 ** Modified date:
 ** Version:
 ** Descriptions:
 **
 ********************************************************************************************************/
//Include for sdcard; Remember change compiler part to ../source/sdcard;../source/sdcard/Fatfs;
#include "khn_gsht.h"
//Validation definition
flash_data_type flash_data;
//u8 buffer_data[512];

FATFS Fatfs;
DIR dir;
char fileName[300];
FIL file;
void KHN_SDCARD_INIT() {
	SPI0_Init();
	spi0_on();

}
int ReadData(char* code, char* date, char* time) {
	FRESULT fr;
//Get DateTime
//Open folder of that date
	f_mount(&Fatfs, "0:", 0);
	sprintf(fileName, "0:/%s/%s.txt", date, time);
	fr = f_open(&file, fileName, FA_READ);
	if (fr == FR_OK)
		return 1;
	else
		return 0;
	//return file;
	//f_close(&file);
}

int WriteData(int code, int date, int time) {
	FRESULT fr;
//Get DateTime
//Create folder of that date
	f_mount(&Fatfs, "0:", 0);
	sprintf(fileName, "%d", date);
	fr = f_mkdir(fileName);
	//check if folder can not create(exist or can create)
	if (fr != FR_OK && fr != FR_EXIST)
		return 1;
	//Create file to hold data
	sprintf(fileName, "0:/%d/%d.txt", date, time);
	/*Open file to write infor if file does not exist create new file*/
	fr = f_open(&file, fileName, FA_WRITE | FA_OPEN_ALWAYS);
	f_lseek(&file, f_size(&file));
	if (fr == FR_OK)
		return 2;
	return 0;
}

void Close() {
	f_close(&file);
}
//////////////////Read Sdcard to print out/////////////////////
struct traffic_violations_type {
	unsigned char latitude[20];    // connect hoac disconnect
	unsigned char longitude[20];
	unsigned char speed[10];
	unsigned char date[15];
	unsigned char time[15];

};

/////////////////////Write basic infor
//How to use
//	char basic_data[]="Khanhhoi2,HA HUY GIAP,4564765765765,51P 637.19;PHAM THE LONG,1232132143,34534543545,12122014,12122024\r\n";
//	write_basic_infor(basic_data);
//"Khanhhoi2,HA HUY GIAP,4564765765765,51P 637.19;PHAM THE LONG,1232132143,34534543545,12122014,12122024\r\n"
int write_basic_infor(char data[250]) {
	FRESULT fr;

	f_mount(&Fatfs, "0:", 0);
	fr = f_mkdir("khnsys");
	//check if folder can not create(exist or can create)
	if (fr != FR_OK && fr != FR_EXIST)
		return 1;
	//Create file to hold data
	sprintf(fileName, "0:/%s/%s.txt", "khnsys", "infor");
	/*Open file to write infor if file does not exist create new file*/
	fr = f_open(&file, fileName, FA_WRITE | FA_OPEN_ALWAYS);
	f_lseek(&file, f_size(&file));
	if (fr == FR_OK) {
		f_puts(data, &file);
		f_close(&file);
		return 2;
	}
	return 0;

}
//How to user
//read_basic_infor();
//UART2_PrintString(flash_data.company);
//UART2_PrintString(flash_data.address);
//UART2_PrintString(flash_data.id_device);
//
//.............
//"Khanhhoi2,HA HUY GIAP,4564765765765,51P 637.19;PHAM THE LONG,1232132143,34534543545,12122014,12122024\r\n"
int read_basic_infor() {
	FRESULT fr;
	char * dataCollection;
	char return_data[250];
	char buffer[250];
	f_mount(&Fatfs, "0:", 0);
	sprintf(fileName, "0:/%s/%s.txt", "khnsys", "infor");
	fr = f_open(&file, fileName, FA_READ);
	if (fr == FR_OK) {
		while (f_gets(buffer, sizeof(buffer), &file)) {
			strcpy(return_data, buffer);
		}
		dataCollection = strtok(return_data, ",");
		int x;
		for (x = 0; x < 9; x++) {
			switch (x) {
			case 0: //company
				strcpy(flash_data.company, dataCollection);
				break;
			case 1: //address
				strcpy(flash_data.address, dataCollection);
				break;
			case 2: //vin
				strcpy(flash_data.vin_No, dataCollection);
				break;
			case 3: //Bien so xe
				strcpy(flash_data.id_device, dataCollection);
				break;
			case 4: //ten tai xe
				strcpy(flash_data.ownerName, dataCollection);
				break;
			case 5: //company
				strcpy(flash_data.license, dataCollection);
				break;
			case 6: //company
				strcpy(flash_data.license_iss_date, dataCollection);
				break;
			case 7: //company
				strcpy(flash_data.license_exp_date, dataCollection);
				break;
			case 8: //so gplx
				strcpy(flash_data.phone, dataCollection);
				break;
			}
			dataCollection = strtok(NULL, ",");
		}
		return 1;
	} else
		return 0;
//return file;
//f_close(&file);
}
///////////////////////////////Read to print
struct print_count_type {
	int num_violation;
	int open;
	int close;
};
typedef struct traffic_violations_type traffic_violations;
typedef struct print_count_type print_count;
int is_high_speed;
int is_high_speed_under_20s;
int is_high_speed_over_20s;
char last_latitude[20];
char last_longitude[20];
char last_speed[20];
char last_date[20];
char last_time[20];
int door_status = 1; // 2 dong;1 mo
int key_status; //2 dong ; 1 mo

traffic_violations violations[15];
print_count count_data;
char time[24][8];
//$3.13,56,862118024728161,1052.0431,N,10641.1769,E,81.00,0,0,0,0,_,000,0,0,050414,080307
int count_violations_from_sdcard(char data[256], int y) {
	char * dataCollection;
	char * latitude;
	char * longitude;
	char * speed;
	char * date;
	char * time;
	char * door;
	char * key;
	dataCollection = strtok(data, ",");
	int x;
	for (x = 0; x < 18; x++) {
		switch (x) {
		case 3: //latitu
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				latitude = dataCollection;
			}
			break;

		case 5:  //longitute
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				longitude = dataCollection;
			}

			break;
		case 7:	//speed
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				speed = dataCollection;

			}
			//flash_data.license_iss_date = dataCollection;
			break;
		case 9:	//cua
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				door = dataCollection;

			}
			//flash_data.license_iss_date = dataCollection;
			break;
		case 10:	//khoa
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				key = dataCollection;

			}
			//flash_data.license_iss_date = dataCollection;
			break;
		case 16:	//date
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				date = dataCollection;

			}
		case 17:	//time
			if (strcmp(dataCollection, "_") == 0) {

			} else {
				time = dataCollection;

			}
		}
		dataCollection = strtok(NULL, ",");
	}
	if (door_status == 2 && atoi(door) == 1) {
		count_data.open++;
		door_status = 1;
	} else if (door_status == 1 && atoi(door) == 2) {
		count_data.close++;
		door_status = 2;
	}else{
		door_status=atoi(door);
	}
//vua tren 80 nhung duoi 20'
	if (atoi(speed) >= 80 && is_high_speed_under_20s == 0) {
		is_high_speed_under_20s = 1;
		strcpy(last_latitude, latitude);
		strcpy(last_longitude, longitude);
		strcpy(last_speed, speed);
		strcpy(last_date, date);
		strcpy(last_time, time);
		return 1;

	} else if (atoi(speed) >= 80 && is_high_speed_under_20s == 1
			&& (atoi(time) - atoi(last_time)) >= 20) {
		is_high_speed_over_20s = 1;
	}
	if (atoi(speed) >= 80 && is_high_speed_over_20s == 1) {

		is_high_speed = 1;
		strcpy(last_latitude, latitude);
		strcpy(last_longitude, longitude);
		strcpy(last_speed, speed);
		strcpy(last_date, date);
		strcpy(last_time, time);
		//clear
//		strcpy(violations[y].latitude, "");
//		strcpy(violations[y].longitude, "");
//		strcpy(violations[y].speed, "");
//		strcpy(violations[y].date, "");
//		strcpy(violations[y].time, "");
		//mark begin high speed
		return 1;

	} else if (atoi(speed) < 80 && is_high_speed == 1) {

		strcpy(violations[y].latitude, last_latitude);
		strcpy(violations[y].longitude, last_longitude);
		strcpy(violations[y].speed, last_speed);
		strcpy(violations[y].date, last_date);
		strcpy(violations[y].time, last_time);

		is_high_speed = 0;
		//remark end high speed
		return 2;
	}
//no high speed
	is_high_speed = 0;
	is_high_speed_under_20s = 0;
	is_high_speed_over_20s = 0;
	return 0;		//strcpy(violations[y].speed, "");
}
void print_detail_traffic_violations(int no, char speed[4], char latitude[20],
		char longitude[20], char date[15], char time[15]) {
	char printTemp[100];

	sprintf(printTemp, "\r\n Lan %d: %s %s\r\n", no, date, time);
	UART2_PrintString(printTemp);
	sprintf(printTemp, "%s , %s\r\n", latitude, longitude);
	UART2_PrintString(printTemp);
	sprintf(printTemp, "TDCP/TDTT:%s/%s km/h\r\n", "80", speed);
	UART2_PrintString(printTemp);

}

void calculate_time(char* current, char* from) {
//174500-120000=5000
	char buffer[100];
	int f = atoi(current) - atoi(from);
	int x = f / 10000;
	int y;
	for (y = 0; y <= x; y++) {
		sprintf(buffer, "%d", atoi(from) + (y * 10000));
		strcpy(time[y], buffer);
	}
}
//folder test 17022014
int count_number_violations(char * date) {
	int y = 0;
	int x = 0;

	char buffer[250];
	char* dataCollection, tempData;

	int is_high_speed;
	while (x < 24) {
		if (time[x] == NULL) {
			break;
		} else {

			//violations = (traffic_violations *) realloc(violations,
			//		y * sizeof(traffic_violations));

			if (ReadData("01", date, time[x]) == 1) {
				while (f_gets(buffer, sizeof buffer, &file)) {
					//Process line of data in sdcard ::::line_of_sddata
					switch (count_violations_from_sdcard(buffer, y)) {
					case 0:
						break;
					case 1:

						break;
					case 2:
						y++;
						break;
					}
				}
			} else {
#if _DEBUG ==1
				//		UART2_PrintString("ERROR: read sdcard error");
#endif
			}
			x++;
		}

	}
	count_data.num_violation = y;
	return 1;
}

//HOW TO USE/////////////////////////////////////////////////////////////////
//int main(void) {
//	char company[] = "Khanhhoi";    // connect hoac disconnect
//	char address[] = "HA HUY GIAP";    // connect hoac disconnect
//	char vin_No[] = "4564765765765";
//	char id_device[] = "51P 637.19";
//	char ownerName[] = "PHAM THE LONG";    // connect hoac disconnect
//	char so_gplx[] = "1232132143";    // connect hoac disconnect
//	char license[] = "34534543545";
//	char license_iss_date[] = "12/12/2014";
//	char license_exp_date[] = "12/12/2024";
//
//	flash_data.company = company;
//	flash_data.address = address;
//	flash_data.vin_No = vin_No;
//	flash_data.id_device = id_device;
//	flash_data.ownerName = ownerName;
//	flash_data.so_gplx = so_gplx;
//	flash_data.license = license;
//	flash_data.license_iss_date = license_iss_date;
//	flash_data.license_exp_date = license_exp_date;
//	UART2_Init(9600);
//	KHN_SDCARD_INIT();
//
//	KHN_Print("07/04/14", "16:18:00", "1234567890", "GSB4324235665", "5", "6");
//}
//------------------------------------------------------------------------------------
void KHN_Print(char print_date[20], char print_time[20], char LXLT[50],
		char LXTN[50]) {
//store temp data
//strcpy(tmp_open,tmp_open);
//	strcpy(tmp_close,tmp_close);
	//clear
	count_data.close=0;
	count_data.open=0;
	count_data.num_violation=0;
	////////////////
	int open = 0;
	int close = 0;
	char temp[100];
	int z = 0;
	calculate_time("171700", "150000");
	count_number_violations("17022014");
	sprintf(temp, "%s\r\n", flash_data.company);
	UART2_PrintString(temp);
	sprintf(temp, "%s\r\n", flash_data.address);
	UART2_PrintString(temp);
	UART2_PrintString("********************\r\n");
	sprintf(temp, "VIN: %s\r\n", flash_data.vin_No);
	UART2_PrintString(temp);
	sprintf(temp, "BKS: %s\r\n", flash_data.id_device);
	UART2_PrintString(temp);
	UART2_PrintString("--------------------\r\n");
	sprintf(temp, "LX: %s\r\n", flash_data.ownerName);	//chuyen thanh lai xe
	UART2_PrintString(temp);
	sprintf(temp, "GPLX: %s\r\n", flash_data.license);
	UART2_PrintString(temp);
	sprintf(temp, "Cap ngay: %s\r\n", flash_data.license_iss_date);
	UART2_PrintString(temp);
	sprintf(temp, "Han den: %s\r\n", flash_data.license_exp_date);
	UART2_PrintString(temp);
	UART2_PrintString("--------------------\r\n");
	sprintf(temp, "Thoi diem in: %s %s\r\n", print_date, print_time);
	UART2_PrintString(temp);

//hien thi so lan vi pham

	int count_violations = 0;
//print out data of violen
	for (z = count_data.num_violation; z > (count_data.num_violation - 10);
			z--) {
		if (strcmp(violations[z].speed, "") == 0) {

		} else {
			print_detail_traffic_violations(count_violations + 1,
					violations[z].speed, violations[z].latitude,
					violations[z].longitude, violations[z].date,
					violations[z].time);
			count_violations++;
		}

	}

//end
	sprintf(temp, "So lan vuot toc do > 80: %d\r\n", count_data.num_violation);
	UART2_PrintString(temp);
	sprintf(temp, "So lan dong cua: %d \r\n", count_data.close);
	UART2_PrintString(temp);
	sprintf(temp, "So lan mo cua: %d \r\n", count_data.open);
	UART2_PrintString(temp);
	UART2_PrintString("--------------------\r\n");
	sprintf(temp, "TG LXLT: %s \r\n", LXLT);
	UART2_PrintString(temp);
	sprintf(temp, "TTGLXTN: %s \r\n", LXTN);
	UART2_PrintString(temp);
}

