//Doesn't seem to be able to read certain parts of the title
//need to fix the n/a issue
//would be nice to show the total amount of works everytime categories is called
//add some check to make sure the work isn't already present before scanning
//on categories, quit only works after the results have been viewed
//funny bug when show more info is called right after collections has been called 

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <regex>

using namespace std;

//Functions
void cleanup2(string &source);//Used to scrub numbers and other characters from whatever strings
void removewhite(string &source);//Used to remove multiple instances of whitespace in a string

int main()
{
    //variables
    int h;
    int i;
    int j;
    int k;
    int l;
    int m;
    int functionselect;//used for making selections on the menu
    int posinwork;//location in current work
    int posinword;//location in current word
    int numofwork;//how many works in consideration
    int linenumber;//location in input file
    int numpass;//number of pass terms
    int numblock;//number of block terms
    int numcompare;//number of compare terms
    int check;//a generic check
    int matches;//total number of matches in search results
    int tempnumber;
    int occurances[2048];
    int tempoccurances[2048];

    string copy;
    string paste;
    string tempstring;
    string tempcode;
    string comparestring;
    string capitals = "PCTAGLCP";//the first letter of each term from nhentai in order of appearance
    string infilepath = "../tagsorter/input.txt";
    string offilepath = "../tagsorter/sauce.txt";
    string terms[8] = {"Parodies:", "Characters:", "Tags:", "Artists:", "Groups:", "Languages:", "Categories:", "Pages:"};
    string results[2048];//Only used to display collections, not search results
    string tempresults[2048];
    string tempcodes[256];//I just set these to 256 to be safe. Set them higher if you want idc
    string passterms[256];
    string temppassterms[256];
    string blockterms[256];
    string tempblockterms[256];
    string compareterms[256];
    string codes[256];
    string tempdisplay[11];

    //initialisations
    fill(occurances, occurances + 2048, 0);
    ifstream inFS;
    ofstream ofFS;
    //script
    cout << "Type 0 to close the program" << endl << "Type 1 to scan input.txt" << endl << "Type 2 to use search function" << endl << "Type 3 to show more info about a certain work" << endl << "Type 4 to view a collection of the requested term" << endl << "Type 5 for help on using this program" << endl;
    cin >> functionselect;
    
    do{
        if(functionselect == 0)//close program
        {
            return 0;
        }
        else if(functionselect == 1)//read input.txt
        {
            inFS.open(infilepath);
            ofFS.open(offilepath, ios::app);

            //initialisations
            posinwork = 0;
            posinword = 0;
            numofwork = 0;
            i = 0;
            j = 0;
            copy.clear();
            paste.clear();
            tempstring.clear();
            //operation

            cout << "Work(s) scanned so far..." << endl;

            while(!inFS.eof())//Extract information about files
            {
                posinword = 0;
                
                getline(inFS, copy);
                if(posinwork == 0)
                {
                    for(posinwork; posinwork < 2; posinwork++)//Grab title
                    {        
                        if(copy[0] == '#')//fires if a secondary title is not found
                        {       
                            posinwork++;
                            ofFS << "No secondary title available" << endl;
                            break;
                        }
                        ofFS << copy << endl;
                        getline(inFS, copy);
                    }
                }
                if(posinwork == 2 && copy[0] == '#')//Found the code and grab
                {
                    while(copy[posinword] != '\0')//Scan through each character
                    {
                        if(copy[posinword] != '#')
                        {
                            paste += copy[posinword];
                        }
                        posinword++;
                    }
                    
                    cout << paste << endl;
                    ofFS << paste << endl;

                    paste.clear();
                    copy.clear();
                    posinwork++;
                }
                if(posinwork == 3)//grabs everything else and arragnes it into one giant string
                {
                    if(copy[0] == 'U')//treats the "Uploaded" term as the limiter for an entry
                    {
                        getline(inFS, paste);//skips to the next entry
                        posinwork++;
                        posinword = 0;
                        numofwork++;
                    }
                    else
                    {
                        tempstring += copy;
                    }
                }
                if(posinwork == 4)//arranges everything that starts with a capital letter into a new line
                {
                    paste.clear();
                    
                    while(tempstring[posinword] != '\0')
                    {
                        paste += tempstring[posinword];
                        for(i = 0; i < 6; i++)
                        {
                            if(tempstring[posinword+1] == capitals[i] || tempstring[posinword+1] == '\0')
                            {
                                for(j; j < 8; j++)
                                {
                                    if(paste[0] == capitals[j])
                                    {
                                        if(j != 7)//removes numbers and K but preserves page count
                                        {
                                            cleanup2(paste);
                                            removewhite(paste);
                                        }
                                        else
                                        {   
                                            paste+= ' ';
                                        }
                                        ofFS << paste << endl;
                                        break;
                                    }
                                    else
                                    {
                                        ofFS << terms[j] << " n/a " << endl;
                                    }
                                }
                                paste.clear();
                                j++;
                            }
                        }
                        posinword++;
                    }
                    ofFS << endl;
                    tempstring.clear();
                    posinwork = 0;
                    j = 0;
                }
            }
            inFS.close();
            ofFS.close();
            
            cout << "Successfully scanned " << numofwork << " work(s)" << endl; 
            cout << "Finished writing to sauce.txt" << endl;
    
            cin >> functionselect;
        }
        else if(functionselect == 2)//search function
        {
            //Initialisations
            linenumber = 0;
            numofwork = 0;
            numcompare = 0;
            posinword = 0;
            numpass = 0;
            numblock = 0;
            check = 0;    
            matches = 0;

            tempcode = "blank";
            tempstring = "blank";

            copy.clear();
            paste.clear();
            
            //script
            ifstream inFS;
            inFS.open(offilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input your search terms:" << endl;
             
                getline(cin.ignore(1,'\n'), copy);//grabs the search terms from the tag and arranges them into an array of strings
                copy += ' ';

                while(copy[posinword] != '\0')
                {
                    if(copy[posinword] == '/')//detects item to block 
                    {
                        check = 1;
                    }
                    else if(copy[posinword] != ' ')
                    {
                        paste += copy[posinword];
                    }
                    else
                    {
                        if(check == 0)//assign to passterms if not blocked item
                        {
                            passterms[numpass] = paste;
                            numpass++;
                        }
                        else//assign to blockterms if blocked item
                        {
                            check = 0;
                            blockterms[numblock] = paste;
                            numblock++;   
                        }
                        paste.clear();
                    }
                    posinword++;
                }
                
                cout << numpass << numblock << endl;//XX
                check = 0;

                while(!inFS.eof())
                {
                    //more intialisations
                    posinword = 0;  
                    numcompare = 0;
                    numofwork = linenumber/12;
                    
                    copy.clear();
                    paste.clear();                    
                    //Used for troubleshooting. Feel free to ignore
                    //cout << "numofwork = " << numofwork << ", linenumber = " << linenumber << ", check = " << check << ", matches = " << matches << ", tempcode = " << tempcode << endl;
                    
                    getline(inFS, copy);//scans the sauce folder line by line, arranging terms into another array of strings 
                    
                    while(copy[posinword] != '\0')//assign each item from each line in sauce to compareterms
                    {
                        if(copy[posinword] != ' ')
                        {
                            paste += copy[posinword];
                        }
                        else
                        {
                            compareterms[numcompare] = paste;
                            numcompare++;
                            paste.clear();
                        }
                        posinword++;
                    }
                    compareterms[numcompare] = paste;
                    
                    for(i = 0; i < numpass; i++)//This reloads the pass and block terms after having been deleted in the search
                    {
                        temppassterms[i] = passterms[i];
                    }
                    for(i = 0; i < numblock; i++)
                    {
                        tempblockterms[i] = blockterms[i];
                    }

                    if(linenumber == 2+(12*numofwork))//grabs the code and stores it into tempcode. Can be recalled if the requested terms are in this code
                    {
                        tempcode = paste;
                    }

                    for(i = 0; i <= numcompare; i++)//for each item in compareterms, check to see if anything from passterms is present
                    {
                        for(j = 0; j < numpass; j++)
                        {
                            if(temppassterms[j] == compareterms[i])//match is found, advance counter by 1
                            {       
                                temppassterms[j] = "PLACEHOLDER";//This avoids the issue where a double occurance of a term in the same line activates the counter
                                check++;
                            }
                        }
                        for(j = 0; j < numblock; j++)
                        {
                            if(blockterms[j] == compareterms[i])//blocked item is found, decrease counter by 1
                            {       
                                tempblockterms[j] = "PLACEHOLDER";
                                check = check - 1;
                            }
                        }
                    }             
                    
                    linenumber++;//tells the script where in the sauce folder it is
               
                    if(linenumber%12 == 0)//resets the number of found passterms
                    {
                        if(check == numpass)//If all passterms have been found in an entry, the tempcode is saved to codes[256] to be presented to the user later
                        {
                            codes[matches] = tempcode;
                            matches++;
                        }     
                        check = 0;
                    }
                }
                inFS.close();

                for(i = 0; i < numpass; i++)
                {
                    passterms[i].clear();
                }
                for(i = 0; i < numblock; i++)
                {
                    blockterms[i].clear();
                }

                cout << "Found " << matches << " works that match the requested terms";

                if(matches > 0)
                {
                    cout << ":" << endl;

                    for(i = 0; i < matches; i++)
                    {
                        tempcodes[i] = codes[i];
                        cout << i + 1 << ") " << tempcodes[i] << endl;
                    }
                }
                else
                {
                    cout << endl;
                }
            }
            else
            {
                cout << "Failed to open sauce.txt. Make sure the .txt file has the correct name and try again" << endl;
            }

            cin >> functionselect;
        }
        else if(functionselect == 3)//show more info 
        {
            //initialisations
            check = 0;

            for(i = 0; i < matches; i++)
            {
                cout << i + 1 << ") " << tempcodes[i] << endl;
            }

            string infilepath = "../tagsorter/sauce.txt";
            ifstream inFS;
            inFS.open(infilepath);

            if(inFS.is_open())//Opens file
            {
                cout << "Input the number corresponding to the code from the previous search query: " << endl;
                cin >> i;

                if(i > matches || i <= 0)
                {
                    check = 0;
                    cout << "Invalid request. You need to select one of the listed codes" << endl; 
                }
                else
                {
                    check = 1;
                }

                if(check == 1)
                {
                    while(!inFS.eof())
                    {
                        for(posinwork = 0; posinwork < 12; posinwork++)
                        {
                            getline(inFS, copy);    
                            tempdisplay[posinwork] = copy;    
                        }
                        if(tempdisplay[2] == tempcodes[i-1])
                        {
                            for(posinwork = 0; posinwork < 11; posinwork++)
                            {
                                cout << tempdisplay[posinwork] << endl;
                            }       
                        }                 
                    }
                    inFS.close();
                    cout << "----------------------------------------" << endl;
                }
            }
            else
            {
                cout << "Failed to open sauce.txt. Make sure the .txt file has the correct name and try again" << endl;
            }

            cin >> functionselect;
        }
        else if(functionselect == 4)//show collections
        {
            //Initialisations
            matches = 0;
            linenumber = 0;
            posinwork = 0;
            numofwork = 0;
            check = 0;

            copy.clear();
            tempstring.clear();

            i = 0;
            j = 0;
            k = 0;
            l = 0;
            
            //script
            string infilepath = "../tagsorter/sauce.txt";
            ifstream inFS;
            inFS.open(infilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input a term (Tags, Parodies, Artists, etc.):" << endl;
             
                getline(cin.ignore(1,'\n'), tempstring);//grabs the search terms from the tag and arranges them into an array of strings
                tempstring += ':';
                
                for(i = 0; i < 8; i++)
                {                    
                    if(tempstring == terms[i])
                    {
                        posinwork = i + 3;//tells the program which lines to scan and which to skip
                        check = 1;
                        break;
                    }
                    else if(i == 7)//no match was found for the input. Break out from the function
                    {
                        cout << "Invalid term. Input something valid or check your spelling" << endl;
                        check = 0;
                    }
                }

                if(check == 1)
                {
                    //cout << "Term = " << term << ", tempstring = " << tempstring << ", h = " << h << ", d = " << d << endl;
                    while(!inFS.eof())
                    {
                        numofwork = linenumber/12;
                        posinword = 0;

                        getline(inFS, copy);//scans the sauce folder line by line, arranging terms into another array of strings 
                        
                        //cout << "numofwork = " << numofwork << ", d = " << d << ", check = " << posinwork+(12*numofwork);

                        if(linenumber == posinwork+(12*numofwork))
                        {   
                            while(copy[posinword] != '\0')//assign each item from each line in sauce to compareterms
                            {
                                if(copy[posinword] != ' ')
                                {
                                    paste += copy[posinword];
                                }
                                else
                                {
                                    if(paste[0] != tempstring[0])
                                    {
                                        //cout << " " << tempstring;
                                        for(j = 0; j <= matches; j++)
                                        {
                                            if(j == matches)
                                            {
                                                results[j] = paste;
                                                occurances[j]++;
                                                //cout << i << " " << occurances[k] << " " << results[k] << endl;
                                                matches++;
                                                break;
                                            }
                                            else if(results[j] == paste)
                                            {
                                                occurances[j]++;
                                                //cout << i << " " << occurances[k] << " " << results[k] << " is a repeat occurance" << endl;
                                                break;
                                            }
                                        }
                                    }
                                    //Assigns the string to results and increments counter by one. If the string already exists, increment counter and continue 
                                    paste.clear();
                                }
                                posinword++;
                            }
                        }
                        copy.clear();
                        linenumber++;//tells the script where in the sauce folder it is
                    }
                    inFS.close();
                    
                    //posinwork = 0;

                    cout << "Found " << matches << " unique items" <<  endl;
                    cout << "Type 0 to quit" << endl;
                    cout << "Type 1 to view by most popular" << endl;
                    cout << "Type 2 to view in alphabetical order" << endl;
                    cin >> functionselect;

                    do
                    {
                        for(i = 0; i < matches; i++)//creates a temporary container for the results so the original is unaltered
                        {
                            tempresults[i] = results[i];
                            tempoccurances[i] = occurances[i];
                        }
                        if(functionselect == 1)//popular case
                        {
                            i = occurances[0];//i is assigned as the max number
                            for(j = 0; j < matches; j++)//find max number in matches
                            {
                                if(occurances[j] > i)
                                {
                                    i = occurances[j];
                                }
                            }
                            cout << "Most popular item appears " << i << " time(s)" << endl;

                            for(k = i; k > 0; k = k-1)
                            {
                                for(j = 0; j < matches; j++)
                                {
                                    if(occurances[j] == k)
                                    {
                                        cout << k << " " << tempresults[j] << endl;
                                    }
                                }
                            }
                            
                            cin >> functionselect;
                            cout << "----------------------------------------" << endl;
                        }
                        else if(functionselect == 2)//alphabetical case
                        { 
                            for(i = 0; i < matches; i++)
                            {
                                tempstring = tempresults[i];//tempstring is assigned as the string with the fewest number of characters

                                for(j = 0; j < matches; j++)
                                {
                                    //min = results[j];
                                    //min = tempresults[j];    
                                    m = tempstring.length();//only compare as many characters that exist in the smaller item
                                    
                                    comparestring = tempresults[j];
                                    tempnumber = occurances[j];
                                
                                    if(tempstring == comparestring)//tells the script to move on when it attempts to compare min against itself
                                    {
                                        l = j;
                                        continue;
                                    }

                                    if(comparestring.length() < m)//grabs the string with the lower character count 
                                    {
                                        m = comparestring.length();
                                    }

                                    for(k = 0; k < m; k++)//compare each character in the string to find that with the lowest alphabetical placement
                                    {
                                        if(tempstring[k] == comparestring[k])
                                        {
                                            continue;
                                        }
                                        else if(tempstring[k] > comparestring[k])//if compare string was lower
                                        {
                                            tempstring = comparestring;
                                            tempoccurances[i] = tempnumber;
                                            l = j;
                                            break;
                                        }
                                        else if(tempstring[k] < comparestring[k])
                                        {
                                            break;
                                        }
                                    }
                                    results[i] = tempstring;
                                }
                                tempresults[l] = "zzzzzzzzzzzzzzzzzzzz";//replaces whatever is in tempresults at that moment with something that will always be higher than whatever is compared against it
                            }

                            for(i = 0; i < matches; i++)
                            {
                                cout << tempoccurances[i] << " " << results[i] << endl;
                            }

                            cin >> functionselect;
                            cout << "----------------------------------------" << endl;
                        }
                        else
                        {
                            cout << "Invalid input. Please select a valid command" << endl;
                            cin >> functionselect;
                        }
                    }while(functionselect !=0);
                    
                    for(i = 0; i < matches; i++)
                    {
                        results[i].clear();
                        occurances[i] = 0;
                    }
                    cout << "Type 0 to close the program" << endl << "Type 1 to scan input.txt" << endl << "Type 2 to use search function" << endl << "Type 3 to show more info about a certain work" << endl << "Type 4 to view a collection of the requested term" << endl << "Type 5 for help on using this program" << endl;
                }
            }
            else
            {
                cout << "Failed to open sauce.txt. Make sure the .txt file has the correct name and try again" << endl;
            }

            cin >> functionselect;
        }
        else if(functionselect == 5)//help screen
        {
            cout << "This script is intended to be used with nhentai.net only. Support for other sites might come later" << endl << endl;
            cout << "Using the upload function:" << endl; 
            cout << ">Copy the entry data from nhentai.net and save it to input.txt (an example of the full data is shown below)" << endl;
            cout << ">This program verifiably supports Firefox, Brave, and Chrome browsers but might work for others" << endl;
            cout << ">You can scan multiple entries at once. Just make sure to seperate each entry with a blank line" << endl;
            cout << ">Also, make sure there are TWO empty lines at the bottom of sauce.txt before scanning" << endl;
            cout << ">Use the scan function and the script will automatically save whatever is in input.txt" << endl;
            cout << ">The input is appended so no need to worry about overwriting anything" << endl;
            cout << ">Because of the aforementioned point, ALWAYS make sure to scan only once and clear input.txt afterwards" << endl;
            cout << ">If you don't, the search function will show repeats of the same work" << endl;
            cout << ">Numbers (such as 4 in 4uu or 2 in 2b) are erased. Manually readd them or they won't appear in search results" << endl;
            cout << endl << "Using the search function:" << endl; 
            cout << ">Enter whatever terms you want. Artist, tag, langauge, whatever" << endl;
            cout << ">To exclude a term, just enter '/' before it (ex: /hairy)" << endl;
            cout << ">The script will search through sauce.txt and return whatever codes match" << endl;
            cout << ">Only codes that match all search terms are displayed" << endl;
            cout << endl << "Show more info:" << endl; 
            cout << ">After searching, input the number corresponding to the code of a work to see the full entry as logged in sauce.txt" << endl;
            cout << ">You need to search beforehand. The program could search codes directly but I think it is more convenient this way" << endl;
            cout << ">Because of the way characters are stored, Japanese text will look screwy" << endl;
            cout << endl << "Show collections:" << endl;
            cout << ">Used to look at stats for a certain category (Arists, Tags, Characters, etc.)" << endl;
            cout << ">Tells the script to look for unique items in the specified category and collect them" << endl;
            cout << ">The results can be viewed by most popular or ascending alphabetically/page count" << endl;
            cout << endl << "Here's an example of sample input from the Firefox browser. Chrome and Brave look different but will also work" << endl;
            cout << R"(
[Jorori] Natsu to Jun | Summer and Innocence (COMIC HOTMILK 2019-01) [English] [Team Koinaka + Rotoscopic] [Digital]
[じょろり] 夏と純 (コミックホットミルク 2019年1月号) [英訳] [DL版]
#258133
Tags: sole female98Ksole male88Kdefloration28Kfull censorship19Ksweating12Kkimono6Kcondom6Kkissing5Kbeauty mark4K
Artists: jorori42
Languages: translated155Kenglish92K
Categories: manga83K
Pages: 22
Uploaded: 4 years, 12 months ago
)" << '\n';
            cin >> functionselect;
        }
        else
        {
            cout << "Invalid input. Please select a valid command" << endl;
            cin >> functionselect;
        }

    }while(functionselect != 0);

    return 0;
}
void cleanup2(string &source)
{
    int k;
    string tempstring;

    k = 0;
    tempstring = source;
    
    //cout << tempstring << endl;

    source.clear();
    tempstring = regex_replace(tempstring, std::regex(": "), "X");
    replace(tempstring.begin(), tempstring.end(), ' ', '_');
    replace(tempstring.begin(), tempstring.end(), 'K', ' ');
    tempstring = regex_replace(tempstring, std::regex("X"), ": ");

    while(tempstring[k] != '\0')//Scan through each character
    {
        if(isdigit(tempstring[k]))
        {
            source += ' ';
        }
        else
        {
            source += tempstring[k];
        }
        k++;
    }
    //cout << source << endl;
}
void removewhite(string &source)
{
    //Declarations
    string temp;
    //Initialisations
    temp = source;
    source.clear(); 
    
    temp = regex_replace(temp, std::regex("  "), " ");
    temp = regex_replace(temp, std::regex("  "), " ");

    source = temp;
}