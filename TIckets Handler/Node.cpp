#include "Node.h"

// Default constructor sets all ptrs to null
Node::Node() : next(nullptr), down(nullptr), payload() {}

// Overloaded constructor with all null ptrs but a seat object
Node::Node(const Seat& payload) : next(nullptr), down(nullptr), payload(payload) {}

Node* Node::getNext() const {
     return next;
}

Node* Node::getDown() const {
     return down;
}

Seat Node::getPayload() const {
     return payload;
}

// Sets pointer to next element of row (next column)
void Node::setNext(Node* nextNode) {
     next = nextNode;
}

// Seats down pointer for next row
void Node::setDown(Node* downNode) {
     down = downNode;
}

// Sets the type of seat
void Node::setPayloadSeat(char seat){
     payload.setTicketType(seat);
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
     os << node.getPayload();
     return os;
}
