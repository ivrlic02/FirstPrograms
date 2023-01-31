#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIRECTORY_NAME (256)

#define ERROR_ALLOCATION (-1)

struct _directory;
typedef struct _directory* DirectoryP;
typedef struct _directory {
	char name[MAX_DIRECTORY_NAME];
	DirectoryP child;
	DirectoryP sibling;
}Directory;

struct _stack;
typedef struct _stack* StackP;
typedef struct _stack {
	DirectoryP data;
	StackP next;
}Stack;

DirectoryP AllocateMemmoryForDirectory();
StackP AllocateMemmoryForStack();
int InsertAfter(StackP, StackP);

int push(DirectoryP, StackP);
DirectoryP pop(StackP);

int Path(StackP);
int Cmd(DirectoryP, StackP);

DirectoryP MakeDirectory(DirectoryP, char*);
DirectoryP ChangeDirectory(DirectoryP, StackP, char*);
DirectoryP PreviousDirectory(DirectoryP, StackP);
int Print(DirectoryP);
int DeleteAll(DirectoryP);

int main(int agrc, char** argv) {
	Directory Root;
	Root.child = NULL;
	Root.sibling = NULL;
	strcpy(Root.name, "C:");

	Stack Head;
	Head.data = NULL;
	Head.next = NULL;

	push(&Root, &Head);
	Cmd(&Root, &Head);

	return 0;
}

DirectoryP AllocateMemmoryForDirectory() {
	DirectoryP new = (DirectoryP)malloc(sizeof(Directory));

	if (new == NULL)
		return ERROR_ALLOCATION;

	new->child = NULL;
	new->sibling = NULL;

	return new;
}

StackP AllocateMemmoryForStack() {
	StackP new = (StackP)malloc(sizeof(Stack));

	if (new == NULL)
		return ERROR_ALLOCATION;

	return new;
}

int InsertAfter(StackP p, StackP q) {
	q->next = p->next;
	p->next = q;

	return 0;
}

int push(DirectoryP current, StackP head) {
	StackP new;
	new = AllocateMemmoryForStack();

	InsertAfter(head, new);
	new->data = current;

	return 0;
}

DirectoryP pop(StackP head) {
	if (head->next == NULL)
		return EXIT_FAILURE;

	StackP temp = head->next;
	StackP position = temp->data;
	head->next = temp->next;
	free(temp);

	return position;
}

int Path(StackP head) {
	char path[MAX_DIRECTORY_NAME] = "";

	if (!head->next) {
		strcat(path, head->data->name);
		strcat(path, ">>");
	}
	else {
		Path(head->next);
		strcat(path, head->data->name);
		strcat(path, ">>");
	}

	printf("%s", path);

	return 0;
}

int Cmd(DirectoryP current, StackP head) {
	char UserInput[MAX_DIRECTORY_NAME];
	char DirectoryName[MAX_DIRECTORY_NAME];
	char command[5];

	do {
		Path(head->next);
		fgets(UserInput, MAX_DIRECTORY_NAME, stdin);
		sscanf(UserInput, "%s %s", command, DirectoryName);

		if (strcmp(command, "md") == NULL)
			current = MakeDirectory(current, DirectoryName);

		else if (strcmp(command, "cd") == NULL)
			current = ChangeDirectory(current, head, DirectoryName);

		else if (strcmp(command, "cd..") == NULL)
			current = PreviousDirectory(current, head);

		else if (strcmp(command, "dir") == NULL)
			Print(current);

		else if (strcmp(command, "exit") == NULL)
			DeleteAll(current);

		else
			printf("Invalid input!\n");

	} while (strcmp(command, "exit"));

	return 0;
}

DirectoryP MakeDirectory(DirectoryP current, char* newName) {
	DirectoryP NewDirectory;
	NewDirectory = AllocateMemmoryForDirectory();

	strcpy(NewDirectory->name, newName);

	if (current->child == NULL)
		current->child = NewDirectory;
	else {
		DirectoryP CurrentChild = current->child;

		while (current->sibling != NULL)
			CurrentChild = CurrentChild->sibling;

		CurrentChild->sibling = NewDirectory;
	}
	return current;
}

DirectoryP ChangeDirectory(DirectoryP current, StackP head, char* wanted) {
	if (current->child == NULL) {
		printf("Directory is empty!\n");
		return current;
	}
	else {
		DirectoryP parent = current;
		DirectoryP search = current->child;
		while (search) {
			if (strcmp(search->name, wanted) == NULL) {
				push(search, head);
				return search;
			}
			search = search->sibling;
		}
		printf("Wanted directory does not exist!\n");
		return parent;
	}
}

DirectoryP PreviousDirectory(DirectoryP current, StackP head) {
	if (head->next == NULL) {
		printf("There is no previous directory to the current one!\n");
		return current;
	}
	else {
		pop(head);
		return head->next->data;
	}
}

int Print(DirectoryP current) {
	if (current->child == NULL)
		printf("Directory is empty!\n");
	else {
		current = current->child;
		while (current) {
			printf(" %s\n", current->name);
			current = current->sibling;
		}
	}

	return 0;
}

int DeleteAll(DirectoryP current) {
	if (current == NULL)
		return NULL;

	DeleteAll(current->child);
	DeleteAll(current->sibling);
	free(current);
}