typedef struct Node{
	int index;
	int number_links;

	char *character;

	Node **link;
}Node;

class Dictionary{
private:
	char **content;
	char **string;

	int maximum_number_words;
	int number_deleted;
	int number_words;

	int *deleted;

	Node root;

	void Release_Node(Node *node);

	int Character_Index(char character_list[], char character);

	Node *String_Node(bool construct, char string[]);
public:
	Dictionary();
	~Dictionary();

	void Delete(char string[]);
	void Insert(char string[], char content[]);

	char* Search(char string[]);
	char* New_Search(char string[]);
};