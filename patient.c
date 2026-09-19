#include <stdio.h>
#include <string.h>
#include "patient.h"

extern int patientCount;
extern int priorityOrder[MAX_PATIENTS];
extern int urgencyLevel[MAX_PATIENTS];
extern char patientName [MAX_PATIENTS][50];
extern int patientAge [MAX_PATIENTS];
extern int specialtyID [MAX_PATIENTS];
extern int waitingTime[MAX_PATIENTS];
extern int admitted [MAX_PATIENTS];
extern int wardID[MAX_PATIENTS];
extern int daysAdmitted[MAX_PATIENTS];
extern int patientID[MAX_PATIENTS];
extern int nextPatientID;
extern int wardCost[MAX_PATIENTS];
extern int baseFee[MAX_PATIENTS];
extern int surCharge[MAX_PATIENTS];
extern int grossTotal[MAX_PATIENTS];
extern float discount[MAX_PATIENTS];
extern float finalBill[MAX_PATIENTS];

//Registration Function
void registerPatient()
{
    if (patientCount>= MAX_PATIENTS)
    {
        printf("No More Patients Can Be Registered!\n");
        return;

    }

    printf("Enter Patient Name: ");
    fgets(patientName[patientCount], sizeof(patientName[patientCount]),stdin);
    patientName[patientCount][strcspn(patientName[patientCount], "\n")] ='\0';

    printf("Enter Patient Age: ");
    scanf("%d",&patientAge[patientCount]);

    printf("Enter Urgency Level (1-Normal,2-Urgent,3-Critical): ");
    scanf("%d",&urgencyLevel[patientCount]);

    while(urgencyLevel[patientCount]<1 || urgencyLevel[patientCount]>3)
    {
        printf("Invalid Input! Enter a number between 1-3 : ");
        scanf("%d",&urgencyLevel[patientCount]);
    }

    printf("Enter Specialty ID (1-4): ");
    scanf("%d",&specialtyID[patientCount]);

    while(specialtyID[patientCount]<1 || specialtyID[patientCount]>4)
    {
        printf("Invalid Input! Enter a number between 1-4 : ");
        scanf("%d",&specialtyID[patientCount]);
    }


    //waiting time and queue

    calculateWaitingTime(patientCount);


    //
    printf("Is the patient admitted? (1-Yes,0-No): ");
    scanf("%d",&admitted[patientCount]);

    while(admitted[patientCount]!= 0 && admitted[patientCount]!= 1)
    {
        printf("Invalid Input! Enter 1 or 0 : ");
        scanf("%d",&admitted[patientCount]);
    }

    if(admitted[patientCount]== 1)
    {
        printf("Enter Ward ID (1-4): ");
        scanf("%d",&wardID[patientCount]);

        while (wardID[patientCount]<1||wardID[patientCount]>4)
        {
            printf("Invalid Input! Enter a number between 1-4: ");
            scanf("%d",&wardID[patientCount]);
        }

        printf("Enter Number of Days Admitted: ");
        scanf("%d",&daysAdmitted[patientCount]);

        while(daysAdmitted[patientCount]<= 0)
        {
            printf("Invalid Input! Enter a positive number of days: ");
            scanf("%d",&daysAdmitted[patientCount]);
        }
    }
    else
    {
        wardID[patientCount]=0;
        daysAdmitted[patientCount]=0;
    }

    //Billing Calculations
    calculateBilling(patientCount);



    while (getchar()!= '\n');

    patientID[patientCount] = nextPatientID;
    nextPatientID++;
    //Allocate bed
    if (admitted[patientCount]==1)
    {
        if (allocateBed()== 0)
        {
            admitted[patientCount] = 0;
            wardID[patientCount] = 0;
            daysAdmitted[patientCount]= 0;
            wardCost[patientCount] = 0;
        }
    }

    //new bug fix
    if(admitted[patientCount]== 0)
    {
        wardCost[patientCount] = 0;

        grossTotal[patientCount] = baseFee[patientCount] + surCharge[patientCount];

        if(patientAge[patientCount] < 5 || patientAge[patientCount] > 65)
        {
            discount[patientCount] = grossTotal[patientCount] * 15.0 / 100.0;
        }
        else
        {
            discount[patientCount] = 0;
        }

        finalBill[patientCount] = grossTotal[patientCount] - discount[patientCount];
    }


    savePatientRecord();

    patientCount++;

}


//Priority Function

void priorityPatients()
{
    int i;

    for(i=0; i< patientCount;i++)
    {
       priorityOrder[i] = i;
    }

    for(i=0; i< patientCount - 1;i++)
    {
        for(int j=0; j< patientCount - i - 1; j++)
        {
            if(urgencyLevel[priorityOrder[j]] < urgencyLevel[priorityOrder[j+1]])
            {
                int temp;

                temp = priorityOrder[j];
                priorityOrder[j] = priorityOrder[j+1];
                priorityOrder[j+1] = temp;

            }

        }
    }

}
