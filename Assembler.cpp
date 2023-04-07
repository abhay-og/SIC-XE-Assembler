#include <bits/stdc++.h>
using namespace std;

string getString(char c)
{
    return string(&c, 1);
}

string IntToStringHex(int x, int fill = 5)
{
    stringstream s;
    s << setfill('0') << setw(fill) << hex << x;
    string temp = s.str();
    temp = temp.substr(temp.length() - fill, fill);
    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    return temp;
}

string ExpandString(string data, int length, char fillChar, bool back = false)
{
    if (back)
    {
        if (length <= data.length())
        {
            return data.substr(0, length);
        }
        else
        {
            for (int i = length - data.length(); i > 0; i--)
            {
                data += fillChar;
            }
        }
    }
    else
    {
        if (length <= data.length())
        {
            return data.substr(data.length() - length, length);
        }
        else
        {
            for (int i = length - data.length(); i > 0; i--)
            {
                data = fillChar + data;
            }
        }
    }
    return data;
}
int stringHexToInt(string x)
{
    return stoul(x, nullptr, 16);
}

string stringToHexString(const string &input)
{
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

bool checkWhiteSpace(char x)
{
    if (x == ' ' || x == '\t')
    {
        return true;
    }
    return false;
}

bool checkCommentLine(string line)
{
    if (line[0] == '.')
    {
        return true;
    }
    return false;
}

bool if_all_num(string x)
{
    bool all_num = true;
    int i = 0;
    while (all_num && (i < x.length()))
    {
        all_num &= isdigit(x[i++]);
    }
    return all_num;
}

void readFirstNonWhiteSpace(string line, int &index, bool &status, string &data, bool readTillEnd = false)
{
    data = "";
    status = true;
    if (readTillEnd)
    {
        data = line.substr(index, line.length() - index);
        if (data == "")
        {
            status = false;
        }
        return;
    }
    while (index < line.length() && !checkWhiteSpace(line[index]))
    { // If no whitespace then data
        data += line[index];
        index++;
    }

    if (data == "")
    {
        status = false;
    }

    while (index < line.length() && checkWhiteSpace(line[index]))
    { // Increase index to pass all whitespace
        index++;
    }
}

void readByteOperand(string line, int &index, bool &status, string &data)
{
    data = "";
    status = true;
    if (line[index] == 'C')
    {
        data += line[index++];
        char identifier = line[index++];
        data += identifier;
        while (index < line.length() && line[index] != identifier)
        { // Copy all data until next identifier regardless of whitespace
            data += line[index];
            index++;
        }
        data += identifier;
        index++; // Shift to next of identifier
    }
    else
    {
        while (index < line.length() && !checkWhiteSpace(line[index]))
        { // In no whitespace then data
            data += line[index];
            index++;
        }
    }

    if (data == "")
    {
        status = false;
    }

    while (index < line.length() && checkWhiteSpace(line[index]))
    { // Increase index to pass all whitespace
        index++;
    }
}

void writeToFile(ofstream &file, string data, bool newline = true)
{
    if (newline)
    {
        file << data << endl;
    }
    else
    {
        file << data;
    }
}

string GetOpCode(string opcode)
{
    if (opcode[0] == '+' || opcode[0] == '@')
    {
        return opcode.substr(1, opcode.length() - 1);
    }
    return opcode;
}

char getFlagFormat(string data)
{
    if (data[0] == '#' || data[0] == '+' || data[0] == '@' || data[0] == '=')
    {
        return data[0];
    }
    return ' ';
}

class EvalString
{
public:
    int getResult();
    EvalString(string data);

private:
    string storedData;
    int index;
    char peek();
    char get();
    int term();
    int factor();
    int number();
};

EvalString::EvalString(string data)
{
    storedData = data;
    index = 0;
}

int EvalString::getResult()
{
    int result = term();
    while (peek() == '+' || peek() == '-')
    {
        if (get() == '+')
        {
            result += term();
        }
        else
        {
            result -= term();
        }
    }
    return result;
}

int EvalString::term()
{
    int result = factor();
    while (peek() == '*' || peek() == '/')
    {
        if (get() == '*')
        {
            result *= factor();
        }
        else
        {
            result /= factor();
        }
    }
    return result;
}

int EvalString::factor()
{
    if (peek() >= '0' && peek() <= '9')
    {
        return number();
    }
    else if (peek() == '(')
    {
        get();
        int result = getResult();
        get();
        return result;
    }
    else if (peek() == '-')
    {
        get();
        return -factor();
    }
    return 0;
}

int EvalString::number()
{
    int result = get() - '0';
    while (peek() >= '0' && peek() <= '9')
    {
        result = 10 * result + get() - '0';
    }
    return result;
}

char EvalString::get()
{
    return storedData[index++];
}

char EvalString::peek()
{
    return storedData[index];
}

/*Conatains code for the TABLES defined across whole assembler*/

struct ExtDefStructure
{
    string name;
    string address;
    char exists;
    ExtDefStructure()
    {
        name = "undefined";
        address = "0";
        exists = 'n';
    }
};

struct ExtRefStructure{
    string name;
    string address;
    char exists;
    ExtRefStructure()
    {
        name = "undefined";
        address = "0";
        exists = 'n';
    }
};

struct ControlSection{
    string name;
    string LOCCTR;
    int section_number;
    int length;
    map<string, ExtDefStructure> EXTDEF_TAB;
    map<string, ExtRefStructure> EXTREF_TAB;
    ControlSection(){
        name = "DEFAULT";
        LOCCTR = "0";
        section_number = 0;
        length = 0;
    }
};

struct OpcodeStructure{
    string opcode;
    int format;
    char exists;
    OpcodeStructure(){
        opcode = "undefined";
        format = 0;
        exists = 'n';
    }
};

struct LiteralStructure{
    string value;
    string address;
    char exists;
    int blockNumber = 0;
    LiteralStructure(){
        value = "";
        address = "?";
        blockNumber = 0;
        exists = 'n';
    }
};

struct LabelStructure{
    string address;
    string name;
    int relative;
    int blockNumber;
    char exists;
    LabelStructure(){
        name = "undefined";
        address = "0";
        blockNumber = 0;
        exists = 'n';
        relative = 0;
    }
};

struct BlockStructure
{
    string startAddress;
    string name;
    string LOCCTR;
    int number;
    char exists;
    BlockStructure(){
        name = "undefined";
        startAddress = "?";
        exists = 'n';
        number = -1;
        LOCCTR = "0";
    }
};

struct struct_register
{
    char num;
    char exists;
    struct_register(){
        num = 'F';
        exists = 'n';
    }
};

typedef map<string, LabelStructure> SYMBOL_TABLE_TYPE;
typedef map<string, OpcodeStructure> OPCODE_TABLE_TYPE;
typedef map<string, struct_register> REG_TABLE_TYPE;
typedef map<string, LiteralStructure> LIT_TABLE_TYPE;
typedef map<string, BlockStructure> BLOCK_TABLE_TYPE;
typedef map<string, ControlSection> CSECT_TABLE_TYPE;

SYMBOL_TABLE_TYPE SYMTAB;
OPCODE_TABLE_TYPE OPTAB;
REG_TABLE_TYPE REGTAB;
LIT_TABLE_TYPE LITTAB;
BLOCK_TABLE_TYPE BLOCKS;
CSECT_TABLE_TYPE CSECT_TAB;

void load_REGTAB()
{
    REGTAB["A"].num = '0',REGTAB["A"].exists = 'y';
    REGTAB["X"].num = '1',REGTAB["X"].exists = 'y';
    REGTAB["L"].num = '2',REGTAB["L"].exists = 'y';
    REGTAB["B"].num = '3',REGTAB["B"].exists = 'y';
    REGTAB["S"].num = '4',REGTAB["S"].exists = 'y';
    REGTAB["T"].num = '5',REGTAB["T"].exists = 'y';
    REGTAB["F"].num = '6',REGTAB["F"].exists = 'y';
    REGTAB["PC"].num = '8',REGTAB["PC"].exists = 'y';
    REGTAB["SW"].num = '9',REGTAB["SW"].exists = 'y';
}

void load_OPTAB()
{
    OPTAB["ADD"].opcode = "18", OPTAB["ADD"].format = 3, OPTAB["ADD"].exists = 'y';
    OPTAB["ADDF"].opcode = "58", OPTAB["ADDF"].format = 3, OPTAB["ADDF"].exists = 'y';
    OPTAB["ADDR"].opcode = "90", OPTAB["ADDR"].format = 2, OPTAB["ADDR"].exists = 'y';
    OPTAB["AND"].opcode = "40", OPTAB["AND"].format = 3, OPTAB["AND"].exists = 'y';
    OPTAB["CLEAR"].opcode = "B4", OPTAB["CLEAR"].format = 2, OPTAB["CLEAR"].exists = 'y';
    OPTAB["COMP"].opcode = "28", OPTAB["COMP"].format = 3, OPTAB["COMP"].exists = 'y';
    OPTAB["COMPF"].opcode = "88", OPTAB["COMPF"].format = 3, OPTAB["COMPF"].exists = 'y';
    OPTAB["COMPR"].opcode = "A0", OPTAB["COMPR"].format = 2, OPTAB["COMPR"].exists = 'y';
    OPTAB["DIV"].opcode = "24", OPTAB["DIV"].format = 3, OPTAB["DIV"].exists = 'y';
    OPTAB["DIVF"].opcode = "64", OPTAB["DIVF"].format = 3, OPTAB["DIVF"].exists = 'y';
    OPTAB["DIVR"].opcode = "9C", OPTAB["DIVR"].format = 2, OPTAB["DIVR"].exists = 'y';
    OPTAB["FIX"].opcode = "C4", OPTAB["FIX"].format = 1, OPTAB["FIX"].exists = 'y';
    OPTAB["FLOAT"].opcode = "C0", OPTAB["FLOAT"].format = 1, OPTAB["FLOAT"].exists = 'y';
    OPTAB["HIO"].opcode = "F4", OPTAB["HIO"].format = 1, OPTAB["HIO"].exists = 'y';
    OPTAB["J"].opcode = "3C", OPTAB["J"].format = 3, OPTAB["J"].exists = 'y';
    OPTAB["JEQ"].opcode = "30", OPTAB["JEQ"].format = 3, OPTAB["JEQ"].exists = 'y';
    OPTAB["JGT"].opcode = "34", OPTAB["JGT"].format = 3, OPTAB["JGT"].exists = 'y';
    OPTAB["JLT"].opcode = "38", OPTAB["JLT"].format = 3, OPTAB["JLT"].exists = 'y';
    OPTAB["JSUB"].opcode = "48", OPTAB["JSUB"].format = 3, OPTAB["JSUB"].exists = 'y';
    OPTAB["LDA"].opcode = "00", OPTAB["LDA"].format = 3, OPTAB["LDA"].exists = 'y';
    OPTAB["LDB"].opcode = "68", OPTAB["LDB"].format = 3, OPTAB["LDB"].exists = 'y';
    OPTAB["LDCH"].opcode = "50", OPTAB["LDCH"].format = 3, OPTAB["LDCH"].exists = 'y';
    OPTAB["LDF"].opcode = "70", OPTAB["LDF"].format = 3, OPTAB["LDF"].exists = 'y';
    OPTAB["LDL"].opcode = "08", OPTAB["LDL"].format = 3, OPTAB["LDL"].exists = 'y';
    OPTAB["LDS"].opcode = "6C", OPTAB["LDS"].format = 3, OPTAB["LDS"].exists = 'y';
    OPTAB["LDT"].opcode = "74", OPTAB["LDT"].format = 3, OPTAB["LDT"].exists = 'y';
    OPTAB["LDX"].opcode = "04", OPTAB["LDX"].format = 3, OPTAB["LDX"].exists = 'y';
    OPTAB["LPS"].opcode = "D0", OPTAB["LPS"].format = 3, OPTAB["LPS"].exists = 'y';
    OPTAB["MUL"].opcode = "20", OPTAB["MUL"].format = 3, OPTAB["MUL"].exists = 'y';
    OPTAB["MULF"].opcode = "60", OPTAB["MULF"].format = 3, OPTAB["MULF"].exists = 'y';
    OPTAB["MULR"].opcode = "98", OPTAB["MULR"].format = 2, OPTAB["MULR"].exists = 'y';
    OPTAB["NORM"].opcode = "C8", OPTAB["NORM"].format = 1, OPTAB["NORM"].exists = 'y';
    OPTAB["OR"].opcode = "44", OPTAB["OR"].format = 3, OPTAB["OR"].exists = 'y';
    OPTAB["RD"].opcode = "D8", OPTAB["RD"].format = 3, OPTAB["RD"].exists = 'y';
    OPTAB["RMO"].opcode = "AC", OPTAB["RMO"].format = 2, OPTAB["RMO"].exists = 'y';
    OPTAB["RSUB"].opcode = "4F", OPTAB["RSUB"].format = 3, OPTAB["RSUB"].exists = 'y';
    OPTAB["SHIFTL"].opcode = "A4", OPTAB["SHIFTL"].format = 2, OPTAB["SHIFTL"].exists = 'y';
    OPTAB["SHIFTR"].opcode = "A8", OPTAB["SHIFTR"].format = 2, OPTAB["SHIFTR"].exists = 'y';
    OPTAB["SIO"].opcode = "F0", OPTAB["SIO"].format = 1, OPTAB["SIO"].exists = 'y';
    OPTAB["SSK"].opcode = "EC", OPTAB["SSK"].format = 3, OPTAB["SSK"].exists = 'y';
    OPTAB["STA"].opcode = "0C", OPTAB["STA"].format = 3, OPTAB["STA"].exists = 'y';
    OPTAB["STB"].opcode = "78", OPTAB["STB"].format = 3, OPTAB["STB"].exists = 'y';
    OPTAB["STCH"].opcode = "54", OPTAB["STCH"].format = 3, OPTAB["STCH"].exists = 'y';
    OPTAB["STF"].opcode = "80", OPTAB["STF"].format = 3, OPTAB["STF"].exists = 'y';
    OPTAB["STI"].opcode = "D4", OPTAB["STI"].format = 3, OPTAB["STI"].exists = 'y';
    OPTAB["STL"].opcode = "14", OPTAB["STL"].format = 3, OPTAB["STL"].exists = 'y';
    OPTAB["STS"].opcode = "7C", OPTAB["STS"].format = 3, OPTAB["STS"].exists = 'y';
    OPTAB["STSW"].opcode = "E8", OPTAB["STSW"].format = 3, OPTAB["STSW"].exists = 'y';
    OPTAB["STT"].opcode = "84", OPTAB["STT"].format = 3, OPTAB["STT"].exists = 'y';
    OPTAB["STX"].opcode = "10", OPTAB["STX"].format = 3, OPTAB["STX"].exists = 'y';
    OPTAB["SUB"].opcode = "1C", OPTAB["SUB"].format = 3, OPTAB["SUB"].exists = 'y';
    OPTAB["SUBF"].opcode = "5C", OPTAB["SUBF"].format = 3, OPTAB["SUBF"].exists = 'y';
    OPTAB["SUBR"].opcode = "94", OPTAB["SUBR"].format = 2, OPTAB["SUBR"].exists = 'y';
    OPTAB["SVC"].opcode = "B0", OPTAB["SVC"].format = 2, OPTAB["SVC"].exists = 'y';
    OPTAB["TD"].opcode = "E0", OPTAB["TD"].format = 3, OPTAB["TD"].exists = 'y';
    OPTAB["TIO"].opcode = "F8", OPTAB["TIO"].format = 1, OPTAB["TIO"].exists = 'y';
    OPTAB["TIX"].opcode = "2C", OPTAB["TIX"].format = 3, OPTAB["TIX"].exists = 'y';
    OPTAB["TIXR"].opcode = "B8", OPTAB["TIXR"].format = 2, OPTAB["TIXR"].exists = 'y';
    OPTAB["WD"].opcode = "DC", OPTAB["WD"].format = 3, OPTAB["WD"].exists = 'y';
}

void load_BLOCKS()
{
    BLOCKS["DEFAULT"].exists = 'y';
    BLOCKS["DEFAULT"].name = "DEFAULT";
    BLOCKS["DEFAULT"].startAddress = "00000";
    BLOCKS["DEFAULT"].number = 0;
    BLOCKS["DEFAULT"].LOCCTR = "0";
}

void load_tables()
{
    load_BLOCKS();
    load_OPTAB();
    load_REGTAB();
}

/*Code for pass1*/

string fileName;
bool error_flag = false;
int program_length;
string *BLocksNumToName;

string firstExecutable_Sec;

void handle_LTORG(string &litPrefix, int &lineNumberDelta, int lineNumber, int &LOCCTR, int &lastDeltaLOCCTR, int currentBlockNumber)
{
    string litAddress, litValue;
    litPrefix = "";
    for (auto const &it : LITTAB)
    {
        litAddress = it.second.address;
        litValue = it.second.value;
        if (litAddress != "?"){
            // .....
        }
        else{
            lineNumber += 5;
            lineNumberDelta += 5;
            LITTAB[it.first].address = IntToStringHex(LOCCTR);
            LITTAB[it.first].blockNumber = currentBlockNumber;
            litPrefix += "\n" + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + "*" + "\t" + "=" + litValue + "\t" + " " + "\t" + " ";

            if (litValue[0] == 'X')
            {
                LOCCTR += (litValue.length() - 3) / 2;
                lastDeltaLOCCTR += (litValue.length() - 3) / 2;
            }
            else if (litValue[0] == 'C')
            {
                LOCCTR += litValue.length() - 3;
                lastDeltaLOCCTR += litValue.length() - 3;
            }
        }
    }
}

void evaluateExpression(string expression, bool &relative, string &tempOperand, int lineNumber, ofstream &errorFile, bool &error_flag)
{
    string singleOperand = "?", singleOperator = "?", valueString = "", valueTemp = "", writeData = "";
    int lastOperand = 0, lastOperator = 0, pairCount = 0;
    char lastByte = ' ';
    bool Illegal = false;

    for (int i = 0; i < expression.length();)
    {
        singleOperand = "";

        lastByte = expression[i];
        while ((lastByte != '+' && lastByte != '-' && lastByte != '/' && lastByte != '*') && i < expression.length())
        {
            singleOperand += lastByte;
            lastByte = expression[++i];
        }

        if (SYMTAB[singleOperand].exists == 'y')
        { // Check operand existence
            lastOperand = SYMTAB[singleOperand].relative;
            valueTemp = to_string(stringHexToInt(SYMTAB[singleOperand].address));
        }
        else if ((singleOperand != "" || singleOperand != "?") && if_all_num(singleOperand))
        {
            lastOperand = 0;
            valueTemp = singleOperand;
        }
        else
        {
            writeData = "Line: " + to_string(lineNumber) + " : Can't find symbol. Found " + singleOperand;
            writeToFile(errorFile, writeData);
            Illegal = true;
            break;
        }

        if (lastOperand * lastOperator == 1)
        { // Check expressions legallity
            writeData = "Line: " + to_string(lineNumber) + " : Illegal expression";
            writeToFile(errorFile, writeData);
            error_flag = true;
            Illegal = true;
            break;
        }
        else if ((singleOperator == "-" || singleOperator == "+" || singleOperator == "?") && lastOperand == 1)
        {
            if (singleOperator == "-")
            {
                pairCount--;
            }
            else
            {
                pairCount++;
            }
        }

        valueString += valueTemp;

        singleOperator = "";
        while (i < expression.length() && (lastByte == '+' || lastByte == '-' || lastByte == '/' || lastByte == '*'))
        {
            singleOperator += lastByte;
            lastByte = expression[++i];
        }

        if (singleOperator.length() > 1)
        {
            writeData = "Line: " + to_string(lineNumber) + " : Illegal operator in expression. Found " + singleOperator;
            writeToFile(errorFile, writeData);
            error_flag = true;
            Illegal = true;
            break;
        }

        if (singleOperator == "*" || singleOperator == "/")
        {
            lastOperator = 1;
        }
        else
        {
            lastOperator = 0;
        }

        valueString += singleOperator;
    }

    if (!Illegal)
    {
        if (pairCount == 1)
        {
            /*relative*/
            relative = 1;
            EvalString tempOBJ(valueString);
            tempOperand = IntToStringHex(tempOBJ.getResult());
        }
        else if (pairCount == 0)
        {
            /*absolute*/
            relative = 0;
            cout << valueString << endl;
            EvalString tempOBJ(valueString);
            tempOperand = IntToStringHex(tempOBJ.getResult());
        }
        else
        {
            writeData = "Line: " + to_string(lineNumber) + " : Illegal expression";
            writeToFile(errorFile, writeData);
            error_flag = true;
            tempOperand = "00000";
            relative = 0;
        }
    }
    else
    {
        tempOperand = "00000";
        error_flag = true;
        relative = 0;
    }
}
void pass1()
{
    ifstream sourceFile; // begin
    ofstream intermediateFile, errorFile;

    sourceFile.open(fileName);
    if (!sourceFile)
    {
        cout << "Unable to open file: " << fileName << endl;
        exit(1);
    }

    intermediateFile.open("intermediate_" + fileName);
    if (!intermediateFile)
    {
        cout << "Unable to open file: intermediate_" << fileName << endl;
        exit(1);
    }
    writeToFile(intermediateFile, "Line\tAddress\tLabel\tOPCODE\tOPERAND\tComment");
    errorFile.open("error_" + fileName);
    if (!errorFile)
    {
        cout << "Unable to open file: error_" << fileName << endl;
        exit(1);
    }
    writeToFile(errorFile, "************PASS1************");

    string fileLine;
    string writeData, writeDataSuffix = "", writeDataPrefix = "";
    int index = 0;

    string currentBlockName = "DEFAULT";
    int currentBlockNumber = 0;
    int totalBlocks = 1;

    bool statusCode;
    string label, opcode, operand, comment;
    string tempOperand;

    int startAddress, LOCCTR, saveLOCCTR, lineNumber, lastDeltaLOCCTR, lineNumberDelta = 0;
    lineNumber = 0;
    lastDeltaLOCCTR = 0;

    getline(sourceFile, fileLine);
    lineNumber += 5;
    while (checkCommentLine(fileLine))
    {
        writeData = to_string(lineNumber) + "\t" + fileLine;
        writeToFile(intermediateFile, writeData);
        getline(sourceFile, fileLine); // read first input line
        lineNumber += 5;
        index = 0;
    }

    readFirstNonWhiteSpace(fileLine, index, statusCode, label);
    readFirstNonWhiteSpace(fileLine, index, statusCode, opcode);

    if (opcode == "START")
    {
        readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
        readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
        startAddress = stringHexToInt(operand);
        // cout<<startAddress<<endl;
        // exit(0);
        LOCCTR = startAddress;
        writeData = to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment;
        writeToFile(intermediateFile, writeData); // Write file to intermediate file

        getline(sourceFile, fileLine); // Read next line
        lineNumber += 5;
        index = 0;
        readFirstNonWhiteSpace(fileLine, index, statusCode, label);  // Parse label
        readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); // Parse OPCODE
    }
    else
    {
        startAddress = 0;
        LOCCTR = 0;
    }
    //*************************************************************
    string currentSectName = "DEFAULT";
    int sectionCounter = 0;
    //**********************************************************************
    while (opcode != "END")
    {
        //****************************************************************

        while (opcode != "END" && opcode != "CSECT")
        {

            //****************************************************************
            if (!checkCommentLine(fileLine))
            {
                if (label != "")
                { // Label found
                    if (SYMTAB[label].exists == 'n')
                    {
                        SYMTAB[label].name = label;
                        SYMTAB[label].address = IntToStringHex(LOCCTR);
                        SYMTAB[label].relative = 1;
                        SYMTAB[label].exists = 'y';
                        SYMTAB[label].blockNumber = currentBlockNumber;

                        //***************************************************************************
                        if (CSECT_TAB[currentSectName].EXTDEF_TAB[label].exists == 'y')
                        {
                            CSECT_TAB[currentSectName].EXTDEF_TAB[label].address = SYMTAB[label].address;
                        }
                        //****************************************************************************
                    }
                    else
                    {
                        writeData = "Line: " + to_string(lineNumber) + " : Duplicate symbol for '" + label + "'. Previously defined at " + SYMTAB[label].address;
                        writeToFile(errorFile, writeData);
                        error_flag = true;
                    }
                }
                if (OPTAB[GetOpCode(opcode)].exists == 'y')
                { // Search for opcode in OPTAB
                    if (OPTAB[GetOpCode(opcode)].format == 3)
                    {
                        LOCCTR += 3;
                        lastDeltaLOCCTR += 3;
                        if (getFlagFormat(opcode) == '+')
                        {
                            LOCCTR += 1;
                            lastDeltaLOCCTR += 1;
                        }
                        if (GetOpCode(opcode) == "RSUB")
                        {
                            operand = " ";
                        }
                        else
                        {
                            readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                            if (operand[operand.length() - 1] == ',')
                            {
                                readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                                operand += tempOperand;
                            }
                        }

                        if (getFlagFormat(operand) == '=')
                        {
                            tempOperand = operand.substr(1, operand.length() - 1);
                            if (tempOperand == "*")
                            {
                                tempOperand = "X'" + IntToStringHex(LOCCTR - lastDeltaLOCCTR, 6) + "'";
                            }
                            if (LITTAB[tempOperand].exists == 'n')
                            {
                                LITTAB[tempOperand].value = tempOperand;
                                LITTAB[tempOperand].exists = 'y';
                                LITTAB[tempOperand].address = "?";
                                LITTAB[tempOperand].blockNumber = -1;
                            }
                        }
                    }
                    else if (OPTAB[GetOpCode(opcode)].format == 1)
                    {
                        operand = " ";
                        LOCCTR += OPTAB[GetOpCode(opcode)].format;
                        lastDeltaLOCCTR += OPTAB[GetOpCode(opcode)].format;
                    }
                    else
                    {
                        LOCCTR += OPTAB[GetOpCode(opcode)].format;
                        lastDeltaLOCCTR += OPTAB[GetOpCode(opcode)].format;
                        readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                        if (operand[operand.length() - 1] == ',')
                        {
                            readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                            operand += tempOperand;
                        }
                    }
                }
                //*************************************************************************************************
                else if (opcode == "EXTDEF")
                {

                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    int length = operand.length();
                    string inp = "";
                    for (int i = 0; i < length; i++)
                    {
                        while (operand[i] != ',' && i < length)
                        {
                            inp += operand[i];
                            i++;
                        }
                        CSECT_TAB[currentSectName].EXTDEF_TAB[inp].name = inp;
                        CSECT_TAB[currentSectName].EXTDEF_TAB[inp].exists = 'y';
                        inp = "";
                    }
                }
                else if (opcode == "EXTREF")
                {

                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    int length = operand.length();
                    string inp = "";
                    for (int i = 0; i < length; i++)
                    {
                        while (operand[i] != ',' && i < length)
                        {
                            inp += operand[i];
                            i++;
                        }
                        CSECT_TAB[currentSectName].EXTREF_TAB[inp].name = inp;
                        CSECT_TAB[currentSectName].EXTREF_TAB[inp].exists = 'y';
                        inp = "";
                    }
                }
                //****************************************************************************************
                else if (opcode == "WORD")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    LOCCTR += 3;
                    lastDeltaLOCCTR += 3;
                }
                else if (opcode == "RESW")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    LOCCTR += 3 * stoi(operand);
                    lastDeltaLOCCTR += 3 * stoi(operand);
                }
                else if (opcode == "RESB")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    LOCCTR += stoi(operand);
                    lastDeltaLOCCTR += stoi(operand);
                }
                else if (opcode == "BYTE")
                {
                    readByteOperand(fileLine, index, statusCode, operand);
                    if (operand[0] == 'X')
                    {
                        LOCCTR += (operand.length() - 3) / 2;
                        lastDeltaLOCCTR += (operand.length() - 3) / 2;
                    }
                    else if (operand[0] == 'C')
                    {
                        LOCCTR += operand.length() - 3;
                        lastDeltaLOCCTR += operand.length() - 3;
                    }
                    // else{
                    //   writeData = "Line: "+to_string(line)+" : Invalid operand for BYTE. Found " + operand;
                    //   writeToFile(errorFile,writeData);
                    // }
                }
                else if (opcode == "BASE")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                }
                else if (opcode == "LTORG")
                {
                    operand = " ";
                    handle_LTORG(writeDataSuffix, lineNumberDelta, lineNumber, LOCCTR, lastDeltaLOCCTR, currentBlockNumber);
                }
                else if (opcode == "ORG")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);

                    char lastByte = operand[operand.length() - 1];
                    while (lastByte == '+' || lastByte == '-' || lastByte == '/' || lastByte == '*')
                    {
                        readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                        operand += tempOperand;
                        lastByte = operand[operand.length() - 1];
                    }

                    int tempVariable;
                    tempVariable = saveLOCCTR;
                    saveLOCCTR = LOCCTR;
                    LOCCTR = tempVariable;

                    if (SYMTAB[operand].exists == 'y')
                    {
                        LOCCTR = stringHexToInt(SYMTAB[operand].address);
                    }
                    else
                    {
                        bool relative;
                        // set error_flag to false
                        error_flag = false;
                        evaluateExpression(operand, relative, tempOperand, lineNumber, errorFile, error_flag);
                        if (!error_flag)
                        {
                            LOCCTR = stringHexToInt(tempOperand);
                        }
                        error_flag = false; // reset error_flag
                    }
                }
                else if (opcode == "USE")
                {
                    // cout<<"Changing block"<<endl;
                    // for(auto const& it: BLOCKS){
                    //   cout<<it.second.name<<":"<<it.second.LOCCTR<<endl;
                    // }
                    //
                    // cout<<"Current block number: "<<currentBlockNumber<<endl;
                    // cout<<"Current LOCCTR: "<<LOCCTR<<endl;

                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    BLOCKS[currentBlockName].LOCCTR = IntToStringHex(LOCCTR);

                    if (BLOCKS[operand].exists == 'n')
                    {
                        // cout<<"Creating block: "<<operand<<endl;
                        BLOCKS[operand].exists = 'y';
                        BLOCKS[operand].name = operand;
                        BLOCKS[operand].number = totalBlocks++;
                        BLOCKS[operand].LOCCTR = "0";
                    }

                    // cout<<"Changing to: "<<operand<<endl;
                    // for(auto const& it: BLOCKS){
                    //   cout<<it.second.name<<":"<<it.second.LOCCTR<<endl;
                    // }

                    currentBlockNumber = BLOCKS[operand].number;
                    currentBlockName = BLOCKS[operand].name;
                    LOCCTR = stringHexToInt(BLOCKS[operand].LOCCTR);

                    // cout<<"Current block number: "<<currentBlockNumber<<endl;
                    // cout<<"Current LOCCTR: "<<LOCCTR<<endl;
                }
                else if (opcode == "EQU")
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    tempOperand = "";
                    bool relative;

                    if (operand == "*")
                    {
                        tempOperand = IntToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
                        relative = 1;
                    }
                    else if (if_all_num(operand))
                    {
                        tempOperand = IntToStringHex(stoi(operand), 6);
                        relative = 0;
                    }
                    else
                    {
                        char lastByte = operand[operand.length() - 1];

                        while (lastByte == '+' || lastByte == '-' || lastByte == '/' || lastByte == '*')
                        {
                            readFirstNonWhiteSpace(fileLine, index, statusCode, tempOperand);
                            operand += tempOperand;
                            lastByte = operand[operand.length() - 1];
                        }

                        // Code for reading whole operand
                        evaluateExpression(operand, relative, tempOperand, lineNumber, errorFile, error_flag);
                    }

                    SYMTAB[label].name = label;
                    SYMTAB[label].address = tempOperand;
                    SYMTAB[label].relative = relative;
                    SYMTAB[label].blockNumber = currentBlockNumber;
                    lastDeltaLOCCTR = LOCCTR - stringHexToInt(tempOperand);
                }
                else
                {
                    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
                    writeData = "Line: " + to_string(lineNumber) + " : Invalid OPCODE. Found " + opcode;
                    writeToFile(errorFile, writeData);
                    error_flag = true;
                }
                readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);
                if (opcode == "EQU" && SYMTAB[label].relative == 0)
                {
                    writeData = writeDataPrefix + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
                }
                //*********************************************************************************
                else if (opcode == "EXTDEF" || opcode == "EXTREF")
                {
                    writeData = writeDataPrefix + to_string(lineNumber) + "\t" + " " + "\t" + " " + "\t" + " " + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
       /* writeData = writeDataPrefix + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR-lastDeltaLOCCTR) + "\t" + blockNumberstr + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;   
      }*/ }
       else if (opcode == "CSECT")
       {
           writeData = writeDataPrefix + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + " " + "\t" + " " + writeDataSuffix;
       }
       //*******************************************************************************
       else
       {
           writeData = writeDataPrefix + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
       }
       writeDataPrefix = "";
       writeDataSuffix = "";
            }
            else
            {
       writeData = to_string(lineNumber) + "\t" + fileLine;
            }
            writeToFile(intermediateFile, writeData);

            BLOCKS[currentBlockName].LOCCTR = IntToStringHex(LOCCTR); // Update LOCCTR of block after every instruction
            getline(sourceFile, fileLine);                            // Read next line
            lineNumber += 5 + lineNumberDelta;
            lineNumberDelta = 0;
            index = 0;
            lastDeltaLOCCTR = 0;
            readFirstNonWhiteSpace(fileLine, index, statusCode, label);  // Parse label
            readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); // Parse OPCODE
        }
        //*****************************************************************************************

        if (opcode != "END")
        {

            if (SYMTAB[label].exists == 'n')
            {
       SYMTAB[label].name = label;
       SYMTAB[label].address = IntToStringHex(LOCCTR);
       SYMTAB[label].relative = 1;
       SYMTAB[label].exists = 'y';
       SYMTAB[label].blockNumber = currentBlockNumber;
            }

            CSECT_TAB[currentSectName].LOCCTR = IntToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
            CSECT_TAB[currentSectName].length = (LOCCTR - lastDeltaLOCCTR);
            LOCCTR = lastDeltaLOCCTR = 0;
            currentSectName = label;
            CSECT_TAB[currentSectName].name = currentSectName;
            sectionCounter++;
            CSECT_TAB[currentSectName].section_number = sectionCounter;

            writeToFile(intermediateFile, writeDataPrefix + to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + to_string(currentBlockNumber) + "\t" + label + "\t" + opcode);

            getline(sourceFile, fileLine); // Read next line
            lineNumber += 5;

            readFirstNonWhiteSpace(fileLine, index, statusCode, label);  // Parse label
            readFirstNonWhiteSpace(fileLine, index, statusCode, opcode); // Parse OPCODE
        }
        else
        {
            CSECT_TAB[currentSectName].LOCCTR = IntToStringHex(LOCCTR - lastDeltaLOCCTR, 6);
            CSECT_TAB[currentSectName].length = (LOCCTR - lastDeltaLOCCTR);

            CSECT_TAB[currentSectName].name = currentSectName;

            CSECT_TAB[currentSectName].section_number = sectionCounter;
        }
        //*******************************************************************************************
    }

    if (opcode == "END")
    {
        firstExecutable_Sec = SYMTAB[label].address;
        SYMTAB[firstExecutable_Sec].name = label;
        SYMTAB[firstExecutable_Sec].address = firstExecutable_Sec;
    }

    readFirstNonWhiteSpace(fileLine, index, statusCode, operand);
    readFirstNonWhiteSpace(fileLine, index, statusCode, comment, true);

    /*Change to deafult before dumping literals*/
    currentBlockName = "DEFAULT";
    currentBlockNumber = 0;
    LOCCTR = stringHexToInt(BLOCKS[currentBlockName].LOCCTR);

    handle_LTORG(writeDataSuffix, lineNumberDelta, lineNumber, LOCCTR, lastDeltaLOCCTR, currentBlockNumber);

    writeData = to_string(lineNumber) + "\t" + IntToStringHex(LOCCTR - lastDeltaLOCCTR) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + comment + writeDataSuffix;
    writeToFile(intermediateFile, writeData);

    int LocctrArr[totalBlocks];
    BLocksNumToName = new string[totalBlocks];
    for (auto const &it : BLOCKS)
    {
        LocctrArr[it.second.number] = stringHexToInt(it.second.LOCCTR);
        BLocksNumToName[it.second.number] = it.first;
    }

    for (int i = 1; i < totalBlocks; i++)
    {
        LocctrArr[i] += LocctrArr[i - 1];
    }

    for (auto const &it : BLOCKS)
    {
        if (it.second.startAddress == "?")
        {
            BLOCKS[it.first].startAddress = IntToStringHex(LocctrArr[it.second.number - 1]);
        }
    }

    program_length = LocctrArr[totalBlocks - 1] - startAddress;

    sourceFile.close();
    intermediateFile.close();
    errorFile.close();
}

/*Code for pass2*/
/*Declaring variables in global space*/
ifstream intermediateFile;
ofstream errorFile, objectFile, ListingFile;

//*************************

ofstream printtab;
string writestring;

//*******************

bool isComment;
string label, opcode, operand, comment;
string operand1, operand2;

int lineNumber, blockNumber, address, startAddress;
string objectCode, writeData, currentRecord, modificationRecord = "M^", endRecord, write_R_Data, write_D_Data, currentSectName = "DEFAULT";
int sectionCounter = 0;
int program_section_length = 0;

int program_counter, base_register_value, currentTextRecordLength;
bool nobase;
/*Declaration ends*/

string readTillTab(string data, int &index)
{
    string tempBuffer = "";

    while (index < data.length() && data[index] != '\t')
    {
        tempBuffer += data[index];
        index++;
    }
    index++;
    if (tempBuffer == " ")
    {
        tempBuffer = "-1";
    }
    return tempBuffer;
}
bool readIntermediateFile(ifstream &readFile, bool &isComment, int &lineNumber, int &address, int &blockNumber, string &label, string &opcode, string &operand, string &comment)
{
    string fileLine = "";
    string tempBuffer = "";
    bool tempStatus;
    int index = 0;
    if (!getline(readFile, fileLine))
    {
        return false;
    }
    lineNumber = stoi(readTillTab(fileLine, index));

    isComment = (fileLine[index] == '.') ? true : false;
    if (isComment)
    {
        readFirstNonWhiteSpace(fileLine, index, tempStatus, comment, true);
        return true;
    }
    address = stringHexToInt(readTillTab(fileLine, index));
    tempBuffer = readTillTab(fileLine, index);
    if (tempBuffer == " ")
    {
        blockNumber = -1;
    }
    else
    {
        blockNumber = stoi(tempBuffer);
    }
    label = readTillTab(fileLine, index);
    if (label == "-1")
    {
        label = " ";
    }
    opcode = readTillTab(fileLine, index);
    if (opcode == "BYTE")
    {
        readByteOperand(fileLine, index, tempStatus, operand);
    }
    else
    {
        operand = readTillTab(fileLine, index);
        if (operand == "-1")
        {
            operand = " ";
        }
        if (opcode == "CSECT")
        {
            return true;
        }
    }
    readFirstNonWhiteSpace(fileLine, index, tempStatus, comment, true);
    return true;
}

string createObjectCodeFormat34()
{
    string objcode;
    int halfBytes;
    halfBytes = (getFlagFormat(opcode) == '+') ? 5 : 3;

    if (getFlagFormat(operand) == '#')
    { // Immediate
        if (operand.substr(operand.length() - 2, 2) == ",X")
        { // Error handling for Immediate with index based
            writeData = "Line: " + to_string(lineNumber) + " Index based addressing not supported with Indirect addressing";
            writeToFile(errorFile, writeData);
            objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
            objcode += (halfBytes == 5) ? "100000" : "0000";
            return objcode;
        }

        string tempOperand = operand.substr(1, operand.length() - 1);
        if (if_all_num(tempOperand) || ((SYMTAB[tempOperand].exists == 'y') && (SYMTAB[tempOperand].relative == 0)
                                        /**/
                                        && (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'n')) /****/)
        { // Immediate integer value
            int immediateValue;

            if (if_all_num(tempOperand))
            {
       immediateValue = stoi(tempOperand);
            }
            else
            {
       immediateValue = stringHexToInt(SYMTAB[tempOperand].address);
            }
            /*Process Immediate value*/
            if (immediateValue >= (1 << 4 * halfBytes))
            { // Can't fit it
       writeData = "Line: " + to_string(lineNumber) + " Immediate value exceeds format limit";
       writeToFile(errorFile, writeData);
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += (halfBytes == 5) ? "100000" : "0000";
            }
            else
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += (halfBytes == 5) ? '1' : '0';
       objcode += IntToStringHex(immediateValue, halfBytes);
            }
            return objcode;
        }
        else if (SYMTAB[tempOperand].exists == 'n' || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
        {

            if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' || halfBytes == 3)
            {
       writeData = "Line " + to_string(lineNumber);
       if (halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
       {
           writeData += " : Invalid format for external reference " + tempOperand;
       }
       else
       {
           writeData += " : Symbol doesn't exists. Found " + tempOperand;
       }
       writeToFile(errorFile, writeData);
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += (halfBytes == 5) ? "100000" : "0000";
       return objcode;
            }

            if (SYMTAB[tempOperand].exists == 'y' && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += "100000";

       modificationRecord += "M^" + IntToStringHex(address + 1, 6) + '^';
       modificationRecord += "05+";
       modificationRecord += CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
       modificationRecord += '\n';

       return objcode;
            }
        }
        else
        {
            int operandAddress = stringHexToInt(SYMTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[tempOperand].blockNumber]].startAddress);

            /*Process Immediate symbol value*/
            if (halfBytes == 5)
            { /*If format 4*/
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += '1';
       objcode += IntToStringHex(operandAddress, halfBytes);

       /*add modifacation record here*/
       modificationRecord += "M^" + IntToStringHex(address + 1, 6) + '^';
       modificationRecord += (halfBytes == 5) ? "05" : "03";
       modificationRecord += '\n';

       return objcode;
            }

            /*Handle format 3*/
            program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
            program_counter += (halfBytes == 5) ? 4 : 3;
            int relativeAddress = operandAddress - program_counter;

            /*Try PC based*/
            if (relativeAddress >= (-2048) && relativeAddress <= 2047)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += '2';
       objcode += IntToStringHex(relativeAddress, halfBytes);
       return objcode;
            }

            /*Try base based*/
            if (!nobase)
            {
       relativeAddress = operandAddress - base_register_value;
       if (relativeAddress >= 0 && relativeAddress <= 4095)
       {
           objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
           objcode += '4';
           objcode += IntToStringHex(relativeAddress, halfBytes);
           return objcode;
       }
            }

            /*Use direct addressing with no PC or base*/
            if (operandAddress <= 4095)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 1, 2);
       objcode += '0';
       objcode += IntToStringHex(operandAddress, halfBytes);

       /*add modifacation record here*/
       modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
       modificationRecord += (halfBytes == 5) ? "05" : "03";
       modificationRecord += '\n';

       return objcode;
            }
        }
    }
    else if (getFlagFormat(operand) == '@')
    {
        string tempOperand = operand.substr(1, operand.length() - 1);
        if (tempOperand.substr(tempOperand.length() - 2, 2) == ",X" || SYMTAB[tempOperand].exists == 'n'  || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y' )
        { // Error handling for Indirect with index based

            
            if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' || halfBytes == 3)
            {
       writeData = "Line " + to_string(lineNumber);
        if (halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
       {
           writeData += " : Invalid format for external reference " + tempOperand;
       }
       else
       { 
           writeData += " : Symbol doesn't exists.Index based addressing not supported with Indirect addressing ";
       } 
       writeToFile(errorFile, writeData);
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
       objcode += (halfBytes == 5) ? "100000" : "0000";
       return objcode;
            }
            /*
                  writeData = "Line "+to_string(lineNumber);
                  writeData += (SYMTAB[tempOperand].exists=='n')?": Symbol doesn't exists":" Index based addressing not supported with Indirect addressing";
                  writeToFile(errorFile,writeData);
                  objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode)+2,2);
                  objcode += (halfBytes==5)?"100000":"0000";
                  return objcode;*/
            /***********/
            if (SYMTAB[tempOperand].exists == 'y' && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
       objcode += "100000";

       modificationRecord += "M^" + IntToStringHex(address + 1, 6) + '^';
       modificationRecord += "05+";
       modificationRecord += CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
       modificationRecord += '\n';

       return objcode;
            } 
        }
        int operandAddress = stringHexToInt(SYMTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[tempOperand].blockNumber]].startAddress);
        program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
        program_counter += (halfBytes == 5) ? 4 : 3;

        if (halfBytes == 3)
        {
            int relativeAddress = operandAddress - program_counter;
            if (relativeAddress >= (-2048) && relativeAddress <= 2047)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
       objcode += '2';
       objcode += IntToStringHex(relativeAddress, halfBytes);
       return objcode;
            }

            if (!nobase)
            {
       relativeAddress = operandAddress - base_register_value;
       if (relativeAddress >= 0 && relativeAddress <= 4095)
       {
           objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
           objcode += '4';
           objcode += IntToStringHex(relativeAddress, halfBytes);
           return objcode;
       }
            }

            if (operandAddress <= 4095)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
       objcode += '0';
       objcode += IntToStringHex(operandAddress, halfBytes);

       /*add modifacation record here*/
       modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
       modificationRecord += (halfBytes == 5) ? "05" : "03";
       modificationRecord += '\n';

       return objcode;
            }
        }
        else
        { // No base or pc based addressing in format 4
            objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
            objcode += '1';
            objcode += IntToStringHex(operandAddress, halfBytes);

            /*add modifacation record here*/
            modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
            modificationRecord += (halfBytes == 5) ? "05" : "03";
            modificationRecord += '\n';

            return objcode;
        }

        writeData = "Line: " + to_string(lineNumber);
        writeData += "Can't fit into program counter based or base register based addressing.";
        writeToFile(errorFile, writeData);
        objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 2, 2);
        objcode += (halfBytes == 5) ? "100000" : "0000";

        return objcode;
    }
    else if (getFlagFormat(operand) == '=')
    { // Literals
        string tempOperand = operand.substr(1, operand.length() - 1);

        if (tempOperand == "*")
        {
            tempOperand = "X'" + IntToStringHex(address, 6) + "'";
            /*Add modification record for value created by operand `*` */
            modificationRecord += "M^" + IntToStringHex(stringHexToInt(LITTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[LITTAB[tempOperand].blockNumber]].startAddress), 6) + '^';
            modificationRecord += IntToStringHex(6, 2);
            modificationRecord += '\n';
        }

        if (LITTAB[tempOperand].exists == 'n')
        {
            writeData = "Line " + to_string(lineNumber) + " : Symbol doesn't exists. Found " + tempOperand;
            writeToFile(errorFile, writeData);

            objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
            objcode += (halfBytes == 5) ? "000" : "0";
            objcode += "000";
            return objcode;
        }

        int operandAddress = stringHexToInt(LITTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[LITTAB[tempOperand].blockNumber]].startAddress);
        program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
        program_counter += (halfBytes == 5) ? 4 : 3;

        if (halfBytes == 3)
        {
            int relativeAddress = operandAddress - program_counter;
            if (relativeAddress >= (-2048) && relativeAddress <= 2047)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
       objcode += '2';
       objcode += IntToStringHex(relativeAddress, halfBytes);
       return objcode;
            }

            if (!nobase)
            {
       relativeAddress = operandAddress - base_register_value;
       if (relativeAddress >= 0 && relativeAddress <= 4095)
       {
           objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
           objcode += '4';
           objcode += IntToStringHex(relativeAddress, halfBytes);
           return objcode;
       }
            }

            if (operandAddress <= 4095)
            {
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
       objcode += '0';
       objcode += IntToStringHex(operandAddress, halfBytes);

       /*add modifacation record here*/
       modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
       modificationRecord += (halfBytes == 5) ? "05" : "03";
       modificationRecord += '\n';

       return objcode;
            }
        }
        else
        { // No base or pc based addressing in format 4
            objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
            objcode += '1';
            objcode += IntToStringHex(operandAddress, halfBytes);

            /*add modifacation record here*/
            modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
            modificationRecord += (halfBytes == 5) ? "05" : "03";
            modificationRecord += '\n';

            return objcode;
        }

        writeData = "Line: " + to_string(lineNumber);
        writeData += "Can't fit into program counter based or base register based addressing.";
        writeToFile(errorFile, writeData);
        objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
        objcode += (halfBytes == 5) ? "100" : "0";
        objcode += "000";

        return objcode;
    }
    else
    { /*Handle direct addressing*/
        int xbpe = 0;
        string tempOperand = operand;
        if (operand.substr(operand.length() - 2, 2) == ",X")
        {
            tempOperand = operand.substr(0, operand.length() - 2);
            xbpe = 8;
        }

        if (SYMTAB[tempOperand].exists == 'n'  || CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y' )
        {
            
            if (CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists != 'y' || halfBytes == 3)
            { 

       writeData = "Line " + to_string(lineNumber);
        if (halfBytes == 3 && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
       {
           writeData += " : Invalid format for external reference " + tempOperand;
       }
       else
       { 
           writeData += " : Symbol doesn't exists. Found " + tempOperand;
       } 
       writeToFile(errorFile, writeData);
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
       objcode += (halfBytes == 5) ? (IntToStringHex(xbpe + 1, 1) + "00") : IntToStringHex(xbpe, 1);
       objcode += "000";
       return objcode;
            }

             if (SYMTAB[tempOperand].exists == 'y' && CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].exists == 'y')
            {

       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
       objcode += "100000";

       modificationRecord += "M^" + IntToStringHex(address + 1, 6) + '^';
       modificationRecord += "05+";
       modificationRecord += CSECT_TAB[currentSectName].EXTREF_TAB[tempOperand].name;
       modificationRecord += '\n';

       return objcode;
            } 
        }
        else
        {
            /*if(SYMTAB[tempOperand].exists=='n'){
               writeData = "Line "+to_string(lineNumber)+" : Symbol doesn't exists. Found " + tempOperand;
               writeToFile(errorFile,writeData);

               objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode)+3,2);
               objcode += (halfBytes==5)?(IntToStringHex(xbpe+1,1)+"00"):IntToStringHex(xbpe,1);
               objcode += "000";
               return objcode;
             }*/

            int operandAddress = stringHexToInt(SYMTAB[tempOperand].address) + stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[tempOperand].blockNumber]].startAddress);
            program_counter = address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress);
            program_counter += (halfBytes == 5) ? 4 : 3;

            if (halfBytes == 3)
            {
       int relativeAddress = operandAddress - program_counter;
       if (relativeAddress >= (-2048) && relativeAddress <= 2047)
       {
           objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
           objcode += IntToStringHex(xbpe + 2, 1);
           objcode += IntToStringHex(relativeAddress, halfBytes);
           return objcode;
       }

       if (!nobase)
       {
           relativeAddress = operandAddress - base_register_value;
           if (relativeAddress >= 0 && relativeAddress <= 4095)
           {
               objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
               objcode += IntToStringHex(xbpe + 4, 1);
               objcode += IntToStringHex(relativeAddress, halfBytes);
               return objcode;
           }
       }

       if (operandAddress <= 4095)
       {
           objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
           objcode += IntToStringHex(xbpe, 1);
           objcode += IntToStringHex(operandAddress, halfBytes);

           /*add modifacation record here*/
           modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
           modificationRecord += (halfBytes == 5) ? "05" : "03";
           modificationRecord += '\n';

           return objcode;
       }
            }
            else
            { // No base or pc based addressing in format 4
       objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
       objcode += IntToStringHex(xbpe + 1, 1);
       objcode += IntToStringHex(operandAddress, halfBytes);

       /*add modifacation record here*/
       modificationRecord += "M^" + IntToStringHex(address + 1 + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
       modificationRecord += (halfBytes == 5) ? "05" : "03";
       modificationRecord += '\n';

       return objcode;
            }

            writeData = "Line: " + to_string(lineNumber);
            writeData += "Can't fit into program counter based or base register based addressing.";
            writeToFile(errorFile, writeData);
            objcode = IntToStringHex(stringHexToInt(OPTAB[GetOpCode(opcode)].opcode) + 3, 2);
            objcode += (halfBytes == 5) ? (IntToStringHex(xbpe + 1, 1) + "00") : IntToStringHex(xbpe, 1);
            objcode += "000";

            return objcode;
        }
    }
    return objcode;
}

void writeTextRecord(bool lastRecord = false)
{
    if (lastRecord)
    {
        if (currentRecord.length() > 0)
        { // Write last text record
            writeData = IntToStringHex(currentRecord.length() / 2, 2) + '^' + currentRecord;
            writeToFile(objectFile, writeData);
            currentRecord = "";
        }
        return;
    }
    if (objectCode != "")
    {
        if (currentRecord.length() == 0)
        {
            writeData = "T^" + IntToStringHex(address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
            writeToFile(objectFile, writeData, false);
        }
        // What is objectCode length > 60
        if ((currentRecord + objectCode).length() > 60)
        {
            // Write current record
            writeData = IntToStringHex(currentRecord.length() / 2, 2) + '^' + currentRecord;
            writeToFile(objectFile, writeData);

            // Initialize new text currentRecord
            currentRecord = "";
            writeData = "T^" + IntToStringHex(address + stringHexToInt(BLOCKS[BLocksNumToName[blockNumber]].startAddress), 6) + '^';
            writeToFile(objectFile, writeData, false);
        }

        currentRecord += objectCode;
    }
    else
    {
        /*Assembler directive which doesn't result in address genrenation*/
        if (opcode == "START" || opcode == "END" || opcode == "BASE" || opcode == "NOBASE" || opcode == "LTORG" || opcode == "ORG" || opcode == "EQU" /****/ || opcode == "EXTREF" || opcode == "EXTDEF" /******/)
        {
            /*DO nothing*/
        }
        else
        {
            // Write current record if exists
            if (currentRecord.length() > 0)
            {
       writeData = IntToStringHex(currentRecord.length() / 2, 2) + '^' + currentRecord;
       writeToFile(objectFile, writeData);
            }
            currentRecord = "";
        }
    }
}

//**************************************************************
void writeDRecord()
{
    write_D_Data = "D^";
    string temp_address = "";
    int length = operand.length();
    string inp = "";
    for (int i = 0; i < length; i++)
    {
        while (operand[i] != ',' && i < length)
        {
            inp += operand[i];
            i++;
        }
        temp_address = CSECT_TAB[currentSectName].EXTDEF_TAB[inp].address;
        write_D_Data += ExpandString(inp, 6, ' ', true) + temp_address;
        inp = "";
    }
    writeToFile(objectFile, write_D_Data);
}

void writeRRecord()
{
    write_R_Data = "R^";
    string temp_address = "";
    int length = operand.length();
    string inp = "";
    for (int i = 0; i < length; i++)
    {
        while (operand[i] != ',' && i < length)
        {
            inp += operand[i];
            i++;
        }
        write_R_Data += ExpandString(inp, 6, ' ', true);
        inp = "";
    }
    writeToFile(objectFile, write_R_Data);
}
//************************************************************

void writeEndRecord(bool write = true)
{
    if (write)
    {
        if (endRecord.length() > 0)
        {
            writeToFile(objectFile, endRecord);
        }
        else
        {
            writeEndRecord(false);
        }
    }
    if ((operand == "" || operand == " ") && currentSectName == "DEFAULT")
    { // If no operand of END
        endRecord = "E^" + IntToStringHex(startAddress, 6);
    }
    else if (currentSectName != "DEFAULT")
    {
        endRecord = "E";
    }
    else
    { // Make operand on end firstExecutableAddress
        int firstExecutableAddress;

        firstExecutableAddress = stringHexToInt(SYMTAB[firstExecutable_Sec].address);

        endRecord = "E^" + IntToStringHex(firstExecutableAddress, 6) + "\n";
    }
}

void pass2()
{
    string tempBuffer;
    intermediateFile.open("intermediate_" + fileName); // begin
    if (!intermediateFile)
    {
        cout << "Unable to open file: intermediate_" << fileName << endl;
        exit(1);
    }
    getline(intermediateFile, tempBuffer); // Discard heading line

    objectFile.open("object_" + fileName);
    if (!objectFile)
    {
        cout << "Unable to open file: object_" << fileName << endl;
        exit(1);
    }

    ListingFile.open("listing_" + fileName);
    if (!ListingFile)
    {
        cout << "Unable to open file: listing_" << fileName << endl;
        exit(1);
    }
    writeToFile(ListingFile, "Line\tAddress\tLabel\tOPCODE\tOPERAND\tObjectCode\tComment");

    errorFile.open("error_" + fileName, fstream::app);
    if (!errorFile)
    {
        cout << "Unable to open file: error_" << fileName << endl;
        exit(1);
    }
    writeToFile(errorFile, "\n\n************PASS2************");
    /*Intialize global variables*/
    objectCode = "";
    currentTextRecordLength = 0;
    currentRecord = "";
    modificationRecord = "";
    blockNumber = 0;
    nobase = true;

    readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
    while (isComment)
    { // Handle with previous comments
        writeData = to_string(lineNumber) + "\t" + comment;
        writeToFile(ListingFile, writeData);
        readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
    }

    if (opcode == "START")
    {
        startAddress = address;
        writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + to_string(blockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
        writeToFile(ListingFile, writeData);
    }
    else
    {
        label = "";
        startAddress = 0;
        address = 0;
    }
    //*************************************************************
    if (BLOCKS.size() > 1)
    {
        program_section_length = program_length;
    }
    else
    {
        program_section_length = CSECT_TAB[currentSectName].length;
    }
    //************************************************************************

    writeData = "H^" + ExpandString(label, 6, ' ', true) + '^' + IntToStringHex(address, 6) + '^' + IntToStringHex(program_section_length, 6);
    writeToFile(objectFile, writeData);

    readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment);
    currentTextRecordLength = 0;

    //******************************************************************************

    //*****************************************************************************

    while (opcode != "END")
    {
        while (opcode != "END" && opcode != "CSECT")
        {
            if (!isComment)
            {
       if (OPTAB[GetOpCode(opcode)].exists == 'y')
       {
           if (OPTAB[GetOpCode(opcode)].format == 1)
           {
               objectCode = OPTAB[GetOpCode(opcode)].opcode;
           }
           else if (OPTAB[GetOpCode(opcode)].format == 2)
           {
               operand1 = operand.substr(0, operand.find(','));
               operand2 = operand.substr(operand.find(',') + 1, operand.length() - operand.find(',') - 1);

               if (operand2 == operand)
               { // If not two operand i.e. a
                   if (GetOpCode(opcode) == "SVC")
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + IntToStringHex(stoi(operand1), 1) + '0';
                   }
                   else if (REGTAB[operand1].exists == 'y')
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + REGTAB[operand1].num + '0';
                   }
                   else
                   {
                       objectCode = GetOpCode(opcode) + '0' + '0';
                       writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                       writeToFile(errorFile, writeData);
                   }
               }
               else
               { // Two operands i.e. a,b
                   if (REGTAB[operand1].exists == 'n')
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + "00";
                       writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                       writeToFile(errorFile, writeData);
                   }
                   else if (GetOpCode(opcode) == "SHIFTR" || GetOpCode(opcode) == "SHIFTL")
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + REGTAB[operand1].num + IntToStringHex(stoi(operand2), 1);
                   }
                   else if (REGTAB[operand2].exists == 'n')
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + "00";
                       writeData = "Line: " + to_string(lineNumber) + " Invalid Register name";
                       writeToFile(errorFile, writeData);
                   }
                   else
                   {
                       objectCode = OPTAB[GetOpCode(opcode)].opcode + REGTAB[operand1].num + REGTAB[operand2].num;
                   }
               }
           }
           else if (OPTAB[GetOpCode(opcode)].format == 3)
           {
               if (GetOpCode(opcode) == "RSUB")
               {
                   objectCode = OPTAB[GetOpCode(opcode)].opcode;
                   objectCode += (getFlagFormat(opcode) == '+') ? "000000" : "0000";
               }
               else
               {
                   objectCode = createObjectCodeFormat34();
               }
           }
       } // If opcode in optab
       else if (opcode == "BYTE")
       {
           if (operand[0] == 'X')
           {
               objectCode = operand.substr(2, operand.length() - 3);
           }
           else if (operand[0] == 'C')
           {
               objectCode = stringToHexString(operand.substr(2, operand.length() - 3));
           }
       }
       else if (label == "*")
       {
           if (opcode[1] == 'C')
           {
               objectCode = stringToHexString(opcode.substr(3, opcode.length() - 4));
           }
           else if (opcode[1] == 'X')
           {
               objectCode = opcode.substr(3, opcode.length() - 4);
           }
       }
       else if (opcode == "WORD")
       {
           objectCode = IntToStringHex(stoi(operand), 6);
       }
       else if (opcode == "BASE")
       {
           if (SYMTAB[operand].exists == 'y')
           {
               base_register_value = stringHexToInt(SYMTAB[operand].address) + stringHexToInt(BLOCKS[BLocksNumToName[SYMTAB[operand].blockNumber]].startAddress);
               nobase = false;
           }
           else
           {
               writeData = "Line " + to_string(lineNumber) + " : Symbol doesn't exists. Found " + operand;
               writeToFile(errorFile, writeData);
           }
           objectCode = "";
       }
       else if (opcode == "NOBASE")
       {
           if (nobase)
           { // check if assembler was using base addressing
               writeData = "Line " + to_string(lineNumber) + ": Assembler wasn't using base addressing";
               writeToFile(errorFile, writeData);
           }
           else
           {
               nobase = true;
           }
           objectCode = "";
       }
       else
       {
           objectCode = "";
       }
       // Write to text record if any generated
       writeTextRecord();

       if (blockNumber == -1 && address != -1)
       {
           writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
       }
       else if (address == -1)
       {
           if (opcode == "EXTDEF")
           {
               writeDRecord();
           }
           else if (opcode == "EXTREF")
           {
               writeRRecord();
           }
           writeData = to_string(lineNumber) + "\t" + " " + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
       }

       else
       {
           writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + to_string(blockNumber) + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
       }
            } // if not comment
            else
            {
       writeData = to_string(lineNumber) + "\t" + comment;
            }
            writeToFile(ListingFile, writeData);                                                                                  // Write listing line
            readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment); // Read next line
            objectCode = "";
        } // while opcode not end
        writeTextRecord();
        writeEndRecord(false);

        if (opcode == "CSECT" && !isComment)
        {
            writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + to_string(blockNumber) + "\t" + label + "\t" + opcode + "\t" + " " + "\t" + objectCode + "\t" + " ";
        }
        else if (!isComment)
        {
            writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + " " + "\t" + label + "\t" + opcode + "\t" + operand + "\t" + "" + "\t" + comment;
        }
        else
        {
            writeData = to_string(lineNumber) + "\t" + comment;
        }
        writeToFile(ListingFile, writeData);

        if (opcode != "CSECT")
        {
            while (readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment))
            {
       if (label == "*")
       {
           if (opcode[1] == 'C')
           {
               objectCode = stringToHexString(opcode.substr(3, opcode.length() - 4));
           }
           else if (opcode[1] == 'X')
           {
               objectCode = opcode.substr(3, opcode.length() - 4);
           }
           writeTextRecord();
       }
       writeData = to_string(lineNumber) + "\t" + IntToStringHex(address) + "\t" + to_string(blockNumber) + label + "\t" + opcode + "\t" + operand + "\t" + objectCode + "\t" + comment;
       writeToFile(ListingFile, writeData);
            }
        }

        writeTextRecord(true);
        if (!isComment)
        {

            writeToFile(objectFile, modificationRecord, false); // Write modification record
            writeEndRecord(true);                               // Write end record
            currentSectName = label;
            modificationRecord = "";
        }
        if (!isComment && opcode != "END")
        {
            writeData = "\n********************object program for " + label + " **************";
            writeToFile(objectFile, writeData);

            writeData = "\nH^" + ExpandString(label, 6, ' ', true) + '^' + IntToStringHex(address, 6) + '^' + IntToStringHex(CSECT_TAB[label].length, 6);
            writeToFile(objectFile, writeData);
        }
        readIntermediateFile(intermediateFile, isComment, lineNumber, address, blockNumber, label, opcode, operand, comment); // Read next line
        objectCode = "";
    }
} // Function end

int main()
{
    cout << "****Input file and executable(assembler.out) should be in same folder****" << endl
         << endl;
    cout << "Enter name of input file:";
    cin >> fileName;

    cout << "\nLoading OPTAB" << endl;
    load_tables();

    cout << "\nPerforming PASS1" << endl;
    cout << "Writing intermediate file to 'intermediate_" << fileName << "'" << endl;
    cout << "Writing error file to 'error_" << fileName << "'" << endl;
    pass1();

    cout << "Writing SYMBOL TABLE" << endl;
    printtab.open("tables_" + fileName);
    writeToFile(printtab, "**********************************SYMBOL TABLE*****************************\n");
    for (auto const &it : SYMTAB)
    {
        writestring += it.first + ":-\t" + "name:" + it.second.name + "\t|" + "address:" + it.second.address + "\t|" + "relative:" + IntToStringHex(it.second.relative) + " \n";
    }
    writeToFile(printtab, writestring);

    writestring = "";
    cout << "Writing LITERAL TABLE" << endl;

    writeToFile(printtab, "**********************************LITERAL TABLE*****************************\n");
    for (auto const &it : LITTAB)
    {
        writestring += it.first + ":-\t" + "value:" + it.second.value + "\t|" + "address:" + it.second.address + " \n";
    }
    writeToFile(printtab, writestring);

    writestring = "";
    cout << "Writing EXTREF TABLE" << endl;

    writeToFile(printtab, "**********************************EXTREF TABLE*****************************\n");
    for (auto const &it0 : CSECT_TAB)
    {
        for (auto const &it : it0.second.EXTREF_TAB)
            writestring += it.first + ":-\t" + "name:" + it.second.name + "\t|" + it0.second.name + " \n";
    }
    writeToFile(printtab, writestring);

    writestring = "";
    cout << "Writing EXTDEF TABLE" << endl;

    writeToFile(printtab, "**********************************EXTDEF TABLE*****************************\n");
    for (auto const &it0 : CSECT_TAB)
    {
        for (auto const &it : it0.second.EXTDEF_TAB)
        {
            if (it.second.name != "undefined")
       writestring += it.first + ":-\t" + "name:" + it.second.name + "\t|" + "address:" + it.second.address + "\t|" + " \n";
        }
    }

    writeToFile(printtab, writestring);

    cout << "\nPerforming PASS2" << endl;
    cout << "Writing object file to 'object_" << fileName << "'" << endl;
    cout << "Writing listing file to 'listing_" << fileName << "'" << endl;
    pass2();
}
