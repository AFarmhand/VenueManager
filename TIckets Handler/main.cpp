/******************************************************************************

Auditorium's seats reservation system

*******************************************************************************/
#include <stdio.h>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <unordered_map>

#include "Seat.h"
#include "Node.h"
#include "Auditorium.h"

/* 
Orders struct holds information about seats a user orders, other information such as
cost and auditorium number are updated for order as seats are reserved for order
*/
struct order {
     float totalCost = 0.0;
     int auditoriumNum;
     int numAdult;
     int numChild;
     int numSenior;
     std::vector<Seat> seats;
};

/*
Users struct holds username, password, and all the orders for the user throughout reservation process
*/
struct user {
     std::string username;
     std::string password;
     std::vector<order> orders;
};

void displayMainMenu(); 
void displayAdminMenu();

bool isChar(std::string input);
bool isInt(std::string input);

bool removeSeatFromUser(user* user, int orderIndex, int row, int column);
void removeFromOrder(order* order, int row, int column); // helper for above

void addOrder(user* user, order newOrder);

order createOrder(int audNum, int row, int column, int adultTickets, int childTickets, int seniorTickets);
void addToOrder(user* user, int orderIndex, int row, int column, int adultTickets, int childTickets, int seniorTickets);

void eraseOrder(user* user, int orderIndex);

void displayOrders(user* user);
void displayOrdersWithoutOrders(user* user);
void displayReceipt(user* user);
void displayReport(Auditorium* A1, Auditorium* A2, Auditorium* A3);

bool compareSeats(const Seat& seat1, const Seat& seat2);

int main()
{
     // Our nice little map :)
     std::unordered_map<std::string, user> userMap;
     
     std::ifstream inputFile("userdb.dat");
     
     // Check if the file is open
     if (!inputFile.is_open()) {
          std::cout << "Error opening file." << std::endl;
          return 1; // Give up
     }
     
     // Read each line from the file to create username and passward hashmap
     std::string line;
     while (std::getline(inputFile, line)) {
          std::istringstream iss(line);
          user u;
          // Read username and password from the line (if they are there)
          if (iss >> u.username >> u.password) {
               // Add the user to the unordered map
               userMap[u.username] = u;
          } 
          // There's something funky in the database that's not read properly
          else {
               std::cout << "Error reading line: " << line << std::endl;
          }
     }

     // Close the file
     inputFile.close();

     // Constructs Auditoriums by reading in txt files
     Auditorium auditoriumOne;
     Auditorium auditoriumTwo;
     Auditorium auditoriumThree;
     auditoriumOne.construct("A1.txt");
     auditoriumTwo.construct("A2.txt");
     auditoriumThree.construct("A3.txt");

     int maxAttempts = 3;
     
     bool exited = false;
     while (!exited) {
          std::string inputUsername;
          std::cout << "Enter username: ";
          std::cin >> inputUsername;

          auto userIt = userMap.find(inputUsername);

          if (userIt != userMap.end()) {
               user* userPtr = &(userIt->second);

               for (int attempts = 0; attempts < maxAttempts; ++attempts) {
                    std::string inputPassword;
                    std::cout << "Enter password (attempt " << attempts + 1 << " of " << maxAttempts << "): ";
                    std::cin >> inputPassword;
                    
                    // Sucesfully logged in and can go to main or admin menu
                    if (inputPassword == userPtr->password) {
                         
                         // If logged into admin, do seperate admin activites
                         if (userPtr->username == "admin"){
                              
                              while(true){
                                   displayAdminMenu();
                                   std::string input;
                                   std::cin >> input;
                                   
                                   if (input == "1"){ // Display auditorium reports
                                        displayReport(&auditoriumOne, &auditoriumTwo, &auditoriumThree);
                                   }
                                   else if (input == "2"){ // Log out
                                        std::cout << "Logging out.\n";
                                        break;
                                   }
                                   else if (input == "3"){ // Exit system
                                        std::cout << "Exiting ticket system. Writing to files. Have a great day!\n";
                                        exited = true;
                                        break;
                                   }
                              }

                              break;
                         }
                         
                         // Access regular user options to reserve seats
                         std::cout << "Login successful!\n";

                         // While loop breaks when logging out
                         while(true){                         
                              displayMainMenu();
                              std::string input;
                              char charInput;
                              int intInput;
                              std::cin >> input;
                              
                              if (input == "1") { // Reserve seats
                                   std::cout << "1. Auditorium 1\n2. Auditorium 2\n3. Auditorium 3" << std::endl;
                                   Auditorium* curAud = nullptr;
                                   int curAudNum = 0;
                                   // Keep asking for auditorium input until it's 1, 2, or 3. then ste aud pointer to that auditorium to modify it
                                   while(!curAud)
                                   {    
                                        std:: cin >> input;
                                        if(input == "1"){
                                             curAud = &auditoriumOne;
                                             curAudNum = 1;
                                        }
                                        else if (input == "2"){
                                             curAud = &auditoriumTwo;
                                             curAudNum = 2;
                                        }
                                        else if (input == "3"){
                                             curAud = &auditoriumThree;
                                             curAudNum = 3;
                                        }
                                   }
                                   
                                        std::cout << *curAud; // Display auditorium for user to see empty seats
                                        // Validate possible row in auditorium
                                        do{
                                             std::cout << "Enter row you wish to reserve in: ";
                                             std::cin >> input;
                                             while(!isInt(input)){ // Use isInt method to validate input is an integer
                                                  std::cout << "Enter row you wish to reserve in: ";
                                             std::cin >> input;
                                        }
                                        intInput = std::stoi(input);
                                        }    
                                        while(!(intInput > 0 && intInput <= curAud->getRows())); // While row number is not in auditorium, keep asking
                                        
                                        // Validate possible column for auditorium
                                        do{
                                             std::cout << "Enter starting seat letter you wish to reserve in: ";
                                             std::cin >> input;
                                             while(!isChar(input)){ // Uses isChar  method to validate input is character
                                                  std::cout << "Enter starting seat letter you wish to reserve in: ";
                                                  std::cin >> input;
                                             }
                                             charInput = input.at(0);
                                        }
                                        while(!(charInput >= 'A' && charInput <= char(curAud->getColumns()) + 65)); // While seat character is not in auditorium, keep asking
                                                                                            // Kep asking
                    
                                        int adultTickets; // Initalizing values that user will put number of specific ticket types as
                                        int childTickets;
                                        int seniorTickets;
                                        std::string inputString;
               
                                        do{
                                             std::cout << "Enter number of adult tickets: ";
                                             std::cin >> inputString;
                                             while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                  std::cout << "Enter number of adult tickets: ";
                                                  std::cin >> inputString;
                                             }
                                        adultTickets = std::stoi(inputString);
                                        }
                                        
                                        while(!(adultTickets >= 0)); // While loops ensure user will enter a nonnegative integer for the types of tickets
                                        do{
                                             std::cout << "Enter number of child tickets: ";
                                             std::cin >> inputString;
                                             while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                  std::cout << "Enter number of child tickets: ";
                                                  std::cin >> inputString;
                                             }
                                             
                                             childTickets = std::stoi(inputString);
                                        }
                                        while(!(childTickets >= 0));
               
                                        do{
                                             std::cout << "Enter number of senior tickets: ";
                                             std::cin >> inputString;
                                             while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                  std::cout << "Enter number of senior tickets: ";
                                                  std::cin >> inputString;
                                             }
                                             seniorTickets = std::stoi(inputString);
                                        }
                                        while(!(seniorTickets >= 0));
                    

                                        int totalSeats = adultTickets + childTickets + seniorTickets; // sum seats for IsAvailable and BestAvailable functions
               
                                        if(curAud->checkAvailability(intInput,charInput, totalSeats)){ // Reservation is available
                                             std::cout << "Seats reserved, enjoy!" << std::endl;
                                             curAud->reserveSeats(intInput, charInput, adultTickets, childTickets, seniorTickets);
                                             order newOrd = createOrder(curAudNum, intInput, charInput, adultTickets, childTickets, seniorTickets);
                                             addOrder(userPtr, newOrd);
                                             
                                        }
                                        else{ // If reservation is not available look for BestAvailable
                                             Seat bestSeat = curAud->bestAvailable(totalSeats);
                                             if(bestSeat.getRow() != 0){
                                                  if(totalSeats == 1){ // Check if they are looking for 1 seat, or a range of seats and output recomended seat accordingly
                                                       std::cout << "The best available seat is " << bestSeat.getRow() << bestSeat.getColumn() << std::endl;
                                                  }
                                                  else
                                                       std::cout << "The best available seats are " << bestSeat.getRow() << bestSeat.getColumn() << " - "
                                                       << bestSeat.getRow() << char(bestSeat.getColumn() + (totalSeats - 1)) << std::endl;
               
                                                  std::cout << "Would you like to reserve them? (Y/N): ";
                                                  std::cin >> charInput;
                                                  if(charInput == 'Y'){ // Confirm user wants the new best available sets, if Y, reserve!
                                                       curAud->reserveSeats(bestSeat.getRow(), bestSeat.getColumn() , adultTickets, childTickets, seniorTickets);
                                                       std::cout << "Seats reserved, enjoy!" << std::endl;
                                                       order newOrd = createOrder(curAudNum, bestSeat.getRow(), bestSeat.getColumn() , adultTickets, childTickets, seniorTickets);
                                                       addOrder(userPtr, newOrd);

                                                  }
                                             }
                                        else{
                                             std::cout << "no seats available" << std::endl;
                                        }
                                   }
                              }
                              else if (input == "2"){ // View orders
                                   displayOrdersWithoutOrders(userPtr);
                              }
                              else if (input == "3"){ // Update Order
                                   displayOrders(userPtr);
                                   
                                   if(!(userPtr->orders).empty()){
                                   std::cout << "What order would you like to update?" << std:: endl; // TODO Order # Validation
                                   int orderNum;
                                   std::cin >> input;
                                   while(!isInt(input)){ // Use isInt method to validate input is an integer
                                        std::cout << "What order would you like to update?";
                                        std::cin >> input;
                                   } 
                                   orderNum = stoi(input); // TODO Validate Order Num
                                   orderNum--; // Decrements order number to get to actual index
                                   std::cout << "1. Add tickets to order \n2. Delete ticket from order \n3. Cancel Order \n";
                                   
                                   while(true){
                                             Auditorium* curAud = nullptr;

                                             if((userPtr->orders)[orderNum].auditoriumNum == 1){
                                                  curAud = &auditoriumOne;
                                             }
                                             else if((userPtr->orders)[orderNum].auditoriumNum == 2){
                                                  curAud = &auditoriumTwo;
                                            }
                                             else if((userPtr->orders)[orderNum].auditoriumNum == 3){
                                                  curAud = &auditoriumThree;
                                             }
                                             

                                        std::cin >> input;
                                        if (input == "1"){ // Add tickets to order
                                             
                                             std::cout << *curAud;
                                             do{
                                                  std::cout << "Enter row you wish to reserve in: ";
                                                  std::cin >> input;
                                                  while(!isInt(input)){ // Use isInt method to validate input is an integer
                                                       std::cout << "Enter row you wish to reserve in: ";
                                                  std::cin >> input;
                                             }
                                             intInput = std::stoi(input);
     
                                             }    
                                             while(!(intInput > 0 && intInput <= curAud->getRows())); // While row number is not in auditorium, keep asking
                                             
                                             do{
                                                  std::cout << "Enter starting seat letter you wish to reserve in: ";
                                                  std::cin >> input;
                                                  while(!isChar(input)){ // Uses isChar  method to validate input is character
                                                       std::cout << "Enter starting seat letter you wish to reserve in: ";
                                                       std::cin >> input;
                                                  }
                                                  charInput = input.at(0);
                                             }
                                             while(!(charInput >= 'A' && charInput <= char(curAud->getColumns()) + 65)); // While seat character is not in auditorium, 
                                                                                                 // Kep asking
                         
                                             int adultTickets; // Initalizing values that user will put number of specific ticket types as
                                             int childTickets;
                                             int seniorTickets;
                    
                                             std::string inputString;
                    
                                             do{
                                                  std::cout << "Enter number of adult tickets: ";
                                                  std::cin >> inputString;
                                                  while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                       std::cout << "Enter number of adult tickets: ";
                                                       std::cin >> inputString;
                                             }
                         
                                                  adultTickets = std::stoi(inputString);
                                             }
                                             while(!(adultTickets >= 0)); // While loops ensure user will enter a nonnegative integer for the types of tickets
                    
                    
                                             do{
                                                  std::cout << "Enter number of child tickets: ";
                                                  std::cin >> inputString;
                                                  while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                       std::cout << "Enter number of child tickets: ";
                                                       std::cin >> inputString;
                                                  }
     
                                                  childTickets = std::stoi(inputString);
                                             }
                                             while(!(childTickets >= 0));
                    
                                             do{
                                                  std::cout << "Enter number of senior tickets: ";
                                                  std::cin >> inputString;
                                                  while(!isInt(inputString)){ // Use isInt method to validate input is integer
                                                       std::cout << "Enter number of senior tickets: ";
                                                       std::cin >> inputString;
                                             }
                                                  seniorTickets = std::stoi(inputString);
                                             }
                                             while(!(seniorTickets >= 0));
                         
     
                                             int totalSeats = adultTickets + childTickets + seniorTickets; // sum seats for IsAvailable and BestAvailable functions
                    
                                             if(curAud->checkAvailability(intInput,charInput, totalSeats)){
                                                  std::cout << "Seats reserved, enjoy!" << std::endl;
                                                  curAud->reserveSeats(intInput, charInput, adultTickets, childTickets, seniorTickets);
                                                  addToOrder(userPtr, orderNum, intInput, charInput, adultTickets, childTickets, seniorTickets);
                                             }
                                             else{ // If reservation is not available look for BestAvailable
                                                  Seat bestSeat = curAud->bestAvailable(totalSeats);
                                                  if(bestSeat.getRow() != 0){
                                                       if(totalSeats == 1){ // Check if they are looking for 1 seat, or a range of seats and output recomended seat accordingly
                                                            std::cout << "The best available seat is " << bestSeat.getRow() << bestSeat.getColumn() << std::endl;
                                                       }
                                                       else
                                                            std::cout << "The best available seats are " << bestSeat.getRow() << bestSeat.getColumn() << " - "
                                                            << bestSeat.getRow() << char(bestSeat.getColumn() + (totalSeats - 1)) << std::endl;
                    
                                                       std::cout << "Would you like to reserve them? (Y/N): ";
                                                       std::cin >> charInput;
                                                       if(charInput == 'Y'){ // Confirm user wants the new best available sets, if Y, reserve!
                                                            curAud->reserveSeats(bestSeat.getRow(), bestSeat.getColumn() , adultTickets, childTickets, seniorTickets);
                                                            std::cout << "Seats reserved, enjoy!" << std::endl;
                                                            addToOrder(userPtr, orderNum, bestSeat.getRow(), bestSeat.getColumn() , adultTickets, childTickets, seniorTickets);
     
                                                       }
                                                  }
                                             else{
                                                  std::cout << "no seats available" << std::endl;
                                             }
                                        
                                             }
                                             break;
                                                  
                                             }
                                        else if (input == "2") {// Delete tickets from order 
                                             
                                             int intInput;
                                             char charInput;
                                             
                                                  do{
                                                       std::cout << "Enter row you wish to remove in: ";
                                                       std::cin >> input;
                                                       while(!isInt(input)){ // Use isInt method to validate input is an integer
                                                            std::cout << "Enter row you wish to remove in: ";
                                                       std::cin >> input;
                                                  }
                                                  intInput = std::stoi(input);
          
                                                  }    
                                                  while(!(intInput > 0 && intInput <= curAud->getRows())); // While row number is not in auditorium, keep asking
                                                  
                                                  do{
                                                       std::cout << "Enter starting seat letter you wish to remove in: ";
                                                       std::cin >> input;
                                                       while(!isChar(input)){ // Uses isChar  method to validate input is character
                                                            std::cout << "Enter starting seat letter you wish to remove in: ";
                                                            std::cin >> input;
                                                       }
                                                       charInput = input.at(0);
                                                  }
                                                  while(!(charInput >= 'A' && charInput <= char(curAud->getColumns()) + 65)); // While seat character is not in auditorium, 
                                             
                                             // TODO: Validate if this seat is in order
                                             curAud->removeSeat(intInput, charInput);
                                             if (removeSeatFromUser(userPtr, orderNum, intInput, charInput))
                                                  curAud->removeSeat(intInput, charInput);
                                             else
                                                  std::cout << "That seat is not in the order!" << std::endl;
                                             
                                             break;
                                        }
                                        else if (input == "3"){ // Erase order from existence
                                             
                                             
                                             for (auto seat : (userPtr->orders)[orderNum].seats){
                                                  curAud->removeSeat(seat.getRow(), seat.getColumn());
                                             }
                                             eraseOrder(userPtr, orderNum);
                                             
                                             break;
                                        }
                                   }
                                   

                                   }
                              }
                              else if (input == "4"){ // Display receipts
                                   std::cout << "Displaying receipt!" << std::endl;;
                                   displayReceipt(userPtr);
                              } 
                              else if (input == "5"){ // Log out
                                   std::cout << "Logging out!" <<std::endl;
                                   break;
                              }
                         }
                         
                         break;
                    }
                    
                    else {
                         if (attempts < 2)
                              std::cout << "Invalid password. Try again.\n";
                         else {
                              std::cout << "Exceeded maximum attempts. Please enter the username again.\n";
                              break;
                         }
                    }
               }

          }
          
          else {
               std::cout << "Username not found. Please try again.\n";
          }
     }
     
     auditoriumOne.write("A1Final.txt");
     auditoriumTwo.write("A2Final.txt");
     auditoriumThree.write("A3Final.txt");
    
     std::cout << std::endl;
     return 0;
}


void addOrder(user* user, order newOrder) {
    user->orders.push_back(newOrder);
}


order createOrder(int audNum, int row, int column, int adultTickets, int childTickets, int seniorTickets){
     // Create new order that we will return to put into our user's order array
     order newOrder;
     
     // Defines # of each ticket type and auditorium number for order
     newOrder.auditoriumNum = audNum;
     newOrder.numAdult = adultTickets;
     newOrder.numChild = childTickets;
     newOrder.numSenior = seniorTickets;
     newOrder.totalCost += (float)(10 * adultTickets) + (float)(5 * childTickets) + (float)(7.50 * seniorTickets);


     std::vector<Seat> seats;
     for(int i = 0; i < adultTickets; i++){
          Seat newSeat(row, (char)(column + i), 'A');
          newOrder.seats.push_back(newSeat);
     }
     for(int i = 0; i < childTickets; i++){
          Seat newSeat(row, (char)(column + i + adultTickets), 'C');
          newOrder.seats.push_back(newSeat);
     }
     for(int i = 0; i < seniorTickets; i++){
          Seat newSeat(row, (char)(column + i + adultTickets + childTickets), 'S');
          newOrder.seats.push_back(newSeat);
     }
     
     return newOrder;
}


// Helper method to determine if a string is a character
bool isChar(std::string input){
     bool charBool = true;
     if (input.length() > 1 || input.length() == 0) // If the length is greater than 1 or 0 the string is not a character
          charBool = false;
     
     for(char c : input){
          if(!isalpha(c) && !isdigit(c)){ // The string is not a character if it's not a digit or alphabetical character
               charBool = false;
          }
     }
     return charBool;
}

// Helper method to determine if a string is an integer
bool isInt(std::string input){
     bool intBool = true;
     for(char c : input){ // For every character in the string, if any are not numerical digits, the string is not an integer 
          if(!isdigit(c)){
               intBool = false;
               break;
          }
     }
     return intBool;
}

void displayMainMenu(){
     std::cout << "1. Reserve Seats \n2. View Orders \n3. Update Order \n4. Display Receipt \n5. Logout \n";
}

void displayAdminMenu(){
     std::cout << "1. Print Report \n2. Logout \n3. Exit\n";
}

void displayOrdersWithoutOrders(user* user){

     if((user->orders).empty()){
          std::cout << "No orders" << std::endl << std::endl;
     }
     
     for (auto order : user->orders){
          std::cout << "Auditorium " << order.auditoriumNum << ", ";
          for (int i = 0; i < (int)order.seats.size(); ++i) {
               const Seat& seat = order.seats[i];
               std::cout << seat.getRow() << seat.getColumn();

               if (i < (int)order.seats.size() - 1) {
               std::cout << ",";
               }
          }          std::cout << std::endl << order.numAdult << " adult, " << order.numChild << " child, " << order.numSenior << " senior" << std::endl;
          std::cout << std::endl;
     }
}

void displayOrders(user* user){
     int orderNum = 1;
     
     if((user->orders).empty()){
          std::cout << "No orders" << std::endl << std::endl;
     }
     
     for (auto order : user->orders){
          std::cout << "Order " << orderNum++ << std::endl;
          std::cout << "Auditorium " << order.auditoriumNum << ", ";
          for (int i = 0; i < (int)order.seats.size(); ++i) {
               const Seat& seat = order.seats[i];
               std::cout << seat.getRow() << seat.getColumn();

               if (i < (int)order.seats.size() - 1) {
               std::cout << ",";
               }
          }          std::cout << std::endl << order.numAdult << " adult, " << order.numChild << " child, " << order.numSenior << " senior" << std::endl;
          std::cout << std::endl;
     }
}

void displayReceipt(user* user){
//     int orderNum = 1;
     float total = 0;
     
     if((user->orders).empty()){
          std::cout << "No orders" << std::endl << std::endl;
     }
     
     for (auto order : user->orders){
//          std::cout << "Order " << orderNum++ << std::endl;
          std::cout << "Auditorium " << order.auditoriumNum << ", ";
          //for(auto seat : order.seats){
               
               
          for (int i = 0; i < (int)order.seats.size(); ++i) {
               const Seat& seat = order.seats[i];
               std::cout << seat.getRow() << seat.getColumn();

               if (i < (int)order.seats.size() - 1) {
               std::cout << ",";
               }
          }
               
               //std::cout << seat.getRow() << seat.getColumn();
          //}
          std::cout << std::endl << order.numAdult << " adult, " << order.numChild << " child, " << order.numSenior << " senior" << std::endl;
          std::cout << std::fixed << std::setprecision(2) << "Order Total: $" << order.totalCost << std::endl << std::endl;
          total += order.totalCost;
     }
     
     
     std::cout << std::fixed << std::setprecision(2) << "Customer Total: $" << total << std::endl << std::endl;
}

void displayReport(Auditorium* A1, Auditorium* A2, Auditorium* A3){
     
    auto displayAud = [](Auditorium* A) 
    { 
          int width = 5; // std::setw(width) spaces out all information into tables
          A->calculateReport();
          std::cout << std::setw(width) << (A->getTotalSeats() - A->getTotalTickets()) // Calculates empty seats (total seats - reserved)
          << std::setw(width) << A->getTotalTickets() 
          << std::setw(width) << A->getTotalAdult() 
          << std::setw(width) << A->getTotalChild() 
          << std::setw(width) << A->getTotalSenior() 
          << std::setw(width) << std::fixed << std::setprecision(2) << "$" << A->getTotalSales()
          << std::endl;
    };

     std::cout << std::left << std::setw(16) << "Auditorium 1 " ;
     displayAud(A1); 
     std::cout << std::left << std::setw(16) << "Auditorium 2 " ;
     displayAud(A2); 
     std::cout << std::left << std::setw(16) << "Auditorium 3 " ;
     displayAud(A3); 
     std::cout << std::left << std::setw(16) << "Total " ;
     Auditorium resultAuditorium = Auditorium::addAuditoriums(*A1, *A2, *A3);
     displayAud(&resultAuditorium);
}

void addToOrder(user* user, int orderIndex, int row, int column, int adultTickets, int childTickets, int seniorTickets){
     if (!(orderIndex >= 0 && orderIndex < static_cast<int>(user->orders.size()))) {
          std::cout << "Invalid index for order removal" << std::endl;
          return;
     }
     
     order* curOrder = &(user->orders)[orderIndex];
     curOrder->numAdult += adultTickets;
     curOrder->numChild += childTickets;
     curOrder->numSenior += seniorTickets;
     curOrder->totalCost += (float)((10 * adultTickets) + (5 * childTickets) + (7.50 * seniorTickets));


     for(int i = 0; i < adultTickets; i++){
          Seat newSeat(row, (char)(column + i), 'A');
          curOrder->seats.push_back(newSeat);
     }
     for(int i = 0; i < childTickets; i++){
          Seat newSeat(row, (char)(column + i + adultTickets), 'C');
          curOrder->seats.push_back(newSeat);
     }
     for(int i = 0; i < seniorTickets; i++){
          Seat newSeat(row, (char)(column + i + adultTickets + childTickets), 'S');
          curOrder->seats.push_back(newSeat);
     }
     
    std::sort(curOrder->seats.begin(), curOrder->seats.end(), compareSeats);

}

void removeFromOrder(order* order, int row, int column){
     for (int i = 0; i < (int)order->seats.size(); i++){
          if (row == (order->seats)[i].getRow() && column == (order->seats)[i].getColumn()){
               switch((order->seats[i].getTicketType())){
                    case 'A':
                         order->numAdult = order->numAdult - 1;
                         order->totalCost -= (10);
                         break;
                    case 'C':
                         order->numChild = order->numChild - 1;
                         order->totalCost -= (5);
                         break;
                    case 'S':
                         order->numSenior = order->numSenior - 1;
                         order->totalCost -= (float)(7.5);
                         break;
               }
               (order->seats).erase((order->seats).begin() + i);
               return;
          }
     }
     std::cout << "Invalid seat for removal" << std::endl;
}

bool removeSeatFromUser(user* user, int orderIndex, int row, int column){
     order* curOrder = &(user->orders)[orderIndex];
     
     bool seatInOrder = false;
     for (auto seat: curOrder->seats){
          if (seat.getColumn() == column && seat.getRow() == row){
               std::cout << "SEAT FOUND MWAHAHA";
               seatInOrder = true;
          }
     }
     
     if (seatInOrder){
          removeFromOrder(curOrder, row, column);
     }
     
     if ((curOrder->seats).empty()){
          if (orderIndex >= 0 && orderIndex < static_cast<int>(user->orders.size())) {
               user->orders.erase(user->orders.begin() + orderIndex);
          }
          else if (seatInOrder) {
               std::cout << "Invalid index for order removal" << std::endl;
          }
     }
     
     return seatInOrder;

}

void eraseOrder(user* user, int orderIndex){
     if (orderIndex >= 0 && orderIndex < static_cast<int>(user->orders.size())) {
          user->orders.erase(user->orders.begin() + orderIndex);
     }
     else {
          std::cout << "Invalid index for order removal" << std::endl;
     }
}

// Helper to sort seats in order
bool compareSeats(const Seat& seat1, const Seat& seat2) {
    if (seat1.getRow() == seat2.getRow()) {
        return seat1.getColumn() < seat2.getColumn();
    }
    return seat1.getRow() < seat2.getRow();
}