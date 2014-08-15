#include "iap.h"
#include "../common.h"
//unsigned char src_buffer[256]; // at least 256 bytes buffer to write into flash

//write string "Khanhhoi,HA HUY GIAP,4564765765765,51P 637.19;PHAM THE LONG,1232132143,34534543545,12122014,12/12/2024"
int writeFlash(char data[256]) {
	char src_buffer[256];
	int x;
	if (u32IAP_PrepareSectors(0, 0) == IAP_STA_CMD_SUCCESS) {
			if (u32IAP_EraseSectors(0, 0) == IAP_STA_CMD_SUCCESS) {
			}
	}

	if (u32IAP_PrepareSectors(0, 0) == IAP_STA_CMD_SUCCESS) {
			/*	Copy data from RAM to Flash, number of bytes to be written should be 256|512|1024|4096 */
			memset(src_buffer, 0, 256); //zero init src buffer

			strcpy(src_buffer, data);
			if (u32IAP_CopyRAMToFlash(DATA_START_SECTOR, (uint32_t) &src_buffer,
					sizeof(src_buffer)) == IAP_STA_CMD_SUCCESS) {
				return 1;
			} else {
				return 0;
			}
	}
	vIAP_ReinvokeISP();
}
int clearFlash() {
	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
		if (u32IAP_EraseSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
			return 1;
		} else {
			return 0;
		}
	}

}
//int readFlash() {
//	memcpy(src_buffer, (void*) DATA_START_SECTOR, 256);
//	return src_buffer;
//}
