#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "billing.h"
#include "hospital.h"
#include "reports.h"
#include <ctype.h>

//Constants
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_PATIENTS 100

//Prototypes

void savePatientRecord();
void loadPatientID();
void clearScreen();
int screenOption();


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

//Clear Screen Function
void clearScreen()
{
    system("cls");
}

//Screen Option Function
int screenOption()
{
    int option;

    while(1)
    {


        printf("\n------------------------------------------------\n");
        printf("1. Return to Main Menu\n");
        printf("2. Exit System\n");
        printf("-------------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid Input! Please Enter 1 or 2\n");

            while(getchar() != '\n');
            continue;
        }

        if(option == 1 || option == 2)
        {
           return option;
        }

        printf("Invalid Choice! Please Enter 1 or 2\n");
    }
}

//main
int main()

{
    int choice = 0;

    loadBedStatus();
    loadPatientID();

    while(choice != 5)
    {
        clearScreen();

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

            printf("Press Enter to continue...");
            getchar();

            continue;
        }

        switch(choice)
        {

           case 1:
              clearScreen();
              while(getchar() != '\n');
              registerPatient();

              if(screenOption()== 2)
                choice = 5;

              break;
           case 2:
              clearScreen();
              displayPatients();

              if(screenOption()== 2)
                choice = 5;

              break;
           case 3:
              clearScreen();
              displayBedAvailability();

              if(screenOption()== 2)
                choice = 5;

              break;
           case 4:
              clearScreen();
              generateReports();

              if(screenOption()== 2)
                choice = 5;

              break;
           case 5:
              printf("Exiting the system...\n");
              break;

           default:
              printf("Invalid Choice! Please Enter a Number FROM 1 to 5\n");
              printf("Press Enter to continue...");
              getchar();
              getchar();
              break;

        }



    }
    saveBedStatus();

    return 0;
}

