#include "patient.h"

extern int patientCount;
extern int priorityOrder[MAX_PATIENTS];
extern int urgencyLevel[MAX_PATIENTS];


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
