/**
 */

#ifndef _CTREENP_HPP_
#define _CTREENP_HPP_

/**
 * Reprezentuje strom s N potomky a rodicem
 */
class CTreeNP
{
	protected:
		CTreeNP**Child;///potomci
		unsigned NumChild;///pocet potomku
		unsigned ChildID;///cislo potomka v rodici
		CTreeNP*Prev;///rodic
	public:
		/**
		 * @brief Konstruktor
		 * @param Parent rodic
		 * @param ChildID cislo potomka v rodici reprezentujici tento uzel
		 * @param NumChild pocet potomku
		 */
		CTreeNP(CTreeNP*Parent,unsigned ChildID,unsigned NumChild);
		/**
		 * Destruktor
		 */
		~CTreeNP();
		/**
		 * @brief Pocet uzlu stromu
		 *
		 * @return pocet uzlu stromu
		 */
		unsigned NumNode();
		/**
		 * @brief Vraci rodice
		 * @return rodice
		 */
		CTreeNP*GetParent();
		/**
		 * Ziska cislo uzlu
		 * @return cislo tohoto uzlu v rodici
		 */
		unsigned GetChildId();
		/**
		 * Nastavi cislo uzlu
		 * @param cislo tohoto uzlu v rodici
		 */
		void SetChildID(unsigned ChildID);
		/**
		 * Ziska potomka
		 * @param ChildID cislo potomka
		 * @return potomek indexovany ChildID
		 */
		CTreeNP*GetChild(unsigned ChildID);
		/**
		 * Nastavi potomka
		 * @param ChildID cislo potomka
		 * @param Child potomek
		 */
		void SetChild(unsigned ChildID,CTreeNP*Child);
		/**
		 * Ziska pocet potomku
		 * @return pocet potomku
		 */
		unsigned GetNumChild();
};

#endif//_CTREENP_HPP_
