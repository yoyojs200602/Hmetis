#include <iostream>
using namespace std;

class SCORE{
    public:
    SCORE(){
        area[0] = 0; area[1] = 0; noTerminal = 0; totalNet = 0; totalWeight = 0;
    }
    int noTerminal, totalNet;
    double area[2];
    float totalWeight;
    void printScore(){
        cout << "Area in top layer: " << area[0] << endl
        << "Area in bottom layer: " << area[1] << endl
        << "Number of net using only one layer / total net number: " << noTerminal << " / " << totalNet << endl
        << "total weight: " << totalWeight << endl; //sum of edge's weight that use two layer, each edge weight: (1/total pin cnt-1)
        return;
    }
};

SCORE test(BOX &box){
    SCORE ret;
    int instSize = box.layer.size();
    for(int i = 1; i < instSize; i++){
        int layer = box.layer[i];
        ret.area[layer] += (box.cellLib[box.instLib[i]].sizeX[layer])*(box.cellLib[box.instLib[i]].sizeY[layer]);
    }
    int cnt = 0, netSize = box.net.size(), pinSize;
    ret.totalNet = netSize-1;
    for(int i = 1; i < netSize; i++){
        pinSize = box.net[i].size();
        int firstL = box.layer[box.net[i][0].first];
        bool success = 1;
        for(int j = 1; j < pinSize; j++){
            if(box.layer[box.net[i][j].first] != firstL){
                success = 0;
                break;
            }
        }
        if(success == 1){
            cnt++;
        }
        else{
            float f = static_cast<float>(pinSize-1);
            if(f == 0) f = 1.0; 
            ret.totalWeight += (1.0/f);
        }
    }
    ret.noTerminal = cnt;
    return ret;
}