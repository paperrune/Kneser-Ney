#include <sstream>
#include <unordered_map>

using namespace std;

class KneserNey{
private:
	int *N;

	unordered_map<string, int> count;
	unordered_map<string, string> start_with;
	unordered_map<string, string> unique;
public:
	int gram;

	KneserNey(int gram);
	~KneserNey();

	void Train(string corpus);

	int Count(string word);

	double Probability(string t_1, string t_2 = "", string t_3 = "", string t_4 = "", bool highest = true);
};
