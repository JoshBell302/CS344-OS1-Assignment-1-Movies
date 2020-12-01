/*
Assignment 1: Movies
Creator: Josh Bell
Date of Creation: October 4th 2020
Date of Completion: October 7th 2020
File: main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ====================================================================================================

/*
Struct: movie
title		Title of the Movie			- pointer char
year		Year of release				- pointer int
language	Languages available			- pointer char 2D array
rating		Rating Value of the Movie	- pointer float
next									- pointer to the next movie
*/
struct movie {
	char * title;
	int * year;
	char * language[5][20];
	float * rating;
	struct movie* next;
};

// ====================================================================================================

/*
struct movie * createMovie
Takes the current line and takes the values and puts them into a movie node to form a linked list
Parameters: currLine
Returns:	movie *
*/
struct movie* createMovie(char* currLine) {
	// Initialize Values
	struct movie* currMovie = malloc(sizeof(struct movie));
	char * saveptr = NULL;
	char * languageStr = NULL;
	char* ratingstr = NULL;

	// The first token is the Movie's Title
	char * token = strtok_r(currLine, ",", &saveptr);
	currMovie->title = calloc(strlen(token) + 2, sizeof(char));
	strcpy(currMovie->title, token);


	// The next token is the Movie's Realese Date
	token = strtok_r(NULL, ",", &saveptr);
	currMovie->year = atoi(token);


	// The next token is the Movie's Languages
	token = strtok_r(NULL, "]", &saveptr);

	// Gets all Languages in a string then removes the first char '[' from the string
	languageStr = calloc(strlen(token) + 1, sizeof(char));
	strcpy(languageStr, token);
	memmove(&languageStr[0], &languageStr[1], strlen(languageStr));

	// Splits the Language string and inputs each language in the movie's language array
	int count = 0;
	token = strtok(languageStr, ";");
	while (token != NULL) {
		currMovie->language[count][0] = token;
		char* movieLan = currMovie->language[count][0];
		token = strtok(NULL, ";");
		count++;
	}


	// The last token is the Movie's Rating
	token = strtok_r(NULL, "\n", &saveptr);
	ratingstr = calloc(strlen(token) + 1, sizeof(char));
	strcpy(ratingstr, token);

	// Removes the ',' from the begining of the string then converts to float
	memmove(&ratingstr[0], &ratingstr[1], strlen(ratingstr));
	currMovie->rating = ratingstr;
	*(currMovie->rating) = atof(currMovie->rating);


	// Set the next node to NULL in the newly created student entry
	currMovie->next = NULL;

	// To show all data processed, not needed to show for assignment but nice to look at
	//char * movieTitle = currMovie->title;
	//char * movieLan1 = currMovie->language[0][0];
	//char * movieLan2 = currMovie->language[1][0];
	//char * movieLan3 = currMovie->language[2][0];
	//char * movieLan4 = currMovie->language[3][0];
	//char * movieLan5 = currMovie->language[4][0];
	//printf("currMovie ---\nTitle: %s\nYear: %d\nLanguage 1: %s\nLanguage 2: %s\nLanguage 3: %s\nLanguage 4: %s\nLanguage 5: %s\nRating: %.1f\n\n", movieTitle, currMovie->year, movieLan1, movieLan2, movieLan3, movieLan4, movieLan5, *(currMovie->rating));

	return currMovie;
}

// ====================================================================================================

/*
struct movie * processFile
Return a list of students by parsing data from each line of the specified file.
Parameters: filePath
Returns:	movie *
*/
struct movie* processFile(char* filePath)
{
	// Open the specified file for reading only
	FILE * movieFile = fopen(filePath, "r");

	// Initialize Values
	char * currLine = NULL;
	size_t len = 0;
	ssize_t nread;
	int count = 0;

	// The head and tail of the linked list
	struct movie* head = NULL;
	struct movie* tail = NULL;

	// Skips the first line of the csv file
	getline(&currLine, &len, movieFile);

	// Read the file line by line
	while ((nread = getline(&currLine, &len, movieFile)) != -1)
	{
		// To count number of movies parsed 
		count++;

		// Get a new movie node corresponding to the current line
		struct movie* newNode = createMovie(currLine);

		// Is this the first node in the linked list? If so then set the 
		if (head == NULL)
		{
			// This is the first node in the linked link
			// Set the head and the tail to this node
			head = newNode;
			tail = newNode;
		}
		else
		{
			// This is not the first node.
			// Add this node to the list and advance the tail
			tail->next = newNode;
			tail = newNode;
		}
	}
	printf("Parsed data for %d movies\n", count);
	free(currLine);
	fclose(movieFile);
	return head;
}

// ====================================================================================================

/*
int userInput
Requests user input [1 to 4] and checks for input errors and returns a valid value
Parameters: userValue
Returns:	int
*/
int userInput(int userValue) {
	// Initialize values
	int arraySize = 10;
	char strChoice[arraySize];
	memset(strChoice, '\0', arraySize);

	// Ask for the type of data the user wants to see, or to exit the program
	printf("Enter a choice from 1 to 4: ");
	fgets(strChoice, arraySize, stdin);
	userValue = strtol(strChoice, NULL, 0);

	if (userValue >= 1 && userValue <= 4)
		return userValue;

	printf("That was not a valid choice, please try again!\n\n");

	return 0;
}

// ====================================================================================================

/*
void dataOutput
Extracts data from a linked list of movies based on the users input and shows it
Parameters: userValue, llMovie
Returns:	---
*/
void dataOutput(int userValue, struct movie* llMovie) {
	if (userValue == 1) { // Show movies released in the specified year
		// Initialize Values
		int arraySize = 10;
		char userInput[arraySize];
		memset(userInput, '\0', arraySize);
		int userInt = 0;
		int count = 0;
		int highCount = 0;
		struct movie * temp;

		// Get year from user
		printf("Enter the year for which you want to see movies: ");
		fgets(userInput, arraySize, stdin);
		userInt = strtol(userInput, NULL, 0);

		// Check which movies were realeased in the given value (userValue)
		temp = llMovie;
		while (temp != NULL) {
			if (temp->year == userInt) {
				printf("%s\n", temp->title);
				count++;
			}
			temp = temp->next;
		}

		// Check to see if the given year was found in movies if not report back to the user
		if (count == 0)
			printf("No data about movies released in the year %d\n", userInt);
		printf("\n");
	}

	if (userValue == 2) { // Show highest rated movie for each year
		// Initialize Values
		struct movie* temp;
		int arraySize = 30;
		int yearArray[arraySize];
		int count = 0;
		int arrayCount = 0;
		int movieCount = 0;
		int highCount = 0;
		float highRating = 0.0;

		for (int i = 0; i < arraySize; i++)
			yearArray[i] = 0;

		// Go through linked list and pull out all years in the list
		temp = llMovie;
		while (temp != NULL) {
			yearArray[arrayCount] = temp->year;
			temp = temp->next;
			arrayCount++;
			movieCount++;
		}

		// Delete duplicate years in yearArray
		for (int i = 0; i < arrayCount; i++) {
			for (int j = i + 1; j < arrayCount; j++) {
				if (yearArray[i] == yearArray[j]) {
					for (int k = j; k < arrayCount; k++)
						yearArray[k] = yearArray[k + 1];
					arrayCount--;
					j--;
				}
			}
		}

		// Find all movies with the same year and show the one with the highest rating
		for (int a = 0; a < arrayCount; a++) {
			count = 0;
			highCount = 0;
			highRating = 0;
			temp = llMovie;
			for (int b = 0; b < movieCount; b++) {
				if (yearArray[a] == temp->year) {
					if (*(temp->rating) > highRating) {
						highRating = *(temp->rating);
						highCount = count;
					}
				}
				count++;
				temp = temp->next;
			}
			temp = llMovie;
			for (int c = 0; c < highCount; c++) {
				temp = temp->next;
			}

			// Print out the movie with the highest rating for the given year
			printf("%d %.1f %s\n", temp->year, *(temp->rating), temp->title);
		}
		printf("\n");
	}

	if (userValue == 3) { // Show the title and year of release of all movies in a specific language
		// Initialize Values
		struct movie* temp;
		int movieSize = 0;
		int count = 0;
		int arraySize = 20;
		char userArray[arraySize];
		memset(userArray, '\0', arraySize);
		char * movieLanguage;
		char * userLanguage;

		temp = llMovie;
		while (temp != NULL) {
			temp = temp->next;
			movieSize++;
		}

		// Get Language from user
		printf("Enter the language for which you want to see movies (Capitalize first letter): ");
		scanf("%s", userArray);

		// Traverse thorugh the linked list and display a movie thats released in that language
		temp = llMovie;
		for (int i = 0; i < movieSize; i++) {
			for (int j = 0; j < 5; j++) {
				if (temp->language[j][0] == NULL)
					break;
				movieLanguage = temp->language[j][0];
				userLanguage = userArray;
				if (*(userLanguage) == *(movieLanguage)) {
					printf("%d %s\n", temp->year, temp->title);
					count++;
				}
			}
			temp = temp->next;
		}

		// Check to see if the given year was found in movies if not report back to the user
		if (count == 0)
			printf("No movies released in %s\n", userArray);
		printf("\n");

		// Fixes a bug, without it userInput() would skip asking the user for their data with this here it will ask 
		fgets(userLanguage, arraySize, stdin);
	}
}

// ====================================================================================================

/*
void output
Shows the type of data that can be found in the linked list of movies and calls userInput() and dataOutput()
Parameters: llMovie
Returns:	---
*/
void output(struct movie* llMovie) {
	// Initialize Values
	int choice = 0;

	// Gives a better outline 
	printf("==================================================\n\n");
	printf("------------------Data Selection------------------\n");

	// Shows the type of data that can be shown
	printf("1. Show movies released in the specified year\n");
	printf("2. Show highest rated movie for each year\n");
	printf("3. Show the title and year of release of all movies in a specific language\n");
	printf("4. Exit from the program\n\n");

	// Requests user input
	while (choice == 0) {
		choice = userInput(choice);
	}

	// Determines data to show depending on value returned by userInput
	if (choice >= 1 && choice <= 3) {
		printf("\n-----------------------Data-----------------------\n");
		dataOutput(choice, llMovie);
		output(llMovie);
	}

	// If choice is 4 then we exit the program
	return;
}

// ====================================================================================================

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("You must provide the name of the file to process\n");
		return EXIT_FAILURE;
	}
	printf("==================================================\n");
	printf("Processed file: %s\n", argv[1]);
	struct movie * list = processFile(argv[1]);
	printf("==================================================\n\n");
	output(list);
	printf("\n==================================================\n");
	return EXIT_SUCCESS;
}