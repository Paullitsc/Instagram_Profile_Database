// Paul Litscher
// 261167726
// File for the main functions derived from database.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "database.h"

// Function: Print database contents in format to standard output
void print_database(Database *db) {

	// Table header
	printf("%-*s | %-*s | %-*s | %-*s\n", 20 , "HANDLE", 10, "FOLLOWERS", 20, "LAST MODIFIED", 30, "COMMENT");
	
	// Line to seperate header from the data rows
	for (int i=0; i < 89; i ++) {
		putc('_', stdout);
	}
	putc('\n', stdout);

	// Iterate through each record and print its details
	for (int i=0; i<(*db).size; i++) {
		char str_followers[20];
		char formatted_time[20];
		
		// Conversion of follower count: numeric -> string
		sprintf(str_followers, "%lu", (*db).records[i].followerCount);
		
		// Format list modified timestamp
		strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M", localtime(&(*db).records[i].lastModified));

		printf("%-20.*s | %-10.*s | %-20.*s | %-30.*s\n", 20, (*db).records[i].handle, 10, str_followers, 20, formatted_time, 30, (*db).records[i].comment);
	}
}

// Function: Parse a single record -> Record struct
Record parse_record(char const * line) {
	
	// Duplicate input line to avoid modifying the original string
        char *str = strdup(line);

        Record record;

	// Begin tokenization of string with comma as delimiter
        char *token = strtok(str, ",");
        char *line_copy = strdup(line);
        int choice = 1;
        
	while (token != NULL) { // Parsing tokens until there are no more.
                char *endptr;
                
		if (choice == 1){ // First field: Handle
                        strcpy(record.handle, token);
                        token = strtok(NULL, ",");
                        choice++;
                
		} else if(choice == 2){ // Second field: Follower Count
                        record.followerCount = strtoul(token, &endptr, 10);
			token = strtok(NULL, ",");
			choice++;
		} else if (choice ==3){ // Third field: Comment
			strcpy(record.comment, token); 
			token = strtok(NULL, ",");
			choice++;
		}else if (choice ==4){ // Last Modified
			record.lastModified = strtoul(token, &endptr, 10);
			token = strtok(NULL, ",");
			choice++;
		}else{ // If more fields than expected, give error
			printf("Error: Too many arguments given\n");
			token = NULL; 
		}
	}
	free(line_copy);
        return record;
}

// Function: Read and append records from the file given in path to the db (1.5)
void db_load_csv(Database * db, char const * path) {
        FILE *file = fopen(path, "r");

	if (file == NULL) {
		printf("File does not exist\n");
		return;
	}
        // Pointer for storing each line of file
	char *line = NULL;
	// Length of line
	size_t len;


	// Loops through file reading it into 'line'
        while (getline(&line, &len, file) != -1) {
                Record record; // Store the parsed record
		record = parse_record(line); // Parse current line into Record structure 
                db_append(db, &record);	
	}
        printf("Loaded %d records.\n", (*db).size);
	
	free(line);
	fclose(file);
}

// Takes and writes data from db to a CSV file specified by 'path'

void db_write_csv(Database *db, char const * path) {
        
	FILE *file = fopen(path, "w");
	
	// Allocate buffer for constructing each line
	char s[sizeof(Record)+sizeof(char)*5];
	char *str = s;

	// Makes sure the file was opened properly
        if (!file) {
                perror("Error opening file\n");
                return;
        }

	// Iterate over each record in database, writing the record's data to the file in CSV format
        for (int i = 0; i <(*db).size; i++) {
                // start constructing the CSV line by copying the handle
		strcpy(str, (*db).records[i].handle);
		str += strlen((*db).records[i].handle);
		*str = ',';
		str++;
			
		// Convert follower count to string and append
		char str_followers[21];
		sprintf(str_followers, "%ld", (*db).records[i].followerCount);
		strcpy(str, str_followers);
		str += strlen(str_followers);
		*str = ',';
		str++;
		
		// Append the comment
		strcpy(str, (*db).records[i].comment);
		str += strlen((*db).records[i].comment);
		*str = ',';
		str++;
	
		// Convert last modified timestamp to string and append.
		char str_date[21];
		sprintf(str_date, "%ld", (*db).records[i].lastModified);
		strcpy(str, str_date);
		str += strlen(str_date);
		*str = '\n';
		str++;
		*str = '\0';
		

		fprintf(file, "%s", s);
		str = s;
        }
        printf("wrote %d records.\n", (*db).size);
	fclose(file);
}

// Function: Prompt user for comment, perform validation, return valid comment
char *find_comment() {
	// Allocate memory for comment
	char *comment = malloc(sizeof(char)*64);
	
	printf("Comment> ");
	
	// Pointer to track start of first valid character in commment
	char *firstValChar = NULL;
	
	// Read line from stdin to the allocated memory
	if (fgets(comment, 64, stdin) == NULL) {
		printf("Error: No comment provided EOF reached\n");
	
	}
	// Loop through characters in commment for non-whitespace character
	for (int i=0; i<strlen(comment); i++) {
		if (comment[i] != ' ' && comment[i] != '\0' && comment[i] != '\n') {
			firstValChar = comment+i;
			break;
		}
	}

	if (firstValChar == NULL) {
		printf("ERROR: cannot have empty comment\n");
		return NULL;
	}else {
		// if valid start of the comment found, shift comment in te buffer till it leads
		strcpy(comment, firstValChar);
	
	}
	// Check if comment contains any commas
	if (strchr(comment, ',') != NULL) {
		printf("Error: Comment cannot contain commas\n");
		return NULL;

	}else if (comment[strlen(comment)-1] == '\n') {
		// replace last character, if new line, with null
		comment[strlen(comment)-1] = '\0';

	}else {
		printf("Error: comment is invalid: (too long, invalid characters)\n");
		
		// Consume remaining characters in input buffer till newline/EOF reached
		char c = getc(stdin);
		while(c !='\n' && c != EOF) {
			c = getc(stdin);
		}
	}
	return comment;
}

int main_loop(Database * db) {
		// Main variable for storing user input and command processing
		char *input = NULL;
		
		char option[7] = {'\0'};		
		char handle[32];
		
		size_t len = 0;
		size_t followers = 0;

		int saved = 1;

	// Main loop to continuously read and process commands
	while(1){ 
		

		option[0] = '\0';
		printf("> ");
		// Read a line of input, when EOF reached, free input buffer and break loop
		if (getline(&input, &len, stdin) == -1) { 
			free(input);
			break;
		
		// Check if the last character is a newline.
		}else if (input[strlen(input)-1] == '\n') {
			input[strlen(input)-1] = '\0';	

		
		}else {  
			 char c = getc(stdin);
			 while (c != '\n' && c != EOF){
				 c = getc(stdin);
			}
			continue;
		}
		// Tokenize input to process commands,'token' points to first word in input
		char *token = strtok(input, " ");
		int count = 1;
		int error_occured = 0;
		
		// Tokenize and process each part of input begins here
		while (token != NULL) {
			switch (count) {
				// First token: Expected to be the command (e.g., "list", "add").
				case 1:
					// Check if the command length is within the expected range.
					if (strlen(token) <= 6){
						strcpy(option, token);
						token = strtok(NULL, " ");
						count++;
						break;
					
					}else {
						printf("Error: Incorrect command (list, add, update, save, exit)\n");
						error_occured = 1;
						count++;
						token = NULL;
						break;
					}
				
				// Second token: Expected to be the handle (a form of identifier).
				case 2:
					// Check if the handle length is within the expected range
					if (strlen(token) <= 31) {
						strcpy(handle, token);
						token = strtok(NULL, " ");
						count++;
						break;
					
					}else {
						printf("Error: handle too long. \n");
						token = NULL;
						error_occured = 1;
						break;
					}
				// Third token processing. This case is only reached if the command is "add" or "update".
				case 3:
					if ((strcmp(option, "add") != 0) && (strcmp(option, "update") != 0)) {
						token = NULL;
						break;
					
					}else if (strlen(token) <= 19) {
						char *endptr;
						char *str_followers = malloc(sizeof(char)*19);
						followers = strtoul(token, &endptr, 10);
						sprintf(str_followers, "%lu", followers);

						if (*endptr != '\0') { 
							// If conversion from string to number is incomplete, report an error
							printf("Error: follower count must be a number\n");
							token = NULL;
							error_occured = 1;
							free(str_followers);
							break;
						
						// Check for negative number by comparing original and converted strings.
						}else if (strcmp(token, str_followers) != 0) {
							printf("Error: Cannot have negative followers\n");
							token = NULL;
							error_occured = 1;
							free(str_followers);
							break;
						// If no errors, free memory and proceed to next token.
						}else {
							free(str_followers);
							token = strtok(NULL, " ");
							count++;
								break;
						}
					

					// Handle case where followers number exceeds maximum expected size.
					}else {
						printf("Error; followers too big\n");
						token = NULL;
						error_occured = 1;
					}	
				 // Case for processing additional arguments if required by the command.
				case 4:
					count++;
					token=NULL;
					break;
			}	
			
		}
		// After processing all tokens, check for errors.
		if (error_occured) {
				continue;
		}else if (strcmp(option, "list") == 0){
			if (count != 2) {
				printf("Error: usage: list\n");
				continue;
			}	
			print_database(db);

		// For "add" command, validate the number of tokens and handle format.
		}else if (strcmp(option, "add") == 0) {

			if (count != 4) {
				printf("Error: usage: add HANDLE FOLLOWERS\n");
				continue;
			
			}else if (handle[0] != '@') {
				printf("Error: Handle always starts with @\n");
				continue;
				
			}else if (handle[1] == '\0') {
				printf("Error: handle cannot be empty\n");
				continue;
				
			}else if (db_lookup(db, handle) != NULL){
				printf("Error: handle %s already exists.\n", handle);
				continue;
				
			}else {
				Record r;
				char *comment = find_comment();
				
				 // If a valid comment is provided, copy it to the record's field.
				if (comment != NULL){
					strcpy(r.comment, comment);
					strcpy(r.handle, handle);
					
					r.followerCount = followers;
					r.lastModified = time(NULL);
					
					db_append(db, &r);
					saved = 0;
					free(comment);
				}
			}


		// For "update" command, validate the number of tokens and handle format.	
		}else if (strcmp(option, "update") == 0) {
				
			if (count != 4) {
				printf("Error: usage: update HANDLE FOLLOWERS\n");
				continue;
			
			}else if (handle[0] != '@') {
				printf("Error: Handle always starts with @\n");
				continue;
		
			}else if (handle[1] == '\0') {
				printf("Error: Handle cannot be empty\n");
				continue;
			}
			
			// Attempt to find the record with the given handle in the database.	
			Record *r = db_lookup(db, handle);
				
			if (r != NULL) {
				char *comment = find_comment();
					
				if (comment != NULL) {
					// Update the record's comment, follower count, and last modified timestamp
					strcpy((*r).comment, comment);
					(*r).followerCount = followers;
					(*r).lastModified = time(NULL);
					
					saved = 0;
					free(comment);
				}
			
			}else {
				printf("Error: no entry with handle %s\n", handle);
				continue;
			}
		// For "save" command, ensure no extra arguments were provided.
		}else if (strcmp(option, "save") == 0) {
			if (count != 2) {
				printf("Error: usage: save\n");
				continue;
				
			}else { 
					db_write_csv(db, "database.csv");
					saved = 1;
			}
			
	
		}else if (strcmp(option, "exit") == 0) {
			// Handle the "exit" command, checking for correct usage based on token count.
			if (count != 2 && count != 3) {
				printf("Error: usage: exit or exit fr\n");
				continue;
				
			}else {
				if (count == 3) {
					// Check if the argument is "fr" for force exit.
					if (strcmp(handle, "fr") == 0){
						free(input);
						db_free(db);
						break;
					}else {
						printf("Error: usage: exit or exit fr\n");
						continue;
					}
				// If exiting without "fr", check if changes are saved.
				}else {
					if (saved){
						free(input);
						db_free(db);
						break;
					 // Warn about unsaved changes if not using "fr" to force exit.
					}else {
						printf("Error: you didn't save your changes. Use 'exit fr' to force exit anyways.\n");
					}
				}
			
			
			}
		// Catch-all for unrecognized commands.
		}else {
				printf("Error: Incorrect command (list, add, update, save, exit)\n");
				continue;
		}
	}
	
}
					
int main() {
	// Initialize the database by creating it.
	Database db = db_create();
	// Load initial data into the database from a CSV file.
	db_load_csv(&db, "database.csv");
	// Enter the main command processing loop.
	return main_loop(&db);
	// The return value from 'main_loop' can be used to indicate the program's exit status.
}

