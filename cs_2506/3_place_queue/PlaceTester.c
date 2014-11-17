#include "Place.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

void print_place(Place* const p);

int main() {
	Place p1;
	Place p2;
	Place p3;
	Place p4;
	Place p5;
	Place p6;
	
	uint32_t FID1 = 123456789;
	uint32_t FID2 = 987654321;
	uint32_t FID3 = 123456789;
	
	char* name1 = "John doe";
	char* name2 = "Doe John";
	char* name3 = "John doe";
	char* lat1 = "12345";
	char* lat2 = "54321";
	char* lat3 = "12345";
	char* long1 = "67890";
	char* long2 = "09876";
	char* long3 = "67890";
	
	bool result = Place_Set(&p1, FID1, name1, lat1, long1);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p1);
	}
	
	result = Place_Set(&p2, FID1, name1, lat1, long2);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p2);
	}
	
	result = Place_Set(&p3, FID1, name1, lat2, long2);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p3);
	}
	
	result = Place_Set(&p4, FID1, name2, lat2, long2);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p4);
	}
	
	result = Place_Set(&p5, FID2, name2, lat2, long2);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p5);
	}
	
	result = Place_Set(&p6, FID3, name3, lat3, long3);
	printf("Place_set:\t%s\n", result ? "true" : "false");
	if (result) {
		printf("Resulting Place\n");
		print_place(&p6);
	}
	
	result = Place_Equals(&p1, &p2);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
	
	result = Place_Equals(&p1, &p3);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
	
	result = Place_Equals(&p2, &p4);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
	
	result = Place_Equals(&p1, &p5);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
	
	result = Place_Equals(&p1, &p1);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
	
	result = Place_Equals(&p1, &p6);
	printf("Place_Equals:\t%s\n", result ? "true" : "false");
}

void print_place(Place* const p) {
	printf("\tFID :\t%"PRId32"\n", p->FID);
	printf("\tName:\t%s\n", p->Name);
	printf("\tLati:\t%s\n", p->Latitude);
	printf("\tLong:\t%s\n", p->Longitude);
}
