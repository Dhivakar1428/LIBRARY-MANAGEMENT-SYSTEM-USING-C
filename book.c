#include "header.h"

static void trim_newline(char *text)
{
    text[strcspn(text, "\r\n")] = '\0';
}

LIB *find_book_by_id(LIB *head, int id)
{
    while (head != NULL)
    {
        if (head->book_id == id)
            return head;
        head = head->next;
    }
    return NULL;
}

void read_books(LIB **head)
{
    FILE *fp = fopen("Book.txt", "r");
    LIB *new_book, *last = NULL;
    char line[128];

    if (fp == NULL)
        return;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        new_book = (LIB *)calloc(1, sizeof(LIB));
        if (new_book == NULL)
            break;

        trim_newline(line);
        if (sscanf(line, "%d", &new_book->book_id) != 1)
        {
            free(new_book);
            break;
        }

        if (fgets(new_book->book_title, sizeof(new_book->book_title), fp) == NULL ||fgets(new_book->book_author, sizeof(new_book->book_author), fp) == NULL ||fgets(line, sizeof(line), fp) == NULL ||sscanf(line, "%d", &new_book->quantity) != 1 ||fgets(new_book->date, sizeof(new_book->date), fp) == NULL ||fgets(new_book->time, sizeof(new_book->time), fp) == NULL)
        {
            free(new_book);
            break;
        }

        trim_newline(new_book->book_title);
        trim_newline(new_book->book_author);
        trim_newline(new_book->date);
        trim_newline(new_book->time);

        new_book->next = NULL;

        if (*head == NULL)
            *head = new_book;
        else
            last->next = new_book;

        last = new_book;
    }

    fclose(fp);
}

void save_books(LIB *head)
{
    FILE *fp = fopen("Book.txt", "w");

    if (fp == NULL)
    {
        printf("Unable to open Book.txt for saving.\n");
        return;
    }

    while (head != NULL)
    {
        fprintf(fp, "%d\n%s\n%s\n%d\n%s\n%s\n",head->book_id, head->book_title, head->book_author,head->quantity, head->date, head->time);
        head = head->next;
    }

    fclose(fp);
}

void add_book(LIB **head)
{
    LIB *new_book, *temp;
    int next_id = 1;
    char title[TITLE_SIZE], author[AUTHOR_SIZE];
    time_t now;
    struct tm *local;

    /* Generate the next Book ID automatically. */
    for (temp = *head; temp != NULL; temp = temp->next)
    {
        if (temp->book_id >= next_id)
            next_id = temp->book_id + 1;
    }

    new_book = (LIB *)calloc(1, sizeof(LIB));
    if (new_book == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    new_book->book_id = next_id;
    printf("Automatically generated Book ID: %d\n", new_book->book_id);

    read_line("Enter Book Title (max 19 characters): ", title, sizeof(title));
    read_line("Enter Author Name (max 19 characters): ", author, sizeof(author));

    if (strlen(title) == 0 || strlen(author) == 0 ||
        strlen(title) >= TITLE_SIZE || strlen(author) >= AUTHOR_SIZE)
    {
        printf("Title and author must contain 1 to 19 characters.\n");
        free(new_book);
        return;
    }

    strcpy(new_book->book_title, title);
    strcpy(new_book->book_author, author);

    new_book->quantity = read_int("Enter Quantity: ");
    if (new_book->quantity < 0)
    {
        printf("Quantity cannot be negative.\n");
        free(new_book);
        return;
    }

    get_current_date(new_book->date, sizeof(new_book->date));
    now = time(NULL);
    local = localtime(&now);
    if (local != NULL)
        strftime(new_book->time, sizeof(new_book->time), "%H:%M:%S", local);

    new_book->next = NULL;

    if (*head == NULL)
        *head = new_book;
    else
    {
        temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_book;
    }

    printf("Book added successfully. Use Save to store the changes.\n");
}

void display_books(LIB *head)
{
    if (head == NULL)
    {
        printf("No book records available.\n");
        return;
    }

    printf("\n%-8s %-20s %-20s %-10s %-12s %-10s\n",
           "Book ID", "Book Title", "Author", "Quantity", "Date", "Time");
    printf("-------------------------------------------------------------------------------\n");

    while (head != NULL)
    {
        printf("%-8d %-20s %-20s %-10d %-12s %-10s\n",
               head->book_id, head->book_title, head->book_author,
               head->quantity, head->date, head->time);
        head = head->next;
    }
}

void search_book(LIB *head)
{
    int choice, id, found = 0;
    char key[NAME_SIZE];

    printf("\nSearch Book\n1. By Book ID\n2. By Book Name\n3. By Author Name\n");
    choice = read_int("Select search type: ");

    if (choice == 1)
    {
        id = read_int("Enter Book ID: ");
        while (head != NULL)
        {
            if (head->book_id == id)
            {
                printf("\nBook Found:\n");
                printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\nDate: %s\nTime: %s\n",
                       head->book_id, head->book_title, head->book_author,
                       head->quantity, head->date, head->time);
                return;
            }
            head = head->next;
        }
    }
    else if (choice == 2 || choice == 3)
    {
        read_line(choice == 2 ? "Enter Book Name: " : "Enter Author Name: ",
                  key, sizeof(key));

        while (head != NULL)
        {
            const char *field = (choice == 2) ? head->book_title : head->book_author;
            if (contains_ignore_case(field, key))
            {
                printf("\nBook Found:\n");
                printf("ID: %d\nTitle: %s\nAuthor: %s\nQuantity: %d\nDate: %s\nTime: %s\n",
                       head->book_id, head->book_title, head->book_author,
                       head->quantity, head->date, head->time);
                found = 1;
            }
            head = head->next;
        }

        if (found)
            return;
    }
    else
    {
        printf("Invalid search option.\n");
        return;
    }

    printf("No matching book record found.\n");
}

void update_book(LIB *head)
{
    int choice, id, found = 0;
    char key[NAME_SIZE];

    printf("\nUpdate Book\n1. By Book ID\n2. By Book Name\n");
    choice = read_int("Select option: ");

    if (choice == 1)
    {
        id = read_int("Enter Book ID: ");
        while (head != NULL)
        {
            if (head->book_id == id)
            {
                found = 1;
                break;
            }
            head = head->next;
        }
    }
    else if (choice == 2)
    {
        read_line("Enter Book Name: ", key, sizeof(key));
        while (head != NULL)
        {
            if (contains_ignore_case(head->book_title, key))
            {
                found = 1;
                break;
            }
            head = head->next;
        }
    }
    else
    {
        printf("Invalid option.\n");
        return;
    }

    if (!found || head == NULL)
    {
        printf("Book not found.\n");
        return;
    }

    printf("Current: %s by %s, Quantity: %d\n",
           head->book_title, head->book_author, head->quantity);
    read_line("New Book Title (Enter to keep current): ", key, sizeof(key));
    if (key[0] != '\0' && strlen(key) < TITLE_SIZE)
        strcpy(head->book_title, key);
    else if (strlen(key) >= TITLE_SIZE)
        printf("Title too long; keeping old title.\n");

    read_line("New Author Name (Enter to keep current): ", key, sizeof(key));
    if (key[0] != '\0' && strlen(key) < AUTHOR_SIZE)
        strcpy(head->book_author, key);
    else if (strlen(key) >= AUTHOR_SIZE)
        printf("Author name too long; keeping old author.\n");

    id = read_int("New Quantity (-1 to keep current): ");
    if (id >= 0)
        head->quantity = id;

    printf("Book updated. Use Save to store the changes.\n");
}

void remove_book(LIB **head)
{
    int choice, id, found = 0;
    char key[NAME_SIZE];
    LIB *current = *head, *previous = NULL;

    printf("\nRemove Book\n1. By Book ID\n2. By Book Name\n");
    choice = read_int("Select option: ");

    if (choice == 1)
        id = read_int("Enter Book ID: ");
    else if (choice == 2)
        read_line("Enter Book Name: ", key, sizeof(key));
    else
    {
        printf("Invalid option.\n");
        return;
    }

    while (current != NULL)
    {
        if ((choice == 1 && current->book_id == id) ||
            (choice == 2 && contains_ignore_case(current->book_title, key)))
        {
            found = 1;
            break;
        }
        previous = current;
        current = current->next;
    }

    if (!found)
    {
        printf("Book not found.\n");
        return;
    }

    if (previous == NULL)
        *head = current->next;
    else
        previous->next = current->next;

    free(current);
    printf("Book removed. Use Save to store the changes.\n");
}

void free_books(LIB *head)
{
    LIB *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
