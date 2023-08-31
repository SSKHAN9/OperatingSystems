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
#include <string.h>

//macros: constants
#define CHUNK_LENGTH (20+1)  //each chunk has twenty characters, we add 1 so
                           //there is space for the null terminator.
#define NUMBER_OF_CHUNKS 4 //the message is spread across 4 chunks.
#define DECRYPTION_SHIFT 5 //this is the ASCII table shift used for decryption.

//forward declarations
void sort_chunks();
void decrypt_chunks();
void display_chunks();

char chunks[NUMBER_OF_CHUNKS][CHUNK_LENGTH];

//given two strings, swaps their contents in memory.
void swap_strings(char* x, char* y) {
    //create a temporary holding place for the data so we don't lose it.
    char temp[CHUNK_LENGTH];

    strcpy(temp, x);
    strcpy(x, y);
    strcpy(y, temp);
}

void display_chunks() {
    for (int i = 0; i < NUMBER_OF_CHUNKS; i++) {
        printf("%s", chunks[i] + 1);
    }
}

void decrypt_chunks() {
    for (int i = 0; i < NUMBER_OF_CHUNKS; i++) {
        char* currChunk = chunks[i];
        for (int j = 1; currChunk[j] != '\0'; j++) {
            currChunk[j] -= DECRYPTION_SHIFT;
        }
    }
}

void sort_chunks(){
    for (int i = 0; i < NUMBER_OF_CHUNKS - 1; i++) {
        for (int j = 0; j < NUMBER_OF_CHUNKS - i - 1; j++) {
            if (chunks[j][0] > chunks[j + 1][0]) {
                swap_strings(chunks[j], chunks[j + 1]);
            }
        }
    }
}

int main() {
	//copy message into memory.
	strcpy(chunks[0], "2i1%fsi%fs%jstwrtzx%");
	strcpy(chunks[1], "1'H%nx%vznwp~1%kqf|j");
	strcpy(chunks[2], "4R3%Wnyhmnj%%%%%%%%%");
	strcpy(chunks[3], "3xzhhjxx3'%2%Ijssnx%");

	//the format of a chunk is a single number indicating its order in overall
	//message followed by twenty encrypted characters.


	//reorder chunks in message by sorting them based on the first digital
	//they contain. looking above, one can see they are currently in the order
	//2, 1, 4, 3 but should be ordered 1, 2, 3, 4.
	sort_chunks();

	//shift the characters in the message to produce the original characters.
	decrypt_chunks();

	//display the decrypted message.
	display_chunks();


	return 0; 
}


