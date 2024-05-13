# Instagram Database Management System (IGDB)
## Description
This repository contains the implementation for the `igdb` command-line program.
The program manages a persistent database stored in a CSV file to track Instagram account records, such as handle, follower count, and comments.

## Features

- **Custom Data Structures**: Utilizes custom `struct` types for handling records and the database itself.
- **CSV Manipulation**: Handles reading and writing of database records to and from a CSV file.
- **User Interaction**: Allows interactive database manipulations via a command-line interface.
- **Adresses Unsupported Actions**: Interface will let user know if incorrect manipulation of commands or command usages.
- **Date and Time Handling**: Implements UNIX timestamp for storing 'date last modified' and converts it into a human-readable format.

## Supported Commands

- `list`: Displays all records in a table format.
- `add <HANDLE> <FOLLOWERS>`: Adds a new record. Prompts for a comment.
- `update <HANDLE> <FOLLOWERS>`: Updates an existing record. Prompts for a new comment.
- `save`: Writes changes to the CSV file.
- `exit`: Exits the program. Warns if there are unsaved changes.
## User Interaction Example
![Screenshot 2024-05-13 132136](https://github.com/Paullitsc/Instagram_Profile_Database/assets/168594999/d1665058-0d34-49d7-97f8-090a3c7f460a)

## Development

Developed using:
- **Language**: C
- **Editor**: Vim
- **Compiler**: GCC on local Linux server
- **Build System**: **'Makefile'** to automate compilation and linking of the project files.
- **Database Format**: Data stored in a **'database.csv'** file, which serves as the backend for the application.
## Compilation and Usage

1. Clone the repository to your local Linux machine. Ensure you are in a Linux environment as the instructions and setup are tailored for Linux systems.

2. Update the `database.csv` file if you wish to manually edit the database before running the program.

3. To compile the program, navigate to the directory containing the source code and the `Makefile`.

4. Run the following command in the terminal:

    ```bash
    make
    ```

    This uses the provided `Makefile` to compile the C files into an executable named `igdb`.

5. To start the program, execute:

    ```bash
    ./igdb
    ```

    Follow the interactive prompts to perform operations such as adding, updating, and listing database records.
