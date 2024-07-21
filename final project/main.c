#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structs.h"
#include "functions.h"

int menu()
{
    char* options[] = {
        "1 - Admit patient",
        "2 - Check for patient's allergies",
        "3 - Display all patients",
        "4 - Display all patient's admissions",
        "5 - Display all patients in line",
        "6 - Advance patient in line",
        "7 - Display list of doctors",
        "8 - Display all patients assigned to a doctor",
        "9 - Discharge patient",
        "10 - Remove visit",
        "11 - Remove patient",
        "12 - Close the hospital" ,
        "0 - Exit Program"
    };

    printf("Please choose an option:\n");
    int option_size = sizeof(options) / sizeof(options[0]);
    for (int i = 0; i < option_size; i++)
        printf("%s\n", options[i]);

    int choice;
    printf("User's choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a number.\n");
        while (getchar() != '\n'); 
        return -1;
    }

    if (choice < 0 || choice > 12) {
        printf("Invalid choice. Please choose a number between 0 and %d.\n", option_size - 1);
        return -1;
    }
    printf("\n");
    return choice;
}

int main()
{

	ll* docList = loadDoctors("Doctors.txt");
	pTree* tree = loadPatients(&docList,"Patients.txt");
	pLine* line = loadLine(tree,"Line.txt");

	while(1){
		int choice = menu();
		switch (choice){
			case 0:
				printf("System shuting down\n");
				updateFiles(tree,line,&docList);
				return 0;
            case 1:
                line->myLine = admitPatient(tree->myTree, docList, line->myLine);
                break;
            case 2:
                checkForAllergies(line->myLine);
                break;
            case 3:
                displayAllPatients(tree->myTree);
                printf("\n");
				break;
            case 4:
                displayPatientAdmissions(tree->myTree);
                break;
            case 5:
                displayPatientsInLine(line->myLine,1);
                printf("\n");
                break;
            case 6:
                line->myLine = advancePatientInLine(line->myLine);
                printf("\n");
                break;
            case 7:
                displayDoctors(docList);
                printf("\n");
                break;
            case 8:
                displayPatientsToDoctor(line->myLine, docList);
                break;
            case 9:
                line->myLine = dischargePatient(line->myLine);
                break;
            case 10:
                removeVisit(tree->myTree);
                break;
            case 11:
                removePatient(tree,line);
                break;
            case 12:
                updateFiles(tree, line, &docList);
                freeAll(docList,tree,line);
                printf("Good bye :)!\n");
                return 0;
		}   
	}
}
