#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Kneser-Ney.h"

KneserNey::KneserNey(int gram){
	N = new int[(this->gram = gram) + 1];

	for (int n = 0; n < gram + 1; n++){
		N[n] = 0;
	}
}
KneserNey::~KneserNey(){
	delete[] N;
}

void KneserNey::Train(string corpus){
	istringstream buffer(corpus);

	vector<string> token;

	for (string s; getline(buffer, s, ' ');){
		token.push_back(s);
	}
	for (int n = 0; n < gram; n++){
		for (int i = 0; i < (int)token.size() - n; i++){
			string words;

			for (int j = 0; j <= n; j++){
				if (!words.empty()){
					words += ' ';
				}
				words += token[i + j];
			}
			if (count.find(words) != count.end()){
				auto c = count.find(words);

				if (c->second <= gram) N[c->second]--;
				c->second++;
				if (c->second <= gram) N[c->second]++;
			}
			else{
				count.insert(pair<string, int>(words, 1));
				N[1]++;
			}

			if (n >= 1){
				string temp[2];

				words = "";
				for (int j = 1; j <= n; j++){
					if (!words.empty()){
						words += ' ';
					}
					words += token[i + j - 1];
				}

				if (start_with.find(words) != start_with.end()){
					auto s = start_with.find(words);

					if (s->second.find(token[i + n]) == string::npos){
						s->second = s->second + ' ' + token[i + n];
					}
				}
				else{
					start_with.insert(pair<string, string>(words, token[i + n]));
				}

				words = "";
				for (int j = 0; j <= n; j++){
					if (!words.empty()){
						words += ' ';
					}
					words += token[i + j];
				}

				switch (n){
				case 1:
					temp[0] = "* " + token[i];
					temp[1] = "* *";
					break;
				case 2:
					temp[0] = "* " + token[i + 1] + ' ' + token[i + 2];
					temp[1] = "* " + token[i + 1] + " *";
					break;
				case 3:
					temp[0] = "* " + token[i + 1] + ' ' + token[i + 2] + ' ' + token[i + 3];
					temp[1] = "* " + token[i + 1] + ' ' + token[i + 2] + " *";
				}

				if (!temp[0].empty() && !temp[1].empty()){
					for (int j = 0; j < 2; j++){
						if (unique.find(temp[j]) != unique.end()){
							auto u = unique.find(temp[j]);

							if (u->second.find(words) == string::npos){
								auto c = count.find(temp[j]);

								c->second++;
								u->second = u->second + '/' + words;
							}
						}
						else{
							count.insert(pair<string, int>(temp[j], 1));
							unique.insert(pair<string, string>(temp[j], words));
						}
					}
				}
			}
		}
	}
	token.clear();
}

int KneserNey::Count(string word){
	auto c = count.find(word);

	if (c != count.end()){
		return c->second;
	}
	return 0;
}

double KneserNey::Probability(string t_1, string t_2, string t_3, string t_4, bool highest){
	double probability = 0;

	if (!t_1.empty() && !t_2.empty() && !t_3.empty() && !t_4.empty()){
		string numor = numor + t_1 + ' ' + t_2 + ' ' + t_3 + ' ' + t_4;
		string denom = denom + t_1 + ' ' + t_2 + ' ' + t_3;

		int number_uniques = 0;

		double discount = 3 - 4 * (double)N[1] / (N[1] + 2 * N[2]) * N[4] / N[3];

		if (!_finite(discount) || discount < 0 || 1 < discount) discount = 0;

		if (highest){
			int denom_count = 0;

			auto s = start_with.find(denom);

			if (s != start_with.end()){
				istringstream iss(s->second);

				for (string s; getline(iss, s, ' ');){
					denom_count += Count(denom + ' ' + s);
					number_uniques++;
				}
			}
			if (Count(numor)){
				probability = (Count(numor) - discount) / denom_count;
			}
			if (denom_count == 0){
				return probability;
			}
			probability += discount / denom_count * number_uniques * Probability(t_2, t_3, t_4, "", false);
		}
	}
	else
	if (!t_1.empty() && !t_2.empty() && !t_3.empty()){
		string numor = t_1 + ' ' + t_2 + ' ' + t_3;
		string denom = t_1 + ' ' + t_2;

		int number_uniques = 0;

		double discount = 2 - 3 * (double)N[1] / (N[1] + 2 * N[2]) * N[3] / N[2];

		if (!_finite(discount) || discount < 0 || 1 < discount) discount = 0;

		if (highest){
			int denom_count = 0;

			auto s = start_with.find(denom);

			if (s != start_with.end()){
				istringstream iss(s->second);

				for (string s; getline(iss, s, ' ');){
					denom_count += Count(denom + ' ' + s);
					number_uniques++;
				}
			}
			if (Count(numor)){
				probability = (Count(numor) - discount) / denom_count;
			}
			if (denom_count == 0){
				return probability;
			}
			probability += discount / denom_count * number_uniques * Probability(t_2, t_3, "", "", false);
		}
		else{
			string temp = "* " + t_1 + ' ' + t_2 + ' ' + t_3;

			int numor_count = 0;
			int denom_count = 0;

			if (Count(temp)){
				probability = Count(temp) - discount;
			}
			if (Count(temp = "* " + t_1 + ' ' + t_2 + " *")){
				probability /= (denom_count = Count(temp));
			}
			if (denom_count == 0){
				return probability;
			}

			auto s = start_with.find(denom);

			if (s != start_with.end()){
				istringstream iss(s->second);

				for (string s; getline(iss, s, ' ');){
					number_uniques++;
				}
			}
			probability += discount / denom_count * number_uniques * Probability(t_2, t_3, "", "", false);
		}
	}
	else
	if (!t_1.empty() && !t_2.empty()){
		string numor = t_1 + ' ' + t_2;
		string denom = t_1;

		int number_uniques = 0;

		double discount = 1 - 2 * (double)N[1] / (N[1] + 2 * N[2]) * N[2] / N[1];

		if (!_finite(discount) || discount < 0 || 1 < discount) discount = 0;
		
		if (highest){
			int denom_count = 0;

			auto s = start_with.find(denom);

			if (s != start_with.end()){
				istringstream iss(s->second);

				for (string s; getline(iss, s, ' ');){
					denom_count += Count(denom + ' ' + s);
					number_uniques++;
				}
			}
			if (Count(numor)){
				probability = (Count(numor) - discount) / denom_count;
			}
			if (denom_count == 0){
				return probability;
			}
			probability += discount / denom_count * number_uniques * Probability(t_2, "", "", "", false);
		}
		else{
			string temp = "* " + t_1 + ' ' + t_2;

			int numor_count = 0;
			int denom_count = 0;

			if (Count(temp)){
				probability = Count(temp) - discount;
			}
			if (Count(temp = "* " + t_1 + " *")){
				probability /= (denom_count = Count(temp));
			}
			if (denom_count == 0){
				return probability;
			}

			auto s = start_with.find(denom);

			if (s != start_with.end()){
				istringstream iss(s->second);

				for (string s; getline(iss, s, ' ');){
					number_uniques++;
				}
			}
			probability += discount / denom_count * number_uniques * Probability(t_2, "", "", "", false);
		}
	}
	else
	if (!t_1.empty() && Count("* *")){
		probability = (double)Count("* " + t_1) / Count("* *");
	}
	return probability;
}
