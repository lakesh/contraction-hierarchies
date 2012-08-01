//
//  experiment.cpp
//  
//
//  Created by lakesh kansakar on 7/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include "io/createGraph.h"
#include "io/output.h"
#include "processing/DijkstraCH.h"
#include "processing/ConstructCH.h"

using namespace std;

const static bool COMPRESSED_SGR_FILE = false;

int main(int argc, char *argv[]) {
    //Search graph of minneapolis
    string sgrFile = "exact-190-1-0-0-0-1000-0-1000-0-60-145-0-1-70-0.sgr";
    int n = 100;
    int source[n];
    int target[n];
    //Number of nodes in the graph of minneapolis
    int num_nodes = 139901;
    
    /* initialize random seed: */
    srand ( time(NULL) );
    
    for(int i=0; i <n; i++) {
        source[i] = rand() % num_nodes;
        target[i] = rand() % num_nodes;
    }
    
    ifstream in(sgrFile.c_str());
    if (!in.is_open()) { cerr << "Cannot open " << sgrFile << endl; exit(1); }
    boost::iostreams::filtering_istream zIn;
    if ( COMPRESSED_SGR_FILE) zIn.push(boost::iostreams::gzip_decompressor());
    zIn.push(in);
    
    datastr::graph::SearchGraph* searchGraph = NULL;
    searchGraph = new datastr::graph::SearchGraph(zIn);
    
}

