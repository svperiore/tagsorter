//Doesn't seem to be able to read certain parts of the title (uncertain about this one)
//need to fix the n/a issue
//stats function. Shows total number of unique items in terms
//gallery to view full work from terminal
//create cut off for items that extend beyond a certain length
//Change how collections outputs things

#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <ctype.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <regex>
#include <cstdlib>

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
    int numexcepts;//number of terms to make exceptions for
    int check1;//a generic check
    int check2;//another generic check
    int searchwascalled;//a check used to make sure that a search was performed prior to calling show more info
    int matches;//total number of matches in search results
    int mokuteki;//same as matches. Used so that results between search function and collections do not mix 
    int pages;//the number of pages in a work
    int tempnumber;
    int occurances[2048];
    int tempoccurances[2048];

    const char* image;

    string copy;
    string paste;
    string tempstring;
    string tempcode;
    string tempname;
    string comparestring;
    string comment;//used to grab any comments made by the user
    string capitals = "PCTAGLCP";//the first letter of each term from nhentai in order of appearance
    string inputfilepath = "../tagsorter/input.txt";
    string saucefilepath = "../tagsorter/sauce.txt";
    string exfilepath = "../tagsorter/exceptions.txt";
    string terms[8] = {"Parodies:", "Characters:", "Tags:", "Artists:", "Groups:", "Languages:", "Categories:", "Pages:"};
    string results[2048];//Only used to display collections, not search results
    string tempresultsa[2048];
    string tempresultsp[2048];
    string codes[2048];//I just set these to some high number to be safe. Set them higher if you want idc
    string names[2048];
    string tempcodes[2048];
    string tempnames[2048];
    string exceptions[256];
    string replacements[256];
    string passterms[256];
    string temppassterms[256];
    string blockterms[256];
    string tempblockterms[256];
    string compareterms[256];
    string tempdisplay[12];

    //initialisations
    matches = 0;
    searchwascalled = 0;
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
            //initialisations
            posinwork = 0;
            posinword = 0;
            numofwork = 0;
            numexcepts = 0;
            linenumber = 0;
            i = 0;
            j = 0;
            k = 0;
            l = 0;
            m = 0;
            copy.clear();
            paste.clear();
            tempstring.clear();
            comment.clear();
            //operation
            inFS.open(exfilepath);
            while(!inFS.eof())//gather all exceptions from exceptions.txt
            {
                getline(inFS, copy);
                while(copy[posinword] != '\0')//Scan through each character
                {
                    if(copy[posinword] != ' ')
                    {
                        paste += copy[posinword];
                    }
                    else
                    {
                        exceptions[linenumber] = paste;
                        paste.clear();
                    }
                    posinword++;
                }  
                replacements[linenumber] = paste;
                paste.clear();
                posinword = 0;
                linenumber++;
            }
            inFS.close();
            numexcepts = linenumber;
            linenumber = -2;
            inFS.open(saucefilepath);
            while(!inFS.eof())//gather all codes from sauce.txt
            {
                getline(inFS, tempstring);
                
                if(linenumber == 0 || linenumber%13 == 0)
                {
                    codes[numofwork] = tempstring;
                    numofwork++;
                }
                linenumber++;
            }
            inFS.close();
            
            inFS.open(inputfilepath);
            ofFS.open(saucefilepath, ios::app);

            cout << "Job details..." << endl;

            while(!inFS.eof())//store all information in tempdisplay
            {
                check1 = 0;
                posinword = 0;
                getline(inFS, copy);
                if(posinwork == 0)
                {
                    for(posinwork; posinwork < 2; posinwork++)//Grab title
                    {        
                        if(copy[0] == '#')//fires if a secondary title is not found
                        {       
                            posinwork++;
                            tempdisplay[k] = "No secondary title available";
                            k++;
                            break;
                        }
                        tempdisplay[k] = copy;
                        getline(inFS, copy);
                        k++;
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
                    tempdisplay[k] = paste;
                    paste.clear();
                    copy.clear();
                    posinwork++;
                    k++;
                }
                if(posinwork == 3)//grabs everything else and arragnes it into one giant string
                {
                    if(copy[0] == 'U')//treats the "Uploaded" term as the limiter for an entry
                    {
                        getline(inFS, comment);//skips to the next entry
                        if(comment.empty() != true)
                        {
                            getline(inFS, paste);
                        }
                        posinwork++;
                        posinword = 0;
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
                                        tempdisplay[k] = paste;
                                        k++;
                                        break;
                                    }
                                    else
                                    {
                                        tempdisplay[k] = terms[j] + " n/a ";
                                        k++;
                                    }
                                }
                                paste.clear();
                                j++;
                            }
                        }
                        posinword++;
                    }
                    for(i = 0; i < numofwork; i++)//if the code does not match any codes from sauce.txt, then write. Else, skip
                    {
                        if(tempdisplay[2] == codes[i])
                        {
                            check1 = 1;
                        }
                    }
                    if(check1 == 1)
                    {
                        cout << "skipped " << tempdisplay[2] << " (already in sauce.txt!)" << endl;
                    }
                    else
                    {
                        for(i = 0; i < 12; i++)
                        {
                            //performs check for exceptions
                            posinword = 0;
                            j = 0;
                            if(i > 2 && i < 8)//only looks in Parodies through Groups 
                            {
                                copy = tempdisplay[i];
                                tempdisplay[i].clear();
                                while(copy[posinword] != '\0')//seperates each line into individual words and stores them in compareterms
                                {
                                    if(copy[posinword] == ' ')
                                    {
                                        compareterms[j] = paste;
                                        paste.clear();
                                        //posinword++;
                                        j++;
                                    }
                                    else
                                    {
                                        paste += copy[posinword];
                                    }
                                    posinword++;
                                }  
                                for(k = 0; k < numexcepts; k++)//compares each word from tempdislay against those gathered from exceptions.txt
                                {
                                    for(m = 0; m < j; m++)
                                    {
                                        if(compareterms[m] == exceptions[k])
                                        {
                                            compareterms[m] = replacements[k];
                                        }
                                    }
                                }
                                for(k = 0; k < j; k++)//reloads the line in tempdisplay after being scanned for exceptions
                                {
                                    tempdisplay[i]+=compareterms[k] + ' ';
                                    compareterms[k].clear();
                                }
                            }
                            if(i == 11)//omits an endline from the last input to allow for comments to be added to the same entry
                            {
                                ofFS << tempdisplay[i];
                            }
                            else
                            {
                                ofFS << tempdisplay[i] << endl;
                            }
                        }
                        if(comment.empty() == true)//adds the comment at the end of the entry if provided
                        {
                            ofFS << "No additional information provided" << endl;
                        }
                        else
                        {
                            ofFS << comment << endl;
                        }
                        ofFS << endl;
                        cout << "scanned " << tempdisplay[2] << endl;
                        l++;
                    }
                    for(i = 0; i < 12; i++)
                    {
                        tempdisplay[i].clear();
                    }
                    tempstring.clear();
                    posinwork = 0;
                    i = 0;
                    j = 0;
                    k = 0;
                }
            }
            inFS.close();
            ofFS.close();
            
            cout << l << " work(s) added to sauce.txt" << endl; 
            cin >> functionselect;
        }
        else if(functionselect == 2)//search function
        {
            //Initialisations
            searchwascalled = 1;
            linenumber = 0;
            numofwork = 0;
            numcompare = 0;
            posinword = 0;
            numpass = 0;
            numblock = 0;
            check1 = 0;    
            matches = 0;

            tempcode = "blank";
            tempstring = "blank";

            copy.clear();
            paste.clear();
            
            //script
            ifstream inFS;
            inFS.open(saucefilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input your search terms..." << endl;
             
                getline(cin.ignore(1,'\n'), copy);//grabs the search terms from the tag and arranges them into an array of strings
                copy += ' ';

                while(copy[posinword] != '\0')
                {
                    if(copy[posinword] == '/')//detects item to block 
                    {
                        check1 = 1;
                    }
                    else if(copy[posinword] != ' ')
                    {
                        paste += copy[posinword];
                    }
                    else
                    {
                        if(check1 == 0)//assign to passterms if not blocked item
                        {
                            passterms[numpass] = paste;
                            numpass++;
                        }
                        else//assign to blockterms if blocked item
                        {
                            check1 = 0;
                            blockterms[numblock] = paste;
                            numblock++;   
                        }
                        paste.clear();
                    }
                    posinword++;
                }
                check1 = 0;

                while(!inFS.eof())
                {
                    //more intialisations
                    posinword = 0;  
                    numcompare = 0;
                    numofwork = linenumber/13;
                    
                    copy.clear();
                    paste.clear();                    
                    //Used for troubleshooting. Feel free to ignore
                    //cout << "numofwork = " << numofwork << ", linenumber = " << linenumber << ", check1 = " << check1 << ", matches = " << matches << ", tempcode = " << tempcode << endl;
                    
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
                    
                    if(linenumber%13 == 0)//This reloads the pass and block terms after having been deleted in the search. Only needs to be performed once per work
                    {
                        for(i = 0; i < numpass; i++)
                        {
                            temppassterms[i] = passterms[i];
                        }
                        for(i = 0; i < numblock; i++)
                        {
                            tempblockterms[i] = blockterms[i];
                        }
                    }
                    if(linenumber == 13*numofwork)
                    {
                        tempname = copy;
                    }
                    if(linenumber == 2+(13*numofwork))//grabs the code and stores it into tempcode. Can be recalled if the requested terms are in this code
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
                                check1++;
                            }
                        }
                        for(j = 0; j < numblock; j++)
                        {
                            if(blockterms[j] == compareterms[i])//blocked item is found, decrease counter by 1
                            {       
                                tempblockterms[j] = "PLACEHOLDER";
                                check1 = check1 - 1;
                            }
                        }
                    }             
                    
                    linenumber++;//tells the script where in the sauce folder it is
               
                    if(linenumber%13 == 0)//resets the number of found passterms
                    {
                        if(check1 == numpass)//If all passterms have been found in an entry, the tempcode is saved to codes[256] to be presented to the user later
                        {
                            codes[matches] = tempcode;
                            names[matches] = tempname;
                            matches++;
                        }     
                        check1 = 0;
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

                cout << "Found " << matches << " match(es) out of " << numofwork << " total work(s)..." << endl;

                if(matches > 0)
                {
                    for(i = 1; i < matches+1; i++)
                    {
                        tempcodes[i] = codes[i-1];
                        tempnames[i] = names[i-1];
                        cout << setw(4) << left << i << setw(7) << left << tempcodes[i] << tempnames[i] << endl;
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
            check1 = 0;
            check2 = 0;
            posinword = 0;
            pages = 0;
            tempstring.clear();
            tempcode.clear();
            paste.clear();

            /*
            if(searchwascalled == 1 && matches > 0)
            {
                for(i = 1; i < matches+1; i++)
                {
                    cout << setw(4) << left << i << setw(7) << left << tempcodes[i] << tempnames[i] << endl;
                }
            }
            */

            inFS.open(saucefilepath);
            if(inFS.is_open())//Opens file
            {
                cout << "Perform a search to collect selectable codes OR type 0 to manually search by code..." << endl;
                cin >> i;
                
                if(i > matches || i < 0)
                {
                    check1 = 0;
                    cout << "Invalid request" << endl; 
                }
                else if(i == 0)
                {
                    cout << "Enter the code of the desired work..." << endl;
                    cin >> tempcodes[i];
                    check1 = 1;
                }
                else
                {
                    check1 = 1;
                }
                if(check1 > 0)
                {
                    while(!inFS.eof())
                    {
                        for(posinwork = 0; posinwork < 13; posinwork++)
                        {
                            getline(inFS, copy);    
                            tempdisplay[posinwork] = copy;    
                        }
                        if(tempdisplay[2] == tempcodes[i])
                        {
                            check2 = 1;//the code has been found
    
                            for(posinwork = 0; posinwork < 12; posinwork++)
                            {
                                cout << "        " << tempdisplay[posinwork] << endl;
                            }
                            //grabs the page count to be used to output a cover
                            copy = tempdisplay[10];
                            
                            while(copy[posinword] != '\0')//grab the page count and convert to int
                            {
                                if(isdigit(copy[posinword]) == true)
                                {
                                    paste+=copy[posinword];
                                }
                                posinword++;
                            }
                            pages = stoi(paste);
                            paste.clear();
                            if(pages/1000 >= 1)
                            {
                                paste = "/0001.";
                            }
                            else if(pages/100 >= 1)
                            {
                                paste = "/001.";
                            }
                            else if(pages/10 >= 1)
                            {
                                paste = "/01.";
                            }
                            else
                            {
                                paste = "/1.";
                            }
                            tempstring = "D:/sebas/Documents/tagsorter/images/" + tempdisplay[2] + paste + "jpg"; 
                            image = tempstring.c_str();
                            system(image);
                            tempstring.clear();
                            tempstring = "D:/sebas/Documents/tagsorter/images/" + tempdisplay[2] + paste + "png";
                            image = tempstring.c_str();
                            system(image);
                            
                            cout << "----------------------------------------" << endl;
                        }
                        else
                        {
                            for(posinwork = 0; posinwork < 13; posinwork++)
                            {
                                tempdisplay[posinwork].clear();
                            }
                        }              
                    }
                    if(check2 == 0)//only fires if the code was not found in the previous search
                    {
                        cout << "Failed to find " << tempcodes[0] << " in sauce.txt" << endl;
                    }
                }
                inFS.close();
            }
            cin >> functionselect;
        }
        else if(functionselect == 4)//show collections
        {
            //Initialisations
            i = 0;
            j = 0;
            k = 0;
            l = 0;
            m = 0;
            mokuteki = 0;
            linenumber = 0;
            posinwork = 0;
            numofwork = 0;
            check1 = 0;

            copy.clear();
            tempstring.clear();

            //script
            ifstream inFS;
            inFS.open(saucefilepath);

            if(inFS.is_open())//opens file
            {
                cout << "Input a term (Tags, Parodies, Artists, etc.)..." << endl;
             
                getline(cin.ignore(1,'\n'), tempstring);//grabs the search terms from the tag and arranges them into an array of strings
                tempstring += ':';
                
                for(i = 0; i < 8; i++)
                {                    
                    if(tempstring == terms[i])
                    {
                        posinwork = i + 3;//tells the program which lines to scan and which to skip
                        check1 = 1;
                        break;
                    }
                    else if(i == 7)//no match was found for the input. Break out from the function
                    {
                        cout << "Invalid term. Input something valid or check your spelling" << endl;
                        check1 = 0;
                    }
                }

                if(check1 == 1)
                {
                    while(!inFS.eof())
                    {
                        numofwork = linenumber/13;
                        posinword = 0;

                        getline(inFS, copy);//scans the sauce folder line by line, arranging terms into another array of strings 
                    
                        if(linenumber == posinwork+(13*numofwork))
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
                                        for(j = 0; j <= mokuteki; j++)
                                        {
                                            if(j == mokuteki)
                                            {
                                                results[j] = paste;
                                                occurances[j]++;
                                                mokuteki++;
                                                break;
                                            }
                                            else if(results[j] == paste)
                                            {
                                                occurances[j]++;
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

                    cout << "Found " << mokuteki << " unique items across " << numofwork << " work(s)..." << endl;

                    for(i = 0; i < mokuteki; i++)//creates a temporary container for the results so the original is unaltered
                    {
                        tempresultsa[i] = results[i];
                        tempresultsp[i] = results[i];
                        tempoccurances[i] = occurances[i];
                    }

                    i = occurances[0];//i is assigned as the max number
                    for(j = 0; j < mokuteki; j++)//find max number in mokuteki
                    {
                        if(occurances[j] > i)
                        {
                            i = occurances[j];
                        }
                    }
                    for(i = 0; i < mokuteki; i++)
                    {
                        tempstring = tempresultsa[i];//tempstring is assigned as the string with the fewest number of characters

                        for(j = 0; j < mokuteki; j++)
                        {
                            m = tempstring.length();//only compare as many characters that exist in the smaller item
                            
                            comparestring = tempresultsa[j];
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
                        tempresultsa[l] = "zzzzzzzzzzzzzzzzzzzz";//replaces whatever is in tempresults at that moment with something that will always be higher than whatever is compared against it
                    }
                    for(k = i; k > 0; k = k-1)
                    {
                        for(j = 0; j < mokuteki; j++)
                        {
                            if(occurances[j] == k)
                            {
                                cout << setw(4) << left << k << tempresultsp[j] << endl;
                            }
                        }
                    }
                    for(i = 0; i < mokuteki; i++)
                    {
                        cout << setw(4) << left << tempoccurances[i] << results[i] << endl;
                    }

                    for(i = 0; i < mokuteki; i++)
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
            cout << endl<< "This script is intended to be used with nhentai.net only. Support for other sites might come later" << endl << endl;
            cout << "Using the upload function..." << endl; 
            cout << ">Copy the entry data from nhentai.net and save it to input.txt (an example of the full data is shown below)" << endl;
            cout << ">This program verifiably supports Firefox, Brave, and Chrome browsers but might work for others" << endl;
            cout << ">You can scan multiple entries at once. Just make sure to seperate each entry with a blank line" << endl;
            cout << ">Also, make sure there are TWO empty lines at the bottom of sauce.txt before scanning" << endl;
            cout << ">Use the scan function and the script will automatically scan whatever is in input.txt" << endl;
            cout << ">It's good practice to clear input.txt afterwards. If you don't, things can get kinda messy" << endl;
            cout << ">Numbers (such as 4 in 4uu or 2 in 2b) are erased. Manually readd them or they won't appear in search results" << endl;
            cout << ">Alternatively, add the item to exceptions.txt to do this automatically. Use this convention: persona_ persona_3" << endl;
            cout << ">You can also add whatever comments you want to work at the bottom of each entry" << endl;
            cout << endl << "Using the search function..." << endl; 
            cout << ">Enter whatever terms you want. Artist, tag, langauge, whatever" << endl;
            cout << ">To exclude a term, just enter '/' before it (ex: /hairy)" << endl;
            cout << ">The script will search through sauce.txt and return whatever codes match" << endl;
            cout << ">Only codes that match all search terms are displayed" << endl;
            cout << endl << "Show more info..." << endl; 
            cout << ">After a search, input the number corresponding to the code of a work to see the full entry as logged in sauce.txt" << endl;
            cout << ">You can also type 0 to make a manual search. This will display the info of whatever work you type the code of" << endl;
            cout << ">Because of the way characters are stored, Japanese text will look screwy (can't be bothered to fix this atm :P )" << endl;
            cout << endl << "Show collections..." << endl;
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
this is where you can add comments. You can just ignore this if you have nothing to add
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