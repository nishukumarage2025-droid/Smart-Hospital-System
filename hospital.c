#include <stdio.h>
#include "hospital.h"

extern int bedOccupancy [NUM_WARDS][20];
extern int wardID[MAX_PATIENTS];
extern int bedID[MAX_PATIENTS];
extern int patientCount;
extern int admitted [MAX_PATIENTS];
extern int totalBedCap[NUM_WARDS];
extern char wardName [NUM_WARDS][50];


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

//Display Bed Availability Function
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

//Bed Status Save Function
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
