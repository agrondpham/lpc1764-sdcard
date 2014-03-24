#include "../common.h"
#include <stdio.h>

#define speedLen 5
#define latLen  12
#define longLen 13
extern char latitude[latLen];   //    ="1049.3361"
extern char longitude[longLen];   // ="10641.8130"
extern char speed_gps[speedLen];
//Function
void process_gps_data(void);
