//#include <string>
#include <iostream>
#include <vector>
//#include <algorithm>
#include <utility>
#include <string>
#include <fstream>
#include <deque>


class Timestamp {
public:
	//Constructor
	Timestamp(float a) 
		: time(a) {}
	//each vector represents <price, quantity/volume> pairs for either the bid or ask of the exchanges 
	std::deque<std::pair <float, float>> pair_1; // USD to BTC, we only need to look at the ask price
	std::deque<std::pair <float, float>> pair_2; // ETH to BTC, we only need to look at the bid price
	std::deque<std::pair <float, float>> pair_3; // ETH to USD, we only need to look at the bid price
	float time;


	//*************************THIS FUNCTION NEEDS TO BE UPDATED BASED ON JAINXIN'S FUNCTION FOR CREATING THE ORDERBOOK********************************
	//*************************************************************************************************************************************************
	//fills up the three pair vectors from the orderbook
	/*void populate(std::vector<std::pair <float, float>> a, std::string file) {
		std::pair <float, float> temp;

		for (int i = 0; i < DeltaT; ++i) {
			std::cin >> temp.first;
			std::cin >> temp.second;
			//repopulate pair_1
			temp.first = 0;
			temp.second = 0;
			a.push_back(temp);
		}
	}*/
	
	//This function does three things:
	//1) It finds the maximum amount of original currency you can invest that won't change any of the exchange rates
	//2) It finds the profit you would obtain from that amount invested
	//3) It updates the orderbook
	// The value returned is the profit from the current iteration
	float invest_update(float &invested) {

		float firstq; // quantity invested for first exchange
		float secondq; // quantity invested for second exchange
		float thirdq; // quantity invested for third exchange
		//Set highest amount invested equal to maxing out the first order in the first exchange
		firstq = pair_1.front().first * pair_1.front().second;
		secondq = pair_1.front().second;
		//Check if second exchange orders satisfies current values
		if (pair_2.front().second < secondq) {
			secondq = pair_2.front().second;
			firstq = pair_1.front().first * secondq;
		}
		thirdq = secondq*pair_2.front().first;
		//repeat for the third exchange
		if (pair_3.front().second < thirdq) {
			thirdq = pair_3.front().second;
			secondq = thirdq/pair_2.front().first;
			firstq = pair_1.front().first * secondq;
		}
		//Amount of profit gained from this iteration
		float revenue = thirdq * pair_3.front().first;

		//Update the orderbook
		pair_1.front().second -= secondq;
		pair_2.front().second -= secondq;
		pair_3.front().second -= thirdq;
		if (pair_1.front().second == 0) pair_1.pop_front();
		if (pair_2.front().second == 0) pair_2.pop_front();
		if (pair_3.front().second == 0) pair_3.pop_front();

		//return the amount of profit gained
		if (revenue > firstq) invested += firstq;
		return revenue - firstq;
	}
};

float profit_calc(Timestamp &time) {
	float total_invest = 0;
	float total_profit = 0;
	float add = 1;
	while (add > 0) {
		if (time.pair_1.empty() || time.pair_2.empty() || time.pair_3.empty()) {
			break;
		}
		add = time.invest_update(total_invest);
		if (add > 0) total_profit += add;
	}
	if (total_invest > 0) return total_profit;
	else return 0;
}


// ########################Edits need to be made so that it automatically reads in the starting timestamp and the ending timestamp######################################
// *********************************************************************************************************************************************************************
int main() {
	std::ifstream ask_one("ETH_USD_asks.txt");
	std::ifstream bid_two("ETH_BTC_bids.txt");
	std::ifstream bid_three("BTC_USD_bids.txt");
	int maxtime = 1547339238;
	for (int mintime = 1547321049; mintime <= maxtime; ++mintime) {
		Timestamp current(mintime);
		std::pair <float, float> a(1, 1);
		std::deque< std::pair <float, float> > x;
		for (int i = 0; i < 50; ++i) {
			x.push_back(a);
		}
		current.pair_1 = x;
		current.pair_2 = x;
		current.pair_3 = x;
		std::string trash;
		ask_one >> trash;
		bid_two >> trash;
		bid_three >> trash;
		for (int i = 0; i < 50; ++i) {
			ask_one >> current.pair_1[i].first;
			ask_one >> current.pair_1[i].second;
			bid_two >> current.pair_2[i].first;
			bid_two >> current.pair_2[i].second;
			bid_three >> current.pair_3[i].first;
			bid_three >> current.pair_3[i].second;

		}
		if (mintime >= 1547324941 && mintime <= 1547324944) {
			std::cout << "TIMESTAMP," << mintime << std::endl;
			for (int i = 0; i < 50; ++i) {
				std::cout << "ETHUSD," << current.pair_1[i].first << "," << current.pair_1[i].second << ",";
				std::cout << "ETHBTC," << current.pair_2[i].first << "," << current.pair_2[i].second << ",";
				std::cout << "BTCUSD," << current.pair_3[i].first << "," << current.pair_3[i].second << "," << std::endl;
			}
			std::cout << "PROFIT," << profit_calc(current) << std::endl;
			std::cout << std::endl;
		}
		//std::cout << mintime << "," << profit_calc(current) << "\n"; // use ">" to redirect output to a csv file
	}
	return 0;
}



