#include "Auditorium.h"
#include <fstream>
#include <iostream>
#include <iomanip>  
#include <string>
#include <cmath>

// Default constructor sets first node pointer to nullptr
Auditorium::Auditorium() : first(nullptr) {}


//Destroys every node going through every node in a row, then to the next row 
Auditorium::~Auditorium() {
     Node* currentRow = first;
     while (currentRow != nullptr) {
         
          Node* currentNode = currentRow;
          Node* nextRow = currentRow->getDown();

          while (currentNode != nullptr) {
               Node* temp = currentNode;
               currentNode = currentNode->getNext();
               delete temp;
          }
          currentRow = nextRow;
     }
}

// Createas linked list nodes which each contain a seat object with its position and ticket type from inputted text file 
void Auditorium::construct(std::string fileName) {
     
/*     std::string fileName; // Creates file object
     std::cout << "Enter file name: ";
     std::cin >> fileName;*/
     std::ifstream inputFile;
     inputFile.open(fileName);
     
     
     while (!inputFile.is_open()) { // Keeps asking for file name from user until valid file is entered 
          std::cerr << "Error: Could not open file " << fileName << std::endl;
          break;
 /*         std::cout << "Enter file name: ";
          std::cin >> fileName;
          inputFile.open(fileName);*/
    }

    Node* previousRowFirst = nullptr;
    Node* currentRowFirst = nullptr;

    char seatChar;
    int row = 1;
    char column = 'A';
    
    char lastChar;

    while (inputFile.get(seatChar)) { // Goes through every character in text file
          lastChar = seatChar;
          if (seatChar == '\n') {
               if (previousRowFirst == nullptr) {
                    first = currentRowFirst;
               } 
            else {
                previousRowFirst->setDown(currentRowFirst); // Sets down pointers for first node in each row
            }
            
            previousRowFirst = currentRowFirst;
            currentRowFirst = nullptr;
            row++;
            column = 'A';  // Reset column for seat object for a new row
        } 
        else {
            // Create Seat and Node with the seatChar as the seat type
            Seat seat(row, column, seatChar);  // Set row and column
            Node* newNode = new Node(seat);

            if (currentRowFirst == nullptr) {
                currentRowFirst = newNode;
            } 
            else { 
                Node* currentNode = currentRowFirst;
                while (currentNode->getNext() != nullptr) {
                    currentNode = currentNode->getNext();
                }
                currentNode->setNext(newNode);

            }

            ++column;  // Move to the next column
        }
    }

     if(lastChar != '\n'){
          previousRowFirst->setDown(currentRowFirst);
     }
     
     // Find num rows and columns for object

     Node* currentRow = first;
     Node* currentColumn = first;

     while (currentRow->getDown() != nullptr) { // Adds to row count for every row which can be called later in main
          rowsIn += 1;
          currentRow = currentRow->getDown();
     }

     while (currentColumn->getNext() != nullptr) {  // Adds to column count for every row which can be called later in main
          columnsIn += 1;
          currentColumn = currentColumn->getNext();
     }
     
     //std::cout << rowsIn << " " << columnsIn; // Tester code


    // Close the file
    inputFile.close();
}

void Auditorium::write(std::string file) {

     std::ofstream writer;      
     writer.open(file); 
     
     if(writer.is_open()){ // writing to A1.txt for every element in auditorium (within given rows and columns)

          Node* currentRow = first;

          while (currentRow != nullptr) {

               Node* currentNode = currentRow;
               while (currentNode != nullptr) {
                    
                    writer << currentNode->getPayload().getTicketType();  //Write each seat character into text file 
               
                    currentNode = currentNode->getNext();
               }
               writer << "\n";  //Insert newline at the end of each row
               currentRow = currentRow->getDown();
    }
    
     writer.close(); // close file :)

     }
}

void Auditorium::calculateReport() {
     Node* currentRow = first;

     while (currentRow != nullptr) {

          Node* currentNode = currentRow;
          while (currentNode != nullptr) {
     
               switch(currentNode->getPayload().getTicketType()){ // sSitch case for every value in auditorium array
                    case 'A':     // Every seat type adds to the total tickets/seats/and type, while empty seats add to total seats
                         totalAdult++;
                         totalSeats++;
                         totalTickets++;
                    break;

                    case 'C':
                         totalChild++;
                         totalSeats++;
                         totalTickets++;
                    break;

                    case 'S':
                         totalSenior++;
                         totalSeats++;
                         totalTickets++;
                    break;
                    
                    case '.':
                         totalSeats++;
                    break;
                    
                    default:
                    break;
               }
          
               currentNode = currentNode->getNext();
          }
          currentRow = currentRow->getDown();
    }

     totalSales = float(totalAdult) * 10 + float(totalChild) * 5 + float(totalSenior) * (float)7.5; // Total sales by multiplying price of each ticket type
}

void Auditorium::displayReport(){
     std::cout << "Total Seats:    " << totalSeats << std::endl;
     std::cout << "Total Tickets:  " << totalTickets << std::endl;
     std::cout << "Adult Tickets:  " << totalAdult << std::endl;
     std::cout << "Child Tickets:  " << totalChild << std::endl;
     std::cout << "Senior Tickets: " << totalSenior << std::endl;
     std::cout << "Total Sales:   $" << std::fixed << std::setprecision(2) << totalSales << std::endl;
}



void Auditorium::reserveSeats(int row, char column, int numAdults, int numChildren, int numSeniors) {
    Node* currentRow = first;

    while (currentRow != nullptr) {
        if (currentRow->getPayload().getRow() == row) {
            Node* currentNode = currentRow;
            while (currentNode != nullptr) {
                if (currentNode->getPayload().getColumn() == column) { // Goes through each node and sets as A, C, or S
                    while (currentNode != nullptr && numAdults > 0 && currentNode->getPayload().getTicketType() == '.') {
                        currentNode->setPayloadSeat('A');
                        --numAdults;
                        currentNode = currentNode->getNext();
                    }

                    while (currentNode != nullptr && numChildren > 0 && currentNode->getPayload().getTicketType() == '.') {
                        currentNode->setPayloadSeat('C');
                        --numChildren;
                        currentNode = currentNode->getNext();
                    }

                    while (currentNode != nullptr && numSeniors > 0 && currentNode->getPayload().getTicketType() == '.') {
                        currentNode->setPayloadSeat('S');
                        --numSeniors;
                        currentNode = currentNode->getNext();
                    }
                }
                if(currentNode != nullptr)
                currentNode = currentNode->getNext();
            }
        }

        currentRow = currentRow->getDown();
    }
}

void Auditorium::removeSeat(int row, char column){
    Node* currentRow = first;

    while (currentRow != nullptr) {
        if (currentRow->getPayload().getRow() == row) {
            Node* currentNode = currentRow;
            while (currentNode != nullptr) {
                if (currentNode->getPayload().getColumn() == column) { // Goes through each node and sets as A, C, or S
                    currentNode->setPayloadSeat('.');
                }
                if(currentNode != nullptr)
                currentNode = currentNode->getNext();
            }
        }

        currentRow = currentRow->getDown();
    }
}


//Checks if a seat and the following (quantity - 1) seats are available
bool Auditorium::checkAvailability(int row, char column, int quantity){

     bool isAvailable = false; // Initalizes false if row and column are not in the auditorium somehow
     Node* currentRow = first;

     while (currentRow != nullptr) {
        if (currentRow->getPayload().getRow() == row) {
            Node* currentNode = currentRow;
            while (currentNode != nullptr) {
                
                if (currentNode->getPayload().getColumn() == column) { // Sets isAvailable to true, and if any of the following are not . (empty seats), then isAvailable becomes false
                    isAvailable = true;
                    
                    int tempQuantity = quantity;
                    
                    while(currentNode != nullptr && tempQuantity > 0){
                         if(currentNode == nullptr || currentNode->getPayload().getTicketType() != '.'){
                              isAvailable = false;
                         }
                         
                         tempQuantity--; // Decrements quantity to make sure only the quantity amount are tested
                         currentNode = currentNode->getNext();
                         
                         if(currentNode == nullptr && tempQuantity > 0){
                              isAvailable = false;
                         }

                    }
                    
                    
                }

                if(currentNode != nullptr)
                     currentNode = currentNode->getNext();
            }
        }

        currentRow = currentRow->getDown();
    }

     return isAvailable;
}

Seat Auditorium::bestAvailable(int numTickets)
{
   bool seatsFound = false;
   double middleRow = (1.0 + (double)this->getRows()) / 2.0;
   double middleColumn = (1.0 + (double)this->getColumns()) / 2.0;

   double bestDistance = sqrt(pow(0 - middleRow, 2) + pow(0 - middleColumn, 2));
   int bestRow = 0;
   char bestColumn = '_';
   Node *currentRow = first;

   Seat bestAvailableSeat;
   while(currentRow != nullptr)
   {
        Node *currentNode = currentRow;
        while(currentNode != nullptr)
        {
            if(checkAvailability(currentNode->getPayload().getRow(), currentNode->getPayload().getColumn(), numTickets) == true)
            {
                seatsFound = true;
                double rowForDistance = currentNode->getPayload().getRow();
                double colForDistance = (currentNode->getPayload().getColumn() - 64) + (numTickets - 1) / 2.0;
                double distanceToMiddle = sqrt(pow(rowForDistance - middleRow, 2) + pow(colForDistance - middleColumn, 2));
                if(bestDistance > distanceToMiddle)
                {
                    bestDistance = distanceToMiddle;
                    bestRow = currentNode->getPayload().getRow();
                    bestColumn = currentNode->getPayload().getColumn();
                }
                else if(bestDistance == distanceToMiddle)
                {
                    if(fabs(rowForDistance - middleRow) < fabs(bestRow - middleRow))
                    {
                       bestRow = currentNode->getPayload().getRow();
                       bestColumn = currentNode->getPayload().getColumn();
                    }
                    else if(fabs(rowForDistance - middleRow) == fabs(bestRow - middleRow))
                    {
                       if(rowForDistance < bestRow)
                       {
                            bestRow = currentNode->getPayload().getRow();
                            bestColumn = currentNode->getPayload().getColumn();
                       }
                    }
                }
            }
            currentNode = currentNode->getNext();
        }
        currentRow = currentRow->getDown();
   }
   if(seatsFound)
   {
        bestAvailableSeat.setRow(bestRow);
        bestAvailableSeat.setColumn((char)(bestColumn));
   }
   return bestAvailableSeat;
}




// Prints out every row and column number alongside the contents of the auditorium with #'s and .'s
std::ostream& operator<<(std::ostream& os, const Auditorium& auditorium) {
    
    Node* currentRow = auditorium.first;

    // Print column letters at the top
    os << "   ";
    Node* tempNode = currentRow;
    while (tempNode != nullptr) {
        os << tempNode->getPayload().getColumn() ;
        tempNode = tempNode->getNext();
    }
    os << "\n";

    while (currentRow != nullptr) {
        os <<  currentRow->getPayload().getRow() << "  ";  // Prints row number

        Node* currentNode = currentRow;
        while (currentNode != nullptr) {
            if(currentNode->getPayload().getTicketType() == 'A' || currentNode->getPayload().getTicketType() == 'C' || currentNode->getPayload().getTicketType() == 'S' )         ////////////////
               os << '#' ;  // Print seat type 
            else if (currentNode->getPayload().getTicketType() == '.' )
               os << '.';
          
          currentNode = currentNode->getNext();
        }
        os << "\n";  // Insert newline at the end of each row
        currentRow = currentRow->getDown();
    }
    os << "\n";
    return os;
}