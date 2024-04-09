//Doesn't seem to be able to read certain parts of the title
//Would be neat if we could see the names (or codes) of the works after scanning
//need to fix the n/a issue
//repeat issue in search terms (use temporary requested terms to fix)
//would be nice to show the total amount of works everytime categories is called
//need to clean up variables

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
    //Declarations
    int a;//Used for making selections on the menu
    int b;//Used as generic integers
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    int k;
    int l;
    int max;
    int lowchars;
    int tempnumber;
    int lengths[7] = {10, 12, 6, 9, 8, 11, 12};//the length of each item. Used to tell the scanner how many characters to skip
    int locations[7] = {69, 69, 69, 69, 69, 69, 69};//the location that each item occurs at in input.txt. Initialised at 69 (nice) to avoid null issues 
    int occurances[2048];
    int tempoccurances[2048];

    string min;
    string copy;
    string paste;
    string term;
    string vitems[8];
    string sitems[8] = {"Parodies:", "Characters:", "Tags:", "Artists:", "Groups:", "Languages:", "Categories:", "Pages:"};
    string tempstring;
    string tempstring2;
    string tempcode;
    string tempcode2;
    string comparestring;
    string results[2048];//Only used to display collections, not search results

    string presults[2048];
    string aresults[2048];

    string tempcodes[256];
    string searchterms[256];//I just set these to 256 to be safe. Set them higher if you want idc
    string tempsearchterms[256];
    string blockterms[256];
    string tempblockterms[256];
    string compareterms[256];
    string codes[256];
    string tempdisplay[11];

    //Initialisations
    fill(occurances, occurances + 2048, 0);

    //Script
    string infilepath = "../tagsorter/input.txt";//Check to see that files are present
    string offilepath = "../tagsorter/sauce.txt";

    ifstream inFS;
    ofstream ofFS;

    inFS.open(infilepath);
    ofFS.open(offilepath, ios::app);

    if(inFS.is_open() && ofFS.is_open())//Opens file
    {
        cout << "Successfully opened both input.txt and sauce.txt" << endl;            
    }
    else
    {
        cout << "Failed to open input.txt or sauce.txt. Make sure the .txt files have the correct name and try again" << endl;
    }  
    
    cout << "Type 0 to close the program" << endl << "Type 1 to scan input.txt" << endl << "Type 2 to use search function" << endl << "Type 3 to show more info about a certain work" << endl << "Type 4 to view a collection of the requested item" << endl << "Type 5 for help on using this program" << endl;
    cin >> a;
    
    do{
        if(a == 0)//Close program
        {
            return 0;
        }
        else if(a == 1)//read input.txt
        {
            tempcode = "PCTAGL";
            
            string infilepath = "../tagsorter/input.txt";
            string offilepath = "../tagsorter/sauce.txt";

            ifstream inFS;
            ofstream ofFS;

            inFS.open(infilepath);
            ofFS.open(offilepath, ios::app);

            if(inFS.is_open() && ofFS.is_open())//Opens file
            {
                //Initialisations
                h = 0;
                i = 0;
                j = 0;
                k = 0;
                l = 0;

                copy.clear();
                paste.clear();

                //Extract information about files
        
                while(!inFS.eof())
                {
                    k = 0;

                    getline(inFS, copy);

                    if(i == 0)
                    {
                        for(i; i < 2; i++)//Grab title
                        {        
                            if(copy[0] == '#')
                            {       
                                i++;

                                ofFS << "No secondary title available" << endl;

                                break;
                            }
                            ofFS << copy << endl;

                            getline(inFS, copy);
                        }
                    }
                    if(i == 2 && copy[0] == '#')//Found the code and grab
                    {
                        while(copy[k] != '\0')//Scan through each character
                        {
                            if(copy[k] != '#')
                            {
                                paste += copy[k];
                            }
                            k++;
                        }
                        
                        ofFS << paste << endl;

                        paste.clear();
                        copy.clear();        
                        i++;
                    }
                    if(i == 3)//grabs everything else and arragnes it into one giant string
                    {
                        if(copy[0] == 'U')//treats the "Uploaded" term as the limiter for an entry
                        {
                            getline(inFS, paste);//skips to the next entry
                            i = 4;
                            k = 0;
                            h++;
                        }
                        else
                        {
                            tempstring += copy;
                        }
                    }
                    if(i == 4)//arranges everything that starts with a capital letter into a new line
                    {
                        paste.clear();
                        tempcode2 = "PCTAGLCP";//can honestly just use tempcode for this

                        while(tempstring[k] != '\0')
                        {
                            paste += tempstring[k];
                            for(j = 0; j < 7; j++)
                            {
                                if(tempstring[k+1] == tempcode[j])
                                {
                                    for(l; l < 8; l++)
                                    {
                                        if(paste[0] == tempcode2[l])
                                        {
                                            if(l != 7)//removes numbers and K but preserves page count
                                            {
                                                cleanup2(paste);
                                                removewhite(paste);
                                            }
                                            else
                                            {   
                                                paste+= ' ';
                                            }
                                            vitems[l] = paste;
                                            ofFS << vitems[l] << endl;
                                            break;
                                        }
                                        else
                                        {
                                            vitems[l] = "placeholder yeah";
                                            ofFS << sitems[l] << " n/a " << endl;
                                        }
                                    }
                                    paste.clear();
                                    l++;
                                }
                            }
                            k++;
                        }
                        ofFS << endl;
                        tempstring.clear();
                        i = 0;
                        l = 0;
                    }
                    k = 0;
                }
                inFS.close();
                ofFS.close();
                
                cout << "Successfully scanned " << h << " work(s)" << endl; 
                cout << "Finished writing to sauce.txt" << endl;
            }
            else
            {
                cout << "Failed to open input.txt or sauce.txt. Make sure the .txt files have the correct name and try again" << endl;
            }  
            cin >> a;
        }
        else if(a == 2)//search function
        {
            //Initialisations
            i = 0;
            j = 0;
            k = 0;
            d = 0;
            e = 0;
            f = 0;
            g = 0;
            tempcode = "blank";
            tempstring = "blank";

            copy.clear();
            paste.clear();
            
            //script
            string infilepath = "../tagsorter/sauce.txt";
            ifstream inFS;
            inFS.open(infilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input your search terms:" << endl;
             
                getline(cin.ignore(1,'\n'), copy);//grabs the search terms from the tag and arranges them into an array of strings
                copy += ' ';

                while(copy[k] != '\0')
                {
                    if(copy[k] == '/')//detects item to block 
                    {
                        f = 1;
                    }
                    else if(copy[k] != ' ')
                    {
                        paste += copy[k];
                    }
                    else
                    {
                        tempstring = paste;
                        
                        if(f == 0)//assign to searchterms if not blocked item
                        {
                            searchterms[i] = tempstring;
                            tempsearchterms[i] = tempstring;
                            i++;
                        }
                        else//assign to blockterms if blocked item
                        {
                            f = 0;
                            blockterms[g] = tempstring;
                            tempblockterms[g] = tempstring;
                            g++;   
                        }
                        paste.clear();
                    }
                    k++;
                }

                h = g;//size of blockterms
                k = i;//size of searchterms
                c = 0;
                f = 0;
                g = 0;
                
                while(!inFS.eof())
                {
                    //More intialisations
                    i = 0;
                    j = 0;
                    e = d/12;//Used to say which entry in the sauce folder the script is looking at
                    
                    //Used for troubleshooting. Feel free to ignore
                    //cout << "e = " << e << ", d = " << d << ", f = " << f << ", c = " << c << ", k = " << k << ", g = " << g << ", tempcode = " << tempcode << ", " << endl;
                    

                    copy.clear();
                    paste.clear();
                    
                    getline(inFS, copy);//scans the sauce folder line by line, arranging terms into another array of strings 
                    
                    while(copy[j] != '\0')//assign each item from each line in sauce to compareterms
                    {
                        if(copy[j] != ' ')
                        {
                            paste += copy[j];
                        }
                        else
                        {
                            tempstring = paste;
                            compareterms[i] = tempstring;
                            i++;
                            paste.clear();
                        }
                        j++;
                    }

                    tempstring = paste;
                    compareterms[i] = tempstring;//i is now size of compareterms
                    
                    for(b = 0; b < k; b++)//This 'reloads' the search and block terms after having been deleted in the search
                    {
                        searchterms[b] = tempsearchterms[b];
                    }
                    for(b = 0; b < h; b++)
                    {
                        blockterms[b] = tempblockterms[b];
                    }

                    if(d == 2+(12*e))//grabs the code and stores it into tempcode. Can be recalled if the requested terms are in this code
                    {
                        tempcode = tempstring;
                    }

                    for(j = 0; j <= i; j++)//for each item in compareterms, check to see if anything from searchterms is present
                    {
                        for(b = 0; b < k; b++)
                        {
                            if(searchterms[b] == compareterms[j])//match is found, advance counter by 1
                            {       
                                //searchterms[b] = "SUTEKI";//This avoids the issue where a double occurance of a term in the same line activates the counter
                                c++;
                            }
                        }
                        for(b = 0; b < h; b++)
                        {
                            if(blockterms[b] == compareterms[j])//blocked item is found, decrease counter by 1
                            {       
                                //blockterms[b] = "SUTEKI";
                                c = c - 1;
                            }
                        }
                    }             
                    
                    d++;//tells the script where in the sauce folder it is
               
                    if(d%12 == 0)//resets the number of found searchterms
                    {
                        if(c == k)//If all searchterms have been found in an entry, the tempcode is saved to codes[256] to be presented to the user later
                        {
                            codes[g] = tempcode;
                            g++;
                        }     
                        c = 0;
                    }
                }
                inFS.close();

                for(b = 0; b < k; b++)
                {
                    searchterms[b].clear();
                }
                for(l = 0; l < h; l++)
                {
                    blockterms[b].clear();
                }

                cout << "Found " << g << " works that match the requested terms";

                if(g > 0)
                {
                    cout << ":" << endl;

                    for(i = 0; i < g; i++)
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

            cin >> a;
        }
        else if(a == 3)//show more info 
        {
            //Initialisations
            j = 0;
            k = 0;

            for(i = 0; i < g; i++)
            {
                cout << i + 1 << ") " << tempcodes[i] << endl;
            }

            string infilepath = "../tagsorter/sauce.txt";
            ifstream inFS;
            inFS.open(infilepath);

            if(inFS.is_open())//Opens file
            {
                cout << "Input the number corresponding to the code from the previous search query: " << endl;
                cin >> j;

                if(j > g || j <= 0)
                {
                    k = 0;
                    cout << "Invalid request. You need to select one of the listed codes" << endl; 
                }
                else
                {
                    k = 1;
                }

                if(k == 1)
                {
                    while(!inFS.eof())
                    {
                        for(i = 0; i < 12; i++)
                        {
                            getline(inFS, copy);    
                            tempdisplay[i] = copy;    
                        }
                        if(tempdisplay[2] == tempcodes[j-1])
                        {
                            for(i = 0; i < 11; i++)
                            {
                                cout << tempdisplay[i] << endl;
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

            cin >> a;
        }
        else if(a == 4)//show collections
        {
            //Initialisations
            copy.clear();
            term.clear();
            tempstring.clear();
            h = 0;
            i = 0;
            j = 0;
            k = 0;
            l = 0;
            g = 0;
            d = 0;
            f = 0;
            
            //script
            string infilepath = "../tagsorter/sauce.txt";
            ifstream inFS;
            inFS.open(infilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input a term (Tags, Parodies, Artists, etc.):" << endl;
             
                getline(cin.ignore(1,'\n'), term);//grabs the search terms from the tag and arranges them into an array of strings
                tempstring2 = term;
                term += ':';
                
                for(i = 0; i < 8; i++)
                {
                    tempstring = sitems[i];
                    
                    if(term == tempstring)
                    {
                        h = i + 3;//tells the program which lines to scan and which to skip
                        g = 1;
                        break;
                    }
                    else if(i == 7)//no match was found for the input. Break out from the function
                    {
                        tempstring.clear();
                        cout << "Invalid term. Input something valid or check your spelling" << endl;
                        g = 0;
                    }
                }
                if(g == 1)
                {
                    //cout << "Term = " << term << ", tempstring = " << tempstring << ", h = " << h << ", d = " << d << endl;

                    i = 0;

                    while(!inFS.eof())
                    {
                        e = d/12;
                        j = 0;

                        getline(inFS, copy);//scans the sauce folder line by line, arranging terms into another array of strings 
                        
                        //cout << "e = " << e << ", d = " << d << ", check = " << h+(12*e);

                        if(d == h+(12*e))
                        {   
                            while(copy[j] != '\0')//assign each item from each line in sauce to compareterms
                            {
                                if(copy[j] != ' ')
                                {
                                    paste += copy[j];
                                }
                                else
                                {
                                    if(paste[0] != term[0])
                                    {
                                        tempstring = paste;
                                        //cout << " " << tempstring;
                                        for(k = 0; k <= i; k++)
                                        {
                                            if(k == i)
                                            {
                                                results[k] = tempstring;
                                                occurances[k]++;
                                                //cout << i << " " << occurances[k] << " " << results[k] << endl;
                                                i++;
                                                break;
                                            }
                                            else if(results[k] == tempstring)
                                            {
                                                occurances[k]++;
                                                //cout << i << " " << occurances[k] << " " << results[k] << " is a repeat occurance" << endl;
                                                break;
                                            }
                                        }
                                    }
                                    //Assigns the string to results and increments counter by one. If the string already exists, increment counter and continue 
                                    paste.clear();
                                }
                                j++;
                            }
                        }
                        copy.clear();
                        d++;//tells the script where in the sauce folder it is
                    }
                    inFS.close();
                    
                    j = 0;
                    h = 0;

                    for(b = 0; b < i; b++)//creates a temporary container for the results so the original is unaltered
                    {
                        aresults[b] = results[b];
                        presults[b] = results[b];
                        tempoccurances[b] = occurances[b];
                    }

                    cout << "Found " << i << " unique items in " << tempstring2 << endl;
                    cout << "Type 0 to quit" << endl;
                    cout << "Type 1 to view by most popular" << endl;
                    cout << "Type 2 to view in alphabetical order" << endl;
                    cin >> l;

                    do
                    {
                        if(l == 1)//popular case
                        {
                            max = occurances[0];
                            for(j = 0; j < i; j++)//find max number
                            {
                                if(occurances[j] > max)
                                {
                                    max = occurances[j];
                                }
                            }
                            cout << "Most popular item appears " << max << " time(s)" << endl;

                            for(b = max; b > 0; b = b-1)
                            {
                                for(j = 0; j < i; j++)
                                {
                                    if(occurances[j] == b)
                                    {
                                        cout << b << " " << presults[j] << endl;
                                    }
                                }
                            }
                            
                            cin >> l;
                            cout << "----------------------------------------" << endl;
                        }
                        else if(l == 2)//alphabetical case
                        { 
                            for(j = 0; j < i; j++)
                            {
                                min = aresults[j];

                                for(h = 0; h < i; h++)
                                {
                                    //min = results[j];
                                    //min = tempresults[j];    
                                    lowchars = min.length();//only compare as many characters that exist in the smaller item
                                    
                                    comparestring = aresults[h];
                                    tempnumber = occurances[h];
                                
                                    if(min == comparestring)//tells the script to move on when it attempts to compare min against itself
                                    {
                                        f = h;
                                        continue;
                                    }

                                    if(comparestring.length() < lowchars)//grabs the string with the lower character count 
                                    {
                                        lowchars = comparestring.length();
                                    }

                                    for(b = 0; b < lowchars; b++)//compare each character in the string to find that with the lowest alphabetical placement
                                    {
                                        if(min[b] == comparestring[b])
                                        {
                                            continue;
                                        }
                                        else if(min[b] > comparestring[b])//if compare string was lower
                                        {
                                            min = comparestring;
                                            tempoccurances[j] = tempnumber;
                                            f = h;
                                            break;
                                        }
                                        else if(min[b] < comparestring[b])
                                        {
                                            break;
                                        }
                                        //might need to account for a case like "anal" and "anal_intercourse", where both will be the same when scanned lowchars many times
                                    }
                                    results[j] = min;
                                }
                                aresults[f] = "zzzzzzzzzzzzzzzzzzzz";//replaces whatever is in tempresults at that moment with something that will always be higher than whatever is compared against it
                            }

                            for(j = 0; j < i; j++)
                            {
                                cout << tempoccurances[j] << " " << results[j] << endl;
                            }

                            cin >> l;
                            cout << "----------------------------------------" << endl;
                        }
                        else
                        {
                            cout << "Invalid input. Please select a valid command" << endl;
                            cin >> l;
                        }
                    }while(l !=0 );
                    
                    for(j = 0; j < i; j++)
                    {
                        results[j].clear();
                        occurances[j] = 0;
                    }

                    cout << "Type 0 to close the program" << endl << "Type 1 to scan input.txt" << endl << "Type 2 to use search function" << endl << "Type 3 to show more info about a certain work" << endl << "Type 4 to view a collection of the requested item" << endl << "Type 5 for help on using this program" << endl;

                    for(b = 0; b < i; b++)//clears results and occruances to avoid overwriting issues
                    {
                        results[b].clear();
                        occurances[b] = 0;
                    }
                }
            }
            else
            {
                cout << "Failed to open sauce.txt. Make sure the .txt file has the correct name and try again" << endl;
            }

            cin >> a;
        }
        else if(a == 5)//help screen
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
            cout << endl << "Using the search function:" << endl; 
            cout << ">Enter whatever terms you want. Artist, tag, langauge, whatever" << endl;
            cout << ">To exclude a term, just enter '/' before it (ex: /hairy)" << endl;
            cout << ">The script will search through sauce.txt and return whatever codes match" << endl;
            cout << ">Only codes that match all search terms are displayed" << endl;
            cout << ">With the input function, numbers in words (like '2' in '2b') are deleted. You can manually re-add them though" << endl;
            cout << endl << "Show more info:" << endl; 
            cout << ">After searching, input the number corresponding to the code of a work to see the full entry as logged in sauce.txt" << endl;
            cout << ">You need to search beforehand. The program could search codes directly but I think it is more convenient this way" << endl;
            cout << ">Because of the way characters are stored, Japanese text will look screwy" << endl;
            cout << endl << "Show collections:" << endl;
            cout << ">Used to look at stats for a certain category (Arists, Tags, Characters, etc.)" << endl;
            cout << ">Tells the script to look for unique items in the specified category and collect them" << endl;
            cout << ">The results can be viewed by most popular or ascending alphabetically/page count" << endl;
            cout << endl << R"(
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
            cin >> a;
        }
        else
        {
            cout << "Invalid input. Please select a valid command" << endl;
            cin >> a;
        }

    }while(a != 0);

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