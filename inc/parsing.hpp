#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#define mp make_pair
using namespace std;

class CellStat{
    public:
        CellStat(){}
        int numOfTech;
        int sizeX[2], sizeY[2], pincnt[2];
        vector<pair<int, int> > pinLoc[2];
        //set size and pincnt, tech is 0 or 1
        void setStat(int x, int y, int cnt, int tech){
            sizeX[tech] = x; sizeY[tech] = y; pincnt[tech] = cnt;
            pinLoc[tech].resize(cnt+1);
        }
        void setPin(int idx, int x, int y, int tech){
            pinLoc[tech][idx] = mp(x, y);
        }
        void printCell(){
            for(int i = 0; i < numOfTech; i++){
                cout << "Tech: " << i+1 << endl;
                cout << sizeX[i] << " " << sizeY[i] << " " << pincnt[i] << endl;
                
                // for(auto &it : pinLoc[i]){
                //     cout << it.first << " " << it.second << endl;
                // }
                for (int j=0; j<pinLoc[i].size(); j++)
                    cout << pinLoc[i][j].first << pinLoc[i][j].second << endl;
            }
        }
};

struct ROW{
    ROW(){}
    ROW(int X, int Y, int l, int h, int cnt):
        startX(X), startY(Y), rowLen(l), rowHeight(h), repCnt(cnt) {}
        // startX{X}, startY{Y}, rowLen{l}, rowHeight{h}, repCnt{cnt}{}
    int startX, startY, rowLen, rowHeight, repCnt;
};
struct cellLocationInfo{
    cellLocationInfo(){}
    cellLocationInfo(int llx, int lly, int urx, int ury)
    {
        this->llx = llx; this->lly = lly; this->urx = urx; this->ury = ury;
        this->centX = double(this->llx + this->urx) / 2;
        this->centY = double(this->lly + this->ury) / 2;
    }
    int llx, lly, urx, ury;
    double centX, centY;
};
struct boundingBox
{
    boundingBox(){}
    boundingBox(int lx, int ly, int hx, int hy)
    {
        this->lx = lx; this->ly = ly;
        this->hx = hx; this->hy = hy;
    }
    int lx, ly, hx, hy;
};
struct terLocationInfo{
    terLocationInfo(){}
    terLocationInfo(int llx, int lly, int urx, int ury)
    {
        this->llx = llx; this->lly = lly; this->urx = urx; this->ury = ury;
        this->centX = double(this->llx + this->urx) / 2;
        this->centY = double(this->lly + this->ury) / 2;
    }
    int llx, lly, urx, ury;
    double centX, centY;
    boundingBox bbx;
};
class Config {
public:
    int numNodeLeft = 200;   // number of node left in the last layer of coarsening
    int numInstances = 20;  // number of initial partition done in "initial partition" phase
    double dropRate = 0.3;    // drop partition this rate worse than the best partition in each uncoarsening layer
    int maxCoarsenIter = 30;   // maximum number of coarening layer (stop coarsening even if the number of node left is larger than "numNodeLeft")
    int maxRefineIter = 30;    // maximum number of coarening layer in refinement (stop coarsening even if the number of node left is larger than "numNodeLeft")
    string coarsenScheme = "MHEC";    // coarsening scheme when coarsening (HEC/MHEC)
    string coarsenScheme_r = "MHEC"; // coarsening scheme when doing refinement (HEC/MHEC)
    string initialParScheme = "FM";
    string uncoarsenScheme = "FM";
    string uncoarsenScheme_r = "FM";
};
class BOX{
    public:
        BOX(){}
        //index start from 1, layer[instName] = (0->top or 1->bottom)
        vector<int> layer;
        //index start from 1, instLib[instName] = libCellName
        int numOfInst;
        vector<int> instLib;
        //index start from 1, net[netName] = vector of pins, net[netName][idx <-start from 0] = (instName, libPinNum)
        int numOfNet;
        int numOfPins;
        vector<vector<pair<int, int> > > net;
        int diellX, diellY, dieurX, dieurY;
        int TopDieMaxUtil, BottomDieMaxUtil;
        //0->TopDieRows; 1->BottomDieRows
        ROW row[2];

        //index start from 1, if you want to get sizeX of MCx in tech A: use cellLib[x].sizeX[0]
        //sizeX of MCx in tech B: use cellLib[x].sizeX[1]
        //pin x of MCy in tech B: use cellLib[y].pinLoc[1][x]
        vector<CellStat> cellLib;
        //0->TopDieTech; 1->BottomDieTech
        int boardTech[2];
        int terminalSizeX, terminalSizeY, terminalSpace;
        int numOfTerminal;
        map<int, int> terminalMP; // (X, Y) : terminal TX in Net NY
        vector<bool> netHasTerminals;
        int numOfTopCell, numOfBotCell;
        
        // for output
        vector<cellLocationInfo> cellBound;
        vector<terLocationInfo> terBound;

        //the output order is the same as document's format of input data in page 4
        void printContent(){
            // for(auto &it : instLib){
            //     cout << it << " ";
            // }
            for (int i=0; i<instLib.size(); i++)
                cout << instLib[i] << " ";
            cout << endl;
            cout << "NET: " << endl;
            int cnt = 0;

            // for(auto &it : net){
            //     cout << "Net " << cnt++ << ":" << endl;
            //     for(auto &it2 : it){
            //         cout << it2.first << " " << it2.second << endl;
            //     }
            //     cout << endl;
            // }
            for(int i=0; i<net.size(); i++){
                cout << "Net " << cnt++ << ":" << endl;
                for(int j=0; j<net[i].size(); j++){
                    cout << net[i][j].first << " " << net[i][j].second << endl;
                }
                cout << endl;
            }

            cout << endl;
            cout << "die:" << endl;
            cout << "llx: " << diellX << " lly: " << diellY << " urx: "  << dieurX << " ury: " << dieurY << endl;
            cout << "TopDieMaxUtil: " << TopDieMaxUtil << " BottomDieMaxUtil:" << BottomDieMaxUtil << endl;
            cout << "Top Die:" << endl;
            cout << "starX: " << row[0].startX << " startY: " << row[0].startY << " rowLen: " << row[0].rowLen << " rowHeight:" << row[0].rowHeight << " repCnt: " << row[0].repCnt << endl;
            cout << "Bottom Die:" << endl;
            cout << "starX: " << row[1].startX << " startY: " << row[1].startY << " rowLen: " << row[1].rowLen << " rowHeight:" << row[1].rowHeight << " repCnt: " << row[1].repCnt << endl;
            cout << "Cell Lib: ";

            // for(auto &it : cellLib){
            //     it.printCell();
            //     cout << endl;
            // }
            for (int i=0; i<cellLib.size(); i++) {
                cellLib[i].printCell();
                cout << endl;
            }

            cout << "Top Die boardTech: "<< boardTech[0] << " Bottom Die boardTech: " << boardTech[1] << endl;
            cout << "terminalSizeX: " << terminalSizeX << " terminalSizeY: " << terminalSizeY << " terminalSpace: " << terminalSpace << endl;
        }
};

static bool isInt(char* c)
{
    for (int i=0; c[i]; i++)
    {
        if (!isdigit(c[i]))
            return false;
    }
    return true;
}

static bool parsing(int argc, char* argv[], Config& cf, BOX &box){
    const struct option opts[] = {
        {"nleft", 1, NULL, 'n'},
        {"ileft", 1, NULL, 'i'},
        {"dr", 1, NULL, 'r'},
        {"max-itr-c", 1, NULL, 'I'},
        {"max-itr-r", 1, NULL, 'R'},
        {"coarsen-scheme", 1, NULL, 'A'},
        {"refine-scheme", 1, NULL, 'B'},
        {"IP-scheme", 1, NULL, 'C'},
        {"UC-scheme", 1, NULL, 'D'},
        {"UC-scheme-r", 1, NULL, 'E'}
    };
    const char *optstring = "n:i:r:I:R:A:B:C:D:E";
    int c;

    while ((c = getopt_long(argc, argv, optstring, opts, NULL)) != -1)
    {
        switch(c)
        {
            case 'n':
                if (!isInt(optarg))
                {
                    printf ("--nleft: Please enter a positive integer\n");
                    return false;
                }
                cf.numNodeLeft = atoi(optarg);
                break;
            case 'i':
                if (!isInt(optarg))
                {
                    printf ("--ileft: Please enter a positive integer\n");
                    return false;
                }
                cf.numInstances = atoi(optarg);
                break;
            case 'r':
            {
                double value = std::atof(optarg);
                if (value == 0.0 && optarg != "0")
                {
                    printf("Drop rate (--dr) invalid.\n");
                    return false;
                }
                else if (value > 1 || value < 0)
                {
                    printf("Drop rate (--dr) must be a value between [0,1].\n");
                    return false;
                }
                else
                    cf.dropRate = value;
                break;
            }
            case 'I':
                if (!isInt(optarg))
                {
                    printf ("--max-itr-c: Please enter a positive integer\n");
                    return false;
                }
                cf.maxCoarsenIter = atoi(optarg);
                break;
            case 'R':
                if (!isInt(optarg))
                {
                    printf ("--max-itr-r: Please enter a positive integer\n");
                    return false;
                }
                cf.maxRefineIter = atoi(optarg);
                break;
            case 'A':
                if (strncmp(optarg, "EC", 2) == 0 || strncmp(optarg, "HEC", 3) == 0 || strncmp(optarg, "MHEC", 3) == 0)
                    cf.coarsenScheme = (string)optarg;
                else
                {
                    printf("Coarsen scheme (--coarsen-scheme) invalid, please use EC/HEC/MHEC.\n");
                    return false;
                }
                break;
            case 'B':
                if (strncmp(optarg, "EC", 2) == 0 || strncmp(optarg, "HEC", 3) == 0 || strncmp(optarg, "MHEC", 3) == 0)
                    cf.coarsenScheme_r = (string)optarg;
                else
                {
                    printf("Coarsen scheme in refinement phase (--refine-scheme) invalid, please use EC/HEC/MHEC.\n");
                    return false;
                }
                break;
            case 'C':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.initialParScheme = (string)optarg;
                else
                {
                    printf("Initial partition scheme (--IP-scheme) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case 'D':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.uncoarsenScheme = (string)optarg;
                else
                {
                    printf("Uncoarsen scheme (--UC-scheme) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case 'E':
                if (strncmp(optarg, "FM", 2) == 0 || strncmp(optarg, "HER", 3) == 0)
                    cf.uncoarsenScheme_r = (string)optarg;
                else
                {
                    printf("Uncoarsen scheme in refinement phase (--UC-scheme-r) invalid, please use FM/HER.\n");
                    return false;
                }
                break;
            case '?':
                printf("Unknown option %s\n", optarg);
                break;
            default:
                printf("------------------------------------\n");
        }
    }

    ifstream inputf(argv[optind]);
    if (!inputf.is_open())
    {
        printf("Cannot open input file...\n");
        return false;
    }
    
    string k = argv[optind];
    string line;
    stringstream ss;
    while(getline(inputf, line)){
        ss.str("");
        ss.clear();
        ss.str(line);
        string tok;
        getline(ss, tok, ' ');
        int numOfTech, numOfCell;
        //NumInstances <instanceCount>
        if(tok == "NumInstances"){
            getline(ss, tok, ' ');
            // box.numOfInst = stoi(tok);
            std::istringstream (tok) >> box.numOfInst;
            box.instLib.resize(box.numOfInst+1);
            box.layer.resize(box.numOfInst+1);
            //Inst <instName> <libCellName>
            for(int i = 0; i < box.numOfInst; i++){
                int a, b;
                getline(inputf, line);
                ss.str("");
                ss.clear();
                ss.str(line);
                getline(ss, tok, ' ');
                getline(ss, tok, ' ');
                tok.erase(0,1);//remove C
                // a = stoi(tok);
                std::istringstream (tok) >> a;

                getline(ss, tok, ' ');
                tok.erase(0,2);//remove MC
                // b = stoi(tok);
                std::istringstream (tok) >> b;

                box.instLib[a] = b;
            }
        }
        //NumNets <netCount>
        else if(tok == "NumNets"){
            getline(ss, tok, ' ');
            // box.numOfNet = stoi(tok);
            std::istringstream (tok) >> box.numOfNet;

            box.net.resize(box.numOfNet+1);
            //Net <netName> <numOfPins>
            for(int i = 0; i < box.numOfNet; i++){
                getline(inputf, line);
                ss.str("");
                ss.clear();
                ss.str(line);
                getline(ss, tok, ' ');
                getline(ss, tok, ' ');
                tok.erase(0,1);//remove N
                // int netIdx = stoi(tok);
                int netIdx;
                std::istringstream (tok) >> netIdx;
                
                getline(ss, tok, ' ');
                // int numOfPin = stoi(tok);
                int numOfPin;
                std::istringstream (tok) >> numOfPin;

                box.numOfPins += numOfPin;
                //Pin <instName>/<libPinName>
                for(int j = 0; j < numOfPin; j++){
                    int a, b;
                    getline(inputf, line);
                    ss.str("");
                    ss.clear();
                    ss.str(line);
                    getline(ss, tok, ' ');
                    getline(ss, tok, '/');
                    tok.erase(0,1);//remove C
                    // a = stoi(tok);
                    std::istringstream (tok) >> a;

                    getline(ss, tok, ' ');
                    tok.erase(0,1);//remove P
                    // b = stoi(tok);
                    std::istringstream (tok) >> b;

                    box.net[netIdx].push_back(mp(a, b));
                }
            }
        }
        //DieSize <lowerLeftX> <lowerLeftY> <upperRightX> <upperRightY>
        else if(tok == "DieSize"){
            getline(ss, tok, ' ');
            // box.diellX = stoi(tok);
            std::istringstream (tok) >> box.diellX;

            getline(ss, tok, ' ');
            // box.diellY = stoi(tok);
            std::istringstream (tok) >> box.diellY;

            getline(ss, tok, ' ');
            // box.dieurX = stoi(tok);
            std::istringstream (tok) >> box.dieurX;

            getline(ss, tok, ' ');
            // box.dieurY = stoi(tok);
            std::istringstream (tok) >> box.dieurY;
        }
        //TopDieMaxUtil <util>
        else if(tok == "TopDieMaxUtil"){
            getline(ss, tok, ' ');
            // box.TopDieMaxUtil = stoi(tok);
            std::istringstream (tok) >> box.TopDieMaxUtil;
        }
        //BottomDieMaxUtil <util>
        else if(tok == "BottomDieMaxUtil"){
            getline(ss, tok, ' ');
            // box.BottomDieMaxUtil = stoi(tok);
            std::istringstream (tok) >> box.BottomDieMaxUtil;
        }
        //TopDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>
        else if(tok == "TopDieRows"){
            int a, b, c, d, e;
            getline(ss, tok, ' ');
            // a = stoi(tok);
            std::istringstream (tok) >> a;

            getline(ss, tok, ' ');
            // b = stoi(tok);
            std::istringstream (tok) >> b;

            getline(ss, tok, ' ');
            // c = stoi(tok);
            std::istringstream (tok) >> c;

            getline(ss, tok, ' ');
            // d = stoi(tok);
            std::istringstream (tok) >> d;

            getline(ss, tok, ' ');
            // e = stoi(tok);
            std::istringstream (tok) >> e;

            box.row[0] = ROW(a, b, c, d, e);
        }

        //BottomDieRows <startX> <startY> <rowLength> <rowHeight> <repeatCount>
        else if(tok == "BottomDieRows"){
            int a, b, c, d, e;
            getline(ss, tok, ' ');
            // a = stoi(tok);
            std::istringstream (tok) >> a;

            getline(ss, tok, ' ');
            // b = stoi(tok);
            std::istringstream (tok) >> b;

            getline(ss, tok, ' ');
            // c = stoi(tok);
            std::istringstream (tok) >> c;

            getline(ss, tok, ' ');
            // d = stoi(tok);
            std::istringstream (tok) >> d;

            getline(ss, tok, ' ');
            // e = stoi(tok);
            std::istringstream (tok) >> e;
            box.row[1] = ROW(a, b, c, d, e);
        }

        //NumTechnologies <technologyCount>
        else if(tok == "NumTechnologies"){
            getline(ss, tok, ' ');
            // numOfTech = stoi(tok);
            std::istringstream (tok) >> numOfTech;

            //for(int i = 0; i < numOfTech; i++){
            //first tech
            //Tech <techName> <libCellCount>
            getline(inputf, line);
            ss.str("");
            ss.clear();
            ss.str(line);
            getline(ss, tok, ' ');
            getline(ss, tok, ' ');
            getline(ss, tok, ' ');
            // numOfCell = stoi(tok);
            std::istringstream (tok) >> numOfCell;

            box.cellLib.resize(numOfCell+1);
            //LibCell <libCellName> <libCellSizeX> <libCellSizeY> <pinCount>
            for(int i = 0; i < numOfCell; i++){
                getline(inputf, line);
                ss.str("");
                ss.clear();
                ss.str(line);
                getline(ss, tok, ' ');
                getline(ss, tok, ' ');
                tok.erase(0,2);//remove MC
                // int cellIdx = stoi(tok);
                int cellIdx;
                std::istringstream (tok) >> cellIdx;

                int sizeX, sizeY, pinCnt;
                CellStat newCell = CellStat();
                newCell.numOfTech = numOfTech;
                getline(ss, tok, ' ');
                // sizeX = stoi(tok);
                std::istringstream (tok) >> sizeX;

                getline(ss, tok, ' ');
                // sizeY = stoi(tok);
                std::istringstream (tok) >> sizeY;

                getline(ss, tok, ' ');
                // pinCnt = stoi(tok);
                std::istringstream (tok) >> pinCnt;

                newCell.setStat(sizeX, sizeY, pinCnt, 0);
                if(numOfTech == 1) newCell.setStat(sizeX, sizeY, pinCnt, 1);
                //Pin <pinName> <pinLocationX> <pinLocationY>
                for(int j = 0; j < pinCnt; j++){
                    getline(inputf, line);
                    ss.str("");
                    ss.clear();
                    ss.str(line);
                    getline(ss, tok, ' ');
                    getline(ss, tok, ' ');
                    tok.erase(0,1);//remove P
                    // int pinIdx = stoi(tok);
                    int pinIdx;
                    std::istringstream (tok) >> pinIdx;

                    int pinX, pinY;
                    getline(ss, tok, ' ');
                    // pinX = stoi(tok);
                    std::istringstream (tok) >> pinX;

                    getline(ss, tok, ' ');
                    // pinY = stoi(tok);
                    std::istringstream (tok) >> pinY;

                    newCell.setPin(pinIdx, pinX, pinY, 0);
                    if(numOfTech == 1) newCell.setPin(pinIdx, pinX, pinY, 1);
                }
                box.cellLib[cellIdx] = newCell;
            }
            //second tech
            if(numOfTech == 2){
                //Tech <techName> <libCellCount>
                getline(inputf, line);
                //LibCell <libCellName> <libCellSizeX> <libCellSizeY> <pinCount>
                for(int i = 0; i < numOfCell; i++){
                    getline(inputf, line);
                    ss.str("");
                    ss.clear();
                    ss.str(line);
                    getline(ss, tok, ' ');
                    getline(ss, tok, ' ');
                    tok.erase(0,2);//remove MC
                    // int cellIdx = stoi(tok);
                    int cellIdx;
                    std::istringstream (tok) >> cellIdx;

                    int sizeX, sizeY, pinCnt;
                    getline(ss, tok, ' ');
                    // sizeX = stoi(tok);
                    std::istringstream (tok) >> sizeX;

                    getline(ss, tok, ' ');
                    // sizeY = stoi(tok);
                    std::istringstream (tok) >>  sizeY;

                    getline(ss, tok, ' ');
                    // pinCnt = stoi(tok);
                    std::istringstream (tok) >>  pinCnt;

                    box.cellLib[cellIdx].setStat(sizeX, sizeY, pinCnt, 1);
                    //Pin <pinName> <pinLocationX> <pinLocationY>
                    for(int j = 0; j < pinCnt; j++){
                        getline(inputf, line);
                        ss.str("");
                        ss.clear();
                        ss.str(line);
                        getline(ss, tok, ' ');
                        getline(ss, tok, ' ');
                        tok.erase(0,1);//remove P
                        // int pinIdx = stoi(tok);
                        int pinIdx;
                        std::istringstream (tok) >> pinIdx;
                        int pinX, pinY;
                        getline(ss, tok, ' ');
                        // pinX = stoi(tok);
                        std::istringstream (tok) >> pinX;

                        getline(ss, tok, ' ');
                        // pinY = stoi(tok);
                        std::istringstream (tok) >> pinY;
                        box.cellLib[cellIdx].setPin(pinIdx, pinX, pinY, 1);
                    }
                }
            }
        }
        //TopDieTech <TechName>
        else if(tok == "TopDieTech"){
            getline(ss, tok, ' ');
            box.boardTech[0] = (tok[1] == 'B' ? 1 : 0);
        }
        //BottomDieTech <TechName>
        else if(tok == "BottomDieTech"){
            getline(ss, tok, ' ');
            //cout << tok << endl;
            box.boardTech[1] = (tok[1] == 'B' ? 1 : 0);
        }
        //TerminalSize <sizeX> <sizeY>
        else if(tok == "TerminalSize"){
            getline(ss, tok, ' ');
            // box.terminalSizeX = stoi(tok);
            std::istringstream (tok) >> box.terminalSizeX;

            getline(ss, tok, ' ');
            // box.terminalSizeY = stoi(tok);
            std::istringstream (tok) >> box.terminalSizeY;
        }
        //TerminalSpacing <spacing>
        else if(tok == "TerminalSpacing"){
            getline(ss, tok, ' ');
            // box.terminalSpace = stoi(tok);
            std::istringstream (tok) >> box.terminalSpace;
        }
    }

    return true;
}
static bool isBookshelfSymbol(unsigned char c) {
    static char symbols[256] = {0};
    static bool inited = false;
    if (!inited) {
        symbols[(int)'('] = 1;
        symbols[(int)')'] = 1;
        // symbols[(int)'['] = 1;
        // symbols[(int)']'] = 1;
        symbols[(int)','] = 1;
        // symbols[(int)'.'] = 1;
        symbols[(int)':'] = 1;
        symbols[(int)';'] = 1;
        // symbols[(int)'/'] = 1;
        symbols[(int)'#'] = 1;
        symbols[(int)'{'] = 1;
        symbols[(int)'}'] = 1;
        symbols[(int)'*'] = 1;
        symbols[(int)'\"'] = 1;
        symbols[(int)'\\'] = 1;

        symbols[(int)' '] = 2;
        symbols[(int)'\t'] = 2;
        symbols[(int)'\n'] = 2;
        symbols[(int)'\r'] = 2;
        inited = true;
    }
    return symbols[(int)c] != 0;
}
static bool readBSLine(istream& is, vector<string>& tokens) {
    tokens.clear();
    string line;
    while (is && tokens.empty()) {
        // read next line in
        getline(is, line);

        char token[1024] = {0};
        int lineLen = (int)line.size();
        int tokenLen = 0;
        for (int i = 0; i < lineLen; i++) {
            char c = line[i];
            if (c == '#') {
                break;
            }
            if (isBookshelfSymbol(c)) {
                if (tokenLen > 0) {
                    token[tokenLen] = (char)0;
                    tokens.push_back(string(token));
                    token[0] = (char)0;
                    tokenLen = 0;
                }
            } else {
                token[tokenLen++] = c;
                if (tokenLen > 1024) {
                    // TODO: unhandled error
                    tokens.clear();
                    return false;
                }
            }
        }
        // line finished, something else in token
        if (tokenLen > 0) {
            token[tokenLen] = (char)0;
            tokens.push_back(string(token));
            tokenLen = 0;
        }
    }
    return !tokens.empty();
}
static void writeResult(char* file, BOX &box){
    ofstream outputf(file, ofstream::trunc);
    vector<string> tokens;
    outputf << "TopDiePlacement " << box.numOfTopCell << endl;
    ifstream intopf("./result/top.pl");
    while (readBSLine(intopf, tokens)) {
        outputf << "Inst " << tokens[0] << " " << tokens[1] << " " << tokens[2] << endl;
    }
    outputf << "BottomDiePlacement " << box.numOfBotCell << endl;
    ifstream inbotf("./result/bot.pl");
    while (readBSLine(inbotf, tokens)) {
        outputf << "Inst " << tokens[0] << " " << tokens[1] << " " << tokens[2] << endl;
    }
    outputf << "NumTerminals " << box.numOfTerminal << endl;
    ifstream interf("./result/ter.pl");
    stringstream ss;
    while (readBSLine(interf, tokens)) {
        ss.clear();
        ss.str(tokens[0]);
        char c; int terName;
        ss >> c >> terName;
        terName = box.terminalMP[terName];
        int coordinateX = stoi(tokens[1]) + box.terminalSpace + ceil(box.terminalSizeX / 2);
        int coordinateY = stoi(tokens[2]) + box.terminalSpace + ceil(box.terminalSizeY / 2);
        outputf << "Terminal N" << terName << " " << coordinateX << " " << coordinateY << endl;
    }
}