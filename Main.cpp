#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
using namespace std;

void creatDictionary(unordered_set<string>& list, vector<string>& dictionary) {
	dictionary.resize(256);
	for (int i = 0; i < 256; i++) {
		dictionary[i] = i;
		list.insert(dictionary[i]);
	}
}

vector<int> encode(string input_file, string output_file, unordered_set<string>& list, vector<string>& dictionary) {
	vector<int> ans(2, 0);
	ifstream file;
	file.open(input_file);
	ofstream fout;
	fout.open(output_file, ios::binary);
	char temp;
	string itr;
	bool check = false;
	while (!file.eof()) {
		if (!check) {
			file >> temp;
			itr += temp;
			ans[0] += 8;
			check = false;
		}
		if (list.find(itr) != list.end()) {
			string res;
			while (list.find(itr) != list.end() && !file.eof()) {
				res = itr;
				file >> temp;
				ans[0] += 8;
				itr += temp;
			}
			dictionary.push_back(itr);
			list.insert(itr);
			for (int i = 0; i < dictionary.size(); i++)
				if (res == dictionary[i]) {
					//cout << i << " ";
					fout.write((char*)&i, sizeof(i));
					if (i <= 255) ans[1] += 8;
					else ans[1] += 9;
					break;
				}
			itr = temp;
			check = true;
		}
	}

	file.close();
	fout.close();
	ans[0] -= 8;
	dictionary.resize(dictionary.size() - 1);
	return ans;

}

vector<int> decode(string input_file, string output_file, unordered_set<string>& list, vector<string>& dictionary) {
	ifstream file;
	file.open(input_file, ios::binary);
	ofstream fout;
	fout.open(output_file);
	vector<int> ans(2, 0);
	vector<int> arr;
	while (!file.eof()) {
		int temp;
		file.read((char*)&temp, sizeof(temp));
		if (temp <= 255) ans[0] += 8;
		else ans[0] += 9;
		arr.push_back(temp);
	}
	arr.resize(arr.size() - 1);
	ans[0] -= 8;
	int i = 0;
	string now;
	while (i < arr.size()) {
		if (arr[i] < dictionary.size()) {
			now += dictionary[arr[i]][0];
			if (list.find(now) == list.end()) {
				list.insert(now);
				dictionary.push_back(now);
			}
			//cout << dictionary[arr[i]];
			fout << dictionary[arr[i]];
			ans[1] += dictionary[arr[i]].length() * 8;
			now = dictionary[arr[i]];
		}
		else {
			now += now[0];
			cout << now;
			dictionary.push_back(now);
			list.insert(now);
		}
		i++;
	}

	file.close();
	fout.close();
	return ans;
}

int main(int argc, char* argv[]) {
	if (argc == 5) {
		string input_file = argv[2];
		string output_file = argv[3];
		string action = argv[1];

		if (action == "-e") {
			unordered_set<string> list;
			vector<string> dictionary;
			creatDictionary(list, dictionary);
			vector<int> ans = encode(input_file, output_file, list, dictionary);
			cout << endl;
			string output_Info = argv[4];
			if (output_Info == "-i") {
				cout << "Input size:  " << ans[0] << endl;
				cout << "Output size: " << ans[1] << endl;
				cout << "Space saved: " << (ans[0] - ans[1]) * 100.0 / ans[0] << "%" << endl;
			}
			else if (output_Info == "-d") {
				for (int i = 256; i < dictionary.size(); i++)
					cout << dictionary[i] << ": " << i << endl;
			}
			else if (output_Info == "-ind") {
				cout << "Input size:  " << ans[0] << endl;
				cout << "Output size: " << ans[1] << endl;
				cout << "Space saved: " << (ans[0] - ans[1]) * 100.0 / ans[0] << "%" << endl;

				cout << endl << endl;
				for (int i = 256; i < dictionary.size(); i++)
					cout << dictionary[i] << ": " << i << endl;

			}
		}

		else if (action == "-d") {
			unordered_set<string> list;
			vector<string> dictionary;
			creatDictionary(list, dictionary);
			vector<int> ans = decode(input_file, output_file, list, dictionary);
			cout << endl;
			string output_Info = argv[4];
			if (output_Info == "-i") {
				cout << "Input size:  " << ans[0] << endl;
				cout << "Output size: " << ans[1] << endl;
			}
			else if (output_Info == "-d") {
				for (int i = 256; i < dictionary.size(); i++)
					cout << dictionary[i] << ": " << i << endl;
			}
			else if (output_Info == "-ind") {
				cout << "Input size:  " << ans[0] << endl;
				cout << "Output size: " << ans[1] << endl;
				cout << endl << endl;
				for (int i = 256; i < dictionary.size(); i++)
					cout << dictionary[i] << ": " << i << endl;
			}

		}
	}

	return 0;
}













