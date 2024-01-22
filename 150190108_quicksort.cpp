#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct City {

    string name;
    int population;

};

void swap(City& x, City& y){

    City temp = x;
    x = y;
    y = temp;

}


int partition(vector<City>& array, int start, int end, char pivotingStrategy){

    srand(time(nullptr)); 

    if(pivotingStrategy=='r'){

        int random = rand() % (end - start + 1) + start;
        
        swap(array[end],array[random]);

    }else if(pivotingStrategy=='m'){

        int random1 = start + rand() % (end - start + 1);
        int population1 = array[random1].population;

        int random2 = start + rand() % (end - start + 1);
        int population2 = array[random2].population;

        int random3 = start + rand() % (end - start + 1);
        int population3 = array[random3].population;

        int median;

        if((population2 <= population1 && population1 <= population3) || (population3 <= population1 && population1 <= population2)){

            median = random1;

        }else if((population1 <= population2 && population2 <= population3) || (population3 <= population2 && population2 <= population1)){

            median = random2;

        }else{

            median = random3;

        }

        swap(array[end],array[median]);
    } 

    City pivot = array[end];

    int i = start - 1;

    for(int j = start; j<end; j++){

        if(array[j].population <= pivot.population){

            i++;

            swap(array[i], array[j]);

        }

    }

    int pivotIndex = i + 1;

    swap(array[pivotIndex],array[end]);

    return pivotIndex;

}

void quickSort (vector<City>& array, int start, int end, char pivotingStrategy, ofstream& verboseOutputFile){

    if(start<end){

        int q = partition(array, start, end, pivotingStrategy);

        if (verboseOutputFile.is_open()) {
            verboseOutputFile << "Pivot: " << array[q].population << " Array: [ ";
            for (int i = start; i <= end; i++) {
                verboseOutputFile << array[i].population << " ";
            }
            verboseOutputFile << "]" << endl;
        }

        quickSort(array, start, q-1, pivotingStrategy, verboseOutputFile);

        quickSort(array, q+1, end, pivotingStrategy, verboseOutputFile);

    }

}

void insertionSort(vector<City>& array, int arraySize){

    int i;
    City key;

    for(int j=1; j<=arraySize; j++){
        
        key = array[j];

        i = j-1;

        while(i>=0 && array[i].population>key.population){

            array[i+1]=array[i];
            
            i--;

        }

        array[i+1] = key;

    }

}

int main(int argc, char* argv[]){

    string inputFileName = argv[1];

    char pivotingStrategy = argv[2][0];

    int threshold = stoi(argv[3]);

    string outputFileName = argv[4];

    bool verbose = false;

    if(argv[5] != NULL && argv[5][0] == 'v'){
        
        verbose = true;

    }

    ifstream inputFile(inputFileName);

    if(!inputFile.is_open()) {

        cerr << "Error opening file: " << inputFileName << endl;

        return 1;

    }

    vector<City> cityArray;
    City city;
    string line;

    while (getline(inputFile, line)) {

        stringstream ss(line);

        if (getline(ss, city.name, ';')) {

            if (ss >> city.population) {

                cityArray.push_back(city);

            }
        }
    }

    inputFile.close();

    ofstream verboseOutputFile("log.txt");

    auto begin = high_resolution_clock::now();

    if(cityArray.size()>threshold){

        quickSort(cityArray, 0, cityArray.size()-1, pivotingStrategy, verboseOutputFile);

    }else{

        insertionSort(cityArray, cityArray.size()-1);

    }

    auto end = high_resolution_clock::now();

    string sortingStrategy = (cityArray.size()>threshold) ? "QuickSort" : "Insertion Sort";

    cout << "Time taken by " << sortingStrategy << " with pivot strategy '" << pivotingStrategy << "' and threshold " << threshold << ": " << duration_cast<nanoseconds>(end - begin).count() << " ns." << endl;

    verboseOutputFile.close();

    ofstream outputFile(outputFileName);

    if(!verbose){

        remove("log.txt");

    }

    if (!outputFile.is_open()) {

        cerr << "Error opening output file: " << outputFileName << endl;

        return 1;

    }

    for (int i = 0; i < cityArray.size(); i++) {

        outputFile << cityArray[i].name << ";" << cityArray[i].population << endl;

    }

    outputFile.close();

}