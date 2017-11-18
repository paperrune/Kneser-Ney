#include <stdio.h>

#include "Kneser-Ney.h"

int main(){
	char *corpus[] = { "a a a b", "a b b a", "c a a a" };

	int gram = 2;

	KneserNey KN = KneserNey(gram);

	for (int i = 0; i < sizeof(corpus) / sizeof(corpus[0]); i++){
		KN.Train(corpus[i]);
	}

	char *word[] = { "a", "b", "c", "a a", "a b", "a c", "b a", "b b", "b c", "c a", "c b", "c c" };

	printf("count\n");
	for (int i = 0; i < sizeof(word) / sizeof(word[0]); i++){
		printf("%s = %d\n", word[i], KN.Count(word[i]));

		if (i % 3 == 2){
			printf("\n");
		}
	}

	printf("probability\n");
	printf("p(a | a) = %lf\n", KN.Probability("a", "a"));
	printf("p(b | a) = %lf\n", KN.Probability("a", "b"));
	printf("p(c | a) = %lf\n", KN.Probability("a", "c"));
	printf("\n");

	printf("p(a | b) = %lf\n", KN.Probability("b", "a"));
	printf("p(b | b) = %lf\n", KN.Probability("b", "b"));
	printf("p(c | b) = %lf\n", KN.Probability("b", "c"));
	printf("\n");

	printf("p(a | c) = %lf\n", KN.Probability("c", "a"));
	printf("p(b | c) = %lf\n", KN.Probability("c", "b"));
	printf("p(c | c) = %lf\n", KN.Probability("c", "c"));

	return 0;
}