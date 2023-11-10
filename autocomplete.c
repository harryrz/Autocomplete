#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;

int sortByName(const void* term1, const void* term2){
    term* t1 = (term*)term1;
    term* t2 = (term*)term2;
    return strcmp(t1 -> term, t2 -> term);
}

int sortByWeight(const void *term1, const void *term2){
    term *t1 = (term *)term1;
    term *t2 = (term *)term2;
    double t1Weight = t1 -> weight;
    double t2Weight = t2 -> weight;
    if(t2Weight > t1Weight){
        return 1;
    } else if(t2Weight < t1Weight) {
        return -1;
    } else {
        return 0;
    }
}

//test function, run through whole term to see if arrange in order
void printTerms(term* t, int numTerms){
    for(int i = 0; i < numTerms; i++){
        printf("%f, %s\n", (*(t+i)).weight, (*(t+i)).term);
    }
}

void read_in_terms(term **terms, int *pnterms, char *filename){

    FILE* cities = fopen(filename, "r");
    char cityLoc[500];      //hard code, don't know length
    char tempCityNum[1000];  //hard code, don't know length of first line
    char cityWeight[500];    //hard code don't know length 
    //int weightInt;
    double weightD;
    char ch;
    char chStr[2] = "\0";

    
    //obtain number of terms
    fgets(tempCityNum, 1000, cities);
    int cityNum = atoi(tempCityNum);
    *pnterms = cityNum;
    
    //printf("%d\n", *pnterms);

    
    //allocate objects in sturct term
    *terms = (term*)malloc(sizeof(term) * cityNum);


    int counter = 0;
    
    //obtain each term (alphabet)
    while ((ch = fgetc(cities)) != EOF) {
        
        cityWeight[0] = '\0';
        if(isdigit(ch)){
            chStr[0] = ch;
            strncat(cityWeight, chStr, 1);
            while(!isblank((ch = fgetc(cities)))){
                chStr[0] = ch;
                strncat(cityWeight, chStr, 1);
            }
            weightD = atof(cityWeight);
            //weightD = (double)weightInt;
            (*terms + counter) -> weight = weightD;

            //printf("%f\n", weightD);
            cityWeight[0] = '\0';

            //double tempWeight = ch - '0';
            //printf("%d", ch - '0');
        }
        
        cityLoc[0] = '\0';
        //NOTE term could contain any special character not only alphabets
        if((ch != '\t') && (!isspace(ch)) && (!isdigit(ch))){
            //printf("%c",ch);
            chStr[0] = ch;
            strncat(cityLoc, chStr, 1);
            while((ch = fgetc(cities)) != '\n' && ch != EOF){
                //printf("%c",ch);
                chStr[0] = ch;
                strncat(cityLoc, chStr, 1);
            }

            strcpy((*terms + counter) -> term, cityLoc);
                        
            counter += 1; 
            //printf("%s\n", cityLoc);
            cityLoc[0] = '\0'; //empty string  
        }

        












    }
    
    
    
    //printf("%s\n", (*((*terms)+0)).term);
    //printf("%f", (*((*terms)+12)).weight);
    
    //*terms = (term *)malloc(sizeof(term));
    //*(terms).term = (char *)malloc(sizeof(char)*200);


    //Sorting part
    qsort(*terms, *pnterms, sizeof(term), &sortByName);
    
    
    fclose(cities);
}


int lowest_match(term* terms, int nterms, char* subStr){
    int low = 0; 
    int high = nterms-1;
    int mid;
    //Assume term always exist
    while (low <= high){
        mid = floor((high+low)/2);
        //printf("%s\n", (terms+mid) -> term);



       




        /**/
        /*
        if(mid-1 ==82784 ){
            
            printf("%s\n", (terms+0)->term);
            printf("%s\n", (terms+82641)->term);
            printf("%s\n", (terms+82782)->term);
            
        

            
        }*/

        //two situation, one is the first element
        if(mid == 0 && strncmp(subStr, (terms+mid) -> term, strlen(subStr)) == 0){
            return mid;
        }
        //second is not the first element but first occurance, order is important here (mid -1 may be undefined)
        else if(strncmp(subStr, (terms+mid) -> term, strlen(subStr)) == 0 && strncmp(subStr, (terms+mid-1) -> term, strlen(subStr)) > 0) {
            return mid;
        }
        //MUST LOW first!!!!!, because we want the first appearance, therefore even if current term does match substring, we still wanna adjust the lower bound to find the first one (that's why it's in else). 
        else if(strncmp(subStr, (terms+mid) -> term, strlen(subStr)) > 0){
            low = mid + 1;
        } else {
            high = mid - 1;
        }   
       
    }
    
    //here there's no such substring in list
    return -10000; 
    
}


int highest_match(term* terms, int nterms, char* subStr){
    int low = 0; 
    int high = nterms-1;
    int mid;
    //Assume term always exist
    while (low <= high){
        mid = floor((high+low)/2);

        
        /*
        printf("%s\n", (terms+mid-3) -> term);
        printf("%s\n", (terms+mid-2) -> term);
        printf("%s\n", (terms+mid-1) -> term);
        */
        //printf("%s\n", (terms+mid) -> term);
        //printf("mid = %d\n", mid);
        //if (mid -1 == 82784){
            //printf("mid-1=%d\n", mid-1);
       // }

        //two situation, one is if cur element is last element  
        if(mid == nterms-1 && strncmp(subStr, (terms+mid) -> term, strlen(subStr)) == 0){
            return mid;
        }
        //second is not the last element but last occurance, order is important here (as mid+1 may be undefined)
        //Error here
        else if(strncmp(subStr, (terms+mid) -> term, strlen(subStr)) == 0 && strncmp(subStr, (terms+mid+1) -> term, strlen(subStr)) < 0) {
            return mid;
        }
        //MUST HIGH first!!!!!, because we'd like to get the least appearance, therefore if all previous condition don't meet, we have to manually make low = mid + 1 
        //if((mid -1) == 82784){
           
        //}
        else if(strncmp(subStr, (terms+mid) -> term, strlen(subStr)) < 0){
            high = mid - 1;
        } else {
            low = mid + 1;
        }   
    }
    
    //here there's no such substring in list
    return -10000; 
    
}

void autocomplete(term** answer, int* n_answer, term *terms, int nterms, char* substr){
    int low = lowest_match(terms, nterms, substr);
    //printf("first appearance = %d\n",low);
    int high = highest_match(terms, nterms, substr);
    //printf("last appearance = %d\n",high);

    if(low != -10000 && high != -10000) {
        *n_answer = high - low + 1;
        *answer = (term *)malloc(sizeof(term) * (*n_answer));
        for(int i = 0; i < (*n_answer); i++){
            strcpy((*answer + i)->term, (terms + low + i)->term);
            (*answer + i)->weight = (terms + low + i)->weight;
        }

    }

    qsort(*answer, *n_answer, sizeof(term), &sortByWeight);

}


/*
int main() {
    char name[] = "cities2.txt";
    term* cityTerm;
    term* answer; 
    int nAnswer; 
    int numCityTerm;
    read_in_terms(&cityTerm, &numCityTerm, name);

    //printf("%s\n", (*(cityTerm+10)).term);
    //printf("%f", (*(cityTerm+10)).weight);
    //printTerms(cityTerm, numCityTerm);

    //printf("%d", strcmp("City", "Cit"));

    printf("first appearance = %d\n", lowest_match(cityTerm, numCityTerm, "Tor"));
    printf("last appearance = %d\n", highest_match(cityTerm, numCityTerm, "Tor"));

    autocomplete(&answer, &nAnswer, cityTerm, numCityTerm, "Tor");

    

    //printTerms(answer, nAnswer);
}*/