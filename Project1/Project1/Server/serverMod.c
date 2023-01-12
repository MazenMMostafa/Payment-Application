#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "serverMod.h"
#include <stdbool.h>
#define nullptr ((void *)0)
#define BUFFER_SIZE 1000

EN_transState_t recieveTransactionData(ST_transaction_t* transData) {
	FILE* tStat;
	uint16_t seq = 0;
	uint16_t flag = 0;
	FILE* sequence;
	sequence = fopen("seq.txt", "r");
	fscanf(sequence, "%d",& seq);
	//printf("%d", seq);
	fclose(sequence);
	uint8_t tstat[10];
	
	//	printf("%s\n%d", tstat,strlen(tstat));
	
//				\\//PERSONAL DATA	\\//
//	EN_cardError_t cardError1 = getCardHolderName(&(transData->cardHolderData));
//	if (cardError1 != CARD_OK) {
//		printf("Wrong Name\n");
//		flag = 1;
//		exit(0);
//		//printf("%s", transData->cardHolderData.cardHolderName);
//	}
//	EN_cardError_t cardError2 = getCardExpiryDate(&(transData->cardHolderData));
//	if (cardError2 != CARD_OK) {
//		printf("Wrong Date\n");
//		flag = 1;
//		exit(0);
//		//printf("%s", transData->cardHolderData.cardExpirationDate);
//	}	
//	EN_cardError_t cardError3 = getCardPAN(&(transData->cardHolderData));
//	if (cardError3 != CARD_OK) {
//		printf("Wrong PAN\n");
//		flag = 1;
//		exit(0);
//	}
//	
//	
////				\\// TRANSACTION DATA	\\//
//	EN_terminalError_t transError1 = getTransactionDate(&(transData->terminalData));
//	if (transError1 != TERMINAL_OK) {
//		printf("Wrong Date");
//		flag = 1;
//		exit(0);
//		
//	}
//	
//	setMaxAmount(&(transData->terminalData), 2000.000);
//	EN_terminalError_t transError2 = getTransactionAmount(&(transData->terminalData));
//	if (transError2 != TERMINAL_OK) {
//		printf("Wrong Amount");
//		flag = 1;
//		exit(0);
//		//printf("%f", transData->terminalData.transAmount);
//	}
//
//	EN_terminalError_t transError3 = isBelowMaxAmount(&(transData->terminalData));
//	if (transError3 != TERMINAL_OK) {
//		tStat = fopen("stat.tmp", "w");
//		fprintf(tStat, "%s", "Failed");
//		fclose(tStat);
//		flag = 1;
//	//	printf("%f", transData->terminalData.maxTransAmount);
//	}
//	
	ST_cardData_t l;
	ST_accountsDB_t k;

	if (isValidAccount(&l,&k) == ACCOUNT_NOT_FOUND) {
		seq++;
		sequence = fopen("seq.txt", "w");
		fprintf(sequence, "%d", seq);
		fclose(sequence);
		flag = 1;
		return FRAUD_CARD;
	}
	if (isBlockedAccount(&k) == BLOCKED_ACCOUNT) {
		seq++;
		sequence = fopen("seq.txt", "w");
		fprintf(sequence, "%d", seq);
		fclose(sequence);
		flag = 1;
		return DECLINED_STOLEN_CARD;

	}
	if (isAmountAvailable(&transData->terminalData.transAmount, &k) == LOW_BALANCE) {
		seq++;
		sequence = fopen("seq.txt", "w");
		fprintf(sequence, "%d", seq);
		fclose(sequence);
		flag = 1;
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (saveTransaction(&transData->cardHolderData) == SAVING_FAILED) {
		seq++;
		sequence = fopen("seq.txt", "w");
		fprintf(sequence, "%d", seq);
		fclose(sequence);
		flag = 1;
		return INTERNAL_SERVER_ERROR;
	}
	if (flag == 0) {
		seq++;
		sequence = fopen("seq.txt", "w");
		fprintf(sequence, "%d", seq);
		fclose(sequence);
		//printf("\nApproved\n");
		return APPROVED;
	}
};
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {
	uint8_t str[100];
	FILE* fr;
	fr = fopen("PAN.txt", "r");
	if (fr == NULL) {
		//printf("Error opening file!\n");
		return 1;
	}
	fgets(str, 100, fr);
	//str[strlen(str)-1] = '\0';
	strncpy(cardData->primaryAccountNumber, str, strlen(str));
	cardData->primaryAccountNumber[strlen(str)-1] = '\0';
	//printf("\nGetpancheck=%s\n\n%s\n%d\n%d\n", cardData->primaryAccountNumber,str,sizeof(str),strlen(str));
	fclose(fr);
	FILE* fptr;
	uint16_t fflag = 0, ctp = 0, cts = 0, ctb = 0, ctc = 0, l = 0, line = 0;;
	uint8_t stt[255] = { 0 };
	uint8_t stat[7] = { 0 };
	uint8_t pan[50] = { 0 };
	uint8_t chkB[7] = { "BLOCKED" };
	uint8_t chkR[7] = { "RUNNING" };
	uint16_t cchkB = 0, cchkR = 0;
	uint8_t bala[50] = { 0 };
	float balance = 0;
	fptr = fopen("Accounts_DB.txt", "r");
	for (uint16_t z = 0; getc(fptr) != EOF; z++) {
	line++;
	ctp = 0, cts = 0, ctb = 0, ctc = 0, l = 0;
	fscanf(fptr, "%[^\n]s", stt);
	for (ctc = 0; ctc < strlen(stt); ctc++) {

		if (stt[ctc] == 123) { // curly braces
		}
		else {
			if (stt[ctc] == ',') {
				fflag++;
			}
			else if (stt[ctc] == 125) {
				fflag = 0;
			}
			if (fflag == 0 && stt[ctc] != ',' && stt[ctc] != ' ' && stt[ctc] != 125) {
				bala[ctb] = stt[ctc];
				ctb++;
			}
			else if (fflag == 1 && stt[ctc] != ',' && stt[ctc] != ' ' && stt[ctc] != 125) {
				stat[cts] = stt[ctc];
				cts++;
			}
			else if (fflag == 2 && stt[ctc] != ',' && stt[ctc] != ' ' && stt[ctc] != 125 && stt[ctc] != 34) {
				pan[ctp] = stt[ctc];
				ctp++;
			}
		}
	}
	pan[ctp] = '\0';
//cardData->primaryAccountNumber
	if (strncmp(pan, cardData->primaryAccountNumber, strlen(pan)) == 0)
	{
		//accountRefrence->state = RUNNING;
		break;
	}
}
fclose(fptr);
FILE* s;
s = fopen("Tempe.tmp", "w");
fwrite(pan, strlen(pan), 1, s);
fclose(s);
{
	FILE* LINE;
	LINE = fopen("line.txt", "w");
	fwrite(&line, sizeof(int), 1, LINE);
	//fprintf(LINE, "%d", line);
	fclose(LINE);
}
if (strncmp(pan, cardData->primaryAccountNumber, strlen(pan)) != 0) {
	//printf("%s\n%s\n%d\n",pan, cardData->primaryAccountNumber, strcmp(pan, cardData->primaryAccountNumber));
	//printf("Actual Result:ACCOUNT_NOT_FOUND\n");
	return ACCOUNT_NOT_FOUND;
}
else
//printf("Actual Result:ACCOUNT_FOUND\n");
balance = atof(bala);
accountRefrence->balance = balance;
for (uint16_t jj = 0; jj < l; jj++) {
	accountRefrence->primaryAccountNumber[jj] = pan[jj];
}
for (uint16_t h = 0; h < 7; h++) {
	if (chkB[h] == stat[h])
		cchkB++;
	else if (chkR[h] == stat[h])
		cchkR++;
}
FILE* sta;
uint8_t kk[8];
sta = fopen("sta.txt", "w");
if (sta == nullptr) {
	printf("\nError State\n");
}
else {
	if (cchkB == 7) {
		fwrite("BLOCKED", 7, 1, sta);
		cchkB = 0;
		accountRefrence->state = BLOCKED;
	}
	else if (cchkR == 7) {
		fwrite("RUNNING", 7, 1, sta);
		cchkR = 0;
		accountRefrence->state = RUNNING;
	}
	fclose(sta);
}
};
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == BLOCKED) {
	//	printf("Actual Result:DECLINED_STOLEN_CARD\n");
		return BLOCKED_ACCOUNT;
	}
	else if (accountRefrence->state == RUNNING) {
		//printf("Actual Result:RUNNING\n");
		return SERVER_OK;
	}
	else if (accountRefrence->state == ACCOUNT_NOT_FOUND)
		return ACCOUNT_NOT_FOUND;
		
};
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {

	FILE* tb;
	float newbalance;
	float tAmountRead;
	FILE* pt;
	pt = fopen("tAm.txt", "r");
	fscanf(pt, "%f", &tAmountRead);
	fclose(pt);
	//	printf("%f\n%f\n", termData->transAmount, accountRefrence->balance);
	if(tAmountRead > accountRefrence->balance){
	
		//printf("Actual Result:DECLINED_INSUFFECIENT_FUND");

		return LOW_BALANCE;
		
	}
	else if ((tAmountRead <= accountRefrence->balance)) {
		FILE* sta;
		uint8_t stat[8];
		sta = fopen("sta.txt", "r");
		fread(&stat, 7, 1, sta);
		stat[7] = '\0';
		fclose(sta);
		//printf("\nkk=%s\n", stat);
		if (stat == "BLOCKED") {
			
		}
		else {
			newbalance = accountRefrence->balance - tAmountRead;
			//printf("Actual Result:SERVER_OK");
			tb = fopen("tb.txt", "w");
			fprintf(tb,"%f", newbalance);
			fclose(tb);
			//printf("\n%f\n%f\n", newbalance, tAmountRead);
			return SERVER_OK;

		}
	}
	else if ((termData->maxTransAmount < termData->transAmount)) {
		// printf("Actual Result:EXCEED_MAX_AMOUNT\n");
	}
};
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	
	// state //
	FILE* sta;
	uint8_t stat[8];
	sta = fopen("sta.txt", "r");
	fread(stat, 7, 1, sta);
	stat[7] = '\0'; // null-terminate stat
	fclose(sta);

	uint8_t tstat[10];
	FILE* tStat;
	tStat = fopen("stat.tmp", "r");
	fscanf(tStat, "%s", tstat);
	fclose(tStat);

	if (strcmp((uint8_t*)stat, "BLOCKED") == 0) {
		
		FILE* tStat;
		tStat = fopen("stat.tmp", "w"); // open file for writing
		if (tStat == NULL) {
		}
		else
		fprintf(tStat,"Failed");
		fclose(tStat);
	}
	else{
	/*uint8_t name[25] = { 0 }, date[6] = { 0 };
	strncpy(date, transData->cardHolderData.cardExpirationDate, 6);
	strncpy(name, transData->cardHolderData.cardHolderName,25);*/
	float balance = 0;
	FILE* tb;
	tb = fopen("tb.txt", "r");
	// BALANCE //
	fscanf(tb, "%f", &balance);

	//printf("%f\n", balance);
	fclose(tb);
	FILE* LINE;
	uint16_t line = 0;
	// FILE LINE //
	LINE = fopen("line.txt", "r");
	fseek(LINE, 0, SEEK_SET);
	fread(&line, sizeof(uint16_t), 1, LINE);
	//printf("\nline=%d\n", line);
	fclose(LINE);
	FILE* s;
	uint8_t  pan[20] = { 0 };
	// PAN //
	s = fopen("Tempe.tmp", "r");
	if (s == nullptr) {
		printf	("Error Server\n");
	}
	else {
		fread(&pan, sizeof(uint8_t), 20, s);
		//printf("%s\n", pan);
	}
	fclose(s);
	//printf("%f", balance);

		FILE* fptr;
		FILE* ftemp;
		uint8_t buffer[BUFFER_SIZE];
		uint8_t newline[BUFFER_SIZE];
		uint16_t count;
		fflush(stdin);
		fptr = fopen("Accounts_DB.txt", "r");
		ftemp = fopen("replace.txt", "w");
		if (fptr == nullptr || ftemp == nullptr)
		{
			printf("\nUnable to open file.\n");
			printf("Please check whether file exists and you have read/write privilege.\n");
			exit(EXIT_SUCCESS);
		}

		count = 0;
		while ((fgets(buffer, BUFFER_SIZE, fptr)) != nullptr)
		{
			count++;
			if (count == line)
				fprintf(ftemp, "{%f, %s, \"%s\"}\n", balance, stat, pan);
			else
				fputs(buffer, ftemp);
		}
		fclose(fptr);
		fclose(ftemp);
		if (remove("Accounts_DB.txt") == 0 || remove("sta.txt") == 0 || remove("tb.txt") == 0) {
			//printf("Removed\n");
		}
		else
			printf("Not removed");
		bool worked = rename("replace.txt", "Accounts_DB.txt");
		rename("replace.txt", "Accounts_DB.txt");
		//printf("%d", worked);
		//printf("\nSuccessfully Completed");
		FILE* tStat;
		tStat = fopen("stat.tmp", "w"); // open file for writing
		if (tStat == NULL) {
		}
		else {
			;
			fprintf(tStat, "Success");
		}
		fclose(tStat);
	
		return APPROVED;
	}
	
};
void listSavedTransactions(void) {
	uint16_t seq = 0;
	FILE* sequence;
	sequence = fopen("seq.txt", "r");
	fscanf(sequence, "%d", &seq);
	fclose(sequence);
	
	uint8_t datee[20];
	FILE* dt;
	dt = fopen("dt.tmp", "r");
	fscanf(dt, "%s", datee);
	fclose(dt);

	float balance;
	FILE* tb;
	tb = fopen("tAm.txt", "r");
	fscanf(tb, "%f", &balance);
	//printf("%f", balance);
	fclose(tb);
	
	uint8_t tstat[64];
	FILE* tStat;
	tStat = fopen("stat.tmp", "r");
	if (tStat == NULL) {
		printf("internal server error !");
		return 1;
	}
	fscanf(tStat, "%s", &tstat);
	fclose(tStat);
	tstat[strlen(tstat)] = '\0';
	//printf("%s", tstat);
	uint8_t name[25];
	FILE* fp;
	fp = fopen("name.txt", "r");
	if (fp == NULL) {
		return 1;
	}
	fscanf(fp, "%[^\n]s", &name);
	fclose(fp);

	uint8_t pan[20] = { 0 };
	FILE* s;
	s = fopen("Tempe.tmp", "r");
	if (s == NULL) {
		printf("Error Server\n");
		return 1;
	}

	fread(&pan, sizeof(uint8_t), 20, s);
	fclose(s);

	uint8_t date[25];
	fp = fopen("xdate.txt", "r");
	if (fp == NULL) {
		return 1;
	}
	fscanf(fp, "%[^\n]s", &date);
	fclose(fp);

	FILE* output;
	output = fopen("transaction_info.txt", "a");
	if (output == NULL) {
		return 1;
	}

	fprintf(output, "#########################\n");
	fprintf(output, "Transaction Sequence Number: %d\n", seq);
	fprintf(output, "Transaction Date :%s\n", datee);
	fprintf(output, "Transaction Amount :%f\n", balance);
	fprintf(output, "Transaction State :%s\n", tstat);
	fprintf(output, "Cardholder Name :%s\n", name);
	fprintf(output, "PAN:%s\n", pan);
	fprintf(output, "Card Expiration Date :%s\n", date);
	fprintf(output, "#########################\n");

	fclose(output);
	//remove("stat.tmp");
	return 0;
}
void recieveTransactionDataTest(void) {
	ST_transaction_t x;
	//printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	//printf("Function Name:recieveTransactionData\n");
	//printf("Test Case 1:\n");
	//printf("Input Data : mazen mounir mostafa\nInput Data : 12/25\nInput Data : 5807007076043875\nInput Data : 12/12/2023\nInput Data : 1200\n");
	//printf("Expected Result : DECLINED_STOLEN_CARD\n");
	//recieveTransactionData(&x);
	//saveTransaction(&x);
	//listSavedTransactions();
	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:recieveTransactionData\n");
	printf("Test Case 2:\n");
	printf("Input Data : mazen mounir mostafa\nInput Data : 12/25\nInput Data : 8989374615436851\nInput Data : 12/12/2023\nInput Data : 1200\n");
	printf("Expected Result : APPROVED\n");
	recieveTransactionData(&x);
	saveTransaction(&x);
	listSavedTransactions();
}
void isValidAccounttest(void) {
	ST_cardData_t x;
	ST_accountsDB_t y;
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isValidAccount\n");
	printf("Test Case 1:\n");
	printf("Input Data: 12312312312312312\n");
	printf("Expected Result:ACCOUNT_NOT_FOUND\n");
	getCardPAN(&x);
	isValidAccount(&x, &y);
	printf("Tester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isValidAccount\n");
	printf("Test Case 2:\n");
	printf("Input Data: 5807007076043875\n");
	printf("Expected Result:ACCOUNT_FOUND\n");
	getCardPAN(&x);
	isValidAccount(&x, &y);
}
void isBlockedAccountTest(void) {
	ST_cardData_t x;
	ST_accountsDB_t y;
	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBlockedAccount\n");
	printf("Test Case 1:\n");
	printf("Input Data : 5807007076043875\n");
	printf("Expected Result : DECLINED_STOLEN_CARD\n");
	getCardPAN(&x);
	isValidAccount(&x, &y);
	isBlockedAccount(&y);
	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isBlockedAccount\n");
	printf("Test Case 2:\n");
	printf("Input Data : 8989374615436851\n");
	printf("Expected Result : RUNNING\n");
	getCardPAN(&x);
	isValidAccount(&x, &y);
	isBlockedAccount(&y);


}
void isAmountAvailableTest(void) {
	ST_cardData_t x;
	ST_accountsDB_t y;
	ST_terminalData_t z;
	//ST_transaction_t u;
	//printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	//printf("Function Name:isAmountAvailable\n");
	//printf("Test Case 1:\n");
	//printf("Input Data : 3221374615434561\nInput Data : 1200\n");
	//printf("Expected Result : DECLINED_INSUFFECIENT_FUND\n");
	//getCardPAN(&x);
	//isValidAccount(&x, &y);
	//isBlockedAccount(&y);
	//getTransactionAmount(&z);
	//isBelowMaxAmount(&z);
	//isAmountAvailable(&z, &y);
	
	printf("\n\nTester Name:Mazen Mounir Moustafa\n");
	printf("Function Name:isAmountAvailable\n");
	printf("Test Case 2:\n");
	printf("Input Data : 4189344615436851\nInput Data : 1200\n");
	printf("Expected Result : SERVER_OK\n");
	getCardPAN(&x);
	isValidAccount(&x, &y);
	isBlockedAccount(&y);
	getTransactionAmount(&z);
	isBelowMaxAmount(&z);
	isAmountAvailable(&z, &y);

}