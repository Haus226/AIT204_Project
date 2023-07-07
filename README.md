# AIT204-Project
- Perform maze exploration using DFS and BFS
- Perform path finding using A* Algorithm
- Perform path scheduling using Ant Colony Optimization Algorithm, Genetic Algorithm and Simulated Annealing with starting point fixed.
## How to Use?
To use the program, follow these steps:
1)Move into the "bin" directory of the downloaded zip file in your terminal.
2)Compile the code by running the following command in the terminal:
g++ -O3 -o main.cpp main
This will produce a main.exe file.
3)To execute the program, use the following command in the terminal:
.\main.exe "path_to_your_testing_maze.txt" "heuristic_parameter" "number_of_directions" "-p" "-v"
- Replace "path_to_your_testing_maze.txt" with the actual path to your testing maze file.
- Replace "heuristic_parameter" with the desired heuristic parameter, such as "euc", "euc2", etc., as shown in the provided table of heuristic functions.
- Replace "number_of_directions" with either 4 or 8, depending on the desired number of directions.
- The arguments "-p" and "-v" are optional. They indicate whether you want to perform the permutation method to find the best visiting sequence and enable visualization, respectively.
- Note that the order of the arguments is not specific. You can pass them in any order you prefer.
Make sure to adjust the command and arguments according to your specific requirements and file paths.
4)After executing the program, you will find the result txt file and the visualization image in the "output" directory, which is located inside the "bin" directory.
5)Please note that each time you choose to enable visualization or permutation, the previous result txt file and visualization file will be deleted. This ensures that the latest results are displayed accurately.

