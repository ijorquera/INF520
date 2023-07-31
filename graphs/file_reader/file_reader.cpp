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

std::vector<unsigned __int128> FileReader::getIntVectorFromFile(const std::string fileName, const std::string mode)
{
    std::vector<unsigned __int128> z_order_vector;
    std::string dir = "data_g/";
    std::string ext = ".txt";
    std::ifstream file_A(dir + fileName + "/" + mode + "_" + fileName + ext); // input file stream to open the file A.txt

    // read it as a vector
    std::string line_A;
    std::string value_as_str;

    unsigned __int128 element_A;

    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream gapVectorFile;
        gapVectorFile.open(dir + fileName + "/compressed/gapVector_" + mode + "_" + fileName + ext);

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
                z_order_vector.push_back(element_A);
            }
        }
        gapVectorFile.close();
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }

    return z_order_vector;
}


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


void FileReader::getWorstCaseEntropyFromFile(const std::string fileName, const std::string mode)
{
    // For an integer universe U = [0 ... u), ...
    // denote the class S \subseteq U such that |S| = n
    // We assume S = {x1, x2, ..., xn}, for 0 <= x1 <= ... <= xn < u
    // Worst case we need B(n, u) = ceil(lg(C(u, n))) bits
    // that means, n = amount of integers in our subset S
    //             u = biggest integer in our universe U, such that xn < u, xn \in S
    //
    // if we take the biggest C unsigned integer u = 4294967295 or 0xffffffff, we have that u >> n
    // and since u is too big we can use Stirling to approximate the convergence of the factorial
    //
    // such that the new equation for worst case is
    // B(n, u) = n lg(u/n) + n lg(e) = n (lg(u/n) + lg(e)), which is around O(lg(u)) bits since u >> n

    std::string dir = "data_g/";
    std::string ext = ".txt";
    std::ifstream file_A(dir + fileName + "/" + mode + "_" + fileName + ext); // input file stream to open the file A.txt

    // read it as a vector
    std::string line_A;
    std::string value_as_str;

    std::vector<unsigned __int128> gap_vector_uniques;
    static const __uint128_t UINT128_MAX =__uint128_t(__int128_t(-1L));



    long double u = (long double)UINT128_MAX;
    unsigned __int128 n = 0;
    
    if (file_A.is_open() && file_A.good())
    {
        // Reading each line of the file
        while (getline(file_A, line_A))
        {
            n += 1;
        }
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }

    std::ofstream worstCaseEntropyFile;
    worstCaseEntropyFile.open(dir + fileName + "/compressed/worstCaseEntropy_" + mode + "_" + fileName + ext);

    // M_LOG2E equivale a log2(e) y tiene valor definido en cmath
    worstCaseEntropyFile << n * (log2(u/n) + M_LOG2E);
    worstCaseEntropyFile.close();
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

unsigned __int128 toNumber(std::string bitvector)
{
    unsigned i;
    unsigned __int128 length = bitvector.length();
    unsigned __int128 decimal = 0;
    
    for (i = 0; i < length; i++) {
        auto tmp = (bitvector[length - i - 1] - '0') * 2;
        if (tmp != 0) {
            decimal += std::pow(tmp, i); 
        }
    }
    return decimal;
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

    std::cout << "consegui el mas grande " << lastline << " con " << n_bits << " bits"<< std::endl;
    std::ofstream trieVectorSizeFile;
    trieVectorSizeFile.open(dir + fileName + "/compressed/trieVector_3_size_" + mode + "_" + fileName + ext);
    trieVectorSizeFile << current_bit_sec.length() << "\n";
    trieVectorSizeFile.close();

    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream trieVectorFile;
        std::ofstream trieVectorNumberFile;
        trieVectorFile.open(dir + fileName + "/compressed/trieVector_2_" + mode + "_" + fileName + ext);
        trieVectorNumberFile.open(dir + fileName + "/compressed/trieVector_3_" + mode + "_" + fileName + ext);

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
                    trieVectorFile << current_bit_sec << "\n";
                    trieVectorNumberFile << uint128ToString(toNumber(current_bit_sec)) << "\n";

                } else {
                    std::string tmp_sec_aux = "";
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
                            tmp_sec_aux += aux[j - dif];
                            trieVectorFile << aux[j - dif];
                            // trieVectorFile << aux[j - dif] << "\n";
                        }
                    }
                    if (flag) {
                        trieVectorFile << "\n";
                        trieVectorNumberFile << uint128ToString(toNumber(tmp_sec_aux)) << "\n";
                    }
    
                    current_bit_sec = new_bit_sec;
                }
                idx += 1;
            }   
            trieEntropy += x_i;
        }
        trieVectorFile.close();
        trieVectorNumberFile.close();
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

int oddsToOne(std::string number) {
    unsigned length = number.length();
    std::string lastChar;
    lastChar = number[length - 1];

    if (lastChar == "1" || lastChar == "3" || lastChar == "5" || lastChar == "7" || lastChar == "9") {
        return 1;
    }
    return 0;
}

std::string divideByTwo(std::string number) {
    std::string new_number = "";
    std::string current_digit = "";
    int new_digit;
    int add = 0;

    unsigned i;
    unsigned length = number.length();

    for (i = 0; i < length; i++) {
        current_digit = number[i];
        new_digit = (number[i] - '0') / 2 + add;
        // new_dgt = (ord(ch) - ord('0'))

        new_number += std::to_string(new_digit);
        add = oddsToOne(current_digit) * 5;
    }

    if (new_number.length() > 0) {
        current_digit = new_number[0];
        if (current_digit == "0") {
            new_number = new_number.substr(1, new_number.length() - 1);
        }
    }
    return new_number;
}

std::string decimalToBinary(std::string number) {
    std::string one = "1";
    std::string zero = "0";
    std::string lastChar = "";
    
    unsigned i;
    unsigned length = number.length();

    std::string binary = "";

    for (i = 0; i < length; i++) {
        lastChar = number[length - i - 1];
        if (lastChar == "1" || lastChar == "3" || lastChar == "5" || lastChar == "7" || lastChar == "9") {
            binary = one + binary;
        } else {
            binary = zero + binary;
        }
    }
    return binary;
}

void FileReader::getTrieFromFile2(const std::string fileName, const std::string mode)
{
    // read it as a vector
    std::string line_A;
    std::string value_as_str;
    std::string lastline;
    std::string current_bit_sec, aux, new_bit_sec;

    unsigned x_i, dif, j;
    unsigned __int128 idx = 0;
    unsigned __int128 trieEntropy = 0;

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

    std::cout << "consegui el mas grande " << lastline << " con " << n_bits << " bits"<< std::endl;
    std::ofstream trieVectorSizeFile;
    trieVectorSizeFile.open(dir + fileName + "/compressed/trieVector_3_size_" + mode + "_" + fileName + ext);
    trieVectorSizeFile << current_bit_sec.length() << "\n";
    trieVectorSizeFile.close();

    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        std::ofstream trieVectorFile;
        std::ofstream trieVectorNumberFile;
        trieVectorFile.open(dir + fileName + "/compressed/trieVector_2_2_" + mode + "_" + fileName + ext);
        trieVectorNumberFile.open(dir + fileName + "/compressed/trieVector_3_2_" + mode + "_" + fileName + ext);

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
                // element_A = atouint128_t(value_as_str);
                std::string tmp_num = value_as_str;
                std::string stack = "";

                if (tmp_num == "0") {
                    stack = "0";
                } else {
                    while (tmp_num.length() > 0) {
                        stack = std::to_string(oddsToOne(tmp_num)) + stack;
                        tmp_num = divideByTwo(tmp_num);
                    }
                }

                // std::cout << "element_A = " << uint128ToString(element_A) << std::endl;
                flag = false;
                x_i = 0;

                if (idx == 0) {
                    // current_bit_sec = toBinary(element_A, n_bits);
                    current_bit_sec = stack;
                    while (current_bit_sec.length() < n_bits) {
                        current_bit_sec = "0" + current_bit_sec;
                    }
                    x_i = current_bit_sec.length();
                    trieVectorFile << current_bit_sec << "\n";
                    trieVectorNumberFile << uint128ToString(toNumber(current_bit_sec)) << "\n";

                } else {
                    std::string tmp_sec_aux = "";
                    // aux = toBinary(element_A);
                    aux = stack;
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
                            tmp_sec_aux += aux[j - dif];
                            trieVectorFile << aux[j - dif];
                            // trieVectorFile << aux[j - dif] << "\n";
                        }
                    }
                    if (flag) {
                        trieVectorFile << "\n";
                        trieVectorNumberFile << uint128ToString(toNumber(tmp_sec_aux)) << "\n";
                    }
    
                    current_bit_sec = new_bit_sec;
                }
                idx += 1;
            }   
            trieEntropy += x_i;
        }
        trieVectorFile.close();
        trieVectorNumberFile.close();
        // std::cout << "Entropy ready" << std::endl; 
        std::ofstream trieEntropyFile;
        trieEntropyFile.open(dir + fileName + "/compressed/trieEntropy_2_" + mode + "_" + fileName + ext);
        trieEntropyFile << "trieEntropy: " << uint128ToString(trieEntropy);
        trieEntropyFile.close();
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }
}


RTrieNode::RTrieNode(std::vector<unsigned __int128> z_order_vector, unsigned __int128 start, unsigned __int128 end)
{
    // z_order_vector should be the full matrix
    // end should be the number of columns or rows of the column (check size_fileName.txt)
    
    unsigned __int128 i, j;
    this->value = true;
    this->up_left = nullptr;
    this->up_right = nullptr;
    this->down_left = nullptr;
    this->down_right = nullptr;

    this->heigth = (z_order_vector.size() / 4);

    if ((end - start) == 1) {
        this->value = z_order_vector[start] == 1;
    } else {
        i = start;
        j = i + ((end - start) / 4);

        std::cout << "1st q" << std::endl;
        std::cout << "i = " << uint128ToString(i) << "  - j = " << uint128ToString(j) << std::endl;


        // Primer cuadrante (up_left)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->up_left = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        std::cout << "2nd q" << std::endl;
        std::cout << "i = " << uint128ToString(i) << "  - j = " << uint128ToString(j) << std::endl;

        // Segundo cuadrante (up_right)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->up_right = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        std::cout << "3rd q" << std::endl;
        std::cout << "i = " << uint128ToString(i) << "  - j = " << uint128ToString(j) << std::endl;

        // Tercer cuadrante (down_left)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->down_left = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        std::cout << "4th q" << std::endl;
        std::cout << "i = " << uint128ToString(i) << "  - j = " << uint128ToString(j) << std::endl;

        // Cuarto cuadrante (down_right)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->down_right = new RTrieNode(z_order_vector, i, j);
        }


        std::cout << "cleaning" << std::endl;

        if (!this->up_left->value && !this->up_right->value && !this->down_left->value && !this->down_right->value) {
            delete this->up_left;
            delete this->up_right;
            delete this->down_left;
            delete this->down_right;
            this->up_left = nullptr;
            this->up_right = nullptr;
            this->down_left = nullptr;
            this->down_right = nullptr;
            this->value = false;
        }
    }
}

RTrieNode::~RTrieNode()
{
}

int RTrieNode::getTrieHeigth()
{
    return this->heigth;
}

void RTrieNode::makeZOrderVector() {
    makeZOrderVector(this, this->z_order_vector);
}
// Function to make a compressed z-order vector
void RTrieNode::makeZOrderVector(RTrieNode* node, std::vector<unsigned __int128> &z_order_vector)
{
    if (!node) {
        return;
    }
    // Deal with the node
    z_order_vector.push_back((int)node->value);

    // Recur on up_left subtree
    makeZOrderVector(node->up_left, z_order_vector);

    // Recur on up_right subtree
    makeZOrderVector(node->up_right, z_order_vector);

    // Recur on down_left subtree
    makeZOrderVector(node->down_left, z_order_vector);

    // Recur on down_right subtree
    makeZOrderVector(node->down_right, z_order_vector);
}

void RTrieNode::saveZOrderVector(const std::string fileName, const std::string mode)
{
    std::string dir = "data_g/";
    std::string ext = ".txt";

    std::ofstream rtrieVectorFile;
    rtrieVectorFile.open(dir + fileName + "/rtrie_zvector_" + mode + "_" + fileName + ext);

    for(unsigned __int128 i = 0; i < this->getZOrderVector().size(); i++){
        rtrieVectorFile << uint128ToString(this->getZOrderVector()[i]) << "\n";
    }

}

std::vector<unsigned __int128> RTrieNode::getZOrderVector() const
{
    return this->z_order_vector;
}

// Function to print preorder traversal
void RTrieNode::printPreorder(RTrieNode* node)
{
    if (!node) {
        return;
    }
    // Deal with the node
    std::cout << node->value << " ";

    // Recur on up_left subtree
    printPreorder(node->up_left);

    // Recur on up_right subtree
    printPreorder(node->up_right);

    // Recur on down_left subtree
    printPreorder(node->down_left);

    // Recur on down_right subtree
    printPreorder(node->down_right);
}
