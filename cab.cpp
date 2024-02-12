#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime> 
#include <algorithm>
#include <cstdio>
#include <filesystem> // C++17 required
#include <cstdlib>
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

using namespace std;
namespace fs = std::filesystem; //alias for std::filesystem

int main(){
    system("cls");
    cout<<endl;
    cout<<" -----------------------------------------------------------------------------------\n";
    cout<<"     This program compares two files and if they are different, it creates a backup \n";
    cout<<"     Location of your files is stored in location.txt \n ";
    cout<<"-----------------------------------------------------------------------------------\n";
    cout<<endl;
    fstream freference, fcurrent, oploc; 
    ofstream fbackup; 

    //char sub1, sub2
    string  filePath1, filePath2, locinput1, locinput2, sub1, sub2;
    int position;

    oploc.open ("location.txt", ios::in|ios::out|ios::binary);
    

    ////////check if location.txt exists. if not, create it and ask for the location of the project
    if (!oploc)
    {
        oploc.open("location.txt", ios::out); //open just output mode to create the file
        cout<<"\n location.txt file does not exist. \n Copy/paste the file path of your project (only english characters) \n";
        getline(cin,locinput1);
        locinput1.erase(std::remove(locinput1.begin(), locinput1.end(), '\r'), locinput1.end());
        cout<<" Copy/paste the folder location where you want to store your backups (only english characters) \n";
        getline(cin,locinput2);
        locinput2.erase(std::remove(locinput2.begin(), locinput2.end(), '\r'), locinput2.end());
        oploc<<locinput1<<endl;
        oploc<<locinput2;
    }
    oploc.close();
    //remove carriage return and newline characters from the strings
    
    oploc.open ("location.txt", ios::in|ios::out|ios::binary); 
    cout<<endl;
    ////////////////store locations into two different strings
    int n=1;
    while(n<3)
    //while(!oploc.eof()) 
    {
        

        if (n==1)
        {
            getline(oploc,filePath1);
            cout<<"Your project is: \n"<<filePath1<<endl;

            #ifdef _WIN32
                // replace the symbol '\' with '\\' to avoid errors
                size_t position1 = 0;
                while ((position1 = filePath1.find("\\", position1)) != std::string::npos) {
                filePath1.replace(position1, 1, "\\\\");
                position1 += 2;} // Continue from two characters after
            #endif
        }                                                       
        else
        {
            getline(oploc,filePath2);
            cout<<" Your backup folder is: \n"<< filePath2 <<endl;
            
            #ifdef _WIN32
                // replace the symbol '\' with '\\' to avoid errors
                size_t position2 = 0;
                while ((position2 = filePath2.find("\\", position2)) != std::string::npos) {
                    filePath2.replace(position2, 1, "\\\\");
                    position2 += 2;} // Continue from two characters after
            #endif            
        }
        n++;

    }
    oploc.close();
    
    cout<<endl;
    //just in case, remove carriage return and newline characters from the strings
    filePath1.erase(std::remove(filePath1.begin(), filePath1.end(), '\n'), filePath1.end());
    filePath1.erase(std::remove(filePath1.begin(), filePath1.end(), '\r'), filePath1.end());
    filePath2.erase(std::remove(filePath2.begin(), filePath2.end(), '\n'), filePath2.end());
    filePath2.erase(std::remove(filePath2.begin(), filePath2.end(), '\r'), filePath2.end());

    ///////////////get file extension
    fs::path p(filePath1); 
    string filext= p.extension().string();


    ///////////////open the files
    string referencepath= filePath2 +"reference_file"+filext;
    fcurrent.open( filePath1 , ios::in|ios::out); 
    freference.open (referencepath, ios::in|ios::out|ios::binary);
    fbackup.open(filePath2);
    
    ///////////project file directory validity control
    if(!fcurrent){
        perror("ERROR:Input file could not be located/opened. Exiting program.");
        //remove("location.txt");
        return(EXIT_FAILURE);
        exit(EXIT_FAILURE);}
    else{
        cout<<"Project file located \n";}
        cout << "File Extension: " << p.extension()<<endl; 
        

    //// create a backup directory if it does not exist. works both for windows and unix

    #ifdef _WIN32
        if (_mkdir(filePath2.c_str()) == 0) {
    #else
        if (mkdir(filePath2.c_str(), 0777) == 0) {
    #endif
            cout << "Backup directory created successfully\n";
        } else {
            cout << "Backup directory located\n";
        }
    
    ////////////////reference file existance control, later to be used for comparison
    if (!freference)
    {
        cout<<"Reference file does not exist. Creating one now \n";
        cout<<endl;
        freference.open(referencepath, ios::out);
        while( getline(fcurrent,sub1) )
            {
            //fcurrent.ignore();
            
            freference<<sub1<<endl;
            }
        freference.close();
    }
    else
    {
        cout<<"Reference file located \n";
        cout<<endl;
    }


    ///difference control
    int ind=0;

    getline(fcurrent,sub1);
    getline(freference,sub2);
    sub1.erase(std::remove(sub1.begin(), sub1.end(), '\r'), sub1.end());
    sub2.erase(std::remove(sub2.begin(), sub2.end(), '\r'), sub2.end());

    //freference.seekg(0, ios::beg); //reset reading index of file
    while(1)
    {
        if(sub1!=sub2){
            cout<<"Files are different. Creating a backup"<<endl;
            cout<<endl;
            ind=1;
            break;
        }
        if((fcurrent.eof())||(freference.eof())){
            break; //if files end
        }
                
        getline(fcurrent,sub1); 
        getline(freference,sub2);
        sub1.erase(std::remove(sub1.begin(), sub1.end(), '\r'), sub1.end());
        sub2.erase(std::remove(sub2.begin(), sub2.end(), '\r'), sub2.end());
    }

    freference.close();
    fcurrent.seekg(0, ios::beg); //reset reading index of file
    fcurrent.clear();


    if(ind) //if files are different
    {
        //compute time and date to append as string to backup file's name
        auto start = chrono::system_clock::now();
        auto end = chrono::system_clock::now();
        time_t current_time = chrono::system_clock::to_time_t(end);
        string stime=ctime(&current_time); //current time string stime

        //erase blankspaces and replace : with % to allow filename appending
        for(int i = 0; i < stime.length(); i++)
        {
            if( isspace(stime[i]) )
                    stime[i] = '_';
        }
        replace( stime.begin(), stime.end(), ':', '%');
        //create a filename string with current time
        string backup_loc= filePath2 + "ProjectBackup_" + stime + filext;
        cout<<"Backup file created at "<<backup_loc<<endl;
        cout<<endl;

        //write the backup and reference
        fbackup.open(backup_loc);
        freference.open(referencepath,ios::out|ofstream::trunc);

            while(!fcurrent.eof()){//store fcurrents data into and freference

            getline(fcurrent,sub1);
            fbackup<<sub1<<endl;
            freference<<sub1<<endl;
            }
    }
    else
        cout<<"No difference detected"<<endl;
        cout<<endl;

    //close files
    freference.close();
    fbackup.close();
    fcurrent.close();
    cout<<"All done \n ";
    system("pause");

    return 0;
}