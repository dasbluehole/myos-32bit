/*

char.c

used in desktop (or graphics.c)

graphical character descriptions and routines to display them;

*/

/*

example : the code for an 'A'

	 |-------------------------------|
	1|x	 |x 	 |x	 |x	 |
	 |-------------------------------|
	2|x	 | 	 |	 |x	 |
	 |-------------------------------|
	3|x	 |x	 |x	 |x	 |
	 |-------------------------------|
	4|x	 | 	 |	 |x	 |
	 |-------------------------------|
	5|x	 | 	 |	 |x	 |


result in matrixes

*/
#include "char.h"
#include "gvga.h"
/*
int chartoprint[5][4];
*/

int keA[5][4];
int keB[5][4];
int keC[5][4];
int keD[5][4];
int keE[5][4];
int keF[5][4];
int keG[5][4];
int keH[5][4];
int keI[5][4];
int keJ[5][4];
int keK[5][4];
int keL[5][4];
int keM[5][4];
int keN[5][4];
int keO[5][4];
int keP[5][4];
int keQ[5][4];
int keR[5][4];
int keS[5][4];
int keT[5][4];
int keU[5][4];
int keV[5][4];
int keW[5][4];
int keX[5][4];
int keY[5][4];
int keZ[5][4];
int kea[5][4];
int keb[5][4];
int kec[5][4];
int ked[5][4];
int kee[5][4];
int kef[5][4];
int keg[5][4];
int keh[5][4];
int kei[5][4];
int kej[5][4];
int kek[5][4];
int kel[5][4];
int kem[5][4];
int ken[5][4];
int keo[5][4];
int kep[5][4];
int keq[5][4];
int ker[5][4];
int kes[5][4];
int ket[5][4];
int keu[5][4];
int kev[5][4];
int kew[5][4];
int kex[5][4];
int key[5][4];
int kez[5][4];
int ke1[5][4];
int ke2[5][4];
int ke3[5][4];
int ke4[5][4];
int ke5[5][4];
int ke6[5][4];
int ke7[5][4];
int ke8[5][4];
int ke9[5][4];
int ke0[5][4];
int ke_space[5][4];
int ke_point[5][4];

void setchartables()
	{
	/* do for A */
	keA[0][0]=1; keA[0][1]=1; keA[0][2]=1; keA[0][3]=1;
	keA[1][0]=1; keA[1][1]=0; keA[1][2]=0; keA[1][3]=1;
	keA[2][0]=1; keA[2][1]=1; keA[2][2]=1; keA[2][3]=1;
	keA[3][0]=1; keA[3][1]=0; keA[3][2]=0; keA[3][3]=1;
	keA[4][0]=1; keA[4][1]=0; keA[4][2]=0; keA[4][3]=1;

	/* do for B */
	keB[0][0]=1; keB[0][1]=1; keB[0][2]=1; keB[0][3]=0;
	keB[1][0]=1; keB[1][1]=0; keB[1][2]=0; keB[1][3]=1;
	keB[2][0]=1; keB[2][1]=1; keB[2][2]=1; keB[2][3]=0;
	keB[3][0]=1; keB[3][1]=0; keB[3][2]=0; keB[3][3]=1;
	keB[4][0]=1; keB[4][1]=1; keB[4][2]=1; keB[4][3]=0;

	/* do for C */
	keC[0][0]=1; keC[0][1]=1; keC[0][2]=1; keC[0][3]=1;
	keC[1][0]=1; keC[1][1]=0; keC[1][2]=0; keC[1][3]=0;
	keC[2][0]=1; keC[2][1]=0; keC[2][2]=0; keC[2][3]=0;
	keC[3][0]=1; keC[3][1]=0; keC[3][2]=0; keC[3][3]=0;
	keC[4][0]=1; keC[4][1]=1; keC[4][2]=1; keC[4][3]=1;

	/* do for D */
	keD[0][0]=1; keD[0][1]=1; keD[0][2]=1; keD[0][3]=0;
	keD[1][0]=1; keD[1][1]=0; keD[1][2]=0; keD[1][3]=1;
	keD[2][0]=1; keD[2][1]=0; keD[2][2]=0; keD[2][3]=1;
	keD[3][0]=1; keD[3][1]=0; keD[3][2]=0; keD[3][3]=1;
	keD[4][0]=1; keD[4][1]=1; keD[4][2]=1; keD[4][3]=0;

	/* do for E */
	keE[0][0]=1; keE[0][1]=1; keE[0][2]=1; keE[0][3]=1;
	keE[1][0]=1; keE[1][1]=0; keE[1][2]=0; keE[1][3]=0;
	keE[2][0]=1; keE[2][1]=1; keE[2][2]=1; keE[2][3]=0;
	keE[3][0]=1; keE[3][1]=0; keE[3][2]=0; keE[3][3]=0;
	keE[4][0]=1; keE[4][1]=1; keE[4][2]=1; keE[4][3]=1;

	/* do for F */
	keF[0][0]=1; keF[0][1]=1; keF[0][2]=1; keF[0][3]=1;
	keF[1][0]=1; keF[1][1]=0; keF[1][2]=0; keF[1][3]=0;
	keF[2][0]=1; keF[2][1]=1; keF[2][2]=1; keF[2][3]=0;
	keF[3][0]=1; keF[3][1]=0; keF[3][2]=0; keF[3][3]=0;
	keF[4][0]=1; keF[4][1]=0; keF[4][2]=0; keF[4][3]=0;

	/* do for G */
	keG[0][0]=1; keG[0][1]=1; keG[0][2]=1; keG[0][3]=1;
	keG[1][0]=1; keG[1][1]=0; keG[1][2]=0; keG[1][3]=0;
	keG[2][0]=1; keG[2][1]=1; keG[2][2]=1; keG[2][3]=1;
	keG[3][0]=1; keG[3][1]=0; keG[3][2]=0; keG[3][3]=1;
	keG[4][0]=1; keG[4][1]=1; keG[4][2]=1; keG[4][3]=1;

	/* do for H */
	keH[0][0]=1; keH[0][1]=0; keH[0][2]=0; keH[0][3]=1;
	keH[1][0]=1; keH[1][1]=0; keH[1][2]=0; keH[1][3]=1;
	keH[2][0]=1; keH[2][1]=1; keH[2][2]=1; keH[2][3]=1;
	keH[3][0]=1; keH[3][1]=0; keH[3][2]=0; keH[3][3]=1;
	keH[4][0]=1; keH[4][1]=0; keH[4][2]=0; keH[4][3]=1;

	/* do for I */
	keI[0][0]=1; keI[0][1]=1; keI[0][2]=1; keI[0][3]=0;
	keI[1][0]=0; keI[1][1]=1; keI[1][2]=0; keI[1][3]=0;
	keI[2][0]=0; keI[2][1]=1; keI[2][2]=0; keI[2][3]=0;
	keI[3][0]=0; keI[3][1]=1; keI[3][2]=0; keI[3][3]=0;
	keI[4][0]=1; keI[4][1]=1; keI[4][2]=1; keI[4][3]=0;


	/* do for J */
	keJ[0][0]=0; keJ[0][1]=0; keJ[0][2]=0; keJ[0][3]=1;
	keJ[1][0]=0; keJ[1][1]=0; keJ[1][2]=0; keJ[1][3]=1;
	keJ[2][0]=0; keJ[2][1]=0; keJ[2][2]=0; keJ[2][3]=1;
	keJ[3][0]=0; keJ[3][1]=0; keJ[3][2]=0; keJ[3][3]=1;
	keJ[4][0]=1; keJ[4][1]=1; keJ[4][2]=1; keJ[4][3]=1;


	/* do for K */
	keK[0][0]=1; keK[0][1]=0; keK[0][2]=0; keK[0][3]=1;
	keK[1][0]=1; keK[1][1]=0; keK[1][2]=1; keK[1][3]=0;
	keK[2][0]=1; keK[2][1]=1; keK[2][2]=0; keK[2][3]=0;
	keK[3][0]=1; keK[3][1]=0; keK[3][2]=1; keK[3][3]=0;
	keK[4][0]=1; keK[4][1]=0; keK[4][2]=0; keK[4][3]=1;

	/* do for L */
	keL[0][0]=1; keL[0][1]=0; keL[0][2]=0; keL[0][3]=0;
	keL[1][0]=1; keL[1][1]=0; keL[1][2]=0; keL[1][3]=0;
	keL[2][0]=1; keL[2][1]=0; keL[2][2]=0; keL[2][3]=0;
	keL[3][0]=1; keL[3][1]=0; keL[3][2]=0; keL[3][3]=0;
	keL[4][0]=1; keL[4][1]=1; keL[4][2]=1; keL[4][3]=1;

	/* do for M */
	keM[0][0]=1; keM[0][1]=0; keM[0][2]=0; keM[0][3]=1;
	keM[1][0]=1; keM[1][1]=1; keM[1][2]=1; keM[1][3]=1;
	keM[2][0]=1; keM[2][1]=1; keM[2][2]=0; keM[2][3]=1;
	keM[3][0]=1; keM[3][1]=0; keM[3][2]=0; keM[3][3]=1;
	keM[4][0]=1; keM[4][1]=0; keM[4][2]=0; keM[4][3]=1;

	/* do for N */
	keN[0][0]=1; keN[0][1]=0; keN[0][2]=0; keN[0][3]=1;
	keN[1][0]=1; keN[1][1]=1; keN[1][2]=0; keN[1][3]=1;
	keN[2][0]=1; keN[2][1]=0; keN[2][2]=1; keN[2][3]=1;
	keN[3][0]=1; keN[3][1]=0; keN[3][2]=0; keN[3][3]=1;
	keN[4][0]=1; keN[4][1]=0; keN[4][2]=0; keN[4][3]=1;

	/* do for O */
	keO[0][0]=0; keO[0][1]=1; keO[0][2]=1; keO[0][3]=0;
	keO[1][0]=1; keO[1][1]=0; keO[1][2]=0; keO[1][3]=1;
	keO[2][0]=1; keO[2][1]=0; keO[2][2]=0; keO[2][3]=1;
	keO[3][0]=1; keO[3][1]=0; keO[3][2]=0; keO[3][3]=1;
	keO[4][0]=0; keO[4][1]=1; keO[4][2]=1; keO[4][3]=0;

	/* do for P */
	keP[0][0]=1; keP[0][1]=1; keP[0][2]=1; keP[0][3]=0;
	keP[1][0]=1; keP[1][1]=0; keP[1][2]=0; keP[1][3]=1;
	keP[2][0]=1; keP[2][1]=1; keP[2][2]=1; keP[2][3]=0;
	keP[3][0]=1; keP[3][1]=0; keP[3][2]=0; keP[3][3]=0;
	keP[4][0]=1; keP[4][1]=0; keP[4][2]=0; keP[4][3]=0;

	/* do for Q */
	keQ[0][0]=1; keQ[0][1]=1; keQ[0][2]=1; keQ[0][3]=0;
	keQ[1][0]=1; keQ[1][1]=0; keQ[1][2]=1; keQ[1][3]=0;
	keQ[2][0]=1; keQ[2][1]=0; keQ[2][2]=1; keQ[2][3]=0;
	keQ[3][0]=1; keQ[3][1]=1; keQ[3][2]=1; keQ[3][3]=0;
	keQ[4][0]=0; keQ[4][1]=0; keQ[4][2]=0; keQ[4][3]=1;

	/* do for R */
	keR[0][0]=1; keR[0][1]=1; keR[0][2]=1; keR[0][3]=0;
	keR[1][0]=1; keR[1][1]=0; keR[1][2]=0; keR[1][3]=1;
	keR[2][0]=1; keR[2][1]=1; keR[2][2]=1; keR[2][3]=0;
	keR[3][0]=1; keR[3][1]=1; keR[3][2]=0; keR[3][3]=0;
	keR[4][0]=1; keR[4][1]=0; keR[4][2]=1; keR[4][3]=0;

	/* do for S */
	keS[0][0]=0; keS[0][1]=1; keS[0][2]=1; keS[0][3]=1;
	keS[1][0]=1; keS[1][1]=0; keS[1][2]=0; keS[1][3]=0;
	keS[2][0]=0; keS[2][1]=1; keS[2][2]=1; keS[2][3]=1;
	keS[3][0]=0; keS[3][1]=0; keS[3][2]=0; keS[3][3]=1;
	keS[4][0]=1; keS[4][1]=1; keS[4][2]=1; keS[4][3]=0;

	/* do for T */
	keT[0][0]=1; keT[0][1]=1; keT[0][2]=1; keT[0][3]=1;
	keT[1][0]=0; keT[1][1]=1; keT[1][2]=0; keT[1][3]=0;
	keT[2][0]=0; keT[2][1]=1; keT[2][2]=0; keT[2][3]=0;
	keT[3][0]=0; keT[3][1]=1; keT[3][2]=0; keT[3][3]=0;
	keT[4][0]=0; keT[4][1]=1; keT[4][2]=0; keT[4][3]=0;

	/* do for U */
	keU[0][0]=1; keU[0][1]=0; keU[0][2]=0; keU[0][3]=1;
	keU[1][0]=1; keU[1][1]=0; keU[1][2]=0; keU[1][3]=1;
	keU[2][0]=1; keU[2][1]=0; keU[2][2]=0; keU[2][3]=1;
	keU[3][0]=1; keU[3][1]=0; keU[3][2]=0; keU[3][3]=1;
	keU[4][0]=0; keU[4][1]=1; keU[4][2]=1; keU[4][3]=0;

	/* do for V */
	keV[0][0]=1; keV[0][1]=0; keV[0][2]=0; keV[0][3]=1;
	keV[1][0]=1; keV[1][1]=0; keV[1][2]=0; keV[1][3]=1;
	keV[2][0]=0; keV[2][1]=1; keV[2][2]=0; keV[2][3]=1;
	keV[3][0]=0; keV[3][1]=1; keV[3][2]=0; keV[3][3]=1;
	keV[4][0]=0; keV[4][1]=0; keV[4][2]=1; keV[4][3]=0;
	
	/* do for W */
	keW[0][0]=1; keW[0][1]=0; keW[0][2]=0; keW[0][3]=1;
	keW[1][0]=1; keW[1][1]=0; keW[1][2]=0; keW[1][3]=1;
	keW[2][0]=1; keW[2][1]=0; keW[2][2]=1; keW[2][3]=1;
	keW[3][0]=1; keW[3][1]=1; keW[3][2]=1; keW[3][3]=1;
	keW[4][0]=1; keW[4][1]=1; keW[4][2]=1; keW[4][3]=1;

	/* do for X */
	keX[0][0]=1; keX[0][1]=0; keX[0][2]=0; keX[0][3]=1;
	keX[1][0]=0; keX[1][1]=1; keX[1][2]=1; keX[1][3]=0;
	keX[2][0]=0; keX[2][1]=1; keX[2][2]=0; keX[2][3]=0;
	keX[3][0]=0; keX[3][1]=1; keX[3][2]=1; keX[3][3]=0;
	keX[4][0]=1; keX[4][1]=0; keX[4][2]=0; keX[4][3]=1;

	/* do for Y */
	keY[0][0]=1; keY[0][1]=0; keY[0][2]=0; keY[0][3]=1;
	keY[1][0]=0; keY[1][1]=1; keY[1][2]=1; keY[1][3]=0;
	keY[2][0]=0; keY[2][1]=1; keY[2][2]=0; keY[2][3]=0;
	keY[3][0]=0; keY[3][1]=1; keY[3][2]=0; keY[3][3]=0;
	keY[4][0]=0; keY[4][1]=1; keY[4][2]=0; keY[4][3]=0;

	/* do for Z */
	keZ[0][0]=1; keZ[0][1]=1; keZ[0][2]=1; keZ[0][3]=1;
	keZ[1][0]=0; keZ[1][1]=0; keZ[1][2]=0; keZ[1][3]=1;
	keZ[2][0]=0; keZ[2][1]=0; keZ[2][2]=1; keZ[2][3]=0;
	keZ[3][0]=0; keZ[3][1]=1; keZ[3][2]=0; keZ[3][3]=0;
	keZ[4][0]=1; keZ[4][1]=1; keZ[4][2]=1; keZ[4][3]=1;

	/* do for a */
	kea[0][0]=1; kea[0][1]=1; kea[0][2]=1; kea[0][3]=1;
	kea[1][0]=1; kea[1][1]=0; kea[1][2]=0; kea[1][3]=1;
	kea[2][0]=1; kea[2][1]=1; kea[2][2]=1; kea[2][3]=1;
	kea[3][0]=1; kea[3][1]=0; kea[3][2]=0; kea[3][3]=1;
	kea[4][0]=1; kea[4][1]=0; kea[4][2]=0; kea[4][3]=1;

	/* do for b */
	keb[0][0]=1; keb[0][1]=1; keb[0][2]=1; keb[0][3]=0;
	keb[1][0]=1; keb[1][1]=0; keb[1][2]=0; keb[1][3]=1;
	keb[2][0]=1; keb[2][1]=1; keb[2][2]=1; keb[2][3]=0;
	keb[3][0]=1; keb[3][1]=0; keb[3][2]=0; keb[3][3]=1;
	keb[4][0]=1; keb[4][1]=1; keb[4][2]=1; keb[4][3]=0;

	/* do for c */
	kec[0][0]=1; kec[0][1]=1; kec[0][2]=1; kec[0][3]=1;
	kec[1][0]=1; kec[1][1]=0; kec[1][2]=0; kec[1][3]=0;
	kec[2][0]=1; kec[2][1]=0; kec[2][2]=0; kec[2][3]=0;
	kec[3][0]=1; kec[3][1]=0; kec[3][2]=0; kec[3][3]=0;
	kec[4][0]=1; kec[4][1]=1; kec[4][2]=1; kec[4][3]=1;

	/* do for d */
	ked[0][0]=1; ked[0][1]=1; ked[0][2]=1; ked[0][3]=0;
	ked[1][0]=1; ked[1][1]=0; ked[1][2]=0; ked[1][3]=1;
	ked[2][0]=1; ked[2][1]=0; ked[2][2]=0; ked[2][3]=1;
	ked[3][0]=1; ked[3][1]=0; ked[3][2]=0; ked[3][3]=1;
	ked[4][0]=1; ked[4][1]=1; ked[4][2]=1; ked[4][3]=0;

	/* do for e */
	kee[0][0]=1; kee[0][1]=1; kee[0][2]=1; kee[0][3]=1;
	kee[1][0]=1; kee[1][1]=0; kee[1][2]=0; kee[1][3]=0;
	kee[2][0]=1; kee[2][1]=1; kee[2][2]=1; kee[2][3]=0;
	kee[3][0]=1; kee[3][1]=0; kee[3][2]=0; kee[3][3]=0;
	kee[4][0]=1; kee[4][1]=1; kee[4][2]=1; kee[4][3]=1;

	/* do for f */
	keF[0][0]=1; keF[0][1]=1; keF[0][2]=1; keF[0][3]=1;
	keF[1][0]=1; keF[1][1]=0; keF[1][2]=0; keF[1][3]=0;
	keF[2][0]=1; keF[2][1]=1; keF[2][2]=1; keF[2][3]=0;
	keF[3][0]=1; keF[3][1]=0; keF[3][2]=0; keF[3][3]=0;
	keF[4][0]=1; keF[4][1]=0; keF[4][2]=0; keF[4][3]=0;

	/* do for g */
	keG[0][0]=1; keG[0][1]=1; keG[0][2]=1; keG[0][3]=1;
	keG[1][0]=1; keG[1][1]=0; keG[1][2]=0; keG[1][3]=0;
	keG[2][0]=1; keG[2][1]=1; keG[2][2]=1; keG[2][3]=1;
	keG[3][0]=1; keG[3][1]=0; keG[3][2]=0; keG[3][3]=1;
	keG[4][0]=1; keG[4][1]=1; keG[4][2]=1; keG[4][3]=1;

	/* do for h */
	keH[0][0]=1; keH[0][1]=0; keH[0][2]=0; keH[0][3]=1;
	keH[1][0]=1; keH[1][1]=0; keH[1][2]=0; keH[1][3]=1;
	keH[2][0]=1; keH[2][1]=1; keH[2][2]=1; keH[2][3]=1;
	keH[3][0]=1; keH[3][1]=0; keH[3][2]=0; keH[3][3]=1;
	keH[4][0]=1; keH[4][1]=0; keH[4][2]=0; keH[4][3]=1;

	/* do for i */
	keI[0][0]=1; keI[0][1]=1; keI[0][2]=1; keI[0][3]=0;
	keI[1][0]=0; keI[1][1]=1; keI[1][2]=0; keI[1][3]=0;
	keI[2][0]=0; keI[2][1]=1; keI[2][2]=0; keI[2][3]=0;
	keI[3][0]=0; keI[3][1]=1; keI[3][2]=0; keI[3][3]=0;
	keI[4][0]=1; keI[4][1]=1; keI[4][2]=1; keI[4][3]=0;


	/* do for j */
	keJ[0][0]=0; keJ[0][1]=0; keJ[0][2]=0; keJ[0][3]=1;
	keJ[1][0]=0; keJ[1][1]=0; keJ[1][2]=0; keJ[1][3]=1;
	keJ[2][0]=0; keJ[2][1]=0; keJ[2][2]=0; keJ[2][3]=1;
	keJ[3][0]=0; keJ[3][1]=0; keJ[3][2]=0; keJ[3][3]=1;
	keJ[4][0]=1; keJ[4][1]=1; keJ[4][2]=1; keJ[4][3]=1;


	/* do for k */
	keK[0][0]=1; keK[0][1]=0; keK[0][2]=0; keK[0][3]=1;
	keK[1][0]=1; keK[1][1]=0; keK[1][2]=1; keK[1][3]=0;
	keK[2][0]=1; keK[2][1]=1; keK[2][2]=0; keK[2][3]=0;
	keK[3][0]=1; keK[3][1]=0; keK[3][2]=1; keK[3][3]=0;
	keK[4][0]=1; keK[4][1]=0; keK[4][2]=0; keK[4][3]=1;

	/* do for l */
	keL[0][0]=1; keL[0][1]=0; keL[0][2]=0; keL[0][3]=0;
	keL[1][0]=1; keL[1][1]=0; keL[1][2]=0; keL[1][3]=0;
	keL[2][0]=1; keL[2][1]=0; keL[2][2]=0; keL[2][3]=0;
	keL[3][0]=1; keL[3][1]=0; keL[3][2]=0; keL[3][3]=0;
	keL[4][0]=1; keL[4][1]=1; keL[4][2]=1; keL[4][3]=1;

	/* do for m */
	keM[0][0]=1; keM[0][1]=0; keM[0][2]=0; keM[0][3]=1;
	keM[1][0]=1; keM[1][1]=1; keM[1][2]=1; keM[1][3]=1;
	keM[2][0]=1; keM[2][1]=1; keM[2][2]=0; keM[2][3]=1;
	keM[3][0]=1; keM[3][1]=0; keM[3][2]=0; keM[3][3]=1;
	keM[4][0]=1; keM[4][1]=0; keM[4][2]=0; keM[4][3]=1;

	/* do for n */
	keN[0][0]=1; keN[0][1]=0; keN[0][2]=0; keN[0][3]=1;
	keN[1][0]=1; keN[1][1]=1; keN[1][2]=0; keN[1][3]=1;
	keN[2][0]=1; keN[2][1]=0; keN[2][2]=1; keN[2][3]=1;
	keN[3][0]=1; keN[3][1]=0; keN[3][2]=0; keN[3][3]=1;
	keN[4][0]=1; keN[4][1]=0; keN[4][2]=0; keN[4][3]=1;

	/* do for o */
	keo[0][0]=0; keo[0][1]=1; keo[0][2]=1; keo[0][3]=0;
	keo[1][0]=1; keo[1][1]=0; keo[1][2]=0; keo[1][3]=1;
	keo[2][0]=1; keo[2][1]=0; keo[2][2]=0; keo[2][3]=1;
	keo[3][0]=1; keo[3][1]=0; keo[3][2]=0; keo[3][3]=1;
	keo[4][0]=0; keo[4][1]=1; keo[4][2]=1; keo[4][3]=0;

	/* do for p */
	keP[0][0]=1; keP[0][1]=1; keP[0][2]=1; keP[0][3]=0;
	keP[1][0]=1; keP[1][1]=0; keP[1][2]=0; keP[1][3]=1;
	keP[2][0]=1; keP[2][1]=1; keP[2][2]=1; keP[2][3]=0;
	keP[3][0]=1; keP[3][1]=0; keP[3][2]=0; keP[3][3]=0;
	keP[4][0]=1; keP[4][1]=0; keP[4][2]=0; keP[4][3]=0;

	/* do for q */
	keQ[0][0]=1; keQ[0][1]=1; keQ[0][2]=1; keQ[0][3]=0;
	keQ[1][0]=1; keQ[1][1]=0; keQ[1][2]=1; keQ[1][3]=0;
	keQ[2][0]=1; keQ[2][1]=0; keQ[2][2]=1; keQ[2][3]=0;
	keQ[3][0]=1; keQ[3][1]=1; keQ[3][2]=1; keQ[3][3]=0;
	keQ[4][0]=0; keQ[4][1]=0; keQ[4][2]=0; keQ[4][3]=1;

	/* do for r */
	ker[0][0]=1; ker[0][1]=1; ker[0][2]=1; ker[0][3]=0;
	ker[1][0]=1; ker[1][1]=0; ker[1][2]=0; ker[1][3]=1;
	ker[2][0]=1; ker[2][1]=1; ker[2][2]=1; ker[2][3]=0;
	ker[3][0]=1; ker[3][1]=1; ker[3][2]=0; ker[3][3]=0;
	ker[4][0]=1; ker[4][1]=0; ker[4][2]=1; ker[4][3]=0;

	/* do for s */
	keS[0][0]=0; keS[0][1]=1; keS[0][2]=1; keS[0][3]=1;
	keS[1][0]=1; keS[1][1]=0; keS[1][2]=0; keS[1][3]=0;
	keS[2][0]=0; keS[2][1]=1; keS[2][2]=1; keS[2][3]=1;
	keS[3][0]=0; keS[3][1]=0; keS[3][2]=0; keS[3][3]=1;
	keS[4][0]=1; keS[4][1]=1; keS[4][2]=1; keS[4][3]=0;

	/* do for t */
	ket[0][0]=1; ket[0][1]=1; ket[0][2]=1; ket[0][3]=1;
	ket[1][0]=0; ket[1][1]=1; ket[1][2]=0; ket[1][3]=0;
	ket[2][0]=0; ket[2][1]=1; ket[2][2]=0; ket[2][3]=0;
	ket[3][0]=0; ket[3][1]=1; ket[3][2]=0; ket[3][3]=0;
	ket[4][0]=0; ket[4][1]=1; ket[4][2]=0; ket[4][3]=0;

	/* do for u */
	keU[0][0]=1; keU[0][1]=0; keU[0][2]=0; keU[0][3]=1;
	keU[1][0]=1; keU[1][1]=0; keU[1][2]=0; keU[1][3]=1;
	keU[2][0]=1; keU[2][1]=0; keU[2][2]=0; keU[2][3]=1;
	keU[3][0]=1; keU[3][1]=0; keU[3][2]=0; keU[3][3]=1;
	keU[4][0]=0; keU[4][1]=1; keU[4][2]=1; keU[4][3]=0;

	/* do for v */
	keV[0][0]=1; keV[0][1]=0; keV[0][2]=0; keV[0][3]=1;
	keV[1][0]=1; keV[1][1]=0; keV[1][2]=0; keV[1][3]=1;
	keV[2][0]=0; keV[2][1]=1; keV[2][2]=0; keV[2][3]=1;
	keV[3][0]=0; keV[3][1]=1; keV[3][2]=0; keV[3][3]=1;
	keV[4][0]=0; keV[4][1]=0; keV[4][2]=1; keV[4][3]=0;

	/* do for w */
	keW[0][0]=1; keW[0][1]=0; keW[0][2]=0; keW[0][3]=1;
	keW[1][0]=1; keW[1][1]=0; keW[1][2]=0; keW[1][3]=1;
	keW[2][0]=1; keW[2][1]=0; keW[2][2]=1; keW[2][3]=1;
	keW[3][0]=1; keW[3][1]=1; keW[3][2]=1; keW[3][3]=1;
	keW[4][0]=1; keW[4][1]=1; keW[4][2]=1; keW[4][3]=1;

	/* do for x */
	keX[0][0]=1; keX[0][1]=0; keX[0][2]=0; keX[0][3]=1;
	keX[1][0]=0; keX[1][1]=1; keX[1][2]=1; keX[1][3]=0;
	keX[2][0]=0; keX[2][1]=1; keX[2][2]=0; keX[2][3]=0;
	keX[3][0]=0; keX[3][1]=1; keX[3][2]=1; keX[3][3]=0;
	keX[4][0]=1; keX[4][1]=0; keX[4][2]=0; keX[4][3]=1;

	/* do for y */
	keY[0][0]=1; keY[0][1]=0; keY[0][2]=0; keY[0][3]=1;
	keY[1][0]=0; keY[1][1]=1; keY[1][2]=1; keY[1][3]=0;
	keY[2][0]=0; keY[2][1]=1; keY[2][2]=0; keY[2][3]=0;
	keY[3][0]=0; keY[3][1]=1; keY[3][2]=0; keY[3][3]=0;
	keY[4][0]=0; keY[4][1]=1; keY[4][2]=0; keY[4][3]=0;

	/* do for z */
	keZ[0][0]=1; keZ[0][1]=1; keZ[0][2]=1; keZ[0][3]=1;
	keZ[1][0]=0; keZ[1][1]=0; keZ[1][2]=0; keZ[1][3]=1;
	keZ[2][0]=0; keZ[2][1]=0; keZ[2][2]=1; keZ[2][3]=0;
	keZ[3][0]=0; keZ[3][1]=1; keZ[3][2]=0; keZ[3][3]=0;
	keZ[4][0]=1; keZ[4][1]=1; keZ[4][2]=1; keZ[4][3]=1;

	/* do for 0 */
	ke0[0][0]=0; ke0[0][1]=1; ke0[0][2]=1; ke0[0][3]=0;
	ke0[1][0]=1; ke0[1][1]=0; ke0[1][2]=0; ke0[1][3]=1;
	ke0[2][0]=1; ke0[2][1]=1; ke0[2][2]=1; ke0[2][3]=1;
	ke0[3][0]=1; ke0[3][1]=0; ke0[3][2]=0; ke0[3][3]=1;
	ke0[4][0]=0; ke0[4][1]=1; ke0[4][2]=1; ke0[4][3]=0;

	/* do for 1 */
	ke1[0][0]=0; ke1[0][1]=0; ke1[0][2]=1; ke1[0][3]=0;
	ke1[1][0]=1; ke1[1][1]=1; ke1[1][2]=1; ke1[1][3]=0;
	ke1[2][0]=0; ke1[2][1]=0; ke1[2][2]=1; ke1[2][3]=0;
	ke1[3][0]=0; ke1[3][1]=0; ke1[3][2]=1; ke1[3][3]=0;
	ke1[4][0]=1; ke1[4][1]=1; ke1[4][2]=1; ke1[4][3]=1;

	/* do for space */
	ke_space[0][0]=0; ke_space[0][1]=0; ke_space[0][2]=0; ke_space[0][3]=0;
	ke_space[1][0]=0; ke_space[1][1]=0; ke_space[1][2]=0; ke_space[1][3]=0;
	ke_space[2][0]=0; ke_space[2][1]=0; ke_space[2][2]=0; ke_space[2][3]=0;
	ke_space[3][0]=0; ke_space[3][1]=0; ke_space[3][2]=0; ke_space[3][3]=0;
	ke_space[4][0]=0; ke_space[4][1]=0; ke_space[4][2]=0; ke_space[4][3]=0;

	/* do for point */
	ke_point[0][0]=0; ke_point[0][1]=0; ke_point[0][2]=0; ke_point[0][3]=0;
	ke_point[1][0]=0; ke_point[1][1]=0; ke_point[1][2]=0; ke_point[1][3]=0;
	ke_point[2][0]=0; ke_point[2][1]=0; ke_point[2][2]=0; ke_point[2][3]=0;
	ke_point[3][0]=0; ke_point[3][1]=0; ke_point[3][2]=0; ke_point[3][3]=0;
	ke_point[4][0]=0; ke_point[4][1]=1; ke_point[4][2]=0; ke_point[4][3]=0;
	}

void displaychar(int x,int y,char kar,int color)
	{
	int row,column;
	int helpx;
	void setchartoprint(char);
	/*_BX=0;
	_AL=0;
	_CX=x;
	_DX=y;*/
	helpx=x;
	setchartoprint(kar);
	for(row=0;row<=4;row++)
		{/*
		_DX=y;*/
		for(column=0;column<=3;column++)
			{/*
			_CX=x;*/
			if(chartoprint[row][column]==1)
				{
				/*_AL=color;
				putpixel();
				*/
				vgaSetPixel(x,y,color);
				}
			x++;
			}
		x=helpx;
		y++;
		}
	}


void setchartoprint(char kar)
	{
	int row,column;
	for(row=0;row<=4;row++)
		{
		for(column=0;column<=3;column++)
			{
			if(kar=='A')chartoprint[row][column]=keA[row][column];
			if(kar=='B')chartoprint[row][column]=keB[row][column];
			if(kar=='C')chartoprint[row][column]=keC[row][column];
			if(kar=='D')chartoprint[row][column]=keD[row][column];
			if(kar=='E')chartoprint[row][column]=keE[row][column];
			if(kar=='F')chartoprint[row][column]=keF[row][column];
			if(kar=='G')chartoprint[row][column]=keG[row][column];
			if(kar=='H')chartoprint[row][column]=keH[row][column];
			if(kar=='I')chartoprint[row][column]=keI[row][column];
			if(kar=='J')chartoprint[row][column]=keJ[row][column];
			if(kar=='K')chartoprint[row][column]=keK[row][column];
			if(kar=='L')chartoprint[row][column]=keL[row][column];
			if(kar=='M')chartoprint[row][column]=keM[row][column];
			if(kar=='N')chartoprint[row][column]=keN[row][column];
			if(kar=='O')chartoprint[row][column]=keO[row][column];
			if(kar=='P')chartoprint[row][column]=keP[row][column];
			if(kar=='Q')chartoprint[row][column]=keQ[row][column];
			if(kar=='R')chartoprint[row][column]=keR[row][column];
			if(kar=='S')chartoprint[row][column]=keS[row][column];
			if(kar=='T')chartoprint[row][column]=keT[row][column];
			if(kar=='U')chartoprint[row][column]=keU[row][column];
			if(kar=='V')chartoprint[row][column]=keV[row][column];
			if(kar=='W')chartoprint[row][column]=keW[row][column];
			if(kar=='X')chartoprint[row][column]=keX[row][column];
			if(kar=='Y')chartoprint[row][column]=keY[row][column];
			if(kar=='Z')chartoprint[row][column]=keZ[row][column];
			if(kar=='a')chartoprint[row][column]=kea[row][column];
			if(kar=='b')chartoprint[row][column]=keb[row][column];
			if(kar=='c')chartoprint[row][column]=kec[row][column];
			if(kar=='d')chartoprint[row][column]=ked[row][column];
			if(kar=='e')chartoprint[row][column]=kee[row][column];
			if(kar=='f')chartoprint[row][column]=kef[row][column];
			if(kar=='g')chartoprint[row][column]=keg[row][column];
			if(kar=='h')chartoprint[row][column]=keh[row][column];
			if(kar=='i')chartoprint[row][column]=kei[row][column];
			if(kar=='j')chartoprint[row][column]=kej[row][column];
			if(kar=='k')chartoprint[row][column]=kek[row][column];
			if(kar=='l')chartoprint[row][column]=kel[row][column];
			if(kar=='m')chartoprint[row][column]=kem[row][column];
			if(kar=='n')chartoprint[row][column]=ken[row][column];
			if(kar=='o')chartoprint[row][column]=keo[row][column];
			if(kar=='p')chartoprint[row][column]=kep[row][column];
			if(kar=='q')chartoprint[row][column]=keq[row][column];
			if(kar=='r')chartoprint[row][column]=ker[row][column];
			if(kar=='s')chartoprint[row][column]=kes[row][column];
			if(kar=='t')chartoprint[row][column]=ket[row][column];
			if(kar=='u')chartoprint[row][column]=keu[row][column];
			if(kar=='v')chartoprint[row][column]=kev[row][column];
			if(kar=='w')chartoprint[row][column]=kew[row][column];
			if(kar=='x')chartoprint[row][column]=kex[row][column];
			if(kar=='y')chartoprint[row][column]=key[row][column];
			if(kar=='z')chartoprint[row][column]=kez[row][column];
			if(kar=='1')chartoprint[row][column]=ke1[row][column];
			if(kar=='2')chartoprint[row][column]=ke2[row][column];
			if(kar=='3')chartoprint[row][column]=ke3[row][column];
			if(kar=='4')chartoprint[row][column]=ke4[row][column];
			if(kar=='5')chartoprint[row][column]=ke5[row][column];
			if(kar=='6')chartoprint[row][column]=ke6[row][column];
			if(kar=='7')chartoprint[row][column]=ke7[row][column];
			if(kar=='8')chartoprint[row][column]=ke8[row][column];
			if(kar=='9')chartoprint[row][column]=ke9[row][column];
			if(kar=='0')chartoprint[row][column]=ke0[row][column];
			if(kar==' ')chartoprint[row][column]=ke_space[row][column];
			if(kar=='.')chartoprint[row][column]=ke_point[row][column];
			}
		}
	}


int displaymessages(int x,int y, const char *message,int color)
	{
	int counterF;
	int xfactortoadd=0;
	/*_BX=0;*/
	for(counterF=0; ;counterF++)
		{
		if(message[counterF]=='$')return 0;
		displaychar((x+xfactortoadd),y,message[counterF],color);
		xfactortoadd=xfactortoadd+6;
		}
	}

int Rsizeof( const char *message)
	{
	int counterSF;
		for(counterSF=0; ;counterSF++)
		{
		if(message[counterSF]=='$')return (counterSF+1);
		
		}
	}	
