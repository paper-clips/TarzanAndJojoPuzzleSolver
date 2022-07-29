# Change to where you installed the boost library
BOOST_HOME := C:"/Program Files"/boost/boost_1_70_0

puzzleSolver: puzzleSolver.cpp
	g++ -I $(BOOST_HOME) puzzleSolver.cpp -o puzzleSolver
