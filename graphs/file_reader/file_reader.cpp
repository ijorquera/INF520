#include "file_reader.h"
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

unsigned __int128 atouint128_t(std::string const & in)
{
    unsigned __int128 res = 0;
    size_t i = 0;

    for (; i < in.size(); ++i) {
        const char c = in[i];
        if (not std::isdigit(c)) 
            throw std::runtime_error(std::string("Non-numeric character: "+ in + c) + c);
        res *= 10;
        res += (c - '0');
    }

    return res;
}

std::string uint128ToString(unsigned __int128 in)
{
    std::string value = "";
    if (in == 0) {
        value += "0";
    }

    while (in != 0) {
        value = std::to_string((int)(in % 10)) + value;
        in = in / 10;
    }

    return value;
}

FileReader::FileReader() { }

FileReader::~FileReader() { }

void FileReader::getGapFromFile(const std::string fileName, const std::string mode)
{
    std::string dir = "data_g/";
    std::string ext = ".txt";
    std::ifstream file_A(dir + fileName + "/" + mode + "_" + fileName + ext); // input file stream to open the file A.txt

    // read it as a vector
    std::string line_A;
    std::string value_as_str;
    unsigned __int128 gapEntropy = 0.0f;
    unsigned __int128 g_i;

    unsigned __int128 prev = 0;
    unsigned __int128 element_A;
    unsigned __int128 gv_i;

    std::vector<unsigned __int128> gap_vector_uniques;
    long double H0_gapEntropy = 0.0f;

    
    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream gapVectorFile;
        gapVectorFile.open(dir + fileName + "/compressed/gapVector_" + mode + "_" + fileName + ext);

        // Reading each line of the file
        while (getline(file_A, line_A))
        {
            g_i = 1;

            // Reading each number of the line, should be only one number (index of z_order_vector value)
            std::stringstream stream_A(line_A);
            while (1)
            {
                stream_A >> value_as_str;
                if (!stream_A)
                    break;
                element_A = atouint128_t(value_as_str);
                gv_i = element_A - prev;
                // std::cout << uint128ToString(gv_i) << std::endl;
                if (gv_i > 0) {
                    g_i = floor(log2(gv_i)) + 1;
                }
        
                gapVectorFile << uint128ToString(gv_i) << "\n";

                prev = element_A + 1;

                // gap_vector_uniques.push_back(gv_i);
                // std::sort(gap_vector_uniques.begin(), gap_vector_uniques.end());
                // auto uniques = std::unique(gap_vector_uniques.begin(), gap_vector_uniques.end());
                // gap_vector_uniques.erase(uniques, gap_vector_uniques.end());
            }
            
            gapEntropy = gapEntropy + g_i;
        }
        gapVectorFile.close();

        // std::cout << "Entropy ready" << std::endl; 
        std::ofstream gapEntropyFile;
        gapEntropyFile.open(dir + fileName + "/compressed/gapEntropy_" + mode + "_" + fileName + ext);
        gapEntropyFile << uint128ToString(gapEntropy);
        gapEntropyFile.close();
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }
}

void FileReader::getRLEFromFile(const std::string fileName, const std::string mode)
{
    std::string dir = "data_g/";
    std::string ext = ".txt";
    std::cout << "Reading file: " << dir + fileName + "/" + mode + "_" + fileName + ext << std::endl;
    std::ifstream file_A(dir + fileName + "/" + mode + "_" + fileName + ext); // input file stream to open the file A.txt

    // read it as a vector
    std::string line_A;
    std::string value_as_str;
    
    unsigned __int128 rleZerosEntropy = 0;
    unsigned __int128 rleOnesEntropy = 0;
    unsigned __int128 rleEntropy = 0;

    unsigned __int128 z_i;
    unsigned __int128 l_i;

    unsigned __int128 prev = 0;    
    unsigned __int128 count = 0;

    unsigned __int128 element_A;
    unsigned __int128 rlev_i;

    // std::vector<unsigned __int128> rle_vector_uniques;
    long double H0_rleEntropy = 0.0f;

    
    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream rleVectorFile;
        rleVectorFile.open(dir + fileName + "/compressed/rleVector_" + mode + "_" + fileName + ext);

        // Reading each line of the file
        while (getline(file_A, line_A))
        {
            // Cantidad de bits para almacenar zeros y enteros en la posición i;
            // rle vector tiene forma z_0l_0 z_1l_1 ... z_il_i
            // donde z_0 es la cantidad de zeros en la posición 0
            // l_0 es la cantidad de unos en la posición 0

            // Reading each number of the line, should be only one number (index of z_order_vector value)
            std::stringstream stream_A(line_A);
            while (1)
            {
                stream_A >> value_as_str;
                if (!stream_A)
                    break;
                element_A = atouint128_t(value_as_str);

                rlev_i = element_A - prev;

                // Ambos comienzan en 0 
                if (element_A == prev) {
                    // Cantidad de ceros (técnicamente cero ceros, por lo que necesito 1 bit en z_i)
                    z_i = 1;
                    rleZerosEntropy += z_i;

                    rleVectorFile << uint128ToString(count) << "\n";

                    count += 1;
                } else if ((prev == 0 && element_A == 1 && count == 0) || (rlev_i > 1)){
                    if (count > 0) {
                        // Cantidad de unos
                        l_i = 1;
                        if (count - 1 > 0) {
                            l_i = floor(log2(count - 1)) + 1;
                        }
                        rleOnesEntropy += l_i;

                        rleVectorFile << uint128ToString(count) << "\n";
                    }
                    // Cantidad de ceros
                    auto n_zeros = rlev_i > 1 ? rlev_i - 1 : rlev_i;
                    z_i = 1;
                    if (n_zeros - 1 > 0) {
                        z_i = floor(log2(n_zeros - 1)) + 1;
                    }
                    rleZerosEntropy += z_i;

                    rleVectorFile << uint128ToString(n_zeros) << "\n";

                    count = 1;
                } else {
                    count += 1;
                }

                // if (rlev_i > 0) {
                //     l_i = floor(log2(rlev_i)) + 1;
                // }
        
                prev = element_A;
            }
            
            rleEntropy = rleEntropy + z_i;
        }
        if (count > 0) {
            // Cantidad de unos
            l_i = 1;
            if (count - 1 > 0) {
                l_i = floor(log2(count - 1)) + 1;
            }
            rleOnesEntropy += l_i;
            rleVectorFile << uint128ToString(count) << "\n";
        }
        rleVectorFile.close();

        // std::cout << "Entropy ready" << std::endl; 
        std::ofstream rleEntropyFile;
        rleEntropyFile.open(dir + fileName + "/compressed/rleEntropy_" + mode + "_" + fileName + ext);
        rleEntropyFile << "rleZerosEntropy: " << uint128ToString(rleZerosEntropy) << "\n";
        rleEntropyFile << "rleOnesEntropy: " << uint128ToString(rleOnesEntropy) << "\n";
        rleEntropyFile << "rleEntropy: " << uint128ToString(rleZerosEntropy + rleOnesEntropy);
        rleEntropyFile.close();
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }
}

std::string toBinary(unsigned __int128 n, unsigned n_bits)
{
    std::string r;

    while(n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n = n / 2;
    }

    while (r.length() < n_bits) {
        r = "0" + r;
    }
    // std::cout << r << std::endl;
    return r;
}

std::string toBinary(unsigned __int128 n)
{
    std::string r;

    while(n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n = n / 2;
    }
    return r;
}

void FileReader::getTrieFromFile(const std::string fileName, const std::string mode)
{
    // read it as a vector
    std::string line_A;
    std::string value_as_str;
    std::string lastline;
    std::string current_bit_sec, aux, new_bit_sec;

    unsigned __int128 element_A;
    unsigned x_i, dif, j;
    unsigned __int128 idx = 0;
    unsigned __int128 trieEntropy = 0;
    long double H0_trieEntropy = 0.0f;

    bool flag = false;

    std::string dir = "data_g/";
    std::string ext = ".txt";
    std::cout << "Reading file: " << dir + fileName + "/" + mode + "_" + fileName + ext << std::endl;
    
    
    std::ifstream file_A(dir + fileName + "/" + mode + "_" + fileName + ext); // input file stream to open the file A.txt

    // Get the last line of the file (biggest value)
    std::ifstream fin(dir + fileName + "/" + mode + "_" + fileName + ext);
    if (fin.is_open() && fin.good()) {
        fin.seekg(-1, std::ios_base::end);
        if(fin.peek() == '\n') {
            fin.seekg(-1, std::ios_base::cur);
            int i = fin.tellg();
            for(;i > 0; i--)
            {
                if(fin.peek() == '\n')
                {
                //Found
                fin.get();
                break;
                }
                fin.seekg(i, std::ios_base::beg);
            }
        }
        getline(fin, lastline);
        fin.close();

        // std::cout << "Result: " << lastline << std::endl;
    } else {
        std::cout << "Failed to open file.  \n";
    }


    lastline = trim_copy(lastline);
    unsigned __int128 biggest_number = atouint128_t(lastline);
    unsigned n_bits = floor(log2(biggest_number)) + 1;

        
    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream trieVectorFile;
        trieVectorFile.open(dir + fileName + "/compressed/trieVector_" + mode + "_" + fileName + ext);

        // Reading each line of the file
        while (getline(file_A, line_A))
        {
            // Reading each number of the line, should be only one number (index of z_order_vector value)
            std::stringstream stream_A(line_A);

            while (1)
            {
                stream_A >> value_as_str;
                if (!stream_A)
                    break;
                element_A = atouint128_t(value_as_str);
                // std::cout << "element_A = " << uint128ToString(element_A) << std::endl;

                flag = false;
                x_i = 0;

                if (idx == 0) {
                    current_bit_sec = toBinary(element_A, n_bits);
                    x_i = current_bit_sec.length();
                    for (int i = 0; i < current_bit_sec.length(); i++) {
                        trieVectorFile << current_bit_sec[i] << "\n";
                    }
                } else {
                    aux = toBinary(element_A);
                    new_bit_sec = "";
                    dif = current_bit_sec.length() - aux.length();

                    // Save the prefix of the original value
                    for (j = 0; j < dif; j++) {
                        new_bit_sec += current_bit_sec[j];
                    }

                    // Start adding bits after the prefix
                    // if at some point the bit in both secs is different,
                    // then the prefix is broken and start adding new bits to the sum
                    for (j = dif; j < current_bit_sec.length(); j++) {
                        if (current_bit_sec[j] == aux[j - dif]) {
                            new_bit_sec += current_bit_sec[j];
                        } else { // Here is the new bit after breaking the prefix
                            flag = true;
                            new_bit_sec += aux[j - dif];
                        }
                        if (flag) {
                            x_i += 1;
                            trieVectorFile << aux[j - dif] << "\n";
                        }
                    }
                    // trieVectorFile << " ";
                    current_bit_sec = new_bit_sec;
                }
                idx += 1;
            }   
            trieEntropy += x_i;
        }
        trieVectorFile.close();
        // std::cout << "Entropy ready" << std::endl; 
        std::ofstream trieEntropyFile;
        trieEntropyFile.open(dir + fileName + "/compressed/trieEntropy_" + mode + "_" + fileName + ext);
        trieEntropyFile << "trieEntropy: " << uint128ToString(trieEntropy);
        trieEntropyFile.close();
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }
}
