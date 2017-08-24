#include <fstream>
#include <sstream>

#include "csv.h"
#include "utils.h"

using namespace std;

CSV::CSV(const char* fname)
    : fname(fname), num_records(0)
{
    ifstream csv_f(fname, ios::in);
    string line;

    // For header line
    //getline(csv_f, line);

    while (getline(csv_f, line)) {
        num_records++;
    }
    csv_f.close();
}
