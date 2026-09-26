# include "core/conversation.h"

//function definitions for Conversation class following Rule of 5
//Constructor
Conversation::Conversation() {

}
//Destructor
Conversation::~Conversation() {
    delete[] data_;
}
//Copy Constructor
Conversation::Conversation(const Conversation& other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_){

    if (capacity_ > 0) {
        data_ = new Message[capacity_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
}
//Copy Assignment Operator
Conversation& Conversation::operator=(const Conversation& other){
    // Check for self-assignment
    if (this != &other){
        return *this;
    }
    // Release existing resources
    delete[] data_;
    data_ = nullptr;
    size_ = other.size_;
    capacity_ = other.capacity_;

    if (capacity_ > 0) {
        data_ = new Message[capacity_];
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    return *this;
}
//Move Constructor
Conversation::Conversation(Conversation&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}
//Move Assignment Operator
Conversation& Conversation::operator=(Conversation&& other) noexcept {
    // Check for self-assignment
    if (this == &other) {
        return *this;
    }
    // Release existing resources
    delete[] data_;
    // Move resources from other
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    // Reset other
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

//append function to add a new message to the conversation
void Conversation::append(Message m) {
    if (size_ == capacity_) {
        // Need to grow the array
        std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        Message* new_data = new Message[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    data_[size_] = std::move(m);
    ++size_;
}
//size function to return the number of messages in the conversation
std::size_t Conversation::size() const noexcept {
    return size_;
}
//at function to access a message at a specific index with bounds checking
const Message& Conversation::at(std::size_t i) const {
    if (i >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return data_[i];
}