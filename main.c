#include <stdio.h>
#include <stdlib.h>
//Constants
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_PATIENTS 100
//Prototypes
void registerPatient();

//Arrays
char specialtyName [NUM_SPECIALTIES][50]= {"General Practice (OPD)","Paediatrics","Cardiology","Neurology"};
int consultationFee[NUM_SPECIALTIES] = {1500,2500,4500,5000};
int consultationTime [NUM_SPECIALTIES] = {15,20,30,30};
int dailyPatientCap [NUM_SPECIALTIES]={30,20,12,10};

char wardName [NUM_WARDS][50]= {"General Ward","Paediatric Ward","Surgical Ward","ICU (Intensive Care Unit)"};
int dailyBedRate [NUM_WARDS]={3000,6000,12000,25000};
int totalBedCap [NUM_WARDS]={20,10,10,5};

int bedOccupancy [NUM_WARDS][20];

char patientName [MAX_PATIENTS][50];
int patientAge [MAX_PATIENTS];
int urgencyLevel[MAX_PATIENTS];
int specialtyID [MAX_PATIENTS];
int admitted [MAX_PATIENTS];
int wardID[MAX_PATIENTS];
int daysAdmitted[MAX_PATIENTS];
int patientCount = 0;


void registerPatient()
{
    if (patientCount>= MAX_PATIENTS)
    {
        printf("No More Patients Can Be Registered!\n");
        return;

    }

    printf("Enter Patient Name: ");
    fgets(patientName[patientCount], sizeof(patientName[patientCount]),stdin);

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

    while (getchar()!= '\n');

    patientCount++;

}

int main()

{
    printf("Smart Hospital & Resource Allocation System\n");
    registerPatient();

    return 0;
}

