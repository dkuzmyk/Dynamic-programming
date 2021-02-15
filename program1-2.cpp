//
// Created by smape on 10/15/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

struct ssum_elem {
    unsigned int x;
    std::string name;
};

/*void printIter(const basic_string<char>& a)
{
    for(char i : a){
        cout << i << " ";
    }
}

string subsetCheck(int index, int tgt, vector<ssum_elem> ele,string subString){
    if(subString == "") {subString.append("{");}
    if(tgt < 0 ) return "error";
    if(tgt == 0) {subString.erase(subString.size()-1); subString.append("}"); return subString;}
    subString.append(to_string(ele[index].x)+",");
    int newTarget = tgt-ele[index].x;
    if(ele[index].x > tgt)
        return subsetCheck(index-1, tgt, ele, subString);
    else
        return subsetCheck(index-1, newTarget, ele, subString);
}

void VectorHold(int t, vector<ssum_elem> v)
{
    vector<string> TV;
    //target is 5

    for(int j = 0; j < v.size(); j++) //iterate through the vector
    {
        TV.push_back("");
        int s = t-v[j].x; //5-1 this should give 4
        for(int k = 0; k < v.size(); k++)
        {
            if(s == v[k].x)  //if we find the sum in the vector
            {
                //store the string at for values 1 and 4
                //index 0
                //index 5
                TV[j].append(v[j].name+" ");
                TV[j].append(v[k].name+" ");
            }
        }
    }

    printf("the number of min sized satisfying solutions %d", TV.size());
    cout << endl;
    for(int z = 0; z < TV.size(); z++)
        cout << TV[z] << endl;
}*/



class ssum_instance {
    unsigned int target=0;
    std::vector<ssum_elem> elems;
    std::vector<std::vector<bool>> feasible;
    std::vector<std::vector<int>> sscount;
    vector<vector<int>> min_card;
    vector<vector<int>> min_subset;
    vector<vector<set<int>>> first_subset;

    //set<vector<unsigned int>> ssm;

    // feasible[i][x] = TRUE if there is a subset
    //  of a[0..i] adding up to x; FALSE otherwise
    //  (once instance has been solved)
    //
    int done=false;    // flag indicating if dp has been run or
    //   not on this instance

public:

    // Function:  read_elems
    // Description:  reads elements from standard-input;
    //   Format:  sequence of <number> <name> pairs where
    //      <number> is non-negative int and
    //      <name> is a string associated with element
    void read_elems( std::istream &stream) {
        ssum_elem e;

        elems.clear();
        // while(std::cin >> e.x && std::cin >> e.name) {
        while(stream>> e.x && stream >> e.name) {
            elems.push_back(e);
        }
        done = false;
    }

    // Function:  solve
    // Desc:  populates dynamic programming table of
    //    calling object for specified target sum.
    //    Returns true/false depending on whether the
    //    target sum is achievalble or not.
    //    Table remains intact after call.
    //    Object can be reused for alternative target sums.
    bool solve(unsigned int tgt) {
        unsigned int n = elems.size();
        unsigned int i, x;

        if(target == tgt && done)
            return feasible[n-1][tgt];

        target = tgt;
        feasible =
                std::vector<std::vector<bool>>(n, std::vector<bool>(target+1, false));
        sscount =
                std::vector<std::vector<int>>(n, std::vector<int>(target+1, 0));

        min_subset = vector<vector<int>>(n, std::vector<int>(target+1, 0));

        min_card = vector<vector<int>>(n, vector<int>(target+1, -1));

        first_subset = vector<vector<set<int>>>(n, vector<set<int>>(target+1, {0}));

        //it = std::vector<std::vector<std::set<std::string>>>(n, std::vector<std::set<std::string>>(target+1));


        // leftmost column (column zero) is all TRUE because
        //    a target sum of zero is always achievable (via the
        //    empty set).
        for(i=0; i<n; i++)
        {
            feasible[i][0] = true;
            sscount[i][0] = 1;
            min_card[i][0] = 0;
            min_subset[i][0] = 1;
            first_subset[i][0].insert(0);
        }

        for(x=1; x<=target; x++) {
            if(elems[0].x == x)
            {
                feasible[0][x] = true;
                sscount[0][x] = 1;
                min_card[0][x] = 1;
                min_subset[0][x] = 1;
                first_subset[0][x].insert(1);
            }
            // otherwise, feasible[0][x] remains false
        }

        for(i=1; i<n; i++) {
            for(x=1; x<=tgt; x++) {
                if(feasible[i-1][x]){
                    feasible[i][x] = true;
                    sscount[i][x] = sscount[i-1][x];
                }
                if(x >= elems[i].x && feasible[i-1][x-elems[i].x]){
                    feasible[i][x] = true;
                    sscount[i][x] = sscount[i-1][x] + sscount[i-1][x-elems[i].x];
                }

                if(feasible[i-1][x] && x >= elems[i].x && feasible[i-1][x-elems[i].x]){
                    min_card[i][x] = min(min_card[i-1][x], min_card[i-1][x-elems[i].x]+1);
                    //min_subset[i][x] = min_subset[i-1][x-elems[i].x]+min_card[i-1][x];
                }
                else if(feasible[i-1][x] && !(x >= elems[i].x && feasible[i-1][x-elems[i].x])){
                    min_card[i][x] = min_card[i-1][x];
                    //min_subset[i][x] = min_subset[i-1][x];
                }
                else if(x >= elems[i].x && feasible[i-1][x-elems[i].x]){
                    min_card[i][x] = min_card[i-1][x-elems[i].x] + 1;
                    //min_subset[i][x] = min_subset[i-1][x-elems[i].x];
                }


                if(x >= elems[i].x && feasible[i-1][x-elems[i].x]){
                    min_subset[i][x] = min_subset[i-1][x-elems[i].x];
                }

                if(feasible[i-1][x] && x == elems[i].x){
                    if(min_card[i-1][x] > min_card[i][x])
                        min_subset[i][x] = 1;
                    else
                        min_subset[i][x] = min_subset[i-1][x]+1;
                }

                else if(feasible[i-1][x] && x < elems[i].x){
                    min_subset[i][x] = min_subset[i-1][x];
                }

                else if(feasible[i-1][x] && x > elems[i].x && feasible[i-1][x-elems[i].x]){
                    if(min_card[i-1][x] < min_card[i-1][x-elems[i].x]+1)
                        min_subset[i][x] = min_subset[i-1][x];
                    else if(min_card[i-1][x] > min_card[i-1][x-elems[i].x]+1)
                        min_subset[i][x] = min_subset[i-1][x-elems[i].x];
                    else if(min_card[i-1][x] == min_card[i-1][x-elems[i].x]+1)
                        min_subset[i][x] = (min_subset[i-1][x] + min_subset[i-1][x-elems[i].x]);
                    else
                        cout << "ERROR" << endl; // debug
                }
                else if(feasible[i-1][x] && x > elems[i].x && !feasible[i-1][x-elems[i].x])
                    min_subset[i][x] = min_subset[i-1][x];
                //else
                    //cout << "SOME OTHER CONDITION" << "i: " << i << " x: " << x << endl;


                if(x == elems[i].x){
                    first_subset[i][x].clear();
                    first_subset[i][x].insert(elems[i].x);
                }

                if(x < elems[i].x){
                    first_subset[i][x].clear();
                    first_subset[i][x] = first_subset[i-1][x];
                }

            }
        }

        //VectorHold(target, elems);

        //for (int i = 0; i < n; i++){
        //    if(sscount[i][tgt]!=0){
                //cout << subsetCheck(i, tgt, elems, "") << endl;
        //    }
        //}

        //cout << subsetCheck(4, 5, elems,"") << endl;

        unsigned int m;         // contains the min size subset for target
        m = min_card[i-1][x-1]; // fixed the table, now this works

        unsigned int d=0;        // number of distinct solutions of min size
        d = min_subset[i-1][x-1];

        // min_card contains the 'number of steps it takes to get to that position' so we find the shortest #steps
        /*for(int a = 0; a < n; a++){
            if(m > min_card[a][tgt] && min_card[a][tgt]>=0)
                m = min_card[a][tgt];
            //cout << min_card[a][tgt];
        }*/

        unsigned int r;
        r = sscount[i-1][x-1];

        cout << "The number of distinct solutions is: " << r << endl;
        cout << "Size of the smallest satisfying subset: " << m << endl;
        cout << "The number of distinct subsets having min size: " << d << endl;


        // debug ====================================================================
/*
        cout << endl << "min_card" << endl << endl;
        for(int g = 0; g < n; g++){
            for(int t = 0; t <= tgt; t++)
            {
                cout << min_card[g][t] << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "sscount" << endl;
        for(i=0; i<n; i++){
            std::cout << std::endl;
            for(int j = 1; j<=tgt; j++){
                std::cout << sscount[i][j] << " ";
            }
        }

        cout << endl << endl << "min subset" << endl;
        for(i=0; i<n; i++){
            std::cout << std::endl;
            for(int j = 1; j<=tgt; j++){
                std::cout << min_subset[i][j] << " ";
            }
        }

        cout << endl << endl << "T/F" << endl;
        for(i=0; i<n; i++){
            std::cout << std::endl;
            for(int j = 1; j<=tgt; j++){
                std::cout << feasible[i][j] << " ";
            }
        }

        cout << endl << endl << "first subset" << endl;
        for(i=0; i<n; i++){
            std::cout << std::endl;
            for(int j = 1; j<=tgt; j++){
                for(auto a : first_subset[i][j])
                    cout << "{" << a << "} ";
            }
        }
*/
        //cout << endl << "elems: " << elems[0].x << endl;

/*
        for(i=0; i<ssmap.size(); i++){
            std::cout << std::endl;
            for (auto j : ssmap[i]) {cout << j;}
        }
*/
        //std::cout << std::endl;
        //std::cout << std::endl;
        // ============================================================================

        done = true;
        return feasible[n-1][target];
    }
};  // end class

/**
* usage:  ssum  <target> < <input-file>
*
*
* input file format:
*
*     sequence of non-negative-int, string pairs
*
*     example:

    12 alice
    9  bob
    22 cathy
    12 doug

* such a file specifies a collection of 4 integers: 12, 9, 22, 12
* "named" alice, bob, cathy and doug.
*/

int main(int argc, char *argv[]) {
    unsigned int target;
    ssum_instance ssi;

    if(argc != 2) {
        fprintf(stderr, "one cmd-line arg expected: target sum\n");
        return 0;
    }
    if(sscanf(argv[1], "%u", &target) != 1) {
        fprintf(stderr, "bad argument '%s'\n", argv[1]);
        fprintf(stderr, "   Expected unsigned integer\n");
        return 0;
    }

    ssi.read_elems(std::cin);

    if(ssi.solve(target) ) {
        //
        // std::cout << "HOORAY!  Apparently, the target sum of " <<
        //          target << " is achievable\n";
        //std::cout << "  How you ask?  Sorry, we just know it is possible...\n";
    }
    else {
        std::cout << "SORRY!  Apparently, the target sum of " <<
                  target << " is NOT achievable\n";
    }

}

