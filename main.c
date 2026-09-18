#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Constants
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_PATIENTS 100
//Prototypes
void registerPatient();
void allocateBed();
void displayPatients();
void priorityPatients();

//Arrays
char specialtyName [NUM_SPECIALTIES][50]= {"General Practice (OPD)","Paediatrics","Cardiology","Neurology"};
int consultationFee[NUM_SPECIALTIES] = {1500,2500,4500,5000};
int consultationTime [NUM_SPECIALTIES] = {15,20,30,30};
int specialtyQueue[NUM_SPECIALTIES]={0};
int dailyPatientCap [NUM_SPECIALTIES]={30,20,12,10};

char wardName [NUM_WARDS][50]= {"General Ward","Paediatric Ward","Surgical Ward","ICU (Intensive Care Unit)"};
int dailyBedRate [NUM_WARDS]={3000,6000,12000,25000};
int totalBedCap [NUM_WARDS]={20,10,10,5};

int bedOccupancy [NUM_WARDS][20];

char patientName [MAX_PATIENTS][50];
int patientAge [MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int patientID[MAX_PATIENTS];
int specialtyID [MAX_PATIENTS];
int waitingTime[MAX_PATIENTS];
int admitted [MAX_PATIENTS];
int wardID[MAX_PATIENTS];
int bedID[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int wardCost[MAX_PATIENTS];
int baseFee[MAX_PATIENTS];
int surCharge[MAX_PATIENTS];
int grossTotal[MAX_PATIENTS];
int discount[MAX_PATIENTS];
int finalBill[MAX_PATIENTS];
int patientCount = 0;
int priorityOrder[MAX_PATIENTS];

//registration Function
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
    waitingTime[patientCount]= specialtyQueue[specialtyID[patientCount]-1] * consultationTime[specialtyID[patientCount]-1];

    specialtyQueue[specialtyID[patientCount]-1]++;

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

    //base fee calculation
    baseFee[patientCount] = consultationFee[specialtyID[patientCount]-1];

    //Surcharge calculation
    if(urgencyLevel[patientCount]== 1)
    {
        surCharge[patientCount]= 0;
    }
    else if (urgencyLevel[patientCount]== 2)
    {
        surCharge[patientCount]= baseFee[patientCount]*20/100;
    }
    else
    {
        surCharge[patientCount]= baseFee[patientCount]*50/100;
    }

    //Ward cost calculation
    if (admitted[patientCount]==1)
    {
        wardCost[patientCount]= daysAdmitted[patientCount] * dailyBedRate[wardID[patientCount]-1];
    }
    else
    {
        wardCost[patientCount]=0;
    }
    //Gross Total calculation
    grossTotal[patientCount] = baseFee[patientCount] + surCharge[patientCount]+wardCost[patientCount];

    //Age subsidy
    if (patientAge[patientCount]< 5 || patientAge[patientCount]> 65)
    {
        discount[patientCount]= grossTotal[patientCount]*15/100;
    }
    else
    {
        discount[patientCount]=0;
    }

    //Final bill calculation
    finalBill[patientCount]= grossTotal[patientCount]- discount[patientCount];

    while (getchar()!= '\n');

    patientID[patientCount] = 1001 + patientCount;

    if (admitted[patientCount]==1)
    {
        allocateBed();
    }

    patientCount++;

}

//Bed allocate function
void allocateBed()
{
    int wardIndex = wardID[patientCount] - 1;
    int bedIndex;
    int bedFound = 0;

    for (bedIndex =0;bedIndex < totalBedCap[wardIndex];bedIndex++)
    {
        if(bedOccupancy[wardIndex][bedIndex]==0)
        {
            bedOccupancy[wardIndex][bedIndex] = 1;
            bedID[patientCount]= bedIndex + 1;
            bedFound = 1;
            break;

        }

    }
    if (bedFound== 0)
    {
        printf("No Beds Available in the Selected Ward!\n");
    }


}

//display function
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

        printf("Patient ID   : PAT-%d\n",patientID[index]);
        printf("Patient Name : %s\n",patientName[index]);
        printf("Age          : %d\n",patientAge[index]);
        printf("Urgency Level: %d\n",urgencyLevel[index]);
        printf("Waiting Time : %d minutes\n",waitingTime[index]);
        printf("Base Fee     : Rs. %d\n",baseFee[index]);
        printf("Surcharge Fee: Rs. %d\n",surCharge[index]);
        printf("Ward Cost Fee: Rs. %d\n",wardCost[index]);
        printf("Gross Total  : Rs. %d\n",grossTotal[index]);
        printf("Discount     : Rs. %d\n",discount[index]);
        printf("Final Bill   : Rs. %d\n",finalBill[index]);

        printf("Specialty    : %s\n",specialtyName[specialtyID[index]-1]);
        printf("Admitted     : %d\n",admitted[index]);

        if (admitted[i] == 1)
        {
            printf("Ward         : %s\n",wardName[wardID[index]-1]);
            printf("Bed          : %d\n",bedID[index]);
            printf("Days Admitted: %d\n",daysAdmitted[index]);
        }


    }
}

//Priority function
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

//main
int main()

{
    int choice = 0;

    printf("Smart Hospital & Resource Allocation System\n");

    while(choice != 3)
    {

        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {

           case 1:
              while(getchar() != '\n');
              registerPatient();
              break;
           case 2:
              displayPatients();
              break;
           case 3:
              printf("Exiting the system...\n");
              break;

        }



    }

    return 0;
}

