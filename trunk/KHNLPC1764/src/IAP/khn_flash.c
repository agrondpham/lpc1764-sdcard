#include "iap.h"
#include "../common.h"
unsigned char src_buffer[256]; // at least 256 bytes buffer to write into flash

//int writeFlash(char* data) {
//	int x;
//	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
//		/*	Copy data from RAM to Flash, number of bytes to be written should be 256|512|1024|4096 */
//		memset(src_buffer, 0, 256); //zero init src buffer
//		for (x = 0; x < 256; x++) {
//			src_buffer[x] = data[x]; // copy phone_1 into 256 bytes buffer
//		}
//		if (u32IAP_CopyRAMToFlash(DATA_START_SECTOR, (uint32_t) &src_buffer,
//				sizeof(src_buffer)) == IAP_STA_CMD_SUCCESS) {
//			return 1;
//		} else {
//			return 0;
//		}
//	}
//}
//int clearFlash() {
//	if (u32IAP_PrepareSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
//		if (u32IAP_EraseSectors(17, 17) == IAP_STA_CMD_SUCCESS) {
//			return 1;
//		} else {
//			return 0;
//		}
//	}
//
//}
//int readFlash() {
//	memcpy(src_buffer, (void*) DATA_START_SECTOR, 256);
//	return src_buffer;
//}
