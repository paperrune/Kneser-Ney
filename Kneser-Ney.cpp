#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Kneser-Ney.h"

KneserNey::KneserNey(int gram){
	N = new int[this->gram = gram + 1];
}
KneserNey::~KneserNey(){
	delete[] N;
}

void KneserNey::Train(char corpus[]){
	char *buffer = new char[strlen(corpus) + 1];
	char *p;

	char **token;

	int number_words = 0;

	strcpy(buffer, corpus);
	for (p = strtok(buffer, " "); p; p = strtok(NULL, " "), number_words++){
		char *content;

		if (content = count.New_Search("<s>")){
			sprintf(content, "%d", atoi(content) + 1);
			count.Insert("<s>", content);
			delete[] content;
		}
		else{
			count.Insert("<s>", "1");
		}
	}

	strcpy(buffer, corpus);
	p = strtok(buffer, " ");
	token = new char*[number_words];

	for (int j = 0; p; p = strtok(NULL, " "), j++){
		token[j] = new char[strlen(p) + 1];
		strcpy(token[j], p);
	}
	delete[] buffer;

	for (int n = 0; n < gram; n++){
		for (int i = 0; i < number_words - n; i++){
			char *content;
			char *words = new char[2];

			strcpy(words, "");
			for (int j = 0; j <= n; j++){
				words = (char*)realloc(words, strlen(words) + strlen(token[i + j]) + 2);
				strcat(words, token[i + j]);
				if (j < n) strcat(words, " ");
			}
			if (content = count.New_Search(words)){
				if (atoi(content) <= gram) N[atoi(content)]--;

				sprintf(content, "%d", atoi(content) + 1);
				count.Insert(words, content);

				if (atoi(content) <= gram) N[atoi(content)]++;
				delete[] content;
			}
			else{
				count.Insert(words, "1");
				N[1]++;
			}

			if (n >= 1){
				char *temp[2];

				strcpy(words, "");
				for (int j = 1; j <= n; j++){
					strcat(words, token[i + j - 1]);
					if (j < n) strcat(words, " ");
				}
				if (content = start_with.New_Search(words)){
					if (!strstr(content, token[i + n])){
						content = (char*)realloc(content, strlen(content) + strlen(token[i + n]) + 2);
						strcat(content, " ");
						strcat(content, token[i + n]);
						start_with.Insert(words, content);
					}
					delete[] content;
				}
				else{
					start_with.Insert(words, token[i + n]);
				}

				strcpy(words, "");
				for (int j = 0; j <= n; j++){
					strcat(words, token[i + j]);
					if (j < n) strcat(words, " ");
				}

				switch (n){
				case 1:
					temp[0] = new char[strlen("* ") + strlen(token[i]) + 1];
					temp[1] = new char[strlen("* *") + 1];

					strcpy(temp[0], "* ");
					strcat(temp[0], token[i]);

					strcpy(temp[1], "* *");
					break;
				case 2:
					temp[0] = new char[strlen("* ") + strlen(token[i + 1]) + strlen(token[i + 2]) + 2];
					temp[1] = new char[strlen("* ") + strlen(token[i + 1]) + strlen(" *") + 1];

					strcpy(temp[0], "* ");
					strcat(temp[0], token[i + 1]);
					strcat(temp[0], " ");
					strcat(temp[0], token[i + 2]);

					strcpy(temp[1], "* ");
					strcat(temp[1], token[i + 1]);
					strcat(temp[1], " *");
					break;
				case 3:
					temp[0] = new char[strlen("* ") + strlen(token[i + 1]) + strlen(token[i + 2]) + strlen(token[i + 3]) + 3];
					temp[1] = new char[strlen("* ") + strlen(token[i + 1]) + strlen(token[i + 2]) + strlen(" *") + 2];

					strcpy(temp[0], "* ");
					strcat(temp[0], token[i + 1]);
					strcat(temp[0], " ");
					strcat(temp[0], token[i + 2]);
					strcat(temp[0], " ");
					strcat(temp[0], token[i + 3]);

					strcpy(temp[1], "* ");
					strcat(temp[1], token[i + 1]);
					strcat(temp[1], " ");
					strcat(temp[1], token[i + 2]);
					strcat(temp[1], " *");
					break;
				}

				for (int j = 0; j < 2; j++){
					if (content = unique.New_Search(temp[j])){
						if (!strstr(content, words)){
							content = (char*)realloc(content, strlen(content) + strlen(words) + 2);
							strcat(content, "/");
							strcat(content, words);
							unique.Insert(temp[j], content);

							delete[] content;
							content = count.New_Search(temp[j]);

							sprintf(content, "%d", atoi(content) + 1);
							count.Insert(temp[j], content);
						}
						delete[] content;
					}
					else{
						unique.Insert(temp[j], words);
						count.Insert(temp[j], "1");
					}
					delete[] temp[j];
				}
			}
			delete[] words;
		}
	}
	for (int j = 0; j < number_words; j++){
		delete[] token[j];
	}
	delete[] token;
}

int KneserNey::Count(char word[]){
	char *content = count.Search(word);

	if (content){
		return atoi(content);
	}
	return 0;
}

double KneserNey::Probability(char t_1[], char t_2[], char t_3[], char t_4[]){
	return Probability(true, t_1, t_2, t_3, t_4);
}
double KneserNey::Probability(bool highest, char t_1[], char t_2[], char t_3[], char t_4[]){
	double probability = 0;

	if (t_1 && t_2 && t_3 && t_4){
		char *p;
		char *numor = new char[strlen(t_1) + strlen(t_2) + strlen(t_3) + strlen(t_4) + 4];
		char *denom = new char[strlen(t_1) + strlen(t_2) + strlen(t_3) + 3];

		int number_uniques = 0;

		double discount = (double)N[1] / (N[1] + 2 * N[2]);

		strcpy(numor, t_1);
		strcat(numor, " ");
		strcat(numor, t_2);
		strcat(numor, " ");
		strcat(numor, t_3);
		strcat(numor, " ");
		strcat(numor, t_4);

		strcpy(denom, t_1);
		strcat(denom, " ");
		strcat(denom, t_2);
		strcat(denom, " ");
		strcat(denom, t_3);

		discount = 3 - 4 * discount * N[4] / N[3];

		if (highest){
			int denom_count = 0;

			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				char *temp = new char[strlen(t_1) + strlen(t_2) + strlen(t_3) + strlen(p) + 4];

				strcpy(temp, t_1);
				strcat(temp, " ");
				strcat(temp, t_2);
				strcat(temp, " ");
				strcat(temp, t_3);
				strcat(temp, " ");
				strcat(temp, p);

				denom_count += atoi(count.Search(temp));
				number_uniques++;

				delete[] temp;
			}
			delete[] p;

			if (p = count.Search(numor)){
				probability = (atoi(p) - discount) / denom_count;
			}
			probability += discount / denom_count * number_uniques * Probability(false, t_2, t_3, t_4);
		}
		else{
			char *temp = new char[strlen("* ") + strlen(t_1) + strlen(t_2) + strlen(t_3) + strlen(t_4) + 4];

			int numor_count = 0;
			int denom_count = 0;

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " ");
			strcat(temp, t_2);
			strcat(temp, " ");
			strcat(temp, t_3);
			strcat(temp, " ");
			strcat(temp, t_4);

			if (p = count.Search(temp)){
				probability = atoi(p) - discount;
			}

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " ");
			strcat(temp, t_2);
			strcat(temp, " ");
			strcat(temp, t_3);
			strcat(temp, " *");

			if (p = count.Search(temp)){
				probability /= (denom_count = atoi(count.Search(temp)));
			}
			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				number_uniques++;
			}
			delete[] p;

			probability += discount / denom_count * number_uniques * Probability(false, t_2, t_3, t_4);

			delete[] temp;
		}
		delete[] numor;
		delete[] denom;
	}
	else
	if (t_1 && t_2 && t_3){
		char *p;
		char *numor = new char[strlen(t_1) + strlen(t_2) + strlen(t_3) + 3];
		char *denom = new char[strlen(t_1) + strlen(t_2) + 2];

		int number_uniques = 0;

		double discount = (double)N[1] / (N[1] + 2 * N[2]);

		strcpy(numor, t_1);
		strcat(numor, " ");
		strcat(numor, t_2);
		strcat(numor, " ");
		strcat(numor, t_3);

		strcpy(denom, t_1);
		strcat(denom, " ");
		strcat(denom, t_2);

		discount = 2 - 3 * discount * N[3] / N[2];

		if (highest){
			int denom_count = 0;

			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				char *temp = new char[strlen(t_1) + strlen(t_2) + strlen(p) + 3];

				strcpy(temp, t_1);
				strcat(temp, " ");
				strcat(temp, t_2);
				strcat(temp, " ");
				strcat(temp, p);

				denom_count += atoi(count.Search(temp));
				number_uniques++;

				delete[] temp;
			}
			delete[] p;

			if (p = count.Search(numor)){
				probability = (atoi(p) - discount) / denom_count;
			}
			probability += discount / denom_count * number_uniques * Probability(false, t_2, t_3);
		}
		else{
			char *temp = new char[strlen("* ") + strlen(t_1) + strlen(t_2) + strlen(t_3) + 3];

			int numor_count = 0;
			int denom_count = 0;

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " ");
			strcat(temp, t_2);
			strcat(temp, " ");
			strcat(temp, t_3);

			if (p = count.Search(temp)){
				probability = atoi(p) - discount;
			}

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " ");
			strcat(temp, t_2);
			strcat(temp, " *");

			if (p = count.Search(temp)){
				probability /= (denom_count = atoi(count.Search(temp)));
			}
			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				number_uniques++;
			}
			delete[] p;

			probability += discount / denom_count * number_uniques * Probability(false, t_2, t_3);

			delete[] temp;
		}
		delete[] numor;
		delete[] denom;
	}
	else
	if (t_1 && t_2){
		char *p;
		char *numor = new char[strlen(t_1) + strlen(t_2) + 2];
		char *denom = new char[strlen(t_1) + 1];

		int number_uniques = 0;

		double discount = (double)N[1] / (N[1] + 2 * N[2]);

		strcpy(numor, t_1);
		strcat(numor, " ");
		strcat(numor, t_2);

		strcpy(denom, t_1);

		discount = 1 - 2 * discount * N[2] / N[1];

		if (highest){
			int denom_count = 0;

			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				char *temp = new char[strlen(t_1) + strlen(p) + 2];

				strcpy(temp, t_1);
				strcat(temp, " ");
				strcat(temp, p);

				denom_count += atoi(count.Search(temp));
				number_uniques++;

				delete[] temp;
			}
			delete[] p;

			if (p = count.Search(numor)){
				probability = (atoi(p) - discount) / denom_count;
			}
			probability += discount / denom_count * number_uniques * Probability(false, t_2);
		}
		else{
			char *temp = new char[strlen("* ") + strlen(t_1) + strlen(t_2) + 2];

			int numor_count = 0;
			int denom_count = 0;

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " ");
			strcat(temp, t_2);

			if (p = count.Search(temp)){
				probability = atoi(p) - discount;
			}

			strcpy(temp, "* ");
			strcat(temp, t_1);
			strcat(temp, " *");

			if (p = count.Search(temp)){
				probability /= (denom_count = atoi(count.Search(temp)));
			}
			for (p = strtok(start_with.New_Search(denom), " "); p; p = strtok(NULL, " ")){
				number_uniques++;
			}
			delete[] p;

			probability += discount / denom_count * number_uniques * Probability(false, t_2);

			delete[] temp;
		}
		delete[] numor;
		delete[] denom;
	}
	else
	if (t_1){
		char *temp = new char[strlen("* ") + strlen(t_1) + 1];

		strcpy(temp, "* ");
		strcat(temp, t_1);

		probability = (double)atoi(count.Search(temp)) / atoi(count.Search("* *"));

		delete[] temp;
	}
	return probability;
}