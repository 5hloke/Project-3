#include "Stack.hpp"


template <typename T>
Stack<T>::Stack() {
	capacity = 100; // Setting the max capacity of the stack to a 100 
	items = new T[capacity]; // creating a new array of items with the given capacity 
	top = 0; // top represents the index where an item could be added (size)
}

template <typename T>
Stack<T>::~Stack() {

	clear(); // clears the stack 
}

template <typename T>
Stack<T>::Stack(const Stack& rhs) {

	capacity = rhs.top; // Set the capacity equal to the current size of the stack 
	top = rhs.top; // Top is set to the top of the stack to be copied 
	items = new T[top]; // create a new array of items with the capacity top 
	
	for (int i = 0; i < rhs.top; i++) { // Loop to copy al the elements in rhs to items 
		items[i] = rhs.items[i];
	}
}


template <typename T>

size_t Stack<T>::size() const {

	return top; // returns top which can also be used to denote the size of the stack 
}

template <typename T>

bool Stack<T>::isEmpty() const{

	bool flag;
	if (top == 0) { // If the size == 0 return true 
		flag = true; 
	}
	else { // If the size is not equal to 0 return false
		flag = false;
	}
	return flag; 
}

template <typename T>

bool Stack<T>::push(const T& newItem) {

	if (top >= capacity) { // If the size is exceeding or equal to the maximum capcaity of the stack then
		T* oitem = items; // set the items to another array 
		items = new T[2 * capacity]; // create a new array withtwice the capacity 
		for (int i = 0; i < capacity; i++) { // for loop to copy all the items in the old array to the new one
			items[i] = oitem[i]; // coipying the items in the old array to the new array with twice the capacity 
		}
		delete [] oitem; // delete the old item not needed anymore 
		oitem = nullptr; // set it equal to nullptr (do not need any dangling pointers)
		capacity = 2 * capacity; // set capacity equal to the current capcacity of the new array
	}
	items[top] = newItem; // if not the case then simply add the item 
	++top; // increment top by 1

	return true;
}

template <typename T>
bool Stack<T>::pop() {

	bool flag;
	if (top > 0) { // If the stack is not empty 
		--top; // then decrease the size of the stack by one and return true 
		flag = true;
	}
	else { // if the stack is empty do nothing and return false 
		flag = false;
	}
	return flag;

}

template <typename T>
const T& Stack<T>::peek() const throw(std::range_error) {
	
		if (isEmpty()) { // if isEmpty() is true then throw a range_error
			throw std::range_error("Peeking at an empty stack");
		}
		else { // if not return the last item added to the stack
			return items[top - 1];
		}
	
}

template <typename T>
void Stack<T>::clear() {

	delete[] items; // delete the items array 
	items = nullptr; // set it equal to nullptr
	top = 0; // and set the top = 0
}
