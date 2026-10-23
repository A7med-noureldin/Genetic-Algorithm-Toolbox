#ifndef APP_H
#define APP_H

#include "../cvrp/Problem.h"

class App {
public:
    App();
    ~App();
    void run();

private:
    BaseProblem* problem;    

    
    template <class C>
    void solve(Problem<C>* p);

};

#endif