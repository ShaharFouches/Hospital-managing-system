#define _CRT_SECURE_NO_WARNINGS

#include "structs.h"
#include "functions.h"

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
