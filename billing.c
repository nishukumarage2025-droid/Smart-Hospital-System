#include <stdio.h>
#include "billing.h"



extern int baseFee[MAX_PATIENTS];
extern int consultationFee[NUM_SPECIALTIES];
extern int specialtyID [MAX_PATIENTS];
extern int urgencyLevel[MAX_PATIENTS];
extern int surCharge[MAX_PATIENTS];
extern int admitted [MAX_PATIENTS];
extern int wardCost[MAX_PATIENTS];
extern int daysAdmitted[MAX_PATIENTS];
extern int dailyBedRate [NUM_WARDS];
extern int wardID[MAX_PATIENTS];
extern int grossTotal[MAX_PATIENTS];
extern int patientAge [MAX_PATIENTS];
extern float discount[MAX_PATIENTS];
extern float finalBill[MAX_PATIENTS];

//Money Format Fuction
void formatMoney(float amount)
{
    int whole =(int)amount;
    int cents =(int)((amount - whole)*100 + 0.5f);

    printf("LKR ");

    if (whole >= 1000)
    {
        printf("%d,%03d.%02d", whole / 1000, whole % 1000, cents);
    }
    else
    {
        printf("%d.%02d", whole, cents);
    }
}

//Bill Calculating Function
void calculateBilling(int index)
{
    //Base fee
    baseFee[index] = consultationFee[specialtyID[index] - 1];

    //Emergency Surcharge
    if (urgencyLevel[index] == 1)
    {
        surCharge[index] = 0;
    }
    else if(urgencyLevel[index] == 2)
    {
        surCharge[index] = baseFee[index] * 20 / 100;
    }
    else
    {
        surCharge[index] = baseFee[index] * 50 / 100;
    }

    //Ward Cost
    if(admitted[index] == 1)
    {
        wardCost[index] = daysAdmitted[index] * dailyBedRate[wardID[index] - 1];
    }
    else
    {
        wardCost[index] = 0;
    }

    //Gross Total
    grossTotal[index] = baseFee[index] + surCharge[index] + wardCost[index];

    //Age Subsidy Discount
    if (patientAge[index] < 5 || patientAge[index] > 65)
    {
        discount[index] = grossTotal[index] * 15.0 / 100.0;
    }
    else
    {
        discount[index] = 0;
    }

    //Final Bill
    finalBill[index] = grossTotal[index] - discount[index];
}

