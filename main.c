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
void generateReports();
void formatMoney(float amount);
void saveBedStatus();
void loadBedStatus();
void savePatientRecord();
void loadPatientID();


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
        discount[patientCount]= grossTotal[patientCount]*15.0/100.0;
    }
    else
    {
        discount[patientCount]=0;
    }

    //Final bill calculation
    finalBill[patientCount]= grossTotal[patientCount]- discount[patientCount];

    while (getchar()!= '\n');

    patientID[patientCount] = nextPatientID;
    nextPatientID++;

    if (admitted[patientCount]==1)
    {
        allocateBed();
    }
    savePatientRecord();

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

    while(choice != 4)
    {
        printf("\n=============================================\n");
        printf("      WELCOME TO SMART HOSPITAL SYSTEM!        \n");
        printf("=============================================\n");
        printf("1. Register Patient\n");
        printf("2. Display Patients\n");
        printf("3. Generate Reports\n");
        printf("4. Exit\n");
        printf("=============================================\n");

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
              generateReports();
              break;
           case 4:
              printf("Exiting the system...\n");
              break;

        }



    }
    saveBedStatus();

    return 0;
}

