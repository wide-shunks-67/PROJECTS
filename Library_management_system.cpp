#include<bits/stdc++.h>
using namespace std;
//Book structure
struct Book{
    string title;
    string author;
    string ISBN;
    bool is_issued=false;
};
//Details of borrowed books
struct BorrowedBook{
    string isbn;
    string due_date;
};
//Borrower structure 
struct Borrower{
    string ID;
    string name;
    vector<BorrowedBook>borrowed_books;
};
//get current date
string get_current_date(){
    time_t now=time(0);
    tm* local=localtime(&now);
    ostringstream oss;
    oss<< put_time(local, "%Y-%m-%d");
    return oss.str();
}
//date after 14 days time
string get_time_14_days_later(){
    time_t now = time(0);
    now+=14 * 24 *60 * 60;

    tm* due_time=localtime(&now);
    ostringstream oss;
    oss<<put_time(due_time, "%Y-%m-%d");

    return oss.str();
}
//Calculating the time at which borrower returned book
int calculate_days_late(const string& dueDate, const string& currentDate){
    tm due{}, now{};

    istringstream ss1(dueDate);
    ss1>>std::get_time(&due, "%Y-%m-%d");

    istringstream ss2(currentDate);
    ss2>>std::get_time(&now, "%Y-%m-%d");

    time_t timedue= mktime(&due);
    time_t timenow= mktime(&now);

    double seconds= difftime(timenow, timedue);
    return static_cast<int>(seconds/ (60*60*24));
}

vector<Book>library;
vector<Borrower>borrowers;
//Function to add book
void add_book(){
    Book book;

    cout<<"Enter the title of book: ";
    getline(cin>>ws, book.title);

    cout<<"Enter the author of book: ";
    getline(cin>>ws, book.author);

    cout<<"Enter the ISBN of book: ";
    getline(cin>>ws, book.ISBN);

    book.is_issued=false;

    library.push_back(book);
    cout<<"\nBook added successfully\n";
}
//Function to view book 
void view_book(){
    if(library.empty()){
        cout<<"\nNo book found\n";
        return ;
    }
    
    for(size_t i=0; i<library.size(); i++){
        cout<<"\nBook "<<i + 1<<":\n";
        cout<<"Title: "<<library[i].title<<endl;
        cout<<"Author: "<<library[i].author<<endl;
        cout<<"ISBN: "<<library[i].ISBN<<endl;
        cout<<"Status: ["<<(library[i].is_issued ? "ISSUED":"AVAILABLE")<<"]\n";
    }
}
//function to search_by_title
void search_by_title(const vector<Book>&library ){
    string title;
    cout<<"Enter the title of book you want to search:  ";
    getline(cin>>ws,title);
    bool found=false;
    for(const auto &book : library ){
        if(book.title == title){
            cout<<"\nBook found.\n";
            cout<<"Title: "<<book.title<<endl;
            cout<<"Author: "<<book.author<<endl;
            cout<<"ISBN: "<<book.ISBN<<endl;
            cout<<"Status: ["<<(book.is_issued ? "ISSUED":"AVAILABLE")<<"]\n";
            found=true;
        }
    }
    if(!found){
        cout<<"\nBook with title: "<<title<<" not found.\n";
    }
}
//function to search_by_author
void search_by_author(const vector<Book>&library ){
    string author;
    cout<<"Enter the name of the author of book you want to search:  ";
    getline(cin>>ws,author);
    bool found=false;
    for(const auto &book : library ){
        if(book.author == author){
            cout<<"\nBook found.\n";
            cout<<"Title: "<<book.title<<endl;
            cout<<"Author: "<<book.author<<endl;
            cout<<"ISBN: "<<book.ISBN<<endl;
            cout<<"Status: ["<<(book.is_issued ? "ISSUED":"AVAILABLE")<<"]\n";
            found=true;
        }
    }
    if(!found){
        cout<<"\nBook with author: "<<author<<" not found.\n";
    }
}
//function to search_by_ISBN
void search_by_ISBN(const vector<Book>&library ){
    string ISBN;
    cout<<"Enter the ISBN of book you want to search:  ";
    getline(cin>>ws,ISBN);
    bool found=false;
    for(const auto &book : library ){
        if(book.ISBN == ISBN){
            cout<<"\nBook found.\n";
            cout<<"Title: "<<book.title<<endl;
            cout<<"Author: "<<book.author<<endl;
            cout<<"ISBN: "<<book.ISBN<<endl;
            cout<<"Status: ["<<(book.is_issued ? "ISSUED":"AVAILABLE")<<"]\n";
            found=true;
        }
    }
    if(!found){
        cout<<"\nBook with ISBN: "<<ISBN<<" not found.\n";
    }
}
//function to issue a book to borrower
void issue_book(vector<Book>&library, vector<Borrower>&borrowers){
    string isbn;
    string due_date;
    cout<<"Enter the ISBN of book you want to issue: ";
    getline(cin>>ws, isbn);

    bool book_found=false;

    for( auto &book:library){
        if(book.ISBN == isbn){
            book_found =true;
            if(book.is_issued){
                cout<<"\nThis book is already issued.\n";
                return;
            }
            Borrower b;
            cout<<"Enter the Borrower ID: ";
            getline(cin>>ws, b.ID);
            cout<<"Enter the Borrower name: ";
            getline(cin>>ws, b.name);
            auto it= find_if(borrowers.begin(),borrowers.end(),[&](const Borrower& br){return br.ID == b.ID;});
            if(it!= borrowers.end()){
                //borrower exist
                BorrowedBook borrowed;
                borrowed.isbn=book.ISBN;
                borrowed.due_date = get_time_14_days_later();
                it->borrowed_books.push_back(borrowed);
                book.is_issued=true;
                cout<<"\nBook successfull issued to "<<it->name<< " with ID: "<<it->ID<<" .\n";

            }
            else{
                //new borrower
            BorrowedBook borrowed;
            borrowed.isbn=book.ISBN;
            borrowed.due_date = get_time_14_days_later();
            b.borrowed_books.push_back(borrowed);
            book.is_issued=true;
            cout<<"\nBook successfull issued to "<<b.name<< " with ID: "<<b.ID<<" .\n";
            borrowers.push_back(b);
            }
            return;
        } 
    }
    if(!book_found){
        cout<<"\nBook with ISBN: "<<isbn<< " not found.\n";
    }
}
//Function to return book to library
void return_book(vector<Book>&library, vector<Borrower>&borrowers){
    string isbn;
    string borrower_id;
    cout<<"\nEnter the ISBN of book to return: ";
    getline(cin>>ws, isbn);
    cout<<"\nEnter Borrower ID: ";
    getline(cin>>ws, borrower_id);

    bool book_found=false;
    bool borrower_found=false;

    for(auto &book: library){
        if(book.ISBN == isbn){
            book_found=true;
            if(!book.is_issued){
                cout<<"This is book is not currently issued.\n";
                return;
            }
            //now find borrower
            for(auto &borrower :borrowers){
                if(borrower.ID == borrower_id){
                    borrower_found = true;

                    auto it= find_if(borrower.borrowed_books.begin(), borrower.borrowed_books.end(),[&](const BorrowedBook& b){return b.isbn == isbn; });
                    if(it != borrower.borrowed_books.end()){
                        //fine calculation
                        string due= it->due_date;
                        int days_late=calculate_days_late(due,get_current_date());
                        if(days_late>0){
                            int fine=days_late * 5;
                            cout<<"Returned "<<days_late<<" days late. Fine: "<<fine<<"\n";
                        }else{
                            cout<<"Returned on time.\n";
                        }

                        borrower.borrowed_books.erase(it);
                        book.is_issued=false;
                        cout<<"Book: "<<book.title<<" returned successfully by "<<borrower.name<<" .\n";
                    }else{
                        cout<<"This borrower didn't borrowed the specified book.\n";
                    }
                    break;
                }
            }
            if(!borrower_found){
                cout<<"Borrower with ID: "<<borrower_id<<" not found.\n";
            }
            return ;
        }        
    }
    if(!book_found){
        cout<<"Book with ISBN: "<<isbn<<" not found.\n";
    }
}
//Function to view list of borrowers
void view_borrower(const vector<Borrower>&borrowers){
    cout<<"\n====List of Borrowers====\n";
    if(borrowers.empty()){
        cout<<"No Borrowers found!\n";
        return;
    }
    for(const auto& borrower :borrowers){
        cout<<"\n ID: "<<borrower.ID;
        cout<<"\n Name: "<<borrower.name;

        if(borrower.borrowed_books.empty()){
            cout<<"\n Borrowed books: NONE\n";
        }else{
            cout<<"Borrowed books: \n";
            for(const auto& book : borrower.borrowed_books){
                cout<<" ISBN- "<<book.isbn;
                cout<<", Due Date- "<<book.due_date<<"\n";
            }
        }
        cout<<"------------------------------\n";
    }
}
//DISPLAY MENU
void display_menu(){
    cout<<"\n===Library Management System Menu===\n";
    cout<<"1. Add a new book.\n";
    cout<<"2. Veiw all books.\n";
    cout<<"3. Search the book by Title.\n";
    cout<<"4. Search the book by Author Name.\n";
    cout<<"5. Search the book by ISBN.\n";
    cout<<"6. Issue a book.\n";
    cout<<"7. View all borrowers.\n";
    cout<<"8. Return a book.\n";
    cout<<"9. Exit.\n";
}
//MAIN BLOCK
int main(){
    
    int choice;
    do{
        display_menu();
        cout<<"Enter your choice (1-9): ";
        cin>>choice;
        cin.ignore();

        switch(choice){
            case 1:
            add_book();
            break;

            case 2:
            view_book();
            break;

            case 3:
            search_by_title(library );
            break;

            case 4:
            search_by_author(library );
            break;

            case 5:
            search_by_ISBN(library );
            break;

            case 6:
            issue_book(library, borrowers);
            break;

            case 7:
            view_borrower(borrowers);
            break;
            
            case 8:
            return_book(library, borrowers);
            break;

            case 9:
            cout<<"\nGOOD BYE!\n";
            break;

            default:
            cout<<"Invalid choice. Try again!\n";
        }
    }while(choice!=9);


    return 0;
}

