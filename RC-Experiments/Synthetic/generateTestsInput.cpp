#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#define SIMP_FLOAT 0
#define SIMP_STRING 1
#define COMP_FLOAT 2
#define COMP_STRING 3
using namespace std;
// GLOBAL VARS
// For the query
string queryType[] = {"all","any"};
string conector[] = {"and","or"};
string operation[] = {"=","<","<=",">",">="};
// For the test case input
int nrT2;
int nrT1;
double correlation;
int nrGroups;
double overlapGroups;
string outlier;
string startDistribution;
string fromDistribution;
string toDistribution;
int nrReqAtts;
int reqAttsTypes[100];
int reqAttsSizes[100];
string reqAtts;
// For our own app
double percentage;
double threshold;

string FLOATATT(int x){ return "(float,"+to_string(x)+") EUCLIDIAN "+to_string(threshold)+"\nUNIFORM 0 1\n";}
string STRINGATT(int x){ return "(string,"+to_string(x)+") LEDIT "+to_string(threshold)+"\nUNIFORM\n";}

void printValues(string directory){
	ofstream output(directory);
	output<<nrT2<<endl;
	output<<nrT1<<endl;
	output<<correlation<<endl;
	output<<nrGroups<<endl;
	output<<outlier<<endl;
	output<<overlapGroups<<endl;
	output<<startDistribution<<endl;
	output<<fromDistribution<<endl;
	output<<toDistribution<<endl;
	output<<nrReqAtts<<endl;
	output<<reqAtts;
	output<<nrReqAtts+1<<endl;
	output<<reqAtts;
	output<<"ATT"<<nrReqAtts<<STRINGATT(10);
	output.close();
}

// void printQuery(string directory, string qType){
// 	ofstream output(directory);
// 	output<<"T1.data;T2.data;"<<nrReqAtts<<";"<<qType<<endl;
// 	// Generating the composed query 1,or,(2,and,(...))
// 	output<<"1";
// 	for(int i=1; i<nrReqAtts; i++){
// 		output<<","<<conector[rand()%2]<<",";
// 		if(nrReqAtts > 2 and i < nrReqAtts-1) output<<"(";
// 		output<<i+1;
// 	}
// 	for(int i=0; i<nrReqAtts-2; i++){
// 		output<<")";
// 	}
// 	output<<endl;
// 	// Generating the comparisions between attributes ATT0 </>/<=/... ATT0, etc
// 	for(int i=0; i<nrReqAtts; i++){
// 		output<<"ATT"<<i<<";"<<"ATT"<<i<<";";
// 		if(reqAttsTypes[i] == SIMP_FLOAT){
// 			output<<operation[rand()%5]<<";0;";
// 		}
// 		else if (reqAttsTypes[i] == SIMP_STRING){
// 			output<<"="<<";0;";
// 		}
// 		//else if(reqAttsTypes[i] == COMP_FLOAT) output<<"EUCLIDIAN;"<<(double)(rand()%99+1)/100.0<<";";
// 		else if(reqAttsTypes[i] == COMP_FLOAT) output<<"EUCLIDIAN;"+to_string(threshold)+";";
// 		else output<<"LEDIT;0;";
// 		output<<endl;
// 	}
// 	// Closing file
// 	output.close();
// }

void printQueries(string directory){
	// Creating files
	ofstream outAll(directory + "queryAll");
	ofstream outAny(directory + "queryAny");
	// Printing headers
	outAll<<"T1.data;T2.data;"<<nrReqAtts<<";"<<"all"<<endl;
	outAny<<"T1.data;T2.data;"<<nrReqAtts<<";"<<"any"<<endl;
	// Generating the composed query 1,or,(2,and,(...))
	outAll<<"1";
	outAny<<"1";
	for(int i=1; i<nrReqAtts; i++){
		outAll<<","<<conector[rand()%2]<<",";
		outAny<<","<<conector[rand()%2]<<",";
		if(nrReqAtts > 2 and i < nrReqAtts-1) {
			outAll<<"(";
			outAny<<"(";
		}
		outAll<<i+1;
		outAny<<i+1;
	}
	for(int i=0; i<nrReqAtts-2; i++){
		outAll<<")";
		outAny<<")";
	}
	outAll<<endl;
	outAny<<endl;
	// Generating the comparisions between attributes ATT0 </>/<=/... ATT0, etc
	for(int i=0; i<nrReqAtts; i++){
		outAll<<"ATT"<<i<<";"<<"ATT"<<i<<";";
		outAny<<"ATT"<<i<<";"<<"ATT"<<i<<";";
		if(reqAttsTypes[i] == SIMP_FLOAT){
			int randOperation = rand()%5;
			outAll<<operation[randOperation]<<";0;";
			outAny<<operation[randOperation]<<";0;";
		}
		else if (reqAttsTypes[i] == SIMP_STRING){
			outAll<<"="<<";0;";
			outAny<<"="<<";0;";
		}
		//else if(reqAttsTypes[i] == COMP_FLOAT) output<<"EUCLIDIAN;"<<(double)(rand()%99+1)/100.0<<";";
		else if(reqAttsTypes[i] == COMP_FLOAT) {
			outAll<<"EUCLIDIAN;"+to_string(threshold)+";";
			outAny<<"EUCLIDIAN;"+to_string(threshold)+";";
		}
		else {
			outAll<<"LEDIT;0;";
			outAny<<"LEDIT;0;";
		}
		outAll<<endl;
		outAny<<endl;
	}
	// Closing file
	outAll.close();
	outAny.close();
}


void printCreateIndexes(string directory){
	ofstream output(directory+"createIndexes.sh");
	for(int i=0; i<nrReqAtts; i++){
		output<<"../../../../../../RC-IndexGenerator/Debug/IndexCreator T1.data "<<i<<" ";
		if(reqAttsTypes[i] == SIMP_FLOAT or reqAttsTypes[i] == SIMP_STRING) output<<"simple"<<endl;
		else output<<"complex"<<endl;
	}
}

void generateRandomAtts(){
	int type;
	reqAtts = "";
	int nrSimpleAtts = nrReqAtts*percentage;
	int nrComplexAtts = nrReqAtts - nrSimpleAtts;
	for(int i=0; i<nrReqAtts; i++){
		// This line is for random types
		// type = rand()%3; // we wont work with coplex strings for now
		// In this case we will have defined the nr of simple atts and the number of complex atts
		if(nrSimpleAtts > 0){
			type = rand()%2;
			nrSimpleAtts--;
		}
		else{
			type = COMP_FLOAT;
			nrComplexAtts--;
		}
		// Let's start
		reqAttsTypes[i] = type;
		reqAtts += "ATT"+to_string(i);
		if(type == SIMP_FLOAT){
			reqAttsSizes[i] = 1;
			reqAtts += FLOATATT(1);
		}
		else if(type == COMP_FLOAT){ 
			//reqAttsSizes[i] = rand()%10 + 2; // variable characteristics vector size
			reqAttsSizes[i] = 10; // fixed characteristics vector size
			reqAtts += FLOATATT(reqAttsSizes[i]);
			for(int j=1; j<reqAttsSizes[i]; j++){
				reqAtts += "UNIFORM 0 1\n";
			}
		}
		else{
			reqAttsSizes[i] = rand()%10 + 1;	
		 	reqAtts += STRINGATT(reqAttsSizes[i]);
	 	}
	}
}

void reinitValues(){
	nrT2 = 5;
	nrT1 = 1000;
	correlation = 0.5;
	nrGroups = 10;
	outlier = "outlier true";
	overlapGroups = 0.1;
	startDistribution = "EXPONENCIAL";
	fromDistribution = "UNIFORM";
	toDistribution = "UNIFORM";	
	nrReqAtts = 2;
	reqAtts = "";
	percentage = 1; // of simple atts. 0.5 means half simple and half complex
	threshold = 0.25;
	generateRandomAtts();
}

void generateRandomCases(int nrCases){
	string basePath = "VarValidGroups/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	nrT1 = 500;
	nrGroups = 100;
	threshold = 0.9;

	for(int i=1; i<=nrCases; i++){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
	}
}

void generateVarT1(int begin, int end, int nrBins){
	string basePath = "VarT1/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(nrT1=begin; nrT1<=end; nrT1+=binSize){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

void generateVarT2(int begin, int end, int nrBins){
	string basePath = "VarT2/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(nrT2=begin; nrT2<=end; nrT2+=binSize){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

void generateVarGroups(int begin, int end, int nrBins){
	string basePath = "VarTG/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(nrGroups=begin; nrGroups<=end; nrGroups+=binSize){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

void generateVarReqAtts(int begin, int end, int nrBins){
	string basePath = "VarL1L2/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(nrReqAtts=begin; nrReqAtts<=end; nrReqAtts+=binSize){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

void generateVarThreshold(int begin, int end, int nrBins){
	string basePath = "VarThreshold/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(threshold=(double)begin/100.0; threshold<=(double)end/100.0; threshold+=(double)binSize/100.0){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

void generateVarGDistribution(){
	string basePath = "VarDistribution/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int i = 1;
	for(int j=1; j<=90; j++){
		if(j==1) startDistribution = "NORMAL";
		if(j==31) startDistribution = "UNIFORM";
		if(j==61) startDistribution = "EXPONENCIAL";
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}

/*
void generateVarPercentage(int begin, int end, int nrBins){
	string basePath = "VarReqs/";
	string fullPath = "";
	string fileName = "";
	string fileExt = ".in";
	int binSize = (end-begin)/(nrBins-1);
	int i = 1;
	for(nrReqAtts=begin; nrReqAtts<=end; nrReqAtts+=binSize){
		generateRandomAtts();
		fileName = to_string(i);
		fullPath = basePath + fileName + "/";
		system(("mkdir -p "+fullPath).c_str());
		printValues(fullPath+fileName+".in");
		printQueries(fullPath);
		printCreateIndexes(fullPath);
		i++;
	}
}
*/

int main(){
	srand((unsigned)time(0));
	
	reinitValues();
	generateRandomCases(100);
	reinitValues();
	generateVarT1(100,10000,100);
	reinitValues();
	generateVarT2(1,50,50);
	reinitValues();
	generateVarGroups(11,110,100);
	reinitValues();
	generateVarReqAtts(2,100,50);
	//reinitValues();
	//generateVarThreshold(1,100,50);
	
	reinitValues();
	generateVarGDistribution();
	//reinitValues();
	//generateVarPercentage(1,100,50);
	return 0;
}
