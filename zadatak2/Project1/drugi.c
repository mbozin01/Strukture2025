/*
2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define MAX 256

struct _person;
typedef struct _person* Position;
typedef struct _person{
	char name[MAX];
	char surname[MAX];
	int birth_year;
	Position next;
}Person;

int Menu();
Position CreatePerson();
Position findLast(Position head);
Position findPrev(Position head, Position current);
int InsertAfter(Position position, Position newPerson);


int AddToBeg(Position head);
int AddToEnd(Position head);
int PrintList(Position head);
Position FindBySurname(Position head, char* lastname);
int DeletePerson(Position head, char* lastname);
void FreeList(Position head);

int AddAfter(Position head, char* surname);
int AddBefore(Position head, char* surname);
int WriteInFile(Position head);
int ReadFromFile(Position head);
int ReadFromFile(Position head);
int SortList(Position head);

int main() {

	Menu();
	return 0;
}

int Menu()
{
	Person head = { .name = "", .surname = "", .birth_year = 0, .next = NULL };

	char choose = '\0';
	int exit = 1;

	char surname[MAX] = { " " };
	Position sname = NULL;

	while (exit)
	{
		printf("A. Add after specific person. \n");
		printf("B. Add before specific person. \n");
		printf("H. Add person at the beginning of the list. \n");
		printf("E. Add person at the end of the list. \n");
		printf("D. Delete person. \n");
		printf("F. Find person by surname. \n");
		printf("P. Print list. \n");
		printf("R. Read from file. \n");
		printf("W. Write list in file. \n");
		printf("S. Sort list. \n");
		printf("X. Exit Menu. \n");

		printf("Enter an upper letter: ");
		scanf(" %c", &choose);

		switch (choose)
		{

		case 'A':
			printf("\nEnter the last name after which the new person will be added: ");
			scanf(" %s", surname);
			AddAfter(&head, surname);
			break;
		case 'B':
			printf("\nEnter the last name before which the new person will be added: ");
			scanf(" %s", surname);
			AddAfter(&head, surname);
			break;
		case 'H':
			AddToBeg(&head);
			break;
		case 'E':
			AddToEnd(&head);
			break;
		case 'D':
			printf("\nEnter the last name of the person you want to delete: ");
			scanf(" %s", surname);
			DeletePerson(&head, surname);
			break;
		case 'F':
			printf("\nEnter the last name of the person you want to find: ");
			scanf(" %s", surname);
			sname = FindBySurname(&head, surname);
			if (sname != NULL)
				printf("Person found.\n");
			else
				printf("Person not found.\n");
			break;
		case 'P':
			PrintList(&head);
			break;
		case 'R':
			ReadFromFile(&head);
			break;
		case 'S':
			SortList(&head);
			break;
		case 'W':
			WriteInFile(&head);
			break;
		case 'X':
			exit = 0;
			break;

		default:
			printf("ERROR! Choose again: ");
			break;
		}

		printf("\n");
	}

	FreeList(&head);
	return 0;
}

Position CreatePerson() {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("Malloc failed\n");
		return NULL;
	}
	newPerson->next = NULL;
	printf("Enter name, surname and birth year: ");
	scanf(" %s %s %d", newPerson->name, newPerson->surname, &newPerson->birth_year);

	return newPerson;

}

Position findLast(Position head) {
	Position last = head;
	while (last->next != NULL) {
		last = last->next;
	}
	return last;
}

Position findPrev(Position head, Position current) {
	Position prev = head;
	while (prev != NULL && prev->next != current) {
		prev = prev->next;
	}

	return prev;
}

int InsertAfter(Position position, Position newPerson) {
	newPerson->next = position->next;
	position->next = newPerson;

	return 0;
}

int AddToBeg(Position head) {
	Position temp = head;
	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		return -1;
	}
	InsertAfter(temp, newPerson);

	return 0;
}

int AddToEnd(Position head) {
	Position last = findLast(head);

	Position newPerson = CreatePerson();
	if (newPerson == NULL) {
		return -1;
	}

	InsertAfter(last, newPerson);
	return 0;
}


int AddAfter(Position head, char* surname) {
	Position newPerson = CreatePerson();
	Position position = FindBySurname(head, surname);
	InsertAfter(position, newPerson);

	return 0;
}


int AddBefore(Position head, char* surname) {
	Position newPerson = CreatePerson();
	Position position = FindBySurname(head, surname);
	Position prev = findPrev(head, position);

	InsertAfter(prev, newPerson);

	return 0;
}

int PrintList(Position head) {
	Position current = head->next;
	printf("\n");
	while (current != NULL) {
		printf("Person:  %s %s %d\n", current->name, current->surname, current->birth_year);
		current = current->next;
	}

	return 0;
}

Position FindBySurname(Position head, char* lastname) {
	Position current = head->next;
	if (current == NULL) {
		return NULL;
	}

	while (current != NULL && strcmp(current->surname, lastname) != 0) {
		current = current->next;
	}

	return current;
}

int DeletePerson(Position head, char* lastname) {
	Position toDelete = FindBySurname(head, lastname);
	

	if (toDelete == NULL) {
		printf("Person not found!\n");
		return -1;
	}
	
	Position prev = findPrev(head, toDelete);
	prev->next = toDelete->next;
	free(toDelete);

	return 0;
}

int WriteInFile(Position head) {
	Position temp = head->next;

	FILE* f = NULL;
	f = fopen("persons.txt", "w");

	while (temp != NULL) {
		fprintf(f, "%s %s %d\n", temp->name, temp->surname, temp->birth_year);
		temp = temp->next;
	}

	fclose(f);
	printf("List written successfully.\n");

	return 0;
}


int ReadFromFile(Position head) {
	FILE* fp;
	fp = fopen("persons.txt", "r");
	while (!feof(fp)) {
		char name[MAX], surname[MAX];
		int birthYear;
		fscanf(fp, "%s %s %d\n", name, surname, &birthYear);
		printf("%s %s %d\n", name, surname, birthYear);;
	}
	return 0;
}

int SortList(Position head) {
	bool swapped;
	Position current;

	do {
		swapped = false;
		current = head;

		while (current->next && current->next->next) {
			if (strcmp(current->next->surname, current->next->next->surname) > 0) {
				Position temp = current->next;
				current->next = temp->next;
				temp->next = current->next->next;
				current->next->next = temp;
				swapped = true;
			}
			current = current->next;
		}
	} while (swapped);

	return 0;
}

void FreeList(Position head) {
	Position temp = NULL;

	while (head->next != NULL) {
		temp = head->next;
		head->next = temp->next;
		temp->next = NULL;
		free(temp);
	}
}

