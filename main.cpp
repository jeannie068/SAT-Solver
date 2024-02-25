
//
//  main.cpp
//  CS_Final


#include <bits/stdc++.h>
using namespace std;

enum Cat {
    satisfied,
    unsatisfied,
    normal, //when the assignment has not been found till now
    completed //when the alg. has completed
};

class Formula {
    public:
      //a vector that stores the value assigned,-1 unsatisfied, 0 false 1 true
      vector<int> literals;
      //store the number of occurrences of each literal
      vector<int> literals_frequency;
      //store the difference in number of occurrences with
      // positive and negative polarity of each literal
      vector<int> literal_polarity;
      // vector to store the clauses
      // for each clauses, if the variable n is of positive polarity, then 2n is
      // stored if the variable n is of negative polarity, then 2n+1 is stored here,
      // n is assumed to be zero indexed

      vector<vector<int>> clauses; //儲存子句資料結構
      Formula() {}

      Formula(const Formula &f) { //把input資料吃進來
        literals = f.literals;
        clauses = f.clauses;
        literals_frequency = f.literals_frequency;
        literal_polarity = f.literal_polarity;
      }
};

class SATSolverDPLL {
    private:
        Formula formula;
        int literal_count;
        int clause_count;
        int unit_propagation(Formula &);
        int apply_transform(Formula &, int);
        static void show_result(Formula &, int, const char * argv);
        int DPLL(Formula, const char * argv);
    public:
        void initial(const char * argv);
        void solve(const char * argv);

};

void SATSolverDPLL::initial(const char * argv) {
    char commend;
    string s; //dummy string aka the cnf
    ifstream infile(argv, ios::in);
    printf("DPLL SAT Solver\n");

    // 讀cnf格式
    infile >> s >> s;
    
    infile >> literal_count;
    //outfile << "literal_count: " << literal_count << endl;
    infile >> clause_count;
    //outfile << "clause_count: " << clause_count << endl;

    //set all the vector to be appropriate size and the initial values
    formula.literals.clear();
    formula.literals.resize(literal_count, -1);

    formula.clauses.clear();
    formula.clauses.resize(clause_count);

    formula.literals_frequency.clear();
    formula.literals_frequency.resize(literal_count, 0);
    formula.literal_polarity.clear();
    formula.literal_polarity.resize(literal_count, 0);

    int literal=0;
    for (int i = 0; i < clause_count; ++i) {
        while (true) {
            infile >> literal;
            if (literal > 0) {
                formula.clauses[i].push_back(2 * (literal - 1));
                formula.literals_frequency[literal - 1]++;
                formula.literal_polarity[literal - 1]++;
            } else if (literal < 0) {
                formula.clauses[i].push_back(2 * ((-1) * literal - 1) + 1);
                formula.literals_frequency[-1 - literal]++;
                formula.literal_polarity[-1 - literal]--;
            } else { // 讀到0
                break;
            }
        }
    } // end 讀取資料
    /*
    outfile << "literals_frequency :";
    for (int i = 0; i < literal_count; ++i) {
        outfile << formula.literals_frequency[i] << " ";
    }
    outfile << endl;
     */
}


int SATSolverDPLL::unit_propagation(Formula &f) {
    bool unit_clause_found = false;
    if (f.clauses.size() == 0) {
        return Cat::satisfied;
    }
    do {
        unit_clause_found = false;
        for (int i = 0; i < f.clauses.size(); ++i) {
            if (f.clauses[i].size() == 1) {
                unit_clause_found = true;
                f.literals[f.clauses[i][0] / 2] = f.clauses[i][0] % 2;
                //0 - if true, 1 - if false, set the literal
                f.literals_frequency[f.clauses[i][0] / 2] = -1;
                int result = apply_transform(f, f.clauses[i][0] / 2);
                if (result == Cat::satisfied or result == Cat::unsatisfied) {
                    return result;
                }
                break;
            } else if (f.clauses[i].size() == 0) {
                return Cat::unsatisfied;
            }
        }
    } while (unit_clause_found);
    return Cat::normal; // assignment has not been found till now
}

int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply) {
    int value_to_apply = f.literals[literal_to_apply];
    // iterate over the clauses in f
    for (int i = 0; i < f.clauses.size(); ++i) {
        for (int j = 0; j < f.clauses[i].size(); ++j) {
            if ((2 * literal_to_apply + value_to_apply) == f.clauses[i][j]) {
                f.clauses.erase(f.clauses.begin() + i);
                i--;
                if (f.clauses.size() == 0) {
                    return Cat::satisfied;
                }
                break;
            } else if (f.clauses[i][j] / 2 == literal_to_apply) {
                f.clauses[i].erase(f.clauses[i].begin() + j);
                j--;
                if (f.clauses[i].size() == 0) {
                    return Cat::unsatisfied;
                }
                break;
            }
        }
    }
    return Cat::normal;
}

int SATSolverDPLL::DPLL(Formula f, const char * argv) {
    
    int result = unit_propagation(f);
    if (result == Cat::satisfied) {
        show_result(f, result, argv);
        return Cat::completed; // the alg. has completed
    } else if (result == Cat::unsatisfied) {
        return Cat::normal;
    }
    int i = distance(f.literals_frequency.begin(), max_element(f.literals_frequency.begin(), f.literals_frequency.end()));
    // find the most frequent literal
    for (int j = 0; j < 2; ++j) {
        Formula new_f = f;
        if (new_f.literal_polarity[i] > 0) { //考慮出現變量的正負，選定的變量如果正的多，先猜測其爲 p，反之 再猜測其值爲 非p
            new_f.literals[i] = j;
        } else {
            new_f.literals[i] = (j + 1) % 2;
        }
        new_f.literals_frequency[i] = -1;
        // reset the frequency to -1 to ignore in the future
        int transform_result = apply_transform(new_f, i);
        if (transform_result == Cat::satisfied) {
            show_result(new_f, transform_result, argv);
            return Cat::completed;
        } else if (transform_result == Cat::unsatisfied) {
            continue;
        }
        int dpll_result = DPLL(new_f, argv);
        if (dpll_result == Cat::completed) {
            return dpll_result;
        }
    }
    return Cat::normal;
}

void SATSolverDPLL::show_result(Formula &f, int result, const char * argv) {
    ofstream outfile(argv, ios::out);
    if (result == Cat::satisfied) {
        outfile << "s SATISFIABLE" << endl;
        for (int i = 0; i < f.literals.size(); ++i) {
            
            outfile << "v \" " << i+1 << " \" -> ";

            if (f.literals[i] != -1) {
                if (pow(-1, f.literals[i]) == -1) {
                    outfile << "False" << endl;
                }
                else outfile << "True" << endl;
                //outfile << pow(-1, f.literals[i]) * (i + 1);
            } else {
                outfile << "True" << endl;
                //outfile << i + 1 << "*"; // true or false皆可
            }
        }
        outfile << "Done" << endl;
    } else {
        outfile << "s UNSATISFIABLE" << endl;
    }
}

void SATSolverDPLL::solve(const char * argv) {
    int result = DPLL(formula, argv);
    if (result == Cat::normal) {
        show_result(formula, Cat::unsatisfied, argv);
    }
}

int main(int argc, const char * argv[]) {
    // argv[0] -> Final.out
    // argv[1] -> testcase1.cnf
    // argv[2] -> testcase1.txt
    SATSolverDPLL dpll;
    dpll.initial(argv[1]);
    dpll.solve(argv[2]);
    
    return 0;
}


/* Pseudo code
DPLL(CNF):
    while CNF含有單位子句（clause）:
        對該CNF使用單位傳播（Unit Propagation）;
    if CNF為空:
        return true;
    else if CNF有空子句:
        return false;
    else: // 仍不能確定CNF的值，需要對更多的變量賦值
        找到CNF中下一個沒有被賦值的變量（或literal）;
        return DPLL(CNF中這個變量為真) || DPLL(CNF中這個變量為假);
 
 CDCL(CNF):
     副本 = CNF // 創建CNF的副本，不更改原CNF
     while true:
         while 副本含有單位子句:
             對副本使用單位傳播;
         if 副本中含有取值為假的子句: // 發現衝突
             if 現在的決策層是0:
                 return false; // 不能滿足
             C = 子句學習(CNF, 副本) // 吸取教訓
             根據C回到一個更早的決策層; // 調整策略
         else:
             if 所有變量都被賦值:
                 return true; // 可滿足
             else: // 進行一次決策（決策就是一次嘗試，令某個文字為真，撞大運）
                 開始一個新的決策層;
                 找到一個未賦值的文字l;
                 副本 = 副本∧{l}
                 // 給l賦值為真
                 // 加入l構成的單位子句，使得副本要滿足就是l要滿足，變相地要求l為真
                 // 對於變量x，若給x賦值為真，就令l = x；若給x賦值為假，就令l = ¬x
*/
