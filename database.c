// Paul Litscher
// 261167726
// The C file responsible for managing a persistant database stored as a CSV file

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


// Define the struct to represent a database record (1.3)
typedef struct Record {
	char handle[32];
	char comment[64];
	
	long unsigned int followerCount;
	long unsigned int lastModified;
}Record;


// The database struct (1.4)
typedef struct Database {
	// Pointer to the underying array  
	struct Record *records;
	// Length of array	
	int capacity;
	// count of array
	int size;
} Database; 


// Function declerations (1.4)

// Database must have initial size 0 and capacity 4 (1.4)
Database db_create() {		
	
	Database db;
	
	// Initialize database size and capacity	
	db.size = 0;
	db.capacity = 4;
	db.records = malloc(db.capacity * sizeof(struct Record));
	// Check for sucecessful memory allocation	
	if (db.records == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	return db;
}


// Function: Append a record to the database (1.4)
void db_append(Database * db, Record const * item) {

	if (db->size >= db->capacity) {		
	   	
		// Expand 'records' array
		int new_capacity = db->capacity * 2;

		// Alocate memory to hold 'new_capacity'
		Record *new_records = realloc(db->records, new_capacity * sizeof(struct Record));

		// Check for sucessful memory reallocation		
		if (new_records == NULL) {
			fprintf(stderr, "Memory reallocation failed\n");
			exit(EXIT_FAILURE);

		}
		// Update the records and capacity
		db->records = new_records;
		db->capacity = new_capacity;
	}

	// Copies item record into array
	db->records[db->size]= *item; 
	db->size++;
}


// Function: Get a pointer to the given index and checks bound of the index (1.4)
Record* db_index(Database * db, int index) {
	
	if (index < 0 || index >= db->size) {
		return NULL; 
	}
	
	return &(db->records[index]);
}

// Function: Look up a Record by its handle (1.4)
Record* db_lookup(Database * db, char const * handle) {
	
	for (int i = 0; i < db->size; i++) {

// Checks the handle of current record
		if (strcmp(db->records[i].handle, handle) == 0) {
			return &db->records[i];
		}
	}
	return NULL;
}

// Release memory in array(1.4)
void db_free(Database * db) {
	
	free(db->records);
	
// Free memory and clean up pointers
	db->records = NULL;
	db->size = 0;
	db->capacity = 0;
}

