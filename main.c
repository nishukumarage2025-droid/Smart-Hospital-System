#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"

//Constants
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_PATIENTS 100

//Prototypes
void registerPatient();
int allocateBed();
void displayPatients();
void priorityPatients();
void generateReports();
void formatMoney(float amount);
void saveBedStatus();
void loadBedStatus();
void savePatientRecord();
void loadPatientID();
void calculateWaitingTime(int index);
void calculateBilling(int index);
void displayBedAvailability();


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
float discount[MAX_PATIENTS];
float finalBill[MAX_PATIENTS];
int priorityOrder[MAX_PATIENTS];
int patientCount = 0;
int nextPatientID = 1001;

//registration Function


//Waiting time Calculation Function
void calculateWaitingTime(int index)
{
    waitingTime[index] =
         specialtyQueue[specialtyID[index]- 1] * consultationTime[specialtyID[index]- 1];

    specialtyQueue[specialtyID[index] - 1]++;
}

//Bill Calculation Function

//Bed allocate function
int allocateBed()
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
        return 0;
    }

    return 1;

}

//Display Bed Availability function
void displayBedAvailability()
{
    printf("\n==================================================\n");
    printf("                BED AVAILABILITY\n");
    printf("\n==================================================\n");
    printf("O = Available    X = Occupied\n");

    for(int i = 0; i< NUM_WARDS; i++)
    {
        printf("\n%s\n", wardName[i]);
        printf("\n--------------------------------------------------\n");

        int rows;

        if (totalBedCap[i]> 10)
        {
            rows = 2;
        }
        else
        {
            rows = 1;
        }

        for(int row = 0; row < rows; row++)
        {
            int start = row * 10;
            int end = start + 10;

            if(end > totalBedCap[i])
            {
                end = totalBedCap[i];
            }

            printf("      ");

            for(int j = start; j < end;j++)
            {
                printf("%02d ", j+1);
            }

            printf("\n");

            printf("      ");

            for(int j = start; j < end; j++)
            {
                if(bedOccupancy[i][j] == 0)
                {
                    printf(" O ");
                }
                else
                {
                    printf(" X ");
                }
            }
            printf("\n\n");
        }




    }
    printf("\n==================================================\n");
}


//display function


//Priority function

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

//Money Formating Function


//Saving Beds Function
void saveBedStatus()
{
    FILE *file;

    file = fopen("beds_status.txt", "w");

    if(file == NULL)
    {
        printf("Error Opening Bed Status File!\n");
        return;
    }

    for(int i = 0; i< NUM_WARDS; i++)
    {
        for(int j = 0; j< totalBedCap[i];j++)
        {
            fprintf(file, "%d ", bedOccupancy[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

//Load bed status Function
void loadBedStatus()
{
    FILE *file;

    file = fopen("beds_status.txt", "r");

    if (file == NULL)
    {
        return;
    }

    for(int i =0; i< NUM_WARDS;i++)
    {
        for(int j = 0; j <totalBedCap[i];j++)
        {
            fscanf(file, "%d", &bedOccupancy[i][j]);
        }
    }
    fclose(file);
}

//Saving Patient Record Function
void savePatientRecord()
{
    FILE *file;

    file = fopen("patient_records.txt", "a");

    if(file == NULL)
    {
        printf("Error Opening Patient Records File!\n");
        return;
    }

    fprintf(file, "Patient ID    : PAT-%d\n", patientID[patientCount]);
    fprintf(file, "Patient Name  : %s\n", patientName[patientCount]);
    fprintf(file, "Specialty     : %s\n", specialtyName[specialtyID[patientCount]- 1]);
    fprintf(file, "Urgency Level : %d\n", urgencyLevel[patientCount]);
    fprintf(file, "Gross Total   : %d.00\n", grossTotal[patientCount]);
    fprintf(file, "Discount      : %.2f\n", discount[patientCount]);
    fprintf(file, "Final Bill    : %.2f\n", finalBill[patientCount]);
    fprintf(file, "Ward Cost     : %d.00\n", wardCost[patientCount]);
    fprintf(file, "Surcharge     : %d.00\n", surCharge[patientCount]);
    fprintf(file, "Waiting Time  : %d mins\n", waitingTime[patientCount]);
    fprintf(file, "\n");

    fclose(file);
}

//Load patient ID Function
void loadPatientID()
{
    FILE *file;
    char line[100];
    int id;

    file = fopen("patient_records.txt", "r");

    if(file == NULL)
    {
        return;
    }

    while (fgets(line, sizeof(line),file) != NULL)
    {
        if(strstr(line, "Patient ID") != NULL)
        {
            if(sscanf(line, "%*[^-]-%d", &id)== 1)
            {
                if(id >= nextPatientID)
                {
                    nextPatientID = id + 1;
                }
            }
        }

    }

    fclose(file);
}


//main
int main()

{
    int choice = 0;

    loadBedStatus();
    loadPatientID();

    while(choice != 5)
    {
        printf("\n=============================================\n");
        printf("      WELCOME TO SMART HOSPITAL SYSTEM!        \n");
        printf("=============================================\n");
        printf("1. Register Patient\n");
        printf("2. Display Patients\n");
        printf("3. Bed Availability\n");
        printf("4. Generate Reports\n");
        printf("5. Exit\n");
        printf("=============================================\n");

        printf("Enter Your Choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid Input! Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

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
              displayBedAvailability();
              break;
           case 4:
              generateReports();
              break;
           case 5:
              printf("Exiting the system...\n");
              break;

        }



    }
    saveBedStatus();

    return 0;
}

