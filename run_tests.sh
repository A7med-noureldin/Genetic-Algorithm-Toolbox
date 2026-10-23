#!/bin/bash
# Put this file in your project folder and run:
#     bash run_tests.sh ./build/Genetic_Algorithm_Toolbox
# It creates a folder called tests/ (one input file per case) and runs all of them.
# For every case it shows what is EXPECTED and what the program GAVE, then PASS or FAIL.

BIN="${1:-./build/Genetic_Algorithm_Toolbox}"
if [ ! -x "$BIN" ]; then echo "Cannot find the program: $BIN"; exit 2; fi
mkdir -p tests
cat > tests/01_tiny_defaults.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
8








END_OF_CASE
cat > tests/02_no_feasible_solution.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
6








END_OF_CASE
cat > tests/03_impossible_data.in <<'END_OF_CASE'
1
0 0
1
3 4 9
1
1
8
END_OF_CASE
cat > tests/04_bad_menu_answers.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
8
1
abc
50
0
0.5
1.5







END_OF_CASE
cat > tests/05_settings_shown.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
8
20
50
0.9
2
5


0.08
2
10
END_OF_CASE
cat > tests/06_different_capacities.in <<'END_OF_CASE'
1
0 0
4
1 1 4
2 2 3
-1 1 5
-2 -2 6
2
2
10
8








END_OF_CASE
cat > tests/07_single_customer.in <<'END_OF_CASE'
1
0 0
1
3 4 2
1
1
5








END_OF_CASE
cat > tests/08_more_trucks_than_customers.in <<'END_OF_CASE'
1
0 0
2
1 0 3
0 2 4
5
1
10








END_OF_CASE
cat > tests/09_decimal_demands.in <<'END_OF_CASE'
1
0 0
2
1 0 0.1
2 0 0.2
1
1
0.3








END_OF_CASE
cat > tests/10_smallest_settings.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
8
2
1
1
2
1000
1
1
0.001
2
2
END_OF_CASE
cat > tests/11_input_ends_in_menu.in <<'END_OF_CASE'
1
0 0
3
3 4 4
6 8 3
-3 -4 5
2
1
8
50
END_OF_CASE
cat > tests/12a_16c_roulette_generational.in <<'END_OF_CASE'
1
50 50
16
41 19 7
50 83 1
6 9 4
68 12 1
46 74 9
7 64 3
27 4 5
11 55 7
53 8 3
30 11 9
70 54 2
7 72 5
15 28 9
80 80 3
74 7 2
73 74 4
4
1
21
100
200
0.8
1
1
1
0.05
1
END_OF_CASE
cat > tests/12b_16c_roulette_steady.in <<'END_OF_CASE'
1
50 50
16
41 19 7
50 83 1
6 9 4
68 12 1
46 74 9
7 64 3
27 4 5
11 55 7
53 8 3
30 11 9
70 54 2
7 72 5
15 28 9
80 80 3
74 7 2
73 74 4
4
1
21
100
200
0.8
1
1
1
0.05
2
20
END_OF_CASE
cat > tests/12c_16c_tournament_generational.in <<'END_OF_CASE'
1
50 50
16
41 19 7
50 83 1
6 9 4
68 12 1
46 74 9
7 64 3
27 4 5
11 55 7
53 8 3
30 11 9
70 54 2
7 72 5
15 28 9
80 80 3
74 7 2
73 74 4
4
1
21
100
200
0.8
2
3
1
1
0.05
1
END_OF_CASE
cat > tests/12d_16c_tournament_steady.in <<'END_OF_CASE'
1
50 50
16
41 19 7
50 83 1
6 9 4
68 12 1
46 74 9
7 64 3
27 4 5
11 55 7
53 8 3
30 11 9
70 54 2
7 72 5
15 28 9
80 80 3
74 7 2
73 74 4
4
1
21
100
200
0.8
2
3
1
1
0.05
2
20
END_OF_CASE
cat > tests/13_16_customers.in <<'END_OF_CASE'
1
50 50
16
41 19 7
50 83 1
6 9 4
68 12 1
46 74 9
7 64 3
27 4 5
11 55 7
53 8 3
30 11 9
70 54 2
7 72 5
15 28 9
80 80 3
74 7 2
73 74 4
4
1
21
100
300
0.8
2
3



2
20
END_OF_CASE
cat > tests/14_30_customers.in <<'END_OF_CASE'
1
50 50
30
30 75 4
69 16 5
47 77 7
60 80 5
74 8 7
77 1 9
60 33 7
70 29 6
24 91 9
60 69 7
70 60 4
50 81 6
19 29 1
81 19 5
66 49 3
94 1 6
85 99 9
8 20 2
97 75 4
5 38 5
99 3 5
34 60 2
76 92 2
49 91 8
100 54 8
50 93 2
73 56 6
17 46 2
12 4 7
17 63 3
5
1
36
100
300
0.8
2
3



2
20
END_OF_CASE
cat > tests/15_accuracy_8c_true_best_308.93.in <<'END_OF_CASE'
1
50 50
8
41 19 7
83 6 2
68 12 6
74 7 9
27 4 2
55 53 2
30 11 9
54 7 2
3
1
17











END_OF_CASE

pass=0; fail=0
run()  { OUT=$(timeout 120 "$BIN" < "tests/$1.in" 2>&1); CODE=$?; }
has()  { echo "$OUT" | grep -Eq -- "$1"; }
dist() { echo "$OUT" | grep "Total distance of all routes" | grep -Eo '[0-9.]+$'; }
feas() { echo "$OUT" | grep -Eo "Feasible: (yes|NO)"; }
got()  { echo "      got:      distance $(dist), $(feas)"; }
begin() { NAME="$1"; EXPECT="$2"; ok=1; echo "[$NAME]"; echo "      expected: $EXPECT"; }
need() { if ! has "$1"; then ok=0; echo "      MISSING in output: $1"; fi; }
forbid() { if has "$1"; then ok=0; echo "      SHOULD NOT appear: $1"; fi; }
finish() { if [ $ok = 1 ] && [ "$CODE" -eq 0 ]; then echo "      PASS"; pass=$((pass+1)); else echo "      FAIL (exit code $CODE)"; fail=$((fail+1)); fi; echo; }
# random cases: up to 3 tries, feasible and distance between $2 and $3
beats() {
    for i in 1 2 3; do
        run "$1"; d=$(dist)
        if has "Feasible: yes" && [ -n "$d" ] && awk "BEGIN { exit !($d < $3 && $d > $2) }"; then got; return 0; fi
    done
    got; echo "      (3 tries, none inside the range)"; return 1
}

echo "Testing: $BIN"; echo

begin 01_tiny_defaults "distance 30.00, Feasible: yes"
run 01_tiny_defaults; got; need "Total distance of all routes: 30.00"; need "Feasible: yes"; finish
begin 02_no_feasible_solution "WARNING overload 1.00, Feasible: NO"
run 02_no_feasible_solution; got; need "total overload = 1.00"; need "Feasible: NO"; finish
begin 03_impossible_data "message 'Impossible: ...' and NO settings menu"
run 03_impossible_data; need "Impossible: a customer needs 9"; forbid "Genetic algorithm settings"; finish
begin 04_bad_menu_answers "'Invalid, try again', then Population 50, Generations 500, Feasible: yes"
run 04_bad_menu_answers; got; need "Invalid, try again"; need "Population:  50"; need "Generations: 500"; need "Feasible: yes"; finish
begin 05_settings_shown "Tournament 5, Order crossover, Swap 0.08, Steady 10, Pc 0.9, Population 20, Generations 50"
run 05_settings_shown; got; need "Tournament.*= 5"; need "Order crossover"; need "Swap mutation.*= 0.08"; need "Steady state.*= 10"; need "Crossover probability Pc: 0.9"; need "Population:  20"; need "Generations: 50"; need "Feasible: yes"; finish
begin 06_different_capacities "Feasible: yes, loads 10 (cap 10) and 8 (cap 8)"
run 06_different_capacities; got; need "Feasible: yes"; need "Load of the route: 10.00 \(capacity 10.00\)"; need "Load of the route: 8.00 \(capacity 8.00\)"; finish
begin 07_single_customer "distance 10.00, Feasible: yes"
run 07_single_customer; got; need "Total distance of all routes: 10.00"; need "Feasible: yes"; finish
begin 08_more_trucks_than_customers "Feasible: yes, unused trucks not printed"
run 08_more_trucks_than_customers; got; need "Feasible: yes"; forbid "Route for vehicle 4"; finish
begin 09_decimal_demands "0.1 + 0.2 fits capacity 0.3, distance 4.00, Feasible: yes"
run 09_decimal_demands; got; need "Feasible: yes"; need "Total distance of all routes: 4.00"; finish
begin 10_smallest_settings "no crash, Feasible: yes"
run 10_smallest_settings; got; need "Best solution found"; need "Feasible: yes"; finish
begin 11_input_ends_in_menu "'Settings were not completed', no solution printed"
run 11_input_ends_in_menu; need "Settings were not completed"; forbid "Best solution found"; finish
for c in 12a_16c_roulette_generational 12b_16c_roulette_steady 12c_16c_tournament_generational 12d_16c_tournament_steady; do
    begin "$c" "no crash, Feasible: yes (distance about 500-650)"
    run "$c"; got; need "Best solution found"; need "Feasible: yes"; finish
done
begin 13_16_customers "distance between 499.3 and 600 (random search: about 627), Feasible: yes"
beats 13_16_customers 499.0 600 || ok=0; CODE=0; finish
begin 14_30_customers "distance below 1100 (random search: about 1197), Feasible: yes"
beats 14_30_customers 300 1100 || ok=0; CODE=0; finish
begin 15_accuracy_8c_true_best_308.93 "distance exactly 308.93 (the true best, found by brute force). Lower is impossible."
beats 15_accuracy_8c_true_best_308.93 308.92 308.94 || ok=0; CODE=0; finish

echo "RESULT: $pass passed, $fail failed"
[ $fail -eq 0 ]
