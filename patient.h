#ifndef PATIENT_H
#define PATIENT_H

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

void registerPatient();
void displayPatients();
void priorityPatients();
void calculateWaitingTime(int index);
int allocateBed();
void calculateBilling(int index);
void savePatientRecord();


#endif // PATIENT_H
