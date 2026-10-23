#include "app.h"

#include <iostream>

#include "../include/ga/engine/GAEngine.h"
#include "../cvrp/CVRPProblem.h"

using namespace std;

App::App() {
    problem = 0;
}

App::~App() {
    delete problem;
}

template <class C>
void App::solve(Problem<C>* p) {
    p->readInputs();

    if (!p->validate()){
         return;
    }
    cout << "\nData accepted.\n";

    GAEngine<C> engine;
    if (!engine.askSettings(*p)) {
        cout << "\nSettings were not completed.\n";
        return;
    }
    engine.printSettings();

    engine.setProblem(p);
    if (!engine.run()){
         return;
    }

    cout << "\n=== Best solution found ===\n";
    p->printSolution(engine.best());
    cout << "Feasible: " << (engine.bestIsFeasible() ? "yes" : "NO") << "\n";
}

void App::run() {
    cout << "=== Genetic Algorithm Toolbox ===\n\n";
    
    CVRPProblem cvrpName;    
    cout << "Choose a problem:\n";
    cout << "  1) " << cvrpName.name() << "\n";
    cout << "Your choice: ";

    int choice = 0;
    while (cin >> choice) {
        if (choice == 1) {
            CVRPProblem* cvrp = new CVRPProblem();
            problem = cvrp;
            solve(cvrp);
            return;
        }
        cout << "Please enter 1: ";
    }
    cout << "\nNo choice entered.\n";
}