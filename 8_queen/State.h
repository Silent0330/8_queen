#ifndef _STATE_H_
#define _STATE_H_
#include <vector>
#include <list>
#include <queue>

using namespace std;

class StateCompare;

class State{
	public:
		static const int board_size = 8;
		int board[board_size];
		int depth;
		int heuristic;
		int cost;
        int parent;

		bool operator==(const State &) const;

		State(int board[board_size]);
		
		bool IsGoal();
		void Expand(vector<State> &discovered, vector<State> &explored, bool useInExp = false);
		void Expand(priority_queue<State, vector<State>, StateCompare> &discovered, vector<State> &explored, bool useInExp = false);
		void Print();
		void PrintGoal();
		void PrintPath(vector<State> &explored);
		bool InExplored(vector<State> &explored);
		void HeuristicFunction();

};
#endif

#ifndef _STATECOMPARE_H_
#define _STATECOMPARE_H_
class StateCompare
{
public:
	int type;
	StateCompare(int functiontype);
	bool operator() (const State &lhs, const State &rhs)const;
};
#endif

