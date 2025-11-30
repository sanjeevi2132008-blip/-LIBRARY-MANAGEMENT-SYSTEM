#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 100

// ---------------- STRUCTURES ----------------
typedef struct {
    int id;
    char title[50];
    char author[50];
    int available;      // 1 = free, 0 = issued
    char issuedTime[50];
    char issuedToName[50];
    char issuedToDept[50];
    char issuedToReg[20];
} Book;

// ---------------- GLOBALS ----------------
Book books[MAX];
int bookCount = 0;
int nextBookID = 1001;

// ---------------- TIME FUNCTION ----------------
void getTimeNow(char *buff) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buff, 50, "%d-%m-%Y %H:%M:%S", t);
}

// ---------------- SAVE DATA ----------------
void saveData() {
    FILE *fb = fopen("books.dat", "wb");
    fwrite(&bookCount, sizeof(int), 1, fb);
    fwrite(books, sizeof(Book), bookCount, fb);
    fwrite(&nextBookID, sizeof(int), 1, fb);
    fclose(fb);
}

// ---------------- LOAD DATA ----------------
void loadData() {
    FILE *fb = fopen("books.dat", "rb");
    if (fb) {
        fread(&bookCount, sizeof(int), 1, fb);
        fread(books, sizeof(Book), bookCount, fb);
        fread(&nextBookID, sizeof(int), 1, fb);
        fclose(fb);
    }
}

// ---------------- FIND BOOK ----------------
int findBook(int id) {
    for (int i = 0; i < bookCount; i++)
        if (books[i].id == id) return i;
    return -1;
}

// ---------------- LOG BOOK ----------------
void logBook() {
    Book *bk = &books[bookCount];

    printf("Enter book title : ");
    scanf(" %[^\n]", bk->title);

    printf("Enter author name : ");
    scanf(" %[^\n]", bk->author);

    bk->id = nextBookID++;
    bk->available = 1;
    strcpy(bk->issuedTime, "Not Issued");
    strcpy(bk->issuedToName, "-");
    strcpy(bk->issuedToDept, "-");
    strcpy(bk->issuedToReg, "-");

    bookCount++;
    printf("Book Logged Successfully! Book ID = %d\n", bk->id);

    saveData();
}

// ---------------- ISSUE BOOK ----------------
void issueBook() {
    int bid;

    printf("Enter Book ID: ");
    if (scanf("%d", &bid) != 1) {
        printf("Invalid input! Only whole numbers allowed.\n");
        while (getchar() != '\n');
        return;
    }

    int i = findBook(bid);
    if (i == -1) { printf("Book not found!\n"); return; }
    if (!books[i].available) { printf("Book already issued!\n"); return; }

    printf("Enter Student Name: ");
    scanf(" %[^\n]", books[i].issuedToName);

    printf("Enter Department: ");
    scanf(" %[^\n]", books[i].issuedToDept);

    printf("Enter Register Number: ");
    scanf(" %[^\n]", books[i].issuedToReg);

    getTimeNow(books[i].issuedTime);
    books[i].available = 0;

    printf("Book issued successfully on %s\n", books[i].issuedTime);

    saveData();
}

// ---------------- RETURN BOOK ----------------
void returnBook() {
    int bid;

    printf("Enter Book ID: ");
    if (scanf("%d", &bid) != 1) {
        printf("Invalid input! Only whole numbers allowed.\n");
        while (getchar() != '\n');
        return;
    }

    int i = findBook(bid);
    if (i == -1) { printf("Book not found!\n"); return; }
    if (books[i].available) { printf("Book is not issued!\n"); return; }

    books[i].available = 1;
    strcpy(books[i].issuedTime, "Not Issued");
    strcpy(books[i].issuedToName, "-");
    strcpy(books[i].issuedToDept, "-");
    strcpy(books[i].issuedToReg, "-");

    printf("Book returned successfully!\n");

    saveData();
}

// ---------------- VIEW ALL BOOKS ----------------
void viewAllBooks() {
    if (bookCount == 0) {
        printf("No books available.\n");
        return;
    }
    printf("\n------ ALL LOGGED BOOKS (Total %d) ------\n", bookCount);
    for (int i = 0; i < bookCount; i++) {
        printf("\nID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n",
            books[i].id, books[i].title, books[i].author,
            books[i].available ? "Available" : "Issued");
    }
}

// ---------------- ISSUED BOOK LIST ----------------
void viewIssuedBooks() {
    int found = 0;
    printf("\n------ ISSUED BOOKS ------\n");
    for (int i = 0; i < bookCount; i++) {
        if (!books[i].available) {
            found = 1;
            printf("\nBook ID: %d\nTitle: %s\nIssued To: %s (%s, %s)\nIssued Time: %s\n",
                books[i].id, books[i].title,
                books[i].issuedToName,
                books[i].issuedToDept,
                books[i].issuedToReg,
                books[i].issuedTime
            );
        }
    }
    if (!found) printf("No books are issued.\n");
}

// ---------------- CHECK AVAILABILITY ----------------
void checkAvailability() {
    int bid;

    printf("Enter Book ID: ");
    if (scanf("%d", &bid) != 1) {
        printf("Invalid input! Only whole numbers allowed.\n");
        while (getchar() != '\n');
        return;
    }

    int i = findBook(bid);
    if (i == -1) { printf("Book not found!\n"); return; }

    if (books[i].available)
        printf("Book is AVAILABLE\n");
    else
        printf("Book is NOT AVAILABLE (Issued to %s, Reg: %s)\n",
               books[i].issuedToName, books[i].issuedToReg);
}

// ---------------- MAIN ----------------
int main() {
    loadData();

    int ch;
    while (1) {
        printf("\n===== LIBRARY MENU =====\n");
        printf("1. Log Book\n");
        printf("2. View All Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Check Availability\n");
        printf("6. Issued Books List\n");
        printf("7. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &ch) != 1) {
            printf("Invalid input! Only whole numbers are allowed.\n");
            while (getchar() != '\n');  // clear buffer
            continue;
        }

        switch (ch) {
            case 1: logBook(); break;
            case 2: viewAllBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: checkAvailability(); break;
            case 6: viewIssuedBooks(); break;
            case 7: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
