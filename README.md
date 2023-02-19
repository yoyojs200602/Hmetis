# Hmetis - A Multilevel Hypergraph Partitioning Algorithm

## Description
This project provides an implementation of Hmetis, a hypergraph partitioning algorithm in C++  
Paper: http://eda.ee.ucla.edu/EE201A-04Spring/hmetis.pdf  

I used it in the 2022 ICCAD contest, Problem B  
Please refer to: http://iccad-contest.org/Problems/CADContest_2022_Problem_B_20220420.pdf

## Usage

* ### Build
```
make
```
(The project can not be compiled under c++98 or earlier versions)
 
* ### Run
```
./hmetis --nleft=200 --ileft=20 --dr=0.3 --max-itr-c=30 --max-itr-r=30 --coarsen-scheme=MHEC --refine-scheme=MHEC --IP-scheme=FM --UC-scheme=FM --UC-scheme-r=FM [input_file] [output_file]  
```
 `--nleft`  Number of clusters left after coarsening (default to 200)  
 `--ileft`  Number of initial partition to do (default to 20)  
 `--dr`  Drop partition instance which is this rate worser in cost than the best instance during uncoarsening (default to 0.3)    
 `--max-itr-c`  maximum layer of coarsening in the first coarsening phase (default to 30)  
 `--mat-itr-r`  maximum layer of coarsening in the refinement phase (default to 30)  
 `--coarsen-scheme`  coarsening scheme in the first coarsening phase, could be EC / HEC / MHEC (default to MHEC)  
 `--refine-scheme`  coarsening scheme in the refinement phase, could be EC / HEC / MHEC (default to MHEC)  
 `--IP-scheme`  initial partition scheme, could be FM / HEC (default to FM)  
 `--UC-scheme`  uncoarsening scheme in first uncoarsening phase, could be FM / HEC (default to FM)  
 `--IP-scheme`  uncoarsening scheme in refinement phase, could be FM / HEC (default to FM)  
 
* ### Input Format  
   * Please refer to: http://iccad-contest.org/Problems/CADContest_2022_Problem_B_20220420.pdf  

* ### Output Format  
```
[Instance Name] [layer 0/1]  
```
   
 ## 
