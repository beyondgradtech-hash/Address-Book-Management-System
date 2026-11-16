/*Name:Likhith DC
Date:03/11/2026
Project Description:

Developed a console-based Address Book Management System using the C programming language to manage contact information efficiently. The application allows users to add, search, edit, delete, and display contacts through a menu-driven interface.

The system stores contact details such as name, phone number, and email address using structures and arrays. It includes input validation to ensure that phone numbers contain exactly 10 digits and email addresses follow the implemented validation rules.

Implemented file handling using fopen(), fprintf(), and fscanf() to save contact details in a CSV file (database.csv) and retrieve previously stored contacts when the application starts. This provides data persistence even after the program is closed.

Key Features:

Add Contact: Add new contacts with name, phone number, and email.

Input Validation: Validate phone numbers and email addresses.

Search Contact: Search contacts using name, phone number, or email.

Edit Contact: Update existing contact information.

Delete Contact: Remove selected contacts from the address book.

Display Contacts: View all stored contacts in a structured format.

File Handling: Save and load contact information using a CSV file.

Menu-Driven Interface: Perform operations through a simple console-based menu.

Technologies and Concepts Used

Programming Language: C

Structures and Arrays

Functions and Pointers

String Handling (strcmp(), strcpy(), strlen())

Conditional Statements and Loops

File Handling (fopen(), fclose(), fprintf(), fscanf())

CSV File Storage

Project Outcome

Successfully implemented a modular contact management application that demonstrates practical usage of C programming concepts, data manipulation, input validation, and file handling.      */
#include "addressbook.h"
#include <stdio.h>
#include <string.h>


// =====================================================
// 1. PHONE NUMBER VALIDATION
// =====================================================

int validate_phone(char phone[])
{
    int i;

    // Check whether the phone number contains exactly 10 digits
    if (strlen(phone) != 10)
    {
        return 0;   // Invalid phone number
    }

    // Check every character in the phone number
    for (i = 0; phone[i] != '\0'; i++)
    {
        // Check whether the character is between 0 and 9
        if (phone[i] < '0' || phone[i] > '9')
        {
            return 0;   // Invalid if any character is not a digit
        }
    }

    return 1;   // All 10 characters are digits
}


// =====================================================
// 2. EMAIL VALIDATION
// =====================================================

int validate_email(char email[])
{
    int i, flag = 0;
    int len = strlen(email);

    // Search for the @ symbol in the email
    for (i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            flag = 1;   // @ symbol found
            break;      // Stop searching
        }
    }

    // If flag is still 0, @ is missing
    if (flag == 0)
    {
        printf("Invalid email: @ is missing\n");
        return 0;
    }

    // @ should not be the first or last character
    if (i == 0 || email[i + 1] == '\0')
    {
        printf("Invalid email: @ position\n");
        return 0;
    }

    // Check whether the email contains uppercase letters
    for (i = 0; email[i] != '\0'; i++)
    {
        if (email[i] >= 'A' && email[i] <= 'Z')
        {
            printf("Invalid email: use lowercase letters\n");
            return 0;
        }
    }

    // Check whether the email ends with .com
    // len < 5 checks whether the email is long enough
    // The last four characters must be .com
    if (len < 5 ||
        email[len - 4] != '.' ||
        email[len - 3] != 'c' ||
        email[len - 2] != 'o' ||
        email[len - 1] != 'm')
    {
        printf("Invalid email: Must end with .com\n");
        return 0;
    }

    return 1;   // Email passed all these checks
}


// =====================================================
// 3. ADD CONTACT
// =====================================================

void add_contact(struct AddressBook *addressBook)
{
    char name[50];
    char phone[15];
    char email[50];

    // Check whether the address book has reached its capacity
    if (addressBook->contactCount >= 100)
    {
        printf("Address Book is full!\n");
        return;
    }

    // Get the contact name from the user
    printf("Enter name: ");
    scanf(" %49[^\n]", name);

    // Keep asking for the phone number until it is valid
    while (1)
    {
        printf("Enter phone number: ");
        scanf("%14s", phone);

        // Call phone validation function
        if (validate_phone(phone) == 1)
        {
            break;   // Exit loop if phone number is valid
        }
        else
        {
            printf("Invalid phone number. Enter 10 digits.\n");
        }
    }

    // Keep asking for email until it is valid
    while (1)
    {
        printf("Enter email: ");
        scanf("%49s", email);

        // Call email validation function
        if (validate_email(email) == 1)
        {
            break;   // Exit loop if email is valid
        }
        else
        {
            printf("Please enter a valid email again.\n");
        }
    }

    // Store the entered name in the next available contact position
    // contactCount represents the index of the next empty position
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);

    // Store the phone number in the same contact position
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);

    // Store the email in the same contact position
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);

    // Increase the total number of contacts after storing the details
    addressBook->contactCount++;

    printf("Contact saved successfully!\n");
}


// =====================================================
// 4. SEARCH CONTACT
// =====================================================

void search_contact(struct AddressBook *addressBook)
{
    char search[50];
    int choice, i;

    // Reset previous search result count
    // This makes the new search start from zero results
    addressBook->ir_size = 0;

    // Check whether at least one contact is available
    if (addressBook->contactCount == 0)
    {
        printf("No contacts available.\n");
        return;
    }

    // Ask the user which field should be used for searching
    printf("\nSearch Contact By:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Accept only choices from 1 to 3
    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice!\n");
        return;
    }

    // Get the value that the user wants to search
    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    // Check every contact in the address book
    for (i = 0; i < addressBook->contactCount; i++)
    {
        // If choice is 1, compare names
        // If choice is 2, compare phone numbers
        // If choice is 3, compare email addresses
        //
        // strcmp() returns 0 when both strings are equal

        if ((choice == 1 &&
             strcmp(addressBook->contacts[i].name, search) == 0) ||

            (choice == 2 &&
             strcmp(addressBook->contacts[i].phone, search) == 0) ||

            (choice == 3 &&
             strcmp(addressBook->contacts[i].email, search) == 0))
        {
            // Store the actual contact index in index_record
            addressBook->index_record[addressBook->ir_size] = i;

            // Increase the number of matching contacts
            addressBook->ir_size++;
        }
    }

    // Check whether any matching contact was found
    if (addressBook->ir_size == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    // Display all matching contacts
    printf("\nMatching Contacts:\n");

    for (i = 0; i < addressBook->ir_size; i++)
    {
        // Display serial number starting from 1
        printf("\nSl.No: %d\n", i + 1);

        // Use the stored actual index to display the contact
        display(addressBook->contacts[addressBook->index_record[i]]);
    }
}


// =====================================================
// 5. EDIT CONTACT
// =====================================================

void edit_contact(struct AddressBook *addressBook)
{
    char search[50];
    char new_name[50];
    char new_phone[15];
    char new_email[50];

    int choice, i, selected, position;

    // Check whether contacts are available
    if (addressBook->contactCount == 0)
    {
        printf("No contacts available to edit.\n");
        return;
    }

    // Reset the search result count
    addressBook->ir_size = 0;

    // Ask how the user wants to find the contact
    printf("\nFind contact to edit by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Validate the user's search choice
    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice!\n");
        return;
    }

    // Get the search value
    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    // Search through all contacts
    for (i = 0; i < addressBook->contactCount; i++)
    {
        // Compare the selected field with the search value
        if ((choice == 1 &&
             strcmp(addressBook->contacts[i].name, search) == 0) ||

            (choice == 2 &&
             strcmp(addressBook->contacts[i].phone, search) == 0) ||

            (choice == 3 &&
             strcmp(addressBook->contacts[i].email, search) == 0))
        {
            // Save the actual position of the matching contact
            addressBook->index_record[addressBook->ir_size] = i;

            // Increase the matching contact count
            addressBook->ir_size++;
        }
    }

    // Check whether a contact was found
    if (addressBook->ir_size == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    // Display all matching contacts
    for (i = 0; i < addressBook->ir_size; i++)
    {
        printf("\nSl.No: %d\n", i + 1);

        display(addressBook->contacts[addressBook->index_record[i]]);
    }

    // By default, select the first matching contact
    selected = 1;

    // If multiple contacts are found, ask the user to select one
    if (addressBook->ir_size > 1)
    {
        printf("\nEnter Sl.No of contact to edit: ");
        scanf("%d", &selected);

        // Check whether the entered serial number is valid
        if (selected < 1 || selected > addressBook->ir_size)
        {
            printf("Invalid serial number!\n");
            return;
        }
    }

    // Convert the selected serial number into the actual array index
    // selected - 1 is used because serial numbers start from 1
    position = addressBook->index_record[selected - 1];

    // Ask which contact field the user wants to update
    printf("\nWhat do you want to edit?\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Perform the update based on the user's choice
    switch (choice)
    {
        case 1:

            // Get the new name
            printf("Enter new name: ");
            scanf(" %49[^\n]", new_name);

            // Replace the old name with the new name
            strcpy(addressBook->contacts[position].name, new_name);

            printf("Name updated successfully!\n");
            break;


        case 2:

            // Ask for the new phone number until it is valid
            while (1)
            {
                printf("Enter new phone number: ");
                scanf("%14s", new_phone);

                if (validate_phone(new_phone))
                {
                    break;
                }

                printf("Invalid phone number. Enter 10 digits.\n");
            }

            // Replace the old phone number
            strcpy(addressBook->contacts[position].phone, new_phone);

            printf("Phone number updated successfully!\n");
            break;


        case 3:

            // Ask for the new email until it is valid
            while (1)
            {
                printf("Enter new email: ");
                scanf("%49s", new_email);

                if (validate_email(new_email))
                {
                    break;
                }

                printf("Please enter a valid email again.\n");
            }

            // Replace the old email
            strcpy(addressBook->contacts[position].email, new_email);

            printf("Email updated successfully!\n");
            break;


        default:

            // Handle an invalid edit option
            printf("Invalid choice!\n");
            break;
    }
}


// =====================================================
// 6. DELETE CONTACT
// =====================================================

void delete_contact(struct AddressBook *addressBook)
{
    char search[50];

    int choice, i, selected, position;

    // Check whether contacts are available
    if (addressBook->contactCount == 0)
    {
        printf("No contacts available to delete.\n");
        return;
    }

    // Reset previous search result count
    addressBook->ir_size = 0;

    // Ask the user how to search for the contact
    printf("\nDelete Contact By:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("3. Email\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Validate the search choice
    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice!\n");
        return;
    }

    // Get the search value
    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    // Search through all contacts
    for (i = 0; i < addressBook->contactCount; i++)
    {
        // Compare the selected field with the search value
        if ((choice == 1 &&
             strcmp(addressBook->contacts[i].name, search) == 0) ||

            (choice == 2 &&
             strcmp(addressBook->contacts[i].phone, search) == 0) ||

            (choice == 3 &&
             strcmp(addressBook->contacts[i].email, search) == 0))
        {
            // Store the actual index of the matching contact
            addressBook->index_record[addressBook->ir_size] = i;

            // Increase the matching result count
            addressBook->ir_size++;
        }
    }

    // Check whether a contact was found
    if (addressBook->ir_size == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    // Display all matching contacts
    for (i = 0; i < addressBook->ir_size; i++)
    {
        printf("\nSl.No: %d\n", i + 1);

        display(addressBook->contacts[addressBook->index_record[i]]);
    }

    // Select the first matching contact by default
    selected = 1;

    // If multiple contacts match, ask which one to delete
    if (addressBook->ir_size > 1)
    {
        printf("\nEnter Sl.No of contact to delete: ");
        scanf("%d", &selected);

        // Validate the selected serial number
        if (selected < 1 || selected > addressBook->ir_size)
        {
            printf("Invalid serial number!\n");
            return;
        }
    }

    // Get the actual array position of the selected contact
    position = addressBook->index_record[selected - 1];

    // Shift all contacts after the deleted contact one position left
    for (i = position; i < addressBook->contactCount - 1; i++)
    {
        // Copy the next contact into the current position
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    // Decrease the total number of contacts by one
    addressBook->contactCount--;

    printf("Contact deleted successfully!\n");
}


// =====================================================
// 7. LIST ALL CONTACTS
// =====================================================

void list_contacts(struct AddressBook *addressBook)
{
    int i;

    // Check whether any contacts are available
    if (addressBook->contactCount == 0)
    {
        printf("No contacts found.\n");
        return;
    }

    // Print the contact list heading
    printf("\n--------------- CONTACT LIST ---------------\n");

    // Display the column headings
    // %-20s means left-align the string in a 20-character space
    printf("%-20s %-20s %-20s %-20s\n",
           "Sl.No.", "Name", "Phone Number", "Email");

    printf("---------------------------------------------------------------\n");

    // Loop through all saved contacts
    for (i = 0; i < addressBook->contactCount; i++)
    {
        // Print serial number, name, phone and email
        printf("%-20d %-20s %-20s %-20s\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf("---------------------------------------------------------------\n");
}


// =====================================================
// 8. DISPLAY ONE CONTACT
// =====================================================

void display(struct Contact contacts)
{
    // Print the name of the contact
    printf("\nName: %s\n", contacts.name);

    // Print the phone number
    printf("Phone: %s\n", contacts.phone);

    // Print the email address
    printf("Email: %s\n", contacts.email);
}


// =====================================================
// 9. SAVE CONTACTS TO FILE
// =====================================================

void save_contacts_to_file(struct AddressBook *addressBook)
{
    FILE *fp;
    int i;

    // Open database.csv in write mode
    // "w" creates the file if it does not exist
    // If the file already exists, its old contents are overwritten
    fp = fopen("database.csv", "w");

    // Check whether the file opened successfully
    if (fp == NULL)
    {
        printf("Unable to open file!\n");
        return;
    }

    // Loop through all contacts in the address book
    for (i = 0; i < addressBook->contactCount; i++)
    {
        // Write one contact's name, phone and email into the file
        // Commas separate the three fields
        // \n moves to the next line for the next contact
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    // Close the file after writing all contacts
    fclose(fp);

    printf("Contacts saved successfully!\n");
}


// =====================================================
// 10. LOAD CONTACTS FROM FILE
// =====================================================

void load_contacts_from_file(struct AddressBook *addressBook)
{
    FILE *fp;

    // Open database.csv in read mode
    // "r" is used to read existing file contents
    fp = fopen("database.csv", "r");

    // If the file does not exist, stop loading
    // This can happen when the program runs for the first time
    if (fp == NULL)
    {
        return;
    }

    // Continue reading while:
    // 1. The address book has space for another contact
    // 2. fscanf successfully reads all three fields

    while (addressBook->contactCount < 100 &&

           fscanf(fp, " %49[^,],%14[^,],%49[^\n]",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        // Increase contactCount after successfully reading one contact
        // The next contact will be stored at the next array position
        addressBook->contactCount++;
    }

    // Close the file after reading all available contacts
    fclose(fp);
}