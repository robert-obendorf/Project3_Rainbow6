#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
using namespace std;

class metaTeamComps {
private:
	unordered_map<string, pair<string*, string*>> metaTeams; // <map, <defense, attack>>

public:
	metaTeamComps();
	metaTeamComps(vector<player> db);
	metaTeamComps(vector<string> maps, vector<string*> defenseOperators, vector<string*> attackOperators);

	void addMap(string map, string* defenseOperators, string* attackOperators);
	void addDefense(string map, string* defenseOperators);
	void addAttack(string map, string* attackOperators);

	string* idealOperators(string map, int team); //0 = defense; 1 = attack
};

metaTeamComps::metaTeamComps() {
	string bank_D[5] = { "Aruni", "Melusi", "Oryx", "Wamai", "Goyo" };
	string bank_A[5] = { "Zero", "Ace", "Iana", "Kali", "Amaru" };
	metaTeams["Bank_D"] = { bank_D, bank_A };
}

metaTeamComps::metaTeamComps(vector<player> db) {
	vector<vector<float>> defenseWinRates; //<maps<operators>>
	vector<vector<float>> attackWinRates;

	for (int i = 0; i < db.size(); i++) {
		for (int m = 0; m < db[i].getNumberOfMaps(); m++) {
			for (int d = 0; d < defenseOperatorSize; d++) {
				defenseWinRates[m][d] += db[i].getOperatorWinRate(defenseOperators[d], m);
				if (i != 0) {
					defenseWinRates[m][d] /= 2; //skip the first iteration and average results afterwards 
				}
			}
			for (int a = 0; a < attackOperatorSize; a++) {
				attackWinRates[m][a] += db[i].getOperatorWinRate(attackOperators[a], m);
				if (i != 0) {
					attackWinRates[m][a] /= 2;
				}
			}
		}
	}

	string max_D[5];
	for (int i = 0; i < defenseWinRates.size(); i++) { //iterate through each map
		for (int j = 0; j < 5; j++) {
			int maxIndex = max_element(defenseWinRates[i].begin(), defenseWinRates[i].end()) - defenseWinRates[i].begin(); //find the operator with the highest win rate
			max_D[j] = defenseOperators[maxIndex];
			defenseWinRates[i][maxIndex] = -1; //set previous highest to -1 to find second highest
		}
	}

	string max_A[5];
	for (int i = 0; i < attackWinRates.size(); i++) { 
		for (int j = 0; j < 5; j++) {
			int maxIndex = max_element(attackWinRates[i].begin(), attackWinRates[i].end()) - attackWinRates[i].begin(); 
			max_D[j] = defenseOperators[maxIndex];
			attackWinRates[i][maxIndex] = -1;
		}
	}


}

metaTeamComps::metaTeamComps(vector<string> maps, vector<string*> defenseOperators, vector<string*> attackOperators) {
	for (int i = 0; i < maps.size(); i++) {
		metaTeams[maps[i]].first = defenseOperators[i];
		metaTeams[maps[i]].second = attackOperators[i];
	}
}

void metaTeamComps::addMap(string map, string* defenseOperators, string* attackOperators) {
	metaTeams[map] = { defenseOperators, attackOperators };
}

void metaTeamComps::addDefense(string map, string* defenseOperators) {
	metaTeams[map].first = defenseOperators;
}

void metaTeamComps::addAttack(string map, string* attackOperators) {
	metaTeams[map].second = attackOperators;
}

string* metaTeamComps::idealOperators(string map, int team) {
	if (team == 0) {
		return metaTeams[map].first;
	}
	else if (team == 1) {
		return metaTeams[map].second;
	}
	else {
		return nullptr;
	}
}