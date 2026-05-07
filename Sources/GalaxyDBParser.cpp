#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

//#define LINUX

// Trims blank spaces from both ends of a string
std::string trim(const std::string& s)
{
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

// Converts J2000 RA (JHHMMSS.s) to degrees
double parseRA(std::string raStr)
{
    if (raStr[0] == 'J') raStr = raStr.substr(1);
    double h = std::stod(raStr.substr(0, 2));
    double m = std::stod(raStr.substr(2, 2));
    double s = std::stod(raStr.substr(4));
    return (h + (m / 60.0) + (s / 3600.0)) * 15.0; // 15 degrees per hour
}

// Converts J2000 DEC (+/-DDMMSS) to degrees
double parseDEC(std::string decStr)
{
    double sign = (decStr[0] == '-') ? -1.0 : 1.0;
    std::string dms = (decStr[0] == '+' || decStr[0] == '-') ? decStr.substr(1) : decStr;
    double d = std::stod(dms.substr(0, 2));
    double m = std::stod(dms.substr(2, 2));
    double s = std::stod(dms.substr(4));
    return sign * (d + (m / 60.0) + (s / 3600.0));
}

int main(int argc, char* argv[])
{
	std::string inputFile;
	float diameter;
    float magnitude = 999.;
    if (argc < 3 )
    {
        std::cout << "Insufficient number of arguments in the commandline." << std::endl;
        std::cout << "Do you want to continue and input the necessary data? <y/n>." << std::endl;
		char response;
		response = std::cin.get();
		if (response == 'y' || response == 'Y')
		{
			std::string userInput;
			std::cout << "Input the path to the database file..." << std::endl;
			std::cin >> userInput;
			inputFile = userInput + '/';

			std::cout << "Input the name of the database file..." << std::endl;
			std::cin >> userInput;
			inputFile += userInput;
			std::cout << "Database file: " << inputFile << std::endl;

			std::cout << "Input the diameter of the object circles: " << std::endl;
			std::cin >> userInput;
			diameter = atof( userInput.c_str() );

            std::cout << "Input the maximal magnitude (input 999 to disable magnitude limit)..." << std::endl;
            std::cin >> userInput;
            magnitude = atof( userInput.c_str() );
		}
		else
		{
			std::cout << "End program." << std::endl;
            #ifdef LINUX
            system("read");
            #else
			system("pause");
			#endif
			return 1;
		}
    }
    else
    {
		inputFile = argv[1];
		diameter = atof(argv[2]);
	}

//    std::string configFile = "columns.txt"; // Contains space-separated column indices (0-based)
    std::string outputFile = "galaxy_data.csv";
    char inDelim = ';';      // Delimiter used in the source database

//    char outDelim = ',';     // Desired output delimiter comma
    char outDelim = 9;       // Desired output delimiter TAB

    std::string prefix = "PGC";
    int prefixColIdx = 0;    // Index of the selected column to receive the prefix
    int magnitudeColIdx = 1; // Index of the magnitude column
    const char* labels[] = {"RA", "DEC", "NAME", "MAGNITUDE", "DIAMETER"}; // Header labels
    // -------------------------

    // 1. Read column selection from definition file
    std::vector<int> selectedCols;
	selectedCols.push_back(23);
	selectedCols.push_back(6);

    std::ifstream in(inputFile);
    std::ofstream out(outputFile);
    std::string line;

    // 2. Write custom header labels
    for (int i = 0; i < 5; ++i)
    {
        out << labels[i] << (i == 4 ? "" : std::string(1, outDelim));
    }
    out << "\n";

    // 3. Process database rows
    int numRecords=0;
    while (std::getline(in, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == '$') continue;

        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, inDelim))
        {
            row.push_back(trim(cell));
        }

        if (row.size() < 3) continue; // Skip malformed rows

        // Parse J2000 (Column 2/Index 1) e.g., "J132755.6+470328"
        std::string j2000 = row[1];
        size_t splitPos = j2000.find_first_of("+-", 1);
        double ra = parseRA(j2000.substr(0, splitPos));
        double dec = parseDEC(j2000.substr(splitPos));

        // Evaluate Flag (Column 3/Index 2)
		// Process only raws of type G or g
        std::string flag = (row[2] == "G" || row[2] == "g") ? "1" : "0";

        // 4. Construct output row
		if (flag == "1")
		{
		    std::string valMag = row[selectedCols[magnitudeColIdx]];
		    float dBmagnitude = atof( valMag.c_str() );
		    if( dBmagnitude < magnitude)
            {
                numRecords++;
                out << std::fixed << std::setprecision(6) << ra << outDelim << dec << outDelim;

                for (size_t i = 0; i < selectedCols.size(); ++i)
                {
                    std::string val = row[selectedCols[i]];
                    if (i == (size_t)prefixColIdx) val = prefix + val; // Apply string prefix
                    out << val << outDelim;
                }
                out << diameter << "\n";
            }
		}
    }

    std::cout << "Processing complete. Output saved to: " << outputFile << std::endl;
    std::cout << "Number of records: " << numRecords << std::endl;
    std::cout << "Circle diameter: " << diameter << std::endl;
    if ( magnitude < 999 )
        std::cout << "Maximal magnitude: " << magnitude << std::endl;
    #ifdef LINUX
    system("read");
    #else
    system("pause");
    #endif
    return 0;
}
