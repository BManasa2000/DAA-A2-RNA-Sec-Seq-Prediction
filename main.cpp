#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

// for doxygen: run "doxygen doxyfile" in command prompt
// For seeing the documentation, open index.html

/*
global file variables
file1: for input.txt file containing the no. of test cases followed by the testcases (strings containing A,C,G,U)
file2: for output.txt file storing the rna seq, max no.of pairs followed by the pairs
file3: for data.txt file storing the number of length of rna sequence, amount of time to compute the max no. of base pairs
*/
fstream file1, file2, file3;

/**A class for finding the maximum number of
 *base pairs in a given nucleic acid sequence.
*/
class RNA
{
	private:
		// rna: string containing the nucleic acid sequence (consists of A, C, G, U)
		string rna;
		// l: length of rna sequence
		int l;
	public:

		/**Function to check if two bases form a pair
		 * @param c1: char representing the 1st base
		 * @param c2: char representing the 2nd base
		 * @return true if they are a match (A,U), (U,A), (C,G), (G,C)
		*/
		bool match (char c1, char c2)
		{
			if (c1 == 'A' && c2 == 'U')
			{
				// cout << "Returning true 1\n";
				return true;
			}
			if (c1 == 'U' && c2 == 'A')
			{
				// cout << "Returning true 2\n";
				return true;
			}
			if (c1 == 'C' && c2 == 'G')
			{
				// cout << "Returning true 3\n";
				return true;
			}
			if (c1 == 'G' && c2 == 'C')
			{
				// cout << "Returning true 4\n";
				return true;
			}
			return false;
		}

		/**Function to print the max number of base pairs 
		 * and the index pairs(1 indexed) for a given rna sequence
		 * @param rna: string containing only A, C, G, U
		*/
		void run(string rna)
		{
			l = rna.size();
			int opt[l+1][l+1];
			vector<pair<int, int> > pairs [l+1][l+1]; 
			memset (opt, 0, sizeof(opt));
			for (int k = 5; k < l; k++)
			{
				for (int i = 1; i <= l-k; i++)
				{
					int j = i + k;
					int m = -1, t1 = -1;
					for (int t = i; t < j-4; t++)
					{
						if (match(rna[t-1], rna[j-1]) == true)
						{
							int l = 1 + opt[i][t-1] + opt[t+1][j-1];
							if (m < l)
							{
								m = l;
                                t1 = t;
							}
						}
					}
					if (m > opt[i][j-1])
					{
						opt[i][j] = m;
						for (auto it: pairs[i][t1-1])
							pairs[i][j].push_back(it);
                        pairs[i][j].push_back(make_pair(t1,j));
                        for (auto it: pairs[t1+1][j-1])
                            pairs[i][j].push_back(it);
					}
					else
					{
						opt[i][j] = opt[i][j-1];
						for (auto it: pairs[i][j-1])
							pairs[i][j].push_back(it);
					}
				}
			}
			cout << endl << "Length of seq: " << l << endl;
			cout << endl << "Max number of base pairs = " << opt[1][l] << endl;
			cout << endl;
			for (auto it: pairs[1][l])
				cout << it.first << "  " << it.second << endl;
		}

		/**Function that takes a vector of rnas as input and 
		 * stores the outputs i.e the max no. of base pairs and
		 * the index pairs (1 indexed) for each testcase in the file
		 * @param rnas: vector of strings containing only A, C, G, U
		*/
		void run(vector<string> rnas)
		{
			file2.open("output.txt", ios::out);
			file3.open("data.txt", ios::out);
			clock_t start, end;
			if (file2.is_open() && file3.is_open())
			{
				for (int z = 0; z < rnas.size(); z++)
				{
					rna = rnas[z];
					l = rna.size();
					int opt[l+1][l+1];
					vector<pair<int, int> > pairs [l+1][l+1];
					start = clock();
					memset (opt, 0, sizeof(opt));
					for (int k = 5; k < l; k++)
					{
						for (int i = 1; i <= l-k; i++)
						{
							int j = i + k;
							int m = -1;
							vector<pair<int, int> > temp;
							for (int t = i; t < j-4; t++)
							{
								if (match(rna[t-1], rna[j-1]) == true)
								{
									int l = 1 + opt[i][t-1] + opt[t+1][j-1];
									if (m < l)
									{
										m = l;
										temp.erase(temp.begin(), temp.end());
										for (auto it: pairs[i][t-1])
											temp.push_back(it);
										temp.push_back(make_pair(t,j));
										for (auto it: pairs[t+1][j-1])
											temp.push_back(it);
									}
								}
								// cout << rna[t-1] << "  " << rna[j-1] << endl;
								// if (match(rna[t-1], rna[j-1]) == true)
									// m = max (m, 1 + opt[i][t-1] + opt[t+1][j-1]);
							}
							if (m > opt[i][j-1])
							{
								opt[i][j] = m;
								for (auto it: temp)
									pairs[i][j].push_back(it);
							}
							else
							{
								opt[i][j] = opt[i][j-1];
								for (auto it: pairs[i][j-1])
									pairs[i][j].push_back(it);
							}
						}
					}
					end = clock();
					double time = double(end - start)/double(CLOCKS_PER_SEC);
					file2 << rna << endl;
					file2 << "Length of seq: " << l << endl;
					file2 << "Max no. of base pairs: " << opt[1][l] << endl;
					file2 << "Index pairs (1 indexed): \n";
					for (auto it: pairs[1][l])
						file2 << it.first << " " << it.second << endl;
					file2 << endl;

					file3 << rna.length() << "," << time << endl;
				}
				file2.close();
				file3.close();
			}
		}
};

int main()
{
	/*file1.open("input.txt", ios::in); // opening input file
	if (file1.is_open())
	{
		RNA seq;
		int t; // number of testcases
		file1 >> t;
		vector<string> rnas;
		for (int i = 0; i < t; i++)
		{
			string rna;
			file1 >> rna;
			rnas.push_back(rna);
		}
		seq.run(rnas);
		file1.close();
	}*/

	RNA seq; // creating an object of the class RNA
	string rna;
	cout << "Please enter the rna sequence\n";
	cin >> rna;
	seq.run(rna);

	return 0;
}













