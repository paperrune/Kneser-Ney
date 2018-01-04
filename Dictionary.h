typedef struct Node{
	int index;

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

	Node *Search_Node(bool construct, char string[]);
public:
	Dictionary();
	~Dictionary();

	void Delete(char string[]);
	void Insert(char string[], char content[]);

	char* Search(char string[]);
	char *Search_Copy(char string[], char *buffer, int buffer_size);
	char* New_Search(char string[]);
};
