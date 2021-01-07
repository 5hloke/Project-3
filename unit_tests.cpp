#include "catch.hpp"

#include "Maze.hpp"

// implicitly testing solve and get
TEST_CASE("create valid", "[Stack]")
{
	INFO("Hint: creating a valid mazes");

	Maze m;

	REQUIRE(m.create(10, 10, 0, 8));

	std::cout << m;

	m.solve();
	std::cout << m;
}

TEST_CASE( "Test for default constructor", "[Stack]" )
{
  //INFO("Hint: creating a valid mazes");

	Maze m;
	std::vector<unsigned char> maze = m.get();
	REQUIRE(maze.empty());
	REQUIRE_FALSE(m.solve());
	/*m.read("maze.png");
	std::cout << m;
	m.solve();*/
	/*std::cout << m.create(400, 400, 0, 8) << std::endl;
	std::cout << m.solve() << std::endl;
	std::cout << m;*/
	/*Maze m1;
	std::cout << m1.set(m.get(), 10) << std::endl;
	m1.read("maze.png");
	std::cout << m.solve() << std::endl;
	std::cout << m.create(10, 60, 0, 0) << std::endl;*/
	/*m.read("maze_solved.png");
	std::cout << m;
	std:: cout << m.solve() << std::endl;*/
	//Maze m1;
	//std::cout << m1.set(m.get(), 10) << std::endl;;
	//std::cout << m1;
	/*std::cout << m1.solve() << std::endl;
	std::cout << m1;*/
	//std::cout << m.solve() <<std::endl;
	//std::cout << m;

	//Maze m1;

	//m1.set(m.get(), 10);
    //REQUIRE(m.create(10,10,0,8));
	//std::cout << m1;

	//m.solve();
	//std::cout << m;
	//m1.solve();
	//std::cout << m1;
	//m1.write("maze1.png");
	//std::cout << m1;

  //m.solve();
  //std::cout << m;
}

TEST_CASE("Testing the Parameterized constructor", "[Stack]")
{
	Maze m(10, 60, 0, 8);
	std::vector<unsigned char> maze = m.get();
	REQUIRE(maze.size() == 600);
	REQUIRE(m.solve());
}

// Implictly tests the get method
TEST_CASE("Testing the set method", "[Stack]") {

	Maze m(10, 60, 0, 8);
	std::vector<unsigned char> maze = m.get();
	REQUIRE(maze.size() == 600);

	Maze m1;
	REQUIRE(m1.set(maze, 10));

	m.solve();
	REQUIRE_FALSE(m1.set(m.get(), 10));

	Maze m2;
	REQUIRE_FALSE(m1.set(m.get(), 10));
}

TEST_CASE("Testing the create method", "[Stack]") {

	Maze m;
	REQUIRE_FALSE(m.create(10, 60, 0, 0));
	REQUIRE(m.create(10, 60, 0, 8));

	Maze m1(10, 60, 0, 8);
	REQUIRE(m1.create(10, 60, 0, 8));
	REQUIRE(m.solve());
	REQUIRE(m1.solve());
}

//TEST_CASE("Testing reading a solved and unsolved maze", "[Stack]")
//{
//	Maze m;
//	REQUIRE(m.read("maze.png"));
//	REQUIRE(m.read("maze_solved.png"));
//	std::cout << m;
//	REQUIRE_FALSE(m.solve());
//}

TEST_CASE("Testing write", "[Stack]")
{
	Maze m(10, 60, 0, 8);
	REQUIRE(m.write("maze2.png"));
	m.solve();
	REQUIRE(m.write("maze2_solve.png"));

}

