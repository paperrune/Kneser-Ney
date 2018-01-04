#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dictionary.h"

void Dictionary::Release_Node(Node *node){
	for (int i = 0; i < 256; i++){
		if (node->link[i]){
			Release_Node(node->link[i]);

			if (node->link[i]->index > 0){
				delete[] content[node->link[i]->index];
				delete[] string[node->link[i]->index];
			}
			delete[] node->link[i];
		}
	}
	delete[] node->link;
}

Node* Dictionary::Search_Node(bool construct, char string[]){
	Node *node = (strlen(string) == 0) ? (NULL) : (&root);

	for (int i = 0; i < strlen(string); i++){
		int index = static_cast<unsigned char>(string[i]);

		if (!node->link[index]){
			if (construct){
				Node *new_node = new Node[1];

				new_node->index = 0;
				memset(new_node->link = new Node*[256], 0, sizeof(Node*) * 256);

				node->link[index] = new_node;
			}
			else{
				return NULL;
			}
		}
		node = node->link[index];
	}
	return node;
}

Dictionary::Dictionary(){
	root.index = 0;
	memset(root.link = new Node*[256], 0, sizeof(Node*)* 256);

	maximum_number_words = 1;
	number_words = 0;
	buffer = new char[1000];
	content = new char*[1];
	string = new char*[1];

	number_deleted = 0;
	deleted = new int[0];
}
Dictionary::~Dictionary(){
	Release_Node(&root);

	delete[] buffer;
	delete[] content;
	delete[] string;
	delete[] deleted;
}

void Dictionary::Delete(char string[]){
	Node *node = Search_Node(false, string);

	if (node == NULL || node->index == 0){
		return;
	}
	if (node->index > 0){
		deleted = (int*)realloc(deleted, sizeof(int)* (number_deleted + 1));
		deleted[number_deleted++] = node->index;

		delete[] this->string[node->index];
		delete[] this->content[node->index];

		node->index = 0;
	}
}
void Dictionary::Insert(char string[], char content[]){
	Node *node = Search_Node(true, string);

	if (node == NULL){
		return;
	}
	if (node->index == 0){
		if (number_deleted){
			node->index = deleted[--number_deleted];

			this->string[node->index] = new char[strlen(string) + 1];
			strcpy(this->string[node->index], string);

			this->content[node->index] = new char[strlen(content) + 1];
			strcpy(this->content[node->index], content);
		}
		else{
			node->index = ++number_words;

			if (maximum_number_words <= number_words){
				maximum_number_words *= 2;
				this->string = (char**)realloc(this->string, sizeof(char*)* (maximum_number_words));
				this->content = (char**)realloc(this->content, sizeof(char*)* (maximum_number_words));
			}
			this->string[number_words] = new char[strlen(string) + 1];
			strcpy(this->string[number_words], string);

			this->content[number_words] = new char[strlen(content) + 1];
			strcpy(this->content[number_words], content);
		}
	}
	else{
		this->string[node->index] = (char*)realloc(this->string[node->index], strlen(string) + 1);
		strcpy(this->string[node->index], string);

		this->content[node->index] = (char*)realloc(this->content[node->index], strlen(content) + 1);
		strcpy(this->content[node->index], content);
	}
}

char* Dictionary::Search(char string[]){
	Node *node = Search_Node(false, string);

	if (node == NULL || node->index == 0){
		return NULL;
	}
	else{
		return content[node->index];
	}
}
char* Dictionary::Search_Copy(char string[]){
	Node *node = Search_Node(false, string);

	if (node == NULL || node->index == 0){
		return NULL;
	}
	else{
		strcpy(buffer, content[node->index]);
		return buffer;
	}
}
char* Dictionary::New_Search(char string[]){
	Node *node = Search_Node(false, string);

	if (node == NULL || node->index == 0){
		return NULL;
	}
	else{
		char *buffer = new char[strlen(content[node->index]) + 2];

		strcpy(buffer, content[node->index]);
		return buffer;
	}
}
