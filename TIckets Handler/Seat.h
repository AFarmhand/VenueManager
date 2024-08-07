#ifndef SEAT_H
#define SEAT_H

#include <iostream>

class Seat {
     
private:
     int row;
     char column;
     char ticketType;

public:
     Seat(int row = 0, char column = '_', char ticketType = '.');  //Default Constructor
     
     int getRow() const; 
     char getColumn() const; 
     char getTicketType() const;
     
     void setRow(int row);
     void setColumn(char column); 
     void setTicketType(char ticketType);

     friend std::ostream& operator<<(std::ostream& os, const Seat& seat);
     
};


#endif