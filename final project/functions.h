#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NONE 0
#define PENICILLIN 1
#define SULFA 2
#define OPIOIDS 4
#define ANESTHETICS 8
#define EGGS 16
#define LATEX 32
#define PRESERVATIVES 64

int menu();
void flushStdin();
void displayError(char* error);
char sumAllergies(char* str);
char sumAllergiesFromNumbers(char* str);
void enterToTree(plnTree** currNode, Patient* tempP);
int isValidString(const char* str);
int extractNum(char* str);
Date* getDate(char* str);
float getDuration(char* str);
Stack* Push(Stack* visits, Visit* tempV);
Stack* pop(Stack* visits, Visit** tempV);
Stack* reverseStack(Stack* stack);
Doc* getDoc(ll* head, char* str);
pTree* loadPatients(ll** head, char* filePath);
void append(ll** curr, Doc** tempD);
plnLine* insertAtEnd(plnLine** curr, Patient** tempP);
Patient* searchPatient(plnTree* curr, char* id);
Doc* createDoc(char* str);
ll* loadDoctors(char* filePath);
pLine* loadLine(pTree* tree, char* filePath);
char* extractAller(char aller);
void writeDuration2File(float num,char* filePath);
void updatePatient(Patient* p, int* num);
void updatePatients(plnTree* curr, int* num);
int updateDoctors(ll* head);
int updateLine(plnLine* line);
void updateFiles(pTree* tree, pLine* line, ll** head);
void assignDoctor2case(ll* ll,Visit* p);
void editName(Patient* p);
int validAllerInput(char* str);
int checkDups(char* str);
int isFull(ll* head);
void editAllergies(Patient* p);
Patient* registerNewPatient(plnTree* root, char* id);
plnLine* admitPatient(plnTree* root, ll* head, plnLine* line);
int activeVisit(Stack* visits);
Visit* createNewVisit(ll* head);
char* getTime();
char* getId();
void checkForAllergies(plnLine* line);
void displayAllPatients(plnTree* root);
void displayPatientAdmissions(plnTree* root);
void printCurrVisit(Patient* tempP);
void displayPatientsInLine(plnLine* line, int extenedInfo);
plnLine* advancePatientInLine(plnLine* line);
void displayDoctors(ll* head);
void displayPatientsToDoctor(plnLine* line, ll* head);
plnLine* removeFromLine(plnLine* line, char* id);
Patient* searchInLine(plnLine* line, char* id);
float calcDuration(Date* arrival, Date* dismiss);
plnLine* dischargePatient(plnLine* line);
int isSingleDigit(int c);
Visit* getSpecificVisit(Patient* p);
void removeFromStack(Patient* p, Visit* v);
void removeVisit(plnTree* root);
plnTree* reArrangeTree(plnTree* left, plnTree* right);
plnTree* removeFromTree(plnTree* root, Patient* p);
void removePatient(pTree* root, pLine* line);
void freeDate(Date* date);
void freeVisit(Visit* visit);
void freeStack(Stack* stack);
void freePatient(Patient* patient);
void freePlnTree(plnTree* tree);
void freePTree(pTree* tree);
void freePlnLine(plnLine* line);
void freePLine(pLine* line);
void freeDoc(Doc* doc);
void freeLL(ll* list);
void freeAll(ll* docList, pTree* tree, pLine* line);
#endif 

