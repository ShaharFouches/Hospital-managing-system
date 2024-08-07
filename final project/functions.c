#define _CRT_SECURE_NO_WARNINGS

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
	flushStdin();
	return choice;
}

void flushStdin() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void displayError(char* error) {
	fprintf(stderr, "%s", error);
}

char sumAllergies(char* str) {

	str = strtok(str, ",");
	char res = 0;
	char* allergies[] = { "none","Penicillin","Sulfa","Opioids",
			"Anesthetics","Eggs","Latex","Preservatives" };
	char aller_vals[] = { NONE,PENICILLIN,SULFA,OPIOIDS,ANESTHETICS,EGGS,LATEX,PRESERVATIVES };
	while (str != NULL) {
		for (int i = 0; i < 8; i++) {
			if (strcmp(str, allergies[i]) == 0) {
				res += aller_vals[i];
				break;
			}
		}
		str = strtok(NULL, ",");
	}
	return res;
}

char sumAllergiesFromNumbers(char* str) {
	str = strtok(str, ",");
	char res = 0;
	char aller_vals[] = { NONE,PENICILLIN,SULFA,OPIOIDS,ANESTHETICS,EGGS,LATEX,PRESERVATIVES };

	while (str != NULL) {
		int index = str[0] - '0';

		if (index >= 0 && index <= 7) {
			res += aller_vals[index];
		}

		str = strtok(NULL, ",");
	}

	return res;
}

int isValidString(const char* str) {
	for (size_t i = 0; i < strlen(str); i++) {
		char c = str[i];
		if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
			return 0;
		}
	}
	return 1;
}

void enterToTree(plnTree** currNode, Patient* tempP) {
	if (*currNode == NULL) {
		(*currNode) = (plnTree*)malloc(sizeof(plnTree));
		if (*currNode==NULL)
		{
			printf("allocation failed\n");
			return;
		}
		(*currNode)->tpatient = tempP;
		(*currNode)->left = NULL;
		(*currNode)->right = NULL;
		return;
	}
	if (strcmp((*currNode)->tpatient->ID, tempP->ID) < 0)
		enterToTree(&((*currNode)->right), tempP);
	else
		enterToTree(&((*currNode)->left), tempP);

}

int extractNum(char* str) {
	int res = 0;
	int len = 0;
	for (char* temp = str; *temp; temp++)
		len++;

	for (; *str; str++) {
		res += *str - 48;
		if (len > 1) {
			res *= 10;
			len--;
		}
	}
	return res;
}

Date* getDate(char* str) {
	Date* tempD = (Date*)malloc(sizeof(Date));
	if (tempD == NULL)
	{
		printf("allocation failed\n");
		return NULL;
	}
	str = strtok(str, "/");
	tempD->Day = extractNum(str);
	str = strtok(NULL, "/");
	tempD->Month = extractNum(str);
	str = strtok(NULL, " ");
	tempD->Year = extractNum(str);
	str = strtok(NULL, ":");
	tempD->Hour = extractNum(str);
	str = strtok(NULL, "\n");
	tempD->Min = extractNum(str);
	return tempD;
}

float getDuration(char* str) {
	float res = 0;
	str = strtok(str, ":");
	res += extractNum(str) * 100;
	str = strtok(NULL, "\r");
	res += extractNum(str);
	res /= 100;
	return res;
}

Stack* Push(Stack* visits, Visit* tempV) {
	Stack* newVisit = (Stack*)malloc(sizeof(Stack));
	if (newVisit == NULL) {
		printf("allocation failed\n");
		return visits;
	}
	newVisit->visit = tempV;
	newVisit->next = visits;
	return newVisit;
}

Stack* pop(Stack* visits, Visit** tempV) {
	if (visits == NULL) {
		*tempV = NULL;
		return NULL;
	}
	Stack* temp = visits;
	*tempV = visits->visit;
	visits = visits->next;
	free(temp);
	return visits;
}

Stack* reverseStack(Stack* stack){
	Stack* reversed = NULL;
	Stack* current = stack;
	while (current != NULL) {
		reversed = Push(reversed, current->visit);
		current = current->next;
	}
	return reversed;
}

Doc* getDoc(ll* head, char* str) {
	if (head == NULL)
		return NULL;
	if (strcmp(head->doc->Name, str) == 0)
		return head->doc;
	else return getDoc(head->next, str);

}

pTree* loadPatients(ll** head, char* filePath) {
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}
	
	plnTree* root = NULL;
	Patient* tempP = NULL;
	Visit* tempV = NULL;
	pTree* tempT = NULL;
	tempT = (pTree*)malloc(sizeof(pTree));
	if (tempT==NULL)
	{
		printf("allocation failed\n");
		return NULL;
	}
	char buffer[256];
	fgets(buffer, sizeof(buffer), file);
	while (fgets(buffer, sizeof(buffer), file)) {
		if (strstr(buffer, "========================") != NULL) {
			if (tempP != NULL) {
				enterToTree(&root, tempP);
				tempV = NULL;
			}
		}
		if (strstr(buffer, ";") != NULL) {
			tempP = (Patient*)malloc(sizeof(Patient));
			if (tempP == NULL)
			{
				printf("allocation failed\n");
				return NULL;
			}
			char* token = strtok(buffer, ";");
			tempP->Name = _strdup(token + 2);
			token = strtok(NULL, ";");
			tempP->ID = _strdup(token);
			token = strtok(NULL, "\n");
			tempP->Allergies = sumAllergies(token);
			tempP->Visits = NULL;
			tempP->nVisits = 0;
		}
		if (strstr(buffer, "Arrival") != NULL) {
			tempP->nVisits += 1;
			tempV = (Visit*)malloc(sizeof(Visit));
			if (tempV == NULL)
			{
				printf("allocation failed\n");
				return NULL;
			}
			char* token = strtok(buffer, ":");
			token = strtok(NULL, "\n");
			tempV->tArrival = getDate(token);
		}
		if (strstr(buffer, "Dismissed") != NULL) {
			char* token = strtok(buffer, ":");
			token = strtok(NULL, "\n");
			if (token != NULL)
				tempV->tDismissed = getDate(token);
			else
				tempV->tDismissed = NULL;
		}
		if (strstr(buffer, "Duration") != NULL) {
			char* token = strtok(buffer, ":");
			token = strtok(NULL, "\n");
			if (token != NULL && strcmp(token, "-1:00") != 0)
				tempV->Duration = getDuration(token);
			else
				tempV->Duration = -1;
		}
		if (strstr(buffer, "Doctor") != NULL) {
			char* token = strtok(buffer, ":");
			token = strtok(NULL, "\n");
			tempV->Doctor = getDoc(*head, token);
		}
		if (strstr(buffer, "Summary") != NULL) {
			char* token = strtok(buffer, ":");
			token = strtok(NULL, "\n");
			if (token != NULL)
				tempV->vSummary = _strdup(token);
			else
				tempV->vSummary = "";
			tempP->Visits = Push(tempP->Visits, tempV);
		}
	}
	tempT->myTree = root;
	fclose(file);
	return tempT;
}

void append(ll** curr, Doc** tempD) {
	if (*curr == NULL) {
		*curr = (ll*)malloc(sizeof(ll));
		if (*curr == NULL)
		{
			printf("memory allocation failed.\n");
			return;
		}
		(*curr)->doc = *tempD;
		(*curr)->next = NULL;
		return;
	}
	else
		append(&(*curr)->next, tempD);
}

plnLine* insertAtEnd(plnLine** curr, Patient** tempP) {
	if (*curr == NULL) {
		*curr = (plnLine*)malloc(sizeof(plnLine));
		if (*curr==NULL)
		{
			printf("allocation failed");
			return NULL;
		}
		(*curr)->lpatient = *tempP;
		(*curr)->next = NULL;
		return *curr;
	}
	else {
		(*curr)->next = insertAtEnd(&((*curr)->next), tempP);
		return *curr;
	}

}

Patient* searchPatient(plnTree* curr, char* id) {
	if (curr == NULL)
		return NULL;
	char* currID = curr->tpatient->ID;
	if (strcmp(currID, id) == 0)
		return curr->tpatient;
	if (strcmp(currID, id) > 0)
		return searchPatient(curr->left, id);
	else
		return searchPatient(curr->right, id);
}

Doc* createDoc(char* str) {
	Doc* tempD = (Doc*)malloc(sizeof(Doc));
	if (tempD == NULL)
	{
		printf("allocation failed\n");
		return NULL;
	}
	str = strtok(str, ";");
	tempD->Name = _strdup(str);
	str = strtok(NULL, ";");
	tempD->nLicense = _strdup(str + 1);
	str = strtok(NULL, "\n");
	tempD->nPatients = extractNum(str + 1);
	return tempD;
}

ll* loadDoctors(char* filePath) {
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}

	Doc* tempD;
	ll* head = NULL;

	char buffer[256];
	fgets(buffer, sizeof(buffer), file);
	fgets(buffer, sizeof(buffer), file);
	while (fgets(buffer, sizeof(buffer), file)) {
		char* token = strtok(buffer, "\n");
		tempD = createDoc(token);
		append(&head, &tempD);
	}
	return head;
}

pLine* loadLine(pTree* tree, char* filePath) {
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}
	pLine* temp=NULL;
	temp = (pLine*)malloc(sizeof(pLine));
	if (temp == NULL)
	{
		printf("allocation failed");
		return NULL;
	}
	plnLine* line = NULL;


	char buffer[256];
	fgets(buffer, sizeof(buffer), file);
	fgets(buffer, sizeof(buffer), file);
	while (fgets(buffer, sizeof(buffer), file)) {
		char* token = strtok(buffer, "\r\n");
		Patient* tempP = searchPatient(tree->myTree, token + 2);
		if (tempP == NULL) {
			printf("No patient with this id:%s is registered", token);
			return NULL;
		}
		else
			line = insertAtEnd(&line, &tempP);
	}
	temp->myLine = line;
	return temp;
}

char* extractAller(char aller) {
	char* res = (char*)malloc(74);
	if (res == NULL)
	{
		printf("allocation failed\n");
		return NULL;
	}
	strcpy(res, "");

	char* allergies[] = {
		"none", "Penicillin", "Sulfa", "Opioids",
		"Anesthetics", "Eggs", "Latex", "Preservatives"
	};

	for (int i = 1, j = 1; i < 8; i++, j *= 2) {
		if (aller & j) {
			if (strcmp(res, "") != 0) {
				strcat(res, ",");
			}
			strcat(res, allergies[i]);
		}
	}
	if (strcmp(res, "") == 0)
		res = _strdup("none");
	strcat(res, "\n");
	return res;
}

void writeDuration2File(float num, char* filePath) {
	FILE* file = fopen(filePath,"a");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	int hour = (int)num;
	int min = (int)((num - hour) * 100);
	fprintf(file, "%d:%02d\n", hour, min);
	fflush(file);
	fclose(file);
}

void updatePatient(Patient* p, int* num) {
	FILE* file = fopen("Patients.txt", "a");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	fprintf(file, "%d.%s;%s;%s", *num, p->Name, p->ID, extractAller(p->Allergies));
	fflush(file);

	(*num)++;
	Visit* tempV = NULL;
	Stack* reversed = reverseStack(p->Visits);
	for (int i = 0; i < p->nVisits; i++) {
		fprintf(file, "\n");
		reversed = pop(reversed,&tempV);
		Date* arrival = tempV->tArrival;
		Date* dismiss = tempV->tDismissed;
		Doc* doc = tempV->Doctor;
		fprintf(file, "Arrival:%02d/%02d/%02d ", arrival->Day, arrival->Month, arrival->Year);
		fprintf(file, "%02d:%02d\n", arrival->Hour, arrival->Min);
		fflush(file);

		if (dismiss != NULL) {
			fprintf(file, "Dismissed:%02d/%02d/%02d ", dismiss->Day, dismiss->Month, dismiss->Year);
			fprintf(file, "%02d:%02d\n", dismiss->Hour, dismiss->Min);
			fflush(file);
		}
		else
			fprintf(file,"Dismissed:\n");
		fprintf(file, "Duration:");
		fflush(file);
		if (tempV->Duration != -1)
			writeDuration2File(tempV->Duration, "Patients.txt");
		else
			fprintf(file, "\n");
		fprintf(file, "Doctor:%s\n", doc->Name);
		fprintf(file, "Summary:%s\n", tempV->vSummary);
		fflush(file);
	}

	fprintf(file, "========================\n");
	fflush(file);
	fclose(file);
}

void updatePatients(plnTree* curr, int* num) {
	if (curr == NULL)
		return;
	updatePatient(curr->tpatient, num);
	updatePatients(curr->left, num);
	updatePatients(curr->right, num);
}

int updateDoctors(ll* head) {
	FILE* file = fopen("Doctors.txt", "a");
	if (file == NULL) {
		perror("Error opening file");
		return ;
	}
	for (ll* curr = head; curr; curr = curr->next) {
		fprintf(file, "%s; %s; %d\n", curr->doc->Name, curr->doc->nLicense, curr->doc->nPatients);
		fflush(file);
	}
	return 0;
	fclose(file);
}

int updateLine(plnLine* line) {
	FILE* file = fopen("Line.txt", "a");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}
	int i = 1;
	for (plnLine* curr = line; curr; curr = curr->next) {
		fprintf(file, "%d.%s\n", i++, curr->lpatient->ID);
		fflush(file);
	}
	return 0;
	fclose(file);
}

void updateFiles(pTree* tree, pLine* line, ll** head) {
	FILE* file1 = fopen("Patients.txt", "w");
	if (file1 == NULL) {
		perror("Error opening file");
		return;
	}
	fprintf(file1, "Name; ID; Allergies\n");
	fflush(file1);
	fprintf(file1, "========================\n");
	fflush(file1);
	fclose(file1);


	FILE* file2 = fopen("Doctors.txt", "w");
	if (file2 == NULL) {
		perror("Error opening file");
		return;
	}
	fprintf(file2, "Full Name; License Number; Number of Patients\n");
	fprintf(file2, "=================================================\n");
	fflush(file2);
	fclose(file2);


	FILE* file3 = fopen("Line.txt", "w");
	if (file3 == NULL) {
		perror("Error opening file");
		return;
	}
	fprintf(file3, "Patients' IDs in line\n");
	fprintf(file3, "=====================\n");
	fflush(file3);
	fclose(file3);
	
	int* num = { 1 };

	if (updateDoctors(*head) == 0)
		printf("Doctors.txt has been updated successfully\n");

	if (line != NULL) {
		if (updateLine(line->myLine) == 0)
			printf("Line.txt has been updated successfully\n");
	}

	updatePatients(tree->myTree, &num);
	printf("Patient.txt has been updated successfully\n");



}

char* getTime() {
	time_t now = time(NULL);
	if (now == (time_t)-1) {
		perror("Error getting the current time");
		return NULL;
	}

	struct tm* local_time = localtime(&now);
	if (local_time == NULL) {
		perror("Error converting to local time");
		return NULL;
	}

	char* time_str = malloc(100);
	if (time_str == NULL) {
		perror("Memory allocation failed");
		return NULL;
	}

	if (strftime(time_str, 100, "%d/%m/%Y %H:%M\n", local_time) == 0) {
		fprintf(stderr, "Error formatting time\n");
		free(time_str);
		return NULL;
	}

	return time_str;
}

char* getId() {
	char* id = malloc(10);
	if (id == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	
	while (1) {
		int whileBreak = 0;
		printf("Please enter ID number: ");
		if (scanf("%9s", id) != 1) {
			printf("Invalid id number\n");
			continue;
		}
		size_t len = strlen(id);
		if (len != 9) {
			printf("ID must be exactly 9 characters long\n");
			continue;
		}
		for (int i = 0; i < 9; i++)
		{
			if (id[i]>'9' || id[i]<'0')
			{
				printf("invalid id, please enter only numbers\n");
				whileBreak = 1;
				break;
			}
		}
		if (whileBreak==0)
		{
			break;
		}	
	}
	flushStdin();
	return id;
}

void assignDoctor2case(ll* head,Visit* v) {
	Doc* leastBusy = head->doc;
	for (ll* temp = head->next;temp;temp=temp->next) {
		if (temp->doc->nPatients < leastBusy->nPatients)
			leastBusy = temp->doc;
	}
	leastBusy->nPatients++;
	v->Doctor = leastBusy;
}

int isFull(ll* head) {
	for (ll* temp = head;temp;temp = temp->next)
		if (temp->doc->nPatients < 4)
			return 1;
	return 0;
}

void editName(Patient* p) {
	char name[35]; 
	printf("Please enter patient's name: ");

	if (scanf(" %[^\n]", name) != 1) {
		printf("Input error. Please try again.\n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}
		editName(p);
		return;
	}

	if (!isValidString(name)) {
		printf("The name entered must contain only characters and spaces\n");
		editName(p);
		return;
	}

	p->Name = malloc(strlen(name) + 1);
	if (p->Name == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}

	strcpy(p->Name, name);
}

int validAllerInput(char* str) {
	for (char* i = str + 1; *i != '\0'; i += 2)
		if (*i != ',') {
			displayError("Invalid format please try again\n");
			return 0;
		}
	return 1;
}

int checkDups(char* str) {
	int seen[8] = { 0 };

	char tempStr[16];
	strcpy(tempStr, str);

	char* token = strtok(tempStr, ",");
	while (token != NULL) {
		int index = token[0] - '0';

		if (index < 0 || index >= 8) {
			printf("Invalid number in input string: %d\n", index);
			return 1;
		}

		if (seen[index]) {
			displayError("There is a duplicate in the format");
			return 1;
		}
		seen[index] = 1;
		token = strtok(NULL, ",");
	}
	return 0;
}

void editAllergies(Patient* p) {
	char* allergies[] = { "none","Penicillin","Sulfa","Opioids",
			"Anesthetics","Eggs","Latex","Preservatives" };

	printf("Out of this list please select all allergies you know of\n");
	printf("Enter numbers from 0 to 7 in this format 0,1,2... \n");
	int aller_size = sizeof(allergies) / sizeof(allergies[0]);
	char aller[16];
	for (int i = 0; i < aller_size; i++)
		printf("%d - %s\n",i, allergies[i]);

	if (scanf("%s", aller) != 1 || !validAllerInput(aller) || checkDups(aller))
		editAllergies(p);
	
	p->Allergies = sumAllergiesFromNumbers(aller);
}

Patient* registerNewPatient(plnTree* root, char* id) {
	Patient* tempP = (Patient*)malloc(sizeof(Patient));
	if (tempP == NULL)
	{
		printf("Memory allocation failed.");
		return NULL;
	}
	tempP->ID = id;
	editName(tempP);
	editAllergies(tempP);
	tempP->Visits = NULL;
	tempP->nVisits = 0;
	enterToTree(&root,tempP);
	return tempP;
}

int activeVisit(Stack* visits) {
	for (Stack* tempS = visits;tempS;tempS = tempS->next)
		if (tempS->visit->tDismissed == NULL)
			return 1;
	return 0;
}

Visit* createNewVisit(ll* head) {
	Visit* tempV = (Visit*)malloc(sizeof(Visit));
	if (tempV== NULL)
	{
		printf("Memory allocation failed.");
		return NULL;
	}
	Date* tempD = getDate(getTime());
	tempV->tArrival = tempD;
	tempV->tDismissed = NULL;
	tempV->Duration = 0;
	tempV->vSummary = "";
	assignDoctor2case(head,tempV);
	return tempV;
}

plnLine* admitPatient(plnTree* root , ll* head, plnLine* line) {
	if (!isFull) {
		printf("There is no space for new patients\n");
		return NULL;
	}
	char* id = getId();
	Patient* tempP = searchPatient(root,id);
	if (tempP == NULL)
		tempP = registerNewPatient(root, id);

	if (activeVisit(tempP->Visits)) {
		printf("There is currently and active visit\n");
		return NULL;
	}
	
	Visit* tempV = createNewVisit(head);
	tempP->Visits = Push(tempP->Visits ,tempV);
	tempP->nVisits++;
	printf("Patient admited successfuly\n\n");
	return insertAtEnd(&line, &tempP);

}

void checkForAllergies(plnLine* line) {
	if (line == NULL) {
		printf("The are no patients in line\n");
		return;
	}
	for (plnLine* tempL = line; tempL; tempL = tempL->next) {
		printf("%s\n",tempL->lpatient->ID);
	}
	printf("\n");
	Patient* tempP = NULL;

	while (tempP == NULL) {
		char* id = getId();

		for (plnLine* tempL = line; tempL; tempL = tempL->next)
			if (strcmp(tempL->lpatient->ID, id) == 0) {
				tempP = tempL->lpatient;
				break;
			}
		if (tempP == NULL) 
			printf("No such patient in line\n");
		
	}


	printf("%s\n",extractAller(tempP->Allergies));
	return;
}

void displayAllPatients(plnTree* root) {
	if (root == NULL)
		return;
	printf("Patient's name: %s\n",root->tpatient->Name);
	printf("Patient's ID: %s\n", root->tpatient->ID);
	printf("-------------------------------------------\n");
	displayAllPatients(root->left);
	displayAllPatients(root->right);
	return;
}

void displayPatientAdmissions(plnTree* root) {
	printf("\n");
	displayAllPatients(root);
	printf("\n");
	Patient* tempP = NULL;
	
	while (tempP == NULL) {
		char* id = getId();
		tempP = searchPatient(root, id);
		if (tempP == NULL) 
			printf("No patient is registered under this ID\n");
	}

	if (tempP->Visits == NULL) {
		printf("There is no admissions for this patient\n");
		return;
	}
	for (Stack* tempS = tempP->Visits;tempS;tempS = tempS->next) {
		Visit* tempV = tempS->visit;
		Date* arrival = tempV->tArrival;
		Date* dismiss = tempV->tDismissed;
		Doc* doc = tempV->Doctor;
		printf("Arrival:%02d/%02d/%02d ", arrival->Day, arrival->Month, arrival->Year);
		printf("%02d:%02d\n", arrival->Hour, arrival->Min);
		if (dismiss != NULL) {
			printf("Dismissed:%02d/%02d/%02d ", dismiss->Day, dismiss->Month, dismiss->Year);
			printf("%02d:%02d\n", dismiss->Hour, dismiss->Min);
		}
		else
			printf("Dismissed:\n");
		printf("Duration:");
		printf("%.2f\n",tempV->Duration);
		printf("Doctor:%s\n", doc->Name);
		printf("Summary:%s\n\n", tempV->vSummary);
	}
}

void printCurrVisit(Patient* tempP) {
	Visit* lastV = tempP->Visits->visit;
	if (lastV->tDismissed != NULL)
		return;
	Date* arrival = lastV->tArrival;
	Doc* doc = lastV->Doctor;
	printf("Arrival:%02d/%02d/%02d ", arrival->Day, arrival->Month, arrival->Year);
	printf("%02d:%02d\n", arrival->Hour, arrival->Min);
	printf("Doctor:%s\n", doc->Name);

}

void displayPatientsInLine(plnLine* line,int extenedInfo) {
	if (line == NULL)
		return;
	printf("======================================\n");
	for (;line;line = line->next) {
		Patient* tempP = line->lpatient;
		printf("Patient's name: %s\n",tempP->Name);
		printf("Patient's ID: %s\n", tempP->ID);
		if (extenedInfo)
			printCurrVisit(tempP);
		printf("======================================\n");
	}
}

plnLine* advancePatientInLine(plnLine* line) {
	printf("Current patients in line:\n");
	displayPatientsInLine(line,0);
	printf("Please enter the patient's ID you want to advance in line\n");
	char* id = getId();
	int found = 0;
	Patient* tempP = NULL;
	
	if (strcmp(line->lpatient->ID, id) == 0 ) {
		printf("Patient is already in the front of the line\n");
		return line;
	}
	else
		for (plnLine* tempL = line; tempL->next; tempL = tempL->next) {
			plnLine* next = tempL->next;
			if (strcmp(next->lpatient->ID, id) == 0) {
				tempP = next->lpatient;
				tempL->next = next->next;
				next->next = line;
				line = next;
				found = 1;
				break;
			}
		}

	if (found == 0) {
		displayError("No such patient in line please try again\n\n");
		return advancePatientInLine(line);
	}
	printf("The updated line is:\n");
	displayPatientsInLine(line,0);
	return line;
}

void displayDoctors(ll* head) {
	for (;head; head = head->next) {
		printf("-----------------------------------------------\n");
		printf("Doctor: %s has %d patients\n", head->doc->Name, head->doc->nPatients);
	}
	printf("-----------------------------------------------\n");

}

void displayPatientsToDoctor(plnLine* line, ll* head) {
	displayDoctors(head);
	char docName1[26];
	char docName2[26];
	printf("Please enter the doctor's name: ");
	while (1) {
		scanf("%s %s", docName1, docName2);
		if (!isValidString(docName1) || !isValidString(docName2)) {
			printf("Doctor's name is invalid\nPlease enter a valid name: ");
			continue;
		}
		break;
	}
	char* docName = strcat(docName1, " ");
	docName = strcat(docName, docName2);
	if (getDoc(head, docName) == NULL) {
		displayError("No such doctor please try again\n\n");
		displayPatientsToDoctor(line, head);
	}
	printf("\n%s patient's information:\n^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^\n\n", docName);
	for (; line; line = line->next) {
		Patient* tempP = line->lpatient;
		Visit* currV = tempP->Visits->visit;
		if (currV == NULL || currV->tDismissed != NULL)
			continue;
		if (strcmp(docName, currV->Doctor->Name) == 0) {
			printf("Patient's name: %s\n", tempP->Name);
			printf("Patient's ID: %s\n", tempP->ID);
			printf("Patient's arrival time: %02d/%02d/%02d ", currV->tArrival->Day, currV->tArrival->Month, currV->tArrival->Year);
			printf("%02d:%02d\n\n", currV->tArrival->Hour, currV->tArrival->Min);
		}
	}
	printf("^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^\n\n");
}

plnLine* removeFromLine(plnLine* line, char* id) {
	if (line == NULL)
		return NULL;
	if (strcmp(line->lpatient->ID, id) == 0)
		return line->next;
	for (plnLine* tempL = line;tempL->next;tempL = tempL->next) {
		plnLine* next = tempL->next;
		if (strcmp(next->lpatient->ID, id) == 0) {
			tempL->next = next->next;
			break;
		}
	}
	return line;
}

Patient* searchInLine(plnLine* line, char* id) {
	for (;line;line = line->next) 
		if (strcmp(line->lpatient->ID, id) == 0)
			return line->lpatient;	
	return NULL;
}

float calcDuration(Date* arrival, Date* dismiss) {
	float res = 0;
	int days1 = 0, days2 = 0;
	days1 = 365 * arrival->Year + 30 * arrival->Month + arrival->Day;
	days2 = 365 * dismiss->Year + 30 * dismiss->Month + dismiss->Day;
	res = (days2 - days1)*24 + dismiss->Hour - arrival->Hour;
	res += (dismiss->Min - arrival->Min) * 0.01;
	return res;
}

plnLine* dischargePatient(plnLine* line) {
	displayPatientsInLine(line,0);
	printf("\n");
	char* id = getId();
	Patient* tempP;
	if ((tempP = searchInLine(line, id)) == NULL) {
		printf("There is no such patient in line\n");
		return line;
	}

	Visit* tempV = tempP->Visits->visit;
	if (tempV == NULL) {
		printf("The is no visits for this patient\n");
		return line;
	}

	tempV->tDismissed = getDate(getTime());
	tempV->Duration = calcDuration(tempV->tArrival,tempV->tDismissed);

	Doc* tempD = tempV->Doctor;
	tempD->nPatients--;

	printf("Line after discharging %s\n",id);
	line = removeFromLine(line,id);
	displayPatientsInLine(line,0);
	printf("\n");


	return line;
}

int isSingleDigit(char* c) {
	if (*(c + 1) == '\0')
		return 1;
	else
		return 0;
}

Visit* getSpecificVisit(Patient* p) {
	printf("Please select a date for a visit you want to remove from the list below:\n");
	int i = 1;
	for (Stack* tempS = p->Visits;tempS;tempS = tempS->next,i++) {
		Date* tempD = tempS->visit->tArrival;
		printf("%d - %02d/%02d/%02d\n",i,tempD->Day,tempD->Month,tempD->Year);
	}
	printf("\nUser's choise: ");

	char input[18];
	int choice;
	while (1) {
		if (scanf("%s", &input) != 1 || !isSingleDigit(input)) {
			printf("Invalid input\nPlease enter a number:");
			flushStdin();
			continue;
		}
		choice = *input - 48;
		if (choice >= i || choice <= 0) {
			printf("Choice out of bounds please choose again:");
			continue;
		}
		break;
	}

	Stack* tempS = p->Visits;
	i--;
	for (;i > choice;tempS = tempS->next, i--);
	return tempS->visit;
}

void removeFromStack(Patient* p, Visit* v) {
	if (p->Visits->visit == v) {
		p->Visits = p->Visits->next;
		p->nVisits--;
		return;
	}
	for (Stack* tempS = p->Visits; tempS->next; tempS = tempS->next) 
		if (tempS->next->visit == v) {
			tempS->next = tempS->next->next;
			p->nVisits--;
			return;
		}
}

void removeVisit(plnTree* root) {
	printf("\n");
	displayAllPatients(root);
	printf("\n");

	Patient* tempP = NULL;
	char* id;

	while (tempP == NULL) {
		id = getId();
		tempP = searchPatient(root, id);
		if (tempP == NULL) 
			displayError("There is no such patient in records\n");
	}
	
	if (tempP->nVisits == 0) {
		printf("The patients have no visits to remove\n\n");
		return;
	}
	Visit* visit = getSpecificVisit(tempP);
	removeFromStack(tempP,visit);

	printf("\nThe remaining visits on record for %s is : %d\n",id,tempP->nVisits);
	for (Stack* tempS = tempP->Visits;tempS;tempS = tempS->next) {
		Visit* tempV = tempS->visit;
		Date* arrival = tempV->tArrival;
		Date* dismiss = tempV->tDismissed;
		Doc* doc = tempV->Doctor;
		printf("Arrival:%02d/%02d/%02d ", arrival->Day, arrival->Month, arrival->Year);
		printf("%02d:%02d\n", arrival->Hour, arrival->Min);
		if (dismiss != NULL) {
			printf("Dismissed:%02d/%02d/%02d ", dismiss->Day, dismiss->Month, dismiss->Year);
			printf("%02d:%02d\n", dismiss->Hour, dismiss->Min);
		}
		else
			printf("Dismissed:\n");
		printf("Duration:");
		printf("%.2f\n", tempV->Duration);
		printf("Doctor:%s\n", doc->Name);
		printf("Summary:%s\n\n", tempV->vSummary);
	}
}

plnTree* reArrangeTree(plnTree* left,plnTree* right) {
	if (left == NULL && right == NULL)
		return NULL;
	if (left == NULL && right != NULL)
		return right;
	if (left != NULL && right == NULL)
		return left;

	plnTree* newT;
	newT = right;
	newT->right = reArrangeTree(right->left, right->right);
	newT->left = left;
	return newT;

}

plnTree* removeFromTree(plnTree* root,Patient* p) {
	if (root == NULL) {
		return NULL;
	}
	if (root->tpatient == p)
		return reArrangeTree(root->left,root->right);
	if (strcmp(root->tpatient->ID, p->ID) < 0)
		root->right = removeFromTree(root->right, p);
	else
		root->left = removeFromTree(root->left, p);
	return root;
}

void removePatient(pTree* tree, pLine* line) {
	printf("\n");
	displayAllPatients(tree->myTree);
	printf("\n");
	char* id = getId();
	Patient* tempP = searchPatient(tree->myTree,id);
	if (tempP == NULL) {
		printf("There is no such patient in records\n");
		return;
	}

	if (activeVisit(tempP->Visits)) {
		Doc* tempD = tempP->Visits->visit->Doctor;
		tempD->nPatients--;
	}
	line->myLine = removeFromLine(line->myLine,id);
	tree->myTree = removeFromTree(tree->myTree,tempP);
	printf("%s has been removed succsesfully\n",id);
	
}

void freeDate(Date* date) {
	if (date != NULL) {
		free(date);
	}
}

void freeVisit(Visit* visit) {
	if (visit != NULL) {
		freeDate(visit->tArrival);
		if(visit->tDismissed != NULL)
			freeDate(visit->tDismissed);
		if(visit->vSummary != "")
			free(visit->vSummary);
		free(visit);
	}
}

void freeStack(Stack* stack) {
	while (stack != NULL) {
		Stack* temp = stack;
		stack = stack->next;
		freeVisit(temp->visit);
		free(temp);
	}
}

void freePatient(Patient* patient) {
	if (patient != NULL) {
		free(patient->Name);
		free(patient->ID);
		freeStack(patient->Visits);
		free(patient);
	}
}

void freePlnTree(plnTree* tree) {
	if (tree != NULL) {
		freePatient(tree->tpatient);
		freePlnTree(tree->left);
		freePlnTree(tree->right);
		free(tree);
	}
}

void freePTree(pTree* tree) {
	if (tree != NULL) {
		freePlnTree(tree->myTree);
		free(tree);
	}
}

void freePlnLine(plnLine* line) {
	while (line != NULL) {
		plnLine* temp = line;
		line = line->next;
		free(temp);
	}
}

void freePLine(pLine* line) {
	if (line != NULL) {
		freePlnLine(line->myLine);
		free(line);
	}
}

void freeDoc(Doc* doc) {
	if (doc != NULL) {
		free(doc->Name);
		free(doc->nLicense);
		free(doc);
	}
}

void freeLL(ll* list) {
	while (list != NULL) {
		ll* temp = list;
		list = list->next;
		freeDoc(temp->doc);
		free(temp);
	}
}

void freeAll(ll* docList, pTree* tree, pLine* line) {
	freePTree(tree);
	freeLL(docList);
	freePLine(line);
}

