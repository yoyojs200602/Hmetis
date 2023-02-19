#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Hmetis.hpp"
#include "Hypergraph.hpp"
#include "benchmark.hpp"

int main(int argc, char* argv[]){
    Config config;
    BOX box;
    if (!parsing(argc, argv, config, box))
        return 0;
    //box.printContent();

    Hmetis hmetis = Hmetis();

    hmetis.configureGraph(box);

    hmetis.coarsen(config.numNodeLeft, false, config.coarsenScheme, config.maxCoarsenIter);

    hmetis.initialPartition(config.numInstances, config.initialParScheme);

    hmetis.uncoarsen(config.dropRate, config.uncoarsenScheme);

    hmetis.refine(config.numNodeLeft, config.dropRate, config.coarsenScheme_r, config.uncoarsenScheme_r, config.maxRefineIter);
    
    hmetis.outputGraph(box);

    SCORE score = test(box);
    score.printScore();

    return 0;
}