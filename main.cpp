#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstring>

class RideReport {
    private:
        int speedCount;
        int slowDownCount;
        int turnRightCount;
        int turnLeftCount;
        char *fileName;
        char *fileData;
        size_t sizeOfFile;

    public:
        void wyswietlRaport();
        bool readData();
        bool processData();
        int getSpeedCount();
        int getSlowDownCount();
        int getLeftTurnCount();
        int getRightTurnCount();
        bool saveCleanedData(const char *fileName);
        bool saveAsText(const char *fileName);
        bool saveAsXML(const char *fileName);
        bool validate(char a);
        int getFileSize();
        RideReport(const char *fileName);
        ~RideReport();
};

RideReport::RideReport(const char *fileName){
    size_t sizeOfText;
    sizeOfText = strlen(fileName)+1;
    this->fileName = new char[sizeOfText];
    strcpy_s(this->fileName, sizeOfText, fileName);
    this->speedCount = 0;
    this->slowDownCount = 0;
    this->turnRightCount = 0;
    this->turnLeftCount = 0;
}

RideReport::~RideReport(){
    delete[] fileName;
    delete[] fileData;
}

int RideReport::getFileSize(){
    std::string line;
    std::ifstream inMyStream(this->fileName);
    this->sizeOfFile = 0;
    while(getline(inMyStream, line)){
        this->sizeOfFile+=line.length();
    }
    inMyStream.close();
    return this->sizeOfFile;
}

bool RideReport::readData(){
    std::fstream plik;
    std::string data;
    int a = 0;
    plik.open(this->fileName, std::ios::in);
    if(plik.is_open()){
        this->fileData = new char[getFileSize()];
        while(!plik.eof()) {
            plik >> this->fileData[a];
            std::cout << this->fileData[a];
            ++a;
        }
        plik.close();
        return true;
    } else {
        this->turnLeftCount = -1;
        this->turnRightCount = -1;
        this->slowDownCount = -1;
        this->speedCount = -1;
        std::cout << "Nie mozna otworzyc pliku";
        return false;
    }
}

bool RideReport::processData(){
    for(int i = 0; i < this->sizeOfFile; i++){
        if(this->fileData[i]=='a' || this->fileData[i]=='A') this->speedCount++;
        else if(this->fileData[i]=='b' || this->fileData[i]=='B') this->slowDownCount++;
        else if(this->fileData[i]=='l' || this->fileData[i]=='L') this->turnLeftCount++;
        else if(this->fileData[i]=='r' || this->fileData[i]=='R') this->turnRightCount++;
    }
    return true;
}
bool RideReport::validate(char a){
    if(a=='a'||a=='A'||a=='b'||a=='B'||a=='l'||a=='L'||a=='r'||a=='R') return true;
    else return false;
}

bool RideReport::saveCleanedData (const char *fileName){
	int a = 0;
	std::string nd[getFileSize()];
	std::string data;
	std::fstream plik;
	plik.open(this->fileName, std::ios::in);
	while( !plik.eof()) {
		getline(plik,data);
		nd[a] = data;
		++a;
		}
		plik.close();
		plik.open(fileName, std::ios::out );
		for( int i = 0; i < a; ++i ){
			for( int d = 0; d < nd[ i ].size(); ++d){
			    if(validate(char(nd[i][d]))) plik << char(nd[i][d]);
			}
			plik << std::endl;
		}
		plik.close();
    return true;
}

bool RideReport::saveAsText(const char *fileName){
	std::fstream plik;
	plik.open(fileName, std::ios::out );
    time_t now = time(0);
    tm *ltm = localtime(&now);
	plik << "Raport dla pliku: " << this->fileName <<std::endl;
    plik << "Data utworzenia: " << ltm->tm_year+1900 << "-" << ltm->tm_mon+1 << "-" << ltm->tm_mday << std::endl;
    plik << "Czas utworzenia: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec <<std::endl;
    plik << "Przspieszenia: " << this->speedCount <<std::endl;
    plik << "Hamowania: " << this->slowDownCount <<std::endl;
    plik << "Lewo: " << this->turnLeftCount <<std::endl;
    plik << "Prawo: " << this->turnRightCount;
	plik.close();
    return true;
}

bool RideReport::saveAsXML(const char *fileName){
	std::fstream plik;
	plik.open(fileName, std::ios::out );
    time_t now = time(0);
    tm *ltm = localtime(&now);
	plik << "<?xml version\"=1.0\"ecnoding=\"utf-8\"?>" << std::endl;
    plik << "<report file=\"" << this->fileName << "\" date=\"" << ltm->tm_year+1999 <<"-"<<ltm->tm_mon+1<<"-"<<ltm->tm_mday<<"\"" << std::endl
    << "time=\"" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\">" <<std::endl
    << "<speed-up-count>" << this->speedCount << "</speed-up-count>" << std::endl
    << "<slow-down-count>" << this->slowDownCount << "</slow-down-count>" << std::endl
    << "<left-turn-count>" << this->turnLeftCount << "</left-turn-count>" << std::endl
    << "<right-turn-count>" << this->turnRightCount << "</right-turn-count>" << std::endl
    << "</report>";
	plik.close();
    return true;
}

int RideReport::getSpeedCount(){
    return this->speedCount;
}

int RideReport::getSlowDownCount(){
    return this->slowDownCount;
}

int RideReport::getRightTurnCount(){
    return this->turnRightCount;
}

int RideReport::getLeftTurnCount(){
    return this->turnLeftCount;
}


int main() {
    RideReport report("dane.txt");
    if(report.readData()){
        if(report.processData()){
            std::cout << "\nPrzyspieszenia: " << report.getSpeedCount();
            std::cout << "\nHamowania: " << report.getSlowDownCount();
            std::cout << "\nW lewo: " << report.getLeftTurnCount();
            std::cout << "\nW prawo: " << report.getRightTurnCount();
            if(report.saveCleanedData("dane_czyste.txt"))
                std::cout << "\nOczyszczono dane zapisane";
            if(report.saveAsText("report.txt"))
                std::cout << "\nRaport tekstowy zapisany";
            if(report.saveAsXML("report.xml"))
                std::cout << "\nRaport XML zapisany";
        }
    }
    return EXIT_SUCCESS;
}