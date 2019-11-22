#include "assembleXX.h"

/*
 * Free the the list of strings elems
 * Arguments:
 *      n : number of elements in elems
 *      elems : the list
 */
void aXX_freeElems(uint8_t n,char** elems){
    for(uint8_t i=0; i<n; i++)
        free(elems[i]);
}

/*
 * take a line and separate the various parts (opperand, register number...) in a small list.
 * Argument:
 *      line : the line wich must be read
 *      list : a list of the various elements
 *  return:
 *      the number of elements in list
 */
uint8_t aXX_preprocessLine(char* line,char** list){
    uint8_t ret = 0;
    bool flagWhitepace = true; //indicate wheaser we are pointing a whitespac or not
    size_t len = strlen(line);
    for(uint8_t i=0; i<len; i++){ //how much words
        if(line[i] == ';' || line[i] == '\n' || line[i] == '\r'){
            i = 0xFF;
            break;
        }
        if(flagWhitepace){
            if(line[i] != ' ' && line[i] != '	'){
                flagWhitepace = false;
                ret++;
            }
        }else{
            if(line[i] == ' ' || line[i] == '	')
                flagWhitepace = true;
        }
    }
    for(uint8_t i=0;i<ret;i++)
        list[i] = malloc(100); //overkill
    uint8_t elem = 0; //which element of the list is to be filled next
    uint8_t start; //Start of a word
    flagWhitepace = true;
    for(uint8_t i=0; i<len; i++){ //copying the words
        if(elem >= ret){
            i=200;
            break;
        }
        if(flagWhitepace){
            if(line[i] != ' ' && line[i] != '	'){
                flagWhitepace = false;
                start = i;
            }
        }else{
            if(line[i] == ' ' || line[i] == '	' || line[i] == '\0' || line[i] == '\n' || line[i] == '\r'){
                flagWhitepace = true;
                line[i] = 0;
                strcpy(list[elem],line+start); //We put a word in line
                elem++;
            }
        }
    }
    return ret;
}

/*
 * Read a hex number and return it's value
 * Arguments:
 *      number : the string represinting the number in hexadecimal
 */
uint64_t aXX_readHex(char* number){
    uint64_t ret = 0;
    for(uint8_t i=0; i<strlen(number); i++){
        if( 48 <= number[i] && number[i] <= 57) //a digit between 0 and 9
            ret = ret + ((number[i] - 48) << (4 * i));
        else if(65 <= number[i] && number[i] <= 70) //a digit between A ans F
            ret = ret + ((number[i] - 55) << (4 * i));
    }
    return ret;
}

/*
 * Read a decimal number and return it's value
 * Arguments:
 *      number : the string represinting the number in decimal
 */
uint64_t aXX_readDec(char* number){
    uint64_t ret = 0;
    uint64_t digit = 1;
    for(uint8_t i=0; i<strlen(number); i++){
        if( 48 <= number[i] && number[i] <= 57) //a digit between 0 and 9
            ret = ret + (number[i] - 48) * digit;
        digit = digit * 10;
    }
    return ret;
}
