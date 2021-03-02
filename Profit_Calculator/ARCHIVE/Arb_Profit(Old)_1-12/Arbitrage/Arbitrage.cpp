#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <boost/algorithm/string.hpp>
#include <iomanip>

using namespace std;


//Class made specifically for reading in CSV files. Reads in a CSV file into a vector of strings.
//This is Step (1) of the process
class CSVReader
{
    string fileName;
    string delimeter;
public:
    CSVReader(string filename, string delm = ",") :
        fileName(filename), delimeter(delm)
    { }

    vector<vector<string>> getData() {
        ifstream file(fileName);

        vector<vector<string> > dataList;

        string line = "";

        while (getline(file, line))
        {
            vector<string> vec;
            boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
            dataList.push_back(vec);
        }
        file.close();

        return dataList;
    }
};


double average(vector<double> v) {
    double sum = 0;
    for (int i = 0; i < v.size(); ++i) {
        sum = sum + v[i];
    }
    double avg = sum / v.size();
    return avg;
}

//Reads in the timestamps from the vector of strings into a list of doubles. 
//this is Step (2)
list<double> get_timestamps(vector<vector<string> > data) {
    list<double> timestamps;
    for (vector<string> vec : data) {
        timestamps.push_back(stod(vec[2]));
    }
    return timestamps;
}

//Reads in the prices from the vector of strings into a list of doubles
//this is Step (3)
list<double> organize_prices(vector<vector<string> > data) {
    list<double> prices;
    for (vector<string> vec : data) {
        string temp = vec[7];
        temp.erase(temp.begin(), temp.begin() + 11);
        temp.erase(temp.end() - 1);
        prices.push_back(stod(temp));
    }
    return prices;
}

vector<double> get_prices(vector<vector<string> > data) {
    vector<double> prices;
    for (vector<string> vec : data) {
        string temp = vec[1];
        prices.push_back(stod(temp));
    }
    return prices;
}

vector<double> temp_timestamps(vector<vector<string> > data) {
    vector<double> timestamps;
    for (vector<string> vec : data) {
        string temp = vec[0];
        timestamps.push_back(stod(temp));
    }
    return timestamps;
}

void fill_in_zeros(vector<double> &v) {
    for (int i = v.size() - 2; i >= 0; --i) {
        if (v[i] == 0) {
            v[i] = v[i + 1];
        }
    }
}

//Deleted code, not useful, only kept for future reference
/*vector<vector<int> > record_victim_indexes(list<double> timestamps) {
    vector<vector<int> > indexes;
    int sequence_num = 0;
    vector<int> start;
    indexes.push_back(start);
    vector<double> v{ make_move_iterator(begin(timestamps)), make_move_iterator(end(timestamps)) };
    for (int i = 1; i < v.size(); ++i) {
        if (v[i] == v[i-1]) {
            indexes[sequence_num].push_back(i);
        }
        else if (i > 1 && v[i-1] == v[i-2]) {
            vector<int> vec;
            indexes.push_back(vec);
            ++sequence_num;
        }
    }
    return indexes;
}*/

//if there are multiple price entries with the same timestamp, the average value is used and the duplicate values are deleted.
void full_price_map(list<double> &timestamps, list<double> &prices) {
    vector<double> average_prices;
    vector<vector<int> > index_list;
    for (int i = 0; i < timestamps.size(); ++i) {
        list<double>::iterator it = timestamps.begin();
        advance(it, i);
        list<double>::iterator ip = prices.begin();
        advance(ip, i);
        vector<int> indexes;
        vector<double> price_temp;
        double avg;
        if (*it == *prev(it) && *prev(it) == *timestamps.begin()) {
            indexes.push_back(0);
            indexes.push_back(i);
            price_temp.push_back(*prev(ip));
            price_temp.push_back(*ip);
        }
        else if (*it == *prev(it)) {
            indexes.push_back(i);
            price_temp.push_back(*ip);
        }
        else if (indexes.size() > 0) {
            avg = average(price_temp);
            average_prices.push_back(avg);
            index_list.push_back(indexes);
            price_temp.clear();
            indexes.clear();
        }
    }
    for (int i = 0; i < index_list.size(); ++i) {
        for (int j = 0; j < index_list[i].size(); ++j) {
            list<double>::iterator it = timestamps.begin();
            advance(it, index_list[i][j]);
            timestamps.erase(it);
        }
    }
    for (int i = 0; i < index_list.size(); ++i) {
        list<double>::iterator ip = prices.begin();
        advance(ip, index_list[i][1]);
        *ip = average_prices[i];
        for (int j = 1; j < index_list[i].size(); ++j) {
            list<double>::iterator ip2 = prices.begin();
            advance(ip2, index_list[i][j]);
            prices.erase(ip2);
        }
    }
}

//If there is a timestamp missing, it is filled in here
void fill_missing_prices(list<double> &timestamps, list<double> &prices) {
    for (int i = timestamps.size() - 1; i >= 0; --i) {
        list<double>::iterator it = timestamps.begin();
        advance(it, i);
        list<double>::iterator ip = prices.begin();
        advance(ip, i);
        if (*it < 1543874197) {
            timestamps.erase(it);
            timestamps.erase(ip);
        }
    }
    /*cout << "done erasing" << endl;
    for (double i = 1543874197; i < 1543894762; ++i) {
        int existence = 0;
        list<double>::iterator it = timestamps.begin();
        list<double>::iterator ip = prices.begin();
        for (int j = 0; j < timestamps.size(); ++j) {
            advance(it, j);
            advance(ip, j);
            if (*it == i) ++existence;
            if (*it > i) break;
        }
        list<double>::iterator itx = timestamps.begin();
        list<double>::iterator ipx = prices.begin();
        if (existence == 0) {
            for (int j = 0; j < timestamps.size(); ++j) {
                advance(itx, j);
                advance(ipx, j);
                if (*itx > j) break;
            }
        }
        cout << setprecision(10) << i << endl;
        timestamps.insert(prev(itx), i);
        prices.insert(prev(ipx), *ipx);
    }*/
}

int main() {
    CSVReader read_in("ethbtc_top.csv"); // Change the name of the input file here **
    vector<vector<string> > dataList = read_in.getData();
    vector<double> prices = get_prices(dataList);
    vector<double> timestamps = temp_timestamps(dataList);
    fill_in_zeros(prices);
    //full_price_map(timestamps, prices);
    //fill_missing_prices(timestamps, prices);
    for (int i = 0; i < prices.size(); ++i) {
        cout << setprecision(10) << timestamps[i] << "," << prices[i] << endl;
    }
    return 0;
}
