#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <tchar.h>
#include "FPImageUtil.h"

#define EL_ALGO_VER		"V6.0"

struct SensorTypeSize stSensorTypeSize[elSquare_TOTAL] = { {160, 160 }, { 120, 120 }, { 96, 96 }, { 80, 80 } };
int nSensorTypeL, nSensorTypeM, nSensorTypeXXS, nSensorTypeXXXS;
int nSensorWidth, nSensorHeight;
int nConnectingTh = 60;
bool bLengthAndQuality = true;
bool bZeroDifferential = true;

struct VerticalTextureInfo *pVerticalTextureInfo = NULL;
struct HorizontalTextureInfo *pHorizontalTextureInfo = NULL;
int VerticalLength = 0;
int HorizontalLength = 0;
int nUpdateVerticalLength = 0;
int nUpdateHorizontalLength = 0;

void SetSensorType(int nSensorType)
{
	if (nSensorType == elSquare_L) {
		/* 160 * 160 */
		nSensorTypeL = 1;
		nSensorTypeM = 0;
		nSensorTypeXXS = 0;
		nSensorTypeXXXS = 0;
	}
	else if (nSensorType == elSquare_M) {
		/* 120 * 120 */
		nSensorTypeL = 0;
		nSensorTypeM = 1;
		nSensorTypeXXS = 0;
		nSensorTypeXXXS = 0;
	}
	else if (nSensorType == elSquare_XXS) {
		/* 96 * 96 */
		nSensorTypeL = 0;
		nSensorTypeM = 0;
		nSensorTypeXXS = 1;
		nSensorTypeXXXS = 0;
	}
	else if (nSensorType == elSquare_XXXS) {
		/* 80 * 80 */
		nSensorTypeL = 0;
		nSensorTypeM = 0;
		nSensorTypeXXS = 0;
		nSensorTypeXXXS = 1;
	}
	else {
		/* Define (nSensorType == elSquare_L) */
		nSensorTypeL = 1;
		nSensorTypeM = 0;
		nSensorTypeXXS = 0;
		nSensorTypeXXXS = 0;
	}
}

int GetSensorType(void)
{
	int nSensorType = -1;
	if (nSensorTypeL == 1) {
		/* 160 * 160 */
		nSensorType = elSquare_L;
	}
	else if (nSensorTypeM == 1) {
		/* 120 * 120 */
		nSensorType = elSquare_M;
	}
	else if (nSensorTypeXXS == 1) {
		/* 96 * 96 */
		nSensorType = elSquare_XXS;
	}
	else if (nSensorTypeXXXS == 1) {
		/* 80 * 80 */
		nSensorType = elSquare_XXXS;
	}
	else {
		/* 160 * 160 */
		nSensorType = elSquare_L;
	}
	return nSensorType;
}

void SensorSizeSetup(int nSensorType)
{
	if (nSensorType == elSquare_L) {
		/* 160 * 160 */
		nConnectingTh = 40;
		nSensorWidth = stSensorTypeSize[nSensorType].width;
		nSensorHeight = stSensorTypeSize[nSensorType].height;
	}
	else if (nSensorType == elSquare_M) {
		/* 120 * 120 */
		nConnectingTh = 30;
		nSensorWidth = stSensorTypeSize[nSensorType].width;
		nSensorHeight = stSensorTypeSize[nSensorType].height;
	}
	else if (nSensorType == elSquare_XXS) {
		/* 96 * 96 */
		nConnectingTh = 20;
		nSensorWidth = stSensorTypeSize[nSensorType].width;
		nSensorHeight = stSensorTypeSize[nSensorType].height;
	}
	else if (nSensorType == elSquare_XXXS) {
		/* 80 * 80 */
		nConnectingTh = 10;
		nSensorWidth = stSensorTypeSize[nSensorType].width;
		nSensorHeight = stSensorTypeSize[nSensorType].height;
	}
	else {
		/* 160 * 160 */
		nConnectingTh = 40;
		nSensorWidth = stSensorTypeSize[nSensorType].width;
		nSensorHeight = stSensorTypeSize[nSensorType].height;
	}
}

//void Smooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
//{
//	short i, j, buf;
//	for (j = 1; j < nHeight - 1; j++) {
//		for (i = 1; i < nWidth - 1; i++) {
//			buf = (short)image_in[j - 1][i - 1] +
//				(short)image_in[j - 1][i] +
//				(short)image_in[j - 1][i + 1] +
//				(short)image_in[j][i - 1] +
//				(short)image_in[j][i] +
//				(short)image_in[j][i + 1] +
//				(short)image_in[j + 1][i - 1] +
//				(short)image_in[j + 1][i] +
//				(short)image_in[j + 1][i + 1];
//			image_out[j][i] = buf / 9;
//		}
//	}
//}

void HorizontalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	for (j = 0; j < nHeight; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)image_in[j][i - 1] +
				  (short)image_in[j][i] + (short)image_in[j][i] +
				  (short)image_in[j][i + 1];
			image_out[j][i] = (buf + 2) / 4;
		}
	}
	
	/* 直接把水平輸入影像的 第1條 跟第160條 影像值直接拿來當輸出水平影像的 第1條 跟 第160條 影像值 */ 
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0 || i == nWidth - 1) {
				image_out[j][i] = 0/*image_in[j][i]*/;
			}
		}
	}
}

void VerticalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	//for (j = 1; j < nHeight - 1; j++) {
		//for (i = 0; i < nWidth; i++) {
	for (i = 0; i < nWidth; i++) {
		for (j = 1; j < nHeight - 1; j++) {
			buf = (short)image_in[j - 1][i] +
				(short)image_in[j][i] + (short)image_in[j][i] +
				(short)image_in[j + 1][i];
			image_out[j][i] = (buf + 2) / 4; /* >> 2 */
		}
	}

	/* 直接把垂直輸入影像的 第1條 跟第160條 影像值直接拿來當輸出垂直影像的 第1條 跟 第160條 影像值 */
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0 || j == nHeight - 1) {
				image_out[j][i] = 0/*image_in[j][i]*/;
			}
		}
	}
}

void HorizontalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;
	
	short i, j, buf;
	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < nWidth - 2; i++) {
			buf = (short)image_in[j][i - 1] * (-1) +
				(short)image_in[j][i] * (0) +
				(short)image_in[j][i + 1] * (1);
			image_out[j][i] = buf + 128/*>> 7*/;
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 1)
				image_out[j][i] = 0/*image_in[j][i - 1]*/;
			if (i == nWidth - 2)
				image_out[j][i] = 0/*image_in[j][i + 1]*/;
		}
	}
}

void VerticalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	//char buffer[1000] = {0};
	short i, j, buf;
	//for (j = 2; j < nHeight - 2; j++) {
		//for (i = 0; i < nWidth; i++) {
	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			buf = (short)(image_in[j - 1][i]) * (-1) +
				  (short)(image_in[j][i]) * (0) +
				  (short)(image_in[j + 1][i]) * (1);
			image_out[j][i] = buf + 128/*>> 7*/;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [%3d][%3d] buf = %3d\r\n", __FUNCTION__, __LINE__, j, i, buf);
			//LOGD(buffer);
		}
	}

	for (i = 0; i < nWidth; i++) {
		for (j = 0; j < nHeight; j++) {
			if (j == 1)
				image_out[j][i] = 0/*image_in[j - 1][i]*/;
			if(j == nHeight - 2)
				image_out[j][i] = 0/*image_in[j + 1][i]*/;
		}
	}
}

void Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if(pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			pImage_out[j][i] = pImage_in[j * nWidth + i];
		}
	}
}

void Cover1DFormat(unsigned char **pImage_in, unsigned char *pImage_out, int nWidth, int nHeight, int bExchangeBits)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if(bExchangeBits == 0)
				pImage_out[j * nWidth + i] = pImage_in[j][i];
			else
				pImage_out[(nHeight - 1 - j) * nWidth + i] = pImage_in[j][i];
		}
	}
}

void HorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < nWidth - 2; i++) {
			sA = pImage_in[j][i - 2];/*A*/
			sB = pImage_in[j][i - 1];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j][i + 1];/*D*/
			sE = pImage_in[j][i + 2];/*E*/

			if (sC < 0 && sD > 0) {							/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					if (pImage_out[j][i + 1] == V0)
						pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					else
						pImage_out[j][i + 1] = R0;			/*L(D) = R0*/

					if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
					}
					else if (pImage_out[j][i - 1] == V0) {	/*L(B) = V0*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 1] == VL) {	/*L(B) = VL*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
					//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					if (pImage_out[j][i + 1] == V0)
						pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					else
						pImage_out[j][i + 1] = R0;			/*L(D) = R0*/

					if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
					}
					else if (pImage_out[j][i - 1] == V0) {	/*L(B) = V0*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 1] == VL) {	/*L(B) = VL*/
						pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
					//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
			}
			else if (sC == 0) {								/*C = 0*/
				if (sB < 0 && sD > 0) {						/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sB > 0 && sD < 0) {				/*B > 0 && D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == V0)
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							else
								pImage_out[j][i] = R0;				/*L(C) = R0*/

							if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
							}
							else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
								pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
							}
							else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
							}
							//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
							//}
						}
						else {
							if (pImage_out[j][i + 1] == V0)
								pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
							else
								pImage_out[j][i + 1] = R0;			/*L(D) = R0*/

							if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
								pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
							}
							else if (pImage_out[j][i - 1] == V0) {	/*L(B) = V0*/
								pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
								pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
							}
							else if (pImage_out[j][i - 1] == VL) {	/*L(B) = VL*/
								pImage_out[j][i + 1] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							}
							//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
							//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//}
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == R0)
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							else
								pImage_out[j][i] = V0;				/*L(C) = V0*/

							if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
							}
							else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
								pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
							}
							else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
							}
							//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
							//}
						}
						else {
							if (pImage_out[j][i + 1] == R0)
								pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
							else
								pImage_out[j][i + 1] = V0;			/*L(D) = V0*/

							if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
								pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
							}
							else if (pImage_out[j][i - 1] == R0) {	/*L(B) = R0*/
								pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
								pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
							}
							else if (pImage_out[j][i - 1] == RL) {	/*L(B) = RL*/
								pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							}
							//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
							//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//}
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {					/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					if (pImage_out[j][i + 1] == R0)
						pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					else
						pImage_out[j][i + 1] = V0;			/*L(D) = V0*/

					if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
					}
					else if (pImage_out[j][i - 1] == R0) {	/*L(B) = R0*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 1] == RL) {	/*L(B) = RL*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
					//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					if (pImage_out[j][i + 1] == R0)
						pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					else
						pImage_out[j][i + 1] = V0;			/*L(D) = V0*/

					if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
					}
					else if (pImage_out[j][i - 1] == R0) {	/*L(B) = R0*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 1] == RL) {	/*L(B) = RL*/
						pImage_out[j][i + 1] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j][i - 1] == DL) {/*L(B) = DL*/
					//	pImage_out[j][i + 1] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < nWidth - 2; i++)
	} //for (j = 0; j < nHeight; j++)
}

void VerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];/*A*/
			sB = pImage_in[j - 1][i];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j + 1][i];/*D*/
			sE = pImage_in[j + 2][i];/*E*/

			if (sC < 0 && sD > 0) {							/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					if (pImage_out[j + 1][i] == V0)
						pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					else
						pImage_out[j + 1][i] = R0;			/*L(D) = R0*/

					if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
					}
					else if (pImage_out[j - 1][i] == V0) {	/*L(B) = V0*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 1][i] == VL) {	/*L(B) = VL*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j - 1][i] == DL) {/*L(B) = DL*/
					//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					///	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					if (pImage_out[j + 1][i] == V0)
						pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					else
						pImage_out[j + 1][i] = R0;			/*L(D) = R0*/

					if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
					}
					else if (pImage_out[j - 1][i] == V0) {	/*L(B) = V0*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}

					else if (pImage_out[j - 1][i] == VL) {	/*L(B) = VL*/
						pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j - 1][i] == DL) {/*L(B) = DL*/
					//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
			}
			else if (sC == 0) {								/*C = 0*/
				if (sB < 0 && sD > 0) {						/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sB > 0 && sD < 0) {				/*B > 0　&& D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == V0)
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							else
								pImage_out[j][i] = R0;				/*L(C) = R0*/

							if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
							}
							else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
								pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
							}
							else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
								pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
							}
							//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
							//}
						}
						else {
							if (pImage_out[j + 1][i] == V0)
								pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
							else
								pImage_out[j + 1][i] = R0;			/*L(D) = R0*/

							if (pImage_out[j][i] == V0) {			/*L(C) = V0*/
								pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
							}
							else if (pImage_out[j - 1][i] == V0) {	/*L(B) = V0*/
								pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
								pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
							}
							else if (pImage_out[j - 1][i] == VL) {	/*L(B) = VL*/
								pImage_out[j + 1][i] = RL;			/*L(D) = RL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							}
							//else if (pImage_out[j - 1][i] == DL) {/*L(B) = DL*/
							//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//}
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == R0)
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							else
								pImage_out[j][i] = V0;				/*L(C) = V0*/

							if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
							}
							else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
								pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
							}
							else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
								pImage_out[j][i] = VL;				/*L(C) = VL*/
								pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
							}
							//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
							//}
						}
						else {
							if (pImage_out[j + 1][i] == R0)
								pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
							else
								pImage_out[j + 1][i] = V0;			/*L(D) = V0*/

							if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
								pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = RL;				/*L(C) = RL*/
							}
							else if (pImage_out[j - 1][i] == R0) {	/*L(B) = R0*/
								pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
								pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
							}
							else if (pImage_out[j - 1][i] == RL) {	/*L(B) = RL*/
								pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
								pImage_out[j][i] = DL;				/*L(C) = DL*/
							}
							//else if (pImage_out[j - 1][i] == DL) {/*L(B) = DL*/
							//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
							//	pImage_out[j][i] = DL;				/*L(C) = DL*/
							//}
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {					/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					if (pImage_out[j + 1][i] == R0)
						pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					else
						pImage_out[j + 1][i] = V0;			/*L(D) = V0*/

					if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
					}
					else if (pImage_out[j - 1][i] == R0) {	/*L(B) = R0*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 1][i] == RL) {	/*L(B) = RL*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j - 1][i] == DL) {/*L(B) = DL*/
					//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					if (pImage_out[j + 1][i] == R0)
						pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					else
						pImage_out[j + 1][i] = V0;			/*L(D) = V0*/

					if (pImage_out[j][i] == R0) {			/*L(C) = R0*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
					}
					else if (pImage_out[j - 1][i] == R0) {	/*L(B) = R0*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 1][i] == RL) {	/*L(B) = RL*/
						pImage_out[j + 1][i] = VL;			/*L(D) = VL*/
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {/*L(B) = DL*/
					//	pImage_out[j + 1][i] = DL;			/*L(D) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//}
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < nHeight - 2; j++)
	} //for (i = 0; i < nWidth; i++)
}

#if 0
void xxHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < nWidth - 2; i++) {
			sA = pImage_in[j][i - 2];/*A*/
			sB = pImage_in[j][i - 1];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j][i + 1];/*D*/
			sE = pImage_in[j][i + 2];/*E*/

			if (sC < 0 && sD > 0) {							/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					pImage_out[j][i + 1] = R0;				/*L(D) = R0*/
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					pImage_out[j][i + 1] = R0;				/*L(D) = R0*/
				}
			}
			else if (sC == 0) {								/*C = 0*/
				if (sB < 0 && sD > 0) {						/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j][i - 2] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j][i - 2] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sB > 0 && sD < 0) {				/*B > 0 && D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == V0)
								pImage_out[j][i] = DL;		/*L(C) = DL*/
							else
								pImage_out[j][i] = R0;		/*L(C) = R0*/
						}
						else {
							pImage_out[j][i + 1] = R0;		/*L(D) = R0*/
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == R0)
								pImage_out[j][i] = DL;		/*L(C) = DL*/
							else
								pImage_out[j][i] = V0;		/*L(C) = V0*/
						}
						else {
							pImage_out[j][i + 1] = V0;		/*L(D) = V0*/
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {					/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					pImage_out[j][i + 1] = V0;				/*L(D) = V0*/
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j][i - 2] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
						pImage_out[j][i - 2] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j][i - 2] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j][i - 2] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j][i - 1] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					pImage_out[j][i + 1] = V0;				/*L(D) = V0*/
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < nWidth - 2; i++)
	} //for (j = 0; j < nHeight; j++)
}

void xxVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];/*A*/
			sB = pImage_in[j - 1][i];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j + 1][i];/*D*/
			sE = pImage_in[j + 2][i];/*E*/

			if (sC < 0 && sD > 0) {							/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					pImage_out[j + 1][i] = R0;				/*L(D) = R0*/
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					pImage_out[j + 1][i] = R0;				/*L(D) = R0*/
				}
			}
			else if (sC == 0) {								/*C = 0*/
				if (sB < 0 && sD > 0) {						/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {		/*L(B) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;			/*L(B) = VL*/
					}
					else if (pImage_out[j - 2][i] == V0) {	/*L(A) = V0*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = VL;			/*L(A) = VL*/
					}
					else if (pImage_out[j - 2][i] == VL) {	/*L(A) = VL*/
						pImage_out[j][i] = RL;				/*L(C) = RL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sB > 0 && sD < 0) {				/*B > 0　&& D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == V0)
								pImage_out[j][i] = DL;		/*L(C) = DL*/
							else
								pImage_out[j][i] = R0;		/*L(C) = R0*/
						}
						else {
							pImage_out[j + 1][i] = R0;		/*L(D) = R0*/
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							if (pImage_out[j][i] == R0)
								pImage_out[j][i] = DL;		/*L(C) = DL*/
							else
								pImage_out[j][i] = V0;		/*L(C) = V0*/
						}
						else {
							pImage_out[j + 1][i] = V0;		/*L(D) = V0*/
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {					/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {					/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else if (abs(sC) > abs(sD)) {				/*abs(C) > abs(D)*/
					pImage_out[j + 1][i] = V0;				/*L(D) = V0*/
				}
				else if (abs(sB) < abs(sE)) {				/*abs(B) < abs(E)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;				/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {		/*L(B) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;			/*L(B) = RL*/
					}
					else if (pImage_out[j - 2][i] == R0) {	/*L(A) = R0*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
						pImage_out[j - 2][i] = RL;			/*L(A) = RL*/
					}
					else if (pImage_out[j - 2][i] == RL) {	/*L(A) = RL*/
						pImage_out[j][i] = VL;				/*L(C) = VL*/
						pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					}
					//else if (pImage_out[j - 2][i] == DL) {	/*L(A) = DL*/
					//	pImage_out[j][i] = DL;				/*L(C) = DL*/
					//	pImage_out[j - 1][i] = DL;			/*L(B) = DL*/
					//}
				}
				else {										/*abs(B) > abs(E)*/
					pImage_out[j + 1][i] = V0;				/*L(D) = V0*/
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < nHeight - 2; j++)
	} //for (i = 0; i < nWidth; i++)
}

void xHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < nWidth - 2; i++) {
			sA = pImage_in[j][i - 2];/*A*/
			sB = pImage_in[j][i - 1];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j][i + 1];/*D*/
			sE = pImage_in[j][i + 2];/*E*/

			if (sC < 0 && sD > 0) {						/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {				/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;			/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {	/*L(B) = V0*/
						pImage_out[j][i] = RL;			/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;		/*L(B) = VL*/
					}
				}
				else if (abs(sC) > abs(sD)) {			/*abs(C) > abs(D)*/
					pImage_out[j][i + 1] = R0;			/*L(D) = R0*/
				}
				else if (abs(sB) < abs(sE)) {			/*abs(B) < abs(E)*/
					pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j][i - 1] == V0) {	/*L(B) = V0*/
						pImage_out[j][i] = RL;			/*L(C) = RL*/
						pImage_out[j][i - 1] = VL;		/*L(B) = VL*/
					}
				}
				else {									/*abs(B) > abs(E)*/
					pImage_out[j][i + 1] = R0;			/*L(D) = R0*/
				}
			}
			else if (sC == 0) {							/*C = 0*/
				if (sB < 0 && sD > 0) {					/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;			/*L(C) = R0*/
				}
				else if (sB > 0 && sD < 0) {			/*B > 0 && D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;			/*L(C) = V0*/
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = R0;		/*L(C) = R0*/
						}
						else {
							pImage_out[j][i + 1] = R0;	/*L(D) = R0*/
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = V0;		/*L(C) = V0*/
						}
						else {
							pImage_out[j][i + 1] = V0;	/*L(D) = V0*/
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {				/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {				/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;			/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {	/*L(B) = R0*/
						pImage_out[j][i] = VL;			/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;		/*L(B) = RL*/
					}
				}
				else if (abs(sC) > abs(sD)) {			/*abs(C) > abs(D)*/
					pImage_out[j][i + 1] = V0;			/*L(D) = V0*/
				}
				else if (abs(sB) < abs(sE)) {			/*abs(B) < abs(E)*/
					pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j][i - 1] == R0) {	/*L(B) = R0*/
						pImage_out[j][i] = VL;			/*L(C) = VL*/
						pImage_out[j][i - 1] = RL;		/*L(B) = RL*/
					}
				}
				else {									/*abs(B) > abs(E)*/
					pImage_out[j][i + 1] = V0;			/*L(D) = V0*/
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < nWidth - 2; i++)
	} //for (j = 0; j < nHeight; j++)
}

void xVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	unsigned char R0 = 90, R1 = 120, R2 = 150, RL = 0, DL = 30, VL = 60, V2 = 240, V1 = 210, V0 = 180, X = 255, TH1 = 9;

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];/*A*/
			sB = pImage_in[j - 1][i];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j + 1][i];/*D*/
			sE = pImage_in[j + 2][i];/*E*/

			if (sC < 0 && sD > 0) {						/*C < 0 && D > 0*/
				if (abs(sC) < abs(sD)) {				/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;			/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {	/*L(B) = V0*/
						pImage_out[j][i] = RL;			/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;		/*L(B) = VL*/
					}
				}
				else if (abs(sC) > abs(sD)) {			/*abs(C) > abs(D)*/
					pImage_out[j + 1][i] = R0;			/*L(D) = R0*/
				}
				else if (abs(sB) < abs(sE)) {			/*abs(B) < abs(E)*/
					pImage_out[j][i] = R0;				/*L(C) = R0*/

					if (pImage_out[j - 1][i] == V0) {	/*L(B) = V0*/
						pImage_out[j][i] = RL;			/*L(C) = RL*/
						pImage_out[j - 1][i] = VL;		/*L(B) = VL*/
					}
				}
				else {									/*abs(B) > abs(E)*/
					pImage_out[j + 1][i] = R0;			/*L(D) = R0*/
				}
			}
			else if (sC == 0) {							/*C = 0*/
				if (sB < 0 && sD > 0) {					/*B < 0 && D > 0*/
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = R0;			/*L(C) = R0*/
				}
				else if (sB > 0 && sD < 0) {			/*B > 0　&& D < 0*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;			/*L(C) = V0*/
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = R0;		/*L(C) = R0*/
						}
						else {
							pImage_out[j + 1][i] = R0;	/*L(D) = R0*/
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = V0;		/*L(C) = V0*/
						}
						else {
							pImage_out[j + 1][i] = V0;	/*L(D) = V0*/
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {				/*C > 0 && D < 0*/
				if (abs(sC) < abs(sD)) {				/*abs(C) < abs(D)*/
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;			/*L(C) = DL*/
					else
						pImage_out[j][i] = V0;			/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {	/*L(B) = R0*/
						pImage_out[j][i] = VL;			/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;		/*L(B) = RL*/
					}
				}
				else if (abs(sC) > abs(sD)) {			/*abs(C) > abs(D)*/
					pImage_out[j + 1][i] = V0;			/*L(D) = V0*/
				}
				else if (abs(sB) < abs(sE)) {			/*abs(B) < abs(E)*/
					pImage_out[j][i] = V0;				/*L(C) = V0*/

					if (pImage_out[j - 1][i] == R0) {	/*L(B) = R0*/
						pImage_out[j][i] = VL;			/*L(C) = VL*/
						pImage_out[j - 1][i] = RL;		/*L(B) = RL*/
					}
				}
				else {									/*abs(B) > abs(E)*/
					pImage_out[j + 1][i] = V0;			/*L(D) = V0*/
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < nHeight - 2; j++)
	} //for (i = 0; i < nWidth; i++)
}
#endif

void HorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[20][79] 存放開始 X 座標 / Vc[n][j] = i
	unsigned char Lw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//左寬
	unsigned char Rw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//右寬

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//頭 Y 座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//尾 Y 座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 }; //Y 長度

	unsigned char sC1x;
	char nC1x[MaxC1Len] = { 0, 1, 2, 3, 4, 5, -1, -2, -3, -4, -5 };
	//char nC1x[MaxC1Len] = { 0, 1, 2, -1, 3, 4, -2, -3, -4, 5, -5 };

	n = 0;
	nRecordNum = 0;
	j = nStartY;
	/*中->左 尋找*/
	for (i = nStartX; i > 0; i--) {
		if (pLabelingData_in[j][i] == V0 || pLabelingData_in[j][i] == VL || pLabelingData_in[j][i] == DL) {
			Vc[n][j] = i;
			nRecordNum = n;
			n++;
			if (n == MaxRecordItemLength - 1)
				break;
		}
	}

	/*中->右 尋找*/
	for (i = nStartX; i < nWidth; i++) {
		if (pLabelingData_in[j][i] == V0 || pLabelingData_in[j][i] == VL || pLabelingData_in[j][i] == DL) {
			Vc[n][j] = i;
			nRecordNum = n;
			n++;
			if (n == MaxRecordItemLength - 1)
				break;
		}
	}
	
	bool bFind, bFindR0, bFindVL, bFindDL;
	int nRecordR0_IDx = -1;
	int jj, nArIdx;
	int nSx = 0, nSy = 0;

	for (n = 0; n < (nRecordNum + 1); n++) {

		/*中 -> 下*/
		nSx = Vc[n][j];	//取 X 座標
		nSy = nStartY;	//取 Y 座標

		for (jj = nSy; jj < nHeight - 2; ) {
			jj = jj + 1;	//將 Y 座標下移
			bFind = false;
			bFindR0 = false;
			bFindVL = false;
			bFindDL = false;
			nRecordR0_IDx = -1;

			for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++) {
				if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
					if (nC1x[nArIdx] > 0)
						continue;
				}
				if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
					if (nC1x[nArIdx] < 0)
						continue;
				}

				if (nSx + nC1x[nArIdx] < nWidth)
					sC1x = nSx + nC1x[nArIdx];
				else
					sC1x = nWidth - 1;

				if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL || pLabelingData_in[jj][sC1x] == DL) {
					bFind = true;
					nSx = sC1x;
					Vc[n][jj] = sC1x;
					break;
				}
				else if (pLabelingData_in[jj][sC1x] == R0) {
					if (bFindR0 == false)
						bFindR0 = true;
					nRecordR0_IDx = nArIdx;
					continue;
				}
				else {
					//全部找完還是沒找到
					if (nArIdx == MaxC1Len - 1) {
						bFind = false;
					}
				}
			}//for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++)

			if (bFind == false)
				break;
		}//for (jj = nSy; jj < nHeight - 2; )

		/*中 -> 上*/
		nSx = Vc[n][j];	//取 X 座標
		nSy = nStartY;	//取 Y 座標

		for (jj = nSy; jj > 1; ) {
			jj = jj - 1;	//將 Y 座標上移
			bFind = false;
			bFindR0 = false;
			bFindVL = false;
			bFindDL = false;
			nRecordR0_IDx = -1;

			for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++) {
				if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
					if (nC1x[nArIdx] > 0)
						continue;
				}
				if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
					if (nC1x[nArIdx] < 0)
						continue;
				}

				if (nSx + nC1x[nArIdx] < nWidth)
					sC1x = nSx + nC1x[nArIdx];
				else
					sC1x = nWidth - 1;

				if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL || pLabelingData_in[jj][sC1x] == DL) {
					bFind = true;
					nSx = sC1x;
					Vc[n][jj] = sC1x;
					break;
				}
				else if (pLabelingData_in[jj][sC1x] == R0) {
					if (bFindR0 == false)
						bFindR0 = true;
					nRecordR0_IDx = nArIdx;
					continue;
				}
				else {
					//全部找完還是沒找到
					if (nArIdx == MaxC1Len - 1) {
						bFind = false;
					}
				}
			}//for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++)

			if (bFind == false)
				break;
		}//for (jj = nSy; jj > 1; )

		int nMin = 0, nMax = 0, nCntNum = 0;
		bool bFirstTime = false;
		for (jj = 0; jj < nHeight; jj++) {
			if (Vc[n][jj] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = jj;
					bFirstTime = true;
				}

				if (jj > nMax)
					nMax = jj;
				if (jj <= nMin)
					nMin = jj;
			}
		}
		Vhead[n] = nMin;
		Vtail[n] = nMax;
		Vlenght[n] = nMax - nMin + 1;
	}//for (n = 0; n < (nRecordNum + 1); n++)

	for (j = 0; j < nHeight; j++)
		memset(pLabelingData_in[j], 255, nWidth);

	for (n = 0; n < (nRecordNum + 1); n++) {
		for (jj = 0; jj < nHeight - 1; jj++) {
			if (Vc[n][jj] != 0) {
				pLabelingData_in[jj][Vc[n][jj]] = V0;
			}
		}
	}
}

void VerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//Hc[20][79] 存放 Y 座標
	unsigned char Uw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//上寬
	unsigned char Dw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//下寬

	unsigned char Hhead[MaxRecordItemLength] = { 0 };	//頭X座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };	//尾X座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };	//X長度

	unsigned char sC1y;
	char nC1y[MaxC1Len] = { 0, 1, 2, 3, 4, 5, -1, -2, -3, -4, -5 };
	//char nC1y[MaxC1Len] = { 0, 1, 2, -1, 3, 4, -2, -3, -4, 5, -5 };

	n = 0;
	nRecordNum = 0;
	i = nStartX;
	/*中->上 尋找*/
	for (j = nStartY; j > 0; j--) {
		if (pLabelingData_in[j][i] == V0 || pLabelingData_in[j][i] == VL || pLabelingData_in[j][i] == DL) {
			Hc[n][i] = j;
			nRecordNum = n;
			n++;
			if (n == MaxRecordItemLength - 1)
				break;
		}
	}

	/*中->下 尋找*/
	for (j = nStartY; j < nHeight; j++) {
		if (pLabelingData_in[j][i] == V0 || pLabelingData_in[j][i] == VL || pLabelingData_in[j][i] == DL) {
			Hc[n][i] = j;
			nRecordNum = n;
			n++;
			if (n == MaxRecordItemLength - 1)
				break;
		}
	}

	bool bFind, bFindR0, bFindVL, bFindDL;
	int nRecordR0_IDx = -1;
	int ii, nArIdx;
	int nSx = 0, nSy = 0;

	for (n = 0; n < (nRecordNum + 1); n++) {

		/*中 -> 右*/
		nSx = nStartX;	//取 X 座標
		nSy = Hc[n][i];	//取 Y 座標

		for (ii = nSx; ii < nWidth - 2; ) {
			ii = ii + 1;	//將 X 座標右移
			bFind = false;
			bFindR0 = false;
			bFindVL = false;
			bFindDL = false;
			nRecordR0_IDx = -1;

			for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++) {
				if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
					if (nC1y[nArIdx] > 0)
						continue;
				}
				if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
					if (nC1y[nArIdx] < 0)
						continue;
				}

				if (nSy + nC1y[nArIdx] < nHeight)
					sC1y = nSy + nC1y[nArIdx];
				else
					sC1y = nHeight - 1;

				if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL || pLabelingData_in[sC1y][ii] == DL) {
					bFind = true;
					nSy = sC1y;
					Hc[n][ii] = sC1y;
					break;
				}
				else if (pLabelingData_in[sC1y][ii] == R0) {
					if (bFindR0 == false)
						bFindR0 = true;
					nRecordR0_IDx = nArIdx;
					continue;
				}
				else {
					//全部找完還是沒找到
					if (nArIdx == MaxC1Len - 1) {
						bFind = false;
					}
				}
			}//for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++)

			if (bFind == false)
				break;
		}//for (ii = nSx; ii < nWidth - 2; )

		/*中 -> 左*/
		nSx = nStartX;	//取 X 座標
		nSy = Hc[n][i];	//取 Y 座標

		for (ii = nSx; ii > 1; ) {
			ii = ii - 1;	//將 X 座標左移
			bFind = false;
			bFindR0 = false;
			bFindVL = false;
			bFindDL = false;
			nRecordR0_IDx = -1;

			for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++) {

				if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
					if (nC1y[nArIdx] > 0)
						continue;
				}
				if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
					if (nC1y[nArIdx] < 0)
						continue;
				}

				if (nSy + nC1y[nArIdx] < nHeight)
					sC1y = nSy + nC1y[nArIdx];
				else
					sC1y = nHeight - 1;

				if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL || pLabelingData_in[sC1y][ii] == DL) {
					bFind = true;
					nSy = sC1y;
					Hc[n][ii] = sC1y;
					break;
				}
				else if (pLabelingData_in[sC1y][ii] == R0) {
					if (bFindR0 == false)
						bFindR0 = true;
					nRecordR0_IDx = nArIdx;
					continue;
				}
				else {
					//全部找完還是沒找到
					if (nArIdx == MaxC1Len - 1) {
						bFind = false;
					}
				}
			}//for (nArIdx = 0; nArIdx < MaxC1Len; nArIdx++)

			if (bFind == false)
				break;
		}//for (ii = nSx; ii > 1; )

		int nMin = 0, nMax = 0, nCntNum = 0;
		bool bFirstTime = false;
		for (ii = 0; ii < nWidth; ii++) {
			if (Hc[n][ii] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = ii;
					bFirstTime = true;
				}

				if (ii > nMax)
					nMax = ii;
				if (ii <= nMin)
					nMin = ii;
			}
		}
		Hhead[n] = nMin;
		Htail[n] = nMax;
		Hlenght[n] = nMax - nMin + 1;
	}//for (n = 0; n < (nRecordNum + 1); n++)

	 //Celan Labeling memory for Connecting Use
	for (j = 0; j < nHeight; j++)
		memset(pLabelingData_in[j], 255, nWidth);

	for (n = 0; n < (nRecordNum + 1); n++) {
		for (ii = 0; ii < nWidth - 1; ii++) {
			if (Hc[n][ii] != 0) {
				pLabelingData_in[Hc[n][ii]][ii] = V0;
			}
		}
	}
}

void xHorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	char buffer[1000] = { 0 };
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nConnectingTh = %3d\r\n", __FUNCTION__, __LINE__, nConnectingTh);
	LOGD(buffer);

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[20][79] 存放 X 座標
	unsigned char Lw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//左寬
	unsigned char Rw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//右寬

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//頭 Y 座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//尾 Y 座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 }; //Y 長度

	unsigned char sC1x;
	//char nC1x[MaxC1Len] = { 0, 1, 2, 3, 4, 5, -1, -2, -3, -4, -5 };
	char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false;
	int nRecordR0_IDx = -1;
	int jj, j1, pj, nj, jLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	//int nX, nY, nD, nU, jd, ju;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先上再下的控制 */
	for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

		if (jLoop == 0 && jLoop % 2 == 0) {
			j1 = 0;
			j = nStartY + j1;
			nj = pj = j;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else if (jLoop > 0 && jLoop % 2 == 0) {
			j1 = 1;//下
			pj = pj + j1;
			j = pj;
			if (j > nHeight - 1)
				j = nHeight - 1;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else {
			j1 = -1;//上
			nj = nj + j1;
			j = nj;
			if (j < 0)
				j = 0;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}

		/* X Right -> Left */
		for (i = 0; i < nWidth; i++) {

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				Vc[n][j] = i;	//存 X 座標
				nRecordNum = n;
				//TRACE("Vc[%3d][%3d] = %3d\r\n", n, j, i);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*中 -> 下 尋找下一點*/
				nSx = Vc[n][j];	//取 X 座標
				nSy = j;		//取 Y 座標
				//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向下尋找 */
				for (jj = nSy; jj < nHeight - 1; ) {
					jj = jj + 1;	//將 Y 座標下移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							Vc[n][jj] = sC1x;
							//if (jj + 1 == nHeight)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_in[jj + 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj < nHeight - 1; )
				//TRACE("\r\n");

				/*中 -> 上 尋找下一點*/
				nSx = Vc[n][j];	//取 X 座標
				nSy = j;		//取 Y 座標
				//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向上尋找 */
				for (jj = nSy; jj > 0; ) {
					jj = jj - 1;	//將 Y 座標上移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							Vc[n][jj] = sC1x;
							//if (jj - 1 < 0)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength - 1) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}
				n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (i = 0; i < nWidth; i++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Horizontal Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum + 1);
	LOGD(buffer);

	int nMin, nMax, nCntNum;
	bool bFirstTime;
	for (n = 0; n < (nRecordNum + 1); n++) {
		nMin = 0;
		nMax = 0;
		nCntNum = 0;
		bFirstTime = false;
		for (jj = 0; jj < nHeight; jj++) {
			/*取 X 座標 */
			if (Vc[n][jj] != 0) {
				/*Calculation X Left and Right Width*/
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d /******\r\n", __FUNCTION__, __LINE__);//KennyKang
				LOGD(buffer);
				GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][jj], jj, Lw[n], Rw[n], n);//KennyKang
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vc[%d][%d] = %d / Lw[%d][%d] = %d / Rw[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, n, jj, Vc[n][jj], n, jj, Lw[n][jj], n, jj, Rw[n][jj]);//KennyKang
				LOGD(buffer);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d ******/\r\n", __FUNCTION__, __LINE__);//KennyKang
				LOGD(buffer);

				nCntNum++;
				if (bFirstTime == false) {
					nMin = jj;
					bFirstTime = true;
				}

				if (jj > nMax)
					nMax = jj;
				if (jj <= nMin)
					nMin = jj;
			}
		}
		Vhead[n] = nMin;
		Vtail[n] = nMax;
		Vlenght[n] = nMax - nMin + 1;
	}//for (n = 0; n < (nRecordNum + 1); n++)

	int nRecordCnt = 0;
	for (n = 0; n < (nRecordNum + 1); n++) {
		if (Vlenght[n] >= nConnectingTh) {
			nRecordCnt++;
		}
	}
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Horizontal nRecordCnt = %d\r\n", __FUNCTION__, __LINE__, nRecordCnt);
	LOGD(buffer);

	if (nRecordCnt != 0) {
		/* Save Horizontal Data to Struct */
		InitHorizontalAlgoMemory(nRecordCnt);
		SetHorizontalLength(nRecordCnt);

		int nItemCnt = 0;
		for (n = 0; n < (nRecordNum + 1); n++) {
			/*取 X 座標*/
			if (Vlenght[n] >= nConnectingTh) {
				/* 儲存資料 */
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Head = Vhead[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Tail = Vtail[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Length = Vlenght[n];
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n]);
				//LOGD(buffer);

				SetHorizontalConnectingData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pHorizontal, Vc[n], Vhead[n], Vtail[n], Vlenght[n], Lw[n], Rw[n], NULL, nItemCnt);
				nItemCnt++;
			}
		}
	}//if (nRecordCnt != 0)

	//Celan Labeling memory for Connecting Use
	for (j = 0; j < nHeight; j++)
		memset(pLabelingData_in[j], 255, nWidth);

	for (n = 0; n < (nRecordNum + 1); n++) {
		for (j = 0; j < nHeight; j++) {
			/*取 X 座標 */
			if (Vc[n][j] != 0 && Vlenght[n] >= nConnectingTh) {
				pLabelingData_in[j][Vc[n][j]] = Vx * 2;
			}
		}
	}
}

void xVerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	char buffer[1000] = { 0 };
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nConnectingTh = %3d\r\n", __FUNCTION__, __LINE__, nConnectingTh);
	LOGD(buffer);

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//Hc[nRecordNum][nStartX] 存放 Y 座標
	unsigned char Uw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//上寬
	unsigned char Dw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };//下寬

	unsigned char Hhead[MaxRecordItemLength] = { 0 };	//頭X座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };	//尾X座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };//X長度

	unsigned char sC1y;
	//char nC1y[MaxC1Len] = { 0, 1, 2, 3, 4, 5, -1, -2, -3, -4, -5 };
	char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false;
	int nRecordR0_IDx = -1;
	int ii, i1, pi, ni, iLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	//int nX, nY, nD, nU, jd, ju;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先左再右的控制 */
	for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

		if (iLoop == 0 && iLoop % 2 == 0) {
			i1 = 0;
			i = nStartX + i1;
			ni = pi = i;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else if (iLoop > 0 && iLoop % 2 == 0) {
			i1 = 1;
			pi = pi + i1;
			i = pi;
			if (i > nWidth - 1)
				i = nWidth - 1;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else {
			i1 = -1;
			ni = ni + i1;
			i = ni;
			if (i < 0)
				i = 0;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}

		/* Y Top - > Down */
		for (j = 0; j < nHeight; j++) {

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				Hc[n][i] = j;	//存 Y 座標
				nRecordNum = n;
				//TRACE("Hc[%3d][%3d] = %3d\r\n", n, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/* 中 -> 右 尋找下一點 */
				nSx = i;		//取 X 座標
				nSy = Hc[n][i];	//取 Y 座標
				//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向右尋找 */
				for (ii = nSx; ii < nWidth - 1; ) {
					ii = ii + 1;	//將 X 座標右移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							Hc[n][ii] = sC1y;
							//if (ii + 1 == nWidth)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_in[nSy][ii + 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii < nWidth - 1; )
				//TRACE("\r\n");

				/*中 -> 左  尋找下一點*/
				nSx = i;		//取 X 座標
				nSy = Hc[n][i];	//取 Y 座標
				//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向左尋找 */
				for (ii = nSx; ii > 0; ) {
					ii = ii - 1;	//將 X 座標左移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							Hc[n][ii] = sC1y;
							//if (ii - 1 < 0)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_in[nSy][ii - 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength - 1) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}
				n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (j = 0; j < nHeight; j++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vertical Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum + 1);
	LOGD(buffer);

	int nMin, nMax, nCntNum;
	bool bFirstTime;
	for (n = 0; n < (nRecordNum + 1); n++) {
		nMin = 0;
		nMax = 0;
		nCntNum = 0;
		bFirstTime = false;
		for (ii = 0; ii < nWidth; ii++) {
			/*取 Y 座標*/
			if (Hc[n][ii] != 0) {
				/*Calculation Y Up and Down Width*/
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d /******\r\n", __FUNCTION__, __LINE__);//KennyKang
				LOGD(buffer);
				GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, Hc[n][ii], Uw[n], Dw[n], n);//KennyKang
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Hc[%d][%d] = %d / Uw[%d][%d] = %d / Dw[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, n, ii, Hc[n][ii], n, ii, Uw[n][ii], n, ii, Dw[n][ii]);//KennyKang
				LOGD(buffer);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d ******/\r\n", __FUNCTION__, __LINE__);//KennyKang
				LOGD(buffer);

				nCntNum++;
				if (bFirstTime == false) {
					nMin = ii;
					bFirstTime = true;
				}

				if (ii > nMax)
					nMax = ii;
				if (ii <= nMin)
					nMin = ii;
			}
		}
		Hhead[n] = nMin;
		Htail[n] = nMax;
		Hlenght[n] = nMax - nMin + 1;
	}//for (n = 0; n < (nRecordNum + 1); n++)

	int nRecordCnt = 0;
	for (n = 0; n < (nRecordNum + 1); n++) {
		if (Hlenght[n] >= nConnectingTh) {
			nRecordCnt++;
		}
	}
	sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vertical nRecordCnt = %d\r\n", __FUNCTION__, __LINE__, nRecordCnt);
	LOGD(buffer);

	if (nRecordCnt != 0) {
		/* Save Vertical Data to Struct */
		InitVerticalAlgoMemory(nRecordCnt);
		SetVerticalLength(nRecordCnt);

		int nItemCnt = 0;
		for (n = 0; n < (nRecordNum + 1); n++) {
			/*取 Y 座標*/
			if (Hlenght[n] >= nConnectingTh) {
				/* 儲存資料 */
				pVerticalTextureInfo->VerticalItem[nItemCnt].Head = Hhead[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Tail = Htail[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Length = Hlenght[n];
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n]);
				//LOGD(buffer);

#if 0
				pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical = (struct txPOINT *) malloc(sizeof(struct txPOINT) * Hlenght[n]);
				memset(pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, 0, sizeof(struct txPOINT) * Hlenght[n]);

				int nCnt = 0;
				for (i = 0; i < nWidth; i++) {
					/*取 Y 座標*/
					if (Hc[n][i] != 0) {
						pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical[nCnt].x = i;		  //存 X 座標
						pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical[nCnt].y = Hc[n][i];//存 Y 座標
						sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nCnt = %d] / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, i, Hc[n][i], pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical[nCnt].x, pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical[nCnt].y);
						LOGD(buffer);
						nCnt++;
					}
				}
#endif
				SetVerticalConnectingData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, Hc[n], Hhead[n], Htail[n], Hlenght[n], Uw[n], Dw[n], NULL, nItemCnt);
				nItemCnt++;
			}
		}

#if 0
		if (pVerticalTextureInfo != NULL) {
			for (i = 0; i < nRecordCnt; i++) {
				if (pVerticalTextureInfo->VerticalItem[i].pVertical != NULL)
					free(pVerticalTextureInfo->VerticalItem[i].pVertical);
			}
			free(pVerticalTextureInfo->VerticalItem);
			free(pVerticalTextureInfo);
			pVerticalTextureInfo = NULL;
		}
#endif
	}//if (nRecordCnt != 0)
	
	 //Celan Labeling memory for Connecting Use
	for (j = 0; j < nHeight; j++)
		memset(pLabelingData_in[j], 255, nWidth);

	for (n = 0; n < (nRecordNum + 1); n++) {
		for (i = 0; i < nWidth; i++) {
			/*取 Y 座標*/
			if (Hc[n][i] != 0 && Hlenght[n] >= nConnectingTh) {
				pLabelingData_in[Hc[n][i]][i] = Vx * 2;
			}
		}
	}
}

void CheckHorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkVc[IMAGE_HEIGHT] = { 0 };				//Check Vc
	unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[nRecordNum][nStartY] 存放 X 座標
	unsigned char Lw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的左寬
	unsigned char Rw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的右寬
	char Vw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };			//根據 X 的左、右寬所計算出來的 Weight
	unsigned char Vj[IMAGE_HEIGHT] = { 0 };						//X 的跳躍點
	unsigned char checkVb[IMAGE_HEIGHT] = { 0 };				//Check Vb
	unsigned char Vb[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//線段頭 Y 的座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//線段尾 Y 的座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 };	//線段 Y 的總長度
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 Y 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 Y 的品質(後三點)

	unsigned char sC1x;
	char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int jj, j1, pj, nj, jLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;
	char buffer[1000] = { 0 };

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先上再下的控制 */
	for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

		if (jLoop == 0 && jLoop % 2 == 0) {
			j1 = 0;
			j = nStartY + j1;
			nj = pj = j;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else if (jLoop > 0 && jLoop % 2 == 0) {
			j1 = 1;//下
			pj = pj + j1;
			j = pj;
			if (j > nHeight - 1)
				j = nHeight - 1;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else {
			j1 = -1;//上
			nj = nj + j1;
			j = nj;
			if (j < 0)
				j = 0;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}

		/* X Right -> Left */
		for (i = 0; i < nWidth; i++) {

			memset(checkVc, 0, IMAGE_HEIGHT);
			memset(checkVb, 0, IMAGE_HEIGHT);

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkVc[j] = i;	//存 X 座標
				//Vc[n][j] = i;	//存 X 座標
				//nRecordNum = n;
				//TRACE("n = %d / checkVc[%d] = %d / (%d, %d)\r\n", n, j, i, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*中 -> 下 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				//nSx = Vc[n][j];	//取 X 座標
				nSy = j;		//取 Y 座標
				//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向下尋找 */
				for (jj = nSy; jj < nHeight - 1; ) {
					jj = jj + 1;	//將 Y 座標下移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj + 1 == nHeight)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_in[jj + 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx/* || pLabelingData_in[jj][sC1x] == V1*/) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj < nHeight - 1; )
				//TRACE("\r\n");

				/*中 -> 上 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				//nSx = Vc[n][j];	//取 X 座標
				nSy = j;		//取 Y 座標
				//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向上尋找 */
				for (jj = nSy; jj > 0; ) {
					jj = jj - 1;	//將 Y 座標上移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj - 1 < 0)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx/* || pLabelingData_in[jj][sC1x] == V1*/) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength/* - 1*/) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0;
				nWeightSum = 0;
				int nRes = CheckConnectLength(&checkVc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvHorizontal);
				//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d\r\n", n, nRes);
				if (nRes == 1) {
					//CheckBifurcation(&checkVc[0], &checkVb[0], n, emhvHorizontal);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nHeight; nId++) {
						/*取 X 座標 */
						if (checkVc[nId] != 0) {
							Vc[n][nId] = checkVc[nId];

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][nId], nId, Lw[n], Rw[n], n);//KennyKang
							
							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, Lw[n], Rw[n], Vw[n], Vc[n][nId], nId, n);//KennyKang
							nWeightSum = nWeightSum + Vw[n][nId];//KennyKang

							/*Check V0 Connecting Quality Status*/
							//CheckV0ConnectingQuality(pLabelingData_in, Vw[n], Vc[n][nId], nId, n, QualityTh, emhvHorizontal);//KennyKang
						}

						if (checkVb[nId] != 0) {
							Vb[n][nId] = checkVb[nId];
							nBifurcationCnt++;
							sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vb[%d][%d] = %d / nBifurcationCnt = %d\r\n", __FUNCTION__, __LINE__, n, nId, Vb[n][nId], nBifurcationCnt);//KennyKang
							LOGD(buffer);
						}
					}
					
					Vhead[n] = nMin;
					Vtail[n] = nMax;
					Vlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;
					//TRACE("Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n]);

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (i = 0; i < nWidth; i++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Horizontal Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum + 1);
		LOGD(buffer);
	}
	else {
		sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Horizontal Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum);
		LOGD(buffer);
	}

	if ((nRecordNum + 1) > 0) {
		/* Save Horizontal Data to Struct */
		InitHorizontalAlgoMemory(nRecordNum + 1);
		SetHorizontalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		for (n = 0; n < (nRecordNum + 1); n++) {
			/*取 X 座標*/
			if (Vlenght[n] >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Vhead[n], Vtail[n], Vlenght[n], Vw[n], n, QualityTh, emhvHorizontal, &nNewHead, &nNewTail);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				LOGD(buffer);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength = 2;
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pEndingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength, Vc[n], nNewHead, nNewTail, emhvHorizontal);
				}

				/* Setting Jumping Minutiae Data */
				memset(Vj, 0, IMAGE_HEIGHT);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Vc[n], nThJumpingLength, &Vj[0], emhvHorizontal);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] pHorizontalTextureInfo->HorizontalItem[%d].nJumpingLength = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, nItemCnt, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength);
				LOGD(buffer);
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pJumpingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength, &Vj[0], emhvHorizontal);
				}

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Vc[n], Vb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvHorizontal);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Vc[n], Vb, (nRecordNum + 1), emhvHorizontal);
					pHorizontalTextureInfo->HorizontalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Horizontal] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", __FUNCTION__, __LINE__, nBifurcationLength, nCurrentBifurcationArrayIDx);
				LOGD(buffer);

				/* 儲存資料 */
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Head = Vhead[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Tail = Vtail[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Length = Vlenght[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].WeightSum = WeightSum[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bTailQuality = bTailQuality[n];
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);
				LOGD(buffer);

				SetHorizontalConnectingData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pHorizontal, Vc[n], Vhead[n], Vtail[n], Vlenght[n], Lw[n], Rw[n], Vw[n], nItemCnt);
				nItemCnt++;

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Vw[n], Vc[n], n, QualityTh, emhvHorizontal);//KennyKang
			}
		}

		int BifurcationY;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationY = pBifurcationInfo[i].xY;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Vhead[pBifurcationInfo[i].xL], Vtail[pBifurcationInfo[i].xL], Vlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationY, emhvHorizontal);//KennyKang
			/* 更新資料結構 */
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (j = 0; j < nHeight; j++) {
				/*取 X 座標 */
				if (Vc[n][j] != 0) {
					pLabelingData_in[j][Vc[n][j]] = Vx * 2;
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}

void CheckVerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkHc[IMAGE_WIDTH] = { 0 };				    //Check Hc
	unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Hc[nRecordNum][nStartX] 存放 Y 座標
	unsigned char Uw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的上寬
	unsigned char Dw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的下寬
	char Hw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };			//根據 Y 的上、下寬所計算出來的 Weight
	unsigned char Hj[IMAGE_WIDTH] = { 0 };						//Y 的跳躍點
	unsigned char checkHb[IMAGE_WIDTH] = { 0 };					//Check Hb
	unsigned char Hb[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量

	unsigned char Hhead[MaxRecordItemLength] = { 0 };	//線段頭 X 的座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };	//線段尾 X 的座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };	//線段 X 的總長度 
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 X 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 X 的品質(後三點)

	unsigned char sC1y;
	char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int ii, i1, pi, ni, iLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;
	char buffer[1000] = { 0 };

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先左再右的控制 */
	for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

		if (iLoop == 0 && iLoop % 2 == 0) {
			i1 = 0;
			i = nStartX + i1;
			ni = pi = i;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else if (iLoop > 0 && iLoop % 2 == 0) {
			i1 = 1;
			pi = pi + i1;
			i = pi;
			if (i > nWidth - 1)
				i = nWidth - 1;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else {
			i1 = -1;
			ni = ni + i1;
			i = ni;
			if (i < 0)
				i = 0;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}

		/* Y Top - > Down */
		for (j = 0; j < nHeight; j++) {

			memset(checkHc, 0, IMAGE_WIDTH);
			memset(checkHb, 0, IMAGE_WIDTH);

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkHc[i] = j;	//存 Y 座標
				//Hc[n][i] = j;	//存 Y 座標
				//nRecordNum = n;
				//TRACE("checkHc[%3d][%3d] = %3d\r\n", n, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/* 中 -> 右 尋找下一點 */
				nSx = i;			//取 X 座標
				//nSy = Hc[n][i];	//取 Y 座標
				nSy = checkHc[i];	//取 Y 座標
				//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向右尋找 */
				for (ii = nSx; ii < nWidth - 1; ) {
					ii = ii + 1;	//將 X 座標右移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii + 1 == nWidth)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_in[nSy][ii + 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx/* || pLabelingData_in[sC1y][ii] == V1*/) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii < nWidth - 1; )
				//TRACE("\r\n");

				/*中 -> 左  尋找下一點*/
				nSx = i;		//取 X 座標
				//nSy = Hc[n][i];	//取 Y 座標
				nSy = checkHc[i];//取 Y 座標
				//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向左尋找 */
				for (ii = nSx; ii > 0; ) {
					ii = ii - 1;	//將 X 座標左移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii - 1 < 0)
								//TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
								//TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) /  %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_in[nSy][ii - 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx/* || pLabelingData_in[sC1y][ii] == V1*/) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength/* - 1*/) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0;
				nWeightSum = 0;
				int nRes = CheckConnectLength(&checkHc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvVertical);
				//TRACE("[Vertical Cehck Length ... n = %d / nRes = %d\r\n", n, nRes);
				if (nRes == 1) {
					//CheckBifurcation(&checkHc[0], &checkHb[0], n, emhvVertical);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nWidth; nId++) {
						/*取 Y 座標*/
						if (checkHc[nId] != 0) {
							Hc[n][nId] = checkHc[nId];

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, nId, Hc[n][nId], Uw[n], Dw[n], n);//KennyKang

							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, Uw[n], Dw[n], Hw[n], nId, Hc[n][nId], n);//KennyKang
							nWeightSum = nWeightSum + Hw[n][nId];//KennyKang
							
							/*Check V0 Connecting Quality Status*/
							//CheckV0ConnectingQuality(pLabelingData_in, Hw[n], nId, Hc[n][nId], n, QualityTh, emhvVertical);//KennyKang
						}

						if (checkHb[nId] != 0) {
							Hb[n][nId] = checkHb[nId];
							nBifurcationCnt++;
							sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Hb[%d][%d] = %d / nBifurcationCnt = %d\r\n", __FUNCTION__, __LINE__, n, nId, Hb[n][nId], nBifurcationCnt);//KennyKang
							LOGD(buffer);
						}
					}
					
					Hhead[n] = nMin;
					Htail[n] = nMax;
					Hlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;
					//TRACE("Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n]);

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (j = 0; j < nHeight; j++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vertical Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum + 1);
		LOGD(buffer);
	}
	else {
		sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Vertical Total Record Line Number = %d\r\n", __FUNCTION__, __LINE__, nRecordNum);
		LOGD(buffer);
	}

	if ((nRecordNum + 1) > 0) {
		/* Save Vertical Data to Struct */
		InitVerticalAlgoMemory(nRecordNum + 1);
		SetVerticalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		for (n = 0; n < (nRecordNum + 1); n++) {
			/*取 Y 座標*/
			if (Hlenght[n] >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Hhead[n], Htail[n], Hlenght[n], Hw[n], n, QualityTh, emhvVertical, &nNewHead, &nNewTail);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				LOGD(buffer);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength = 2;
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pEndingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength, Hc[n], nNewHead, nNewTail, emhvVertical);
				}
				
				/* Setting Jumping Minutiae Data */
				memset(Hj, 0, IMAGE_WIDTH);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Hc[n], nThJumpingLength, &Hj[0], emhvVertical);
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] pVerticalTextureInfo->VerticalItem[%d].nJumpingLength = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, nItemCnt, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength);
				LOGD(buffer);
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pJumpingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength, &Hj[0], emhvVertical);
				}

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Hc[n], Hb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvVertical);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Hc[n], Hb, (nRecordNum + 1), emhvVertical);
					pVerticalTextureInfo->VerticalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Vertical] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", __FUNCTION__, __LINE__, nBifurcationLength, nCurrentBifurcationArrayIDx);
				LOGD(buffer);

				/* 儲存資料 */
				pVerticalTextureInfo->VerticalItem[nItemCnt].Head = Hhead[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Tail = Htail[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Length = Hlenght[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].WeightSum = WeightSum[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bTailQuality = bTailQuality[n];
				sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", __FUNCTION__, __LINE__, nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);
				LOGD(buffer);

				SetVerticalConnectingData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, Hc[n], Hhead[n], Htail[n], Hlenght[n], Uw[n], Dw[n], Hw[n], nItemCnt);
				nItemCnt++;

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Hw[n], Hc[n], n, QualityTh, emhvVertical);//KennyKang
			}
		}

		int BifurcationX;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationX = pBifurcationInfo[i].xX;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Hhead[pBifurcationInfo[i].xL], Htail[pBifurcationInfo[i].xL], Hlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationX, emhvVertical);//KennyKang
																																																																						  /* 更新資料結構 */
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (i = 0; i < nWidth; i++) {
				/*取 Y 座標*/
				if (Hc[n][i] != 0) {
					pLabelingData_in[Hc[n][i]][i] = Vx * 2;
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}

void CheckHorizontalConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkVc[IMAGE_HEIGHT] = { 0 };				//Check Vc
	unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[nRecordNum][nStartY] 存放 X 座標
	unsigned char checkLw[IMAGE_HEIGHT] = { 0 };				//Check Lw
	unsigned char Lw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的左寬
	unsigned char checkRw[IMAGE_HEIGHT] = { 0 };				//Check Rw
	unsigned char Rw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的右寬
	char checkVw[IMAGE_HEIGHT] = { 0 };							//Check Vw
	char Vw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };			//根據 X 的左、右寬所計算出來的 Weight
	unsigned char checkVb[IMAGE_HEIGHT] = { 0 };				//Check Vb
	unsigned char Vb[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量
	unsigned char Vj[IMAGE_HEIGHT] = { 0 };						//X 的跳躍點

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//線段頭 Y 的座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//線段尾 Y 的座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 }; //線段 Y 的總長度
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 Y 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 Y 的品質(後三點)

	unsigned char sC1x;
	char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int jj, j1, pj, nj, jLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先上再下的控制 */
	for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

		if (jLoop == 0 && jLoop % 2 == 0) {
			j1 = 0;
			j = nStartY + j1;
			nj = pj = j;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else if (jLoop > 0 && jLoop % 2 == 0) {
			j1 = 1;//下
			pj = pj + j1;
			j = pj;
			if (j > nHeight - 1)
				j = nHeight - 1;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else {
			j1 = -1;//上
			nj = nj + j1;
			j = nj;
			if (j < 0)
				j = 0;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}

		/* X Right -> Left */
		for (i = 0; i < nWidth; i++) {

			memset(checkVc, 0, IMAGE_HEIGHT);
			memset(checkLw, 0, IMAGE_HEIGHT);
			memset(checkRw, 0, IMAGE_HEIGHT);
			memset(checkVw, 0, IMAGE_HEIGHT);
			memset(checkVb, 0, IMAGE_HEIGHT);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkVc[j] = i;	//存 X 座標				
				//TRACE("n = %d / checkVc[%d] = %d / (%d, %d)\r\n", n, j, i, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation X Left and Right Width*/
				GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[j], j, &checkLw[0], &checkRw[0], n);//KennyKang
				/*Check X Left and Right Width Quality and Calculation Weight Sum*/
				CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[j], j, n);//KennyKang
				//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", j, checkVc[j], j, checkLw[j], j, checkRw[j], j, checkVw[j]);//KennyKang
				nWeightSum = nWeightSum + checkVw[j];//KennyKang

				/*中 -> 下 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向下尋找 */
				for (jj = nSy; jj < nHeight - 1; ) {
					jj = jj + 1;	//將 Y 座標下移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj + 1 == nHeight)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_in[jj + 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
							//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
							nWeightSum = nWeightSum + checkVw[jj];//KennyKang
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj < nHeight - 1; )
				//TRACE("\r\n");

				 /*中 -> 上 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向上尋找 */
				for (jj = nSy; jj > 0; ) {
					jj = jj - 1;	//將 Y 座標上移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
							//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
							nWeightSum = nWeightSum + checkVw[jj];//KennyKang
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&checkVc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(&checkVc[0], nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {
					//CheckBifurcation(&checkVc[0], &checkVb[0], n, emhvHorizontal);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nHeight; nId++) {
						/*取 X 座標 */
						if (checkVc[nId] != 0) {
							Vc[n][nId] = checkVc[nId];
							Lw[n][nId] = checkLw[nId];
							Rw[n][nId] = checkRw[nId];
							Vw[n][nId] = checkVw[nId];
							//TRACE("checkVc[%d] = %d / Vc[%d][%d] = %d / Lw[%d][%d] = %d / Rw[%d][%d] = %d / Vw[%d][%d] = %d\r\n", nId, checkVc[nId], n, nId, Vc[n][nId], n, nId, Lw[n][nId], n, nId, Rw[n][nId], n, nId, Vw[n][nId]);//KennyKang
						}

						if (checkVb[nId] != 0) {
							Vb[n][nId] = checkVb[nId];
							nBifurcationCnt++;
							//TRACE("Vb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Vb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Vhead[n] = nMin;
					Vtail[n] = nMax;
					Vlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;//KennyKang
					//TRACE("Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (i = 0; i < nWidth; i++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
	}
	else {
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Horizontal Data to Struct */
		InitHorizontalAlgoMemory(nRecordNum + 1);
		SetHorizontalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Vlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Vlenght[n] + WeightSum[n];
			}

			/* 取 X 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Vhead[n], Vtail[n], Vlenght[n], Vw[n], n, QualityTh, emhvHorizontal, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength = 2;
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pEndingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength, Vc[n], nNewHead, nNewTail, emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Vj, 0, IMAGE_HEIGHT);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Vc[n], nThJumpingLength, &Vj[0], emhvHorizontal);
				//TRACE("[nItemCnt = %d] pHorizontalTextureInfo->HorizontalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength);
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pJumpingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength, &Vj[0], emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Vc[n], Vb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvHorizontal);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Vc[n], Vb, (nRecordNum + 1), emhvHorizontal);
					pHorizontalTextureInfo->HorizontalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Horizontal] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Head = Vhead[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Tail = Vtail[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Length = Vlenght[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].WeightSum = WeightSum[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetHorizontalConnectingData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pHorizontal, Vc[n], Vhead[n], Vtail[n], Vlenght[n], Lw[n], Rw[n], Vw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Vw[n], Vc[n], n, QualityTh, emhvHorizontal);//KennyKang
				//TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateHorizontalLength(nItemCnt);
		//TRACE("HorizontalLength = %d / UpdateHorizontalLength = %d \r\n", GetHorizontalLength(), GetUpdateHorizontalLength());
		//TRACE("\r\n");

		int BifurcationY;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationY = pBifurcationInfo[i].xY;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Vhead[pBifurcationInfo[i].xL], Vtail[pBifurcationInfo[i].xL], Vlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationY, emhvHorizontal);//KennyKang
																																																																							/* 更新資料結構 */
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (j = 0; j < nHeight; j++) {
				if (bLengthAndQuality == false) {
					/* 取 X 座標 / 再次過濾 L 條件 */
					if (Vc[n][j] != 0 && (Vlenght[n] >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
				else {
					/* 取 X 座標 / 再次過濾 L + Q 條件 */
					if (Vc[n][j] != 0 && ((Vlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}

void CheckVerticalConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkHc[IMAGE_WIDTH] = { 0 };					//Check Hc
	unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Hc[nRecordNum][nStartX] 存放 Y 座標
	unsigned char checkUw[IMAGE_WIDTH] = { 0 };					//Check Uw
	unsigned char Uw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Y 的上寬
	unsigned char checkDw[IMAGE_WIDTH] = { 0 };					//Check Dw
	unsigned char Dw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Y 的下寬
	char checkHw[IMAGE_WIDTH] = { 0 };							//Check Hw
	char Hw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };			//根據 Y 的上、下寬所計算出來的 Weight
	unsigned char checkHb[IMAGE_WIDTH] = { 0 };					//Check Hb
	unsigned char Hb[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量
	unsigned char Hj[IMAGE_WIDTH] = { 0 };						//Y 的跳躍點

	unsigned char Hhead[MaxRecordItemLength] = { 0 };	//線段頭 X 的座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };	//線段尾 X 的座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };	//線段 X 的總長度 
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 X 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 X 的品質(後三點)

	unsigned char sC1y;
	char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int ii, i1, pi, ni, iLoop, nArrayIDx;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先左再右的控制 */
	for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

		if (iLoop == 0 && iLoop % 2 == 0) {
			i1 = 0;
			i = nStartX + i1;
			ni = pi = i;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else if (iLoop > 0 && iLoop % 2 == 0) {
			i1 = 1;
			pi = pi + i1;
			i = pi;
			if (i > nWidth - 1)
				i = nWidth - 1;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else {
			i1 = -1;
			ni = ni + i1;
			i = ni;
			if (i < 0)
				i = 0;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}

		/* Y Top - > Down */
		for (j = 0; j < nHeight; j++) {

			memset(checkHc, 0, IMAGE_WIDTH);
			memset(checkUw, 0, IMAGE_WIDTH);
			memset(checkDw, 0, IMAGE_WIDTH);
			memset(checkHw, 0, IMAGE_WIDTH);
			memset(checkHb, 0, IMAGE_WIDTH);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkHc[i] = j;	//存 Y 座標
				//TRACE("n = %3d / checkHc[%3d] = %3d / (%d, %d)\r\n", n, i, j, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation Y Up and Down Width*/
				GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, i, checkHc[i], &checkUw[0], &checkDw[0], n);//KennyKang
				/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
				CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], i, checkHc[i], n);//KennyKang
				//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", i, checkHc[i], i, checkUw[i], i, checkDw[i], i, checkHw[i]);//KennyKang
				nWeightSum = nWeightSum + checkHw[i];//KennyKang

				/* 中 -> 右 尋找下一點 */
				nSx = i;		 //取 X 座標
				nSy = checkHc[i];//取 Y 座標
				//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向右尋找 */
				for (ii = nSx; ii < nWidth - 1; ) {
					ii = ii + 1;	//將 X 座標右移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii + 1 == nWidth)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_in[nSy][ii + 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
							//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
							nWeightSum = nWeightSum + checkHw[ii];//KennyKang
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii < nWidth - 1; )
				//TRACE("\r\n");

				/*中 -> 左  尋找下一點*/
				nSx = i;		 //取 X 座標
				nSy = checkHc[i];//取 Y 座標
				//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向左尋找 */
				for (ii = nSx; ii > 0; ) {
					ii = ii - 1;	//將 X 座標左移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_in[nSy][ii - 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
							//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
							nWeightSum = nWeightSum + checkHw[ii];//KennyKang
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
						else {
							//全部找完還是沒找到
							if (nArrayIDx == MaxC1Len - 1) {
								//TRACE("Not Find\r\n");
								bFind = false;
							}
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&checkHc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(&checkHc[0], nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {
					//CheckBifurcation(&checkHc[0], &checkHb[0], n, emhvVertical);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nWidth; nId++) {
						/*取 Y 座標*/
						if (checkHc[nId] != 0) {
							Hc[n][nId] = checkHc[nId];
							Uw[n][nId] = checkUw[nId];
							Dw[n][nId] = checkDw[nId];
							Hw[n][nId] = checkHw[nId];
							//TRACE("checkHc[%d] = %d / Hc[%d][%d] = %d / Uw[%d][%d] = %d / Dw[%d][%d] = %d / Hw[%d][%d] = %d\r\n", nId, checkHc[nId], n, nId, Hc[n][nId], n, nId, Uw[n][nId], n, nId, Dw[n][nId], n, nId, Hw[n][nId]);//KennyKang
						}

						if (checkHb[nId] != 0) {
							Hb[n][nId] = checkHb[nId];
							nBifurcationCnt++;
							//TRACE("Hb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Hb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Hhead[n] = nMin;
					Htail[n] = nMax;
					Hlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;
					//TRACE("Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (j = 0; j < nHeight; j++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
	}
	else {
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Vertical Data to Struct */
		InitVerticalAlgoMemory(nRecordNum + 1);
		SetVerticalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Hlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Hlenght[n] + WeightSum[n];
			}

			/* 取 Y 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Hhead[n], Htail[n], Hlenght[n], Hw[n], n, QualityTh, emhvVertical, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength = 2;
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pEndingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength, Hc[n], nNewHead, nNewTail, emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Hj, 0, IMAGE_WIDTH);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Hc[n], nThJumpingLength, &Hj[0], emhvVertical);
				//TRACE("[nItemCnt = %d] pVerticalTextureInfo->VerticalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength);
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pJumpingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength, &Hj[0], emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Hc[n], Hb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvVertical);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Hc[n], Hb, (nRecordNum + 1), emhvVertical);
					pVerticalTextureInfo->VerticalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Vertical] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pVerticalTextureInfo->VerticalItem[nItemCnt].Head = Hhead[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Tail = Htail[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Length = Hlenght[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].WeightSum = WeightSum[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetVerticalConnectingData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, Hc[n], Hhead[n], Htail[n], Hlenght[n], Uw[n], Dw[n], Hw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Hw[n], Hc[n], n, QualityTh, emhvVertical);//KennyKang
				//TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateVerticalLength(nItemCnt);
		//TRACE("VerticalLength = %d / UpdateVerticalLength = %d \r\n", GetVerticalLength(), GetUpdateVerticalLength());
		//TRACE("\r\n");

		int BifurcationX;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationX = pBifurcationInfo[i].xX;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Hhead[pBifurcationInfo[i].xL], Htail[pBifurcationInfo[i].xL], Hlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationX, emhvVertical);//KennyKang
																																																																						  /* 更新資料結構 */
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (i = 0; i < nWidth; i++) {
				if (bLengthAndQuality == false) {
					/* 取 Y 座標 / 再次過濾 L 條件 */
					if (Hc[n][i] != 0 && (Hlenght[n] >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
				else {
					/* 取 Y 座標 / 再次過濾 L + Q 條件 */
					if (Hc[n][i] != 0 && ((Hlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}

#ifdef EMC_Embedded
void emCheckHorizontalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	//unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[nRecordNum][nStartY] 存放 X 座標
	//unsigned char Vb[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的分岔點
	//char          Vw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//根據 X 的左、右寬所計算出來的 Weight

	char buffer[1000] = { 0 };
	unsigned char checkLw[IMAGE_HEIGHT] = { 0 };		//Check Lw
	unsigned char checkRw[IMAGE_HEIGHT] = { 0 };		//Check Rw
	unsigned char Vj[IMAGE_HEIGHT] = { 0 };				//X 的跳躍點

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//線段頭 Y 的座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//線段尾 Y 的座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 }; //線段 Y 的總長度
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 Y 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 Y 的品質(後三點)

	int nBifurcationCnt = 0;							//分岔點的總數量

	unsigned char sC1x;
	char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int jj, j1, pj, nj, jLoop, nArrayIDx, nArrayCnt, nArrayX, nArrayY;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	unsigned char **Vc = NULL;
	unsigned char **Vb = NULL;
	char **Vw = NULL;
	Vc = (unsigned char **)malloc(sizeof(unsigned char *) * MaxRecordItemLength);
	Vb = (unsigned char **)malloc(sizeof(unsigned char *) * MaxRecordItemLength);
	Vw = (char **)malloc(sizeof(char *) * MaxRecordItemLength);
	for (n = 0; n < MaxRecordItemLength; n++) {
		Vc[n] = (unsigned char *)malloc(sizeof(unsigned char) * IMAGE_HEIGHT);
		memset(Vc[n], 0, IMAGE_HEIGHT);
		Vb[n] = (unsigned char *)malloc(sizeof(unsigned char) * IMAGE_HEIGHT);
		memset(Vb[n], 0, IMAGE_HEIGHT);
		Vw[n] = (char *)malloc(sizeof(char) * IMAGE_HEIGHT);
		memset(Vw[n], 0, IMAGE_HEIGHT);
	}

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先上再下的控制 */
	for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

		if (jLoop == 0 && jLoop % 2 == 0) {
			j1 = 0;
			j = nStartY + j1;
			nj = pj = j;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else if (jLoop > 0 && jLoop % 2 == 0) {
			j1 = 1;//下
			pj = pj + j1;
			j = pj;
			if (j > nHeight - 1)
				j = nHeight - 1;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else {
			j1 = -1;//上
			nj = nj + j1;
			j = nj;
			if (j < 0)
				j = 0;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}

		/* X Right -> Left */
		for (i = 0; i < nWidth; i++) {
			memset(Vc[n], 0, IMAGE_HEIGHT);
			memset(Vw[n], 0, IMAGE_HEIGHT);
			memset(Vb[n], 0, IMAGE_HEIGHT);
			memset(checkLw, 0, IMAGE_HEIGHT);
			memset(checkRw, 0, IMAGE_HEIGHT);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				//checkVc[j] = i;	//存 X 座標	
				//TRACE("n = %d / checkVc[%d] = %d / (%d, %d)\r\n", n, j, i, i, j);
				Vc[n][j] = i;		//存 X 座標
				//TRACE("Vc[%d][%d] = %d / (%d, %d)\r\n", n, j, i, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation X Left and Right Width*/
				GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][j], j, &checkLw[0], &checkRw[0], n);//KennyKang

				/*Check X Left and Right Width Quality and Calculation Weight Sum*/
				CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &Vw[n][0]/*&checkVw[0]*/, Vc[n][j], j, n);//KennyKang
				nWeightSum = nWeightSum + Vw[n][j]/*checkVw[j]*/;//KennyKang

				/*中 -> 下 尋找下一點*/
				nSx = Vc[n][j];		//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向下尋找 */
				for (jj = nSy; jj < nHeight - 1; ) {
					jj = jj + 1;	//將 Y 座標下移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, sC1x, jj);
						if (nArrayIDx == 0) {
							nArrayX = sC1x;
							nArrayY = jj;
						}

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							Vc[n][jj] = sC1x;
							//checkVc[jj] = sC1x;
							//if (jj + 1 == nHeight)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_in[jj + 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][jj]/*checkVc[jj]*/, jj, &checkLw[0], &checkRw[0], n);//KennyKang

							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &Vw[n][0]/*&checkVw[0]*/, Vc[n][jj], jj, n);//KennyKang
							nWeightSum = nWeightSum + Vw[n][jj]/*checkVw[jj]*/;//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx) {
							//checkVb[jj] = sC1x;
							Vb[n][jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {

						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Down ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Down ... Not Find / (%d, %d) / (%d, %d)\r\n", nSx, jj, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (/*nSx*/nArrayX + nC1x[nArrayIDx] > nWidth - 1)
									nDx = nWidth - 1;
								else if (/*nSx*/nArrayX + nC1x[nArrayIDx] < 1)
									nDx = 1;
								else
									nDx = /*nSx*/nArrayX + nC1x[nArrayIDx];

								nDy = nArrayY;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[jj][nDx] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], jj, nDx, pLabelingData_in[jj][nDx]);

									bFind = true;
									nSx = nDx;
									//checkVc[jj] = nDx;
									Vc[n][jj] = nDx;
									//if (jj + 1 == nHeight)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx], nDx, jj + 1, pLabelingData_in[jj + 1][nDx]);

									//將該點標記 V0/VL 改變為 Vx
									pLabelingData_in[jj][nDx] = Vx;

									/*Calculation X Left and Right Width*/
									GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][jj]/*checkVc[jj]*/, jj, &checkLw[0], &checkRw[0], n);//KennyKang

									/*Check X Left and Right Width Quality and Calculation Weight Sum*/
									CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &Vw[n][0]/*&checkVw[0]*/, Vc[n][jj]/*checkVc[jj]*/, jj, n);//KennyKang
									nWeightSum = nWeightSum + Vw[n][jj]/*checkVw[jj]*/;//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
									//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj < nHeight - 1; )
				 //TRACE("\r\n");

				/*中 -> 上 尋找下一點*/
				//nSx = checkVc[j];	//取 X 座標
				nSx = Vc[n][j];		//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向上尋找 */
				for (jj = nSy; jj > 0; ) {
					jj = jj - 1;	//將 Y 座標上移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d) / nArrayCnt = %d\r\n", nArrayIDx, sC1x, jj, nArrayCnt);
						if (nArrayIDx == 0) {
							nArrayX = sC1x;
							nArrayY = jj;
						}

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							Vc[n][jj] = sC1x;
							//checkVc[jj] = sC1x;
							//if (jj - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][jj]/*checkVc[jj]*/, jj, &checkLw[0], &checkRw[0], n);//KennyKang

							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &Vw[n][0]/*&checkVw[0]*/, Vc[n][jj]/*checkVc[jj]*/, jj, n);//KennyKang
							nWeightSum = nWeightSum + Vw[n][jj]/*checkVw[jj]*/;//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx) {
							//checkVb[jj] = sC1x;
							Vb[n][jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {

						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Up ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Up ... Not Find / (%d, %d) / (%d, %d)\r\n", nSx, jj, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSx + nC1x[nArrayIDx] > nWidth - 1)
									nDx = nWidth - 1;
								else if (nSx + nC1x[nArrayIDx] < 1)
									nDx = 1;
								else
									nDx = nSx + nC1x[nArrayIDx];

								nDy = nArrayY;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[jj][nDx] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], jj, nDx, pLabelingData_in[jj][nDx]);

									bFind = true;
									nSx = nDx;
									//checkVc[jj] = nDx;
									Vc[n][jj] = nDx;
									//if (jj - 1 < 0)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

									//將該點標記 V0/VL 改變為 Vx
									pLabelingData_in[jj][nDx] = Vx;

									/*Calculation X Left and Right Width*/
									GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, Vc[n][jj]/*checkVc[jj]*/, jj, &checkLw[0], &checkRw[0], n);//KennyKang

									/*Check X Left and Right Width Quality and Calculation Weight Sum*/
									CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &Vw[n][0]/*&checkVw[0]*/, Vc[n][jj]/*checkVc[jj]*/, jj, n);//KennyKang
									nWeightSum = nWeightSum + Vw[n][jj]/*checkVw[jj]*/;//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if (nArrayIDx == MaxC1Len - 1)
									//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj > 0; )
				 //TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&Vc[n][0]/*&checkVc[0]*/, nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(Vc[n]/*&checkVc[0]*/, nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nHeight; nId++) {
						if (Vb[n][nId] != 0) {
							nBifurcationCnt++;
							//TRACE("Vb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Vb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Vhead[n] = nMin;
					Vtail[n] = nMax;
					Vlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;//KennyKang
					//TRACE("Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//TRACE("\r\n");
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (i = 0; i < nWidth; i++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		sprintf(buffer, "Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		LOGD(buffer);
	}
	else {
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		sprintf(buffer, "Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		LOGD(buffer);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Horizontal Data to Struct */
		InitHorizontalAlgoMemory(nRecordNum + 1);
		SetHorizontalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Vlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Vlenght[n] + WeightSum[n];
			}

			/* 取 X 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Vhead[n], Vtail[n], Vlenght[n], Vw[n], n, QualityTh, emhvHorizontal, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength = 2;
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pEndingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength, Vc[n], nNewHead, nNewTail, emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Vj, 0, IMAGE_HEIGHT);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Vc[n], nThJumpingLength, &Vj[0], emhvHorizontal);
				//TRACE("[nItemCnt = %d] pHorizontalTextureInfo->HorizontalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength);
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pJumpingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength, &Vj[0], emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				//nBifurcationLength = CheckConnectingToBifurcationMinutiae(Vc[n], Vb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvHorizontal);
				nBifurcationLength = CheckImageConnectingToBifurcationMinutiae(Vc[n], Vb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvHorizontal);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					//SetBifurcationMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Vc[n], Vb, (nRecordNum + 1), emhvHorizontal);
					SetImageBifurcationMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Vc[n], Vb, (nRecordNum + 1), emhvHorizontal);
					pHorizontalTextureInfo->HorizontalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Horizontal] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Head = Vhead[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Tail = Vtail[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Length = Vlenght[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].WeightSum = WeightSum[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetHorizontalConnectingData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pHorizontal, Vc[n], Vhead[n], Vtail[n], Vlenght[n], NULL, NULL, Vw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateHorizontalLength(nItemCnt);
		//TRACE("HorizontalLength = %d / UpdateHorizontalLength = %d \r\n", GetHorizontalLength(), GetUpdateHorizontalLength());
		//TRACE("\r\n");

		int BifurcationY;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationY = pBifurcationInfo[i].xY;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Vhead[pBifurcationInfo[i].xL], Vtail[pBifurcationInfo[i].xL], Vlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationY, emhvHorizontal);//KennyKang
																																																																							/* 更新資料結構 */
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (j = 0; j < nHeight; j++) {
				if (bLengthAndQuality == false) {
					/* 取 X 座標 / 再次過濾 L 條件 */
					if (Vc[n][j] != 0 && (Vlenght[n] >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
				else {
					/* 取 X 座標 / 再次過濾 L + Q 條件 */
					if (Vc[n][j] != 0 && ((Vlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)

	if (Vc != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Vc[n]);
		}
		free(Vc);
	}

	if (Vb != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Vb[n]);
		}
		free(Vb);
	}

	if (Vw != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Vw[n]);
		}
		free(Vw);
	}
}

void emCheckVerticalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	char buffer[1000] = { 0 };
	unsigned char checkUw[IMAGE_WIDTH] = { 0 };					//Check Uw
	unsigned char checkDw[IMAGE_WIDTH] = { 0 };					//Check Dw
	unsigned char Hj[IMAGE_WIDTH] = { 0 };						//Y 的跳躍點

	//unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Hc[nRecordNum][nStartX] 存放 Y 座標
	//unsigned char Hb[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的分岔點
	//char Hw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };			//根據 Y 的上、下寬所計算出來的 Weight

	unsigned char Hhead[MaxRecordItemLength] = { 0 };			//線段頭 X 的座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };			//線段尾 X 的座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };			//線段 X 的總長度 
	short WeightSum[MaxRecordItemLength] = { 0 };				//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };			//線段頭 X 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };			//線段尾 X 的品質(後三點)

	int nBifurcationCnt = 0;									//分岔點的總數量

	unsigned char sC1y;
	char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int ii, i1, pi, ni, iLoop, nArrayIDx, nArrayCnt, nArrayX, nArrayY;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	unsigned char **Hc = NULL;
	unsigned char **Hb = NULL;
	char **Hw = NULL;
	Hc = (unsigned char **)malloc(sizeof(unsigned char *) * MaxRecordItemLength);
	Hb = (unsigned char **)malloc(sizeof(unsigned char *) * MaxRecordItemLength);
	Hw = (char **)malloc(sizeof(char *) * MaxRecordItemLength);
	for (n = 0; n < MaxRecordItemLength; n++) {
		Hc[n] = (unsigned char *)malloc(sizeof(unsigned char) * IMAGE_WIDTH);
		memset(Hc[n], 0, IMAGE_WIDTH);
		Hb[n] = (unsigned char *)malloc(sizeof(unsigned char) * IMAGE_WIDTH);
		memset(Hb[n], 0, IMAGE_WIDTH);
		Hw[n] = (char *)malloc(sizeof(char) * IMAGE_WIDTH);
		memset(Hw[n], 0, IMAGE_WIDTH);
	}

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先左再右的控制 */
	for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

		if (iLoop == 0 && iLoop % 2 == 0) {
			i1 = 0;
			i = nStartX + i1;
			ni = pi = i;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else if (iLoop > 0 && iLoop % 2 == 0) {
			i1 = 1;
			pi = pi + i1;
			i = pi;
			if (i > nWidth - 1)
				i = nWidth - 1;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else {
			i1 = -1;
			ni = ni + i1;
			i = ni;
			if (i < 0)
				i = 0;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}

		/* Y Top - > Down */
		for (j = 0; j < nHeight; j++) {

			memset(&Hc[n][0], 0, IMAGE_WIDTH);
			memset(&Hw[n][0], 0, IMAGE_WIDTH);
			memset(&Hb[n][0], 0, IMAGE_WIDTH);
			memset(checkUw, 0, IMAGE_WIDTH);
			memset(checkDw, 0, IMAGE_WIDTH);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				//checkHc[i] = j;	//存 Y 座標
				//TRACE("n = %3d / checkHc[%3d] = %3d / (%d, %d)\r\n", n, i, j, i, j);
				Hc[n][i] = j;	//存 Y 座標
				//TRACE("Hc[%d][%d] = %d / (%d, %d)\r\n", n, i, j, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation Y Up and Down Width*/
				GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, i, Hc[n][i], &checkUw[0], &checkDw[0], n);//KennyKang

				/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
				CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &Hw[n][0], i, Hc[n][i], n);//KennyKang
				nWeightSum = nWeightSum + Hw[n][i];//KennyKang

				/* 中 -> 右 尋找下一點 */
				nSx = i;		 //取 X 座標
				nSy = Hc[n][i];	 //取 Y 座標
				//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向右尋找 */
				for (ii = nSx; ii < nWidth - 1; ) {
					ii = ii + 1;	//將 X 座標右移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, ii, sC1y);
						if (nArrayIDx == 0) {
							nArrayX = ii;
							nArrayY = sC1y;
						}

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							Hc[n][ii] = sC1y;
							//checkHc[ii] = sC1y;
							//if (ii + 1 == nWidth)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_in[nSy][ii + 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, Hc[n][ii], &checkUw[0], &checkDw[0], n);//KennyKang

							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &Hw[n][0], ii, Hc[n][ii], n);//KennyKang
							nWeightSum = nWeightSum + Hw[n][ii];//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx) {
							Hb[n][ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {
						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Right ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Right ... Not Find / (%d, %d) / (%d, %d)\r\n", ii, nSy, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSy + nC1y[nArrayIDx] > nHeight - 1)
									nDy = nHeight - 1;
								else if (nSy + nC1y[nArrayIDx] < 1)
									nDy = 1;
								else
									nDy = nSy + nC1y[nArrayIDx];

								nDx = nArrayX;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[nDy][ii] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], nDy, ii, pLabelingData_in[nDy][ii]);

									bFind = true;
									nSy = nDy;
									Hc[n][ii] = nDy;
									//if (ii + 1 == nWidth)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx], ii + 1, nDy, pLabelingData_in[nDy][ii + 1]);

									//將該點原來的標記 V0/VL 改變為 Vx
									pLabelingData_in[nDy][ii] = Vx;

									/*Calculation Y Up and Down Width*/
									GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, Hc[n][ii], &checkUw[0], &checkDw[0], n);//KennyKang

																																										  /*Check Y Up and Down Width Quality and Calculation Weight Sum*/
									CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &Hw[n][0], ii, Hc[n][ii], n);//KennyKang
									nWeightSum = nWeightSum + Hw[n][ii];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
									//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii < nWidth - 1; )
				 //TRACE("\r\n");

				 /*中 -> 左  尋找下一點*/
				nSx = i;		 //取 X 座標
				nSy = Hc[n][i];	 //取 Y 座標
				//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向左尋找 */
				for (ii = nSx; ii > 0; ) {
					ii = ii - 1;	//將 X 座標左移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, ii, sC1y);
						if (nArrayIDx == 0) {
							nArrayX = ii;
							nArrayY = sC1y;
						}

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							Hc[n][ii] = sC1y;

							//if (ii - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_in[nSy][ii - 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, Hc[n][ii], &checkUw[0], &checkDw[0], n);//KennyKang

							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &Hw[n][0], ii, Hc[n][ii], n);//KennyKang			
							nWeightSum = nWeightSum + Hw[n][ii];//KennyKang
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx) {
							Hb[n][ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {
						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Left ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Left ... Not Find / (%d, %d) / (%d, %d)\r\n", ii, nSy, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSy + nC1y[nArrayIDx] > nHeight - 1)
									nDy = nHeight - 1;
								else if (nSy + nC1y[nArrayIDx] < 1)
									nDy = 1;
								else
									nDy = nSy + nC1y[nArrayIDx];

								nDx = nArrayX;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[nDy][ii] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], nDy, ii, pLabelingData_in[nDy][ii]);

									bFind = true;
									nSy = nDy;
									Hc[n][ii] = nDy;

									//if (ii - 1 < 0)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx], ii - 1, nDy, pLabelingData_in[nDy][ii - 1]);

									//將該點原來的標記 V0/VL 改變為 Vx
									pLabelingData_in[nDy][ii] = Vx;

									/*Calculation Y Up and Down Width*/
									GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, Hc[n][ii], &checkUw[0], &checkDw[0], n);//KennyKang

									/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
									CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &Hw[n][0], ii, Hc[n][ii], n);//KennyKang
									nWeightSum = nWeightSum + Hw[n][ii];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
									//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii > 0; )
				 //TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&Hc[n][0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(&Hc[n][0], nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {
					//CheckBifurcation(&checkHc[0], &checkHb[0], n, emhvVertical);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nWidth; nId++) {
						if (Hb[n][nId] != 0) {
							nBifurcationCnt++;
							//TRACE("Hb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Hb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Hhead[n] = nMin;
					Htail[n] = nMax;
					Hlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;
					//TRACE("Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (j = 0; j < nHeight; j++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		sprintf(buffer, "Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		LOGD(buffer);
	}
	else {
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		sprintf(buffer, "Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		LOGD(buffer);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Vertical Data to Struct */
		InitVerticalAlgoMemory(nRecordNum + 1);
		SetVerticalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Hlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Hlenght[n] + WeightSum[n];
			}

			/* 取 Y 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Hhead[n], Htail[n], Hlenght[n], Hw[n], n, QualityTh, emhvVertical, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength = 2;
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pEndingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength, Hc[n], nNewHead, nNewTail, emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Hj, 0, IMAGE_WIDTH);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Hc[n], nThJumpingLength, &Hj[0], emhvVertical);
				//TRACE("[nItemCnt = %d] pVerticalTextureInfo->VerticalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength);
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pJumpingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength, &Hj[0], emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				//nBifurcationLength = CheckConnectingToBifurcationMinutiae(Hc[n], Hb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvVertical);
				nBifurcationLength = CheckImageConnectingToBifurcationMinutiae(Hc[n], Hb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvVertical);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					//SetBifurcationMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Hc[n], Hb, (nRecordNum + 1), emhvVertical);
					SetImageBifurcationMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Hc[n], Hb, (nRecordNum + 1), emhvVertical);
					pVerticalTextureInfo->VerticalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Vertical] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pVerticalTextureInfo->VerticalItem[nItemCnt].Head = Hhead[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Tail = Htail[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Length = Hlenght[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].WeightSum = WeightSum[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetVerticalConnectingData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, Hc[n], Hhead[n], Htail[n], Hlenght[n], NULL, NULL, Hw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Hw[n], Hc[n], n, QualityTh, emhvVertical);//KennyKang
																											  //TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateVerticalLength(nItemCnt);
		//TRACE("VerticalLength = %d / UpdateVerticalLength = %d \r\n", GetVerticalLength(), GetUpdateVerticalLength());
		//TRACE("\r\n");

		int BifurcationX;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationX = pBifurcationInfo[i].xX;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Hhead[pBifurcationInfo[i].xL], Htail[pBifurcationInfo[i].xL], Hlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationX, emhvVertical);//KennyKang
																																																																						  /* 更新資料結構 */
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (i = 0; i < nWidth; i++) {
				if (bLengthAndQuality == false) {
					/* 取 Y 座標 / 再次過濾 L 條件 */
					if (Hc[n][i] != 0 && (Hlenght[n] >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
				else {
					/* 取 Y 座標 / 再次過濾 L + Q 條件 */
					if (Hc[n][i] != 0 && ((Hlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)

	if (Hc != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Hc[n]);
		}
		free(Hc);
	}

	if (Hb != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Hb[n]);
		}
		free(Hb);
	}

	if (Hw != NULL) {
		for (n = 0; n < MaxRecordItemLength; n++) {
			free(Hw[n]);
		}
		free(Hw);
	}
}
#else
void CheckHorizontalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkVc[IMAGE_HEIGHT] = { 0 };				//Check Vc
	unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[nRecordNum][nStartY] 存放 X 座標
	unsigned char checkLw[IMAGE_HEIGHT] = { 0 };				//Check Lw
	unsigned char Lw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的左寬
	unsigned char checkRw[IMAGE_HEIGHT] = { 0 };				//Check Rw
	unsigned char Rw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的右寬
	char checkVw[IMAGE_HEIGHT] = { 0 };							//Check Vw
	char Vw[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };			//根據 X 的左、右寬所計算出來的 Weight
	unsigned char checkVb[IMAGE_HEIGHT] = { 0 };				//Check Vb
	unsigned char Vb[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量
	unsigned char Vj[IMAGE_HEIGHT] = { 0 };						//X 的跳躍點

	unsigned char Vhead[MaxRecordItemLength] = { 0 };	//線段頭 Y 的座標
	unsigned char Vtail[MaxRecordItemLength] = { 0 };	//線段尾 Y 的座標
	unsigned char Vlenght[MaxRecordItemLength] = { 0 }; //線段 Y 的總長度
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 Y 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 Y 的品質(後三點)

	unsigned char sC1x;
	char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int jj, j1, pj, nj, jLoop, nArrayIDx, nArrayCnt, nArrayX, nArrayY;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先上再下的控制 */
	for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

		if (jLoop == 0 && jLoop % 2 == 0) {
			j1 = 0;
			j = nStartY + j1;
			nj = pj = j;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else if (jLoop > 0 && jLoop % 2 == 0) {
			j1 = 1;//下
			pj = pj + j1;
			j = pj;
			if (j > nHeight - 1)
				j = nHeight - 1;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}
		else {
			j1 = -1;//上
			nj = nj + j1;
			j = nj;
			if (j < 0)
				j = 0;
			//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
		}

		/* X Right -> Left */
		for (i = 0; i < nWidth; i++) {

			memset(checkVc, 0, IMAGE_HEIGHT);
			memset(checkLw, 0, IMAGE_HEIGHT);
			memset(checkRw, 0, IMAGE_HEIGHT);
			memset(checkVw, 0, IMAGE_HEIGHT);
			memset(checkVb, 0, IMAGE_HEIGHT);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkVc[j] = i;	//存 X 座標				
				//TRACE("n = %d / checkVc[%d] = %d / (%d, %d)\r\n", n, j, i, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation X Left and Right Width*/
				GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[j], j, &checkLw[0], &checkRw[0], n);//KennyKang
				/*Check X Left and Right Width Quality and Calculation Weight Sum*/
				CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[j], j, n);//KennyKang
				//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", j, checkVc[j], j, checkLw[j], j, checkRw[j], j, checkVw[j]);//KennyKang
				nWeightSum = nWeightSum + checkVw[j];//KennyKang

				/*中 -> 下 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向下尋找 */
				for (jj = nSy; jj < nHeight - 1; ) {
					jj = jj + 1;	//將 Y 座標下移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, sC1x, jj);
						if (nArrayIDx == 0) {
							nArrayX = sC1x;
							nArrayY = jj;
						}

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj + 1 == nHeight)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_in[jj + 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
							//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
							nWeightSum = nWeightSum + checkVw[jj];//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx/* || pLabelingData_in[jj][sC1x] == V1*/) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {

						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Down ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Down ... Not Find / (%d, %d) / (%d, %d)\r\n", nSx, jj, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (/*nSx*/nArrayX + nC1x[nArrayIDx] > nWidth - 1)
									nDx = nWidth - 1;
								else if (/*nSx*/nArrayX + nC1x[nArrayIDx] < 1)
									nDx = 1;
								else
									nDx = /*nSx*/nArrayX + nC1x[nArrayIDx];

								nDy = nArrayY;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[jj][nDx] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], jj, nDx, pLabelingData_in[jj][nDx]);

									bFind = true;
									nSx = nDx;
									checkVc[jj] = nDx;
									//if (jj + 1 == nHeight)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx], nDx, jj + 1, pLabelingData_in[jj + 1][nDx]);

									//將該點標記 V0/VL 改變為 Vx
									pLabelingData_in[jj][nDx] = Vx;

									/*Calculation X Left and Right Width*/
									GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
									/*Check X Left and Right Width Quality and Calculation Weight Sum*/
									CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
									//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
									nWeightSum = nWeightSum + checkVw[jj];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
										//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj < nHeight - 1; )
				 //TRACE("\r\n");

				 /*中 -> 上 尋找下一點*/
				nSx = checkVc[j];	//取 X 座標
				nSy = j;			//取 Y 座標
				//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向上尋找 */
				for (jj = nSy; jj > 0; ) {
					jj = jj - 1;	//將 Y 座標上移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
							if (nC1x[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
							if (nC1x[nArrayIDx] < 0)
								continue;
						}

						if (nSx + nC1x[nArrayIDx] > nWidth - 1)
							sC1x = nWidth - 1;
						else if (nSx + nC1x[nArrayIDx] < 1)
							sC1x = 1;
						else
							sC1x = nSx + nC1x[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d) / nArrayCnt = %d\r\n", nArrayIDx, sC1x, jj, nArrayCnt);
						if (nArrayIDx == 0) {
							nArrayX = sC1x;
							nArrayY = jj;
						}

						if (pLabelingData_in[jj][sC1x] == V0 || pLabelingData_in[jj][sC1x] == VL) {
							bFind = true;
							nSx = sC1x;
							//Vc[n][jj] = sC1x;
							checkVc[jj] = sC1x;
							//if (jj - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_in[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

							//將該點標記 V0/VL 改變為 Vx
							pLabelingData_in[jj][sC1x] = Vx;

							/*Calculation X Left and Right Width*/
							GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
							/*Check X Left and Right Width Quality and Calculation Weight Sum*/
							CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
							//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
							nWeightSum = nWeightSum + checkVw[jj];//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[jj][sC1x] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[jj][sC1x] == Vx/* || pLabelingData_in[jj][sC1x] == V1*/) {
							checkVb[jj] = sC1x;
							//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_in[jj][sC1x]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {

						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Up ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Up ... Not Find / (%d, %d) / (%d, %d)\r\n", nSx, jj, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSx + nC1x[nArrayIDx] > nWidth - 1)
									nDx = nWidth - 1;
								else if (nSx + nC1x[nArrayIDx] < 1)
									nDx = 1;
								else
									nDx = nSx + nC1x[nArrayIDx];

								nDy = nArrayY;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[jj][nDx] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], jj, nDx, pLabelingData_in[jj][nDx]);

									bFind = true;
									nSx = nDx;
									checkVc[jj] = nDx;
									//if (jj - 1 < 0)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, nDx, jj, pLabelingData_in[jj][nDx], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_in[jj - 1][nSx]);

									//將該點標記 V0/VL 改變為 Vx
									pLabelingData_in[jj][nDx] = Vx;

									/*Calculation X Left and Right Width*/
									GetHorizontalConnectingLeftRightWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, checkVc[jj], jj, &checkLw[0], &checkRw[0], n);//KennyKang
									/*Check X Left and Right Width Quality and Calculation Weight Sum*/
									CheckHorizontalConnectingWidthQuality(pDifferentialData_in, &checkLw[0], &checkRw[0], &checkVw[0], checkVc[jj], jj, n);//KennyKang
									//TRACE("checkVc[%d] = %d / checkLw[%d] = %d / checkRw[%d] = %d / checkVw[%d] = %d\r\n", jj, checkVc[jj], jj, checkLw[jj], jj, checkRw[jj], jj, checkVw[jj]);//KennyKang
									nWeightSum = nWeightSum + checkVw[jj];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if (nArrayIDx == MaxC1Len - 1)
										//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (jj = nSy; jj > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&checkVc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(&checkVc[0], nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {
					//CheckBifurcation(&checkVc[0], &checkVb[0], n, emhvHorizontal);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nHeight; nId++) {
						/*取 X 座標 */
						if (checkVc[nId] != 0) {
							Vc[n][nId] = checkVc[nId];
							Lw[n][nId] = checkLw[nId];
							Rw[n][nId] = checkRw[nId];
							Vw[n][nId] = checkVw[nId];
							//TRACE("checkVc[%d] = %d / Vc[%d][%d] = %d / Lw[%d][%d] = %d / Rw[%d][%d] = %d / Vw[%d][%d] = %d\r\n", nId, checkVc[nId], n, nId, Vc[n][nId], n, nId, Lw[n][nId], n, nId, Rw[n][nId], n, nId, Vw[n][nId]);//KennyKang
						}

						if (checkVb[nId] != 0) {
							Vb[n][nId] = checkVb[nId];
							nBifurcationCnt++;
							//TRACE("Vb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Vb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Vhead[n] = nMin;
					Vtail[n] = nMax;
					Vlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;//KennyKang
					//TRACE("Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//TRACE("\r\n");
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (i = 0; i < nWidth; i++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
	}
	else {
		//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Horizontal Data to Struct */
		InitHorizontalAlgoMemory(nRecordNum + 1);
		SetHorizontalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Vlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Vlenght[n] + WeightSum[n];
			}

			/* 取 X 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Vhead[n], Vtail[n], Vlenght[n], Vw[n], n, QualityTh, emhvHorizontal, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength = 2;
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pEndingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nEndingLength, Vc[n], nNewHead, nNewTail, emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Vj, 0, IMAGE_HEIGHT);
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Vc[n], nThJumpingLength, &Vj[0], emhvHorizontal);
				//TRACE("[nItemCnt = %d] pHorizontalTextureInfo->HorizontalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength);
				if (pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pJumpingMinutiae, pHorizontalTextureInfo->HorizontalItem[nItemCnt].nJumpingLength, &Vj[0], emhvHorizontal);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Vc[n], Vb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvHorizontal);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Vc[n], Vb, (nRecordNum + 1), emhvHorizontal);
					pHorizontalTextureInfo->HorizontalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Horizontal] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Head = Vhead[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Tail = Vtail[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].Length = Vlenght[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].WeightSum = WeightSum[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pHorizontalTextureInfo->HorizontalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Vhead[%d] = %d / Vtail[%d] = %d / Vlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Vhead[n], n, Vtail[n], n, Vlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetHorizontalConnectingData(&pHorizontalTextureInfo->HorizontalItem[nItemCnt].pHorizontal, Vc[n], Vhead[n], Vtail[n], Vlenght[n], Lw[n], Rw[n], Vw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Vw[n], Vc[n], n, QualityTh, emhvHorizontal);//KennyKang
				//TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateHorizontalLength(nItemCnt);
		//TRACE("HorizontalLength = %d / UpdateHorizontalLength = %d \r\n", GetHorizontalLength(), GetUpdateHorizontalLength());
		//TRACE("\r\n");

		int BifurcationY;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationY = pBifurcationInfo[i].xY;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Vhead[pBifurcationInfo[i].xL], Vtail[pBifurcationInfo[i].xL], Vlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationY, emhvHorizontal);//KennyKang
			/* 更新資料結構 */
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pHorizontalTextureInfo->HorizontalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Clean Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (j = 0; j < nHeight; j++) {
				if (bLengthAndQuality == false) {
					/* 取 X 座標 / 再次過濾 L 條件 */
					if (Vc[n][j] != 0 && (Vlenght[n] >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
				else {
					/* 取 X 座標 / 再次過濾 L + Q 條件 */
					if (Vc[n][j] != 0 && ((Vlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[j][Vc[n][j]] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}

void CheckVerticalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL)
		return;

	int i, j, n, nRecordNum;
	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

	int nStartX = (nWidth / 2) - 1;	//(160 / 2)-1 = 79
	int nStartY = (nHeight / 2) - 1;//(160 / 2)-1 = 79

	unsigned char checkHc[IMAGE_WIDTH] = { 0 };					//Check Hc
	unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Hc[nRecordNum][nStartX] 存放 Y 座標
	unsigned char checkUw[IMAGE_WIDTH] = { 0 };					//Check Uw
	unsigned char Uw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Y 的上寬
	unsigned char checkDw[IMAGE_WIDTH] = { 0 };					//Check Dw
	unsigned char Dw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Y 的下寬
	char checkHw[IMAGE_WIDTH] = { 0 };							//Check Hw
	char Hw[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };			//根據 Y 的上、下寬所計算出來的 Weight
	unsigned char checkHb[IMAGE_WIDTH] = { 0 };					//Check Hb
	unsigned char Hb[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的分岔點
	int nBifurcationCnt = 0;									//分岔點的總數量
	unsigned char Hj[IMAGE_WIDTH] = { 0 };						//Y 的跳躍點

	unsigned char Hhead[MaxRecordItemLength] = { 0 };	//線段頭 X 的座標
	unsigned char Htail[MaxRecordItemLength] = { 0 };	//線段尾 X 的座標
	unsigned char Hlenght[MaxRecordItemLength] = { 0 };	//線段 X 的總長度 
	short WeightSum[MaxRecordItemLength] = { 0 };		//該線段上所有點集合的Weight加總
	bool  bHeadQuality[MaxRecordItemLength] = { 0 };	//線段頭 X 的品質(前三點)
	bool  bTailQuality[MaxRecordItemLength] = { 0 };	//線段尾 X 的品質(後三點)

	unsigned char sC1y;
	char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

	bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
	int nRecordR0_IDx = -1;
	int ii, i1, pi, ni, iLoop, nArrayIDx, nArrayCnt, nArrayX, nArrayY;
	int nSx = 0, nSy = 0;
	int nWeightSum = 0;

	n = 0;
	nRecordNum = 0;

	/* 最外圈負責移動先左再右的控制 */
	for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

		if (iLoop == 0 && iLoop % 2 == 0) {
			i1 = 0;
			i = nStartX + i1;
			ni = pi = i;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else if (iLoop > 0 && iLoop % 2 == 0) {
			i1 = 1;
			pi = pi + i1;
			i = pi;
			if (i > nWidth - 1)
				i = nWidth - 1;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}
		else {
			i1 = -1;
			ni = ni + i1;
			i = ni;
			if (i < 0)
				i = 0;
			//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
		}

		/* Y Top - > Down */
		for (j = 0; j < nHeight; j++) {

			memset(checkHc, 0, IMAGE_WIDTH);
			memset(checkUw, 0, IMAGE_WIDTH);
			memset(checkDw, 0, IMAGE_WIDTH);
			memset(checkHw, 0, IMAGE_WIDTH);
			memset(checkHb, 0, IMAGE_WIDTH);
			nWeightSum = 0;

			if (pLabelingData_in[j][i] == V0) {

				/* 保存找到的點 */
				checkHc[i] = j;	//存 Y 座標
				//TRACE("n = %3d / checkHc[%3d] = %3d / (%d, %d)\r\n", n, i, j, i, j);

				//將該點標記 V0 改變為 Vx
				pLabelingData_in[j][i] = Vx;

				/*Calculation Y Up and Down Width*/
				GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, i, checkHc[i], &checkUw[0], &checkDw[0], n);//KennyKang
				/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
				CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], i, checkHc[i], n);//KennyKang
				//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", i, checkHc[i], i, checkUw[i], i, checkDw[i], i, checkHw[i]);//KennyKang
				nWeightSum = nWeightSum + checkHw[i];//KennyKang

				/* 中 -> 右 尋找下一點 */
				nSx = i;		 //取 X 座標
				nSy = checkHc[i];//取 Y 座標
				//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向右尋找 */
				for (ii = nSx; ii < nWidth - 1; ) {
					ii = ii + 1;	//將 X 座標右移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, ii, sC1y);
						if (nArrayIDx == 0) {
							nArrayX = ii;
							nArrayY = sC1y;
						}

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii + 1 == nWidth)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_in[nSy][ii + 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
							//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
							nWeightSum = nWeightSum + checkHw[ii];//KennyKang

							//找到符合條件，必須清除旗標，避免找到的是nArrayIDx的最後一個，導致誤判
							nArrayCnt = 0;
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx/* || pLabelingData_in[sC1y][ii] == V1*/) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {
						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Right ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Right ... Not Find / (%d, %d) / (%d, %d)\r\n", ii, nSy, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSy + nC1y[nArrayIDx] > nHeight - 1)
									nDy = nHeight - 1;
								else if (nSy + nC1y[nArrayIDx] < 1)
									nDy = 1;
								else
									nDy = nSy + nC1y[nArrayIDx];

								nDx = nArrayX;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[nDy][ii] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], nDy, ii, pLabelingData_in[nDy][ii]);

									bFind = true;
									nSy = nDy;
									checkHc[ii] = nDy;
									//if (ii + 1 == nWidth)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx], ii + 1, nDy, pLabelingData_in[nDy][ii + 1]);

									//將該點原來的標記 V0/VL 改變為 Vx
									pLabelingData_in[nDy][ii] = Vx;

									/*Calculation Y Up and Down Width*/
									GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
									/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
									CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
									//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
									nWeightSum = nWeightSum + checkHw[ii];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
										//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii < nWidth - 1; )
				//TRACE("\r\n");

				/*中 -> 左  尋找下一點*/
				nSx = i;		 //取 X 座標
				nSy = checkHc[i];//取 Y 座標
				//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

				/* 將找到的點繼續向左尋找 */
				for (ii = nSx; ii > 0; ) {
					ii = ii - 1;	//將 X 座標左移
					bFind = false;
					bFindR0 = false;
					nRecordR0_IDx = -1;
					nArrayCnt = 0;

					for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
						nArrayCnt++;

						if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
							if (nC1y[nArrayIDx] > 0)
								continue;
						}
						if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
							if (nC1y[nArrayIDx] < 0)
								continue;
						}

						if (nSy + nC1y[nArrayIDx] > nHeight - 1)
							sC1y = nHeight - 1;
						else if (nSy + nC1y[nArrayIDx] < 1)
							sC1y = 1;
						else
							sC1y = nSy + nC1y[nArrayIDx];

						//紀錄該輪最開始的起點
						//TRACE("nArrayIDx = %d / (%d, %d)\r\n", nArrayIDx, ii, sC1y);
						if (nArrayIDx == 0) {
							nArrayX = ii;
							nArrayY = sC1y;
						}

						if (pLabelingData_in[sC1y][ii] == V0 || pLabelingData_in[sC1y][ii] == VL) {
							bFind = true;
							nSy = sC1y;
							//Hc[n][ii] = sC1y;
							checkHc[ii] = sC1y;
							//if (ii - 1 < 0)
							//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx]);
							//else
							//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_in[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_in[nSy][ii - 1]);

							//將該點原來的標記 V0/VL 改變為 Vx
							pLabelingData_in[sC1y][ii] = Vx;

							/*Calculation Y Up and Down Width*/
							GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
							/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
							CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
							//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
							nWeightSum = nWeightSum + checkHw[ii];//KennyKang
							break;
						}
						else if (pLabelingData_in[sC1y][ii] == R0) {
							if (bFindR0 == false)
								bFindR0 = true;
							nRecordR0_IDx = nArrayIDx;
							continue;
						}
						else if (pLabelingData_in[sC1y][ii] == Vx/* || pLabelingData_in[sC1y][ii] == V1*/) {
							checkHb[ii] = sC1y;
							//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_in[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_in[sC1y][ii]);
							break;
						}
					}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

					 //全部找完還是沒找到， 若(1)bZeroDifferential = true ，則繼續找 Differential = 0 的條件，否則直接離開程式
					if (nArrayCnt == MaxC1Len) {
						if (bZeroDifferential == false) {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Left ... Not Find\r\n");
						}
						else {
							nArrayCnt = 0;
							//TRACE("[KennyKang] Middle-> Left ... Not Find / (%d, %d) / (%d, %d)\r\n", ii, nSy, nArrayX, nArrayY);

							int nDx, nDy;
							for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
								if (nSy + nC1y[nArrayIDx] > nHeight - 1)
									nDy = nHeight - 1;
								else if (nSy + nC1y[nArrayIDx] < 1)
									nDy = 1;
								else
									nDy = nSy + nC1y[nArrayIDx];

								nDx = nArrayX;
								//TRACE("[KennyKang] nDx = %d / nDy = %d / pDifferentialData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx]);

								if (pDifferentialData_in[nDy][nDx] == 0 && (pLabelingData_in[nDy][nDx] != Vx && pLabelingData_in[nDy][nDx] != R0)) {
									//將該點標記為DL，用來做繞道
									pLabelingData_in[nDy][ii] = DL;
									//TRACE("[KennyKang] (%d, %d) / pDifferentialData_in[%d][%d] = %d / pLabelingData_in[%d][%d] = %d\r\n", nDx, nDy, nDy, nDx, pDifferentialData_in[nDy][nDx], nDy, ii, pLabelingData_in[nDy][ii]);

									bFind = true;
									nSy = nDy;
									checkHc[ii] = nDy;
									//if (ii - 1 < 0)
									//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx]);
									//else
									//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, nDy, pLabelingData_in[nDy][ii], nC1y[nArrayIDx], ii - 1, nDy, pLabelingData_in[nDy][ii - 1]);

									//將該點原來的標記 V0/VL 改變為 Vx
									pLabelingData_in[nDy][ii] = Vx;

									/*Calculation Y Up and Down Width*/
									GetVerticalConnectingUpDownWidth(pDifferentialData_in, pLabelingData_in, nWidth, nHeight, ii, checkHc[ii], &checkUw[0], &checkDw[0], n);//KennyKang
									/*Check Y Up and Down Width Quality and Calculation Weight Sum*/
									CheckVerticalConnectingWidthQuality(pDifferentialData_in, &checkUw[0], &checkDw[0], &checkHw[0], ii, checkHc[ii], n);//KennyKang
									//TRACE("checkHc[%d] = %d / checkUw[%d] = %d / checkDw[%d] = %d / checkHw[%d] = %d\r\n", ii, checkHc[ii], ii, checkUw[ii], ii, checkDw[ii], ii, checkHw[ii]);//KennyKang
									nWeightSum = nWeightSum + checkHw[ii];//KennyKang
									break;
								}
								else {
									//TRACE("[KennyKang] No Find DifferentialData / [nArrayIDx = %d]\r\n", nArrayIDx);
									//if(nArrayIDx == MaxC1Len - 1)
										//bFind = false;//KennyKang
								}
							}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)
						}
					}//if (nArrayCnt == MaxC1Len)

					if (bFind == false)
						break;
				}//for (ii = nSx; ii > 0; )
				//TRACE("\r\n");

				if (n == MaxRecordItemLength) {
					//TRACE("bMaxRecord == true\r\n");
					bMaxRecord = true;
					break;
				}

				int nMax = 0, nMin = 0, nRes;
				if (bLengthAndQuality == false) {
					/* 以線段長度(L)做門檻 */
					nRes = CheckConnectLength(&checkHc[0], nWidth, nHeight, nConnectingTh, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));
				}
				else {
					/* 以線段長度+線段權重品質(L + Q)做門檻 */
					nRes = CheckConnectLengthAndQuality(&checkHc[0], nWidth, nHeight, nConnectingTh, nWeightSum, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d / nWeightSum = %d\r\n", n, nRes, (nMax - nMin + 1), nWeightSum);
				}

				/* 符合 L / (L + Q) 條件，開始收資料與紀錄該線段為有效線段 */
				if (nRes == 1) {
					//CheckBifurcation(&checkHc[0], &checkHb[0], n, emhvVertical);//KennyKang

					if (n == 0 && bFirstAvailable == false) {
						bFirstAvailable = true;
					}

					for (int nId = 0; nId < nWidth; nId++) {
						/*取 Y 座標*/
						if (checkHc[nId] != 0) {
							Hc[n][nId] = checkHc[nId];
							Uw[n][nId] = checkUw[nId];
							Dw[n][nId] = checkDw[nId];
							Hw[n][nId] = checkHw[nId];
							//TRACE("checkHc[%d] = %d / Hc[%d][%d] = %d / Uw[%d][%d] = %d / Dw[%d][%d] = %d / Hw[%d][%d] = %d\r\n", nId, checkHc[nId], n, nId, Hc[n][nId], n, nId, Uw[n][nId], n, nId, Dw[n][nId], n, nId, Hw[n][nId]);//KennyKang
						}

						if (checkHb[nId] != 0) {
							Hb[n][nId] = checkHb[nId];
							nBifurcationCnt++;
							//TRACE("Hb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Hb[n][nId], nBifurcationCnt);//KennyKang
						}
					}

					Hhead[n] = nMin;
					Htail[n] = nMax;
					Hlenght[n] = nMax - nMin + 1;
					WeightSum[n] = nWeightSum;
					//TRACE("Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d\r\n", n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n]);
					//TRACE("\r\n");

					nRecordNum = n;
					n++;
				}
				//n++;
			}//if (pLabelingData_in[j][i] == V0)
		}//for (j = 0; j < nHeight; j++)

		if (bMaxRecord == true) {
			//TRACE("bMaxRecord == true\r\n");
			break;
		}
	}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)

	if (bFirstAvailable == true) {
		bFirstAvailable = false;
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
	}
	else {
		//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
	}
	//TRACE("\r\n");

	if ((nRecordNum + 1) > 0) {
		/* Save Vertical Data to Struct */
		InitVerticalAlgoMemory(nRecordNum + 1);
		SetVerticalLength(nRecordNum + 1);

		int nItemCnt = 0;
		unsigned char nNewHead, nNewTail;
		int nThJumpingLength = JumpingTh;
		int nBifurcationLength;
		int nCurrentBifurcationArrayIDx = 0;/* 紀錄分岔點陣列目前在記憶體的位置 */
		struct BifurcationInfo *pBifurcationInfo = NULL;
		if (nBifurcationCnt > 0) {
			pBifurcationInfo = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationCnt);
			memset(pBifurcationInfo, 0, sizeof(struct BifurcationInfo) * nBifurcationCnt);
		}

		int nCheckCondition;
		for (n = 0; n < (nRecordNum + 1); n++) {

			if (bLengthAndQuality == false) {
				/* 以線段長度(L)做門檻 */
				nCheckCondition = Hlenght[n];
			}
			else {
				/* 以線段長度+線段權重品質(L + Q)做門檻 */
				nCheckCondition = Hlenght[n] + WeightSum[n];
			}

			/* 取 Y 座標 / 再次過濾 L or (L + Q) 條件 */
			if (nCheckCondition >= nConnectingTh) {

				/* Setting Endind Minutiae Data */
				CheckConnectingToEndingMinutiae(&bHeadQuality[n], &bTailQuality[n], Hhead[n], Htail[n], Hlenght[n], Hw[n], n, QualityTh, emhvVertical, &nNewHead, &nNewTail);
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d / nNewHead = %d / nNewTail = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n], nNewHead, nNewTail);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength = 2;
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength != 0) {
					SetEndingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pEndingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nEndingLength, Hc[n], nNewHead, nNewTail, emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Jumping Minutiae Data */
				memset(Hj, 0, IMAGE_WIDTH);
				pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength = CheckConnectingToJumpingMinutiae(Hc[n], nThJumpingLength, &Hj[0], emhvVertical);
				//TRACE("[nItemCnt = %d] pVerticalTextureInfo->VerticalItem[%d].nJumpingLength = %d\r\n", nItemCnt, nItemCnt, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength);
				if (pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength != 0) {
					SetJumpingMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pJumpingMinutiae, pVerticalTextureInfo->VerticalItem[nItemCnt].nJumpingLength, &Hj[0], emhvVertical);
				}
				//TRACE("\r\n");

				/* Setting Bifurcation Minutiae Data */
				nBifurcationLength = 0;
				nBifurcationLength = CheckConnectingToBifurcationMinutiae(Hc[n], Hb, (nRecordNum + 1), n, pBifurcationInfo, &nCurrentBifurcationArrayIDx, emhvVertical);
				if (nBifurcationLength > 0) {
					/* 儲存分岔點資料 */
					SetBifurcationInfo(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationInfo, pBifurcationInfo, nBifurcationLength, &nCurrentBifurcationArrayIDx);
					SetBifurcationMinutiaeData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pBifurcationMinutiae, nBifurcationLength, Hc[n], Hb, (nRecordNum + 1), emhvVertical);
					pVerticalTextureInfo->VerticalItem[nItemCnt].nBifurcationLength = nBifurcationLength;

					/* 更新記憶體指標位置 */
					nCurrentBifurcationArrayIDx = nCurrentBifurcationArrayIDx + nBifurcationLength;
				}
				//TRACE("[Vertical] nBifurcationLength = %d / nCurrentBifurcationArrayIDx = %d\r\n", nBifurcationLength, nCurrentBifurcationArrayIDx);

				/* 儲存資料 */
				pVerticalTextureInfo->VerticalItem[nItemCnt].Head = Hhead[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Tail = Htail[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].Length = Hlenght[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].WeightSum = WeightSum[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bHeadQuality = bHeadQuality[n];
				pVerticalTextureInfo->VerticalItem[nItemCnt].bTailQuality = bTailQuality[n];
				//TRACE("[nItemCnt = %d] Hhead[%d] = %d / Htail[%d] = %d / Hlenght[%d] = %d / WeightSum[%d] = %d / bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", nItemCnt, n, Hhead[n], n, Htail[n], n, Hlenght[n], n, WeightSum[n], n, bHeadQuality[n], n, bTailQuality[n]);

				SetVerticalConnectingData(&pVerticalTextureInfo->VerticalItem[nItemCnt].pVertical, Hc[n], Hhead[n], Htail[n], Hlenght[n], Uw[n], Dw[n], Hw[n], nItemCnt);
				nItemCnt++;
				//TRACE("\r\n");

				/*Check V0 Connecting Quality Status*/
				CheckV0ConnectingLinePointQuality(pLabelingData_in, Hw[n], Hc[n], n, QualityTh, emhvVertical);//KennyKang
				//TRACE("\r\n");
			}
		}

		/* Update Information */
		UpdateVerticalLength(nItemCnt);
		//TRACE("VerticalLength = %d / UpdateVerticalLength = %d \r\n", GetVerticalLength(), GetUpdateVerticalLength());
		//TRACE("\r\n");

		int BifurcationX;
		for (i = 0; i < nCurrentBifurcationArrayIDx; i++) {
			//TRACE("(%d, %d) / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", pBifurcationInfo[i].xX, pBifurcationInfo[i].xY, i, pBifurcationInfo[i].xL, i, pBifurcationInfo[i].BxL);
			BifurcationX = pBifurcationInfo[i].xX;
			UpdateConnectingQualityReferenceBifurcation(&bHeadQuality[pBifurcationInfo[i].xL], &bTailQuality[pBifurcationInfo[i].xL], Hhead[pBifurcationInfo[i].xL], Htail[pBifurcationInfo[i].xL], Hlenght[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, &BifurcationX, emhvVertical);//KennyKang
																																																																						  /* 更新資料結構 */
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bHeadQuality = bHeadQuality[pBifurcationInfo[i].xL];
			pVerticalTextureInfo->VerticalItem[pBifurcationInfo[i].xL].bTailQuality = bTailQuality[pBifurcationInfo[i].xL];
			//TRACE("[Update] bHeadQuality[%d] = %d / bTailQuality[%d] = %d\r\n", pBifurcationInfo[i].xL, bHeadQuality[pBifurcationInfo[i].xL], pBifurcationInfo[i].xL, bTailQuality[pBifurcationInfo[i].xL]);
		}

		if (pBifurcationInfo != NULL)
			free(pBifurcationInfo);

		//TRACE("\r\n");

		//Celan Labeling memory for Connecting Use
		for (j = 0; j < nHeight; j++)
			memset(pLabelingData_in[j], 255, nWidth);

		for (n = 0; n < (nRecordNum + 1); n++) {
			for (i = 0; i < nWidth; i++) {
				if (bLengthAndQuality == false) {
					/* 取 Y 座標 / 再次過濾 L 條件 */
					if (Hc[n][i] != 0 && (Hlenght[n] >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
				else {
					/* 取 Y 座標 / 再次過濾 L + Q 條件 */
					if (Hc[n][i] != 0 && ((Hlenght[n] + WeightSum[n]) >= nConnectingTh)) {
						pLabelingData_in[Hc[n][i]][i] = Vx * 2;
					}
				}
			}
		}
	}//if ((nRecordNum + 1) > 0)
}
#endif

int CheckConnectLength(unsigned char *pArray, int nWidth, int nHeight, int nLengthTh, int *pMaximum, int *pMinimum, HorizontalVerticalType nHorizontalVerticalType)
{
	int nMin = 0, nMax = 0, nCntNum = 0;
	bool bFirstTime = false;
	if (nHorizontalVerticalType == emhvHorizontal) {
		for (int jj = 0; jj < nHeight; jj++) {
			if (pArray[jj] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = jj;
					bFirstTime = true;
				}

				if (jj > nMax)
					nMax = jj;
				if (jj <= nMin)
					nMin = jj;
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		for (int ii = 0; ii < nWidth; ii++) {
			if (pArray[ii] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = ii;
					bFirstTime = true;
				}

				if (ii > nMax)
					nMax = ii;
				if (ii <= nMin)
					nMin = ii;
			}
		}
	}

	*pMaximum = nMax;
	*pMinimum = nMin;

	if (nCntNum >= nLengthTh)
		return 1;
	return 0;
}

int CheckConnectLengthAndQuality(unsigned char *pArray, int nWidth, int nHeight, int nLengthTh, int nWeightSum, int *pMaximum, int *pMinimum, HorizontalVerticalType nHorizontalVerticalType)
{
	int nMin = 0, nMax = 0, nCntNum = 0;
	bool bFirstTime = false;
	if (nHorizontalVerticalType == emhvHorizontal) {
		for (int jj = 0; jj < nHeight; jj++) {
			if (pArray[jj] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = jj;
					bFirstTime = true;
				}

				if (jj > nMax)
					nMax = jj;
				if (jj <= nMin)
					nMin = jj;
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		for (int ii = 0; ii < nWidth; ii++) {
			if (pArray[ii] != 0) {
				nCntNum++;
				if (bFirstTime == false) {
					nMin = ii;
					bFirstTime = true;
				}

				if (ii > nMax)
					nMax = ii;
				if (ii <= nMin)
					nMin = ii;
			}
		}
	}

	*pMaximum = nMax;
	*pMinimum = nMin;

	if (((nMax - nMin + 1) + nWeightSum) >= nLengthTh)
		return 1;
	return 0;
}

void GetHorizontalAndVerticalConnectingData(short **pDifferentialData_in, unsigned char **pLabelingData_in, unsigned char **pLabelingData_out, int nWidth, int nHeight, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL || pLabelingData_out == NULL)
		return;

	for (int j = 0; j < nHeight; j++)
		memcpy(pLabelingData_out[j], pLabelingData_in[j], nWidth);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int i, j, n, nRecordNum;
		unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

		int nStartX = (nWidth / 2) - 1;
		int nStartY = (nHeight / 2) - 1;

		unsigned char checkVc[IMAGE_HEIGHT] = { 0 };				//Check Vc
		unsigned char Vc[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//Vc[nRecordNum][nStartY] 存放 X 座標
		unsigned char checkVb[IMAGE_HEIGHT] = { 0 };				//Check Vb
		unsigned char Vb[MaxRecordItemLength][IMAGE_HEIGHT] = { 0 };//X 的分岔點
		int nBifurcationCnt = 0;									//分岔點的總數量

		unsigned char sC1x;
		char nC1x[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

		bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
		int nRecordR0_IDx = -1;
		int jj, j1, pj, nj, jLoop, nArrayIDx;
		int nSx = 0, nSy = 0;
		//TRACE("Get Horizontal Connecting Data\r\n");

		n = 0;
		nRecordNum = 0;

		/* 最外圈負責移動先上再下的控制 */
		for (jLoop = 0; jLoop < nHeight - 1; jLoop++) {

			if (jLoop == 0 && jLoop % 2 == 0) {
				j1 = 0;
				j = nStartY + j1;
				nj = pj = j;
				//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
			}
			else if (jLoop > 0 && jLoop % 2 == 0) {
				j1 = 1;//下
				pj = pj + j1;
				j = pj;
				if (j > nHeight - 1)
					j = nHeight - 1;
				//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
			}
			else {
				j1 = -1;//上
				nj = nj + j1;
				j = nj;
				if (j < 0)
					j = 0;
				//TRACE("jLoop = %d / j1 = %d / j = %d / nj = %d / pj  %d\r\n", jLoop, j1, j, nj, pj);
			}

			/* X Right -> Left */
			for (i = 0; i < nWidth; i++) {

				memset(checkVc, 0, IMAGE_HEIGHT);
				memset(checkVb, 0, IMAGE_HEIGHT);

				if (pLabelingData_out[j][i] == V0) {

					/* 保存找到的點 */
					checkVc[j] = i;	//存 X 座標				
					//TRACE("n = %d / checkVc[%d] = %d / (%d, %d)\r\n", n, j, i, i, j);

					//將該點標記 V0 改變為 Vx
					pLabelingData_out[j][i] = Vx;

					/*中 -> 下 尋找下一點*/
					nSx = checkVc[j];	//取 X 座標
					nSy = j;			//取 Y 座標
					//TRACE("Middle-> Down Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

					/* 將找到的點繼續向下尋找 */
					for (jj = nSy; jj < nHeight - 1; ) {
						jj = jj + 1;	//將 Y 座標下移
						bFind = false;
						bFindR0 = false;
						nRecordR0_IDx = -1;

						for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
							if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
								if (nC1x[nArrayIDx] > 0)
									continue;
							}
							if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
								if (nC1x[nArrayIDx] < 0)
									continue;
							}

							if (nSx + nC1x[nArrayIDx] > nWidth - 1)
								sC1x = nWidth - 1;
							else if (nSx + nC1x[nArrayIDx] < 1)
								sC1x = 1;
							else
								sC1x = nSx + nC1x[nArrayIDx];

							if (pLabelingData_out[jj][sC1x] == V0 || pLabelingData_out[jj][sC1x] == VL) {
								bFind = true;
								nSx = sC1x;
								checkVc[jj] = sC1x;
								//if (jj + 1 == nHeight)
								//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_out[jj][sC1x], nC1x[nArrayIDx]);
								//else
								//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_out[jj][sC1x], nC1x[nArrayIDx], nSx, jj + 1, pLabelingData_out[jj + 1][nSx]);

								//將該點標記 V0/VL 改變為 Vx
								pLabelingData_out[jj][sC1x] = Vx;
								break;
							}
							else if (pLabelingData_out[jj][sC1x] == R0) {
								if (bFindR0 == false)
									bFindR0 = true;
								nRecordR0_IDx = nArrayIDx;
								continue;
							}
							else if (pLabelingData_out[jj][sC1x] == Vx) {
								checkVb[jj] = sC1x;
								//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_out[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_out[jj][sC1x]);
							}
							else {
								//全部找完還是沒找到
								if (nArrayIDx == MaxC1Len - 1) {
									//TRACE("Not Find\r\n");
									bFind = false;
								}
							}
						}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

						if (bFind == false)
							break;
					}//for (jj = nSy; jj < nHeight - 1; )
					 //TRACE("\r\n");

					 /*中 -> 上 尋找下一點*/
					nSx = checkVc[j];	//取 X 座標
					nSy = j;			//取 Y 座標
					//TRACE("Middle-> Up Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

					/* 將找到的點繼續向上尋找 */
					for (jj = nSy; jj > 0; ) {
						jj = jj - 1;	//將 Y 座標上移
						bFind = false;
						bFindR0 = false;
						nRecordR0_IDx = -1;

						for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
							if (bFindR0 == true && nC1x[nRecordR0_IDx] > 0) {
								if (nC1x[nArrayIDx] > 0)
									continue;
							}
							if (bFindR0 == true && nC1x[nRecordR0_IDx] < 0) {
								if (nC1x[nArrayIDx] < 0)
									continue;
							}

							if (nSx + nC1x[nArrayIDx] > nWidth - 1)
								sC1x = nWidth - 1;
							else if (nSx + nC1x[nArrayIDx] < 1)
								sC1x = 1;
							else
								sC1x = nSx + nC1x[nArrayIDx];

							if (pLabelingData_out[jj][sC1x] == V0 || pLabelingData_out[jj][sC1x] == VL) {
								bFind = true;
								nSx = sC1x;
								checkVc[jj] = sC1x;
								//if (jj - 1 < 0)
								//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, sC1x, jj, pLabelingData_out[jj][sC1x], nC1x[nArrayIDx]);
								//else
								//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, sC1x, jj, pLabelingData_out[jj][sC1x], nC1x[nArrayIDx], nSx, jj - 1, pLabelingData_out[jj - 1][nSx]);

								//將該點標記 V0/VL 改變為 Vx
								pLabelingData_out[jj][sC1x] = Vx;
								break;
							}
							else if (pLabelingData_out[jj][sC1x] == R0) {
								if (bFindR0 == false)
									bFindR0 = true;
								nRecordR0_IDx = nArrayIDx;
								continue;
							}
							else if (pLabelingData_out[jj][sC1x] == Vx) {
								checkVb[jj] = sC1x;
								//TRACE("[Horizontal] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_out[%d][%d] = %d\r\n", n, nRecordNum, sC1x, jj, jj, sC1x, pLabelingData_out[jj][sC1x]);
							}
							else {
								//全部找完還是沒找到
								if (nArrayIDx == MaxC1Len - 1) {
									//TRACE("Not Find\r\n");
									bFind = false;
								}
							}
						}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

						if (bFind == false)
							break;
					}//for (jj = nSy; jj > 0; )
					//TRACE("\r\n");

					if (n == MaxRecordItemLength) {
						//TRACE("bMaxRecord == true\r\n");
						bMaxRecord = true;
						break;
					}

					int nMax = 0, nMin = 0, nRes, nLengthTh;
					/* 以線段長度(L = 0)做門檻 */
					nLengthTh = 0;
					nRes = CheckConnectLength(&checkVc[0], nWidth, nHeight, nLengthTh, &nMax, &nMin, emhvHorizontal);
					//TRACE("[Horizontal] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));

					/* 符合 L = 0 條件，開始收資料與紀錄該線段為有效線段 */
					if (nRes == 1) {

						if (n == 0 && bFirstAvailable == false) {
							bFirstAvailable = true;
						}

						for (int nId = 0; nId < nHeight; nId++) {
							/*取 X 座標 */
							if (checkVc[nId] != 0) {
								Vc[n][nId] = checkVc[nId];
								//TRACE("checkVc[%d] = %d / Vc[%d][%d] = %d\r\n", nId, checkVc[nId], n, nId, Vc[n][nId]);//KennyKang
							}

							if (checkVb[nId] != 0) {
								Vb[n][nId] = checkVb[nId];
								nBifurcationCnt++;
								//TRACE("Vb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Vb[n][nId], nBifurcationCnt);//KennyKang
							}
						}

						nRecordNum = n;
						n++;
					}
				}//if (pLabelingData_out[j][i] == V0)
			}//for (i = 0; i < nWidth; i++)

			if (bMaxRecord == true) {
				//TRACE("bMaxRecord == true\r\n");
				break;
			}
		}//for (jLoop = 0; jLoop < nHeight - 1; jLoop++)

		if (bFirstAvailable == true) {
			bFirstAvailable = false;
			//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		}
		else {
			//TRACE("Horizontal Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		}
		//TRACE("\r\n");

		if ((nRecordNum + 1) > 0) {
			//Celan Labeling memory for Connecting Use
			for (j = 0; j < nHeight; j++)
				memset(pLabelingData_out[j], 255, nWidth);

			for (n = 0; n < (nRecordNum + 1); n++) {
				for (j = 0; j < nHeight; j++) {
					/* 取 X 座標 / 再次過濾 L = 0 條件 */
					if (Vc[n][j] != 0) {
						pLabelingData_out[j][Vc[n][j]] = Vx * 2;
					}
				}
			}
		}//if ((nRecordNum + 1) > 0)
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int i, j, n, nRecordNum;
		unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75, V1 = 210;

		int nStartX = (nWidth / 2) - 1;
		int nStartY = (nHeight / 2) - 1;

		unsigned char checkHc[IMAGE_WIDTH] = { 0 };					//Check Hc
		unsigned char Hc[MaxRecordItemLength][IMAGE_WIDTH] = { 0 };	//Hc[nRecordNum][nStartX] 存放 Y 座標
		unsigned char checkHb[IMAGE_WIDTH] = { 0 };					//Check Hb
		unsigned char Hb[MaxRecordItemLength][IMAGE_WIDTH] = { 0 }; //Y 的分岔點
		int nBifurcationCnt = 0;									//分岔點的總數量

		unsigned char sC1y;
		char nC1y[MaxC1Len] = { 0, 1, -1, 2, -2, 3, -3, 4, -4, 5, -5 };

		bool bFind, bFindR0, bMaxRecord = false, bFirstAvailable = false;
		int nRecordR0_IDx = -1;
		int ii, i1, pi, ni, iLoop, nArrayIDx;
		int nSx = 0, nSy = 0;
		//TRACE("Get Vertical Connecting Data\r\n");

		n = 0;
		nRecordNum = 0;

		/* 最外圈負責移動先左再右的控制 */
		for (iLoop = 0; iLoop < nWidth - 1; iLoop++) {

			if (iLoop == 0 && iLoop % 2 == 0) {
				i1 = 0;
				i = nStartX + i1;
				ni = pi = i;
				//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
			}
			else if (iLoop > 0 && iLoop % 2 == 0) {
				i1 = 1;
				pi = pi + i1;
				i = pi;
				if (i > nWidth - 1)
					i = nWidth - 1;
				//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
			}
			else {
				i1 = -1;
				ni = ni + i1;
				i = ni;
				if (i < 0)
					i = 0;
				//TRACE("iLoop = %d / i1 = %d / i = %d / ni = %d / pi  %d\r\n", iLoop, i1, i, ni, pi);
			}

			/* Y Top - > Down */
			for (j = 0; j < nHeight; j++) {

				memset(checkHc, 0, IMAGE_WIDTH);
				memset(checkHb, 0, IMAGE_WIDTH);

				if (pLabelingData_out[j][i] == V0) {

					/* 保存找到的點 */
					checkHc[i] = j;	//存 Y 座標
					//TRACE("n = %3d / checkHc[%3d] = %3d / (%d, %d)\r\n", n, i, j, i, j);

					//將該點標記 V0 改變為 Vx
					pLabelingData_out[j][i] = Vx;

					/* 中 -> 右 尋找下一點 */
					nSx = i;		 //取 X 座標
					nSy = checkHc[i];//取 Y 座標
					//TRACE("Middle-> Right Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

					/* 將找到的點繼續向右尋找 */
					for (ii = nSx; ii < nWidth - 1; ) {
						ii = ii + 1;	//將 X 座標右移
						bFind = false;
						bFindR0 = false;
						nRecordR0_IDx = -1;

						for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
							if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
								if (nC1y[nArrayIDx] > 0)
									continue;
							}
							if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
								if (nC1y[nArrayIDx] < 0)
									continue;
							}

							if (nSy + nC1y[nArrayIDx] > nHeight - 1)
								sC1y = nHeight - 1;
							else if (nSy + nC1y[nArrayIDx] < 1)
								sC1y = 1;
							else
								sC1y = nSy + nC1y[nArrayIDx];

							if (pLabelingData_out[sC1y][ii] == V0 || pLabelingData_out[sC1y][ii] == VL) {
								bFind = true;
								nSy = sC1y;
								checkHc[ii] = sC1y;
								//if (ii + 1 == nWidth)
								//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_out[sC1y][ii], nC1y[nArrayIDx]);
								//else
								//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_out[sC1y][ii], nC1y[nArrayIDx], ii + 1, nSy, pLabelingData_out[nSy][ii + 1]);

								//將該點原來的標記 V0/VL 改變為 Vx
								pLabelingData_out[sC1y][ii] = Vx;
								break;
							}
							else if (pLabelingData_out[sC1y][ii] == R0) {
								if (bFindR0 == false)
									bFindR0 = true;
								nRecordR0_IDx = nArrayIDx;
								continue;
							}
							else if (pLabelingData_out[sC1y][ii] == Vx) {
								checkHb[ii] = sC1y;
								//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_out[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_out[sC1y][ii]);
							}
							else {
								//全部找完還是沒找到
								if (nArrayIDx == MaxC1Len - 1) {
									//TRACE("Not Find\r\n");
									bFind = false;
								}
							}
						}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

						if (bFind == false)
							break;
					}//for (ii = nSx; ii < nWidth - 1; )
					//TRACE("\r\n");

					/*中 -> 左  尋找下一點*/
					nSx = i;		 //取 X 座標
					nSy = checkHc[i];//取 Y 座標
					//TRACE("Middle-> Left Start ... (n = %d / nSx = %d, nSy = %d)\r\n", n, nSx, nSy);

					/* 將找到的點繼續向左尋找 */
					for (ii = nSx; ii > 0; ) {
						ii = ii - 1;	//將 X 座標左移
						bFind = false;
						bFindR0 = false;
						nRecordR0_IDx = -1;

						for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++) {
							if (bFindR0 == true && nC1y[nRecordR0_IDx] > 0) {
								if (nC1y[nArrayIDx] > 0)
									continue;
							}
							if (bFindR0 == true && nC1y[nRecordR0_IDx] < 0) {
								if (nC1y[nArrayIDx] < 0)
									continue;
							}

							if (nSy + nC1y[nArrayIDx] > nHeight - 1)
								sC1y = nHeight - 1;
							else if (nSy + nC1y[nArrayIDx] < 1)
								sC1y = 1;
							else
								sC1y = nSy + nC1y[nArrayIDx];

							if (pLabelingData_out[sC1y][ii] == V0 || pLabelingData_out[sC1y][ii] == VL) {
								bFind = true;
								nSy = sC1y;
								checkHc[ii] = sC1y;
								//if (ii - 1 < 0)
								//	TRACE("n = %d / (%d, %d) / %d / %d\r\n", n, ii, sC1y, pLabelingData_out[sC1y][ii], nC1y[nArrayIDx]);
								//else
								//	TRACE("n = %d / (%d, %d) / %d / %d / Next -> (%d, %d) / %d\r\n", n, ii, sC1y, pLabelingData_out[sC1y][ii], nC1y[nArrayIDx], ii - 1, nSy, pLabelingData_out[nSy][ii - 1]);

								//將該點原來的標記 V0/VL 改變為 Vx
								pLabelingData_out[sC1y][ii] = Vx;
								break;
							}
							else if (pLabelingData_out[sC1y][ii] == R0) {
								if (bFindR0 == false)
									bFindR0 = true;
								nRecordR0_IDx = nArrayIDx;
								continue;
							}
							else if (pLabelingData_out[sC1y][ii] == Vx) {
								checkHb[ii] = sC1y;
								//TRACE("[Vertical] n = %d / nRecordNum = %d / (%d, %d) / pLabelingData_out[%d][%d] = %d\r\n", n, nRecordNum, ii, sC1y, sC1y, ii, pLabelingData_out[sC1y][ii]);
							}
							else {
								//全部找完還是沒找到
								if (nArrayIDx == MaxC1Len - 1) {
									//TRACE("Not Find\r\n");
									bFind = false;
								}
							}
						}//for (nArrayIDx = 0; nArrayIDx < MaxC1Len; nArrayIDx++)

						if (bFind == false)
							break;
					}//for (ii = nSx; ii > 0; )
					//TRACE("\r\n");

					if (n == MaxRecordItemLength) {
						//TRACE("bMaxRecord == true\r\n");
						bMaxRecord = true;
						break;
					}

					int nMax = 0, nMin = 0, nRes, nLengthTh;
					/* 以線段長度(L = 0)做門檻 */
					nLengthTh = 0;
					nRes = CheckConnectLength(&checkHc[0], nWidth, nHeight, nLengthTh, &nMax, &nMin, emhvVertical);
					//TRACE("[Vertical] Cehck Length ... n = %d / nRes = %d / Length = %d\r\n", n, nRes, (nMax - nMin + 1));

					/* 符合 L = 0 條件，開始收資料與紀錄該線段為有效線段 */
					if (nRes == 1) {
						
						if (n == 0 && bFirstAvailable == false) {
							bFirstAvailable = true;
						}

						for (int nId = 0; nId < nWidth; nId++) {
							/*取 Y 座標*/
							if (checkHc[nId] != 0) {
								Hc[n][nId] = checkHc[nId];
								//TRACE("checkHc[%d] = %d / Hc[%d][%d] = %d\r\n", nId, checkHc[nId], n, nId, Hc[n][nId]);//KennyKang
							}

							if (checkHb[nId] != 0) {
								Hb[n][nId] = checkHb[nId];
								nBifurcationCnt++;
								//TRACE("Hb[%d][%d] = %d / nBifurcationCnt = %d\r\n", n, nId, Hb[n][nId], nBifurcationCnt);//KennyKang
							}
						}

						nRecordNum = n;
						n++;
					}
				}//if (pLabelingData_out[j][i] == V0)
			}//for (j = 0; j < nHeight; j++)

			if (bMaxRecord == true) {
				//TRACE("bMaxRecord == true\r\n");
				break;
			}
		}//for (iLoop = 0; iLoop < nWidth - 1; iLoop++)

		if (bFirstAvailable == true) {
			bFirstAvailable = false;
			//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum + 1, nBifurcationCnt);
		}
		else {
			//TRACE("Vertical Total Record Line Number = %d / nBifurcationCnt = %d\r\n", nRecordNum, nBifurcationCnt);
		}
		//TRACE("\r\n");

		if ((nRecordNum + 1) > 0) {
			//Celan Labeling memory for Connecting Use
			for (j = 0; j < nHeight; j++)
				memset(pLabelingData_out[j], 255, nWidth);

			for (n = 0; n < (nRecordNum + 1); n++) {
				for (i = 0; i < nWidth; i++) {
					/* 取 Y 座標 / 再次過濾 L = 0 條件 */
					if (Hc[n][i] != 0) {
						pLabelingData_out[Hc[n][i]][i] = Vx * 2;
					}
				}
			}
		}//if ((nRecordNum + 1) > 0)
	}
}

void InitVerticalAlgoMemory(int nLength)
{
	if (pVerticalTextureInfo == NULL) {
		pVerticalTextureInfo = (struct VerticalTextureInfo *) malloc (sizeof(struct VerticalTextureInfo));
		pVerticalTextureInfo->nLength = nLength;
		pVerticalTextureInfo->VerticalItem = (struct VerticalData *) malloc (sizeof(struct VerticalData) * nLength);
		memset(pVerticalTextureInfo->VerticalItem, 0, sizeof(struct VerticalData) * nLength);	
	}
	else {
		FreeVerticalAlgoMemory();
		pVerticalTextureInfo = (struct VerticalTextureInfo *) malloc (sizeof(struct VerticalTextureInfo));
		pVerticalTextureInfo->nLength = nLength;
		pVerticalTextureInfo->VerticalItem = (struct VerticalData *) malloc(sizeof(struct VerticalData) * nLength);
		memset(pVerticalTextureInfo->VerticalItem, 0, sizeof(struct VerticalData) * nLength);
	}
}

void FreeVerticalAlgoMemory(void)
{
	if (pVerticalTextureInfo != NULL)  {
		for (int n = 0; n < pVerticalTextureInfo->nLength; n++) {
			struct MinutiaePOINT *pEndingMinutiae = pVerticalTextureInfo->VerticalItem[n].pEndingMinutiae;
			if (pEndingMinutiae != NULL)
				free(pEndingMinutiae);

			struct MinutiaePOINT *pJumpingMinutiae = pVerticalTextureInfo->VerticalItem[n].pJumpingMinutiae;
			if (pJumpingMinutiae != NULL)
				free(pJumpingMinutiae);

			struct MinutiaePOINT *pBifurcationMinutiae = pVerticalTextureInfo->VerticalItem[n].pBifurcationMinutiae;
			if (pBifurcationMinutiae != NULL)
				free(pBifurcationMinutiae);

			struct BifurcationInfo *pBifurcationInfo = pVerticalTextureInfo->VerticalItem[n].pBifurcationInfo;
			if (pBifurcationInfo != NULL)
				free(pBifurcationInfo);

			struct MinutiaePOINT *pCrossingMinutiae = pVerticalTextureInfo->VerticalItem[n].pCrossingMinutiae;
			if (pCrossingMinutiae != NULL)
				free(pCrossingMinutiae);

			struct txPOINT *pPOINT = pVerticalTextureInfo->VerticalItem[n].pVertical;
			if (pPOINT != NULL)
				free(pPOINT);
		}
		free(pVerticalTextureInfo->VerticalItem);
		free(pVerticalTextureInfo);
		pVerticalTextureInfo = NULL;
	}
}

void SetVerticalConnectingData(struct txPOINT **pPoint, unsigned char *Hc, unsigned char Hhead, unsigned char Htail, unsigned char Hlenght, unsigned char *Uw, unsigned char *Dw, char *Hw, int nIDx)
{
#ifdef EMC_Embedded
	if (pPoint == NULL || Hc == NULL || Hw == NULL)
		return;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] Hhead = %d / Htail = %d / Hlenght = %d\r\n", __FUNCTION__, __LINE__, nIDx, Hhead, Htail, Hlenght);
	//LOGD(buffer);

	struct txPOINT *pPointData = NULL;
	pPointData = (struct txPOINT *) malloc(sizeof(struct txPOINT) * Hlenght);
	memset(pPointData, 0, sizeof(struct txPOINT) * Hlenght);

	int nCnt = 0;
	int nWidth = IMAGE_WIDTH;
	for (int i = 0; i < nWidth; i++) {
		/*取 Y 座標*/
		if (Hc[i] != 0) {
			pPointData[nCnt].x = i;			//存 X 座標
			pPointData[nCnt].y = Hc[i];		//存 Y 座標
			pPointData[nCnt].Weight = Hw[i];//存 Y 的Weight
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nCnt = %d] / (%d, %d) / (%d, %d) / (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, i, Hc[i], pPointData[nCnt].x, pPointData[nCnt].y, pPointData[nCnt].Uw, pPointData[nCnt].Dw, pPointData[nCnt].Weight);
			//LOGD(buffer);
			nCnt++;
		}
	}
	*pPoint = pPointData;
#else
	if (pPoint == NULL || Hc == NULL || Uw == NULL || Dw == NULL || Hw == NULL)
		return;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] Hhead = %d / Htail = %d / Hlenght = %d\r\n", __FUNCTION__, __LINE__, nIDx, Hhead, Htail, Hlenght);
	//LOGD(buffer);

	struct txPOINT *pPointData = NULL;
	pPointData = (struct txPOINT *) malloc(sizeof(struct txPOINT) * Hlenght);
	memset(pPointData, 0, sizeof(struct txPOINT) * Hlenght);

	int nCnt = 0;
	int nWidth = IMAGE_WIDTH;
	for (int i = 0; i < nWidth; i++) {
		/*取 Y 座標*/
		if (Hc[i] != 0) {
			pPointData[nCnt].x = i;			//存 X 座標
			pPointData[nCnt].y = Hc[i];		//存 Y 座標
			pPointData[nCnt].Uw = Uw[i];	//存 Y 的上寬
			pPointData[nCnt].Dw = Dw[i];	//存 Y 的下寬
			pPointData[nCnt].Weight = Hw[i];//存 Y 的Weight
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nCnt = %d] / (%d, %d) / (%d, %d) / (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, i, Hc[i], pPointData[nCnt].x, pPointData[nCnt].y, pPointData[nCnt].Uw, pPointData[nCnt].Dw, pPointData[nCnt].Weight);
			//LOGD(buffer);
			nCnt++;
		}
	}
	*pPoint = pPointData;
#endif
}

struct VerticalTextureInfo *GetVerticalConnectingData(void)
{
	if (pVerticalTextureInfo == NULL)
		return NULL;
	return pVerticalTextureInfo;
}

void SetVerticalLength(int Length)
{
	VerticalLength = Length;
}

int GetVerticalLength(void)
{
	return VerticalLength;
}

void UpdateVerticalLength(int nNewLength)
{
	nUpdateVerticalLength = nNewLength;
}

int GetUpdateVerticalLength(void)
{
	return nUpdateVerticalLength;
}

void InitHorizontalAlgoMemory(int nLength)
{
	if (pHorizontalTextureInfo == NULL) {
		pHorizontalTextureInfo = (struct HorizontalTextureInfo *) malloc(sizeof(struct HorizontalTextureInfo));
		pHorizontalTextureInfo->nLength = nLength;
		pHorizontalTextureInfo->HorizontalItem = (struct HorizontalData *) malloc(sizeof(struct HorizontalData) * nLength);
		memset(pHorizontalTextureInfo->HorizontalItem, 0, sizeof(struct HorizontalData) * nLength);
	}
	else {
		FreeHorizontalAlgoMemory();
		pHorizontalTextureInfo = (struct HorizontalTextureInfo *) malloc(sizeof(struct HorizontalTextureInfo));
		pHorizontalTextureInfo->nLength = nLength;
		pHorizontalTextureInfo->HorizontalItem = (struct HorizontalData *) malloc(sizeof(struct HorizontalData) * nLength);
		memset(pHorizontalTextureInfo->HorizontalItem, 0, sizeof(struct HorizontalData) * nLength);
	}
}

void FreeHorizontalAlgoMemory(void)
{
	if (pHorizontalTextureInfo != NULL) {
		for (int n = 0; n < pHorizontalTextureInfo->nLength; n++) {
			struct MinutiaePOINT *pEndingMinutiae = pHorizontalTextureInfo->HorizontalItem[n].pEndingMinutiae;
			if (pEndingMinutiae != NULL)
				free(pEndingMinutiae);

			struct MinutiaePOINT *pJumpingMinutiae = pHorizontalTextureInfo->HorizontalItem[n].pJumpingMinutiae;
			if (pJumpingMinutiae != NULL)
				free(pJumpingMinutiae);

			struct MinutiaePOINT *pBifurcationMinutiae = pHorizontalTextureInfo->HorizontalItem[n].pBifurcationMinutiae;
			if (pBifurcationMinutiae != NULL)
				free(pBifurcationMinutiae);

			struct BifurcationInfo *pBifurcationInfo = pHorizontalTextureInfo->HorizontalItem[n].pBifurcationInfo;
			if (pBifurcationInfo != NULL)
				free(pBifurcationInfo);

			struct MinutiaePOINT *pCrossingMinutiae = pHorizontalTextureInfo->HorizontalItem[n].pCrossingMinutiae;
			if (pCrossingMinutiae != NULL)
				free(pCrossingMinutiae);

			struct txPOINT *pPOINT = pHorizontalTextureInfo->HorizontalItem[n].pHorizontal;
			if (pPOINT != NULL)
				free(pPOINT);
		}
		free(pHorizontalTextureInfo->HorizontalItem);
		free(pHorizontalTextureInfo);
		pHorizontalTextureInfo = NULL;
	}
}

void SetHorizontalConnectingData(struct txPOINT **pPoint, unsigned char *Vc, unsigned char Vhead, unsigned char Vtail, unsigned char Vlenght, unsigned char *Lw, unsigned char *Rw, char *Vw, int nIDx)
{
#ifdef EMC_Embedded
	if (pPoint == NULL || Vc == NULL || Vw == NULL)
		return;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] Vhead = %d / Vtail = %d / Vlenght = %d\r\n", __FUNCTION__, __LINE__, nIDx, Vhead, Vtail, Vlenght);
	//LOGD(buffer);

	struct txPOINT *pPointData = NULL;
	pPointData = (struct txPOINT *) malloc(sizeof(struct txPOINT) * Vlenght);
	memset(pPointData, 0, sizeof(struct txPOINT) * Vlenght);

	int nCnt = 0;
	int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
	for (int j = 0; j < nHeight; j++) {
		/*取 X 座標*/
		if (Vc[j] != 0) {
			pPointData[nCnt].x = Vc[j];		//存 X 座標
			pPointData[nCnt].y = j;			//存 Y 座標
			pPointData[nCnt].Weight = Vw[j];//存 X 的Weight
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nCnt = %d] / (%d, %d) / (%d, %d) / (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, Vc[j], j, pPointData[nCnt].x, pPointData[nCnt].y, pPointData[nCnt].Lw, pPointData[nCnt].Rw, pPointData[nCnt].Weight);
			//LOGD(buffer);
			nCnt++;
		}
	}
	*pPoint = pPointData;
#else
	if (pPoint == NULL || Vc == NULL || Lw == NULL || Rw == NULL || Vw == NULL)
		return;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] Vhead = %d / Vtail = %d / Vlenght = %d\r\n", __FUNCTION__, __LINE__, nIDx, Vhead, Vtail, Vlenght);
	//LOGD(buffer);

	struct txPOINT *pPointData = NULL;
	pPointData = (struct txPOINT *) malloc(sizeof(struct txPOINT) * Vlenght);
	memset(pPointData, 0, sizeof(struct txPOINT) * Vlenght);

	int nCnt = 0;
	int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
	for (int j = 0; j < nHeight; j++) {
		/*取 X 座標*/
		if (Vc[j] != 0) {
			pPointData[nCnt].x = Vc[j];		//存 X 座標
			pPointData[nCnt].y = j;			//存 Y 座標
			pPointData[nCnt].Lw = Lw[j];	//存 X 的左寬
			pPointData[nCnt].Rw = Rw[j];	//存 X 的右寬
			pPointData[nCnt].Weight = Vw[j];//存 X 的Weight
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nCnt = %d] / (%d, %d) / (%d, %d) / (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, Vc[j], j, pPointData[nCnt].x, pPointData[nCnt].y, pPointData[nCnt].Lw, pPointData[nCnt].Rw, pPointData[nCnt].Weight);
			//LOGD(buffer);
			nCnt++;
		}
	}
	*pPoint = pPointData;
#endif
}

struct HorizontalTextureInfo *GetHorizontalConnectingData(void)
{
	if (pHorizontalTextureInfo == NULL)
		return NULL;
	return pHorizontalTextureInfo;
}

void SetHorizontalLength(int Length)
{
	HorizontalLength = Length;
}

int GetHorizontalLength(void)
{
	return HorizontalLength;
}

void UpdateHorizontalLength(int nNewLength)
{
	nUpdateHorizontalLength = nNewLength;
}

int GetUpdateHorizontalLength(void)
{
	return nUpdateHorizontalLength;
}

void GetHorizontalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight)
{
#if 0
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	unsigned char *pInData = NULL;
	pInData = (unsigned char *)malloc(sizeof(unsigned char) * nWidth * nHeight);
	memset(pInData, 0, nWidth * nHeight);

	short *pOutData = NULL;
	pOutData = (short *)malloc(sizeof(short) * nWidth * nHeight);
	memset(pOutData, 0, nWidth * nHeight);

	Cover1DFormat(p2dInData, pInData, nWidth, nHeight, 0);

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0 || i == 1 || i == nWidth - 1 || i == nWidth - 2) {
				sbuf = pInData[j * nWidth + i];
				pOutData[j * nWidth + i] = sbuf;
			}
			else {
				sbuf = pInData[j * nWidth + i] - 128;
				pOutData[j * nWidth + i] = sbuf;
			}
		}
	}
	free(pInData);

	/* 影像範圍為 0~159 */
	/* 將水平的第 0 條、第 1 條分別補上第 2 條的影像值 / 第 158 條、第 159 條分別補上第 157 條的影像值 */
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0 || i == 1) {
				pOutData[j * nWidth + i] = pOutData[j * nWidth + 2];
			}
			if (i == nWidth - 1 || i == nWidth - 2) {
				pOutData[j * nWidth + i] = pOutData[j * nWidth + (nWidth - 3)];
			}
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			p2dOutData[j][i] = pOutData[j * nWidth + i];
		}
	}
	free(pOutData);
#else
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0 || i == 1 || i == nWidth - 1 || i == nWidth - 2) {
				sbuf = p2dInData[j][i];
				p2dOutData[j][i] = sbuf;
			}
			else {
				sbuf = p2dInData[j][i] - 128;
				p2dOutData[j][i] = sbuf;
			}
		}
	}

	/* 影像範圍為 0~159 */
	/* 將水平的第 0 條、第 1 條分別補上第 2 條的影像值 / 第 158 條、第 159 條分別補上第 157 條的影像值 */
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0 || i == 1) {
				p2dOutData[j][i] = p2dOutData[j][2];
			}
			if (i == nWidth - 1 || i == nWidth - 2) {
				p2dOutData[j][i] = p2dOutData[j][nWidth - 3];
			}
		}
	}
#endif
}

void GetVerticalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight)
{
#if 0
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	unsigned char *pInData = NULL;
	pInData = (unsigned char *)malloc(sizeof(unsigned char) * nWidth * nHeight);
	memset(pInData, 0, nWidth * nHeight);

	short *pOutData = NULL;
	pOutData = (short *)malloc(sizeof(short) * nWidth * nHeight);
	memset(pOutData, 0, nWidth * nHeight);

	Cover1DFormat(p2dInData, pInData, nWidth, nHeight, 0);

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0 || j == 1 || j == nHeight - 1 || j == nHeight - 2) {
				sbuf = pInData[j * nWidth + i];
				pOutData[j * nWidth + i] = sbuf;
			}
			else {
				sbuf = pInData[j * nWidth + i] - 128;
				pOutData[j * nWidth + i] = sbuf;
			}
		}
	}
	free(pInData);

	/* 影像範圍為 0~159 */
	/* 將垂直方向的第 0 條、第 1 條分別補上第 2 條的影像值 / 第 158 條、第 159 條分別補上第 157 條的影像值 */
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0 || j == 1) {
				pOutData[j * nWidth + i] = pOutData[2 * nWidth + i];
			}
			if (j == nHeight - 1 || j == nHeight - 2) {
				pOutData[j * nWidth + i] = pOutData[(nHeight - 3) * nWidth + i];
			}
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			p2dOutData[j][i] = pOutData[j * nWidth + i];
		}
	}
	free(pOutData);
#else
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0 || j == 1 || j == nHeight - 1 || j == nHeight - 2) {
				sbuf = p2dInData[j][i];
				p2dOutData[j][i] = sbuf;
			}
			else {
				sbuf = p2dInData[j][i] - 128;
				p2dOutData[j][i] = sbuf;
			}
		}
	}

	/* 影像範圍為 0~159 */
	/* 將垂直方向的第 0 條、第 1 條分別補上第 2 條的影像值 / 第 158 條、第 159 條分別補上第 157 條的影像值 */
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0 || j == 1) {
				p2dOutData[j][i] = p2dOutData[2][i];
			}
			if (j == nHeight - 1 || j == nHeight - 2) {
				p2dOutData[j][i] = p2dOutData[(nHeight - 3)][i];
			}
		}
	}
#endif
}

int GetVerticalConnectingUpDownWidth(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight, unsigned char nStartX, unsigned char nStartY, unsigned char *Uw, unsigned char *Dw, int nIDx)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL || Uw == NULL || Dw == NULL)
		return -1;

	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75;
	int nX, nY, nD, nU, jd, ju;

	/* To Down */
	nX = nStartX;//取 X 座標
	nY = nStartY;//取 Y 座標
	//TRACE("Down Start.... nIDx = %d / (%d, %d)\r\n", nIDx, nX, nY);

	for (jd = 1; jd < 6; jd++) {
		nY = nStartY + jd;//往下移動 Y 座標
		if (nY > nHeight - 3)
			break;

		if (pLabelingData_in[nY][nX] == R0 || pLabelingData_in[nY][nX] == VL || pLabelingData_in[nY][nX] == DL || abs(pDifferentialData_in[nY][nX]) >= THw) {
			//TRACE("jd = %d / pLabelingData_in[%d][%d] = %d / abs(pDifferentialData_in[%d][%d] = %d\r\n", jd, nY, nX, pLabelingData_in[nY][nX], nY, nX, abs(pDifferentialData_in[nY][nX]));
			break;
		}
	}//for (jd = 1; jd < 6; jd++)

	nD = jd - 1;
	//TRACE("nIDx = %d / (%d, %d) / nD = %d\r\n", nIDx, nStartX, nStartY, nD);
	Dw[nStartX] = nD;

	//TRACE("\r\n");

	/* To Up */
	nX = nStartX;//取 X 座標
	nY = nStartY;//取 Y 座標
	//TRACE("Up Start.... nIDx = %d / (%d, %d)\r\n", nIDx, nX, nY);

	for (ju = -1; ju > -6; ju--) {
		nY = nStartY + ju;//往上移動 Y 座標
		if (nY < 2)
			break;

		if (pLabelingData_in[nY][nX] == R0 || pLabelingData_in[nY][nX] == VL || pLabelingData_in[nY][nX] == DL || abs(pDifferentialData_in[nY][nX]) >= THw) {
			//TRACE("ju = %d / pLabelingData_in[%d][%d] = %d / abs(pDifferentialData_in[%d][%d] = %d\r\n", ju, nY, nX, pLabelingData_in[nY][nX], nY, nX, abs(pDifferentialData_in[nY][nX]));
			break;
		}
	}//for (ju = -1; ju > -6; ju--)

	nU = ju + 1;
	//TRACE("nIDx = %d / (%d, %d) / nU = %d\r\n", nIDx, nStartX, nStartY, nU);
	Uw[nStartX] = abs(nU);
	return 0;
}

int GetHorizontalConnectingLeftRightWidth(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight, unsigned char nStartX, unsigned char nStartY, unsigned char *Lw, unsigned char *Rw, int nIDx)
{
	if (pDifferentialData_in == NULL || pLabelingData_in == NULL || Lw == NULL || Rw == NULL)
		return -1;

	unsigned char R0 = 90, V0 = 180, DL = 30, VL = 60, RL = 0, THw = 9, THc = 3, THr = 5, Vx = 75;
	int nX, nY, nR, nL, ir, il;

	/* To Right */
	nX = nStartX;	//取 X 座標
	nY = nStartY;	//取 Y 座標
	//TRACE("Right Start.... nIDx = %d (%d, %d)\r\n", nIDx, nX, nY);

	for (ir = 1; ir < 6; ir++) {
		nX = nStartX + ir;//往右移動 X 座標
		if (nX > nWidth - 3)
			break;

		if (pLabelingData_in[nY][nX] == R0 || pLabelingData_in[nY][nX] == VL || pLabelingData_in[nY][nX] == DL || abs(pDifferentialData_in[nY][nX]) >= THw) {
			//TRACE("ir = %d / pLabelingData_in[%d][%d] = %d / abs(pDifferentialData_in[%d][%d] = %d\r\n", ir, nY, nX, pLabelingData_in[nY][nX], nY, nX, abs(pDifferentialData_in[nY][nX]));
			break;
		}
	}//for (ir = 1; ir < 6; ir++)

	nR = ir - 1;
	//TRACE("nIDx = %d (%d, %d) / nR = %d\r\n", nIDx, nStartX, nStartY, nR);
	Rw[nStartY] = nR;

	//TRACE("\r\n");

	/* To Left */
	nX = nStartX;	//取 X 座標
	nY = nStartY;	//取 Y 座標
	//TRACE("Left Start.... nIDx = %d (%d, %d)\r\n", nIDx, nX, nY);

	for (il = -1; il > -6; il--) {
		nX = nStartX + il;//往左移動 X 座標
		if (nX < 2)
			break;

		if (pLabelingData_in[nY][nX] == R0 || pLabelingData_in[nY][nX] == VL || pLabelingData_in[nY][nX] == DL || abs(pDifferentialData_in[nY][nX]) >= THw) {
			//TRACE("il = %d / pLabelingData_in[%d][%d] = %d / abs(pDifferentialData_in[%d][%d] = %d\r\n", il, nY, nX, pLabelingData_in[nY][nX], nY, nX, abs(pDifferentialData_in[nY][nX]));
			break;
		}
	}//for (il = -1; il > -6; il--)

	nL = il + 1;
	//TRACE("nIDx = %d (%d, %d) / nL = %d\r\n", nIDx, nStartX, nStartY, nL);
	Lw[nStartY] = abs(nL);
	return 0;
}

int CheckVerticalConnectingWidthQuality(short **pDifferentialData_in, unsigned char *pUpWidth, unsigned char *pDownWidth, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx)
{
	if (pDifferentialData_in == NULL || pUpWidth == NULL || pDownWidth == NULL || pWeight == NULL)
		return -1;

	int THw = 9;
	char nWeight;
	unsigned char nU, nD;
	//char buffer[1000] = { 0 };

	nU = pUpWidth[nStartX];
	nD = pDownWidth[nStartX];
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Check Vertical Quality ... [nIDx = %d] nStartX = %d / nStartY = %d / nU = %d / nD = %d\r\n", __FUNCTION__, __LINE__, nIDx, nStartX, nStartY, nU, nD);
	//LOGD(buffer);

	if (nU == 0 && nD == 0) {
		if (abs(pDifferentialData_in[nStartY][nStartX]) >= THw) {
			nWeight = 3;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
			//LOGD(buffer);
		}
		else {
			nWeight = 2;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
			//LOGD(buffer);
		}
	}
	else if ((nU + nD) >= 1 && (nU + nD) <= 2) {
		nWeight = 1;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if ((nU + nD) >= 3 && (nU + nD) <= 4) {
		nWeight = 0;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if (nU == 5 && nD == 5) {
		nWeight = -3;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if (nU == 5 || nD == 5) {
		nWeight = -2;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if ((nU + nD) >= 5) {
		nWeight = -1;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nWeight = %d\r\n", __FUNCTION__, __LINE__, nWeight);
	//LOGD(buffer);
	pWeight[nStartX] = nWeight;
	return 0;
}

int CheckHorizontalConnectingWidthQuality(short **pDifferentialData_in, unsigned char *pLeftWidth, unsigned char *pRightWidth, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx)
{
	if (pDifferentialData_in == NULL || pLeftWidth == NULL || pRightWidth == NULL || pWeight == NULL)
		return -1;

	int THw = 9;
	char nWeight;
	unsigned char nL, nR;
	//char buffer[1000] = { 0 };

	nL = pLeftWidth[nStartY];
	nR = pRightWidth[nStartY];
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Check Horizontal Quality ... [nIDx = %d] nStartX = %d / nStartY = %d / nL = %d / nR = %d\r\n", __FUNCTION__, __LINE__, nIDx, nStartX, nStartY, nL, nR);
	//LOGD(buffer);

	if (nL == 0 && nR == 0) {
		if (abs(pDifferentialData_in[nStartY][nStartX]) >= THw) {
			nWeight = 3;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
			//LOGD(buffer);
		}
		else {
			nWeight = 2;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
			//LOGD(buffer);
		}
	}
	else if ((nL + nR) >= 1 && (nL + nR) <= 2) {
		nWeight = 1;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if ((nL + nR) >= 3 && (nL + nR) <= 4) {
		nWeight = 0;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if (nL == 5 && nR == 5) {
		nWeight = -3;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if (nL == 5 || nR == 5) {
		nWeight = -2;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	else if ((nL + nR) >= 5) {
		nWeight = -1;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nWeight = %d\r\n", __FUNCTION__, __LINE__, nWeight);
	//LOGD(buffer);
	pWeight[nStartY] = nWeight;
	return 0;
}

int CheckHeadTailQuality(bool *bHeadStatus, bool *bTailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh)
{
	if (pWeight == NULL)
		return -1;

	int i;
	unsigned char nGoodHeadQualityStatusCnt = 0;
	unsigned char nGoodTailQualityStatusCnt = 0;
	char nHeadQualityStatus = -1;
	char nTailQualityStatus = -1;
	//char buffer[1000] = { 0 };

	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Head = %d / Tail = %d\r\n", __FUNCTION__, __LINE__, Head, Tail);
	//LOGD(buffer);

	for (i = 0; i < Lenght; i++) {
		if (i >= 0 && i <= 2) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, Head + i, pWeight[Head + i]);
			//LOGD(buffer);
			if (pWeight[Head + i] >= nQualityTh) {
				nGoodHeadQualityStatusCnt++;
			}
		}

		if (i >= (Lenght - 3) && i <= (Lenght - 1)) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, Tail - ((Lenght - 1) - i), pWeight[Tail - ((Lenght - 1) - i)]);
			//LOGD(buffer);
			if (pWeight[Tail - ((Lenght - 1) - i)] >= nQualityTh) {
				nGoodTailQualityStatusCnt++;
			}
		}
	}

	/*Head Quality Status*/
	if (nGoodHeadQualityStatusCnt >= 2)
		nHeadQualityStatus = true;
	else
		nHeadQualityStatus = false;

	/*Tail Quality Status*/
	if (nGoodTailQualityStatusCnt >= 2)
		nTailQualityStatus = true;
	else
		nTailQualityStatus = false;

	*bHeadStatus = nHeadQualityStatus;
	*bTailStatus = nTailQualityStatus;
	return 0;
}

int UpdateHeadTailConnecting(unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh, HeadTailType nHeadTailType, unsigned char *NewHead, unsigned char *NewTail)
{
	if (pWeight == NULL)
		return -1;

	//char buffer[1000] = { 0 };
	int nNewHead = -1, nNewTail = -1, nNewHeadIDx = -1, nNewTailIDx = -1;
	int nHeadStart = 2, nHeadEnd = 0, nTailStart = Lenght - 3, nTailEnd = Lenght - 1;
	int i, nMin;

	if (nHeadTailType == emhtHeadTail) {
		nMin = pWeight[Head + nHeadStart];
		nNewHeadIDx = nHeadStart;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nMin = %d / nNewHeadIDx = %d / nHeadStart = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nMin, nNewHeadIDx, nHeadStart, nHeadStart, pWeight[nHeadStart]);
		//LOGD(buffer);
		for (i = nHeadStart; i >= nHeadEnd; i--) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [i = %d] nMin = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, i, nMin, Head + i, pWeight[Head + i]);
			//LOGD(buffer);
			if (pWeight[Head + i] < nMin && pWeight[Head + i] >= nQualityTh) {
				nMin = pWeight[Head + i];
				nNewHeadIDx = i;
			}
		}
		nNewHead = Head + nNewHeadIDx;

		nMin = pWeight[Tail - ((Lenght - 1) - nTailStart)];
		nNewTailIDx = nTailStart;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nMin = %d / nNewTailIDx = %d / nTailStart = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nMin, nNewTailIDx, nTailStart, Tail - ((Lenght - 1) - nTailStart), pWeight[Tail - ((Lenght - 1) - nTailStart)]);
		//LOGD(buffer);
		//for (i = nTailStart; i < Lenght; i++) {
		for (i = nTailStart; i <= nTailEnd; i++) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [i = %d] nMin = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, i, nMin, Tail - ((Lenght - 1) - i), pWeight[Tail - ((Lenght - 1) - i)]);
			//LOGD(buffer);
			if (pWeight[Tail - ((Lenght - 1) - i)] < nMin && pWeight[Tail - ((Lenght - 1) - i)] >= nQualityTh) {
				nMin = pWeight[Tail - ((Lenght - 1) - i)];
				nNewTailIDx = i;
			}
		}
		nNewTail = Tail - ((Lenght - 1) - nNewTailIDx);

		*NewHead = nNewHead;
		*NewTail = nNewTail;
	} 
	else if (nHeadTailType == emhtHead) {
		nMin = pWeight[Head + nHeadStart];
		nNewHeadIDx = nHeadStart;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nMin = %d / nNewHeadIDx = %d / nHeadStart = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nMin, nNewHeadIDx, nHeadStart, nHeadStart, pWeight[nHeadStart]);
		//LOGD(buffer);
		for (i = nHeadStart; i >= nHeadEnd; i--) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [i = %d] nMin = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, i, nMin, Head + i, pWeight[Head + i]);
			//LOGD(buffer);
			if (pWeight[Head + i] < nMin && pWeight[Head + i] >= nQualityTh) {
				nMin = pWeight[Head + i];
				nNewHeadIDx = i;
			}
		}
		nNewHead = Head + nNewHeadIDx;
		nNewTail = Tail;

		*NewHead = nNewHead;
		*NewTail = nNewTail;
	}
	else if (nHeadTailType == emhtTail) {
		nMin = pWeight[Tail - ((Lenght - 1) - nTailStart)];
		nNewTailIDx = nTailStart;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nMin = %d / nNewTailIDx = %d / nTailStart = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nMin, nNewTailIDx, nTailStart, Tail - ((Lenght - 1) - nTailStart), pWeight[Tail - ((Lenght - 1) - nTailStart)]);
		//LOGD(buffer);
		for (i = nTailStart; i < Lenght; i++) {
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [i = %d] nMin = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, i, nMin, Tail - ((Lenght - 1) - i), pWeight[Tail - ((Lenght - 1) - i)]);
			//LOGD(buffer);
			if (pWeight[Tail - ((Lenght - 1) - i)] < nMin && pWeight[Tail - ((Lenght - 1) - i)] >= nQualityTh) {
				nMin = pWeight[Tail - ((Lenght - 1) - i)];
				nNewTailIDx = i;
			}
		}
		nNewTail = Tail - ((Lenght - 1) - nNewTailIDx);
		nNewHead = Head;

		*NewHead = nNewHead;
		*NewTail = nNewTail;
	}
	else {
		nNewHead = Head;
		nNewTail = Tail;

		*NewHead = nNewHead;
		*NewTail = nNewTail;
	}
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nIDx = %d / nNewHead = %d / nNewTail = %d / [nHeadTailType = %d]\r\n", __FUNCTION__, __LINE__, nIDx, nNewHead, nNewTail, nHeadTailType);
	//LOGD(buffer);
	return 0;
}

int CheckV0ConnectingQuality(unsigned char **pLabelingData_in, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pLabelingData_in == NULL || pWeight == NULL)
		return -1;

	unsigned char Vx = 75, V1 = 210;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / nQualityTh = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nIDx, nQualityTh, nStartX, nStartY);
	//LOGD(buffer);

	if (nHorizontalVerticalType == emhvHorizontal) {
		if (pLabelingData_in[nStartY][nStartX] == Vx && pWeight[nStartY] < nQualityTh) {
			//將該點標記 Vx 改變為 V1
			pLabelingData_in[nStartY][nStartX] = V1;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / (%d, %d) / pLabelingData_in[%d][%d] =% d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, nStartX, nStartY, nStartY, nStartX, pLabelingData_in[nStartY][nStartX], nStartY, pWeight[nStartY]);
			//LOGD(buffer);
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		if (pLabelingData_in[nStartY][nStartX] == Vx && pWeight[nStartX] < nQualityTh) {
			//將該點標記 Vx 改變為 V1
			pLabelingData_in[nStartY][nStartX] = V1;
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / (%d, %d) / pLabelingData_in[%d][%d] =% d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, nStartX, nStartY, nStartY, nStartX, pLabelingData_in[nStartY][nStartX], nStartY, pWeight[nStartX]);
			//LOGD(buffer);
		}
	}
	return 0;
}

int CheckV0ConnectingLinePointQuality(unsigned char **pLabelingData_in, char *pWeight, unsigned char *pConnectingArray, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pLabelingData_in == NULL || pWeight == NULL || pConnectingArray == NULL)
		return -1;

	unsigned char Vx = 75, V1 = 210;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / nQualityTh = %d\r\n", __FUNCTION__, __LINE__, nIDx, nQualityTh);
	//LOGD(buffer);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int j = 0; j < nHeight; j++) {
			if (pLabelingData_in[j][pConnectingArray[j]] == Vx && pWeight[j] < nQualityTh) {
				pLabelingData_in[j][pConnectingArray[j]] = V1;
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / (%d, %d) / pLabelingData_in[%d][%d] = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, pConnectingArray[j], j, j, pConnectingArray[j], pLabelingData_in[j][pConnectingArray[j]], j, pWeight[j]);
				//LOGD(buffer);
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth; i++) {
			if (pLabelingData_in[pConnectingArray[i]][i] == Vx && pWeight[i] < nQualityTh) {
				pLabelingData_in[pConnectingArray[i]][i] = V1;
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / (%d, %d) / pLabelingData_in[%d][%d] = %d / pWeight[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, i, pConnectingArray[i], pConnectingArray[i], i, pLabelingData_in[pConnectingArray[i]][i], i, pWeight[i]);
				//LOGD(buffer);
			}
		}
	}
	return 0;
}

int CheckConnectingToEndingMinutiae(bool *HeadStatus, bool *TailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType, unsigned char *pNewHead, unsigned char *pNewTail)
{
	if (pWeight == NULL)
		return -1;

	bool bHeadStatus, bTailStatus;
	unsigned char NewHead = 0, NewTail = 0;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / nHorizontalVerticalType = %d / nQualityTh = %d\r\n", __FUNCTION__, __LINE__, nIDx, nHorizontalVerticalType, nQualityTh);
	//LOGD(buffer);

	if (nHorizontalVerticalType == emhvHorizontal) {
		CheckHeadTailQuality(&bHeadStatus, &bTailStatus, Head, Tail, Lenght, pWeight, nIDx, nQualityTh);
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Head = %d / Tail = %d / Lenght = %d / bHeadStatus = %d / bTailStatus = %d\r\n", __FUNCTION__, __LINE__, Head, Tail, Lenght, bHeadStatus, bTailStatus);
		//LOGD(buffer);

		if (bHeadStatus == true && bTailStatus == true) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtHeadTail, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else if (bHeadStatus == true && bTailStatus == false) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtHead, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else if (bHeadStatus == false && bTailStatus == true) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtTail, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtNone, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		CheckHeadTailQuality(&bHeadStatus, &bTailStatus, Head, Tail, Lenght, pWeight, nIDx, nQualityTh);
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d Head = %d / Tail = %d / Lenght = %d / bHeadStatus = %d / bTailStatus = %d\r\n", __FUNCTION__, __LINE__, Head, Tail, Lenght, bHeadStatus, bTailStatus);
		//LOGD(buffer);

		if (bHeadStatus == true && bTailStatus == true) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtHeadTail, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else if (bHeadStatus == true && bTailStatus == false) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtHead, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else if (bHeadStatus == false && bTailStatus == true) {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtTail, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
		else {
			UpdateHeadTailConnecting(Head, Tail, Lenght, pWeight, nIDx, nQualityTh, emhtNone, &NewHead, &NewTail);
			//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [Update] NewHead = %d / NewTail = %d\r\n", __FUNCTION__, __LINE__, NewHead, NewTail);
			//LOGD(buffer);
		}
	}
	*HeadStatus = bHeadStatus;
	*TailStatus = bTailStatus;
	*pNewHead = NewHead;
	*pNewTail = NewTail;
	return 0;
}

int SetEndingMinutiaeData(struct MinutiaePOINT **pEndingMinutiae, int nEndingLength, unsigned char *pConnectingArray, unsigned char NewHead, unsigned char NewTail, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pEndingMinutiae == NULL || pConnectingArray == NULL || nEndingLength == 0)
		return -1;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nEndingLength = %d / NewHead = %d / NewTail = %d / nHorizontalVerticalType = %d\r\n", __FUNCTION__, __LINE__, nEndingLength, NewHead, NewTail, nHorizontalVerticalType);
	//LOGD(buffer);

	struct MinutiaePOINT *pEndingPointData = NULL;
	pEndingPointData = (struct MinutiaePOINT *) malloc(sizeof(struct MinutiaePOINT) * nEndingLength);
	memset(pEndingPointData, 0, sizeof(struct MinutiaePOINT) * nEndingLength);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nCnt = 0;
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int j = 0; j < nHeight; j++) {
			if (j == NewHead || j == NewTail) {
				pEndingPointData[nCnt].X = pConnectingArray[j]; //存 X 座標
				pEndingPointData[nCnt].Y = j;					//存 Y 座標
				pEndingPointData[nCnt].T = emEnding;			//存 Minutiae 的型態分類
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pEndingPointData[%d].X = %d / pEndingPointData[%d].Y = %d / pEndingPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pEndingPointData[nCnt].X, nCnt, pEndingPointData[nCnt].Y, nCnt, pEndingPointData[nCnt].T);
				//LOGD(buffer);
				nCnt++;
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nCnt = 0;
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth; i++) {
			if (i == NewHead || i == NewTail) {
				pEndingPointData[nCnt].X = i;					//存 X 座標
				pEndingPointData[nCnt].Y = pConnectingArray[i];	//存 Y 座標
				pEndingPointData[nCnt].T = emEnding;			//存 Minutiae 的型態分類
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pEndingPointData[%d].X = %d / pEndingPointData[%d].Y = %d / pEndingPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pEndingPointData[nCnt].X, nCnt, pEndingPointData[nCnt].Y, nCnt, pEndingPointData[nCnt].T);
				//LOGD(buffer);
				nCnt++;
			}
		}
	}
	*pEndingMinutiae = pEndingPointData;
	return 0;
}

int CheckConnectingToJumpingMinutiae(unsigned char *pConnectingArray, int nThJumpingLength, unsigned char *pJumpingArray, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pConnectingArray == NULL || pJumpingArray == NULL)
		return -1;

	int nCnt = 0;
	int nLength = 0;
	//char buffer[1000] = { 0 };

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int j = 0; j < nHeight - 1; j++) {
			if (pConnectingArray[j] != 0 && pConnectingArray[j + 1] != 0) {
				nLength = pConnectingArray[j + 1] - pConnectingArray[j];
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nLength = %d / %d + 1 = %d / j = %d / %d - %d = %d\r\n", __FUNCTION__, __LINE__, nLength, j, j + 1, j, pConnectingArray[j + 1], pConnectingArray[j],pConnectingArray[j + 1] - pConnectingArray[j]);
				//LOGD(buffer);
				if (abs(nLength) >= nThJumpingLength) {
					pJumpingArray[j + 1] = pConnectingArray[j + 1];//存 X 座標
					nCnt++;
					//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, pConnectingArray[j + 1], j + 1);
					//LOGD(buffer);
				}
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth - 1; i++) {
			if (pConnectingArray[i] != 0 && pConnectingArray[i + 1] != 0) {
				nLength = pConnectingArray[i + 1] - pConnectingArray[i];
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nLength = %d / %d + 1 = %d / i = %d / %d - %d = %d\r\n", __FUNCTION__, __LINE__, nLength, i, i + 1, i, pConnectingArray[i + 1], pConnectingArray[i], pConnectingArray[i + 1] - pConnectingArray[i]);
				//LOGD(buffer);
				if (abs(nLength) >= nThJumpingLength) {
					pJumpingArray[i + 1] = pConnectingArray[i + 1];//存 Y 座標
					nCnt++;
					//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nCnt, i + 1, pConnectingArray[i + 1]);
					//LOGD(buffer);
				}	
			}
		}
	}
	return nCnt;
}

int SetJumpingMinutiaeData(struct MinutiaePOINT **pJumpingMinutiae, int nJumpingLength, unsigned char *pJumpingArray, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pJumpingMinutiae == NULL || nJumpingLength == 0 || pJumpingArray == NULL)
		return -1;
	
	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nJumpingLength = %d / nHorizontalVerticalType = %d\r\n", __FUNCTION__, __LINE__, nJumpingLength, nHorizontalVerticalType);
	//LOGD(buffer);

	struct MinutiaePOINT *pJumpingPointData = NULL;
	pJumpingPointData = (struct MinutiaePOINT *) malloc(sizeof(struct MinutiaePOINT) * nJumpingLength);
	memset(pJumpingPointData, 0, sizeof(struct MinutiaePOINT) * nJumpingLength);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nCnt = 0;
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int j = 0; j < nHeight; j++) {
			if (pJumpingArray[j] != 0) {
				pJumpingPointData[nCnt].X = pJumpingArray[j];//存 X 座標
				pJumpingPointData[nCnt].Y = j;				 //存 Y 座標
				pJumpingPointData[nCnt].T = emJumping;		 //存 Minutiae 的型態分類
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pJumpingPointData[%d].X = %d / pJumpingPointData[%d].Y = %d / pJumpingPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pJumpingPointData[nCnt].X, nCnt, pJumpingPointData[nCnt].Y, nCnt, pJumpingPointData[nCnt].T);
				//LOGD(buffer);
				nCnt++;
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nCnt = 0;
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth - 1; i++) {
			if (pJumpingArray[i] != 0) {
				pJumpingPointData[nCnt].X = i;				 //存 X 座標
				pJumpingPointData[nCnt].Y = pJumpingArray[i];//存 Y 座標
				pJumpingPointData[nCnt].T = emJumping;		 //存 Minutiae 的型態分類
				//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pJumpingPointData[%d].X = %d / pJumpingPointData[%d].Y = %d / pJumpingPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pJumpingPointData[nCnt].X, nCnt, pJumpingPointData[nCnt].Y, nCnt, pJumpingPointData[nCnt].T);
				//LOGD(buffer);
				nCnt++;
			}
		}
	}
	*pJumpingMinutiae = pJumpingPointData;
	return 0;
}

int CheckConnectingToBifurcationMinutiae(unsigned char *pConnectingArray, unsigned char(*pBifurcationArray)[IMAGE_HEIGHT]/*unsigned char pBifurcationArray[MaxRecordItemLength][IMAGE_HEIGHT]*/, int nMaxRecordNum, int nIDx, struct BifurcationInfo *pBifurcationInfo, int *pCurrentBifurcationArrayIDx, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pConnectingArray == NULL || pBifurcationArray == NULL || pBifurcationInfo == NULL)
		return -1;
	
	//char buffer[1000] = { 0 };
	int nBifurcationFindLineIDx;		/* 紀錄發現分岔點的線段Index */
	int nBifurcationFindLineArrayIDx;	/* 紀錄發現分岔點的陣列Index */
	int nBifurcationLineIDx;			/* 紀錄被分岔點給叉到的線段Index */
	int nCnt = 0;
	int nCurrentArrayIndex;				/* 載入目前指標所在記憶體的位置 */
	nCurrentArrayIndex = *pCurrentBifurcationArrayIDx;
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / nMaxRecordNum = %d / nCurrentArrayIndex = %d / *pCurrentBifurcationArrayIDx = %d / nHorizontalVerticalType = %d\r\n", __FUNCTION__, __LINE__, nIDx, nMaxRecordNum, nCurrentArrayIndex, *pCurrentBifurcationArrayIDx, nHorizontalVerticalType);
	//LOGD(buffer);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int j = 0; j < nHeight; j++) {
				if (pBifurcationArray[ni][j] != 0) {
					if (pConnectingArray[j] != 0) {
						if (pBifurcationArray[ni][j] == pConnectingArray[j]) {
							nBifurcationFindLineIDx = ni;
							nBifurcationFindLineArrayIDx = j;
							nBifurcationLineIDx = nIDx;
							//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / pBifurcationArray[%d][%d] = %d / (%d, %d) / pConnectingArray[%d] = %d / (%d, %d) / nBifurcationFindLineIDx = %d / nBifurcationFindLineArrayIDx = %d / nBifurcationLineIDx = %d\r\n", __FUNCTION__, __LINE__, nIDx, ni, j, pBifurcationArray[ni][j], pBifurcationArray[ni][j], j, j, pConnectingArray[j], pConnectingArray[j], j, nBifurcationFindLineIDx, nBifurcationFindLineArrayIDx, nBifurcationLineIDx);
							//LOGD(buffer);
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xX = pBifurcationArray[ni][j]; /*紀錄分叉點 X 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xY = j;						   /*紀錄分叉點 Y 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xL = ni;					   /*紀錄分叉點線段*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL = nIDx;					   /*紀錄該線段有被其他分叉點給岔到*/
							//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / pBifurcationInfo[%d].xX = %d / pBifurcationInfo[%d].xY = %d / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", __FUNCTION__, __LINE__, nIDx, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xX, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xY, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xL, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL);
							//LOGD(buffer);
							nCnt++;
						}
					}
				}
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth - 1; i++) {
			for (int ni = 0; ni < nMaxRecordNum; ni++) {
				if (pBifurcationArray[ni][i] != 0) {
					if (pConnectingArray[i] != 0) {
						if (pBifurcationArray[ni][i] == pConnectingArray[i]) {
							nBifurcationFindLineIDx = ni;
							nBifurcationFindLineArrayIDx = i;
							nBifurcationLineIDx = nIDx;
							//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / pBifurcationArray[%d][%d] = %d / (%d, %d) / pConnectingArray[%d] = %d / (%d, %d) / nBifurcationFindLineIDx = %d / nBifurcationFindLineArrayIDx = %d / nBifurcationLineIDx = %d\r\n", __FUNCTION__, __LINE__, nIDx, ni, i, pBifurcationArray[ni][i], i, pBifurcationArray[ni][i], i, pConnectingArray[i], i, pConnectingArray[i], nBifurcationFindLineIDx, nBifurcationFindLineArrayIDx, nBifurcationLineIDx);
							//LOGD(buffer);
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xX = i;						   /*紀錄分叉點 X 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xY = pBifurcationArray[ni][i]; /*紀錄分叉點 Y 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xL = ni;					   /*紀錄分叉點線段*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL = nIDx;					   /*紀錄該線段有被其他分叉點給岔到*/
							//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] / pBifurcationInfo[%d].xX = %d / pBifurcationInfo[%d].xY = %d / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", __FUNCTION__, __LINE__, nIDx, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xX, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xY, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xL, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL);
							//LOGD(buffer);
							nCnt++;
						}
					}
				}
			}
		}
	}
	return nCnt;
}

int SetBifurcationMinutiaeData(struct MinutiaePOINT **pBifurcationMinutiae, int nBifurcationLength, unsigned char *pConnectingArray, unsigned char(*pBifurcationArray)[IMAGE_HEIGHT]/*unsigned char pBifurcationArray[MaxRecordItemLength][IMAGE_HEIGHT]*/, int nMaxRecordNum, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pBifurcationMinutiae == NULL || nBifurcationLength == 0 || pBifurcationArray == NULL)
		return -1;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nBifurcationLength = %d / nHorizontalVerticalType = %d\r\n", __FUNCTION__, __LINE__, nBifurcationLength, nHorizontalVerticalType);
	//LOGD(buffer);
	
	struct MinutiaePOINT *pBifurcationPointData = NULL;
	pBifurcationPointData = (struct MinutiaePOINT *) malloc(sizeof(struct MinutiaePOINT) * nBifurcationLength);
	memset(pBifurcationPointData, 0, sizeof(struct MinutiaePOINT) * nBifurcationLength);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nCnt = 0;
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int j = 0; j < nHeight; j++) {
				if (pBifurcationArray[ni][j] != 0 && pConnectingArray[j] != 0) {
					if (pBifurcationArray[ni][j] == pConnectingArray[j]) {
						pBifurcationPointData[nCnt].X = pBifurcationArray[ni][j];//存 X 座標
						pBifurcationPointData[nCnt].Y = j;						 //存 Y 座標
						pBifurcationPointData[nCnt].T = emBifurcation;			 //存 Minutiae 的型態分類
						//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pBifurcationPointData[%d].X = %d / pBifurcationPointData[%d].Y = %d / pBifurcationPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pBifurcationPointData[nCnt].X, nCnt, pBifurcationPointData[nCnt].Y, nCnt, pBifurcationPointData[nCnt].T);
						//LOGD(buffer);
						nCnt++;
					}
				}
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nCnt = 0;
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int i = 0; i < nWidth - 1; i++) {
				if (pBifurcationArray[ni][i] != 0 && pConnectingArray[i] != 0) {
					if (pBifurcationArray[ni][i] == pConnectingArray[i]) {
						pBifurcationPointData[nCnt].X = i;						 //存 X 座標
						pBifurcationPointData[nCnt].Y = pBifurcationArray[ni][i];//存 Y 座標
						pBifurcationPointData[nCnt].T = emBifurcation;			 //存 Minutiae 的型態分類
						//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nHorizontalVerticalType = %d] pBifurcationPointData[%d].X = %d / pBifurcationPointData[%d].Y = %d / pBifurcationPointData[%d].T = %d\r\n", __FUNCTION__, __LINE__, nHorizontalVerticalType, nCnt, pBifurcationPointData[nCnt].X, nCnt, pBifurcationPointData[nCnt].Y, nCnt, pBifurcationPointData[nCnt].T);
						//LOGD(buffer);
						nCnt++;
					}
				}
			}
		}
	}
	*pBifurcationMinutiae = pBifurcationPointData;
	return 0;
}

void SetBifurcationInfo(struct BifurcationInfo **pBifurcationInfo_Out, struct BifurcationInfo *pBifurcationInfo, int nBifurcationInfoLength, int *pCurrentBifurcationArrayIDx)
{
	if (pBifurcationInfo_Out == NULL || pBifurcationInfo == NULL || nBifurcationInfoLength == 0)
		return;

	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d nBifurcationInfoLength = %d / *pCurrentBifurcationArrayIDx = %d\r\n", __FUNCTION__, __LINE__, nBifurcationInfoLength, *pCurrentBifurcationArrayIDx);
	//LOGD(buffer);
	
	int nCurrentArrayIndex;			/* 載入目前指標所在記憶體的位置 */
	nCurrentArrayIndex = *pCurrentBifurcationArrayIDx;

	struct BifurcationInfo *pBifurcationInfoData = NULL;
	pBifurcationInfoData = (struct BifurcationInfo *) malloc(sizeof(struct BifurcationInfo) * nBifurcationInfoLength);
	memset(pBifurcationInfoData, 0, sizeof(struct BifurcationInfo) * nBifurcationInfoLength);

	for (int i = 0; i < nBifurcationInfoLength; i++) {
		pBifurcationInfoData[i].xX = pBifurcationInfo[i + nCurrentArrayIndex].xX;
		pBifurcationInfoData[i].xY = pBifurcationInfo[i + nCurrentArrayIndex].xY;
		pBifurcationInfoData[i].xL = pBifurcationInfo[i + nCurrentArrayIndex].xL;
		pBifurcationInfoData[i].BxL = pBifurcationInfo[i + nCurrentArrayIndex].BxL;
		//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d pBifurcationInfoData[%d].xX = %d / pBifurcationInfoData[%d].xY = %d / pBifurcationInfoData[%d].xL = %d / pBifurcationInfoData[%d].BxL = %d\r\n", __FUNCTION__, __LINE__, i, pBifurcationInfoData[i].xX, i, pBifurcationInfoData[i].xY, i, pBifurcationInfoData[i].xL, i, pBifurcationInfoData[i].BxL);
		//LOGD(buffer);
	}
	*pBifurcationInfo_Out = pBifurcationInfoData;
}

void CheckBifurcation(unsigned char *pConnectingArray, unsigned char *pBifurcationArray, int nIDx, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pConnectingArray == NULL || pBifurcationArray == NULL)
		return;

	char buffer[1000] = { 0 };
	if (nHorizontalVerticalType == emhvHorizontal) {
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int j = 0; j < nHeight; j++) {
			sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] pConnectingArray[%d] = %d / pBifurcationArray[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, j, pConnectingArray[j], j, pBifurcationArray[j]);
			LOGD(buffer);
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth; i++) {
			sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] pConnectingArray[%d] = %d / pBifurcationArray[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, i, pConnectingArray[i], i, pBifurcationArray[i]);
			LOGD(buffer);
		}
	}
}

void UpdateConnectingQualityReferenceBifurcation(bool *HeadStatus, bool *TailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, int nIDx, int *pBifurcationFindLineArrayIndex, HorizontalVerticalType nHorizontalVerticalType)
{
	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] *HeadStatus = %d / *TailStatus = %d / Head = %d / Tail = %d / Lenght = %d / *pBifurcationFindLineArrayIndex = %d\r\n", __FUNCTION__, __LINE__, nIDx, *HeadStatus, *TailStatus, Head, Tail, Lenght, *pBifurcationFindLineArrayIndex);
	//LOGD(buffer);
	if (nHorizontalVerticalType == emhvHorizontal) {
		if ((Tail + 1) == *pBifurcationFindLineArrayIndex) {
			*TailStatus = false;
		}
		else if ((Head - 1) == *pBifurcationFindLineArrayIndex) {
			*HeadStatus = false;
		}
		else {
			return;
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		if ((Tail + 1) == *pBifurcationFindLineArrayIndex) {
			*TailStatus = false;
		}
		else if ((Head - 1) == *pBifurcationFindLineArrayIndex) {
			*HeadStatus = false;
		}
		else {
			return;
		}
	}
	//sprintf(buffer, "[FPImageUtil.c] %s() #Line %d [nIDx = %d] [Update] *HeadStatus = %d / *TailStatus = %d / Head = %d / Tail = %d / Lenght = %d / *pBifurcationFindLineArrayIndex = %d\r\n", __FUNCTION__, __LINE__, nIDx, *HeadStatus, *TailStatus, Head, Tail, Lenght, *pBifurcationFindLineArrayIndex);
	//LOGD(buffer);
}

int CheckImageConnectingToBifurcationMinutiae(unsigned char *pConnectingArray, unsigned char **pBifurcationArray, int nMaxRecordNum, int nIDx, struct BifurcationInfo *pBifurcationInfo, int *pCurrentBifurcationArrayIDx, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pConnectingArray == NULL || pBifurcationArray == NULL || pBifurcationInfo == NULL)
		return -1;

	int nBifurcationFindLineIDx;		/* 紀錄發現分岔點的線段Index */
	int nBifurcationFindLineArrayIDx;	/* 紀錄發現分岔點的陣列Index */
	int nBifurcationLineIDx;			/* 紀錄被分岔點給叉到的線段Index */
	int nCnt = 0;
	int nCurrentArrayIndex;				/* 載入目前指標所在記憶體的位置 */
	nCurrentArrayIndex = *pCurrentBifurcationArrayIDx;

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int j = 0; j < nHeight; j++) {
				if (pBifurcationArray[ni][j] != 0) {
					if (pConnectingArray[j] != 0) {
						if (pBifurcationArray[ni][j] == pConnectingArray[j]) {
							nBifurcationFindLineIDx = ni;
							nBifurcationFindLineArrayIDx = j;
							nBifurcationLineIDx = nIDx;
							//TRACE("[nIDx = %d] / pBifurcationArray[%d][%d] = %d / (%d, %d) / pConnectingArray[%d] = %d / (%d, %d) / nBifurcationFindLineIDx = %d / nBifurcationFindLineArrayIDx = %d / nBifurcationLineIDx = %d\r\n", nIDx, ni, j, pBifurcationArray[ni][j], pBifurcationArray[ni][j], j, j, pConnectingArray[j], pConnectingArray[j], j, nBifurcationFindLineIDx, nBifurcationFindLineArrayIDx, nBifurcationLineIDx);
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xX = pBifurcationArray[ni][j]; /*紀錄分叉點 X 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xY = j;						   /*紀錄分叉點 Y 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xL = ni;					   /*紀錄分叉點線段*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL = nIDx;					   /*紀錄該線段有被其他分叉點給岔到*/
							//TRACE("[nIDx = %d] / pBifurcationInfo[%d].xX = %d / pBifurcationInfo[%d].xY = %d / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", nIDx, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xX, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xY, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xL, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL);
							nCnt++;
						}
					}
				}
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int i = 0; i < nWidth - 1; i++) {
			for (int ni = 0; ni < nMaxRecordNum; ni++) {
				if (pBifurcationArray[ni][i] != 0) {
					if (pConnectingArray[i] != 0) {
						if (pBifurcationArray[ni][i] == pConnectingArray[i]) {
							nBifurcationFindLineIDx = ni;
							nBifurcationFindLineArrayIDx = i;
							nBifurcationLineIDx = nIDx;
							//TRACE("[nIDx = %d] / pBifurcationArray[%d][%d] = %d / (%d, %d) / pConnectingArray[%d] = %d / (%d, %d) / nBifurcationFindLineIDx = %d / nBifurcationFindLineArrayIDx = %d / nBifurcationLineIDx = %d\r\n", nIDx, ni, i, pBifurcationArray[ni][i], i, pBifurcationArray[ni][i], i, pConnectingArray[i], i, pConnectingArray[i], nBifurcationFindLineIDx, nBifurcationFindLineArrayIDx, nBifurcationLineIDx);
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xX = i;						   /*紀錄分叉點 X 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xY = pBifurcationArray[ni][i]; /*紀錄分叉點 Y 座標*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].xL = ni;					   /*紀錄分叉點線段*/
							pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL = nIDx;					   /*紀錄該線段有被其他分叉點給岔到*/
							//TRACE("[nIDx = %d] / pBifurcationInfo[%d].xX = %d / pBifurcationInfo[%d].xY = %d / pBifurcationInfo[%d].xL = %d / pBifurcationInfo[%d].BxL = %d\r\n", nIDx, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xX, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xY, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].xL, nCnt + nCurrentArrayIndex, pBifurcationInfo[nCnt + nCurrentArrayIndex].BxL);
							nCnt++;
						}
					}
				}
			}
		}
	}
	return nCnt;
}

int SetImageBifurcationMinutiaeData(struct MinutiaePOINT **pBifurcationMinutiae, int nBifurcationLength, unsigned char *pConnectingArray, unsigned char **pBifurcationArray, int nMaxRecordNum, HorizontalVerticalType nHorizontalVerticalType)
{
	if (pBifurcationMinutiae == NULL || nBifurcationLength == 0 || pBifurcationArray == NULL)
		return -1;

	struct MinutiaePOINT *pBifurcationPointData = NULL;
	pBifurcationPointData = (struct MinutiaePOINT *) malloc(sizeof(struct MinutiaePOINT) * nBifurcationLength);
	memset(pBifurcationPointData, 0, sizeof(struct MinutiaePOINT) * nBifurcationLength);

	if (nHorizontalVerticalType == emhvHorizontal) {
		int nCnt = 0;
		int nHeight = /*IMAGE_HEIGHT*/nSensorHeight;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int j = 0; j < nHeight; j++) {
				if (pBifurcationArray[ni][j] != 0 && pConnectingArray[j] != 0) {
					if (pBifurcationArray[ni][j] == pConnectingArray[j]) {
						pBifurcationPointData[nCnt].X = pBifurcationArray[ni][j];//存 X 座標
						pBifurcationPointData[nCnt].Y = j;						 //存 Y 座標
						pBifurcationPointData[nCnt].T = emBifurcation;			 //存 Minutiae 的型態分類
						//TRACE("[nHorizontalVerticalType = %d] pBifurcationPointData[%d].X = %d / pBifurcationPointData[%d].Y = %d / pBifurcationPointData[%d].T = %d\r\n", nHorizontalVerticalType, nCnt, pBifurcationPointData[nCnt].X, nCnt, pBifurcationPointData[nCnt].Y, nCnt, pBifurcationPointData[nCnt].T);
						nCnt++;
					}
				}
			}
		}
	}
	else if (nHorizontalVerticalType == emhvVertical) {
		int nCnt = 0;
		int nWidth = /*IMAGE_WIDTH*/nSensorWidth;
		for (int ni = 0; ni < nMaxRecordNum; ni++) {
			for (int i = 0; i < nWidth - 1; i++) {
				if (pBifurcationArray[ni][i] != 0 && pConnectingArray[i] != 0) {
					if (pBifurcationArray[ni][i] == pConnectingArray[i]) {
						pBifurcationPointData[nCnt].X = i;						 //存 X 座標
						pBifurcationPointData[nCnt].Y = pBifurcationArray[ni][i];//存 Y 座標
						pBifurcationPointData[nCnt].T = emBifurcation;			 //存 Minutiae 的型態分類
						//TRACE("[nHorizontalVerticalType = %d] pBifurcationPointData[%d].X = %d / pBifurcationPointData[%d].Y = %d / pBifurcationPointData[%d].T = %d\r\n", nHorizontalVerticalType, nCnt, pBifurcationPointData[nCnt].X, nCnt, pBifurcationPointData[nCnt].Y, nCnt, pBifurcationPointData[nCnt].T);
						nCnt++;
					}
				}
			}
		}
	}
	*pBifurcationMinutiae = pBifurcationPointData;
	return 0;
}
