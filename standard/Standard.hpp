#ifndef _STANDARD_HPP_
#define _STANDARD_HPP_

#include<stdlib.h>
#include<locale.h>
#include<stdarg.h>
#include<string.h>
#include<iostream>
#include<fstream>
//#include<GL/gl.h>
//#include<GL/glu.h>

/**
 * Rozhodne na zaklade koncovky co je to za soubor
 * @param FileName cely nazev souboru
 * @param NumType pocet typu
 * Dalsi parametry je vycet typu v retezcovem tvaru
 * @return vraci poradove cislo podle koncovky
 */
int FileTypeSwitch(std::string FileName,unsigned NumType,...);

/**
 * Precte cely soubor
 * @param Len delka data
 * @param File nazev soubor k precteni
 * @return vraci data
 */
unsigned char*ReadWholeFile(int*Len,std::string File);

/**
 * Zjisti delku UTF-8 textu
 * @param Text text
 * @return vraci pocet znaku v textu
 */
unsigned UTF8Lenght(std::string Text);

/**
 * Ziska z daneho textu v UTF-8 znak na dane pozici
 * @param Text text
 * @param Index index
 * @return vraci znak
 */
std::string UTF8GetChar(std::string Text,unsigned Index);

/**
 * @brief Porovna dva znaky v UTF-8
 *
 * @param Char0 prvni znak
 * @param Char1 druhy znak
 *
 * @return -1 pro Char0<Char1, 0 pro Char0==Char1, 1 pro Char0>Char1
 */
int UTF8CharCmp(std::string Char0,std::string Char1);

/**
 * @brief Pokud je na zacatku Text escape sekvence prevede ji na znak v 
 * UTF8 pokud ne vraci prvni znak
 *
 * @param Text text prohledavany na escape sekvenci
 * @param Len pocet BYTE ktere funkce ukousla
 *
 * @return prevedeny znak
 */
std::string UTF8EscapeSeq(std::string Text,unsigned*Len);

/**
 * Prevede cislo integer na text
 * @param c cislo
 * @return textova reprezentace cisla
 */
std::string IntToStr(int c);
/**
 * @brief Prevede cislo unsigned long long na text
 *
 * @param c cislo
 *
 * @return textova reprezentace cisla
 */
std::string ULLToStr(unsigned long long c);

#endif//_STANDARD_HPP_
