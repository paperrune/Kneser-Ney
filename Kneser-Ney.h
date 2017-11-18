#include "Dictionary.h"

class KneserNey{
private:
	int gram;

	int *N;

	Dictionary count;
	Dictionary start_with;
	Dictionary unique;

	double Probability(bool highest, char t_1[], char t_2[] = 0, char t_3[] = 0, char t_4[] = 0);
public:
	KneserNey(int gram);
	~KneserNey();

	void Train(char corpus[]);

	int Count(char word[]);

	double Probability(char t_1[], char t_2[] = 0, char t_3[] = 0, char t_4[] = 0);
};