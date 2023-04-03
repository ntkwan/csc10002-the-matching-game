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

void LinkedList::addTail(Cell* _node, bool is_added) {
    Cell* new_node = createNode(_node);
    if (is_added == true) ++list_size;

    if (isEmpty() == true) {
        head = tail = new_node;
        return;
    }

    tail->next = new_node;
    new_node->prev = tail;
    tail = new_node;
}

void LinkedList::removeHead() {
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
    if (isEmpty() == true) return;

    if (head->next == nullptr) {
        delete head;
        head = tail = nullptr;
        return;
    }

    Cell* next_node = tail->prev;
    delete tail;
    tail->prev = nullptr;

    tail = next_node;
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

void LinkedList::shiftCellToLeft() {
    if (head == nullptr || isEmpty() == true) return;

    Cell* cur_node = head;

    while (cur_node != nullptr) {
        if (cur_node == head) {
            cur_node->setCellPosX(0);
        } else {
            cur_node->setCellPosX(cur_node->prev->cell_pos_x+1);
        }

        cur_node = cur_node->next;
    }
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

void LinkedList::removePos(int _idx) {
    if (isEmpty() == true) return;

    if (_idx < 0 || _idx >= list_size) return;

    if (head->next == nullptr) {
        if (_idx == 0) {
            removeHead();
            return;
        }

        return;
    }

    if (head->next->next == nullptr) {
        if (_idx == 0) {
            removeHead();
            shiftCellToLeft();
            return;
        }

        if (_idx == 1) {
            removeTail();
            return;
        }

        return;
    }

    if (_idx == 0) {
        removeHead();
        shiftCellToLeft();
        return;
    }

    if (_idx == list_size-1) {
        removeTail();
        return;
    }

    Cell* prev_node = nullptr;
    Cell* cur_node = head;
    Cell* next_node = head->next;

    for (int i = 0; i < _idx; ++i) {
        prev_node = cur_node;
        cur_node = next_node;
        next_node = next_node->next;
    }

    prev_node->next = next_node;
    next_node->prev = prev_node;
    delete cur_node;
    cur_node->next = nullptr;
    cur_node->prev = nullptr;

    shiftCellToLeft();
}

void LinkedList::printList() {
    Cell* cur_node = head;
    while (cur_node != nullptr) {
        std::cout<<cur_node->cell_value<<" ";
        cur_node = cur_node->next;
    }
}
