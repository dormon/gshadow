/**
 */

#ifndef _CLIST_HPP_
#define _CLIST_HPP_

#include<iostream>
using namespace std;

/**
 * Linerani seznam
 */
class CList
{
	protected:
		CList*Next;//nasledujici prvek
	public:
		/**
		 * Konstruktor
		 */
		CList();
		/**
		 * Destruktor
		 */
		~CList();
		/**
		 * Ziska nasledujici prvek
		 * @return nasleduji prvek
		 */
		CList*GetNext();
		/**
		 * Ziska adresu dalsiho
		 * @return adresa dalsiho
		 */
		CList**GetANext();
		/**
		 * Prodlouzi
		 * @param Next nasleduji prvek/list
		 */
		void Extend(CList*Next);
};

#endif//_CLIST_HPP_
