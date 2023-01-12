#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "cardMod.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    uint8_t name[25] = { 0 };
    printf("Enter Name:");
    fgets(name,25,stdin);
    name[strlen(name)] = '\0';
    uint16_t i = strlen(name);
    uint16_t ct = 0;
    if (i < 20 || i > 24)   
    {
      //name check that in range of 20 to 24
 //       printf("Actual Result: WRONG_NAME\n");
        return WRONG_NAME;
    }
    else {
        uint16_t flag = 0;
        for (ct = 0; ct < i-1; ct++) {
            //printf("\n%c\n", name[ct]);
            if (!isalpha(name[ct]) && (name[ct] !=' ')) {
                //to check that all are characters none number exist -- !=' ' means that space are allowed
               
                flag = 1; // this flag for adding null to the last and then print card ok
               
                break;
            }
            else {
                cardData->cardHolderName[ct] = name[ct];
            }
        }
        if (flag == 0) {
            cardData->cardHolderName[i-1] = '\0'; //adding null to the last
       //     printf("Actual Result:Card Ok\n");
            FILE* fp; 
            fp= fopen("name.txt", "w");
            if (fp == NULL) {
           
            }
            fprintf(fp, "%s\n", name);
            //printf("%s", name);
            fclose(fp);
      
            return CARD_OK;
        }
        else {
    //        printf("Actual Result:WRONG_NAME\n");
            return WRONG_NAME;
        }
        }
}     

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
    uint8_t xDate[25] = { 0 };
    uint16_t flag = 0;
    printf("Expiration Date(MM/YY): ");
    fgets(xDate, 25, stdin); //MM/YY
    //first char is 0 or 1 only
    //second char to cases if first is 0 then could be from 0 to 9 but if 1 only from 0 to 2
    //third char must be "/"
    //fourth is 2 or 3
    //fifth 3 or higher to 9
   
    if (!isdigit(xDate[0]) || (!isdigit(xDate[1])) || (!isdigit(xDate[3])) || (!isdigit(xDate[4]))) {
        //0,1 and 3,4 must be digits
        flag = 1;
    }
    else if (xDate[2] != '/') {
        flag = 1;
    }
    //0 1 2 3 4 5 6 7 8 9 10 11 12
    else if (((xDate[0] == '1') && (xDate[1] > '2')) || ('0'> xDate[0] || xDate[0] > '1') || (xDate[1] > '9')) {
        flag = 1;
    }
    //23 24 25 26 ... 39
    else if ((xDate[3] <= '2' && (xDate[4] <= '2')) || (xDate[3] > '3' || xDate[3] < '2' || xDate[4]>'9')) {
        flag = 1;
    }
    if (flag == 1) {
 //       printf("Actual Result:WRONG_EXP_DATE\n");
        return WRONG_EXP_DATE;
    }
    else {
        
   //     printf("Actual Result:CARD_OK\n\n");
        for (uint16_t k = 0; k <= 5; k++) {
            cardData->cardExpirationDate[k] = xDate[k];
        }
        cardData->cardExpirationDate[5] = '\0';
        FILE* fp;
        fp = fopen("xdate.txt", "w");
        if (fp == NULL) {

        }
        fprintf(fp, "%s\n", xDate);
       // printf("\n%s\n", xDate);
        fclose(fp);
        return CARD_OK;
    }
    
    }

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
    // must be 16~19
    //only digits
    uint8_t cPAN[50] = { 0 };
    printf("Enter PAN:");
    fgets(cPAN, 50, stdin);
    cPAN[sizeof(cPAN) - 1] = '\0';
    uint16_t i = strlen(cPAN);
    uint16_t flag = 1;
    uint16_t ct = 0;
    if (i <= 19 && i >= 16) {
    for (ct = 0; ct < i; ct++) {
        if (isdigit(cPAN[ct])) {
            flag++;
            
        }
    }
    if (flag == i) {
//        printf("Actual Result:CARD_OK\n");
        strncpy(cardData->primaryAccountNumber, cPAN, sizeof(cardData->primaryAccountNumber));
        cardData->primaryAccountNumber[sizeof(cardData->primaryAccountNumber) - 1] = '\0';
        FILE* fptr;
        fptr = fopen("PAN.txt", "w");
        if (fptr == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
        fprintf(fptr, "%s", cardData->primaryAccountNumber);
        fclose(fptr);
/*
        for (uint16_t k = 0; k < i - 1 && k < sizeof(cardData->primaryAccountNumber); k++) {
            cardData->primaryAccountNumber[k] = cPAN[k];
        }
        cardData->primaryAccountNumber[sizeof(cardData->primaryAccountNumber) - 1] = '\0';
      */
        return CARD_OK;
    }
    else {
  //      printf("Actual Result:WRONG_PAN\n");
        return WRONG_PAN;
    }
        }
    else {
    //    printf("Actual Result:WRONG_PAN\n");
        return WRONG_PAN;
    }
}
        void getCardHolderNameTest(void) {
       ST_cardData_t x;
        printf("Tester Name:Mazen Mounir Moustafa\n");
        printf("Function Name:getCardHolderName\n");
        printf("Test Case 1:\n");
        printf("Input Data: Mazen Mounir Moustafa\n");
        printf("Expected Result:Card_OK\n");
        getCardHolderName(&x);
        printf("\nTester Name:Mazen Mounir Moustafa\n");
        printf("Function Name:getCardHolderName\n");
        printf("Test Case 2:\n");
        printf("Input Data: Mazen Mounir Moustafa2\n");
        printf("Expected Result:WRONG_NAME\n");
        getCardHolderName(&x);
        printf("\nTester Name:Mazen Mounir Moustafa\n");
        printf("Function Name:getCardHolderName\n");
        printf("Test Case 3:\n");
        printf("Input Data: Mazen Mounir\n");
        printf("Expected Result:WRONG_NAME\n");
        getCardHolderName(&x);
    }
        void getCardExpiryDateTest(void) {
            ST_cardData_t x;
            printf("\n\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardExpiryDate\n");
            printf("Test Case 1:\n");
            printf("Input Data : 07/23\n");
            printf("Expected Result : Card_OK\n");
            getCardExpiryDate(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardExpiryDate\n");
            printf("Test Case 2:\n");
            printf("Input Data: 07/e3\n");
            printf("Expected Result:WRONG_EXP_DATE\n");
            getCardExpiryDate(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardExpiryDate\n");
            printf("Test Case 3:\n");
            printf("Input Data: 07|23\n");
            printf("Expected Result:WRONG_EXP_DATE\n");
            getCardExpiryDate(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardExpiryDate\n");
            printf("Test Case 4:\n");
            printf("Input Data: 21/23\n");
            printf("Expected Result:WRONG_EXP_DATE\n");
            getCardExpiryDate(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardExpiryDate\n");
            printf("Test Case 5:\n");
            printf("Input Data: 07/21\n");
            printf("Expected Result:WRONG_EXP_DATE\n");
            getCardExpiryDate(&x);
        }
        void getCardPANTest(void) {
            ST_cardData_t x;
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardPAN\n");
            printf("Test Case 1:\n");
            printf("Input Data: 123456789123456789123\n");
            printf("Expected Result:WRONG_PAN\n");
            getCardPAN(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardPAN\n");
            printf("Test Case 2:\n");
            printf("Input Data: 1234567891234567E\n");
            printf("Expected Result:WRONG_PAN\n");
            getCardPAN(&x);
            printf("\nTester Name:Mazen Mounir Moustafa\n");
            printf("Function Name:getCardPAN\n");
            printf("Test Case 3:\n");
            printf("Input Data : 12345678912345678\n");
            printf("Expected Result : CARD_OK\n");
            getCardPAN(&x);
        }