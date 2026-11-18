LIBRARY MANAGEMENT SYSTEM (C)

Source files:
  main.c, book.c, issue.c, utils.c, header.h

Build:
  make

Run:
  make run
  or ./library

Clean:
  make clean

Data files:
  Book.txt   - Book records
  Issued.txt - Issue and return records

Book ID:
  Book ID is generated automatically. The next ID is one greater than
  the highest Book ID currently loaded from Book.txt. No ID input is needed.

Keep Book.txt and Issued.txt in the same folder as the executable.
The menu's Save option and Exit option save both data files.
