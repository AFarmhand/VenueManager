#ifndef NODE_H
#define NODE_H

#include "Seat.h"
#include <iostream>

class Node {
private:
     Node* next;
     Node* down;
     Seat payload;

public:
     Node();  
     Node(const Seat& payload);   
     
     Node* getNext() const;  
     Node* getDown() const;  
     Seat getPayload() const; 
     
     void setNext(Node* nextNode);  
     void setDown(Node* downNode); 
     void setPayloadSeat(char seat);

     
     friend std::ostream& operator<<(std::ostream& os, const Node& node);  
};

#endif 