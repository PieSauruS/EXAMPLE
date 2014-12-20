#include <iostream>
#include <stdlib.h>
#include <pqxx/pqxx>
#include <iomanip>
#include <string>

using namespace std;
using namespace pqxx;


struct userObj{
    string username;
    int hours;
    int played;
    int posCom;
    int negCom;
};

void printUsers(connection *conDataBase)
{
    const char* sqlQuery = "SELECT * from USR";
    nontransaction N(*conDataBase);
    result R(N.exec(sqlQuery));
    N.commit();
    for(result::const_iterator c = R.begin(); c!= R.end(); ++c)
    {
        cout << setw(20) << "login = " << c[0].as<string>() << endl;
    }
}

void initUser(connection *conDataBase, userObj *currentUser)
{
    string sqlQuery = "SELECT * FROM USER_FILTERS WHERE username = \'";
    sqlQuery += currentUser->username;
    sqlQuery += "\'";
    nontransaction N(*conDataBase);
    result R(N.exec(sqlQuery.c_str()));
    for(result::const_iterator c = R.begin(); c!=R.end(); ++c)
    {
        currentUser->hours = c[1].as<int>();
        currentUser->played = c[2].as<int>();
        currentUser->posCom = c[3].as<int>();
        currentUser->negCom = c[4].as<int>();
    }
}

int updateFilter(connection *conDataBase, string filterQuery, string username)
{
    string howmuch;
    cout << "How much?" << endl;
    cin >> howmuch;
    getchar();
    string sqlQuery = "UPDATE USER_FILTERS SET ";
    sqlQuery += filterQuery;
    sqlQuery += " = \'";
    sqlQuery += howmuch;
    sqlQuery += "\' WHERE username = \'";
    sqlQuery += username;
    sqlQuery += "\'";
    work W(*conDataBase);
    W.exec(sqlQuery.c_str());
    W.commit();
}

void setFilters(connection *conDataBase, userObj *currentUser)
{
    string sqlQuery = "SELECT * FROM USER_FILTERS WHERE username = \'";
    sqlQuery += currentUser->username;
    sqlQuery += "\'";
    bool inMenu = true;
    char filterChoice = ' ';
    string partQuery = " ";
    while(inMenu)
    {
        system("clear");
        nontransaction N(*conDataBase);
        result R(N.exec(sqlQuery.c_str()));
        N.commit();
        inMenu = true;
        cout << "===============================================" << endl;
        for(result::const_iterator c = R.begin(); c!=R.end(); ++c)
        {
            cout << "Filter for Username: " << setw(65) << c[0].as<string>() << endl;
            cout << "1) hours played: " << setw(25) << c[1].as<int>() << endl;
            cout << "2) games played: " << setw(25) << c[2].as<int>() << endl;
            cout << "3) positive commendations: " << setw(15) << c[3].as<int>() << endl;
            cout << "4) negative commendations: " << setw(15) << c[4].as<int>() << endl;
            cout << "0) return to menu" << endl;
        }
        cout << "===============================================" << endl;
        cout << "Which filter would you like to change?" << endl;
        cin >> filterChoice;
        switch(filterChoice)
        {
            case '0':
                inMenu = false;
                break;
            case '1':   
                cout << "Filter for hours" << endl;
                partQuery = "hours_played"; 
                updateFilter(conDataBase, partQuery, currentUser->username);
                break;
            case '2':
                cout << "Filter for games played" << endl;
                partQuery = "games_played"; 
                updateFilter(conDataBase, partQuery, currentUser->username);
                break;
            case '3' :
                cout << "Filter for positive commendations" << endl;
                partQuery = "pos_commend"; 
                updateFilter(conDataBase, partQuery, currentUser->username);
                break;
            case '4':
                cout << "Filter for negative commendations" << endl;
                partQuery = "neg_commend"; 
                updateFilter(conDataBase, partQuery, currentUser->username);
                break;
            default:
                cout << "Not a valid choice!" << endl;
                break;
        }
    }
}


void viewRecord(connection *conDataBase, string currentUser)
{
    system("clear");
    string sqlQuery = "SELECT * FROM USR WHERE login = \'";
    sqlQuery += currentUser;
    sqlQuery += "\'";
    nontransaction N(*conDataBase);
    result R(N.exec(sqlQuery.c_str()));
    N.commit();
    for(result::const_iterator c = R.begin(); c!=R.end(); ++c)
    {
        cout << "Username: " << setw(70) << c[0].as<string>() << endl;
        cout << "hours played: " << setw(25) << c[5].as<int>() << endl;
        cout << "games played: " << setw(25) << c[6].as<int>() << endl;
        cout << "positive commendations: " << setw(15) << c[7].as<int>() << endl;
        cout << "negative commendations: " << setw(15) << c[8].as<int>() << endl;
    }
    cout << endl << endl << endl;
    cout << "...Please press enter to return to the menu" << endl;
    getchar();
    return;
}

string LogIn(connection *conDataBase)
{
    string login, password;
    string sqlQuery;
    cout << "Enter user login: " << endl;
    cin >> login;
    cout << "Enter user password: " << endl;
    cin >> password;
    //setting up the string query;
    sqlQuery = "SELECT * FROM Usr WHERE login =\'";
    sqlQuery += login;
    sqlQuery += "\' AND password = \'";
    sqlQuery += password;
    sqlQuery += "\'";
    //making a nontrasnaction object to execute the query;

    nontransaction nonTransact(*conDataBase);
    result R(nonTransact.exec(sqlQuery.c_str()));
    nonTransact.commit();
    for(result::const_iterator c = R.begin(); c!=R.end(); ++c)
        return c[0].as<string>();
    //If it reaches here it means the user does not exist or the password is incorrect
    cout << "The user name does not exist or the password is incorrect" << endl;
    return "\0";
}

//menu that will be presented to the user
void mainMenu(connection *conDataBase, bool* loggedin, userObj *authorisedUser)
{
    char input;
    system("clear"); //clear the terminal
    cout << "====================================" << endl;
    cout << "\tWelcome to Refilter" << endl;
    cout << "====================================" << endl;
    cout << "1. Find Match"  <<  endl;
    cout << "2. Set Filter Options" << endl;
    cout << "3. View Record/Profile" << endl;
    cout << "0. Logout" << endl;
    cout << "====================================" << endl;
    cin >> input;
    getchar();
    if(input > 47 && input < 58) //the range for valid ascii inputs 0...9
    {
        switch(input)
        {
            case '0'://Quit
                *loggedin = false;
                return;
            case '1': //Find Match;
                break;
            case '2': //Set Filter Option
                setFilters(conDataBase, authorisedUser);
                getchar();
                break;
            case '3': //Look At Your Own Record
                viewRecord(conDataBase, authorisedUser->username);
                break;
            default:
                cout << "Not a valid option...";
                getchar();
                break;
        }
    }
    else
    {
        cout << "Not a valid option...";
        getchar();
    }
}

void loginMenu(connection *conDataBase)
{
    char input;
    bool keepon = true;
    while(keepon)
    {
        userObj authorisedUser;
        char input;
        system("clear"); //clear the terminal
        //printUsers(conDataBase); //testing purposes
        cout << "====================================" << endl;
        cout << "\tWelcome to Refilter" << endl;
        cout << "====================================" << endl;
        cout << "1. Login"  <<  endl;
        cout << "2. Create User" << endl;
        cout << "0. Quit" << endl;
        cout << "====================================" << endl;
        cin >> input;
        getchar();//clear stream buffer containing newlines
        switch(input)
        {
            case '0': //quit
                system("clear");
                cout << "\n\n\tThank You for Using Refilter" << endl;
                getchar();
                return;
            case '1': //login
                authorisedUser.username = LogIn(conDataBase);
                break;
            case '2': // Create new user
                break;
            default: 
                cout << "Unrecognized choice!" << endl;
                break;
        }
        if(authorisedUser.username != "\0")
        {
            initUser(conDataBase, &authorisedUser);
            bool loggedin = true;
            while(loggedin)
            {
                mainMenu(conDataBase, &loggedin, &authorisedUser);
            }
        }
    }
}



int main()
{
    //connect to the database
    try{
        pqxx::connection conDataBase("user=ksuh");
        if(conDataBase.is_open()){
            cout << "\tOpened database successfully:" << conDataBase.dbname() << endl;
        }
        else{
            cout << "Can't open database" << endl;
            return 1;
        }
        //execute query
        loginMenu(&conDataBase);
        //disconnect from the database
        conDataBase.disconnect();
    }
    catch(const std::exception &e)
    {
        cerr << e.what() << std::endl;
        return 1;
    }
}

