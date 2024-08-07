#include "Seat.h"


Seat::Seat(int row, char column, char ticketType)
    : row(row), column(column), ticketType(ticketType) {}

int Seat::getRow() const {
     return row;
}

char Seat::getColumn() const {
     return column;
}    

char Seat::getTicketType() const {
     return ticketType;
}

// Sets row position to reference back from node in Auditorium
void Seat::setRow(int r) {
     row = r;
}

// Sets column position to reference back from node in Auditorium
void Seat::setColumn(char c) {
     column = c;
}

void Seat::setTicketType(char ticket) {
     ticketType = ticket;
}

std::ostream& operator<<(std::ostream& os, const Seat& seat) {
    os << seat.getTicketType();
    return os;
}
