#include "lodepng.h"
#include "Maze.hpp"
#include "Stack.hpp"
#include <random>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <iomanip>

// Defauly constructor for the MAze class Initializes a maze with 0 rows and 0 cols 
Maze::Maze() {
    rows = 0;
    cols = 0;
    begin = 0;
    end = 0;
}

//Parameterized constructor for the Maze class creates a valid maze with the given number of rows and columns
// If te maze is invalid or the arguments passd into the function are invalid then the function throws an exception
Maze::Maze(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end) throw(std::invalid_argument) {

    // Initializing the parameters of the maze
    this->rows = rows;
    this->cols = cols;
    this->begin = begin;
    this->end = end;

    size_t neigh_index = 0;
    bool begin_access = false; // begin access is to check for the validity of the maze after the maze has been created
    bool end_access = false; // Similar to begin access


    std::size_t size = rows * cols; // Size of the maze 
    Stack<int> stack; // initializing the stack
    size_t curr_index = 0; // current index is set to zero 


    std::vector<std::size_t> neighbors; // vectors to hold the neighbors of the current index
    std::vector<std::size_t> neigh_possible; // Possible nighbors
    srand(time(NULL));


    int wall_ind; 
    
    // if any of the following paramter conditions meet then the exception is thrown
    if (rows <= 0 || cols <= 0 || end >= size || end < 0 || begin < 0 || begin >= size || begin == end) {
        throw std::invalid_argument("Invalid input arguments to the function");
    }
    else { // else if the parameters are valid then
        // first create a maze with all walls to start with 
        for (int i = 0; i < size; i++) {

            maze.push_back('#');

        }
        // setting the begining index as a ' ' breaking the wall at the index 
        maze[begin] = ' ';
        // push the index to the stack 
        stack.push(begin);
        while (!stack.isEmpty()) {
            //set curr_index to the index which is at the top of the stack
            curr_index = stack.peek();
            // pop the index
            stack.pop();
            // get neighbors of the current index
            neighbors = getNeighbors(curr_index);
            // for all the neighbors of the curr_index chek if the neighbors are valid
            for (int i = 0; i < neighbors.size(); i++) {
                // check if the neighbor is equal to the current index or not and check if the neighbor is a blank ' ' 
                // if both the conditions meet then the neighbor is a valid neighbor and push it to the vector containing valid
                // neighbors 
                if (neighbors[i] != curr_index && maze[neighbors[i]] != ' ') {
                    neigh_possible.push_back(neighbors[i]);
                    
                }
                
                
            }
            // randomizing the neighbor ndex to pick
            if (neigh_possible.size() > 0) {
                neigh_index = rand() % neigh_possible.size() + 0;
            }
            else {
                neigh_index = 0;
            }
            // if there are valid neighbors then 
            if (neigh_possible.size() != 0) {
                // push the current index back to the stack
                stack.push(curr_index);

                // randomize the vector 
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(neigh_possible.begin(), neigh_possible.end(), g);
                
                // get the the index of the wall between the neighbor and the current index 
                wall_ind = this->getWall(curr_index, neigh_possible[neigh_index]);

                // set the neighbor to a ' ' 
                maze[neigh_possible[neigh_index]] = ' ';

                // break the wall 
                maze[wall_ind] = ' ';

                // push the neighbor just used back to the stack 
                stack.push(neigh_possible[neigh_index]);

            }
            neigh_possible.clear();
        }
        // Set the begining of the maze to a B
        maze[begin] = 'B';
        //Set the the ending of the maze to a E
        maze[end] = 'E';
        // Get the successors of B and E 
        std::vector<std::size_t> successors_begin = getSuccessors(begin);
        std::vector<std::size_t> successors_end = getSuccessors(end);
        //Checking if both B and E are accessible if they are acccessible then begin_access an end_access are set to
        // true (they will continue to be false if the conditions do not hold true)
        for (int i = 0; i < successors_begin.size(); i++) {
            if (successors_begin[i] != begin && maze[successors_begin[i]] != '#') {
                begin_access = true;
            }
        }
        for (int i = 0; i < successors_end.size(); i++) {
            if (successors_end[i] != end && maze[successors_end[i]] != '#') {
                end_access = true;
            }
        }

    }
    // check if begin_access && end_access is true , if either of the two is false then throw an exception
    if (!begin_access || !end_access) {
        maze.clear();
        throw std::invalid_argument("Invalid input arguments to the function");
    }

}

//settigng/replacing the current maze if any with a valid maze that has been input with the given number of rows 
bool Maze::set(const std::vector<unsigned char>& maze, std::size_t rows) {

    bool flag = false;
    bool begin_access = false;
    bool end_access = false;

    // if the input maze is an empty maze then clear this-> maze and return false
    if (maze.size() == 0) {
        this->maze.clear();
        flag = false;
    }
    else {
        //setting the rows and column of this->maze
        this->rows = rows;
        cols = maze.size() / rows;
        
        //resetting the maze 
        this->maze.clear();
        if (this->maze.empty()) {
            for (int i = 0; i < maze.size(); i++) {
                this->maze.push_back('#');
            }
        }
        // for every element in the input maze 
        for (int i = 0; i < maze.size(); i++) {
            // if maze[i] is either a 'B' or ' ' or '#' or 'E' then the maze is a valid maze 
            if (maze[i] == 'B' || maze[i] == ' ' || maze[i] == '#' || maze[i] == 'E') {
                //std::cout << "Here -- " << std::endl;
                flag = true;
                this->maze[i] = maze[i];
                // setting the begining and the ending index 
                if (maze[i] == 'B') {
                    this->begin = i;
                }
                if (maze[i] == 'E') {
                    this->end = i;
                }

            }
            else { // otherwise return false
                return false;

            }
        }
        // after the maze is created
        flag = true;
        // get the successors of the begining and the ending index 
        std::vector<std::size_t> successors_begin = getSuccessors(begin);
        std::vector<std::size_t> successors_end = getSuccessors(end);
        //Checking if both B and E are accessible if they are acccessible then begin_access an end_access are set to
        // true (they will continue to be false if the conditions do not hold true)
        for (int i = 0; i < successors_begin.size(); i++) {
            if (successors_begin[i] != begin && maze[successors_begin[i]] != '#') {
                begin_access = true;
            }
        }
        for (int i = 0; i < successors_end.size(); i++) {
            if (successors_end[i] != end && maze[successors_end[i]] != '#') {
                end_access = true;
            }
        }

    }
    // check if begin_access && end_access is true , if either of the two is false then return false
    if (begin_access && end_access) {
        flag = true;
    }
    else {
        flag = false;
    }
    return flag;

}

const std::vector<unsigned char>& Maze::get() const {

    return maze; // return tha maze
}

// Creates a randomized maze with the given set of paramters and returns true if the maze is valid or false if the maze
// created or if any of tha parameters are invalid 
bool Maze::create(std::size_t rows, std::size_t cols, std::size_t begin, std::size_t end) {
    // Initializing the parameters of the maze
    this->rows = rows;
    this->cols = cols;
    this->begin = begin;
    this->end = end;


    bool flag = false;
    srand(time(NULL));
    size_t neigh_index = 0;


    std::size_t size = rows * cols;
    Stack<int> stack;


    maze.clear();
    size_t curr_index = 0;
    std::vector<std::size_t> neighbors;
    std::vector<std::size_t> neigh_possible;


    bool begin_access = false; // begin access is to check for the validity of the maze after the maze has been created
    bool end_access = false;

    
    int wall_ind; 
    // Theh function has the same functionality as that of the parameterized constructor 
    if (rows <= 0 || cols <= 0 || end >= size || end < 0 || begin < 0 || begin >= size || begin == end ) {
        flag = false;
        
    }
    else {
        // creating a maze with all walls initially
        for (int i = 0; i < size; i++) {

            maze.push_back('#');

        }
        // begin is set to ' ' 
        maze[begin] = ' ';
        // push being to the stack 
        stack.push(begin);
        // while stack is not empty
        while (!stack.isEmpty()) {
            // get the top element of the stack and set it equla to curr_index
            curr_index = stack.peek();
            //pop the element
            stack.pop();
            // get nieghbors of te index
            neighbors = getNeighbors(curr_index);
            for (int i = 0; i < neighbors.size(); i++) {
                // check if the neighbor is equal to the current index or not and check if the neighbor is a blank ' ' 
                // if both the conditions meet then the neighbor is a valid neighbor and push it to the vector containing valid
                // neighbors 
                if (neighbors[i] != curr_index && maze[neighbors[i]] != ' ') {
                    neigh_possible.push_back(neighbors[i]);
                }
            }
            //randomizing the index to pick
            if (neigh_possible.size() > 0) {
                neigh_index = rand() % neigh_possible.size() + 0;
            }
            else {
                neigh_index = 0;
            }
            // if the vector for the possible neighbors is not empty
            if (neigh_possible.size() != 0) {
                // push the current index to the stack
                stack.push(curr_index);
                // randomize the vector 
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(neigh_possible.begin(), neigh_possible.end(), g);

                // get the wall between the neighbor and the curr index if there is one
                wall_ind = this->getWall(curr_index, neigh_possible[neigh_index]);

                // create a path by breaking down the walls
                maze[neigh_possible[neigh_index]] = ' ';

                maze[wall_ind] = ' ';
                //push the chosen neighbor to the stack
                stack.push(neigh_possible[neigh_index]);

            }
            neigh_possible.clear();
        }
        
        // begining and the ending if the maze are set to 'B' and 'E' respectively
        maze[begin] = 'B';
        maze[end] = 'E';
        std::vector<std::size_t> successors_begin = getSuccessors(begin);
        std::vector<std::size_t> successors_end = getSuccessors(end);
        //Checking if both B and E are accessible if they are acccessible then begin_access an end_access are set to
        // true (they will continue to be false if the conditions do not hold true)
        for (int i = 0; i < successors_begin.size(); i++) {
            if (successors_begin[i] != begin && maze[successors_begin[i]] != '#') {
                begin_access = true;
            }
        }
        for (int i = 0; i < successors_end.size(); i++) {
            if (successors_end[i] != end && maze[successors_end[i]] != '#') {
                end_access = true;
            }
        }

    }
    // check if begin_access && end_access is true , if either of the two is false then return false
    if (begin_access && end_access) {
        flag = true;
    }
    else {
        flag = false;
    }
    return flag;
}

// the function to solve the maze 
bool Maze::solve() {

    bool result = false;
    // calls the backtrack function in call
    if (!maze.empty()) {
        result = backtrack(begin);
        maze[begin] = 'B';
    }
    return result;
}

// get the neighbors of the index that has been passed in 
std::vector<std::size_t> Maze :: getNeighbors(std::size_t ind) const {
    std::vector<std::size_t> neighbors;

    // x and y are the row and the column number of the index that has been passed in 
    int x = ind / cols;
    int y = ind % cols;

    // if the row number is greater than 1
    if (x > 1) {

        neighbors.push_back(ind - 2 * cols);
    }
    else { // otherwise push the index becasue there are no valid neighbors above the current index (can not be -1 because size_t is an unsigned int)
        neighbors.push_back(ind);
    }

    // if the row number is less than rows -2 
    if (x < rows - 2) {
        neighbors.push_back(ind + 2 * cols);
    }
    else {// otherwise push the index becasue there are no valid neighbors below the current index (can not be -1 because size_t is an unsigned int)
        neighbors.push_back(ind);
    }

    // if the column number is greater than one 
    if (y > 1) {

       
        neighbors.push_back(ind -2);
    }
    else {// otherwise push the index becasue there are no valid neighbors to the left of the current index (can not be -1 because size_t is an unsigned int)
       
        neighbors.push_back(ind);
    }

    // if the column number is less than cols -2
    if (y < cols - 2) {

        
        neighbors.push_back(ind+2);
    }
    else {// otherwise push the index becasue there are no valid neighbors to the right of the current index (can not be -1 because size_t is an unsigned int)
       
        neighbors.push_back(ind);
    }

    return neighbors;
}


// assuming that ind1 is the current index the counter is at, and ind2 is the index of the neighbour
int Maze::getWall(std::size_t ind1, std::size_t ind2) const {

    std::size_t wall_ind = 0;

    // checking the in which direction the wall we have to look for is in 
    if (ind2 - ind1 == 2) {

        wall_ind = ind1 + 1;

    }
    else if (ind2 - ind1 == -2) {

        wall_ind = ind1 - 1;

    }
    else if (ind2 - ind1 == cols * 2) {

        wall_ind = ind1 + cols;

    }
    else if (ind2 - ind1 == cols * -2) {

        wall_ind = ind1 - cols;

    }
    // check if there is a wall at the given index if yes then return the index other wise return -1
    if (maze[wall_ind] == '#') {
        return wall_ind;
    }
    else {

        return -1;
    }
}

// gets the successors of the current index
std::vector<std::size_t> Maze::getSuccessors(std::size_t ind) const {
    std::vector<std::size_t> successors;
    if (!maze.empty()) { // if the maze is not empty 
        int x = ind / cols;
        int y = ind % cols;

        if (x > 0) {
            successors.push_back(ind - cols);
        }
        else {// otherwise push the index becasue there are no valid successors above the current index (can not be -1 because size_t is an unsigned int)
            successors.push_back(ind);
        }

        if (x < rows - 1) {
            successors.push_back(ind + cols);
        }
        else {// otherwise push the index becasue there are no valid successors below the current index (can not be -1 because size_t is an unsigned int)
            successors.push_back(ind);
        }

        if (y > 0) {
            successors.push_back(ind - 1);
        }
        else {// otherwise push the index becasue there are no valid successors to the left of the current index (can not be -1 because size_t is an unsigned int)
            successors.push_back(ind);
        }

        if (y < cols - 1) {
            successors.push_back(ind + 1);
        }
        else {// otherwise push the index becasue there are no valid successors to the right of the current index (can not be -1 because size_t is an unsigned int)
            successors.push_back(ind);
        }
    }
    return successors;

}

// Backtrack method is to solve the maze 
bool Maze::backtrack(std::size_t ind) {
    // The maze will only solve if it is not empty
    if (!maze.empty()) {
        std::size_t curr_index = ind; // curr_index is the index 
        std::vector<std::size_t> successors;
        successors = getSuccessors(curr_index); // getSuccessors of the curr_index
        bool result;
        bool solved = false; // solved is false


        if (maze[curr_index] == 'E') {// check if the element at the curr_index is the end if yes then return 
            solved = true;
            return true;
        }
        if (maze[curr_index] == '#' || maze[curr_index] == '*') { // if the element at the curr_index is a wall or a visited index then return false
            solved = false;
            return false;
            // result = false;
        }
        else {//if(!solved) {

            maze[curr_index] = '*'; // setting the path to the solution
            if (backtrack(successors[0])) { // move up if possible 
                maze[begin] = 'B';
                result = true;
                return result;
            }
            else {
                result = false;
            }


            if (backtrack(successors[1])) { // move down if possible 
                maze[begin] = 'B';
                result = true;
                return result;
            }
            else {
                result = false;
            }



            if (backtrack(successors[2])) {// move left is possible 
                maze[begin] = 'B';
                result = true;
                return result;
            }
            else {
                result = false;
            }



            if (backtrack(successors[3])) {// move right if possible
                maze[begin] = 'B';
                result = true;
                return result;
            }
            else {
                result = false;
            }

             
            // If all of the motions return a false then the path is not a part of the solution as we are blocked
            // so backtrack and unmark the location
             // Unmarking this location
            if (!result) {
                maze[curr_index] = ' ';
            }

        }
    }
    
    return false;

}

// read avalid maze from a file 
bool Maze::read(std::string filename)
{
  bool flag = false;
  bool result = false;
  std::vector<std::uint8_t> pixels;
  std::cout << pixels.size() << std::endl;
  unsigned int w, h;

  bool begin_access = false;
  bool end_access = false;
  
  //read png
  if(filename.empty())
    flag = lodepng::decode(pixels, w, h, "maze.png");
  else
    flag = lodepng::decode(pixels, w, h, filename);

  if (flag == 0) { // if there were no errors in reading the maze from the file 
      maze.clear(); // clear the maze that is already there
      cols = w; // set the number of columns 
      rows = h; // set the number of rows
      // create a maze with all walls 
      for (int i = 0; i < w * h; i++) {
          maze.push_back('#');
      }
      // for every element in the maze 
      for (int i = 0; i < w * h; i++) {
          // check the color code and set the corressponding element 
          // # - Black 
          // B - green
          // ' ' - white 
          // * - blue 
          // E - red
          if (pixels[i * 4] == 0 && pixels[i * 4 + 1] == 0 && pixels[i * 4 + 2] == 0 && pixels[i * 4 + 3] == 255) {
              maze[i] = '#';
          }
          if (pixels[i * 4] == 255 && pixels[i * 4 + 1] == 255 && pixels[i * 4 + 2] == 255 && pixels[i * 4 + 3] == 255) {
              maze[i] = ' ';
          }
          if (pixels[i * 4] == 0 && pixels[i * 4 + 1] == 0 && pixels[i * 4 + 2] == 255 && pixels[i * 4 + 3] == 255) {
              maze[i] = '*';
          }
          if (pixels[i * 4] == 0 && pixels[i * 4 + 1] == 255 && pixels[i * 4 + 2] == 0 && pixels[i * 4 + 3] == 255) {
              maze[i] = 'B';
              begin = i;
          }
          if (pixels[i * 4] == 255 && pixels[i * 4 + 1] == 0 && pixels[i * 4 + 2] == 0 && pixels[i * 4 + 3] == 255) {
              maze[i] = 'E';
              end = i;
          }
      }
      result = true;
      // check if the begining and the ending of the maze just read and created are accessible or not
      std::vector<std::size_t> successors_begin = getSuccessors(begin);
      std::vector<std::size_t> successors_end = getSuccessors(end);
      for (int i = 0; i < successors_begin.size(); i++) {
          if (successors_begin[i] != begin && maze[successors_begin[i]] != '#') {
              begin_access = true;
          }
      }
      for (int i = 0; i < successors_end.size(); i++) {
          if (successors_end[i] != end && maze[successors_end[i]] != '#') {
              end_access = true;
          }
      }

  }
  // if the maze is vaalid then return true else return false
  if (begin_access && end_access) {
      result = true;
  }
  else {
      result = false;
  }
  return result;
}

bool Maze::write(std::string filename) const
{
  bool flag = false;

  std::vector<std::uint8_t> pixels = std::vector<std::uint8_t>(4*cols*rows,255); //RGBA values for each pixel in image

  for (int i = 0; i < maze.size(); i++) {
      // check the element and set the element to the given colr code 
       // # - Black 
       // B - green
       // ' ' - white 
       // * - blue 
       // E - red
      if (maze[i] == '#') {  // if # then rgba  - 0 0 0 255
 
          pixels[i * 4] = 0;
          pixels[i * 4 + 1] = 0;
          pixels[i * 4 + 2] = 0;
          pixels[i * 4 + 3] = 255;
      }
      if (maze[i] == ' ') { // if ' ' then rgba - 255 255 255 255  
          pixels[i * 4] = 255;
          pixels[i * 4 + 1] = 255;
          pixels[i * 4 + 2] = 255;
          pixels[i * 4 + 3] = 255;
      }
      if (maze[i] == '*') { // if * then rgba - 0 0 255 255

          pixels[i * 4] = 0;
          pixels[i * 4 + 1] = 0;
          pixels[i * 4 + 2] = 255;
          pixels[i * 4 + 3] = 255;
      }

      if (maze[i] == 'B') { // if B then rgba - 0 255 0 255 

          pixels[i * 4] = 0;
          pixels[i * 4 + 1] = 255;
          pixels[i * 4 + 2] = 0;
          pixels[i * 4 + 3] = 255;
      }

      if (maze[i] == 'E') { // if E then rgba - 255 0 0 255

          pixels[i * 4] = 255;
          pixels[i * 4 + 1] = 0;
          pixels[i * 4 + 2] = 0;
          pixels[i * 4 + 3] = 255;
      }
  }

  if(filename.empty())
    flag = lodepng::encode("maze.png", pixels, cols, rows);
  else
    flag = lodepng::encode(filename, pixels, cols, rows);
  
  return true;
}

std::ostream& operator<<(std::ostream& os, const Maze& m)
{
  for(int i = 0; i < m.rows*m.cols; i++)
    {
      if(i % m.cols == 0)
	os  << std::endl;
      
      os << m.maze[i];
    }

  os  << std::endl << std::endl;
  
  return os;
}

