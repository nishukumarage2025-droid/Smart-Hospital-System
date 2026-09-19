#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "patient.h"
#include "billing.h"


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
extern int bedID[MAX_PATIENTS];
extern char specialtyName[NUM_SPECIALTIES][50];
extern char wardName[NUM_WARDS][50];
extern int specialtyQueue[NUM_SPECIALTIES];
extern int consultationTime[NUM_SPECIALTIES];

//Registration Function
void registerPatient()
{
    if (patientCount>= MAX_PATIENTS)
    {
        printf("No More Patients Can Be Registered!\n");
        return;

    }

    printf("\n==================================================\n");
    printf("               PATIENT REGISTRATION\n");
    printf("==================================================\n\n");

    printf("Patient Information\n");
    printf("--------------------------------------------------\n");

    printf("Enter Patient Name: ");
    fgets(patientName[patientCount], sizeof(patientName[patientCount]),stdin);
    patientName[patientCount][strcspn(patientName[patientCount], "\n")] ='\0';

    int validName = 1;

    for(int i = 0; patientName[patientCount][i] != '\0'; i++)
    {
        if(!isalpha(patientName[patientCount][i]) && patientName[patientCount][i] != ' ')
        {
            validName = 0;
            break;
        }
    }

    while(!validName)
    {
        printf("Invalid Name! Please Use Letters & Spaces Only: ");

        fgets(patientName[patientCount], sizeof(patientName[patientCount]),stdin);
        patientName[patientCount][strcspn(patientName[patientCount], "\n")] ='\0';

        validName = 1;

        for(int i = 0; patientName[patientCount][i] != '\0';i++)
        {
            if(!isalpha(patientName[patientCount][i]) && patientName[patientCount][i] != ' ')
            {
                validName = 0;
                break;
            }
        }

    }

    //
    printf("Enter Patient Age : ");

    while(scanf("%d", &patientAge[patientCount])!= 1 || patientAge[patientCount] < 0)
    {
        printf("Invalid Age! Please Enter a Valid Age: ");

        while(getchar() != '\n');
    }

    //
    printf("\nUrgency Level\n");
    printf("--------------------------------------------------\n");
    printf("1. Normal\n");
    printf("2. Urgent\n");
    printf("3. Critical\n\n");

    printf("Enter Urgency Level: ");
    scanf("%d",&urgencyLevel[patientCount]);

    while(urgencyLevel[patientCount]<1 || urgencyLevel[patientCount]>3)
    {
        printf("Invalid Input! Enter a number between 1-3 : ");
        scanf("%d",&urgencyLevel[patientCount]);
    }
    //
    printf("\nSpecialty\n");
    printf("--------------------------------------------------\n");
    printf("1. General Practice (OPD)\n");
    printf("2. Paediatrics\n");
    printf("3. Cardiology\n");
    printf("4. Neurology\n\n");

    printf("Enter Specialty ID: ");
    scanf("%d",&specialtyID[patientCount]);

    while(specialtyID[patientCount]<1 || specialtyID[patientCount]>4)
    {
        printf("Invalid Input! Enter a number between 1-4 : ");
        scanf("%d",&specialtyID[patientCount]);
    }


    //waiting time and queue

    calculateWaitingTime(patientCount);


    //
    printf("\nAdmission\n");
    printf("--------------------------------------------------\n");
    printf("1 - Yes\n");
    printf("2 - No\n\n");

    printf("Is the patient admitted?: ");
    scanf("%d",&admitted[patientCount]);

    while(admitted[patientCount]!= 0 && admitted[patientCount]!= 1)
    {
        printf("Invalid Input! Enter 1 or 0 : ");
        scanf("%d",&admitted[patientCount]);
    }

    if(admitted[patientCount]== 1)
    {
        printf("\nWard Selection\n");
        printf("--------------------------------------------------\n");
        printf("1 - General Ward\n");
        printf("2 - Paediatric Ward\n");
        printf("3 - Surgical Ward\n");
        printf("4 - ICU (Intensive Care Unit)\n\n");

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


//Waiting time Calculation Function
void calculateWaitingTime(int index)
{
    waitingTime[index] =
         specialtyQueue[specialtyID[index]- 1] * consultationTime[specialtyID[index]- 1];

    specialtyQueue[specialtyID[index] - 1]++;
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

//Display Function

void displayPatients()
{
    if (patientCount==0)
    {
        printf("No patients registered.\n");
        return;
    }

    priorityPatients();

    for(int i=0; i<patientCount;i++)
    {
        int index = priorityOrder[i];

        printf("\n================================================\n");
        printf("          SMART HOSPITAL ADMISSION & BILL\n");
        printf("------------------------------------------------\n\n");

        printf("Patient ID                : PAT-%d\n",patientID[index]);
        printf("Patient Name              : %s\n",patientName[index]);
        if(patientAge[index] < 5 || patientAge[index]> 65)
        {
            printf("Age                       : %d Years (15%% Subsidy Eligible)\n", patientAge[index]);
        }
        else
        {
            printf("Age                       : %d Years\n", patientAge[index]);
        }
        printf("Specialty                 : %s\n",specialtyName[specialtyID[index]-1]);
        printf("Urgency Level             : Level %d ",urgencyLevel[index]);

        if(urgencyLevel[index]== 1)
            printf("(Normal)\n");
        else if(urgencyLevel[index]== 2)
            printf("(Urgent)\n");
        else
            printf("(Critical)\n");

         //

        if(admitted[index] == 1)
        {
            printf("Admission                 : Yes\n");
        }
        else
        {
            printf("Admission                 : No\n");
        }
        //
        if (admitted[index] == 1)
        {
            printf("Assigned Ward             : %s (Bed #%02d)\n",wardName[wardID[index]-1],bedID[index]);

            printf("Days Admitted             : %d Days\n",daysAdmitted[index]);
        }

        printf("\n------------------------------------------------\n");


        printf("Base Consultation Fee     : ");
        formatMoney(baseFee[index]);
        printf("\n");
        //
        printf("Emergency Surcharge Fee   : ");
        formatMoney(surCharge[index]);

        if(urgencyLevel[index]==2)
        {
            printf(" (20%%)");

        }
        else if(urgencyLevel[index]==3)
        {
            printf(" (50%%)");
        }
        printf("\n");

        //
        if (admitted[index]== 1)
        {
            printf("Ward Stay Cost (%d Days)   : ",daysAdmitted[index]);
            formatMoney(wardCost[index]);
            printf("\n");
        }
        else
        {
            printf("Ward Stay Cost            : ");
            formatMoney(wardCost[index]);
            printf("\n");
        }
        printf("\n------------------------------------------------\n");
        //
        printf("Gross Total Bill          : ");
        formatMoney(grossTotal[index]);
        printf("\n");

        //
        if (discount[index]> 0)
        {
            printf("Age Subsidy Discount      : LKR -");
            printf("%.2f",discount[index]);
            printf(" (15%%)\n");
        }
        else
        {
            printf("Age Subsidy Discount      : LKR 0.00\n");
        }
        printf("\n------------------------------------------------\n");
        //
        printf("Final Payable Amount      : ");
        formatMoney(finalBill[index]);
        printf("\n");
        //
        if (waitingTime[index] == 0)
        {
            printf("Estimated Waiting Time    : 0.00 mins (Immediate Attention)\n");
        }
        else
        {
            printf("Estimated Waiting Time    : %.2f mins\n",(float)waitingTime[index]);

        }
        printf("\n================================================\n");

    }
}
