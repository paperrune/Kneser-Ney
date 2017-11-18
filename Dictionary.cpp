#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Dictionary.h"

void Dictionary::Release_Node(Node *node){
	if (node->number_links){
		for (int i = 0; i < node->number_links; i++){
			Release_Node(node->link[i]);

			if (node->link[i]->index > 0){
				delete[] content[node->link[i]->index];
				delete[] string[node->link[i]->index];
			}
			delete[] node->link[i];
		}
		delete[] node->character;
		delete[] node->link;
	}
}

int Dictionary::Character_Index(char character_list[], char character){
	if (character_list == NULL){
		return -1;
	}
	return strchr(character_list, character) - character_list;
}

Node* Dictionary::String_Node(bool construct, char string[]){
	Node *node = (strlen(string) == 0) ? (NULL) : (&root);

	for (int i = 0; i < strlen(string); i++){
		int index = Character_Index(node->character, string[i]);

		if (index < 0){
			if (construct){
				Node *new_node = new Node[1];

				index = node->number_links++;

				node->character = (node->number_links == 1) ? (new char[2]) : ((char*)realloc(node->character, node->number_links + 1));
				node->character[node->number_links - 1] = string[i];
				node->character[node->number_links] = '\0';

				new_node->index = 0;
				new_node->number_links = 0;
				new_node->character = 0;

				node->link = (node->number_links == 1) ? (new Node*[1]) : (Node**)realloc(node->link, sizeof(Node*)* node->number_links);
				node->link[node->number_links - 1] = new_node;
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
	root.number_links = 0;
	root.character = 0;

	maximum_number_words = 1;
	number_words = 0;
	content = new char*[1];
	string = new char*[1];

	number_deleted = 0;
	deleted = new int[0];
}
Dictionary::~Dictionary(){
	Release_Node(&root);

	delete[] content;
	delete[] string;
	delete[] deleted;
}

void Dictionary::Delete(char string[]){
	Node *node = String_Node(false, string);

	if (node == NULL || node->index == 0){
		return;
	}
	else if (node->index > 0){
		deleted = (int*)realloc(deleted, sizeof(int)* (number_deleted + 1));
		deleted[number_deleted++] = node->index;

		delete[] this->string[node->index];
		delete[] this->content[node->index];

		node->index = 0;
	}
}
void Dictionary::Insert(char string[], char content[]){
	Node *node = String_Node(true, string);

	if (node == NULL) return;

	if (node->index == 0){
		if (number_deleted){
			node->index = deleted[number_deleted - 1];

			this->string[node->index] = new char[strlen(string) + 1];
			strcpy(this->string[node->index], string);

			this->content[node->index] = new char[strlen(content) + 1];
			strcpy(this->content[node->index], content);

			deleted = (int*)realloc(deleted, sizeof(int)* --number_deleted);
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
	Node *node = String_Node(false, string);

	if (node == NULL || node->index == 0){
		return NULL;
	}
	else{
		return content[node->index];
	}
}
char* Dictionary::New_Search(char string[]){
	Node *node = String_Node(false, string);

	if (node == NULL || node->index == 0){
		return NULL;
	}
	else{
		char *buffer = new char[strlen(content[node->index]) + 2];

		strcpy(buffer, content[node->index]);
		return buffer;
	}
}