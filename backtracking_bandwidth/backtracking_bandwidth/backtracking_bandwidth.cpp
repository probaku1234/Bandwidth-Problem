// backtracking_bandwidth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <array>
#include <io.h>
#include <time.h>
using namespace std;

#define MAX_VERTICES 256
#define NMAX 100000

char *edge1, *edge2;   
char *edges;

char current_perm[MAX_VERTICES]; 
char posn[MAX_VERTICES];  
char best_perm[MAX_VERTICES];  
char used[MAX_VERTICES];  
char maxl[MAX_VERTICES];  
char rmap[MAX_VERTICES]; 
char larg[MAX_VERTICES];

int n;
int m; 
int minv;  
int cnt = 0;
int glug = 0;

int max_edge_length() {
	int v = 0, maxv = 0, v1, v2;
	for (int i = 0; i<m; i++) {
		v1 = edge1[i];
		v2 = edge2[i];
		v = abs(posn[v2] - posn[v1]);
		if (v > maxv) {
			maxv = v;
			glug = v1; if (rand() % 2) glug = v2;
		}
	}
	return maxv;
}

void backtracking(int step) {
	int i;

	if (step == n) {
		int current_max_bandwidth = max_edge_length(); 
		if (current_max_bandwidth<minv) {
			minv = current_max_bandwidth;
			for (int i = 0; i < n; i++) 
				best_perm[i] = current_perm[i];
		}

	}
	else {
		for (i = 0; i < m; i++) {
			if (used[edge1[i]] && !used[edge2[i]])
				if (posn[edge1[i]] <= step - minv) 
					return;
			if (!used[edge1[i]] && used[edge2[i]])
				if (posn[edge2[i]] <= step - minv) 
					return;
		}
		for (int k = 0; k < n; k++) {
			i = rmap[k];
			if (!used[i]) {
				used[i] = 1;
				current_perm[step] = i;
				posn[i] = step;
				backtracking(step + 1);
				used[i] = 0;
			}
		} 
	}
}

void generate_permutation(string path, string name) {
	ifstream inputfile(path + name);
	if (inputfile.is_open()) {
		inputfile >> n;
		inputfile >> m;

		edge1 = (char *)malloc(m);
		edge2 = (char *)malloc(m);
		edges = (char *)malloc(256 * n);

		for (int i = 0; i < 256 * n; i++)
			edges[i] = 0;

		int edge_count = 0;
		int a, b, j;		
		while (inputfile >> a >> b)
		{
			a--;
			b--;
			edge1[edge_count] = a;
			edge2[edge_count] = b;
			edge_count++;
			edges[(a << 8) + b] = 1;
			edges[(b << 8) + a] = 1;
		}
		m = edge_count;

		for (int i = 0; i<n; i++) { 
			current_perm[i] = i;
			used[i] = 0;
			posn[i] = i;
			rmap[i] = i;
		}

		srand(1023 * 3942);
		for (int i = 0; i < 1000; i++) {
			a = rand() % n;
			b = rand() % n;
			j = rmap[a]; 
			rmap[a] = rmap[b]; 
			rmap[b] = j;
		}

		int temp1, temp2, count;
		for (int i = 0; i<1000; i++) {
			b = rand() % n;
			temp1 = max_edge_length();
			a = posn[glug];
			count = 0;
			do {
				j = current_perm[a]; 
				current_perm[a] = current_perm[b]; 
				current_perm[b] = j;
				posn[current_perm[a]] = a;
				posn[current_perm[b]] = b;
				temp2 = max_edge_length();
				if (temp2 > temp1) {
					j = current_perm[a]; 
					current_perm[a] = current_perm[b]; 
					current_perm[a] = j;
					posn[current_perm[a]] = a;
					posn[current_perm[b]] = b;
				};
				count++;
				b = rand() % n;
			} while (temp2 >= temp1 && count < 50);
		}

		minv = max_edge_length();
		for (int i = 0; i<n; i++) best_perm[i] = current_perm[i];
		for (int i = 0; i<n; i++) { // No vertices used up yet
			current_perm[i] = i;
			used[i] = 0;
			posn[i] = i;
			rmap[i] = i;
		}

		backtracking(0);
		std::cout << "permutation : ";
		for (int i = 0; i < n; i++) {
			std::cout << best_perm[i] + 1 << " ";
		}
		std::cout << "max bandwidth : " << minv << endl;
		free(edge1);
		free(edge2);
		free(edges);
	}
}

int main()
{	
	string path = "C:\\Users\\fhzot\\Documents\\Visual Studio 2017\\Projects\\backtracking_bandwidth\\backtracking_bandwidth\\data\\";
	string filter = "*.txt*";
	struct _finddata_t fd;
	vector<string> file_list;
	long handle = _findfirst((path+filter).c_str(), &fd);
	if (handle == -1)
		return 0;

	int result = 0;
	while (true)
	{
		file_list.push_back(fd.name);
		
		clock_t begin, end;
		cout << fd.name << endl;
		begin = clock();
		generate_permutation(path, fd.name);
		end = clock();
		cout << "Time : " << ((end - begin)) << endl;
		result = _findnext(handle, &fd);
		if (result == -1)
			break;
	} 	 
	_findclose(handle);
    return 0;
}

