#include <stdio.h>
#include "reports.h"
#include "billing.h"

extern int patientCount;
extern int urgencyLevel[MAX_PATIENTS];
extern float finalBill[MAX_PATIENTS];
extern float discount[MAX_PATIENTS];
extern char patientName [MAX_PATIENTS][50];

extern int totalBedCap [NUM_WARDS];
extern int bedOccupancy [NUM_WARDS][20];
extern char wardName [NUM_WARDS][50];





//Report Generation Function
void generateReports()
{
    int normal = 0;
    int urgent = 0;
    int critical = 0;
    float totalRevenue = 0;
    float totalDiscount = 0;
    int occupiedBeds;
    int highestPatient = 0;

    if(patientCount == 0)
    {
        printf("No Patients Registered Yet!\n");
        return;
    }

    for (int i = 0; i< patientCount;i++)
    {
       switch(urgencyLevel[i])
       {
            case 1:
                normal++;
                break;
            case 2:
                urgent++;
                break;
            case 3:
                critical++;
                break;

       }

       totalRevenue += finalBill[i];
       totalDiscount += discount[i];
    }

    for(int i = 1 ; i< patientCount;i++)
    {
        if(finalBill[i] > finalBill[highestPatient])
        {
            highestPatient = i;
        }
    }

    printf("\n   Highest-Paying Patient\n");
    printf("--------------------------------\n");
    printf("Patient Name      : %s\n", patientName[highestPatient]);
    printf("Total Bill        : ");
    formatMoney(finalBill[highestPatient]);
    printf("\n");

    printf("\n   Bed Occupancy Report   \n");
    printf("--------------------------------\n");

    for(int i = 0;i < NUM_WARDS;i++)
    {
        occupiedBeds = 0;

        for(int j = 0; j < totalBedCap[i];j++)
        {
            if(bedOccupancy[i][j] == 1)
            {
                occupiedBeds++;
            }
        }

        float occupancyPercentage = (float) occupiedBeds / totalBedCap[i] * 100;
        printf("%s : %.1f%% occupied\n", wardName[i], occupancyPercentage);
    }

    printf("\n  Patient Registration Summary\n");
    printf("---------------------------------\n");
    printf("Total Patients Registered : %d\n", patientCount);

    printf("\n  Patient Count by Urgency\n");
    printf("---------------------------------\n");
    printf("Normal Patients   : %d\n", normal);
    printf("Urgent Patients   : %d\n", urgent);
    printf("Critical Patients : %d\n", critical);

    printf("\n      Financial Summary       \n");
    printf("---------------------------------\n");
    printf("Total Revenue     : ");
    formatMoney(totalRevenue);
    printf("\n");

    printf("Total Discount    : ");
    formatMoney(totalDiscount);
    printf("\n");


}

