#ifndef _MYMATH_HPP_
#define _MYMATH_HPP_

#include<math.h>

namespace MyMath{
	/**
	 * secte vektory
	 * @param a vysledny vektor a
	 * @param b druhy vektor
	 */
	void Add3(float*a,float*b);
	/**
	 * odecte vektory
	 * @param a vysledny vektor a
	 * @param b druhy vektor
	 */
	void Sub3(float*a,float*b);
	/**
	 * vynasobi vektory
	 * @param a vysledny vektor a
	 * @param b druhy vektor
	 */
	void Mul3(float*a,float*b);
	/**
	 * skalarne vynasobi vektory
	 * @param a vysledny vektor a
	 * @param b druhy vektor
	 * @return skralarni nasobek
	 */
	float Dot3(float*a,float*b);
	/**
	 * vynasobi vektor konstantou
	 * @param a vysledny vektor a
	 * @param K konstanta
	 */
	void KMul3(float*a,float K);
	/**
	 * velikost vektoru
	 * @param a vektor
	 * @return vraci velikost vektoru
	 */
	float Len3(float*a);
	/**
	 * vytvori kolmy vektor na 2 vektory
	 * @param c vysledny vektor
	 * @param a vektor 1
	 * @param b vektor 2
	 */
	void Ortho3(float*c,float*a,float*b);
	/**
	 * @brief Spocita cross product
	 *
	 * @param c vystup
	 * @param a jeden vektor
	 * @param b druhy vektor
	 */
	void Cross3(float*c,float*a,float*b);
	/**
	 * znormalizuje vektor
	 * @param a vysledny vektor
	 */
	void Nor3(float*a);
	/**
	 * spocita vektor na minus prvni
	 * @param a vektor
	 */
	void Rot3(float*a);
	/**
	 * Vlozi vektor do druheho
	 * @param a vysledny vektor
	 * @param b druhy vektor
	 */
	void Mov3(float*a,float*b);
	/**
	 * vygeneruje nahodny jednotkovy vektor
	 * @param a vektor
	 */
	void Rnd3(float*a);
	/**
	 * vytvori jednotkovy linearne nezavisly vektor b k a
	 * @param a vektor a
	 * @param b linearne nezavisly vektor
	 */
	void LN3(float*b,float*a);
	/**
	 * Spocita kosinus uhlu mezi vektory
	 * @param u vektor u
	 * @param v vektor v
	 * @return cosinus uhlu
	 */
	float Cos3(float*u,float*v);
	/**
	 * Vytvori vektor mezi body A a B
	 * @param O vystupni vektor
	 * @param A bod zacateku vektoru
	 * @param B bod konce vektoru
	 */
	void Make3(float*O,float*A,float*B);
	/**
	 * @brief Provede reflexi vektoru (uhel odrazu == uhel dopadu)
	 *
	 * @param O vystupni vektor
	 * @param V vstupni vektor
	 * @param N normala povrchu
	 */
	void Reflect3(float*O,float*V,float*N);
	/**
	 * @brief Zkontroluje na identitu
	 *
	 * @param A vektor a
	 * @param B vektor b
	 *
	 * @return vraci A==B
	 */
	int Eq3(float*A,float*B);
  int Greater3(float*A,float*B);
};

#endif//_MYMATH_HPP_
