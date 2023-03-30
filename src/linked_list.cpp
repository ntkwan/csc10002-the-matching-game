#include "linked_list.h"

LinkedList::LinkedList()
          : head(nullptr),
            tail(nullptr),
            list_size(0) {}

Cell* LinkedList::getHead() { return head; }

Cell* LinkedList::getTail() { return tail; }

Cell* LinkedList::createNode(Cell* _node) {
    Cell* newNode = new Cell(_node);
return newNode;
}

bool LinkedList::isEmpty() {
    if (head == nullptr && tail == nullptr) return true;
return false;
}

void LinkedList::addHead(Cell* _node) {
    Cell* new_node = createNode(_node);
    ++list_size;

    if (isEmpty() == true) {
        head = tail = new_node;
        return;
    }

    head->prev = new_node;
    new_node->next = head;
    head = new_node;
}

void LinkedList::addTail(Cell* _node) {
    Cell* new_node = createNode(_node);
    ++list_size;

    if (isEmpty() == true) {
        head = tail = new_node;
        return;
    }

    tail->next = new_node;
    new_node->prev = tail;
    tail = new_node;
}

void LinkedList::removeHead() {
    --list_size;
    if (isEmpty() == true) return;

    if (head->next == nullptr) {
        delete head;
        head = tail = nullptr;
        return;
    }

    Cell* next_node = head->next;
    delete head;
    head->next = nullptr;

    head = next_node;
}

void LinkedList::removeTail() {
    --list_size;
    if (isEmpty() == true) return;

    if (head->next == nullptr) {
        delete head;
        head = tail = nullptr;
        return;
    }

    Cell* next_node = head->prev;
    delete head;
    head->prev = nullptr;

    head = next_node;
}

void LinkedList::removeAll() {
    for (Cell* cur_node = head; cur_node != nullptr;) {
        Cell* next_node = cur_node->next;

        delete cur_node;
        cur_node->prev = nullptr;
        cur_node->next = nullptr;

        head = next_node;
        cur_node = next_node;
    }

    tail = head = nullptr;
}

Cell* LinkedList::getPos(int _idx) {
    if (isEmpty() == true) return nullptr;

    if (_idx < 0 || _idx >= list_size) return nullptr;

    Cell* cur_node = head;
    for (int i = 0; i < _idx; ++i) {
        cur_node = cur_node->next;
    }

return cur_node;
}

void LinkedList::printList() {
    Cell* cur_node = head;
    while (cur_node != nullptr) {
        std::cout<<cur_node->cell_value<<" ";
        cur_node = cur_node->next;
    }
}
