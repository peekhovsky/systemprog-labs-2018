#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "Memory.h"

#include <stdio.h>
#include <iostream>	

using namespace std;

int get_int(int, int);


int main(int args, char* argv[]) {

	Memory* memory = new Memory;

	vector<Object*> objects;

	while (true) {
			
		cout << "1 - add new object, 2 - show page by index, 3 - show objects, 4 - delete object by index, 0 - exit" << endl;
		cout << "Buffer: " << memory->get_buffer() << endl;
		int choise = get_int(0, 4);
		
		switch (choise) {

		case 1: {
			cout << "Enter an object size: " << endl;
			int object_size = get_int(0, memory->get_page_size() + 1);

			Object* object = memory->malloc(object_size);
			if (object != nullptr) {
				objects.push_back(object);
			}
			else {
				break;
			}

			cout << "Enter an string value: " << endl;
			rewind(stdin);
			string value;
			cin >> value;
			memory->set_value(object, value);

			break;
		}

		case 2: {
			cout << "Enter an index: " << endl;
			int index = get_int(0, memory->get_page_amount());
			memory->show_page(index);
			break;
		}

		case 3: {
			cout << endl;
			for (int i = 0; i < objects.size(); i++) {
				cout << "Object index: " << i << endl;
				cout << "Page index: " << objects[i]->page_index << endl;
				cout << "Page offset: " << objects[i]->page_offset << endl;
				cout << "Object value: " << memory->get_value(objects[i]) << endl;

				cout << "Object size: " << objects[i]->size << endl << endl;
			}
			break;
		}

		case 4: {
			cout << "Enter an index: " << endl;
			int index = get_int(0, objects.size());
			memory->free(objects[index]);
			objects.erase(objects.begin() + index);
			break;
		}

		case 0: {
			delete memory;
			exit(0);
			break;
		}
		}
	}


    return 0;
}



int get_int(int min, int max) {
	fflush(stdin);
	rewind(stdin);
	cin.clear();
	int res = 0;
	do {
		cin >> res;
		if ((res < min) || (res > max)) {
			cout << "\nIncorrect input. Try again\n";
			rewind(stdin);
			cin.clear();
			continue;
		}
		else break;
	} while (true);
	return res;
}

