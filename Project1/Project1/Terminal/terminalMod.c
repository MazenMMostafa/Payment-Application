#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "terminalMod.h"

	
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	// DD/MM/YYYY, e.g 25/06/2022 6~9
	uint8_t tDate[12] = { 0 };
	uint16_t flag = 0;
	printf("Enter Transaction Date(DD/MM/YYYY):");
	fgets(tDate, 20, stdin);
	uint8_t Day[2] = { 0 },Month[2] = { 0 }, Year[4] = { 0 };
	if (isalpha(tDate[0]) == 1 || isalpha(tDate[1]) == 1){ //Day
		flag = 1;
	}
	else if (isalpha(tDate[3]) == 1 || isalpha(tDate[4]) == 1) { //Month
		flag = 1;
	}
	else if (isalpha(tDate[6]) == 1 || isalpha(tDate[7]) == 1 || isalpha(tDate[8]) == 1 || isalpha(tDate[9]) == 1) { //year
		flag = 1;
	}
	if ((tDate[2] == '/') && (tDate[5] == '/')) {
		for (uint16_t i = 0; i < 2; i++) {
			Day[i] = tDate[i];
			Month[i] = tDate[i + 3];
		}
		for (uint16_t i = 0; i < 4; i++) {
			Year[i] = tDate[i + 6];
		}
		uint16_t dDay = 0, mMonth = 0, yYear = 0;
		dDay = atoi(Day), mMonth = atoi(Month), yYear = atoi(Year); //converting digits string to int
		//printf("%d %d %d", dDay, mMonth, yYear);
		//maximum days are 31
		if (((yYear / 2) % 2 == 1) && (mMonth == 2) && (dDay > 28)) 
			flag = 1;
		
		else if (((yYear / 2) % 2 != 1) && (mMonth == 2) && (dDay > 29))// leap year
			flag = 1;

		// 1 3 5 7 8 10 12
		else if ((mMonth == 4 || mMonth == 6 || mMonth == 9 || mMonth == 11) && (dDay > 30)) // Months Day rules
			flag = 1;
		
		else if ((mMonth == 1 || mMonth == 3 || mMonth == 5 || mMonth == 7 || mMonth == 8 || mMonth == 10 || mMonth == 12) && (dDay > 31))// Months day rules
			flag = 1;
	
		else if (yYear < 2023) 
			flag = 1;

		if (flag == 1) {
//			printf("Actual Result: WRONG_DATE\n");
			
			return WRONG_DATE;
		}
		else {
//			printf("Actual Result: TERMINAL_OK\n");
			
			for (uint16_t k = 0; k < 11; k++) {
				termData->transactionDate[k] = tDate[k];
				

			}
			termData->transactionDate[strlen(tDate)-1] = '\0';
			
		//printf("td=%s\nterm=%s\n", tDate, termData->transactionDate);
		}
		FILE* dt;
		dt = fopen("dt.tmp", "w");
		fprintf(dt, "%s", tDate);
		fclose(dt);
		return TERMINAL_OK;
	}
	else {
//		printf("Actual Result: WRONG_DATE\n");
		
		return WRONG_DATE;
	}
}
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {//MM/YY    DD/MM/YYYY
										//M							M
	uint8_t xMonth[2] = { cardData->cardExpirationDate[0],cardData->cardExpirationDate[1] }; 
									   //Y							Y
	uint8_t xYear[2] = { cardData->cardExpirationDate[3],cardData->cardExpirationDate[4] };	
									 //M+3							M+3
	uint8_t tMonth[2] = { termData->transactionDate[3],termData->transactionDate[4] };	    
									//M+3							M+3
	uint8_t tYear[2] = { termData->transactionDate[8],termData->transactionDate[9] };

	int XMonth = atoi(xMonth), XYear = atoi(xYear), TMonth = atoi(tMonth), TYear = atoi(tYear);
	if ((XYear < TYear) || ((XYear == TYear) && (XMonth < TMonth))) {
//		printf("Actual Result: EXPIRED_CARD\n");
		return EXPIRED_CARD;

	}
	else {
//		printf("Actual Result: TERMINAL_OK\n");
		return TERMINAL_OK;
		
	}
	
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	float tAmount = 0.0;
	printf("Enter Transaction Amount:");
	scanf_s("%f", &tAmount);
		if (tAmount <=0) {
		//printf("Actual Result: INVALID_CARD\n");
			return INVALID_CARD;
		}
		else {
			termData->transAmount = tAmount;
		//printf("Actual Result: TERMINAL_OK\n");
			return TERMINAL_OK;

		}
	
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	float maxAmount = 2000.000;
	float tAmount = 0;
	tAmount= termData->transAmount;
	//printf("\n%f\n", termData->transAmount);
	if (termData->maxTransAmount > 0) {
		maxAmount = termData->maxTransAmount;
	}
	if (tAmount > maxAmount) {
		//printf("Actual Result: EXCEED_MAX_AMOUNT\n");
		return EXCEED_MAX_AMOUNT;
	}
	else {
		//printf("Actual Result: TERMINAL_OK\n");
		FILE* pt;
		pt = fopen("tAm.txt", "w");
		fprintf(pt, "%f", tAmount);
		fclose(pt);
		return TERMINAL_OK;
	}
	
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount){
	termData->maxTransAmount = 2000;
	//printf("Enter Maximum Transaction Amount:");
	//scanf("%f", &maxAmount);// maxAmount =23123;
	if (maxAmount <=0) {
		//printf("Actual Result: INVALID_MAX_AMOUNT\n");
	//	printf("In this condition the max amount is at the last condition = %f\n",termData->maxTransAmount);
		return INVALID_MAX_AMOUNT;
	}
	else {
	//	printf("Actual Result: TERMINAL_OK\n");
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}
void getTransactionDateTest(void) {
	ST_terminalData_t term;
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionDate\n");
	printf("Test Case 1:\n");
	printf("Input Data: 25/04/2023\n");
	printf("Expected Result:TERMINAL_OK\n");
	getTransactionDate(&term);

	printf("\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionDate\n");
	printf("Test Case 2:\n");
	printf("Input Data: 29/02/2023\n");
	printf("Expected Result:WRONG_DATE\n");
	getTransactionDate(&term);

	printf("\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionDate\n");
	printf("Test Case 3:\n");
	printf("Input Data: 25.04/2023\n");
	printf("Expected Result:WRONG_DATE\n");
	getTransactionDate(&term);

	printf("\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionDate\n");
	printf("Test Case 4:\n");
	printf("Input Data: 25/04/2022\n");
	printf("Expected Result:WRONG_DATE\n");
	getTransactionDate(&term);

}
void isCardExpriedTest(void) {
	ST_terminalData_t x;
	ST_cardData_t y;
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isCardExpired\n");
	printf("Test Case 1:\n");
	printf("Input Data Expiration: 12/25\nInput Data Transaction: 25/04/2023\n");
	printf("Expected Result:TERMINAL_OK\n");
	getCardExpiryDate(&y);
	getTransactionDate(&x);
	isCardExpired(&y, &x);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isCardExpired\n");
	printf("Test Case 2:\n");
	printf("Input Data Expiration: 12/23\nInput Data Transaction: 25/04/2024\n");
	printf("Expected Result:EXPIRED_CARD\n");
	getCardExpiryDate(&y);
	getTransactionDate(&x);
	isCardExpired(&y, &x);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isCardExpired\n");
	printf("Test Case 3:\n");
	printf("Input Data Expiration: 05/24\nInput Data Transaction: 25/07/2024\n");
	printf("Expected Result:EXPIRED_CARD\n");
	getCardExpiryDate(&y);
	getTransactionDate(&x);
	isCardExpired(&y, &x);
}
void getTransactionAmountTest(void) {
	ST_terminalData_t x;
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionAmount\n");
	printf("Test Case 1:\n");
	printf("Input Data: 0\n");
	printf("Expected Result:INVALID_AMOUNT\n");
	getTransactionAmount(&x);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionAmount\n");
	printf("Test Case 2:\n");
	printf("Input Data: -451.22\n");
	printf("Expected Result:INVALID_AMOUNT\n");
	getTransactionAmount(&x);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:getTransactionAmount\n");
	printf("Test Case 3:\n");
	printf("Input Data: 20145.547\n");
	printf("Expected Result:TERMINAL_OK\n");
	getTransactionAmount(&x);
}
void isBelowMaxAmountTest(void){
	ST_terminalData_t term;
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBelowMaxAmount\n");
	printf("Test Case 1:\n");
	printf("Input Data: 2500\n");
	printf("Expected Result:EXCEED_MAX_AMOUNT\n");
	getTransactionAmount(&term);
	isBelowMaxAmount(&term);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBelowMaxAmount\n");
	printf("Test Case 2:\n");
	printf("Input Data: 1200.785\n");
	printf("Expected Result:TERMINAL_OK\n");
	getTransactionAmount(&term);
	isBelowMaxAmount(&term);
}
void setMaxAmountTest(void) {
	ST_terminalData_t term;

	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBelowMaxAmount\n");
	printf("Test Case 1:\n");
	printf("Input Data: 2500\n");
	printf("Expected Result:TERMINAL_OK\n");
	setMaxAmount(&term, 2000);
	getTransactionAmount(&term);
	isBelowMaxAmount(&term);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBelowMaxAmount\n");
	printf("Test Case 2:\n");
	printf("Input Data: 0\n");
	printf("Expected Result:INVALID_MAX_AMOUNT\n");
	setMaxAmount(&term, 2000);
	getTransactionAmount(&term);
	isBelowMaxAmount(&term);

	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBelowMaxAmount\n");
	printf("Test Case 3:\n");
	printf("Input Data: -152.78\n");
	printf("Expected Result:INVALID_MAX_AMOUNT\n");
	setMaxAmount(&term, 2000);
	getTransactionAmount(&term);
	isBelowMaxAmount(&term);
}