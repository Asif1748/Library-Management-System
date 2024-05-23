#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function prototypes
int main();
void database();
void developer();
void mainmenu();
void user_login();
void admin_login();
void admin_menu();
void update_admin();
void user_menu();
void update_user();
void load_book_number();
void save_book_number();
void load_books_from_file();
void remove_book_by_name();
void add_book();
void write_to_file();
void display_books_from_file();
void request_book();
void write_requested_books_to_file();
void display_requested_books();
void search_book_by_name();
void borrow_book();
void save_borrowed_book();
void track_book();
void load_members();


//LOGIN AND SIGN IN STRUCTURE
int a_count=0,m_count=0,mt_count=0,no=1,request_book_no=1;
char a_id[10],m_id[10],user_ID[8],file1[]="books.txt",file2[]="request_books.txt",file3[]="book_number.txt";
typedef struct book
{
    char book_name[100];
    char book_author[100];
    int book_no,publish_year,number_of_books;
    struct book *next;
} book;
book *head=NULL,*rear=NULL,*head2=NULL,*tail=NULL;
struct borrowed_book
{
    char book_author[100];
    char book_name[100];
    int book_no;
} b;
struct member
{
    char id[16];
    char name[50];
    char mail[50];
    char num[11];
    char pass[50];
    struct member*next;
} m,s;
struct member *m_head=NULL,*m_temp=NULL,*s_temp=NULL,*mnode;
struct admin
{
    char id[16];
    char name[50];
    char mail[50];
    char num[11];
    char pass[50];
    struct admin* next;
} a,f;
struct admin *a_head=NULL,*a_temp=NULL,*b_temp=NULL,*node;
void load_book_number()
{
    FILE *file = fopen(file3, "r");
    if (file != NULL)
    {
        fscanf(file, "%d", &no);
        fclose(file);
    }
}
// Function to save the last assigned book number to a file
void save_book_number()
{
    FILE *file = fopen(file3, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", no);
        fclose(file);
    }
}
void load_books_from_file(const char *book_name, int hint)
{
    FILE *file = fopen(file1, "r");
    if (file == NULL)
    {
        printf("\n\t\t\t\t\t\t\tSORRY ! NO FILE FOUND.\n");
        return;
    }

    while (!feof(file))
    {
        book *new_book = (book *)malloc(sizeof(book));
        if (new_book == NULL)
        {
            printf("\n\t\t\t\t\t\t\tSORRY ! MEMORY NOT ALLOCATED.\n");
            fclose(file);
            return;
        }

        if (fscanf(file, "*Book No : %d*\nBook Name : %[^\n]\nAuthor : %[^\n]\nPublished in %d\nNumber of books : %d\n",
                   &new_book->book_no, new_book->book_name, new_book->book_author, &new_book->publish_year, &new_book->number_of_books) != 5)
        {
            free(new_book);
            break;
        }

        new_book->next = NULL;

        if (head == NULL)
        {
            head=head2 = rear = new_book;
        }
        else
        {
            rear->next = new_book;
            rear = new_book;
        }
    }

    fclose(file);
    if (hint == 1)
    {
        remove_book_by_name(book_name);
    }
    else
    {
        return;
    }
}
void remove_book_by_name(const char *book_name)
{
    book *current = head2;
    book *prev = NULL;
    // Traverse the linked list to find the book by name
    while (current != NULL && strcmp(current->book_name, book_name) != 0)
    {
        prev = current;
        current = current->next;
    }
    // If book is not found
    if (current == NULL)
    {
        printf("\n\t\t\t\t\t\t\tBook with name '%s' not found.\n", book_name);
        printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
        fflush(stdin);   // fflush is used for flushing the output buffer of a stream
        getchar();
        admin_menu();
    }
    // If the book to be removed is the first node
    if (prev == NULL)
    {
        head2 = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    // Free the memory allocated for the book
    free(current);
    printf("\n\t\t\t\t\t\t\tBook with name '%s' removed successfully.\n", book_name);
// Renumber the books
    book *yes = head2;
    int new_book_no = 1;
    while (yes != NULL)
    {
        yes->book_no = new_book_no++;
        yes = yes->next;
    }

    // Write the new count to file3
    FILE *filee = fopen(file3, "w");
    if (filee != NULL)
    {
        fprintf(filee, "%d", new_book_no - 1);  // Corrected to new_book_no - 1
        fclose(filee);
    }
    // Rewrite the updated linked list to the file
    FILE *file = fopen(file1, "w");
    if (file == NULL)
    {
        printf("\n\t\t\t\t\t\t\tFailed to open file for writing.\n");
        return;
    }

    book *currentt = head2;
    while (currentt != NULL)
    {
        fprintf(file, "*Book No : %d*\nBook Name : %s\nAuthor : %s\nPublished in %d\nNumber of books : %d\n",
                currentt->book_no, currentt->book_name, currentt->book_author, currentt->publish_year, currentt->number_of_books);
        currentt = currentt->next;
    }
    fclose(file);
    printf("\n\t\t\t\t\t\t\tUpdatedlist written to file.\n");
    printf("\nPRESS ENTER TO RETURN......");
    fflush(stdin);   // fflush is used for flushing the output buffer of a stream
    getchar();
    admin_menu();
}
void add_book(int ko)
{
    printf("\n\t\t\t\t\t\t\t|  ENTER BOOK INFORMATION  |\n\n");
    load_book_number();
    for(int i=0; i<ko; i++)
    {
        book *new_book=(book*)malloc(sizeof(book));
        new_book->book_no=no++;
        printf("\t\t\t\t\t\t\tBook Name      : ");
        getchar();
        scanf("%[^\n]s",&new_book->book_name);
        getchar();
        printf("\t\t\t\t\t\t\tAuthor         : ");
        scanf("%[^\n]s",&new_book->book_author);
        printf("\t\t\t\t\t\t\tPulishing year : ");
        scanf("%d",&new_book->publish_year);
        printf("\t\t\t\t\t\t\tNumber of books: ");
        scanf("%d",&new_book->number_of_books);
        new_book->next=NULL;
        if(new_book==NULL)
        {
            printf("\t\t\t\t\t\t\tMEMORY ALLOCATION UNSUCCESSFUL !\n");
        }
        else if(head==NULL)
        {
            head=rear=new_book;
        }
        else
        {
            rear->next=new_book;
            rear=new_book;
        }
        write_to_file();
    }
    save_book_number();
    printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tBOOKS ADDED SUCCESSFULLT !");
    printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
    getchar();
    admin_menu();
}
void write_to_file()
{
    FILE *file = fopen(file1, "a+");
    if (file == NULL)
    {
        printf("SORRY ! FAILED TO OPEN THE FILE.\n");
        return;
    }

    book *current = rear;
    while (current != NULL)
    {
        fprintf(file, "*Book No : %d*\nBook Name : %s\nAuthor : %s\nPublished in %d\nNumber of books : %d\n",current->book_no, current->book_name, current->book_author, current->publish_year, current->number_of_books);
        current = current->next;
    }
    fclose(file);
}
void display_books_from_file(int mi)
{
    FILE *file = fopen(file1, "r");
    if (file == NULL)
    {
        printf("\n\t\t\t\t\t\t\tSORRY ! CURRENTLY THERE ARE NO BOOKS\n");
        printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();
        if(mi == 2)
        {
            user_menu();
        }
        else
        {
          admin_menu();
        }

    }
    char line[256];
    printf("\t\t\t\t\t\t\t*********************************\n");
    printf("\t\t\t\t\t\t\t|           Book List           |\n");
    printf("\t\t\t\t\t\t\t*********************************\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("\t\t\t\t\t\t\t%s", line);
    }
    fclose(file);
    printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
    getchar();
    if(mi==1)
        admin_menu();
    else
        user_menu();
}
void request_book()
{
    printf("\t\t\t\t\t\t\t|| REQUEST FOR A BOOK ||\n\n");
    book *n_book=(book*)malloc(sizeof(book));
    n_book->book_no=request_book_no++;
    printf("\t\t\t\t\t\t\tBook Name : ");
    getchar();
    scanf("%[^\n]s",&n_book->book_name);
    getchar();
    printf("\t\t\t\t\t\t\tAuthor : ");
    scanf("%[^\n]s",&n_book->book_author);
    printf("\t\t\t\t\t\t\tPulishing year : ");
    scanf("%d",&n_book->publish_year);
    n_book->next=NULL;
    if(n_book==NULL)
    {
        printf("\n\t\t\t\t\t\t\tMEMORY ALLOCATION UNSUCCESSFUL!\n");
    }

    else if(head2==NULL)
    {
        head2=tail=n_book;
    }
    else
    {
        tail->next=n_book;
        tail=n_book;
    }
    write_requested_books_to_file();
    printf("\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tREQUEST WAS SUCCESSFUL!");
    printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
    getchar();
    user_menu();
}
void write_requested_books_to_file()
{
    FILE *filee=fopen(file2,"w+");
    if(filee==NULL)
    {
        printf("\t\t\t\t\t\t\tSORRY ! FILE NOT OPENED .\n");
    }
    book *latest=head2;
    while(latest!=NULL)
    {
        fprintf(filee,"%d_______\nBook Name : %s\nAuthor : %s\nPublishing Year : %d\n",latest->book_no,latest->book_name,latest->book_author,latest->publish_year);
        latest=latest->next;
    }
    fclose(filee);
}
void display_requested_books()
{
    FILE *filee=fopen(file2,"r");
    printf("\t\t\t\t\t\t\t*********************************\n");
    printf("\t\t\t\t\t\t\t|    Requested   Book   List    |\n");
    printf("\t\t\t\t\t\t\t*********************************\n");
    if(filee==NULL)
    {
        printf("\n\t\t\t\t\t\t\tFILE IS EMPTY !\n");
    }
    char line[256];
    while(fgets(line,sizeof(line),filee))
    {
        printf("\t\t\t\t\t\t\t%s\n",line);
    }
    fclose(filee);
    printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
    getchar();
    admin_menu();
}
void search_book_by_name(char search_name[],int mi)
{
    int rem1;
    FILE *file = fopen(file1, "r");
    if (file == NULL)
    {
        printf("\n\t\t\t\t\t\t\tSORRY ! NO FILE FOUND\n");
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, search_name) != NULL)
        {
            printf("\t\t\t\t\t\t\t%s", line);
            fgets(line, sizeof(line), file); // Read next line to get author
            printf("\t\t\t\t\t\t\t%s", line);
            fgets(line, sizeof(line), file); // Read next line to get publish year
            printf("\t\t\t\t\t\t\t%s", line);
            fgets(line, sizeof(line), file); // Read next line to get number of books
            printf("\t\t\t\t\t\t\t%s", line);
            found = 1;
        }
    }
    fclose(file);
    if (!found)
    {
        printf("\t\t\t\t\t\t\tBook not found with the name '%s'\n", search_name);
        if(mi==2)
        {
            printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
            fflush(stdin);   //fflush is used for flushing the output buffer of a stream
            getchar();
            user_menu();
        }
        else
        {
            printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
            fflush(stdin);   //fflush is used for flushing the output buffer of a stream
            getchar();
            admin_menu();
        }
    }
    else
    {
        if(mi==2)
        {
            printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
            fflush(stdin);   //fflush is used for flushing the output buffer of a stream
            getchar();
            user_menu();
        }
        else
        {
            printf("\t\t\t\t\t\t-------------------------------------\n");
            printf("\t\t\t\t\t\t|      1.REMOVE      |   2. RETURN  |\n");
            printf("\t\t\t\t\t\t-------------------------------------\n");
            printf("\t\t\t\t\t\t\tEnter your choice : ");
            scanf("%d",&rem1);
            if(rem1==1)
            {
                system("cls");
                load_books_from_file(search_name,rem1);
            }
            else
            {
                system("cls");
                admin_menu();
            }
        }
    }
}
// Function to borrow a book
void borrow_book()
{
    char book_borrow[100];
    printf("\n\t\t\t\t\t\t\tEnter the book name you want to borrow: ");
    getchar();
    scanf("%[^\n]s", &book_borrow);
    load_books_from_file(book_borrow,2);
    book *currentt = head;
    while (currentt != NULL)
    {
        if (strcmp(currentt->book_name,book_borrow)==0)
        {
            if (currentt->number_of_books > 0)
            {
                currentt->number_of_books--;
                strcpy(b.book_name, currentt->book_name);
                strcpy(b.book_author, currentt->book_author);
                save_borrowed_book();
                printf("\n\t\t\t\t\t\t\tBook '%s' borrowed successfully!", currentt->book_name);
                printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......\n");
                fflush(stdin);   // fflush is used for flushing the output buffer of a stream
                getchar();
                user_menu();
            }
            else
            {
                printf("\t\t\t\t\t\t\tSorry, the book is out of stock!\n");
                printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
                fflush(stdin);   // fflush is used for flushing the output buffer of a stream
                getchar();
                user_menu();
            }
        }
        currentt = currentt->next;
    }

    printf("\t\t\t\t\t\t\tBook with the provided name not found!\n");
    printf("\n\t\t\t\t\t\t\tPRESS ENTER TO RETURN......");
    fflush(stdin);   // fflush is used for flushing the output buffer of a stream
    getchar();
    user_menu();
}

// Function to save information about borrowed book to a file
void save_borrowed_book()
{
    FILE *inputFile = fopen("user.txt", "rb");
    if (inputFile == NULL)
    {
        printf("Error opening source file.\n");
        return;
    }

    FILE *outputFile = fopen("borrowed.txt", "a+b");
    if (outputFile == NULL)
    {
        printf("Error opening destination file.\n");
        fclose(inputFile);
        return;
    }
    int found = 0;

    while (fread(&m, sizeof(m), 1, inputFile) == 1)
    {
        if (strcmp(m.id, user_ID) == 0)
        {
            fwrite(&m, sizeof(m), 1, outputFile);
            fwrite(&b, sizeof(b), 1, outputFile);
            found = 1;
            break; // Assuming the ID is unique, so we can stop searching
        }
    }

    if (!found)
    {
        printf("No user found with ID %s\n", user_ID);
    }
    else
    {
        printf("\t\t\t\t\t\t\tUser informationhas been written\n");
    }

    fclose(inputFile);
    fclose(outputFile);
}
void track_book()
{
    printf("\t\t\t\t\t\t\t========================================================\n");
    printf("\t\t\t\t\t\t\t|              ***BORROWED BOOK INFO***                |\n");
    printf("\t\t\t\t\t\t\t========================================================\n\n");
    FILE *inputFile = fopen("borrowed.txt", "rb");
    if (inputFile == NULL)
    {
        printf("\t\t\t\t\t\t\tError opening borrowed file.\n");
        return;
    }

    struct member temp_member;
    struct borrowed_book temp_borrowed_book;

    while (fread(&temp_member, sizeof(struct member), 1, inputFile) == 1)
    {
        fread(&temp_borrowed_book, sizeof(struct borrowed_book), 1, inputFile);
        printf("\t\t\t\t\t\t\tUser ID: %s\n", temp_member.id);
        printf("\t\t\t\t\t\t\tName: %s\n", temp_member.name);
        printf("\t\t\t\t\t\t\tEmail: %s\n", temp_member.mail);
        printf("\t\t\t\t\t\t\tPhone: %s\n", temp_member.num);
        printf("\t\t\t\t\t\t\tBook Name: %s\n", temp_borrowed_book.book_name);
        printf("\t\t\t\t\t\t\tBook Author: %s\n", temp_borrowed_book.book_author);
        printf("\t\t\t\t\t\t\tBook Number: %d\n", temp_borrowed_book.book_no);
        printf("\t\t\t\t\t\t\t--------------------------------------\n");
    }

    fclose(inputFile);
}

FILE *fp;//admin data
FILE *ft;
void admin_login()
{
    printf("\t\t\t\t\t\t\t#################################################################\n");
    printf("\t\t\t\t\t\t\t#                                                               #\n");
    printf("\t\t\t\t\t\t\t#                         ***ADMIN LOGIN***                     #\n");
    printf("\t\t\t\t\t\t\t#                                                               #\n");
    printf("\t\t\t\t\t\t\t#################################################################\n");
    int match=0,count=0;
    char id[8];
    char pass[50];
    if(a_count == 0)
    {
        printf("\t\t\t\t\t\t\tADD LIBRARIAN........ \n\n");

        printf("\t\t\t\t\t\t\tID NUMBER : ");
        fflush(stdin);
        gets(a.id);
        for(int i = 0; a.id[i] != '\0'; i++)
        {
            if (a.id[i] >= '0' && a.id[i] <= '9')
            {
                count++;
            }
            else
            {
                printf("\n\n\t\t\t\t\t\tInvalid Input!!! Press ENTER to try again...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                admin_login();
            }
        }
        if(count != 8)
        {
            printf("\n\n\t\t\t\t\t\t\tThis ID is INVALID. Please,Enter Correct ID...");
            fflush(stdin);// Clear the input buffer
            getchar(); // Wait for Enter key
            system("cls");
            admin_login();
        }

        printf("\t\t\t\t\t\t\tNAME      : ");
        fflush(stdin);
        gets(a.name);

        printf("\t\t\t\t\t\t\tEMAIL     : ");
        fflush(stdin);
        gets(a.mail);

        printf("\t\t\t\t\t\t\tPHONE     : ");
        fflush(stdin);
        gets(a.num);
        printf("\t\t\t\t\t\t\tPASSWORD  : ");
        fflush(stdin);
        gets(a.pass);
        if(strlen(a.name)==0 || strlen(a.mail)==0 || strlen(a.num)==0 || strlen(a.pass)== 0)
        {
            printf("\n\t\t\t\t\t\t\tINVALID INPUT.ENTER YOUR |");
        }
        if(strlen(a.name)==0)
        {
            printf(" NAME |");
        }
        if(strlen(a.mail)==0)
        {
            printf(" EMAIL |");
        }
        if(strlen(a.num)==0)
        {
            printf(" PHONE NUMBER |");
        }
        if(strlen(a.pass)== 0)
        {
            printf(" PASSWORD |");
        }
        if(strlen(a.name)==0 || strlen(a.mail)==0 || strlen(a.num)==0 || strlen(a.pass)== 0)
        {
            printf(" CORRECTLY AND TRY AGAIN.");
            printf("\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE........");

            fflush(stdin);   //fflush is used for flushing the output buffer of a stream
            getchar();       //getchar() function is used to wait to read a single character
            system("cls");   //console clear
            admin_login();

        }
        fp = fopen("admin.txt", "ab");
        fwrite(&a, sizeof(a), 1, fp);
        fclose(fp);
        a_count++;

        //copying data to linklist
        while(1)
        {
            node=(struct admin*)malloc(sizeof(struct admin));
            if (node == NULL)
            {
                continue;
            }
            else
            {
                strcpy(node->id, a.id);
                strcpy(node->name, a.name);
                strcpy(node->mail, a.mail);
                strcpy(node->num, a.num);
                strcpy(node->pass, a.pass);
                node->next = NULL;
                a_head=node;
            }
            break;
        }
        printf("\n\n\t\t\t\t\t\t\tLIBRARIAN ADDED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        admin_login();

    }
    else
    {

        printf("\t\t\t\t\t\t\tID number: ");
        fflush(stdin);
        gets(id);
        printf("\n\t\t\t\t\t\t\tPassword : ");
        fflush(stdin);
        gets(pass);
        a_temp=a_head;
        while(a_temp != NULL)
        {
            if (strcmp(id,a_temp->id) == 0)
            {
                if(strcmp(pass, a_temp->pass) == 0)
                {
                    match=1;
                }
            }
            a_temp=a_temp->next;

        }
        if(match==1)
        {
            a_temp=NULL;
            strcpy(a_id,id);
            admin_menu();
        }
        else
        {
            printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
            fflush(stdin);// Clear the input buffer
            getchar(); // Wait for Enter key
            system("cls");
            mainmenu();
        }
    }
}
void admin_menu()
{
    int ch,p,m=0,ko;
    system("cls");
    printf("\t\t\t\t\t\t\t=================================================================\n");
    printf("\t\t\t\t\t\t\t|                       ****************                        |\n");
    printf("\t\t\t\t\t\t\t|                       *  ADMIN MENU  *                        |\n");
    printf("\t\t\t\t\t\t\t|                       ****************                        |\n");
    printf("\t\t\t\t\t\t\t=================================================================\n");
    printf("\t\t\t\t\t\t\t1.TRACK BORROWED BOOKS\n");
    printf("\t\t\t\t\t\t\t2.BOOK AVAILABILITY\n");
    printf("\t\t\t\t\t\t\t3.BOOK REQUEST LIST\n");
    printf("\t\t\t\t\t\t\t4.ADD NEW BOOK\n");
    printf("\t\t\t\t\t\t\t5.REMOVE A BOOK\n");
    printf("\t\t\t\t\t\t\t6.VIEW MEMBER LIST\n");
    printf("\t\t\t\t\t\t\t7.VIEW LIBRARIAN LIST\n");
    printf("\t\t\t\t\t\t\t8.TERMINATE A MEMBER\n");
    char rem_name[100];
    if(strcmp(a_id, a_head->id) == 0)
    {
        printf("\t\t\t\t\t\t\t9.UPDATE LIBRARIAN LIST\n");
        printf("\t\t\t\t\t\t\t10.LOG OUT\n");
        printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 to 10): ");
    }
    else if(strcmp(a_id, a_head->next->id) == 0)
    {
        printf("\t\t\t\t\t\t\t9.CHANGE HEAD LIBRARIAN\n");
        printf("\t\t\t\t\t\t\t10.LOG OUT\n");
        printf("\n\t\t\t\t\t\tPlease Enter your choice (1 to 10): ");
    }
    else
    {
        printf("\t\t\t\t\t\t\t9.LOG OUT\n");
        printf("\n\t\t\t\t\t\tPlease Enter your choice (1 to 9): ");
    }
    scanf("%d", &ch);
    switch(ch)
    {

    case 1:
        system("cls");
        track_book();
        printf("\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE......");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();
        admin_menu();
        break;

    case 2:
        system("cls");
        //fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        //getchar();
        display_books_from_file(1);
        break;

    case 3:
        system("cls");
        display_requested_books();
        break;

    case 4:
        printf("\n\t\t\t\t\t\t\tHOW MANY BOOKS YOU WANT TO ENTER? ");
        scanf("%d",&ko);
        system("cls");
        add_book(ko);
        break;

    case 5:
        system("cls");
        printf("\t\t\t\t\t\t\tEnter BOOK NAME: ");
        getchar();
        scanf("%[^\n]s",rem_name);
        search_book_by_name(rem_name,1);
        break;
    case 6:
        system("cls");
        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
        printf("\t\t\t\t\t\t\t|                         MEMBER LIST                           |\n");
        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n\n");
        if(m_count == 0)
        {
            printf("\t\t\t\t\t\t\tCURRENTLY WE DON'T HAVE ANY MEMBER.PRESS ENTER TO RETURN...");
            fflush(stdin);// Clear the input buffer
            getchar(); // Wait for Enter key
            system("cls");
            admin_menu();
        }
        else
        {
            printf("\t\t\t\t\t\t\tCURRENTLY WE HAVE %d MEMBER IN OUR LIBRARY.",m_count);
            m_temp=m_head;
            while(m_temp != NULL)
            {
                printf("\n\n\t\t\t\t\t\t\tID NUMBER : %s",m_temp->id);
                printf("\n\t\t\t\t\t\t\tNAME      : %s",m_temp->name);
                //printf("\n\t\t\t\t\t\t\tPHONE NUM : %s",m_temp->num);
                printf("\n\t\t\t\t\t\t\tEMAIL     : %s",m_temp->mail);
                m_temp=m_temp->next;
            }
            m_temp=NULL;
            printf("\n\n\n\t\t\t\t\t\t\tPress ENTER to return...");
            fflush(stdin);// Clear the input buffer
            getchar(); // Wait for Enter key
            system("cls");
            admin_menu();

        }
        break;

    case 7:
        system("cls");
        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
        printf("\t\t\t\t\t\t\t|                        LIBRARIAN LIST                         |\n");
        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n\n");
        printf("\t\t\t\t\t\t\tHEAD LIBRARIAN :");
        printf("\n\n\t\t\t\t\t\t\tID NUMBER : %s",a_head->id);
        printf("\n\t\t\t\t\t\t\tNAME      : %s",a_head->name);
        //printf("\n\t\t\t\t\t\t\tPHONE NUM : %s",a_head->num);
        printf("\n\t\t\t\t\t\t\tEMAIL     : %s",a_head->mail);
        if(a_head->next != NULL)
        {
            printf("\n\n\t\t\t\t\t\t\tSENIOR LIBRARIAN OFFICER :");
            printf("\n\n\t\t\t\t\t\t\tID NUMBER : %s",a_head->next->id);
            printf("\n\t\t\t\t\t\t\tNAME      : %s",a_head->next->name);
            //printf("\n\t\t\t\t\t\t\tPHONE NUM : %s",a_head->next->num);
            printf("\n\t\t\t\t\t\t\tEMAIL     : %s",a_head->next->mail);

            if(a_head->next->next != NULL)
            {
                printf("\n\n\t\t\t\t\t\t\tLIBRARIAN OFFICERS :");
            }
            a_temp=a_head->next->next;
            while(a_temp != NULL)
            {
                printf("\n\n\t\t\t\t\t\t\tID NUMBER : %s",a_temp->id);
                printf("\n\t\t\t\t\t\t\tNAME      : %s",a_temp->name);
                //printf("\n\t\t\t\t\t\t\tPHONE NUM : %s",a_temp->num);
                printf("\n\t\t\t\t\t\t\tEMAIL     : %s",a_temp->mail);
                a_temp=a_temp->next;

            }
        }

        a_temp=NULL;
        printf("\n\n\n\t\t\t\t\t\t\tPress ENTER to return...");
        fflush(stdin);// Clear the input buffer
        getchar(); // Wait for Enter key
        system("cls");
        admin_menu();
        break;

    case 8:
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                       UPDATE MEMBER LIST                      |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n\n");
            int ter=0;
            printf("\t\t\t\t\t\t\tTERMINATE A MEMBER........ \n\n");
            printf("\t\t\t\t\t\t\tFIND ID :");
            fflush(stdin);
            gets(s.id);
            m_temp=m_head;
            while(m_temp != NULL)
            {
                if (strcmp(s.id,m_temp->id) == 0)
                {
                    ter++;
                }
                m_temp=m_temp->next;

            }
            m_temp=NULL;
            if(ter == 0)

            {
                printf("\n\n\t\t\t\t\t\t\tNO MATCHED ID FOUND. Please,Enter Correct ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                continue;

            }
            else
            {
                if (strcmp(s.id,m_head->id) == 0)
                {
                    m_temp=m_head;
                    m_head=m_head->next;
                    free(m_temp);
                    m_temp=NULL;
                }
                else
                {
                    m_temp=m_head;
                    while(m_temp != NULL)
                    {
                        if(strcmp(m_temp->next->id,s.id) == 0)
                        {
                            s_temp=m_temp->next;
                            m_temp->next=m_temp->next->next;
                            free(s_temp);
                            s_temp=NULL;
                            m_temp=NULL;
                            break;
                        }
                        m_temp=m_temp->next;
                    }

                }
                m_temp=m_head;
                ft = fopen("user.txt", "wb");
                while(m_temp != NULL)
                {
                    strcpy(s.id, m_temp->id);
                    strcpy(s.name, m_temp->name);
                    strcpy(s.mail, m_temp->mail);
                    strcpy(s.num, m_temp->num);
                    strcpy(s.pass, m_temp->pass);
                    fwrite(&s, sizeof(s), 1, ft);
                    m_temp=m_temp->next;
                }
                fclose(ft);
                m_temp=NULL;
                m_count--;
                printf("\n\n\t\t\t\t\t\t\tMEMBER TERMINATED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
                fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                getchar();       //getchar() function is used to wait to read a single character
                system("cls");   //console clear
                admin_menu();

            }
            break;
        }
        break;

    case 9:
        if(strcmp(a_id, a_head->id) == 0)
        {
            while(1)
            {
                system("cls");
                printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                printf("\t\t\t\t\t\t\t|                                                               |\n");
                printf("\t\t\t\t\t\t\t|                      UPDATE LIBRARIAN LIST                    |\n");
                printf("\t\t\t\t\t\t\t|                                                               |\n");
                printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                printf("\n\t\t\t\t\t\t\t1.APPOINT A NEW LIBRARIAN");
                printf("\n\t\t\t\t\t\t\t2.TERMINATE A LIBRARIAN");
                printf("\n\t\t\t\t\t\t\t3.MAINMENU");
                printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 to 3): ");
                scanf("%d",&p);
                if(p == 1 || p == 2 || p== 3)
                {
                    break;
                }
                else
                {
                    printf("\n\t\t\t\t\t\t\tInvalid input!!! Press ENTER to try again......");
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();       //getchar() function is used to wait to read a single character
                    continue;
                }
            }
            if(p==1)
            {
                while(1)
                {
                    int c=0;
                    system("cls");
                    printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                    printf("\t\t\t\t\t\t\t|                                                               |\n");
                    printf("\t\t\t\t\t\t\t|                      UPDATE LIBRARIAN LIST                    |\n");
                    printf("\t\t\t\t\t\t\t|                                                               |\n");
                    printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                    printf("\n\t\t\t\t\t\t\t1.CONFIRM TO APPOINT A NEW LIBRARIAN");
                    printf("\n\t\t\t\t\t\t\t2.MAINMENU");
                    printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 OR 2): ");
                    scanf("%d",&c);
                    if(c == 1)
                    {
                        update_admin(1);
                        break;
                    }
                    else if(c==2)
                    {
                        admin_menu();
                        break;
                    }
                    else
                    {
                        printf("\n\t\t\t\t\t\tInvalid input!!! Press ENTER to try again......");
                        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                        getchar();       //getchar() function is used to wait to read a single character
                        continue;
                    }
                }
            }
            if(p==2)
            {
                if(a_count<2)
                {
                    system("cls");
                    printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                    printf("\t\t\t\t\t\t\t|                                                               |\n");
                    printf("\t\t\t\t\t\t\t|                      UPDATE LIBRARIAN LIST                    |\n");
                    printf("\t\t\t\t\t\t\t|                                                               |\n");
                    printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                    printf("\n\t\t\t\t\t\t\tNO LIBRARIAN LEFT TO TERMINATE.PRESS ENTER TO CONTINUE......");
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();//getchar() function is used to wait to read a single character
                    admin_menu();
                }
                else
                {
                    while(1)
                    {
                        int c=0;
                        system("cls");
                        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                        printf("\t\t\t\t\t\t\t|                                                               |\n");
                        printf("\t\t\t\t\t\t\t|                      UPDATE LIBRARIAN LIST                    |\n");
                        printf("\t\t\t\t\t\t\t|                                                               |\n");
                        printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                        printf("\n\t\t\t\t\t\t\t1.CONFIRM TO TERMINATE A NEW LIBRARIAN");
                        printf("\n\t\t\t\t\t\t\t2.MAINMENU");
                        printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 OR 2): ");
                        scanf("%d",&c);
                        if(c == 1)
                        {
                            update_admin(2);
                            break;
                        }
                        else if(c==2)
                        {
                            admin_menu();
                            break;
                        }
                        else
                        {
                            printf("\n\t\t\t\t\t\t\tInvalid input!!! Press ENTER to try again......");
                            fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                            getchar();       //getchar() function is used to wait to read a single character
                            continue;
                        }
                    }
                }
            }
            if (p == 3)
            {
                admin_menu();
            }

        }
        else if(strcmp(a_id, a_head->next->id) == 0)
        {
            while(1)
            {
                system("cls");
                printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                printf("\t\t\t\t\t\t\t|                                                               |\n");
                printf("\t\t\t\t\t\t\t|                      CHANGE HEAD LIBRARIAN                    |\n");
                printf("\t\t\t\t\t\t\t|                                                               |\n");
                printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
                printf("\n\t\t\t\t\t\t\t1.CONFIRM TO CHANGE HEAD LIBRARIAN");
                printf("\n\t\t\t\t\t\t\t2.MAINMENU");
                printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 OR 2): ");
                scanf("%d",&p);
                if(a_count <= 2)
                {
                    printf("\n\n\t\t\t\t\t\t\tYOU CAN NOT CHANGE HEAD LIBRARIAN.PRESS ENTER TO CONTINUE......");
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();
                    admin_menu();
                }
                else if(p == 1)
                {
                    update_admin(63);
                    break;
                }
                else if (p == 2)
                {
                    admin_menu();
                    break;
                }
                else
                {
                    printf("\n\t\t\t\t\t\t\tInvalid input!!! Press ENTER to try again......");
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();       //getchar() function is used to wait to read a single character
                    continue;
                }
            }
        }
        else
        {
            a_temp=a_head->next->next;
            while(a_temp != NULL)
            {
                if(strcmp(a_id, a_temp->id) == 0)
                {
                    m=1;
                }
                a_temp=a_temp->next;
            }
            if(m==1)
            {
                a_temp=NULL;
                mainmenu();
            }
            else
            {
                printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                admin_menu();

            }

        }

        break;

    case 10:
        if(strcmp(a_id, a_head->id) == 0 || strcmp(a_id, a_head->next->id) == 0)
        {
            mainmenu();
        }
        else
        {
            printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
            fflush(stdin);// Clear the input buffer
            getchar(); // Wait for Enter key
            system("cls");
            admin_menu();
        }

        break;

    default:
        printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
        fflush(stdin);// Clear the input buffer
        getchar(); // Wait for Enter key
        system("cls");
        admin_menu();

    }
}
void update_admin(int x)
{
    if(x == 63)
    {
        int count=0;
        a_temp=a_head->next->next;
        while(a_temp != NULL)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
            printf("\t\t\t\t\t\t\t|---------------------------------------------------------------|\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                     ***UPDATE LIBRARIAN***                    |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            printf("\n\t\t\t\t\t\t\tEVERYONE HAVE  TO INPUT THEIR PASSWORD........");
            printf("\n\n\t\t\t\t\t\t\tID   : %s",a_temp->id);
            printf("\n\t\t\t\t\t\t\tNAME : %s",a_temp->name);
            printf("\n\t\t\t\t\t\t\tPASS : ");
            fflush(stdin);
            gets(a.pass);
            if(strcmp(a.pass, a_temp->pass) == 0)
            {
                a_temp=a_temp->next;
                count=0;
                continue;
            }
            else
            {
                count++;
                if(count>2)
                {
                    printf("\n\n\t\t\t\t\t\t\tLIBRARIAN CAN NOT BE UPDATED.PRESS ENTER TO CONTINUE........");
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();       //getchar() function is used to wait to read a single character
                    system("cls");   //console clear
                    a_temp=NULL;
                    count=0;
                    admin_menu();
                }
                else
                {
                    printf("\n\n\t\t\t\t\t\t\tWORNG PASSWORD. %d TRY LEFT.\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE........",(3-count));
                    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                    getchar();       //getchar() function is used to wait to read a single character
                    system("cls");   //console clear
                    continue;
                }
            }
        }
        a_temp=a_head;
        a_head=a_head->next;
        free(a_temp);
        a_count--;
        a_temp=a_head;
        fp = fopen("admin.txt", "wb");
        while(a_temp != NULL)
        {
            strcpy(a.id, a_temp->id);
            strcpy(a.name, a_temp->name);
            strcpy(a.mail, a_temp->mail);
            strcpy(a.num, a_temp->num);
            strcpy(a.pass, a_temp->pass);
            fwrite(&a, sizeof(a), 1, fp);
            a_temp=a_temp->next;
        }
        fclose(fp);
        a_temp=NULL;
        printf("\n\n\t\t\t\t\t\t\tLIBRARIAN UPDATED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        admin_menu();
    }
    else if(x == 1)
    {
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                        UPDATE LIBRARIAN                       |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            int count=0,matched=0;
            printf("\t\t\t\t\t\t\tADD A NEW LIBRARIAN........ \n\n");
            printf("\t\t\t\t\t\t\tID NUMBER : ");
            fflush(stdin);
            gets(a.id);
            for(int i = 0; a.id[i] != '\0'; i++)
            {
                if (a.id[i] >= '0' && a.id[i] <= '9')
                {
                    count++;
                }
                else
                {
                    printf("\n\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to try again...");
                    fflush(stdin);// Clear the input buffer
                    getchar(); // Wait for Enter key
                    continue;
                }
            }
            if(count != 8)
            {
                printf("\n\n\t\t\t\t\t\t\tThis ID is INVALID. Please,Enter Correct ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                continue;
            }
            a_temp=a_head;
            while(a_temp != NULL)
            {
                if (strcmp(a.id,a_temp->id) == 0)
                {
                    matched++;
                }
                a_temp=a_temp->next;
            }
            a_temp=NULL;
            if(matched > 0)
            {
                printf("\n\n\t\t\t\t\t\t\tThis ID is Already Taken. Please,Enter Correct ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                continue;
            }
            printf("\t\t\t\t\t\t\tNAME      : ");
            fflush(stdin);
            gets(a.name);
            printf("\t\t\t\t\t\t\tEMAIL     : ");
            fflush(stdin);
            gets(a.mail);
            printf("\t\t\t\t\t\t\tPHONE NUM : ");
            fflush(stdin);
            gets(a.num);
            printf("\t\t\t\t\t\t\tPASSWORD  : ");
            fflush(stdin);
            gets(a.pass);
            if(strlen(a.name)==0 || strlen(a.mail)==0 || strlen(a.num)==0 || strlen(a.pass)== 0)
            {
                printf("\n\t\t\t\t\t\t\tINVALID INPUT.ENTER YOUR |");
            }
            if(strlen(a.name)==0)
            {
                printf(" NAME |");
            }
            if(strlen(a.mail)==0)
            {
                printf(" EMAIL |");
            }
            if(strlen(a.num)==0)
            {
                printf(" PHONE NUMBER |");
            }
            if(strlen(a.pass)== 0)
            {
                printf(" PASSWORD |");
            }
            if(strlen(a.name)==0 || strlen(a.mail)==0 || strlen(a.num)==0 || strlen(a.pass)== 0)
            {
                printf(" CORRECTLY AND TRY AGAIN.");
                printf("\n\t\t\t\t\t\tPRESS ENTER TO CONTINUE........");

                fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                getchar();       //getchar() function is used to wait to read a single character
                continue;
            }
            break;
        }
        fp = fopen("admin.txt", "ab");
        fwrite(&a, sizeof(a), 1, fp);
        fclose(fp);
        a_count++;
        //copying data to linklist
        while(1)
        {
            node=(struct admin*)malloc(sizeof(struct admin));
            if (node == NULL)
            {
                continue;
            }
            else
            {
                strcpy(node->id, a.id);
                strcpy(node->name, a.name);
                strcpy(node->mail, a.mail);
                strcpy(node->num, a.num);
                strcpy(node->pass, a.pass);
                node->next = NULL;
                a_temp=a_head;
                while(a_temp->next !=NULL)
                {
                    a_temp=a_temp->next;
                }
                a_temp->next=node;
                a_temp=NULL;
            }
            break;
        }
        printf("\n\n\t\t\t\t\t\t\tLIBRARIAN ADDED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        admin_menu();
    }
    else if(x == 2)
    {
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                        UPDATE LIBRARIAN                       |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t-----------------------------------------------------------------\n");
            int ter=0;
            printf("\t\t\t\t\t\t\tTERMINATE A LIBRARIAN........ \n\n");
            printf("\t\t\t\t\t\t\tFIND ID :");
            fflush(stdin);
            gets(f.id);
            a_temp=a_head;
            while(a_temp != NULL)
            {
                if (strcmp(f.id,a_temp->id) == 0)
                {
                    ter++;
                }
                a_temp=a_temp->next;
            }
            a_temp=NULL;
            if(ter == 0)

            {
                printf("\n\n\t\t\t\t\t\t\tNO MATCHED ID FOUND. Please,Enter Correct ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                continue;
            }
            break;
        }
        a_temp=a_head;
        while(a_temp != NULL)
        {
            if(strcmp(a_temp->next->id,f.id) == 0)
            {
                b_temp=a_temp->next;
                a_temp->next=a_temp->next->next;
                free(b_temp);
                b_temp=NULL;
                a_temp=NULL;
                break;
            }
            a_temp=a_temp->next;
        }
        a_temp=a_head;
        fp = fopen("admin.txt", "wb");
        while(a_temp != NULL)
        {
            strcpy(a.id, a_temp->id);
            strcpy(a.name, a_temp->name);
            strcpy(a.mail, a_temp->mail);
            strcpy(a.num, a_temp->num);
            strcpy(a.pass, a_temp->pass);
            fwrite(&a, sizeof(a), 1, fp);
            a_temp=a_temp->next;
        }
        fclose(fp);
        a_temp=NULL;
        a_count--;
        printf("\n\n\t\t\t\t\t\t\tLIBRARIAN TERMINATED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        admin_menu();
    }
}
void load_members()
{
    ft = fopen("user.txt", "r");
    if (ft == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (1)
    {
        mnode = (struct member *)malloc(sizeof(struct member));
        if (mnode == NULL)
        {
            continue;
        }

        if (fscanf(ft, "ID: %s\nName: %s\nMail: %s\nNumber: %s\nPassword: %s\n", mnode->id, mnode->name, mnode->mail, mnode->num, mnode->pass) != 5)
        {
            free(mnode);
            break;
        }

        mnode->next = NULL;
        if (m_head == NULL)
        {
            m_head = mnode;
        }
        else
        {
            m_temp = m_head;
            while (m_temp->next != NULL)
            {
                m_temp = m_temp->next;
            }
            m_temp->next = mnode;
        }
        m_count++;
    }
    fclose(ft);
}
void update_user(int x)
{
    if(x==1)
    {
        system("cls");
        printf("\t\t\t\t\t\t\t=================================================================\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t|                    UPDATE YOUR INFORMATION                    |\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t=================================================================\n\n");
        m_temp=m_head;
        while(m_temp != NULL)
        {
            if(strcmp(m_id, m_temp->id) == 0)
            {
                while(1)
                {

                    printf("\t\t\t\t\t\t\tNAME      : ");
                    fflush(stdin);
                    gets(m.name);

                    printf("\t\t\t\t\t\t\tEMAIL     : ");
                    fflush(stdin);
                    gets(m.mail);

                    printf("\t\t\t\t\t\t\tPHONE     : ");
                    fflush(stdin);
                    gets(m.num);
                    printf("\t\t\t\t\t\t\tPASSWORD  : ");
                    fflush(stdin);
                    gets(m.pass);
                    if(strlen(m.name)==0 || strlen(m.mail)==0 || strlen(m.num)==0 || strlen(m.pass)== 0)
                    {
                        printf("\n\t\t\t\t\t\t\tINVALID INPUT.ENTER YOUR |");
                    }
                    if(strlen(m.name)==0)
                    {
                        printf(" NAME |");
                    }
                    if(strlen(m.mail)==0)
                    {
                        printf(" EMAIL |");
                    }
                    if(strlen(m.num)==0)
                    {
                        printf(" PHONE NUMBER |");
                    }
                    if(strlen(a.pass)== 0)
                    {
                        printf(" PASSWORD |");
                    }
                    if(strlen(m.name)==0 || strlen(m.mail)==0 || strlen(m.num)==0 || strlen(m.pass)== 0)
                    {
                        printf("\n\t\t\t\t\t\t\tCORRECTLY AND TRY AGAIN.");
                        printf("\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE........");

                        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                        getchar();       //getchar() function is used to wait to read a single character
                        system("cls");   //console clear
                        update_user(1);

                    }
                    break;
                }
                strcpy(m_temp->name, m.name);
                strcpy(m_temp->mail, m.mail);
                strcpy(m_temp->num, m.num);
                strcpy(m_temp->pass, m.pass);

            }
            m_temp=m_temp->next;
        }
        m_temp=m_head;
        ft = fopen("user.txt", "wb");
        while(m_temp != NULL)
        {
            strcpy(m.id, m_temp->id);
            strcpy(m.name, m_temp->name);
            strcpy(m.mail, m_temp->mail);
            strcpy(m.num, m_temp->num);
            strcpy(m.pass, m_temp->pass);
            fwrite(&m, sizeof(m), 1, ft);
            m_temp=m_temp->next;


        }
        fclose(ft);
        m_temp=NULL;
        system("cls");
        printf("\t\t\t\t\t\t\t=================================================================\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t|                    UPDATE YOUR INFORMATION                    |\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t=================================================================\n\n");
        printf("\n\n\t\t\t\t\t\t\tYOUR INFORMATION SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        user_menu();



    }
    else if(x==2)
    {
        m_temp=m_head;
        while(m_temp != NULL)
        {
            if (strcmp(m_id,m_head->id) == 0)
            {
                s_temp=m_head;
                m_head=m_head->next;
                free(s_temp);
                s_temp=NULL;
                m_temp=NULL;
                m_count--;
                break;
            }
            else if(strcmp(m_temp->next->id,m_id) == 0)
            {
                s_temp=m_temp->next;
                m_temp->next=m_temp->next->next;
                free(s_temp);
                s_temp=NULL;
                m_temp=NULL;
                m_count--;
                break;
            }
        }
        m_temp=m_head;
        ft = fopen("user.txt", "wb");
        while(m_temp != NULL)
        {
            strcpy(m.id, m_temp->id);
            strcpy(m.name, m_temp->name);
            strcpy(m.mail, m_temp->mail);
            strcpy(m.num, m_temp->num);
            strcpy(m.pass, m_temp->pass);
            fwrite(&m, sizeof(m), 1, ft);
            m_temp=m_temp->next;
        }
        fclose(ft);
        m_temp=NULL;
        system("cls");
        printf("\t\t\t\t\t\t\t=================================================================\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t|                   ***MEMBERSHIP CANCELLATION***               |\n");
        printf("\t\t\t\t\t\t\t|                                                               |\n");
        printf("\t\t\t\t\t\t\t=================================================================\n");
        printf("\n\n\t\t\t\t\t\t\tCANCELLEATTION PROCESS COMPLETED SUCESSFULLY.PRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        mainmenu();
    }
}
void database()
{
    if(a_count == 0)
    {
        fp = fopen("admin.txt", "rb");
        a_head=NULL;
        while(fread(&a, sizeof(a), 1, fp) == 1)
        {
            while(1)
            {
                node=(struct admin*)malloc(sizeof(struct admin));
                //node=(struct admin*)malloc(sizeof(struct admin));
                if(node == NULL)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            strcpy(node->id, a.id);
            strcpy(node->name, a.name);
            strcpy(node->mail, a.mail);
            strcpy(node->num, a.num);
            strcpy(node->pass, a.pass);
            node->next = NULL;
            if(a_head == NULL)
            {
                a_head=node;
                a_count++;
            }
            else
            {
                a_temp=a_head;
                while(a_temp->next != NULL)
                {
                    a_temp=a_temp->next;
                }
                a_temp->next=node;
                a_temp=NULL;
                a_count++;
            }
        }
        fclose(fp);
    }
    if(m_count == 0)
    {
        ft = fopen("user.txt", "rb");
        m_head=NULL;
        while(fread(&m, sizeof(m), 1, ft) == 1)
        {
            while(1)
            {
                mnode=(struct member*)malloc(sizeof(struct member));
                if(mnode == NULL)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            strcpy(mnode->id, m.id);
            strcpy(mnode->name, m.name);
            strcpy(mnode->mail, m.mail);
            strcpy(mnode->num, m.num);
            strcpy(mnode->pass, m.pass);
            mnode->next = NULL;
            if(m_head == NULL)
            {
                m_head=mnode;
                m_count++;
            }
            else
            {
                m_temp=m_head;
                while(m_temp->next != NULL)
                {
                    m_temp=m_temp->next;
                }
                m_temp->next=mnode;
                m_temp=NULL;
                m_count++;
            }

        }
        fclose(ft);
    }
}

void user_login()
{
    int choice;
    if(mt_count == 0)
    {
        system("cls");

        printf("\t\t\t\t\t\t\t<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printf("\t\t\t\t\t\t\t|                                                                 |\n");
        printf("\t\t\t\t\t\t\t|                        MEMBER LOGIN PAGE                        |\n");
        printf("\t\t\t\t\t\t\t|                                                                 |\n");
        printf("\t\t\t\t\t\t\t===================================================================\n");
        printf("\t\t\t\t\t\t\t|                      |                 |                        |\n");
        printf("\t\t\t\t\t\t\t|      1. LOG IN       |     2. SIGN UP  |   3. MANMENU           |\n");
        printf("\t\t\t\t\t\t\t|                      |                 |                        |\n");
        printf("\t\t\t\t\t\t\t<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printf("\n\t\t\t\t\t\t\tEnter your choice: ");
        scanf("%d",&choice);
    }
    if(choice == 1 || mt_count == 1)
    {
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t#################################################################\n");
            printf("\t\t\t\t\t\t\t#                                                               #\n");
            printf("\t\t\t\t\t\t\t#                       ***LOG IN PAGE****                      #\n");
            printf("\t\t\t\t\t\t\t#                                                               #\n");
            printf("\t\t\t\t\t\t\t#################################################################\n");
            int match=0;
            char id[8];
            char pass[50];
            printf("\n\t\t\t\t\t\t\tINPUT YOUR ID OR PRESS ENTER TO RETURN .........\n\n");
            printf("\t\t\t\t\t\t\tID number  : ");
            fflush(stdin);
            gets(id);
            if(strlen(id) == 0)
            {
               user_login();
            }
            printf("\n\t\t\t\t\t\t\tPassword : ");
            fflush(stdin);
            gets(pass);
            m_temp=m_head;
            while(m_temp != NULL)
            {
                if (strcmp(id,m_temp->id) == 0)
                {
                    if(strcmp(pass, m_temp->pass) == 0)
                    {
                        match=1;
                    }
                }
                m_temp=m_temp->next;

            }
            if(match==1)
            {
                m_temp=NULL;
                strcpy(m_id,id);
                strcpy(user_ID,id);
                mt_count=0;
                user_menu();

                break;
            }
            else
            {
                printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                mt_count=1;
                continue;

            }
        }

    }
    else if(choice == 2 || mt_count == 2)
    {
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t*****************************************************************\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                         SIGN UP PAGE                          |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t*****************************************************************\n");
            printf("\n\t\t\t\t\t\t\tINPUT YOUR INFORMATION OR PRESS ENTER TO RETURN .........\n\n");

            int count=0,match=0;
            printf("\n\t\t\t\t\t\t\tID NUMBER : ");
            fflush(stdin);
            gets(m.id);
            if(strlen(m.id) == 0)
            {
               user_login();
            }
            for(int i = 0; m.id[i] != '\0'; i++)
            {
                if (m.id[i] >= '0' && m.id[i] <= '9')
                {
                    count++;
                }
                else
                {
                    printf("\n\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to try again...");
                    fflush(stdin);// Clear the input buffer
                    getchar(); // Wait for Enter key
                    system("cls");
                    mt_count=2;
                    user_login();
                }
            }
            if(count != 8)
            {
                printf("\n\n\t\t\t\t\t\t\tThis ID is INVALID. Please,Enter Correct ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                continue;
            }
            m_temp=m_head;
            while(m_temp != NULL)
            {
                if (strcmp(m.id,m_temp->id) == 0)
                {
                    match=1;
                }
                m_temp=m_temp->next;

            }
            m_temp=NULL;
            if(match==1)
            {
                printf("\n\n\t\t\t\t\t\t\tTHIS ID IS ALREADY TAKEN.PLEASE,ENTER CORRECT ID...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                match=0;
                continue;
            }

            printf("\t\t\t\t\t\t\tNAME      : ");
            fflush(stdin);
            gets(m.name);

            printf("\t\t\t\t\t\t\tEMAIL     : ");
            fflush(stdin);
            gets(m.mail);

            printf("\t\t\t\t\t\t\tPHONE     : ");
            fflush(stdin);
            gets(m.num);
            printf("\t\t\t\t\t\t\tPASSWORD  : ");
            fflush(stdin);
            gets(m.pass);
            if(strlen(m.name)==0 || strlen(m.mail)==0 || strlen(m.num)==0 || strlen(m.pass)== 0)
            {
                printf("\n\t\t\t\t\t\t\tINVALID INPUT.ENTER YOUR |");
            }
            if(strlen(m.name)==0)
            {
                printf(" NAME |");
            }
            if(strlen(m.mail)==0)
            {
                printf(" EMAIL |");
            }
            if(strlen(m.num)==0)
            {
                printf(" PHONE NUMBER |");
            }
            if(strlen(m.pass)== 0)
            {
                printf(" PASSWORD |");
            }
            if(strlen(m.name)==0 || strlen(m.mail)==0 || strlen(m.num)==0 || strlen(m.pass)== 0)
            {
                printf("\n\t\t\t\t\t\t\tCORRECTLY AND TRY AGAIN.");
                printf("\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE........");

                fflush(stdin);   //fflush is used for flushing the output buffer of a stream
                getchar();       //getchar() function is used to wait to read a single character
                system("cls");   //console clear
                continue;

            }
            break;
        }


        ft = fopen("user.txt", "ab");
        fwrite(&m, sizeof(m), 1, ft);
        fclose(ft);
        m_count++;
        mt_count=0;

        //copying data to linklist
        while(1)
        {
            mnode=(struct member*)malloc(sizeof(struct member));
            if (mnode == NULL)
            {
                continue;
            }
            else
            {
                strcpy(mnode->id, m.id);
                strcpy(mnode->name, m.name);
                strcpy(mnode->mail, m.mail);
                strcpy(mnode->num, m.num);
                strcpy(mnode->pass, m.pass);
                mnode->next = NULL;
                if(m_head == NULL)
                {
                    m_head=mnode;
                }
                else
                {
                    m_temp=m_head;
                    while(m_temp->next != NULL)
                    {
                        m_temp=m_temp->next;
                    }
                    m_temp->next=mnode;
                }
            }
            break;
        }
        printf("\n\n\t\t\t\t\t\t\tYOUR INFO ADDED SUCESSFULLY.YOU ARE NOW A MEMBER  OF OUR LIBRARY.\n\n\t\t\t\t\t\t\tPRESS ENTER TO CONTINUE........");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        user_login();

    }
    else if(choice == 3)
    {
     mainmenu();
    }
    else
    {
        printf("\n\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to try again...");
        fflush(stdin);// Clear the input buffer
        getchar(); // Wait for Enter key
        system("cls");
        user_login();
    }
}

void user_menu()
{
    system("cls");
    printf("\t\t\t\t\t\t\t=================================================================\n");
    printf("\t\t\t\t\t\t\t|***************************************************************|\n");
    printf("\t\t\t\t\t\t\t|                            ***MENU***                         |\n");
    printf("\t\t\t\t\t\t\t|***************************************************************|\n");
    printf("\t\t\t\t\t\t\t=================================================================\n\n");
    int ch;
    char search_name[100];
    printf("\t\t\t\t\t\t\t1.BOOK LIST\n");
    printf("\t\t\t\t\t\t\t2.SEARCH A BOOK\n");
    printf("\t\t\t\t\t\t\t3.BORROW A BOOK\n");
    printf("\t\t\t\t\t\t\t4.REQUEST FOR A BOOK\n");
    printf("\t\t\t\t\t\t\t5.UPDATE YOUR DETAILS\n");
    printf("\t\t\t\t\t\t\t6.CANCEL MEMBERSHIP\n");
    printf("\t\t\t\t\t\t\t7.LOG OUT\n");
    printf("\t\t\t\t\t\t\tEnter your choice (1-7): ");
    scanf("%d",&ch);
    switch(ch)
    {
    case 1:
        system("cls");
        display_books_from_file(2);;
        break;

    case 2:
        system("cls");
        getchar();
        printf("\t\t\t\t\t\t\tEnter Book Name : ");
        scanf("%[^\n]s",&search_name);
        search_book_by_name(search_name,2);
        break;

    case 3:
        system("cls");
        borrow_book();
        break;

    case 4:
        system("cls");
        request_book();
        break;

    case 5:
        update_user(1);
        user_menu();
        break;
    case 6:
        while(1)
        {
            system("cls");
            printf("\t\t\t\t\t\t\t*****************************************************************\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t|                 ___MEMBERSHIP CANCELLATION___                 |\n");
            printf("\t\t\t\t\t\t\t|                                                               |\n");
            printf("\t\t\t\t\t\t\t*****************************************************************\n");
            int x;
            printf("\t\t\t\t\t\t\t1.CONFIRM YOUR CANCELLATION\n");
            printf("\t\t\t\t\t\t\t2.MAINMENU");
            printf("\n\t\t\t\t\t\t\tPlease Enter your choice (1 or 2): ");
            scanf("%d",&x);
            if(x==1)
            {
                update_user(2);

            }
            else if(x==2)
            {
                user_menu();
            }
            else
            {
                printf("\n\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
                fflush(stdin);// Clear the input buffer
                getchar(); // Wait for Enter key
                system("cls");
                continue;
            }
        }

        break;
    case 7:
        mainmenu();
        break;

    default:
        printf("\n\t\t\t\t\t\t\tInvalid Input!!! Press ENTER to return...");
        fflush(stdin);// Clear the input buffer
        getchar(); // Wait for Enter key
        system("cls");
        user_menu();
    }
}
void mainmenu()
{
    system("cls");
    printf("\t\t\t\t\t\t\t<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("\t\t\t\t\t\t\t>                  *************************               <\n");
    printf("\t\t\t\t\t\t\t<                  LIBRARY MANAGEMENT SYSTEM               >\n");
    printf("\t\t\t\t\t\t\t>                  *************************               <\n");
    printf("\t\t\t\t\t\t\t============================================================\n");
    printf("\t\t\t\t\t\t\t>      *********       #     *******     #   ************  <\n");
    printf("\t\t\t\t\t\t\t<      1. ADMIN        #     2. User     #   3. Developer  >\n");
    printf("\t\t\t\t\t\t\t>      *********       #     *******     #   ************  <\n");
    printf("\t\t\t\t\t\t\t<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

    int choose;
    printf("\n\t\t\t\t\t\t\tEnter your choice (1-3): ");
    scanf("%d", &choose);

    if (choose == 1)
    {
        system("cls");
        admin_login();
    }
    else if (choose == 2)
    {
        system("cls");
        user_login();
    }
    else if (choose == 3)
    {
        system("cls");
        developer();
    }

    else
    {
        printf("\n\t\t\t\t\t\tInvalid input!!! Press ENTER to try again......");
        fflush(stdin);   //fflush is used for flushing the output buffer of a stream
        getchar();       //getchar() function is used to wait to read a single character
        system("cls");   //console clear
        mainmenu();
    }
}
void developer()
{
    system("cls");
    printf("\t\t\t\t\t\t\t---------------------------------------------------------------\n");
    printf("\t\t\t\t\t\t\t|*************************************************************|\n");
    printf("\t\t\t\t\t\t\t|                     **ABOUT DEVELOPERS**                    |\n");
    printf("\t\t\t\t\t\t\t|-------------------------------------------------------------|\n");
    printf("\t\t\t\t\t\t\t***************************************************************\n");
    printf("\t\t\t\t\t\t\tName: Asif Jahan\n\t\t\t\t\t\t\tID: 232-15-748\n\t\t\t\t\t\t\tSection: 65_C\n\t\t\t\t\t\t\tDepartment of CSE, DIU\n\n");
    printf("\t\t\t\t\t\t\tName: Fahim Muntasir\n\t\t\t\t\t\t\tID: 232-15-063\n\t\t\t\t\t\t\tSection: 65_C\n\t\t\t\t\t\t\tDepartment of CSE, DIU\n\n");
    printf("\t\t\t\t\t\t\tName: Meher Niger \n\t\t\t\t\t\t\tID: 232-15-075\n\t\t\t\t\t\t\tSection: 65_C\n\t\t\t\t\t\t\tDepartment of CSE, DIU\n\n");
    printf("\t\t\t\t\t\t\tName: Shakil Ahammed Sihab\n\t\t\t\t\t\t\tID: 232-15-003\n\t\t\t\t\t\t\tSection: 65_C\n\t\t\t\t\t\t\tDepartment of CSE, DIU\n\n");
    printf("\t\t\t\t\t\t\tName: Emon Shahriare\n\t\t\t\t\t\t\tID: 232-15-725\n\t\t\t\t\t\t\tSection: 65_C\n\t\t\t\t\t\t\tDepartment of CSE, DIU\n\n");

    printf("\t\t\t\t\t\t\tPress Enter Key to return ");

    fflush(stdin);   //fflush is used for flushing the output buffer of a stream
    getchar();       //getchar() function is used to wait to read a single character
    system("cls");   //console clear
    mainmenu();
}

int main()
{

    system("color 3");
    system("cls");
    database();
    mainmenu();
    return 0;
}





