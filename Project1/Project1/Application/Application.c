#include "Application.h"
 void appStart(void) {
    ST_cardData_t x;
    ST_terminalData_t z;
    ST_transaction_t y;
 
    if (getCardHolderName(&x) == CARD_OK) {
        if (getCardExpiryDate(&x) == CARD_OK) {
            if (getCardPAN(&x) == CARD_OK) {
                if (getTransactionDate(&z) == TERMINAL_OK) {
                    if (getTransactionAmount(&z) == TERMINAL_OK) {
                        if (isBelowMaxAmount(&z) == TERMINAL_OK) {
                            uint16_t result = recieveTransactionData(&y);
                            if (result == APPROVED) {
                                printf("Approved");
                                listSavedTransactions();
                            }
                            else if (result == FRAUD_CARD) {
                                printf("\n Account Not Found \n");
                            }
                            else if (result == DECLINED_STOLEN_CARD) {
                                printf("\n Declined Stolen Card \n");

                            }
                            else if (result == DECLINED_INSUFFECIENT_FUND) {
                                printf("\n Declined INSUFFECIENT Fund \n");
                                listSavedTransactions();
                            }
                        }
                        else
                            printf("\n Exceeded maximum Amount \n");
                    }
                        else
                            printf("\n Invalid Amount \n");
                    }
                    else
                        printf("\n Wrong TransactionDate !\n");
                }
                else
                    printf("\n WRONG PAN !\n");
            }
            else
                printf("\n Expired Card !\n");
        }
        else
            printf("\n WRONG NAME !\n");
   
 }