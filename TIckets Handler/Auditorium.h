#ifndef AUDITORIUM_H
#define AUDITORIUM_H

#include "Node.h"
#include <string>

class Auditorium {
private:
    Node* first;  // Head pointer for the linked list
    int rowsIn = 1; // Number of rows
    int columnsIn = 1; // Number of columns
    
     int totalAdult = 0;
     int totalChild = 0;
     int totalSenior = 0;
     int totalSeats = 0;
     int totalTickets = 0;
     float totalSales = 0;


public:
    Auditorium();  // Constructor
    ~Auditorium();  // Destructor 
    
    void construct(std::string);  // Construct auditorium from inputted file
    void write(std::string file); // Writes auditorium into file A1.txt
    void calculateReport();
    void displayReport(); 
    
     // Getters for stuff
     int getTotalAdult() const { return totalAdult; }
     int getTotalChild() const { return totalChild; }
     int getTotalSenior() const { return totalSenior; }
     int getTotalSeats() const { return totalSeats; }
     int getTotalTickets() const { return totalTickets; }
     float getTotalSales() const { return totalSales; }

     void reserveSeats(int row, char column, int numAdults, int numChildren, int numSeniors);
     void removeSeat(int row, char column);
    
     bool checkAvailability(int row, char column, int quantity);
    
     Seat bestAvailable(int quantity);
     
     int getRows() const {return rowsIn;} 
     int getColumns() const {return columnsIn;}
     
     static Auditorium addAuditoriums(const Auditorium& auditorium1, const Auditorium& auditorium2, const Auditorium& auditorium3) {
          Auditorium result;

          // Add the private int values
          result.totalAdult = auditorium1.totalAdult + auditorium2.totalAdult + auditorium3.totalAdult;
          result.totalChild = auditorium1.totalChild + auditorium2.totalChild + auditorium3.totalChild;
          result.totalSenior = auditorium1.totalSenior + auditorium2.totalSenior + auditorium3.totalSenior;
          result.totalSeats = auditorium1.totalSeats + auditorium2.totalSeats + auditorium3.totalSeats;
          result.totalTickets = auditorium1.totalTickets + auditorium2.totalTickets + auditorium3.totalTickets;

          // Add the private float values
          result.totalSales = auditorium1.totalSales + auditorium2.totalSales + auditorium3.totalSales;
          return result;
     }

    
     friend std::ostream& operator<<(std::ostream& os, const Auditorium& auditorium);  // Overloaded << operator
};

#endif 