#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TITLE_SIZE 20
#define AUTHOR_SIZE 20
#define NAME_SIZE 50
#define DATE_SIZE 20
#define TIME_SIZE 20

typedef struct library
{
    int book_id;
    char book_title[TITLE_SIZE];
    char book_author[AUTHOR_SIZE];
    int quantity;
    char date[DATE_SIZE];
    char time[TIME_SIZE];
    struct library *next;
} LIB;

typedef struct issue
{
    int issue_id;
    int book_id;
    int user_id;
    char user_name[NAME_SIZE];
    char issue_date[DATE_SIZE];
    char due_date[DATE_SIZE];
    char return_date[DATE_SIZE];
    int fine;
    struct issue *next;
} ISSUE;

/* Utility functions */
void read_line(const char *prompt, char *buffer, size_t size);
int read_int(const char *prompt);
int contains_ignore_case(const char *text, const char *pattern);
void get_current_date(char *buffer, size_t size);
void get_due_date(char *buffer, size_t size, int days);
int calculate_late_days(const char *due_date, const char *return_date);

/* Book functions */
void read_books(LIB **head);
void save_books(LIB *head);
void add_book(LIB **head);
void update_book(LIB *head);
void remove_book(LIB **head);
void search_book(LIB *head);
void display_books(LIB *head);
LIB *find_book_by_id(LIB *head, int id);
void free_books(LIB *head);

/* Issue functions */
void read_issues(ISSUE **head);
void save_issues(ISSUE *head);
void issue_book(LIB *books, ISSUE **issues);
void return_book(LIB *books, ISSUE *issues);
void display_issued_books(ISSUE *head);
void free_issues(ISSUE *head);

#endif
