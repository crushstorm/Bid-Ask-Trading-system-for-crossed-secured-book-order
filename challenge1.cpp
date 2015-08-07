#include <iostream>
#include <queue>
#include <string>
#include <map> 
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <set>
#include <sstream>
#include <unordered_map>

#define prepo 1e-5
using namespace std;

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const
  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

class stockDetail {
public:
	long timeStamp;
	string symbol;
	string typeofOrder;
	string side;
	long size;
	double price;
	long order_id;	

	stockDetail(long timeStamp,string symbol,string side,long size,double price, long order_id)
	{
		this->timeStamp = timeStamp;
		this->symbol = symbol;
		this->side = side;
		this->size = size;
		this->price = price;
		this->order_id = order_id;
	}
};

struct compare {
    bool operator()(const pair <long,double> &lhs, 
                    const pair <long,double> &rhs) const {
        return lhs.second < rhs.second;
    }
};
int main()
{
		cout << fixed; 
		cout << setprecision(4);
	unordered_map <pair<string,string>, multiset < pair <long,double> , compare> ,pairhash > stockMapping;
	string input;
	unordered_map <long,double> priceof; // find the price of the orderid
	unordered_map <string,int> locked; // check whether it's same or locked
	unordered_map <long,stockDetail*> orderidlookup; 
	pair <string,string> a;
	while (!cin.fail())
	{
		getline(cin,input);
		stringstream ss(input);
		string output;
		long timestamp;
		int counter = 0;
		bool add = false;
		bool ask = false;
		long ordertomodify;
		long orderid;
		long newsize;
		double newprice;
		double newamount;
		long sizetomodify;
		string symbol;
		while (ss >> output)
		{
			if (counter == 0)
			{
				timestamp = stoi(output);
			}
			else if (counter == 1)
			{
				if (output == "A")
				{
					add = true;
				}
			}
			else if (counter == 2)
			{
				if (add)
				{
					if (output == "A")
					{
						ask = true;
					}
				}
				else {
					orderid = stoi(output);
				}
			}
			else if (counter == 3)
			{
				if (add) {
					symbol = output;
				}
				else
				{
					sizetomodify = stoi(output);
				}	
			}	
			else if (counter == 4)
			{
				if (add)
				{
					newsize = stoi(output);
				}
				else 
				{ // modified new price
					newamount = stod(output);
				}
			}
			else if (counter == 5)
			{
				newprice = stod(output);
			}
			else if (counter == 6)
			{
				orderid = stoi(output);
			}
			counter++;
		}
		if (add)
		{
			if (ask)
			{
				stockMapping[make_pair("A",symbol)].insert(make_pair(orderid,newprice));
				orderidlookup[orderid] = new stockDetail(timestamp,symbol,"A",newsize,newprice,orderid);
				priceof[orderid]  = newprice;
			}
			else {
				stockMapping[make_pair("B",symbol)].insert(make_pair(orderid,newprice));
				orderidlookup[orderid] = new stockDetail(timestamp,symbol,"B",newsize,newprice,orderid);
				priceof[orderid]  = newprice;
			}
			if (locked[symbol] == 0)
			{

				if (!stockMapping[make_pair("A",symbol)].empty() && !stockMapping[make_pair("B",symbol)].empty())
				{
					if (stockMapping[make_pair("A",symbol)].begin()->second < stockMapping[make_pair("B",symbol)].rbegin()->second)
					{
						locked[symbol] = 1;
						cout << timestamp << " " << symbol << " crossed " << stockMapping[make_pair("B",symbol)].rbegin()->second << " " << stockMapping[make_pair("A",symbol)].begin()->second << endl;
					

					}
					else if (abs(stockMapping[make_pair("A",symbol)].begin()->second - stockMapping[make_pair("B",symbol)].rbegin()->second) < prepo) {
						locked[symbol] = 1;
						cout << timestamp << " " << symbol << " locked " << stockMapping[make_pair("B",symbol)].rbegin()->second << " " << stockMapping[make_pair("A",symbol)].begin()->second << endl;
					}
				}

			
			}
		
		}
		else {
			pair <string,string> info;
			if (sizetomodify != 0)
			{
				double tempprice =  priceof[orderid];
				
				stockMapping[make_pair(orderidlookup[orderid]->side,orderidlookup[orderid]->symbol)].erase(make_pair(orderid,priceof[orderid]));
				stockMapping[make_pair(orderidlookup[orderid]->side,orderidlookup[orderid]->symbol)].insert(make_pair(orderid,newamount));
				orderidlookup[orderid]->size = sizetomodify;
				orderidlookup[orderid]->price = newamount;
				priceof[orderid] = newamount;
			
				if (locked[orderidlookup[orderid]->symbol] == 0)
				{
					if (!stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && !stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty())
					{
						if (stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second < stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second)
						{
							locked[orderidlookup[orderid]->symbol] = 1;
							cout << timestamp <<" " << orderidlookup[orderid]->symbol << " locked" << stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second << " " << stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second;
						}
						else if (abs(stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second - stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second) < prepo) {
							locked[orderidlookup[orderid]->symbol] = 1;
							cout << timestamp << " " << orderidlookup[orderid]->symbol << " crossed" << stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second << " " << stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second;
						}
					}
				}
				else {
					if (!stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && !stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty())
					{
						if (stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second > stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second)
						{
							locked[orderidlookup[orderid]->symbol] = 0;
							cout << timestamp << " " << symbol << " normal";
						}
					}
				}

			}	
			else {
				stockMapping[make_pair(orderidlookup[orderid]->side,orderidlookup[orderid]->symbol)].erase(make_pair(orderid,priceof[orderid]));
				orderidlookup[orderid]->size = sizetomodify;
				orderidlookup[orderid]->price = newamount;
				priceof[orderid] = newamount;


				if (locked[orderidlookup[orderid]->symbol] == 0)
				{
					if (!stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && !stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty())
					{
						if (stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second < stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second)
						{
							locked[orderidlookup[orderid]->symbol] = 1;
							cout << timestamp << " " << orderidlookup[orderid]->symbol << " locked" << stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second << " " << stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second;
						}
						else if (abs(stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second - stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second) < prepo) {
							locked[orderidlookup[orderid]->symbol] = 1;
							cout << timestamp <<" " <<  orderidlookup[orderid]->symbol << " crossed" << stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second << " " << stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second;
						}
					}
				}
				else {
					if (!stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && !stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty())
					{
						if (stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].begin()->second > stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].rbegin()->second)
						{
							locked[orderidlookup[orderid]->symbol] = 0;
							cout << timestamp << " " << orderidlookup[orderid]->symbol << " normal";
						}
					}
					else if (stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && !stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty()) {
							locked[orderidlookup[orderid]->symbol] = 0;
							cout << timestamp << " " << orderidlookup[orderid]->symbol << " normal";

					}
					else if  (!stockMapping[make_pair("A",orderidlookup[orderid]->symbol)].empty() && stockMapping[make_pair("B",orderidlookup[orderid]->symbol)].empty()) {
						locked[orderidlookup[orderid]->symbol] = 0;
							cout << timestamp << " " << orderidlookup[orderid]->symbol << " normal";
					}
				}
			}
			
		}
	}
	
}