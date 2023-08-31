/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS



////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef enum {SER, EGR, CSE, EEE} Subject;

typedef struct CourseNode {
    Subject subject;
    int number;
    char teacher[1024];
    int credit_hours;
    struct CourseNode* next;
    struct CourseNode* prev;
} CourseNode;




////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct CourseNode* course_collection = NULL;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);
void course_insert(Subject subject, int number, char* teacher, int credit_hours);
void course_drop(Subject subject, int number);



void add_class() {
    Subject subject;
    int number;
    char teacher[1024];
    int credit_hours;

    printf("Enter class subject (SER, EGR, CSE, EEE): ");
    scanf("%s", &subject);
    printf("Enter class number: ");
    scanf("%d", &number);
    printf("Enter class teacher: ");
    scanf("%s", teacher);
    printf("Enter class credit hours: ");
    scanf("%d", &credit_hours);

    course_insert(subject, number, teacher, credit_hours);
}

void drop_class() {
    Subject subject;
    int number;

    printf("Enter class subject (SER, EGR, CSE, EEE): ");
    scanf("%s", &subject);
    printf("Enter class number: ");
    scanf("%d", &number);

    course_drop(subject, number);
}

void credit() {
    int total_credits = 0;
    CourseNode* current = course_collection;
    while (current != NULL) {
        total_credits += current->credit_hours;
        current = current->next;
    }
    printf("Total credits: %d\n", total_credits);
}

void schedule_print() {
    CourseNode* current = course_collection;
    while (current != NULL) {
        switch (current->subject) {
            case SER:
                printf("SER ");
                break;
            case EGR:
                printf("EGR ");
                break;
            case CSE:
                printf("CSE ");
                break;
            case EEE:
                printf("EEE ");
                break;
        }
        printf("%d %d %s\n", current->number, current->credit_hours, current->teacher);
        current = current->next;
    }
}

void schedule_load() {
    FILE* data_file = fopen("courses.txt", "r");
    if (data_file == NULL) {
        return;
    }
    Subject subject;
    int number;
    char teacher[1024];
    int credit_hours;
    while (fscanf(data_file, "%d %d %d %s", &subject, &number, &credit_hours, teacher) == 4) {
        course_insert(subject, number, teacher, credit_hours);
    }
    fclose(data_file);
}

void schedule_save() {
    FILE* data_file = fopen("courses.txt", "w");
    if (data_file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    CourseNode* current = course_collection;
    while (current != NULL) {
        fprintf(data_file, "%d %d %d %s\n", current->subject, current->number, current->credit_hours, current->teacher);
        current = current->next;
    }
    fclose(data_file);
}

//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
	char input_buffer;

	printf("\n\nWelcome to ASU Class Schedule\n");

    schedule_load();

	//menu and input loop
	do {
		printf("\nMenu Options\n");
		printf("------------------------------------------------------\n");
		printf("a: Add a class\n");
		printf("d: Drop a class\n");
		printf("s: Show your classes\n");
		printf("q: Quit\n");
		credit();
		printf("Please enter a choice ---> ");

		scanf(" %c", &input_buffer);

		branching(input_buffer);
	} while (input_buffer != 'q');

    schedule_save();

	return 0;
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
	switch (option) {
	case 'a':
		add_class();
		break;

	case 'd':
		drop_class();
		break;

	case 's':
		schedule_print();
		break;

	case 'q':
		schedule_save();
		break;

	default:
		printf("\nError: Invalid Input.  Please try again...");
		break;
	}
}

void course_insert(Subject subject, int number, char *teacher, int credit_hours) {

    CourseNode* current = course_collection;
    while (current != NULL) {
        if (current->subject == subject && current->number == number) {
            printf("Course already exists.\n");
            return;
        }
        current = current->next;
    }

    CourseNode* new_node = (CourseNode*) malloc(sizeof(CourseNode));
    new_node->subject = subject;
    new_node->number = number;
    strcpy(new_node->teacher, teacher);
    new_node->credit_hours = credit_hours;
    new_node->next = NULL;
    new_node->prev = NULL;


    if (course_collection == NULL) {

        course_collection = new_node;
        course_collection->next = NULL;
        course_collection->prev = NULL;
    } else {
        current = course_collection;
        while (current->next != NULL && (current->subject < subject || (current->subject == subject && current->number < number))) {
            current = current->next;
        }
        if (current->prev == NULL && (current->subject > subject || (current->subject == subject && current->number > number))) {

            new_node->next = current;
            current->prev = new_node;
            course_collection = new_node;
        } else if (current->next == NULL && (current->subject < subject || (current->subject == subject && current->number < number))) {

            current->next = new_node;
            new_node->prev = current;
        } else {

            new_node->prev = current->prev;
            new_node->next = current;
            current->prev->next = new_node;
            current->prev = new_node;
        }
    }
}

void course_drop(Subject subject, int number) {
    CourseNode* current = course_collection;
    while (current != NULL) {
        if (current->subject == subject && current->number == number) {
            if (current->prev == NULL) {
                course_collection = current->next;
                if (current->next != NULL) {
                    current->next->prev = NULL;
                }
            } else if (current->next == NULL) {
                current->prev->next = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            free(current);
            return;
        }
        current = current->next;
    }
    printf("Course not found.\n");
}
