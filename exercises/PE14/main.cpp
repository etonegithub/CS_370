#include <cmath>
#include <fstream>
#include <vector>
#include "stats.h"
using namespace std;

int main(int argc, char *argv[]) {
    // initialize variables
    string input;
    ifstream infile;
    ofstream outfile;
    vector<double> data;
    double min, max, avg, std_dev;

    // open and clear output file
    outfile.open(argv[argc - 1], ofstream::trunc);

    for (int i = 1; i < argc - 1; i++) {
        // clear data vector for next file
        data.clear();
        
        // read from file and store in vector
        infile.open(argv[i]);
        while(infile >> input) {
            data.push_back(stod(input));
        }
        infile.close();

        // apply stats function and write output to outfile
        stats(data, min, max, avg, std_dev);
        outfile << min << "," << max << "," << avg << "," << std_dev << endl;
    }
}