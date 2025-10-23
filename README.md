# To run
*This compiler is built solely for Macos* <br/>
`g++ -std=c++20 -Wall -Wextra -pthread -I./include -o chat main.cpp ./src/*.cpp` <br/>
Optionally: `g++ -std=c++20 -Wall -Wextra -pthread -fsanitize=address -g -I./include -o chat main.cpp ./src/*.cpp`

# Features
TCP socket communication between clients <br/>
Reactor design pattern with `kqueue` for Mac <br/>
HTTP endpoint at `:8080` using raw C++ http <br/>
Auto-creation and deletion of threads <br/>
Utilized Valgrind, LLDB, and -fsanitize=address for memory and error checking <br/>
Automated startup and shutdown utilizing `SIGINT` as the default stop check <br/>
