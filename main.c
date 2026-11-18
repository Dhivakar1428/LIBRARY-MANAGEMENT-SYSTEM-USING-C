#include "header.h"

static void book_menu(void)
{
    printf("\n========== BOOK MANAGEMENT MENU ==========\n");
    printf("1. Add New Book\n");
    printf("2. Update Book Details\n");
    printf("3. Remove Book\n");
    printf("4. Search Book\n");
    printf("5. View All Books\n");
    printf("6. Issue Book\n");
    printf("7. Return Book\n");
    printf("8. List Issued Books\n");
    printf("9. Save\n");
    printf("10. Exit\n");
    printf("==========================================\n");
}

int main(void)
{
    LIB *books = NULL;
    ISSUE *issues = NULL;
    int choice;

    read_books(&books);
    read_issues(&issues);

    printf("\nLibrary Management System\n");
    printf("Book records and issue records loaded.\n");

    while (1)
    {
        book_menu();
        choice = read_int("Enter your choice: ");

        switch (choice)
        {
            case 1:
                add_book(&books);
                break;
            case 2:
                update_book(books);
                break;
            case 3:
                remove_book(&books);
                break;
            case 4:
                search_book(books);
                break;
            case 5:
                display_books(books);
                break;
            case 6:
                issue_book(books, &issues);
                break;
            case 7:
                return_book(books, issues);
                break;
            case 8:
                display_issued_books(issues);
                break;
            case 9:
                save_books(books);
                save_issues(issues);
                printf("All records saved successfully.\n");
                break;
            case 10:
                save_books(books);
                save_issues(issues);
                free_books(books);
                free_issues(issues);
                printf("Data saved. Thank you!\n");
                return 0;
            default:
                printf("Invalid choice. Enter a number from 1 to 10.\n");
        }
    }
}
