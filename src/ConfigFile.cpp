#include "../include/ConfigFile.h"
#include "../src/ConfigFilePrepare.cpp"

ConfigFile::ConfigFile(string fileName, vector<ConfigFile::ConfigInputLine> cinput) {

    for (auto const &item : cinput) {
        this->backbone[item.name] = backboneItem{item.type, item.defaultVal, item.required, false};
    }

    this->cfile.open(fileName.c_str());
    if (cfile.is_open()) {
        string line, name, value, valueNoWS;
        size_t delimiter;
        unsigned lineNum = 0;

        //Scaning file
        //while(!cfile.eof()){
        while (getline(cfile, line)) {
            //getline(cfile, line);
            lineNum++;
            line = this->whiteCharsEraser(line);
            //Empty or comment line
            if (line[0] == '#' || line.empty()) continue;

            //Checking exist delimiter
            delimiter = line.find('=');
            if (delimiter == string::npos) {
                throw Exception("Error in line " + to_string(lineNum) + " can't find delimiter\n" + line);
            }

            //Save to vars
            name = line.substr(0, delimiter);
            value = line.substr(delimiter + 1);

            //Delete white char from end
            if(isspace(value[value.size() - 1])){
                value.pop_back();
            }

            transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            //Checking correction
            if (value.size() == 0)
                throw Exception("Error in line " + to_string(lineNum) + " no assigned value\n" + line);

            if (name.size() == 0)
                throw Exception("Error in line " + to_string(lineNum) + " parameter no name\n" + line);

            //Checking value is string
            valueNoWS = value;
            valueNoWS.erase(
                    remove_if(valueNoWS.begin(), valueNoWS.end(), [](unsigned char x) { return std::isspace(x); }),
                    valueNoWS.end());
            if (!this->isString(valueNoWS))
                value = valueNoWS;

            //Checking if is defined
            if (this->backbone.count(name) > 0) {
                //Checking type is correct
                if (!this->checkType(value, backbone[name].type)) {
                    throw Exception("Error in line " + to_string(lineNum) + " bad type\n" + line);
                }
            }

            //saving
            this->backbone[name].exist = true;
            this->cdata[name] = value;
        }

        cfile.close();

        //Checking all required value is passed and pass default
        /*map<string, backboneItem>::iterator = iter;
        for(iter = this->backbone.begin(); iter != this->backbone.end(); i++){
        }
        */
        for (auto const &mitem: this->backbone) {
            string key = mitem.first;
            backboneItem value = mitem.second;
            if (this->cdata.count(key) == 0) {
                if (value.required) {
                    string err = "Error can't find requried key: ";
                    throw Exception(err + key);
                } else {
                    this->cdata[key] = value.defaultVal;
                }
            }
        }
    } else {
        throw Exception("Can't open config file");
    }
}

ConfigFile::~ConfigFile() {
    //dtor
}

string ConfigFile::whiteCharsEraser(string txt) {
    short mode = 0;
    string out = "";
    char ch;
    int size = txt.size();
    for (int i = 0; i < size; i++) {
        ch = txt[i];
        if (mode == 0) {
            if (!isspace(ch)) {
                out += ch;
                if (ch == '=') mode++;
            }
        } else if (mode == 1) {
            if (!isspace(ch)) {
                out += ch;
                mode++;
            }
        } else {
            out += ch;
        }
    }
    return out;
}

bool ConfigFile::checkType(string val, unsigned short type) {
    //switch(type){
    /*case 0 :
        val = transform(val.begin(), val.end(),  val.begin(), ::tolower);
        if(val == 'true' || 'false' || '0' || '1')
            return true;
        else
            return false;
        break;
    case 1 :
        int num = 0;
        num = atoi(val);
        if(num == 0 && val[0] != '0') return false;
        else return true;
        break;
    case 2:


    }*/
    if (CFILE_BOOL == type) {
        transform(val.begin(), val.end(), val.begin(), [](unsigned char c) { return std::tolower(c); });
        if (val == "true" || val == "false" || val == "0" || val == "1")
            return true;
        else
            return false;
    } else if (CFILE_INT == type) {
        if (this->isNumber<int>(val))
            return true;
        else
            return false;
    } else if (CFILE_DOUBLE == type) {
        if (this->isNumber<double>(val))
            return true;
        else
            return false;
    } else
        return true;

}

template<typename type>
bool ConfigFile::isNumber(string txt) {
    type testType;
    istringstream teststream(txt);
    teststream >> testType;
    return (!teststream.fail());
}

bool ConfigFile::isString(string txt) {
    if (this->checkType(txt, CFILE_INT) || this->checkType(txt, CFILE_DOUBLE) || this->checkType(txt, CFILE_BOOL))
        return false;
    else
        return true;
}

int ConfigFile::getIntValue(string key) {
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    return this->getValue<int>(key);
}

double ConfigFile::getDoubleValue(string key) {
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    return this->getValue<double>(key);
}

string ConfigFile::getStringValue(string key) {
    transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return std::tolower(c); });
    return this->cdata[key];
}

template<typename type>
type ConfigFile::getValue(string key) {
    transform(key.begin(), key.end(),  key.begin(), [](unsigned char c){ return std::tolower(c); });
    if (this->cdata.count(key) == 0) {
        throw Exception("Error can't find value assigned to key: " + key);
    } else {
        type out;
        istringstream iss(this->cdata[key]);
        iss >> noskipws >> out;
        if (iss.fail()) {
            throw Exception("Error value assigned to key " + key + " is not " + typeid(type).name());
        }
        return out;
    }
}

/*
template <>
string ConfigFile::getValue<string>(string key){
    return this->cdata[key];
}
*/
/*string ConfigFile::getValue(string key) {
    return this->cdata[key];
}*/

template bool ConfigFile::getValue<bool>(string key);
template int ConfigFile::getValue<int>(string key);
template double ConfigFile::getValue<double>(string key);
template string ConfigFile::getValue<string>(string key);
