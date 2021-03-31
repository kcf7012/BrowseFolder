#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <tchar.h>
#include "FPImageUtil.h"
#include "FPTexture.h"

#define xVFAbs(x) abs(x)
#define xVFMax(a, b) ((b) > (a) ? (b) : (a))
#define xVFMin(a, b) ((b) < (a) ? (b) : (a))
#define xVFRound(x) ((INT)((x) >= 0 ? (x) + 0.5 : (x) - 0.5))
#define xVFRoundP(x) ((INT)((x) + 0.5))
#define xVFSqr(x) ((x) * (x))

#define xVFDIR_0    0
#define xVFDIR_45  30
#define xVFDIR_90  (xVFDIR_45 * 2)
#define xVFDIR_135 (xVFDIR_45 * 3)
#define xVFDIR_180 (xVFDIR_45 * 4)
#define xVFDIR_225 (xVFDIR_45 * 5)
#define xVFDIR_270 (xVFDIR_45 * 6)
#define xVFDIR_315 (xVFDIR_45 * 7)
#define xVFDIR_360 (xVFDIR_45 * 8)
#define xVFDIR_UNKNOWN    127
#define xVFDIR_BACKGROUND 255

#define xVF_ATANS_MAX 50
unsigned char _vfAtans_[xVF_ATANS_MAX][xVF_ATANS_MAX] =
{
	{ 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 60,30,18,12, 9, 8, 6, 5, 5, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 60,42,30,22,18,15,12,11, 9, 8, 8, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	{ 60,48,38,30,25,21,18,15,14,12,11,10, 9, 9, 8, 8, 7, 7, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2 },
	{ 60,51,42,35,30,26,22,20,18,16,15,13,12,11,11,10, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3 },
	{ 60,52,45,39,34,30,27,24,21,19,18,16,15,14,13,12,12,11,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4 },
	{ 60,54,48,42,38,33,30,27,25,22,21,19,18,17,15,15,14,13,12,12,11,11,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5 },
	{ 60,55,49,45,40,36,33,30,27,25,23,22,20,19,18,17,16,15,14,13,13,12,12,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 5 },
	{ 60,55,51,46,42,39,35,33,30,28,26,24,22,21,20,19,18,17,16,15,15,14,13,13,12,12,11,11,11,10,10,10, 9, 9, 9, 9, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 6, 6 },
	{ 60,56,52,48,44,41,38,35,32,30,28,26,25,23,22,21,20,19,18,17,16,15,15,14,14,13,13,12,12,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7 },
	{ 60,56,52,49,45,42,39,37,34,32,30,28,27,25,24,22,21,20,19,19,18,17,16,16,15,15,14,14,13,13,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8 },
	{ 60,57,53,50,47,44,41,38,36,34,32,30,28,27,25,24,23,22,21,20,19,18,18,17,16,16,15,15,14,14,13,13,13,12,12,12,11,11,11,11,10,10,10,10, 9, 9, 9, 9, 9, 8 },
	{ 60,57,54,51,48,45,42,40,38,35,33,32,30,28,27,26,25,23,22,22,21,20,19,18,18,17,17,16,15,15,15,14,14,13,13,13,12,12,12,11,11,11,11,10,10,10,10,10, 9, 9 },
	{ 60,57,54,51,49,46,43,41,39,37,35,33,32,30,29,27,26,25,24,23,22,21,20,20,19,18,18,17,17,16,16,15,15,14,14,14,13,13,13,12,12,12,11,11,11,11,11,10,10,10 },
	{ 60,57,55,52,49,47,45,42,40,38,36,35,33,31,30,29,27,26,25,24,23,22,22,21,20,19,19,18,18,17,17,16,16,15,15,15,14,14,13,13,13,13,12,12,12,12,11,11,11,11 },
	{ 60,57,55,52,50,48,45,43,41,39,38,36,34,33,31,30,29,28,27,26,25,24,23,22,21,21,20,19,19,18,18,17,17,16,16,15,15,15,14,14,14,13,13,13,13,12,12,12,12,11 },
	{ 60,58,55,53,51,48,46,44,42,40,39,37,35,34,33,31,30,29,28,27,26,25,24,23,22,22,21,20,20,19,19,18,18,17,17,16,16,16,15,15,15,14,14,14,13,13,13,13,12,12 },
	{ 60,58,56,53,51,49,47,45,43,41,40,38,37,35,34,32,31,30,29,28,27,26,25,24,24,23,22,21,21,20,20,19,19,18,18,17,17,16,16,16,15,15,15,14,14,14,14,13,13,13 },
	{ 60,58,56,54,52,50,48,46,44,42,41,39,38,36,35,33,32,31,30,29,28,27,26,25,25,24,23,22,22,21,21,20,20,19,19,18,18,17,17,17,16,16,15,15,15,15,14,14,14,13 },
	{ 60,58,56,54,52,50,48,47,45,43,41,40,38,37,36,34,33,32,31,30,29,28,27,26,26,25,24,23,23,22,22,21,20,20,19,19,19,18,18,17,17,17,16,16,16,15,15,15,14,14 },
	{ 60,58,56,54,52,51,49,47,45,44,42,41,39,38,37,35,34,33,32,31,30,29,28,27,27,26,25,24,24,23,22,22,21,21,20,20,19,19,19,18,18,17,17,17,16,16,16,15,15,15 },
	{ 60,58,56,55,53,51,49,48,46,45,43,42,40,39,38,36,35,34,33,32,31,30,29,28,27,27,26,25,25,24,23,23,22,22,21,21,20,20,19,19,18,18,18,17,17,17,16,16,16,15 },
	{ 60,58,57,55,53,51,50,48,47,45,44,42,41,40,38,37,36,35,34,33,32,31,30,29,28,28,27,26,25,25,24,24,23,22,22,21,21,20,20,20,19,19,18,18,18,17,17,17,16,16 },
	{ 60,58,57,55,53,52,50,49,47,46,44,43,42,40,39,38,37,36,35,34,33,32,31,30,29,28,28,27,26,26,25,24,24,23,23,22,22,21,21,20,20,20,19,19,18,18,18,17,17,17 },
	{ 60,58,57,55,54,52,51,49,48,46,45,44,42,41,40,39,38,36,35,34,33,33,32,31,30,29,28,28,27,26,26,25,25,24,23,23,22,22,22,21,21,20,20,19,19,19,18,18,18,17 },
	{ 60,58,57,55,54,52,51,50,48,47,45,44,43,42,41,39,38,37,36,35,34,33,32,32,31,30,29,29,28,27,27,26,25,25,24,24,23,23,22,22,21,21,21,20,20,19,19,19,18,18 },
	{ 60,59,57,56,54,53,51,50,49,47,46,45,43,42,41,40,39,38,37,36,35,34,33,32,32,31,30,29,29,28,27,27,26,25,25,24,24,23,23,22,22,22,21,21,20,20,20,19,19,19 },
	{ 60,59,57,56,54,53,52,50,49,48,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,31,30,29,29,28,27,27,26,26,25,25,24,24,23,23,22,22,21,21,21,20,20,20,19 },
	{ 60,59,57,56,55,53,52,51,49,48,47,46,45,43,42,41,40,39,38,37,36,35,35,34,33,32,31,31,30,29,29,28,27,27,26,26,25,25,24,24,23,23,22,22,22,21,21,21,20,20 },
	{ 60,59,57,56,55,53,52,51,50,49,47,46,45,44,43,42,41,40,39,38,37,36,35,34,34,33,32,31,31,30,29,29,28,28,27,26,26,25,25,24,24,24,23,23,22,22,21,21,21,20 },
	{ 60,59,57,56,55,54,52,51,50,49,48,47,45,44,43,42,41,40,39,38,38,37,36,35,34,33,33,32,31,31,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,22,21,21 },
	{ 60,59,58,56,55,54,53,52,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,36,35,34,33,33,32,31,31,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,23,22,22,22 },
	{ 60,59,58,56,55,54,53,52,51,50,48,47,46,45,44,43,42,41,40,40,39,38,37,36,35,35,34,33,33,32,31,31,30,29,29,28,28,27,27,26,26,25,25,24,24,24,23,23,22,22 },
	{ 60,59,58,57,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,38,37,36,35,35,34,33,32,32,31,31,30,29,29,28,28,27,27,26,26,25,25,25,24,24,23,23,23 },
	{ 60,59,58,57,56,54,53,52,51,50,49,48,47,46,45,44,43,42,41,41,40,39,38,37,37,36,35,34,34,33,32,32,31,31,30,29,29,28,28,27,27,26,26,26,25,25,24,24,24,23 },
	{ 60,59,58,57,56,55,54,52,51,50,49,48,47,46,45,45,44,43,42,41,40,39,39,38,37,36,36,35,34,34,33,32,32,31,31,30,29,29,28,28,27,27,27,26,26,25,25,24,24,24 },
	{ 60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,41,40,39,38,38,37,36,35,35,34,33,33,32,32,31,31,30,29,29,28,28,28,27,27,26,26,25,25,25,24 },
	{ 60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,44,43,42,41,40,40,39,38,37,37,36,35,35,34,33,33,32,32,31,31,30,29,29,29,28,28,27,27,26,26,25,25,25 },
	{ 60,59,58,57,56,55,54,53,52,51,50,49,48,47,47,46,45,44,43,42,41,41,40,39,38,38,37,36,36,35,34,34,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,26,26,25 },
	{ 60,59,58,57,56,55,54,53,52,51,50,49,49,48,47,46,45,44,43,43,42,41,40,40,39,38,38,37,36,36,35,34,34,33,33,32,32,31,30,30,30,29,29,28,28,27,27,26,26,26 },
	{ 60,59,58,57,56,55,54,53,52,52,51,50,49,48,47,46,45,45,44,43,42,42,41,40,39,39,38,37,37,36,35,35,34,34,33,33,32,31,31,30,30,30,29,29,28,28,27,27,27,26 },
	{ 60,59,58,57,56,55,54,54,53,52,51,50,49,48,47,47,46,45,44,43,43,42,41,40,40,39,38,38,37,36,36,35,35,34,34,33,32,32,31,31,30,30,30,29,29,28,28,27,27,27 },
	{ 60,59,58,57,56,55,55,54,53,52,51,50,49,49,48,47,46,45,45,44,43,42,42,41,40,39,39,38,38,37,36,36,35,35,34,33,33,32,32,31,31,30,30,30,29,29,28,28,27,27 },
	{ 60,59,58,57,56,56,55,54,53,52,51,50,50,49,48,47,46,46,45,44,43,43,42,41,41,40,39,39,38,37,37,36,36,35,34,34,33,33,32,32,31,31,30,30,30,29,29,28,28,28 },
	{ 60,59,58,57,57,56,55,54,53,52,51,51,50,49,48,47,47,46,45,44,44,43,42,42,41,40,40,39,38,38,37,37,36,35,35,34,34,33,33,32,32,31,31,30,30,30,29,29,28,28 },
	{ 60,59,58,57,57,56,55,54,53,52,52,51,50,49,48,48,47,46,45,45,44,43,43,42,41,41,40,39,39,38,38,37,36,36,35,35,34,34,33,33,32,32,31,31,30,30,30,29,29,28 },
	{ 60,59,58,58,57,56,55,54,53,53,52,51,50,49,49,48,47,46,46,45,44,44,43,42,42,41,40,40,39,39,38,37,37,36,36,35,35,34,34,33,33,32,32,31,31,30,30,30,29,29 },
	{ 60,59,58,58,57,56,55,54,54,53,52,51,50,50,49,48,47,47,46,45,45,44,43,43,42,41,41,40,39,39,38,38,37,37,36,36,35,35,34,34,33,33,32,32,31,31,30,30,30,29 },
	{ 60,59,58,58,57,56,55,54,54,53,52,51,51,50,49,48,48,47,46,46,45,44,44,43,42,42,41,40,40,39,39,38,38,37,36,36,35,35,34,34,33,33,33,32,32,31,31,30,30,30 },
	{ 60,59,58,58,57,56,55,55,54,53,52,52,51,50,49,49,48,47,47,46,45,45,44,43,43,42,41,41,40,40,39,38,38,37,37,36,36,35,35,34,34,33,33,32,32,32,31,31,30,30 }
};

#define PI				3.14159265
#define _SEARCH_SIZE_				3
#define SEARCH_SIZE					5
#define PORE_RANGE_SEARCH_SIZE		SEARCH_SIZE
#define RANGE_SEARCH_SIZE			9
#define MERGR_RANGE_SEARCH_SIZE		15

/* 採用 Bounding Ridge 找尋 Ridge Ending 或是 採用 Valleys 分叉點去找尋 Ridge Ending ， 預設採用 Bounding Ridge Search 方法 */
#define __USE_RIDGE_SEARCH__	1

char strGlobalPath[256] = { 0 };
int g_nCol_X = -1, g_nRow_Y = -1;

short *pValleysDirection = NULL, *pRidgesDirection = NULL, *pPoreDirection = NULL;
int nValleysDirectionCnt = -1, nRidgesDirectionCnt = -1, nPoreDirectionCnt = -1;

int HorizontalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//水平 Smooth
	HorizontalSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);

	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		//memset(p2DImageTemp[j], 0, nWidth);
	}

	//水平 Differential
	HorizontalDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//Copy output image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

int VerticalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直 Smooth
	//VerticalSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);
	Smooth(p2DImageIn, p2DImageTemp, nWidth, nHeight, txHorizontal);

	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		//memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直 Differential
	VerticalDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//Copy output image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

/*Texture Horizontal Image Process*/
int TextureHorizontalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//水平 Smooth
	TextureHorizontalSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);

	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		//memset(p2DImageTemp[j], 0, nWidth);
	}

	//水平 Differential
	TextureHorizontalDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//Copy output image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

/*Texture Vertical Image Process*/
int TextureVerticalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直 Smooth
	//TextureVerticalSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);
	TextureReduceSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight, txHorizontal);
	TextureExpandSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight, txHorizontal);
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		//memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直 Differential
	//TextureVerticalDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);
	//TextureVerticalReduceDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);
	TextureVerticalExpandDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//Copy output image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

int TextureVerticalReduceImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直/水平 Reduce Smooth
	TextureReduceSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight, txHorizontal);
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		//memset(p2DImageTemp[j], 0, nWidth);
	}

	//垂直 Reduce Differential
	TextureVerticalReduceDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//Copy output image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

/*Texture Smoothing*/
void TextureHorizontalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 2));
		memset(pImage_In[j], 0, (nWidth + 2));
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 2));
		memset(pImage_Out[j], 0, (nWidth + 2));
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth + 2; i++) {
			if(i == 0)
				pImage_In[j][i] = image_in[j][i];
			else if(i == 1)
				pImage_In[j][i] = image_in[j][i - 1];
			else if(i == (nWidth + 2) - 2)
				pImage_In[j][i] = image_in[j][i - 1];
			else if(i == (nWidth + 2) - 1)
				pImage_In[j][i] = image_in[j][i - 2];
			else
				pImage_In[j][i] = image_in[j][i - 1];
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 1; i < (nWidth + 2) - 1; i++) {
			buf = (short)pImage_In[j][i - 1] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j][i + 1];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}
	//SaveTXT("ExternTextureHorizontalSmooth.txt", pImage_Out, nWidth + 2, nHeight);

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			image_out[j][i] = pImage_Out[j][i + 1];
		}
	}
	//SaveTXT("ETextureHorizontalSmooth.txt", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void TextureVerticalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 2));
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 2));
	for (j = 0; j < (nHeight + 2); j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < (nHeight + 2); j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0)
				pImage_In[j][i] = image_in[j][i];
			else if (j == 1)
				pImage_In[j][i] = image_in[j - 1][i];
			else if (j == (nHeight + 2) - 2)
				pImage_In[j][i] = image_in[j - 1][i];
			else if (j == (nHeight + 2) - 1)
				pImage_In[j][i] = image_in[j - 2][i];
			else
				pImage_In[j][i] = image_in[j - 1][i];
		}
	}

	for (j = 1; j < (nHeight + 2) - 1; j++) {
		for (i = 0; i < nWidth; i++) {
			buf = (short)pImage_In[j - 1][i] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j + 1][i];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}
	//SaveTXT("ExternTextureVerticalSmooth.txt", pImage_Out, nWidth, (nHeight + 2));

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			image_out[j][i] = pImage_Out[j + 1][i];
		}
	}
	//SaveTXT("ETextureVerticalSmooth.txt", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < (nHeight + 2); j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < (nHeight + 2); j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void TextureExpandSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;

	if (nHVType == txHorizontal) {
		pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		for (j = 0; j < nHeight; j++) {
			pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 2));
			memset(pImage_In[j], 0, (nWidth + 2));
			pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 2));
			memset(pImage_Out[j], 0, (nWidth + 2));
		}

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth + 2; i++) {
				if (i == 0)
					pImage_In[j][i] = image_in[j][i];
				else if (i == 1)
					pImage_In[j][i] = image_in[j][i - 1];
				else if (i == (nWidth + 2) - 2)
					pImage_In[j][i] = image_in[j][i - 1];
				else if (i == (nWidth + 2) - 1)
					pImage_In[j][i] = image_in[j][i - 2];
				else
					pImage_In[j][i] = image_in[j][i - 1];
			}
		}

		for (j = 0; j < nHeight; j++) {
			for (i = 1; i < (nWidth + 2) - 1; i++) {
				buf = (short)pImage_In[j][i - 1] +
					(short)pImage_In[j][i] + (short)pImage_In[j][i] +
					(short)pImage_In[j][i + 1];
				pImage_Out[j][i] = (buf + 2) / 4;
			}
		}
		//SaveTXT("ExternTextureHorizontalSmooth.txt", pImage_Out, nWidth + 2, nHeight);

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				image_out[j][i] = pImage_Out[j][i + 1];
			}
		}
		//SaveTXT("ETextureHorizontalSmooth.txt", image_out, nWidth, nHeight);

		if (pImage_Out != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_Out[j]);
			}
			free(pImage_Out);
			pImage_Out = NULL;

		}
		if (pImage_In != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_In[j]);
			}
			free(pImage_In);
			pImage_In = NULL;
		}
	}
	else {
		pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 2));
		pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 2));
		for (j = 0; j < (nHeight + 2); j++) {
			pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_In[j], 0, nWidth);
			pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_Out[j], 0, nWidth);
		}

		for (j = 0; j < (nHeight + 2); j++) {
			for (i = 0; i < nWidth; i++) {
				if (j == 0)
					pImage_In[j][i] = image_in[j][i];
				else if (j == 1)
					pImage_In[j][i] = image_in[j - 1][i];
				else if (j == (nHeight + 2) - 2)
					pImage_In[j][i] = image_in[j - 1][i];
				else if (j == (nHeight + 2) - 1)
					pImage_In[j][i] = image_in[j - 2][i];
				else
					pImage_In[j][i] = image_in[j - 1][i];
			}
		}

		for (j = 1; j < (nHeight + 2) - 1; j++) {
			for (i = 0; i < nWidth; i++) {
				buf = (short)pImage_In[j - 1][i] +
					(short)pImage_In[j][i] + (short)pImage_In[j][i] +
					(short)pImage_In[j + 1][i];
				pImage_Out[j][i] = (buf + 2) / 4;
			}
		}
		//SaveTXT("ExternTextureVerticalSmooth.txt", pImage_Out, nWidth, (nHeight + 2));

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				image_out[j][i] = pImage_Out[j + 1][i];
			}
		}
		//SaveTXT("ETextureVerticalSmooth.txt", image_out, nWidth, nHeight);

		if (pImage_Out != 0) {
			for (j = 0; j < (nHeight + 2); j++) {
				free(pImage_Out[j]);
			}
			free(pImage_Out);
			pImage_Out = NULL;

		}
		if (pImage_In != 0) {
			for (j = 0; j < (nHeight + 2); j++) {
				free(pImage_In[j]);
			}
			free(pImage_In);
			pImage_In = NULL;
		}
	}
}

void TextureReduceSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;

	if (nHVType == txHorizontal) {
		pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		for (j = 0; j < nHeight; j++) {
			pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_In[j], 0, nWidth);
			pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_Out[j], 0, nWidth);
		}

		for (j = 0; j < nHeight; j++)
			memcpy(pImage_In[j], image_in[j], nWidth);

		for (j = 0; j < nHeight; j++) {
			for (i = 1; i < nWidth - 1; i++) {
				buf = (short)pImage_In[j][i - 1] +
					  (short)pImage_In[j][i] + (short)pImage_In[j][i] +
					  (short)pImage_In[j][i + 1];
				pImage_Out[j][i] = (buf + 2) / 4;
			}
		}

		for (j = 0; j < nHeight; j++)
			memcpy(image_out[j], pImage_Out[j], nWidth);
		//SaveTXT("TextureHorizontalReduceSmooth.txt", image_out, nWidth, nHeight);
		SaveTXT("TextureHorizontalReduceSmooth.csv", image_out, nWidth, nHeight);

		if (pImage_Out != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_Out[j]);
			}
			free(pImage_Out);
			pImage_Out = NULL;

		}
		if (pImage_In != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_In[j]);
			}
			free(pImage_In);
			pImage_In = NULL;
		}
	}
	else {
		pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
		for (j = 0; j < nHeight; j++) {
			pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_In[j], 0, nWidth);
			pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
			memset(pImage_Out[j], 0, nWidth);
		}

		for (j = 0; j < nHeight; j++)
			memcpy(pImage_In[j], image_in[j], nWidth);

		for (j = 1; j < nHeight - 1; j++) {
			for (i = 0; i < nWidth; i++) {
				buf = (short)pImage_In[j - 1][i] +
					  (short)pImage_In[j][i] + (short)pImage_In[j][i] +
					  (short)pImage_In[j + 1][i];
				pImage_Out[j][i] = (buf + 2) / 4;
			}
		}

		for (j = 0; j < nHeight; j++)
			memcpy(image_out[j], pImage_Out[j], nWidth);
		//SaveTXT("TextureVerticalReduceSmooth.txt", image_out, nWidth, nHeight);
		SaveTXT("TextureVerticalReduceSmooth.csv", image_out, nWidth, nHeight);

		if (pImage_Out != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_Out[j]);
			}
			free(pImage_Out);
			pImage_Out = NULL;

		}
		if (pImage_In != 0) {
			for (j = 0; j < nHeight; j++) {
				free(pImage_In[j]);
			}
			free(pImage_In);
			pImage_In = NULL;
		}
	}
}

void Smooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;

	if (nHVType == txHorizontal) {
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
					//image_out[j][i] = 0/*image_in[j][i]*/;
				}
			}
		}
	}
	else {
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
}

/*Texture Differentia*/
void TextureHorizontalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 4));
		memset(pImage_In[j], 0, (nWidth + 4));
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + 4));
		memset(pImage_Out[j], 0, (nWidth + 4));
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < (nWidth + 4); i++) {
			if (i == 0)
				pImage_In[j][i] = image_in[j][i];
			else if (i == 1)
				pImage_In[j][i] = image_in[j][i - 1];
			else if (i == 2)
				pImage_In[j][i] = image_in[j][i - 2];
			else if (i == (nWidth + 4) - 3)
				pImage_In[j][i] = image_in[j][i - 2];
			else if (i == (nWidth + 4) - 2)
				pImage_In[j][i] = image_in[j][i - 3];
			else if (i == (nWidth + 4) - 1)
				pImage_In[j][i] = image_in[j][i - 4];
			else
				pImage_In[j][i] = image_in[j][i - 2];
		}
	}
	//SaveTXT("ExternTextureHorizontalDifferential.txt", pImage_In, nWidth + 4, nHeight);

	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < (nWidth + 4) - 2; i++) {
			buf = (short)pImage_In[j][i - 1] * (-1) +
				(short)pImage_In[j][i] * (0) +
				(short)pImage_In[j][i + 1] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}
	//SaveTXT("E4_ExternTextureHorizontalDifferential.txt", pImage_Out, nWidth + 4, nHeight);

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			image_out[j][i] = pImage_Out[j][i + 2];
		}
	}

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void TextureVerticalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 4));
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + 4));
	for (j = 0; j < (nHeight + 4); j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < (nHeight + 4); j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0)
				pImage_In[j][i] = image_in[j][i];
			else if (j == 1)
				pImage_In[j][i] = image_in[j - 1][i];
			else if (j == 2)
				pImage_In[j][i] = image_in[j - 2][i];
			else if (j == (nHeight + 4) - 3)
				pImage_In[j][i] = image_in[j - 2][i];
			else if (j == (nHeight + 4) - 2)
				pImage_In[j][i] = image_in[j - 3][i];
			else if (j == (nHeight + 4) - 1)
				pImage_In[j][i] = image_in[j - 4][i];
			else
				pImage_In[j][i] = image_in[j - 2][i];
		}
	}
	//SaveTXT("ExternTextureVerticalDifferential.txt", pImage_In, nWidth, (nHeight + 4));

	for (j = 1; j < (nHeight + 4) - 1; j++) {
		for (i = 0; i < nWidth ; i++) {
			buf = (short)pImage_In[j - 1][i] * (-1) +
				  (short)pImage_In[j][i] * (0) +
				  (short)pImage_In[j + 1][i] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}
	//SaveTXT("E4_ExternTextureVerticalDifferential.txt", pImage_Out, nWidth, (nHeight + 4));

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			image_out[j][i] = pImage_Out[j + 2][i];
		}
	}

	if (pImage_Out != 0) {
		for (j = 0; j < (nHeight + 4); j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < (nHeight + 4); j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void TextureVerticalExpandDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	const char cExpandLine = 2;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + cExpandLine));
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + cExpandLine));
	for (j = 0; j < (nHeight + cExpandLine); j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < (nHeight + cExpandLine); j++) {	//0 ~ 161
		for (i = 0; i < nWidth; i++) {
			if (j == 0)     //第0條
				pImage_In[j][i] = image_in[j][i];
			else if (j == 1)//第1條
				pImage_In[j][i] = image_in[j - 1][i];
			else if (j == (nHeight + cExpandLine) - 2)//第160條
				pImage_In[j][i] = image_in[j - 1][i];
			else if (j == (nHeight + cExpandLine) - 1)//第161條
				pImage_In[j][i] = image_in[j - 2][i];
			else
				pImage_In[j][i] = image_in[j - 1][i];
		}
	}
	//SaveTXT("ExternTextureVerticalDifferential.txt", pImage_In, nWidth, (nHeight + cExpandLine));

	for (j = 1; j < (nHeight + cExpandLine) - 1; j++) {
		for (i = 0; i < nWidth; i++) {
			buf = (short)pImage_In[j - 1][i] * (-1) +
				  (short)pImage_In[j][i] * (0) +
				  (short)pImage_In[j + 1][i] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}
	SaveTXT("E2_ExternTextureVerticalDifferential.txt", pImage_Out, nWidth, (nHeight + cExpandLine));

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			image_out[j][i] = pImage_Out[j + 1][i];
		}
	}

	if (pImage_Out != 0) {
		for (j = 0; j < (nHeight + cExpandLine); j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < (nHeight + cExpandLine); j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void TextureVerticalReduceDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	//將Reduce Smooth的左右邊緣補值
	//1.先直接複製數值
	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	//2.左右邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				pImage_In[j][i] = image_in[j][i + 1];//第1行拿第2行的數值去補
			if(i == nWidth - 1)//第79行
				pImage_In[j][i] = image_in[j][i - 1];//第79行拿第78行的數值去補
		}
	}

	//進行差分運算
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 0; i < nWidth; i++) {
			buf = (short)pImage_In[j - 1][i] * (-1) +
				  (short)pImage_In[j][i] * (0) +
				  (short)pImage_In[j + 1][i] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}
	//SaveTXT("TextureVerticalReduceDifferential.txt", pImage_Out, nWidth, nHeight);
	SaveTXT("TextureVerticalReduceDifferential.csv", pImage_Out, nWidth, nHeight);

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}
	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

/*Texture Labeling*/
void TextureHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	//char buffer[1000] = { 0 };
	unsigned char R1 = 0, V1 = 255, E0 = 170, TH = 9, flag = 0, last_flag = 0;
	int i, j, sA, sB, A, B;

	for (j = 0; j < nHeight; j++) {
		A = /*2*/0;
		B = /*3*/1;
		flag = 0;
		last_flag = 0;

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d (j = %3d)\r\n", __FUNCTION__, __LINE__, j);
		//LOGD(buffer);

		do {

			sA = pImage_in[j][A];
			sB = pImage_in[j][B];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
			//LOGD(buffer);

			//DB > 0
			if (sB > 0)
				flag = 0;

			//DB < 0
			if (sB < 0)
				flag = 1;

			/*Case1.(-, +)*/
			if (sA < 0 && sB > 0) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (last_flag = %d / flag = %d)\r\n", __FUNCTION__, __LINE__, last_flag, flag);
				//LOGD(buffer);

				for (i = A + 1; i < B; i++) {
					if (abs(pImage_in[j][i]) <= TH) {
						pImage_out[j][i] = R1;
					}
				}

				//(abs(sA) < TH)
				if (abs(sA) <= TH) {
					pImage_out[j][A] = R1;
					if (last_flag == 1 && flag == 0)
						pImage_out[j][A] = V1;
				}
				//(abs(sA) >= TH)
				/*else {
					if (abs(sA) == min(abs(sA), abs(sB))) {
						pImage_out[j][A] = R1;
						if (last_flag == 1 && flag == 0) {
							pImage_out[j][A] = V1;
						}
					}
				}*/

				//(abs(sB) < TH)
				if (abs(sB) <= TH) {
					pImage_out[j][B] = R1;
				}
				//(abs(sB) >= TH)
				/*else {
					if (abs(sB) == min(abs(sA), abs(sB)))
						pImage_out[j][B] = R1;
				}*/

				if (abs(sA) <= TH && abs(sB) <= TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[j][A] = R1;
						if (last_flag == 1 && flag == 0)
							pImage_out[j][A] = V1;
					}
					else {
						pImage_out[j][B] = R1;
					}
				}
				
				if (abs(sA) > TH && abs(sB) > TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[j][A] = R1;
						if (last_flag == 1 && flag == 0)
							pImage_out[j][A] = V1;
					}
					else {
						pImage_out[j][B] = R1;
					}
				}

				B++;
				A = B - 1;

				//保留這次flag狀態，提供給下次參考用
				last_flag = flag;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}

			/*Case2.(+, -)*/
			else if (sA > 0 && sB < 0) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (last_flag = %d / flag = %d)\r\n", __FUNCTION__, __LINE__, last_flag, flag);
				//LOGD(buffer);

				for (i = A + 1; i < B; i++) {
					if (abs(pImage_in[j][i]) <= TH) {
						pImage_out[j][i] = V1;
					}
				}

				//(abs(sA) < TH)
				if (abs(sA) <= TH) {
					pImage_out[j][A] = V1;
					if (last_flag == 0 && flag == 1) {
						pImage_out[j][A] = V1;
					}
				}
				//(abs(sA) >= TH)
				/*else {
					if (abs(sA) == min(abs(sA), abs(sB))) {
						pImage_out[j][A] = V1;
						if (last_flag == 0 && flag == 1) {
							pImage_out[j][A] = V1;
						}
					}
				}*/

				//(abs(sB) < TH)
				if (abs(sB) <= TH) {
					pImage_out[j][B] = V1;
				}
				//(abs(sB) >= TH)
				/*else {
					if (abs(sB) == min(abs(sA), abs(sB)))
						pImage_out[j][B] = V1;
				}*/

				if (abs(sA) <= TH && abs(sB) <= TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[j][A] = V1;
					}
					else {
						pImage_out[j][B] = V1;
					}
				}

				if (abs(sA) > TH && abs(sB) > TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[j][A] = V1;
					}
					else {
						pImage_out[j][B] = V1;
					}
				}

				B++;
				A = B - 1;

				//保留這次flag狀態，提供給下次參考用
				last_flag = flag;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}

			else {
				/*(-, -), (+, +), (0, -), (0, +)*/
				if (sB != 0) {
					B++;
					A = B - 1;
				}
				/*(-, 0). (+, 0), (0, 0)*/
				else {
					B++;
				}
				//清除flag狀態
				last_flag = 0;
				flag = 0;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}
		} while (A < nWidth - 1/*3*/ && B < nWidth /*- 2*/);

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}//for (j = 0; j < nHeight; j++)
}

void TextureVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	//char buffer[1000] = { 0 };
	unsigned char R1 = 0, V1 = 255, E0 = 170, TH = 9, flag = 0, last_flag = 0;
	short i, j, sA, sB, A, B;

	for (i = 0; i < nWidth; i++) {
		A = /*2*/0;
		B = /*3*/1;
		flag = 0;
		last_flag = 0;

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d (i = %3d)\r\n", __FUNCTION__, __LINE__, i);
		//LOGD(buffer);

		do {
			sA = pImage_in[A][i];
			sB = pImage_in[B][i];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
			//LOGD(buffer);

			//DB > 0
			if (sB > 0)
				flag = 0;

			//DB < 0
			if (sB < 0)
				flag = 1;

			/*(-, +)*/
			if (sA < 0 && sB > 0) {

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (last_flag = %d / flag = %d)\r\n", __FUNCTION__, __LINE__, last_flag, flag);
				//LOGD(buffer);

				for (j = A + 1; j < B; j++) {
					if (abs(pImage_in[j][i]) <= TH) {
						pImage_out[j][i] = R1;
					}
				}

				//(abs(sA) < TH)
				if (abs(sA) <= TH) {
					pImage_out[A][i] = R1;
					if (last_flag == 1 && flag == 0)
						pImage_out[A][i] = V1;
				}
				//(abs(sA) >= TH)
				/*else {
					if (abs(sA) == min(abs(sA), abs(sB))) {
						pImage_out[A][i] = R1;
						if (last_flag == 1 && flag == 0) {
							pImage_out[A][i] = V1;
						}
					}
				}*/

				//(abs(sB) < TH)
				if (abs(sB) <= TH) {
					pImage_out[B][i] = R1;
				}
				//(abs(sB) >= TH)
				/*else {
					if (abs(sB) == min(abs(sA), abs(sB)))
						pImage_out[B][i] = R1;
				}*/

				if (abs(sA) <= TH && abs(sB) <= TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[A][i] = R1;
						if (last_flag == 1 && flag == 0)
							pImage_out[A][i] = V1;
					}
					else {
						pImage_out[B][i] = R1;
					}
				}

				if (abs(sA) > TH && abs(sB) > TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[A][i] = R1;
						if (last_flag == 1 && flag == 0)
							pImage_out[A][i] = V1;
					}
					else {
						pImage_out[B][i] = R1;
					}

				}
				B++;
				A = B - 1;

				//保留這次flag狀態，提供給下次參考用
				last_flag = flag;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}/*(-, +)*/

			 /*(+, -)*/
			else if (sA > 0 && sB < 0) {

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (last_flag = %d / flag = %d)\r\n", __FUNCTION__, __LINE__, last_flag, flag);
				//LOGD(buffer);

				for (j = A + 1; j < B; j++) {
					if (abs(pImage_in[j][i]) <= TH) {
						pImage_out[j][i] = V1;
					}
				}

				//(abs(sA) < TH)
				if (abs(sA) <= TH) {
					pImage_out[A][i] = V1;
					if (last_flag == 0 && flag == 1) {
						pImage_out[A][i] = V1;
					}
				}
				//(abs(sA) >= TH)
				/*else {
					if (abs(sA) == min(abs(sA), abs(sB))) {
						pImage_out[A][i] = V1;
						if (last_flag == 0 && flag == 1) {
							pImage_out[A][i] = V1;
						}
					}
				}*/

				//(abs(sB) < TH)
				if (abs(sB) <= TH) {
					pImage_out[B][i] = V1;
				}
				//(abs(sB) >= TH)
				/*else {
					if (abs(sB) == min(abs(sA), abs(sB)))
						pImage_out[B][i] = V1;
				}*/

				if (abs(sA) <= TH && abs(sB) <= TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[A][i] = V1;
					}
					else {
						pImage_out[B][i] = V1;
					}
				}

				if (abs(sA) > TH && abs(sB) > TH) {
					if (abs(sA) < abs(sB)) {
						pImage_out[A][i] = V1;
					}
					else {
						pImage_out[B][i] = V1;
					}
				}

				B++;
				A = B - 1;

				//保留這次flag狀態，提供給下次參考用
				last_flag = flag;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}/*(+, -)*/

			else {
				/*(-, -), (+, +), (0, -), (0, +)*/
				if (sB != 0) {
					B++;
					A = B - 1;
				}
				/*(-, 0). (+, 0), (0, 0)*/
				else {
					B++;
				}
				//清除flag狀態
				last_flag = 0;
				flag = 0;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (sA = %3d, sB = %3d) / Next (A = %3d, B = %3d)\r\n", __FUNCTION__, __LINE__, sA, sB, A, B);
				//LOGD(buffer);
			}
		} while (A < nHeight - 1/*3*/ && B < nHeight /*- 2*/);

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
		//LOGD(buffer);
	}//for (i = 0; i < nWidth; i++)
}

/*Texture New Labeling*/
void TextureNewVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;

#if 1
	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];	/*A*/
			sB = pImage_in[j - 1][i];	/*B*/
			sC = pImage_in[j][i];		/*C*/
			sD = pImage_in[j + 1][i];	/*D*/
			sE = pImage_in[j + 2][i];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pImage_out[j + 1][i] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pImage_out[j][i] = R0;
				}
				else {
					pImage_out[j + 1][i] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = R0;
						}
						else {
							pImage_out[j + 1][i] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = V0;
						}
						else {
							pImage_out[j + 1][i] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pImage_out[j + 1][i] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pImage_out[j][i] = V0;
				}
				else {
					pImage_out[j + 1][i] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < nHeight - 2; j++)
	} //for (i = 0; i < nWidth; i++)
#else
	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];/*A*/
			sB = pImage_in[j - 1][i];/*B*/
			sC = pImage_in[j][i];	 /*C*/
			sD = pImage_in[j + 1][i];/*D*/
			sE = pImage_in[j + 2][i];/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (abs(sC) > abs(sD))
					pImage_out[j + 1][i] = R0;
				else if (abs(sB) < abs(sE))
					pImage_out[j][i] = R0;
				else
					pImage_out[j + 1][i] = R0;
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
			}
			else if (sD == 0) {
				if (sB < 0 && sE > 0) {
					if (abs(sB) < abs(sE))
						pImage_out[j][i] = R0;
					else
						pImage_out[j + 1][i] = R0;
				}
				else if (sB > 0 && sE < 0) {
					if (abs(sB) < abs(sE))
						pImage_out[j][i] = V0;
					else
						pImage_out[j + 1][i] = V0;
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
				else if (abs(sC) > abs(sD))
					pImage_out[j + 1][i] = V0;
				else if (abs(sB) < abs(sE))
					pImage_out[j][i] = V0;
				else
					pImage_out[j + 1][i] = V0;
			}
		}
		sA = 255;
		sB = 255;
		sC = 255;
		sD = 255;
		sE = 255;
	}
#endif
}

void TextureVerticalReduceLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < nHeight - 2; j++) {
			sA = pImage_in[j - 2][i];	/*A*/
			sB = pImage_in[j - 1][i];	/*B*/
			sC = pImage_in[j][i];		/*C*/
			sD = pImage_in[j + 1][i];	/*D*/
			sE = pImage_in[j + 2][i];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pImage_out[j + 1][i] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pImage_out[j][i] = R0;
				}
				else {
					pImage_out[j + 1][i] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pImage_out[j][i] == V0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = R0;
						}
						else {
							pImage_out[j + 1][i] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pImage_out[j][i] = V0;
						}
						else {
							pImage_out[j + 1][i] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pImage_out[j][i] == R0)
						pImage_out[j][i] = DL;
					else
						pImage_out[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pImage_out[j + 1][i] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pImage_out[j][i] = V0;
				}
				else {
					pImage_out[j + 1][i] = V0;
				}
			}
			else if (sC > 0 && sD > 0) {
				if (sB < 0 && sC > 0) {
					if (abs(sB) > abs(sC))
						pImage_out[j][i] = R0;
					else
						pImage_out[j - 1][i] = R0;

					//if(pImage_out[j - 1][i] != R0)
					//	pImage_out[j][i] = R0;
				}
			}
			else if (sC < 0 && sD < 0) {
				if (sB > 0 && sC < 0) {
					if (abs(sB) > abs(sC))
						pImage_out[j][i] = V0;
					else
						pImage_out[j - 1][i] = V0;

					//if(pImage_out[j - 1][i] != V0)
					//	pImage_out[j][i] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < nHeight - 2; j++)
	} //for (i = 0; i < nWidth; i++)

	//將X軸的第1條與最後1條補成BG定義的數值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)
				pImage_out[j][i] = BG;
			if(i == nWidth - 1)
				pImage_out[j][i] = BG;
		}
	}
}

void TextureVerticalExpandTopAndReduceLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j, nExpandTopLine = 2;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	short **pIn_Image = NULL, sbuf;
	unsigned char **pOut_Image = NULL;

	pIn_Image = (short **)malloc(sizeof(short *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pIn_Image[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			pIn_Image[j][i] = 0;
		}
	}

	pOut_Image = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pOut_Image[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pOut_Image[j], 0, nWidth);
	}

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {	//0 ~ 82
		for (i = 0; i < nWidth; i++) {
			if (j == 0)						//第0條
				pIn_Image[j][i] = pImage_in[j][i];
			else if (j == 1)				//第1條
				pIn_Image[j][i] = pImage_in[j - 1][i];
			else
				pIn_Image[j][i] = pImage_in[j - 2][i];
		}
	}

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = pIn_Image[j][i];
			pOut_Image[j][i] = sbuf + 128;
		}
	}
	//SaveTXT("TextureVerticalExpandTopAndReduceLabeling.csv", pOut_Image, nWidth, (nHeight + nExpandTopLine));

	for (j = 0; j < (nHeight + nExpandTopLine); j++)
		memset(pOut_Image[j], BG, nWidth);

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++) {
			sA = pIn_Image[j - 2][i];	/*A*/
			sB = pIn_Image[j - 1][i];	/*B*/
			sC = pIn_Image[j][i];		/*C*/
			sD = pIn_Image[j + 1][i];	/*D*/
			sE = pIn_Image[j + 2][i];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j + 1][i] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = R0;
				}
				else {
					pOut_Image[j + 1][i] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = R0;
						}
						else {
							pOut_Image[j + 1][i] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = V0;
						}
						else {
							pOut_Image[j + 1][i] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pOut_Image[j + 1][i] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = V0;
				}
				else {
					pOut_Image[j + 1][i] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++)
	} //for (i = 0; i < nWidth; i++)

	for (i = 0; i < nWidth; i++) {
		for (j = 0; j < nHeight; j++) {
			//抓取 (nWidth * nHeight) 的數據 (80 * 80)
			pImage_out[j][i] = pOut_Image[j + nExpandTopLine][i];

			//將X軸的第1條與最後1條補成BG定義的數值
			if (i == 0)
				pImage_out[j][i] = BG;
			if (i == nWidth - 1)
				pImage_out[j][i] = BG;
		}
	}

	if (pOut_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pOut_Image[j]);
		}
		free(pOut_Image);
		pOut_Image = NULL;
	}

	if (pIn_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pIn_Image[j]);
		}
		free(pIn_Image);
		pIn_Image = NULL;
	}
}

/*Texture Block Labeling*/
void TextureHorizontalBlockLabeling(short **pImage_in, unsigned char **pImage_out, struct BlockInfo *pBlockInfoData, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL || pBlockInfoData == NULL)
		return;

	char buffer[1000] = { 0 };
	unsigned char CntR1, CntV1, CntE0, CntD1;
	unsigned char R1 = 0, V1 = 255, E0 = 170, D1 = 85, TH = 9;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	short i, j, ii, jj, block_id, item_id;
	short **pBlock = NULL;
	unsigned char BlockEdgeStatus[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE][EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { -1 };

	sprintf(buffer, "[FPTexture.c] %s() #Line %d nWidth = %d / nHeight = %d\r\n", __FUNCTION__, __LINE__, nWidth, nHeight);
	LOGD(buffer);

	pBlock = (short **)malloc(sizeof(short *) * OneBlockSize);
	for (j = 0; j < OneBlockSize; j++) {
		pBlock[j] = (short *)malloc(sizeof(short) * OneBlockSize);
		memset(pBlock[j], 0, sizeof(short) * OneBlockSize);
	}

	block_id = 0;
	item_id = 0;
	CntR1 = 0;
	CntV1 = 0;
	CntE0 = 0;
	CntD1 = 0;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i % OneBlockSize == 0 && j % OneBlockSize == 0) {
				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%3d, %3d) / Block ID = %3d\r\n", __FUNCTION__, __LINE__, i, j, block_id);
				LOGD(buffer);
				for (jj = 0; jj < OneBlockSize; jj++) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [jj = %3d, ii = %3d] / pImage_in[%3d][%3d] = %3d / pImage_out[%3d][%3d] = %3d, Item_ID = %3d\r\n", __FUNCTION__, __LINE__, jj, ii, j + jj, i + ii, pImage_in[j + jj][i + ii], j + jj, i + ii, pImage_out[j + jj][i + ii], item_id++);
						//LOGD(buffer);

						if (pImage_out[j + jj][i + ii] == R1) {
							CntR1++;
							pBlock[jj][ii] = R1;
						}
						else if (pImage_out[j + jj][i + ii] == V1) {
							CntV1++;
							pBlock[jj][ii] = V1;
						}
						else if (pImage_out[j + jj][i + ii] == E0) {
							if (abs(pImage_in[j + jj][i + ii]) < TH) {
								CntD1++;
								pBlock[jj][ii] = D1;
								pImage_out[j + jj][i + ii] = D1;
							}
							else {
								CntE0++;
								pBlock[jj][ii] = E0;
							}
						}
					}
				}

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d  CntR1 = %d / CntV1 = %d / CntD1 = %d, CntE0 = %d / %d\r\n", __FUNCTION__, __LINE__, CntR1, CntV1, CntD1, CntE0, ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) - CntR1 - CntV1 - CntD1));
				//LOGD(buffer);

				if (((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) - CntR1 - CntV1 - CntD1) <= ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) / 3)) {
					pBlockInfoData->EdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_BAD;
					BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_BAD;
				}
				else {
					pBlockInfoData->EdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_GOOD;
					BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_GOOD;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d BlockEdgeStatus[%3d][%3d] = %3d\r\n", __FUNCTION__, __LINE__, j / EMC_BLOCK_SIZE, i / EMC_BLOCK_SIZE, BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE]);
				//LOGD(buffer);

				//Clear Parameters
				for (int n = 0; n < OneBlockSize; n++) {
					memset(pBlock[n], 0, sizeof(short) * OneBlockSize);
				}

				block_id++;
				item_id = 0;
				CntR1 = 0;
				CntV1 = 0;
				CntE0 = 0;
				CntD1 = 0;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
				//LOGD(buffer);
			}
		}
	}

	if (pBlock != NULL) {
		for (j = 0; j < OneBlockSize; j++) {
			free(pBlock[j]);
		}
		free(pBlock);
		pBlock = NULL;
	}
}

void TextureVerticalBlockLabeling(short **pImage_in, unsigned char **pImage_out, struct BlockInfo *pBlockInfoData, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL || pBlockInfoData == NULL)
		return;

	//char buffer[1000] = { 0 };
	char pStrPath[256] = { 0 };
	unsigned char CntR1, CntV1, CntE0, CntD1;
	unsigned char R1 = 0, V1 = 255, E0 = 170, D1 = 85, TH = 9;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	short i, j, ii, jj, block_id, item_id;
	short **pBlock = NULL;
	unsigned char BlockEdgeStatus[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE][EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { -1 };

	pBlock = (short **)malloc(sizeof(short *) * OneBlockSize);
	for (j = 0; j < OneBlockSize; j++) {
		pBlock[j] = (short *)malloc(sizeof(short) * OneBlockSize);
		memset(pBlock[j], 0, sizeof(short) * OneBlockSize);
	}

	block_id = 0;
	item_id = 0;
	CntR1 = 0;
	CntV1 = 0;
	CntE0 = 0;
	CntD1 = 0;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i % OneBlockSize == 0 && j % OneBlockSize == 0) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%3d, %3d) / Block ID = %3d\r\n", __FUNCTION__, __LINE__, i, j, block_id);
				//LOGD(buffer);
				for (jj = 0; jj < OneBlockSize; jj++) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [jj = %3d, ii = %3d] / pImage_in[%3d][%3d] = %3d / pImage_out[%3d][%3d] = %3d, Item_ID = %3d\r\n", __FUNCTION__, __LINE__, jj, ii, j + jj, i + ii, pImage_in[j + jj][i + ii], j + jj, i + ii, pImage_out[j + jj][i + ii], item_id++);
						//LOGD(buffer);

						if (pImage_out[j + jj][i + ii] == R1) {
							CntR1++;
							pBlock[jj][ii] = R1;
						}
						else if (pImage_out[j + jj][i + ii] == V1) {
							CntV1++;
							pBlock[jj][ii] = V1;
						}
						else if (pImage_out[j + jj][i + ii] == E0) {
							if (abs(pImage_in[j + jj][i + ii]) < TH) {
								CntD1++;
								pBlock[jj][ii] = D1;
								pImage_out[j + jj][i + ii] = D1;
							}
							else {
								CntE0++;
								pBlock[jj][ii] = E0;
							}
						}
					}
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d  CntR1 = %d / CntV1 = %d / CntD1 = %d, CntE0 = %d / %d / ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) / 3) = %d\r\n", __FUNCTION__, __LINE__, CntR1, CntV1, CntD1, CntE0, ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) - CntR1 - CntV1 - CntD1), ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) / 3));
				//LOGD(buffer);

				if (((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) - CntR1 - CntV1 - CntD1) <= ((EMC_BLOCK_SIZE * EMC_BLOCK_SIZE) / 3)) {
					pBlockInfoData->EdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_BAD;
					BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_BAD;
				}
				else {
					pBlockInfoData->EdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_GOOD;
					BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE] = EMC_BLOCK_GOOD;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d BlockEdgeStatus[%3d][%3d] = %3d\r\n", __FUNCTION__, __LINE__, j / EMC_BLOCK_SIZE, i / EMC_BLOCK_SIZE, BlockEdgeStatus[j / EMC_BLOCK_SIZE][i / EMC_BLOCK_SIZE]);
				//LOGD(buffer);
				
				//Clear Parameters
				for (int n = 0; n < OneBlockSize; n++) {
					memset(pBlock[n], 0, sizeof(short) * OneBlockSize);
				}
				
				block_id++;
				item_id = 0;
				CntR1 = 0;
				CntV1 = 0;
				CntE0 = 0;
				CntD1 = 0;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
				//LOGD(buffer);
			}
		}
	}

	if (pBlock != NULL) {
		for (j = 0; j < OneBlockSize; j++) {
			free(pBlock[j]);
		}
		free(pBlock);
		pBlock = NULL;
	}
}

/*Texture Merge Block Labeling*/
void TextureMergeBlockLabeling(unsigned char **pHorizontalImage_in, unsigned char **pVerticalImage_in, unsigned char **pMergeImage_out, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, struct Texture *pTexture, int nWidth, int nHeight)
{
	if (pHorizontalImage_in == NULL || pVerticalImage_in == NULL || pMergeImage_out == NULL || pHorizontalBlockInfoData == NULL || pVerticalBlockInfoData == NULL || pTexture == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj;
	unsigned char R1 = 0, V1 = 255, E0 = 170, D1 = 85, TH = 9;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	/* (-1)代表整個Col都是Good Block */
	//signed char InValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) };//保存 Y 的 Bad Block 位置

	/*尋找分界線位置*/
	//for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
	//	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
	//		if (pVerticalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
	//			//sprintf(buffer, "[FPTexture.c] %s() #Line %d i = %2d / j = %2d / Index = %3d\r\n", __FUNCTION__, __LINE__, i, j, j * (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) + i);
	//			//LOGD(buffer);
	//			InValidRow[i] = j;//紀錄無效區域的Y位置
	//			break;
	//		}
	//	}
	//}

	//Start Merger
	int nRidgeCnt = 0, nValleyCnt = 0;
	for (i = 0; i < nWidth; i++) {
		for (j = 0; j < nHeight; j++) {
			if (i % OneBlockSize == 0 && j % OneBlockSize == 0) {
				if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d i = %3d / j = %3d / Index = %3d\r\n", __FUNCTION__, __LINE__, i / OneBlockSize, j / OneBlockSize, (j / OneBlockSize) * (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) + (i / OneBlockSize));
					//LOGD(buffer);
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);

					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							pMergeImage_out[j + jj][i + ii] = pHorizontalImage_in[j + jj][i + ii];

							//Search Column Block R1 & V1
							if (((j + jj) % OneBlockSize == 0) || (j + jj) == (nHeight - 1)) {
								if (pMergeImage_out[j + jj][i + ii] == R1) {
									nRidgeCnt++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d) / nRidgeCnt = %d\r\n", __FUNCTION__, __LINE__, i + ii, j + jj, nRidgeCnt);
									//LOGD(buffer);
								}
								if (pMergeImage_out[j + jj][i + ii] == V1) {
									nValleyCnt++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d) / nValleyCnt = %d\r\n", __FUNCTION__, __LINE__, i + ii, j + jj, nValleyCnt);
									//LOGD(buffer);
								}
							}
						}
					}
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d i = %3d / j = %3d / Index = %3d\r\n", __FUNCTION__, __LINE__, i / OneBlockSize, j / OneBlockSize, (j / OneBlockSize) * (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) + (i / OneBlockSize));
					//LOGD(buffer);
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);

					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							pMergeImage_out[j + jj][i + ii] = pVerticalImage_in[j + jj][i + ii];

							//Search Column Block R1 & V1
							if (((i + ii) % OneBlockSize == 0) || (i + ii) == (nWidth - 1)) {
								if (pMergeImage_out[j + jj][i + ii] == R1) {
									nRidgeCnt++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d) / nRidgeCnt = %d\r\n", __FUNCTION__, __LINE__, i + ii, j + jj, nRidgeCnt);
									//LOGD(buffer);
								}
								if (pMergeImage_out[j + jj][i + ii] == V1) {
									nValleyCnt++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d) / nValleyCnt = %d\r\n", __FUNCTION__, __LINE__, i + ii, j + jj, nValleyCnt);
									//LOGD(buffer);
								}
							}
						}
					}
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Ridge = %d / Valley = %d\r\n", __FUNCTION__, __LINE__, nRidgeCnt, nValleyCnt);
	//LOGD(buffer);

	//Save Column Block R1 & V1
	pTexture->pBoundRidge = (struct TexturePOINT *) malloc(sizeof(struct TexturePOINT) * nRidgeCnt);
	memset(pTexture->pBoundRidge, 0, sizeof(struct TexturePOINT) * nRidgeCnt);
	pTexture->nTotalBoundRidge = nRidgeCnt;
	pTexture->pBoundValley = (struct TexturePOINT *) malloc(sizeof(struct TexturePOINT) * nValleyCnt);
	memset(pTexture->pBoundValley, 0, sizeof(struct TexturePOINT) * nValleyCnt);
	pTexture->nTotalBoundValley = nValleyCnt;

	int nBoundRidgeIDx = 0, nBoundValleyIDx = 0;
	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth - 1) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						if (i != nWidth - 1) {
							for (ii = 0; ii < OneBlockSize; ii++) {
								if (i + ii > nWidth - 1)
									break;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i + ii, j);
								//LOGD(buffer);

								if (pMergeImage_out[j][i + ii] == R1) {
									pTexture->pBoundRidge[nBoundRidgeIDx].x = i + ii;
									pTexture->pBoundRidge[nBoundRidgeIDx].y = j;
									pTexture->pBoundRidge[nBoundRidgeIDx].type = txBound;
									pTexture->pBoundRidge[nBoundRidgeIDx].valid = false;
									nBoundRidgeIDx++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Ridge / (%d, %d) / nBoundRidgeIDx = %d\r\n", __FUNCTION__, __LINE__, i + ii, j, nBoundRidgeIDx);
									//LOGD(buffer);
								}
								if (pMergeImage_out[j][i + ii] == V1) {
									pTexture->pBoundValley[nBoundValleyIDx].x = i + ii;
									pTexture->pBoundValley[nBoundValleyIDx].y = j;
									pTexture->pBoundValley[nBoundValleyIDx].type = txBound;
									pTexture->pBoundValley[nBoundValleyIDx].valid = false;
									nBoundValleyIDx++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Valley / (%d, %d) / nBoundValleyIDx = %d\r\n", __FUNCTION__, __LINE__, i + ii, j, nBoundValleyIDx);
									//LOGD(buffer);
								}
							}
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						if (j != nHeight - 1) {
							for (jj = 0; jj < OneBlockSize; jj++) {
								if (j + jj > nHeight - 1)
									break;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j + jj);
								//LOGD(buffer);
								if (pMergeImage_out[j + jj][i] == R1) {
									pTexture->pBoundRidge[nBoundRidgeIDx].x = i;
									pTexture->pBoundRidge[nBoundRidgeIDx].y = j + jj;
									pTexture->pBoundRidge[nBoundRidgeIDx].type = txBound;
									pTexture->pBoundRidge[nBoundRidgeIDx].valid = false;
									nBoundRidgeIDx++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Ridge / (%d, %d) / nBoundRidgeIDx = %d\r\n", __FUNCTION__, __LINE__, i, j + jj, nBoundRidgeIDx);
									//LOGD(buffer);
								}
								if (pMergeImage_out[j + jj][i] == V1) {
									pTexture->pBoundValley[nBoundValleyIDx].x = i;
									pTexture->pBoundValley[nBoundValleyIDx].y = j + jj;
									pTexture->pBoundValley[nBoundValleyIDx].type = txBound;
									pTexture->pBoundValley[nBoundValleyIDx].valid = false;
									nBoundValleyIDx++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Valley / (%d, %d) / nBoundValleyIDx = %d\r\n", __FUNCTION__, __LINE__, i, j + jj, nBoundValleyIDx);
									//LOGD(buffer);
								}
							}
						}
					}
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d nBoundRidgeIDx = %d / nBoundValleyIDx = %d\r\n", __FUNCTION__, __LINE__, nBoundRidgeIDx, nBoundValleyIDx);
	//LOGD(buffer);
}

void CheckAvgBoundValidData(struct Texture *pTexture, int nWidth, int nHeight)
{
	if (pTexture == NULL)
		return;

	char buffer[1000] = { 0 };
	int n, m, x, y, i, j;
	int nRecordNRidge = 0, nCntRidge = 0, nStartNRidge, nIdxNRidge;
	int nRecordNValley = 0, nCntValley = 0, nStartNValley, nIdxNValley;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;

	for (i = 0; i < nWidth; i++) {
		if ((i % OneBlockSize == 0 || i == nWidth - 1)) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					LOGD(buffer);

					//Ridge
					for (n = nRecordNRidge; n < pTexture->nTotalBoundRidge; n++) {
						x = pTexture->pBoundRidge[n].x;
						y = pTexture->pBoundRidge[n].y;
						if (x == i && y < (j + OneBlockSize)) {
							nCntRidge++;
							sprintf(buffer, "[FPTexture.c] %s() #Line %d n = %d / nRecordNRidge = %d / (%d, %d) / nCntRidge = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, n, nRecordNRidge, i, j, nCntRidge, x, y);
							LOGD(buffer);
							nRecordNRidge = n + 1;//保存下一個Index
						}
					}

					if (nCntRidge != 0) {
						if (nCntRidge > 2) {
							if (nCntRidge % 2 == 0) {
								nStartNRidge = (nRecordNRidge - 1) - (nCntRidge - 1);
								nIdxNRidge = nStartNRidge + (nCntRidge / 2);
								sprintf(buffer, "[FPTexture.c] %s() #Line %d n = %d / (nRecordNRidge - 1) = %d / nCntRidge = %d / nStartNRidge = %d / nIdxNRidge = %d\r\n", __FUNCTION__, __LINE__, n, (nRecordNRidge - 1), nCntRidge, nStartNRidge, nIdxNRidge);
								LOGD(buffer);
								pTexture->pBoundRidge[nIdxNRidge].valid = true;
							}
							else {
								nStartNRidge = (nRecordNRidge - 1) - (nCntRidge - 1);
								nIdxNRidge = nStartNRidge + (int)(nCntRidge / 2);
								sprintf(buffer, "[FPTexture.c] %s() #Line %d n = %d / (nRecordNRidge - 1) = %d / nCntRidge = %d / nStartNRidge = %d / nIdxNRidge = %d\r\n", __FUNCTION__, __LINE__, n, (nRecordNRidge - 1), nCntRidge, nStartNRidge, nIdxNRidge);
								LOGD(buffer);
								pTexture->pBoundRidge[nIdxNRidge].valid = true;
							}
						}
						else if (nCntRidge == 2) {
							nStartNRidge = (nRecordNRidge - 1) - (nCntRidge - 1);
							nIdxNRidge = nStartNRidge + (nCntRidge / 2);
							sprintf(buffer, "[FPTexture.c] %s() #Line %d n = %d / (nRecordNRidge - 1) = %d / nCntRidge = %d / nStartNRidge = %d / nIdxNRidge = %d\r\n", __FUNCTION__, __LINE__, n, (nRecordNRidge - 1), nCntRidge, nStartNRidge, nIdxNRidge);
							LOGD(buffer);
							pTexture->pBoundRidge[nIdxNRidge].valid = true;
						}
						else {
							nStartNRidge = (nRecordNRidge - 1) - (nCntRidge - 1);
							nIdxNRidge = nStartNRidge;
							sprintf(buffer, "[FPTexture.c] %s() #Line %d n = %d / (nRecordNRidge - 1) = %d / nCntRidge = %d / nStartNRidge = %d / nIdxNRidge = %d\r\n", __FUNCTION__, __LINE__, n, (nRecordNRidge - 1), nCntRidge, nStartNRidge, nIdxNRidge);
							LOGD(buffer);
							pTexture->pBoundRidge[nIdxNRidge].valid = true;
						}
					}
					nCntRidge = 0;

					//Valley
					for (m = nRecordNValley; m < pTexture->nTotalBoundValley; m++) {
						x = pTexture->pBoundValley[m].x;
						y = pTexture->pBoundValley[m].y;
						if (x == i && y < (j + OneBlockSize)) {
							nCntValley++;
							sprintf(buffer, "[FPTexture.c] %s() #Line %d m = %d / nRecordNValley = %d / (%d, %d) / nCntValley = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, m, nRecordNValley, i, j, nCntValley, x, y);
							LOGD(buffer);
							nRecordNValley = m + 1;//保存下一個Index
						}
					}
					if (nCntValley != 0) {
						if (nCntValley > 2) {
							if (nCntValley % 2 == 0) {
								nStartNValley = (nRecordNValley - 1) - (nCntValley - 1);
								nIdxNValley = nStartNValley + (nCntValley / 2);
								sprintf(buffer, "[FPTexture.c] %s() #Line %d m = %d / (nRecordNValley - 1) = %d / nCnt = %d / nStartNValley = %d / nIdxNValley = %d\r\n", __FUNCTION__, __LINE__, m, (nRecordNValley - 1), nCntValley, nStartNValley, nIdxNValley);
								LOGD(buffer);
								pTexture->pBoundValley[nIdxNValley].valid = true;
							}
							else {
								nStartNValley = (nRecordNValley - 1) - (nCntValley - 1);
								nIdxNValley = nStartNValley + (int)(nCntValley / 2);
								sprintf(buffer, "[FPTexture.c] %s() #Line %d m = %d / (nRecordNValley - 1) = %d / nCntValley = %d / nStartNValley = %d / nIdxNValley = %d\r\n", __FUNCTION__, __LINE__, m, (nRecordNValley - 1), nCntValley, nStartNValley, nIdxNValley);
								LOGD(buffer);
								pTexture->pBoundValley[nIdxNValley].valid = true;
							}
						}
						else if (nCntValley == 2) {
							nStartNValley = (nRecordNValley - 1) - (nCntValley - 1);
							nIdxNValley = nStartNValley + (nCntValley / 2);
							sprintf(buffer, "[FPTexture.c] %s() #Line %d m = %d / (nRecordNValley - 1) = %d / nCntValley = %d / nStartNValley = %d / nIdxNValley = %d\r\n", __FUNCTION__, __LINE__, m, (nRecordNValley - 1), nCntValley, nStartNValley, nIdxNValley);
							LOGD(buffer);
							pTexture->pBoundValley[nIdxNValley].valid = true;
						}
						else {
							nStartNValley = (nRecordNValley - 1) - (nCntValley - 1);
							nIdxNValley = nStartNValley;
							sprintf(buffer, "[FPTexture.c] %s() #Line %d m = %d / (nRecordNValley - 1) = %d / nCntValley = %d / nStartNValley = %d / nIdxNValley = %d\r\n", __FUNCTION__, __LINE__, m, (nRecordNValley - 1), nCntValley, nStartNValley, nIdxNValley);
							LOGD(buffer);
							pTexture->pBoundValley[nIdxNValley].valid = true;
						}
					}
					nCntValley = 0;
				}
			}
		}
	}
}

void CheckMinDiffBoundValidData(struct Texture *pTexture, unsigned char **OgrImage_in, struct BlockInfo *pVerticalBlockInfoData, short **pMergeData, int nWidth, int nHeight)
{
	if (pTexture == NULL || OgrImage_in == NULL || pVerticalBlockInfoData == NULL)
		return;

	//char buffer[1000] = { 0 };
	int  n, x, y, i, j, ii, jj;
	short sbuf;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	//(-1)代表整個Col都是Good Block
	//signed char InValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) };//保存 Y 的 Bad Block 位置
	int nRecordRidge = 0, nTotalRidge = 0, nStartRidge, nIDxRidge;
	int nRecordValley = 0, nTotalValley = 0, nStartValley, nIDxValley;

	unsigned char **pHorizontalData = NULL, **pVerticalData = NULL;
	pHorizontalData = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pVerticalData = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pHorizontalData[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pHorizontalData[j], 255, nWidth);
		pVerticalData[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pVerticalData[j], 255, nWidth);
	}

	TextureHorizontalImageProcess(OgrImage_in, pHorizontalData, nWidth, nHeight);
	TextureVerticalImageProcess(OgrImage_in, pVerticalData, nWidth, nHeight);

	/*尋找分界線位置*/
	//for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
	//	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
	//		if (pVerticalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
	//			InValidRow[i] = j;//紀錄無效區域的Y位置
	//			break;
	//		}
	//	}
	//}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			pMergeData[j][i] = 255;
		}
	}

	//Merge Differential Data
	for (i = 0; i < nWidth; i++) {
		for (j = 0; j < nHeight; j++) {
			if (i % OneBlockSize == 0 && j % OneBlockSize == 0) {
				if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							sbuf = pHorizontalData[j + jj][i + ii] - 128;
							pMergeData[j + jj][i + ii] = sbuf;
						}
					}
				}
				else {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							sbuf = pVerticalData[j + jj][i + ii] - 128;
							pMergeData[j + jj][i + ii] = sbuf;
						}
					}
				}
			}
		}
	}
	Save2ByteTXT("MergeDifferentialData.txt", pMergeData, nWidth, nHeight);

	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth - 1) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						if (i != nWidth - 1) {
							for (ii = 0; ii < OneBlockSize; ii++) {
								if (i + ii > nWidth - 1)
									break;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i + ii, j);
								//LOGD(buffer);

								//Ridge
								for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
									x = pTexture->pBoundRidge[n].x;
									y = pTexture->pBoundRidge[n].y;
									if (x == (i + ii) && y == j) {
										nTotalRidge++;
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d Ridge *** (%d, %d) / n = %d ***\r\n", __FUNCTION__, __LINE__, x, y, n);
										//LOGD(buffer);
										nRecordRidge = n;
									}
								}

								//Valley  
								for (n = 0; n < pTexture->nTotalBoundValley; n++) {
									x = pTexture->pBoundValley[n].x;
									y = pTexture->pBoundValley[n].y;
									if (x == (i + ii) && y == j) {
										nTotalValley++;
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d Valley *** (%d, %d) / n = %d ***\r\n", __FUNCTION__, __LINE__, x, y, n);
										//LOGD(buffer);
										nRecordValley = n;
									}
								}
							}
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** nTotalRidge = %d / nTotalValley = %d ***\r\n", __FUNCTION__, __LINE__, nTotalRidge, nTotalValley);
							//LOGD(buffer);

							if (nTotalRidge != 0) {
								nStartRidge = nRecordRidge - (nTotalRidge - 1);
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d nStartRidge = % d / nTotalRidge = %d / nStartRidge + nTotalRidge = %d\r\n", __FUNCTION__, __LINE__, nStartRidge, nTotalRidge, nStartRidge + nTotalRidge);
								//LOGD(buffer);

								short *pRidgeData = NULL, k = 0;
								pRidgeData = (short *)malloc(sizeof(short) * nTotalRidge);
								memset(pRidgeData, 0, sizeof(short) * nTotalRidge);

								for (n = nStartRidge; n < (nStartRidge + nTotalRidge); n++) {
									x = pTexture->pBoundRidge[n].x;
									y = pTexture->pBoundRidge[n].y;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);

									pRidgeData[k] = pMergeData[y][x];
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d pRidgeData[%d] = %d\r\n", __FUNCTION__, __LINE__, k, pRidgeData[k]);
									//LOGD(buffer);
									k++;
								}

								int nMinIDx = 0;
								GetMinimumValue(pRidgeData, nTotalRidge, &nMinIDx);
								nIDxRidge = nStartRidge + nMinIDx;
								pTexture->pBoundRidge[nIDxRidge].valid = true;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d nMinIDx = %d / nIDxRidge = %d\r\n", __FUNCTION__, __LINE__, nMinIDx, nIDxRidge);
								//LOGD(buffer);
								free(pRidgeData);
							}
							nTotalRidge = 0;

							if (nTotalValley != 0) {
								nStartValley = nRecordValley - (nTotalValley - 1);
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d nStartValley = % d / nTotalValley = %d / nStartValley + nTotalValley = %d\r\n", __FUNCTION__, __LINE__, nStartValley, nTotalValley, nStartValley + nTotalValley);
								//LOGD(buffer);

								short *pValleyaData = NULL, k = 0;
								pValleyaData = (short *)malloc(sizeof(short) * nTotalValley);
								memset(pValleyaData, 0, sizeof(short) * nTotalValley);

								for (n = nStartValley; n < (nStartValley + nTotalValley); n++) {
									x = pTexture->pBoundValley[n].x;
									y = pTexture->pBoundValley[n].y;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);

									pValleyaData[k] = pMergeData[y][x];
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d pRidgeData[%d] = %d\r\n", __FUNCTION__, __LINE__, k, pValleyaData[k]);
									//LOGD(buffer);
									k++;
								}

								int nMinIDx = 0;
								GetMinimumValue(pValleyaData, nTotalValley, &nMinIDx);
								nIDxValley = nStartValley + nMinIDx;
								pTexture->pBoundValley[nIDxValley].valid = true;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d nMinIDx = %d / nIDxValley = %d\r\n", __FUNCTION__, __LINE__, nMinIDx, nIDxValley);
								//LOGD(buffer);
								free(pValleyaData);
							}
							nTotalValley = 0;
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (j + jj > nHeight - 1)
								break;
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j + jj);
							//LOGD(buffer);

							//Ridge
							for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
								x = pTexture->pBoundRidge[n].x;
								y = pTexture->pBoundRidge[n].y;
								if (x == i && y == (j + jj)) {
									nTotalRidge++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Ridge *** (%d, %d) / n = %d ***\r\n", __FUNCTION__, __LINE__, x, y, n);
									//LOGD(buffer);
									nRecordRidge = n;
								}
							}

							//Valley  
							for (n = 0; n < pTexture->nTotalBoundValley; n++) {
								x = pTexture->pBoundValley[n].x;
								y = pTexture->pBoundValley[n].y;
								if (x == i && y == (j + jj)) {
									nTotalValley++;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d Valley *** (%d, %d) / n = %d ***\r\n", __FUNCTION__, __LINE__, x, y, n);
									//LOGD(buffer);
									nRecordValley = n;
								}
							}
						}
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** nTotalRidge = %d / nTotalValley = %d ***\r\n", __FUNCTION__, __LINE__, nTotalRidge, nTotalValley);
						//LOGD(buffer);

						if (nTotalRidge != 0) {
							nStartRidge = nRecordRidge - (nTotalRidge - 1);
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d nStartRidge = % d / nTotalRidge = %d / nStartRidge + nTotalRidge = %d\r\n", __FUNCTION__, __LINE__, nStartRidge, nTotalRidge, nStartRidge + nTotalRidge);
							//LOGD(buffer);

							short *pRidgeData = NULL, k = 0;
							pRidgeData = (short *)malloc(sizeof(short) * nTotalRidge);
							memset(pRidgeData, 0, sizeof(short) * nTotalRidge);

							for (n = nStartRidge; n < (nStartRidge + nTotalRidge); n++) {
								x = pTexture->pBoundRidge[n].x;
								y = pTexture->pBoundRidge[n].y;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);

								pRidgeData[k] = pMergeData[y][x];
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pRidgeData[%d] = %d\r\n", __FUNCTION__, __LINE__, k, pRidgeData[k]);
								//LOGD(buffer);
								k++;
							}

							int nMinIDx = 0;
							GetMinimumValue(pRidgeData, nTotalRidge, &nMinIDx);
							nIDxRidge = nStartRidge + nMinIDx;
							pTexture->pBoundRidge[nIDxRidge].valid = true;
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d nMinIDx = %d / nIDxRidge = %d\r\n", __FUNCTION__, __LINE__, nMinIDx, nIDxRidge);
							//LOGD(buffer);
							free(pRidgeData);
						}
						nTotalRidge = 0;

						if (nTotalValley != 0) {
							nStartValley = nRecordValley - (nTotalValley - 1);
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d nStartValley = % d / nTotalValley = %d / nStartValley + nTotalValley = %d\r\n", __FUNCTION__, __LINE__, nStartValley, nTotalValley, nStartValley + nTotalValley);
							//LOGD(buffer);

							short *pValleyaData = NULL, k = 0;
							pValleyaData = (short *)malloc(sizeof(short) * nTotalValley);
							memset(pValleyaData, 0, sizeof(short) * nTotalValley);

							for (n = nStartValley; n < (nStartValley + nTotalValley); n++) {
								x = pTexture->pBoundValley[n].x;
								y = pTexture->pBoundValley[n].y;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);

								pValleyaData[k] = pMergeData[y][x];
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pValleyaData[%d] = %d\r\n", __FUNCTION__, __LINE__, k, pValleyaData[k]);
								//LOGD(buffer);
								k++;
							}

							int nMinIDx = 0;
							GetMinimumValue(pValleyaData, nTotalValley, &nMinIDx);
							nIDxValley = nStartValley + nMinIDx;
							pTexture->pBoundValley[nIDxValley].valid = true;
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d nMinIDx = %d / nIDxValley = %d\r\n", __FUNCTION__, __LINE__, nMinIDx, nIDxValley);
							//LOGD(buffer);
							free(pValleyaData);
						}
						nTotalValley = 0;
					}
				}
			}
		}
	}

	if (pVerticalData != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(pVerticalData[j]);
		}
		free(pVerticalData);
		pVerticalData = NULL;
	}

	if (pHorizontalData != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(pHorizontalData[j]);
		}
		free(pHorizontalData);
		pHorizontalData = NULL;
	}
}

void GetMinimumValue(short *pData, int nMaxCount, int *pIndex)
{
	if (pData == NULL)
		return;

	int nCurrentIDx = -1;
	int nMinimum = abs(pData[0]);
	nCurrentIDx = 0;
	for (int i = 0; i < nMaxCount; i++) {
		if (abs(pData[i]) <= nMinimum) {
			nMinimum = abs(pData[i]);
			nCurrentIDx = i;
		}
	}
	*pIndex = nCurrentIDx;
}

void DoubleCheckMinDiffBoundValidData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pVerticalBlockInfoData, short **pMergeData, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTexture == NULL || pVerticalBlockInfoData == NULL || pMergeData == NULL)
		return;

	char buffer[1000] = { 0 };
	int i, j, ii, jj, n, x, y;
	unsigned char R1 = 0, V1 = 255, E0 = 170, D1 = 85, nR1 = 10, nV1 = 245;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	//(-1)代表整個Col都是Good Block
	//signed char InValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) };//保存 Y 的 Bad Block 位置

	unsigned char **pLocalMergeImage = NULL;
	pLocalMergeImage = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pLocalMergeImage[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pLocalMergeImage[j], 255, nWidth);
		memcpy(pLocalMergeImage[j], pMergeImage[j], nWidth);
	}
	//變更Ridges 0 -> 10 標記作業
	for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
		if (pTexture->pBoundRidge[n].valid == true) {
			x = pTexture->pBoundRidge[n].x;
			y = pTexture->pBoundRidge[n].y;
			pLocalMergeImage[y][x] = nR1;
		}
	}
	//變更Valleys 255 -> 245 標記作業
	for (n = 0; n < pTexture->nTotalBoundValley; n++) {
		if (pTexture->pBoundValley[n].valid == true) {
			x = pTexture->pBoundValley[n].x;
			y = pTexture->pBoundValley[n].y;
			pLocalMergeImage[y][x] = nV1;
		}
	}

	/*尋找分界線位置*/
	//for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
	//	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
	//		if (pVerticalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
	//			InValidRow[i] = j;//紀錄無效區域的Y位置
	//			break;
	//		}
	//	}
	//}

	//Valley
	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth - 1) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);

						if (i != nWidth - 1) {
							for (ii = 0; ii < OneBlockSize; ii++) {
								if (i + ii > nWidth - 1)
									break;

								//Valley
								if (pLocalMergeImage[j][i + ii] == V1) {
									x = i + ii;
									y = j;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d x = %d / y = %d\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);

									if ((x + 1) < (i + 8)) {
										if ((x + 2) < (i + 8)) {
											if (pLocalMergeImage[y][x + 2] == nV1) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 2, y);
												//LOGD(buffer);
												break;
											}
										}
										if (pLocalMergeImage[y][x + 1] == nV1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 1, y);
											//LOGD(buffer);
											break;
										}
										if (pLocalMergeImage[y][x + 1] == V1) {
											if (abs(pMergeData[y][x]) < abs(pMergeData[y][x + 1])) {
												pLocalMergeImage[y][x] = nV1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
												//LOGD(buffer);
												break;
											}
											else {
												pLocalMergeImage[y][x + 1] = nV1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 1, y);
												//LOGD(buffer);
												break;
											}
										}
									}
									if ((x - 1) >= i) {
										if ((x - 2) >= i) {
											if (pLocalMergeImage[y][x - 2] == nV1) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 2, y);
												//LOGD(buffer);
												break;
											}
										}
										if (pLocalMergeImage[y][x - 1] == nV1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 1, y);
											//LOGD(buffer);
											break;	
										}
										if (pLocalMergeImage[y][x - 1] == V1) {
											if (abs(pMergeData[y][x]) < abs(pMergeData[y][x - 1])) {
												pLocalMergeImage[y][x] = nV1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
												//LOGD(buffer);
												break;
											}
											else {
												pLocalMergeImage[y][x - 1] = nV1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 1, y);
												//LOGD(buffer);
												break;
											}
										}
									}
									pLocalMergeImage[y][x] = nV1;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);
								}
							}
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);

						for (jj = 0; jj < OneBlockSize; jj++) {
							if (j + jj > nHeight - 1)
								break;

							//Valley
							if (pLocalMergeImage[j + jj][i] == V1) {
								x = i;
								y = j + jj;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d x = %d / y = %d\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);

								if ((y + 1) < (j + 8)) {
									if ((y + 2) < (j + 8)) {
										if (pLocalMergeImage[y + 2][x] == nV1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 2);
											//LOGD(buffer);
											break;
										}
									}
									if (pLocalMergeImage[y + 1][x] == nV1) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 1);
										//LOGD(buffer);
										break;
									}
									if (pLocalMergeImage[y + 1][x] == V1) {
										if (abs(pMergeData[y][x]) < abs(pMergeData[y + 1][x])) {
											pLocalMergeImage[y][x] = nV1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
											//LOGD(buffer);
											break;
										}
										else {
											pLocalMergeImage[y + 1][x] = nV1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 1);
											//LOGD(buffer);
											break;
										}
									}
								}
								if ((y - 1) >= j) {
									if ((y - 2) >= j) {
										if (pLocalMergeImage[y - 2][x] == nV1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 2);
											//LOGD(buffer);
											break;
										}
									}
									if (pLocalMergeImage[y - 1][x] == nV1) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 1);
										//LOGD(buffer);
										break;
									}
									if (pLocalMergeImage[y - 1][x] == V1) {
										if (abs(pMergeData[y][x]) < abs(pMergeData[y - 1][x])) {
											pLocalMergeImage[y][x] = nV1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
											//LOGD(buffer);
											break;
										}
										else {
											pLocalMergeImage[y - 1][x] = nV1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 1);
											//LOGD(buffer);
											break;
										}
									}
								}
								pLocalMergeImage[y][x] = nV1;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);
							}
						}
					}
				}
			}
		}
	}

	//Ridge
	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth - 1) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);

						if (i != nWidth - 1) {
							for (ii = 0; ii < OneBlockSize; ii++) {
								if (i + ii > nWidth - 1)
									break;

								//Ridge
								if (pLocalMergeImage[j][i + ii] == R1) {
									x = i + ii;
									y = j;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d x = %d / y = %d\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);

									if ((x + 1) < (i + 8)) {
										if ((x + 2) < (i + 8)) {
											if (pLocalMergeImage[y][x + 2] == nR1) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 2, y);
												//LOGD(buffer);
												break;
											}
										}
										if (pLocalMergeImage[y][x + 1] == nR1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 1, y);
											//LOGD(buffer);
											break;
										}
										if (pLocalMergeImage[y][x + 1] == R1) {
											if (abs(pMergeData[y][x]) < abs(pMergeData[y][x + 1])) {
												pLocalMergeImage[y][x] = nR1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
												//LOGD(buffer);
												break;
											}
											else {
												pLocalMergeImage[y][x + 1] = nR1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x + 1, y);
												//LOGD(buffer);
												break;
											}
										}
									}
									if ((x - 1) >= i) {
										if ((x - 2) >= i) {
											if (pLocalMergeImage[y][x - 2] == nR1) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 2, y);
												//LOGD(buffer);
												break;
											}
										}
										if (pLocalMergeImage[y][x - 1] == nR1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 1, y);
											//LOGD(buffer);
											break;
											
										}
										if (pLocalMergeImage[y][x - 1] == R1) {
											if (abs(pMergeData[y][x]) < abs(pMergeData[y][x - 1])) {
												pLocalMergeImage[y][x] = nR1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
												//LOGD(buffer);
												break;
											}
											else {
												pLocalMergeImage[y][x - 1] = nR1;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x - 1, y);
												//LOGD(buffer);
												break;
											}
										}
									}
									pLocalMergeImage[y][x] = nR1;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
									//LOGD(buffer);
								}
							}
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);

						for (jj = 0; jj < OneBlockSize; jj++) {
							if (j + jj > nHeight - 1)
								break;

							//Ridge
							if (pLocalMergeImage[j + jj][i] == R1) {
								x = i;
								y = j + jj;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d x = %d / y = %d\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);

								if ((y + 1) < (j + 8)) {
									if ((y + 2) < (j + 8)) {
										if (pLocalMergeImage[y + 2][x] == nR1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 2);
											//LOGD(buffer);
											break;
										}
									}
									if (pLocalMergeImage[y + 1][x] == nR1) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 1);
										//LOGD(buffer);
										break;
									}
									if (pLocalMergeImage[y + 1][x] == R1) {
										if (abs(pMergeData[y][x]) < abs(pMergeData[y + 1][x])) {
											pLocalMergeImage[y][x] = nR1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
											//LOGD(buffer);
											break;
										}
										else {
											pLocalMergeImage[y + 1][x] = nR1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y + 1);
											//LOGD(buffer);
											break;
										}
									}
								}
								if ((y - 1) >= j) {
									if ((y - 2) >= j) {
										if (pLocalMergeImage[y - 2][x] == nR1) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 2);
											//LOGD(buffer);
											break;
										}
									}
									if (pLocalMergeImage[y - 1][x] == nR1) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 1);
										//LOGD(buffer);
										break;	
									}
									if (pLocalMergeImage[y - 1][x] == R1) {
										if (abs(pMergeData[y][x]) < abs(pMergeData[y - 1][x])) {
											pLocalMergeImage[y][x] = nR1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
											//LOGD(buffer);
											break;
										}
										else {
											pLocalMergeImage[y - 1][x] = nR1;
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y - 1);
											//LOGD(buffer);
											break;
										}
									}
								}
								pLocalMergeImage[y][x] = nR1;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO (%d, %d)\r\n", __FUNCTION__, __LINE__, x, y);
								//LOGD(buffer);
							}
						}
					}
				}
			}
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			//Valley
			if (pLocalMergeImage[j][i] == nV1) {
				for (n = 0; n < pTexture->nTotalBoundValley; n++) {
					if (pTexture->pBoundValley[n].valid == false) {
						x = pTexture->pBoundValley[n].x;
						y = pTexture->pBoundValley[n].y;
						if (x == i && y == j) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d Find Valley (%d, %d) / n = %d\r\n", __FUNCTION__, __LINE__, x, y, n);
							//LOGD(buffer);
							pTexture->pBoundValley[n].valid = true;
						}
					}
				}
			}
			//Ridge
			if (pLocalMergeImage[j][i] == nR1) {
				for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
					if (pTexture->pBoundRidge[n].valid == false) {
						x = pTexture->pBoundRidge[n].x;
						y = pTexture->pBoundRidge[n].y;
						if (x == i && y == j) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d Find Ridge (%d, %d) / n = %d\r\n", __FUNCTION__, __LINE__, x, y, n);
							//LOGD(buffer);
							pTexture->pBoundRidge[n].valid = true;
						}
					}
				}
			}
		}
	}

	//過濾跨Block的連續標記，當前Block最後一個數值與下一個Block的第一個數值同時被標記時，只保留Differential最小的那個標記當做有效值，另一個標記則需要變更為無效值flag
	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth - 1) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= g_nRow_Y /*InValidRow[(i / OneBlockSize)]*/ && /*InValidRow[(i / OneBlockSize)]*/ g_nRow_Y != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						if (i != nWidth - 1) {

								if (i + (EMC_BLOCK_SIZE - 1) > nWidth - 1 || i + EMC_BLOCK_SIZE > nWidth - 1)
									break;

								int x1, x2, y1, y2;
								//Valleys
								if (pLocalMergeImage[j][i + (EMC_BLOCK_SIZE - 1)] == nV1) {
									x1 = i + (EMC_BLOCK_SIZE - 1);
									y1 = j;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 = %d / y1 = %d\r\n", __FUNCTION__, __LINE__, x1, y1);
									//LOGD(buffer);
									if (pLocalMergeImage[j][i + EMC_BLOCK_SIZE] == nV1) {
										x2 = i + EMC_BLOCK_SIZE;
										y2 = j;
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 = %d / y2 = %d\r\n", __FUNCTION__, __LINE__, x2, y2);
										//LOGD(buffer);

										//Y 座標已經超過 Block Bad 的限制線，落在 Block Good 範圍內
										if (y2 < /*InValidRow[(x2 / OneBlockSize)]*/ g_nRow_Y * EMC_BLOCK_SIZE) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Break...\r\n", __FUNCTION__, __LINE__);
											//LOGD(buffer);
											break;
										}
										else {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Do Something...\r\n", __FUNCTION__, __LINE__);
											//LOGD(buffer);

											if (abs(pMergeData[y1][x1]) < abs(pMergeData[y2][x2])) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 / y1 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x1, y1);
												//LOGD(buffer);
												for (n = 0; n < pTexture->nTotalBoundValley; n++) {
													x = pTexture->pBoundValley[n].x;
													y = pTexture->pBoundValley[n].y;
													if (x == x2 && y == y2) {//修改與變更X2/Y2為無效旗標
														pTexture->pBoundValley[n].valid = false;
														//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundValley[n].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundValley[n].x, pTexture->pBoundValley[n].y, pTexture->pBoundValley[n].valid);
														//LOGD(buffer);
													}
												}
											}
											else {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 / y2 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x2, y2);
												//LOGD(buffer);
												for (n = 0; n < pTexture->nTotalBoundValley; n++) {
													x = pTexture->pBoundValley[n].x;
													y = pTexture->pBoundValley[n].y;
													if (x == x1 && y == y1) {//修改與變更X1/Y1為無效旗標
														pTexture->pBoundValley[n].valid = false;
														//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundValley[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundValley[n].x, pTexture->pBoundValley[n].y, n, pTexture->pBoundValley[n].valid);
														//LOGD(buffer);
													}
												}
											}
										}
									}
								}

								//Ridges
								if (pLocalMergeImage[j][i + (EMC_BLOCK_SIZE - 1)] == nR1) {
									x1 = i + (EMC_BLOCK_SIZE - 1);
									y1 = j;
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 = %d / y1 = %d\r\n", __FUNCTION__, __LINE__, x1, y1);
									//LOGD(buffer);
									if (pLocalMergeImage[j][i + EMC_BLOCK_SIZE] == nR1) {
										x2 = i + EMC_BLOCK_SIZE;
										y2 = j;
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 = %d / y2 = %d\r\n", __FUNCTION__, __LINE__, x2, y2);
										//LOGD(buffer);

										//Y 座標已經超過 Block Bad 的限制線，落在 Block Good 範圍內
										if (y2 < /*InValidRow[(x2 / OneBlockSize)]*/ g_nRow_Y * EMC_BLOCK_SIZE) {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Break...\r\n", __FUNCTION__, __LINE__);
											//LOGD(buffer);
											break;
										}
										else {
											//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Do Something...\r\n", __FUNCTION__, __LINE__);
											//LOGD(buffer);

											if (abs(pMergeData[y1][x1]) < abs(pMergeData[y2][x2])) {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 / y1 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x1, y1);
												//LOGD(buffer);
												for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
													x = pTexture->pBoundRidge[n].x;
													y = pTexture->pBoundRidge[n].y;
													if (x == x2 && y == y2) {//修改與變更X2/Y2為無效旗標
														pTexture->pBoundRidge[n].valid = false;
														//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundRidge[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundRidge[n].x, pTexture->pBoundRidge[n].y, n, pTexture->pBoundRidge[n].valid);
														//LOGD(buffer);
													}
												}
											}
											else {
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 / y2 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x2, y2);
												//LOGD(buffer);
												for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
													x = pTexture->pBoundRidge[n].x;
													y = pTexture->pBoundRidge[n].y;
													if (x == x1 && y == y1) {//修改與變更X1/Y1為無效旗標
														pTexture->pBoundRidge[n].valid = false;
														//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundRidge[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundRidge[n].x, pTexture->pBoundRidge[n].y, n, pTexture->pBoundRidge[n].valid);
														//LOGD(buffer);
													}
												}
											}
										}

									}
								}
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);

						if ((j + (EMC_BLOCK_SIZE - 1)) > nHeight - 1 || j + EMC_BLOCK_SIZE > nHeight - 1)
							break;

						int x1, x2, y1, y2;
						//Valleys
						if (pLocalMergeImage[j + (EMC_BLOCK_SIZE - 1)][i] == nV1) {
							x1 = i;
							y1 = j + (EMC_BLOCK_SIZE - 1);
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 = %d / y1 = %d\r\n", __FUNCTION__, __LINE__, x1, y1);
							//LOGD(buffer);
							if (pLocalMergeImage[j + EMC_BLOCK_SIZE][i] == nV1) {
								x2 = i;
								y2 = j + EMC_BLOCK_SIZE;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 = %d / y2 = %d\r\n", __FUNCTION__, __LINE__, x2, y2);
								//LOGD(buffer);

								//Y 座標已經超過 Block Good 的限制線，落在 Block Bad 範圍內
								if (y2 >= /*InValidRow[(x2 / OneBlockSize)]*/ g_nRow_Y * EMC_BLOCK_SIZE) {
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Break...\r\n", __FUNCTION__, __LINE__);
									//LOGD(buffer);
									break;
								}
								else {
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Do Something...\r\n", __FUNCTION__, __LINE__);
									//LOGD(buffer);

									if (abs(pMergeData[y1][x1]) < abs(pMergeData[y2][x2])) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 / y1 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x1, y1);
										//LOGD(buffer);
										for (n = 0; n < pTexture->nTotalBoundValley; n++) {
											x = pTexture->pBoundValley[n].x;
											y = pTexture->pBoundValley[n].y;
											if (x == x2 && y == y2) {//修改與變更X2/Y2為無效旗標
												pTexture->pBoundValley[n].valid = false;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundValley[n].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundValley[n].x, pTexture->pBoundValley[n].y, pTexture->pBoundValley[n].valid);
												//LOGD(buffer);
											}
										}
									}
									else {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 / y2 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x2, y2);
										//LOGD(buffer);
										for (n = 0; n < pTexture->nTotalBoundValley; n++) {
											x = pTexture->pBoundValley[n].x;
											y = pTexture->pBoundValley[n].y;
											if (x == x1 && y == y1) {//修改與變更X1/Y1為無效旗標
												pTexture->pBoundValley[n].valid = false;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundValley[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundValley[n].x, pTexture->pBoundValley[n].y, n, pTexture->pBoundValley[n].valid);
												//LOGD(buffer);
											}
										}
									}
								}
							}	
						}

						//Ridges
						if (pLocalMergeImage[j + (EMC_BLOCK_SIZE - 1)][i] == nR1) {
							x1 = i;
							y1 = j + (EMC_BLOCK_SIZE - 1);
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 = %d / y1 = %d\r\n", __FUNCTION__, __LINE__, x1, y1);
							//LOGD(buffer);
							if (pLocalMergeImage[j + EMC_BLOCK_SIZE][i] == nR1) {
								x2 = i;
								y2 = j + EMC_BLOCK_SIZE;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 = %d / y2 = %d\r\n", __FUNCTION__, __LINE__, x2, y2);
								//LOGD(buffer);

								//Y 座標已經超過 Block Good 的限制線，落在 Block Bad 範圍內
								if (y2 >= /*InValidRow[(x2 / OneBlockSize)]*/ g_nRow_Y * EMC_BLOCK_SIZE) {
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Break...\r\n", __FUNCTION__, __LINE__);
									//LOGD(buffer);
									break;
								}
								else {
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Do Something...\r\n", __FUNCTION__, __LINE__);
									//LOGD(buffer);

									if (abs(pMergeData[y1][x1]) < abs(pMergeData[y2][x2])) {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x1 / y1 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x1, y1);
										//LOGD(buffer);
										for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
											x = pTexture->pBoundRidge[n].x;
											y = pTexture->pBoundRidge[n].y;
											if (x == x2 && y == y2) {//修改與變更X2/Y2為無效旗標
												pTexture->pBoundRidge[n].valid = false;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundRidge[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundRidge[n].x, pTexture->pBoundRidge[n].y, n, pTexture->pBoundRidge[n].valid);
												//LOGD(buffer);
											}
										}
									}
									else {
										//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] x2 / y2 Min is (%d, %d)\r\n", __FUNCTION__, __LINE__, x2, y2);
										//LOGD(buffer);
										for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
											x = pTexture->pBoundRidge[n].x;
											y = pTexture->pBoundRidge[n].y;
											if (x == x1 && y == y1) {//修改與變更X1/Y1為無效旗標
												pTexture->pBoundRidge[n].valid = false;
												//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Update (%d, %d) / pTexture->pBoundRidge[%d].valid =  %d\r\n", __FUNCTION__, __LINE__, pTexture->pBoundRidge[n].x, pTexture->pBoundRidge[n].y, n, pTexture->pBoundRidge[n].valid);
												//LOGD(buffer);
											}
										}
									}
								}
							}
						}

					}
				}
			}
		}
	}

#if 0
	//Check and Update
	for (j = 0; j < nHeight; j++) {
		memset(pLocalMergeImage[j], 255, nWidth);
		memcpy(pLocalMergeImage[j], pMergeImage[j], nWidth);
	}
	//變更Ridges 0 -> 10 標記作業
	for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
		if (pTexture->pBoundRidge[n].valid == true) {
			x = pTexture->pBoundRidge[n].x;
			y = pTexture->pBoundRidge[n].y;
			pLocalMergeImage[y][x] = nR1;
		}
	}

	//變更Valleys 255 -> 245 標記作業
	for (n = 0; n < pTexture->nTotalBoundValley; n++) {
		if (pTexture->pBoundValley[n].valid == true) {
			x = pTexture->pBoundValley[n].x;
			y = pTexture->pBoundValley[n].y;
			pLocalMergeImage[y][x] = nV1;
		}
	}
	SaveTXT("TempMergerImage.txt", pLocalMergeImage, nWidth, nHeight);
#endif

	if (pLocalMergeImage != NULL) {
		for (j = 0; j < nHeight; j++)
			free(pLocalMergeImage[j]);
		free(pLocalMergeImage);
		pLocalMergeImage = NULL;
	}
}

void GetValidValleyData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTexture == NULL || pHorizontalBlockInfoData == NULL || pVerticalBlockInfoData == NULL)
		return;
	
	//char buffer[1000] = { 0 };
	int i, j, /*jj, */ n, x, y;
	unsigned char R1 = 0, V1 = 255, E0 = 170, D1 = 85, nV1 = 245, nR1 = 10;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	//signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) }; //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 }; //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	//unsigned char HorizontalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };//保存水平方向 Row 的無效區域總數
	//unsigned char HorizontalInValidCol[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };//保存水平方向 Col 的無效區域總數
	struct OrientationInfo **pOrientationInfo = NULL;
	pOrientationInfo = (struct OrientationInfo **) malloc (sizeof(struct OrientationInfo *) * (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE));
	for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
		pOrientationInfo[j] = (struct OrientationInfo *) malloc (sizeof(struct OrientationInfo) * (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE));
	}

	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
		VerticalInValidRow[j] = -1; /* KennyKang20190619 Modify */
		for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
			pOrientationInfo[j][i].pValleys = NULL;
			pOrientationInfo[j][i].ValleyTotal = 0;
			pOrientationInfo[j][i].theta1 = 255;
			pOrientationInfo[j][i].theta2 = 255;
		}
	}

#if 0
	/*尋找水平方向的分界線位置*/
	/*1.紀錄Row方向無效區數量*/
	int nBadRowCnt = 0, nBadColCnt = 0;
	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
		for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
			if (pHorizontalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD)
				nBadRowCnt++;
		}
		HorizontalInValidRow[j] = nBadRowCnt;//紀錄Row方向的無效區域總數
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d j = %d / nBadRowCnt = %d\r\n", __FUNCTION__, __LINE__, j, nBadRowCnt);
		//LOGD(buffer);
		nBadRowCnt = 0;
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/*2.紀錄Col方向無效區數量*/
	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
		if (HorizontalInValidRow[j] == 1 || HorizontalInValidRow[j] == 2) {
			for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
				if (pHorizontalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d i = %d / j = %d\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);
					for (jj = 0; jj < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; jj++) {
						if (pHorizontalBlockInfoData->EdgeStatus[jj][i] == EMC_BLOCK_BAD) {
							nBadColCnt++;
						}
						if (HorizontalInValidRow[jj] == 0)//代表這一列沒有Bad Block，用此行來過濾與判斷
							break;
					}
					if (j <= jj) {
						HorizontalInValidCol[i] = nBadColCnt;//紀錄Col方向的無效區域總數
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d, %d) / nBadColCnt = %d\r\n", __FUNCTION__, __LINE__, i, j, jj, nBadColCnt);
						//LOGD(buffer);
						nBadColCnt = 0;
					}
				}
			}
		}
	}

	int nMax = 0, nColIDx = 0;
	nMax = HorizontalInValidCol[0];
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		if (HorizontalInValidCol[i] >= nMax) {
			nMax = HorizontalInValidCol[i];
			nColIDx = i;
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d nColIDx = %d\r\n", __FUNCTION__, __LINE__, nColIDx);
	LOGD(buffer);

	/*尋找垂直方向的分界線位置*/
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
			if (pVerticalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
				VerticalInValidRow[i] = j;//紀錄無效區域的Y位置
				break;
			}
		}
	}
#endif

	/*尋找垂直方向的分界線位置*/
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		VerticalInValidRow[i] = g_nRow_Y;//紀錄無效區域的Y位置
	}

	unsigned char **pLocalMergeImage = NULL;
	pLocalMergeImage = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pLocalMergeImage[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pLocalMergeImage[j], 255, nWidth);
		memcpy(pLocalMergeImage[j], pMergeImage[j], nWidth);
	}

	for (n = 0; n < pTexture->nTotalBoundValley; n++) {
		if (pTexture->pBoundValley[n].valid == true) {
			x = pTexture->pBoundValley[n].x;
			y = pTexture->pBoundValley[n].y;
			pLocalMergeImage[y][x] = nV1;
		}
	}

	GetBlockValleyDataStatus(pOrientationInfo, pLocalMergeImage, VerticalInValidRow, nWidth, nHeight);//Bounding Valleys 符合 245 的資料結構
	SearchValleyStart(pOrientationInfo, pLocalMergeImage, VerticalInValidRow, g_nCol_X /*nColIDx*/, nWidth, nHeight);
	GetValleysDirection(pLocalMergeImage, VerticalInValidRow, nWidth, nHeight);//Get Valid Valleys Direction

	for (j = 0; j < nHeight; j++) {
		memcpy(pMergeImage[j], pLocalMergeImage[j], nWidth);
	}

	if (pLocalMergeImage != NULL) {
		for (j = 0; j < nHeight; j++)
			free(pLocalMergeImage[j]);
		free(pLocalMergeImage);
		pLocalMergeImage = NULL;
	}

	if (pOrientationInfo != NULL) {
		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			for (i = 0; i < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); i++) {
				if (pOrientationInfo[j][i].pValleys != NULL) {
					free(pOrientationInfo[j][i].pValleys);
					pOrientationInfo[j][i].pValleys = NULL;
				}
			}
		}

		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			free(pOrientationInfo[j]);
		}		
		free(pOrientationInfo);
		pOrientationInfo = NULL;
	}
}

void GetBlockValleyDataStatus(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pOrientationInfo == NULL || pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nV1 = 245;

	int nValleyCnt = 0;
	for (i = 0; i < nWidth; i++) {
		if (i % OneBlockSize == 0 || i == nWidth -1 ) {
			for (j = 0; j < nHeight; j++) {
				if (j % OneBlockSize == 0 || j == nHeight - 1) {
					if ((j / OneBlockSize) >= VerticalInValidRow[(i / OneBlockSize)] && VerticalInValidRow[(i / OneBlockSize)] != -1) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						if (i != nWidth - 1) {
							for (ii = 0; ii < OneBlockSize; ii++) {
								if (i + ii > nWidth - 1)
									break;
								if (pMergeImage[j][i + ii] == nV1) {
									//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i + ii, j);
									//LOGD(buffer);
									nValleyCnt++;
								}
							}
							if (j == nHeight - 1) {
								pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal = pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal + nValleyCnt;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pOrientationInfo[%d][%d].ValleyTotal = %d\r\n", __FUNCTION__, __LINE__, j / OneBlockSize, i / OneBlockSize, pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal);
								//LOGD(buffer);
							}
							else {
								pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal = nValleyCnt;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pOrientationInfo[%d][%d].ValleyTotal = %d\r\n", __FUNCTION__, __LINE__, j / OneBlockSize, i / OneBlockSize, pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal);
								//LOGD(buffer);
							}
							nValleyCnt = 0;
						}
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
						//LOGD(buffer);
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (j + jj > nHeight - 1)
								break;
							if (pMergeImage[j + jj][i] == nV1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j + jj);
								//LOGD(buffer);
								nValleyCnt++;
							}
						}
						if (j != nHeight - 1) {
							if (i == nWidth - 1) {
								pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal = pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal + nValleyCnt;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pOrientationInfo[%d][%d].ValleyTotal = %d\r\n", __FUNCTION__, __LINE__, j / OneBlockSize, i / OneBlockSize, pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal);
								//LOGD(buffer);
							}
							else {
								pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal = nValleyCnt;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d pOrientationInfo[%d][%d].ValleyTotal = %d\r\n", __FUNCTION__, __LINE__, j / OneBlockSize, i / OneBlockSize, pOrientationInfo[(j / OneBlockSize)][(i / OneBlockSize)].ValleyTotal);
								//LOGD(buffer);
							}
						}
						nValleyCnt = 0;
					}
				}
			}
		}
	}

	//Save Valleys X/Y Data
	int nTotal = 0, nLen = 0, nVIDx = 0;
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {

			nLen = pOrientationInfo[j][i].ValleyTotal;
			nTotal = nTotal + nLen;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Block (%d, %d) / nLen = %d ***\r\n", __FUNCTION__, __LINE__, i, j , nLen);
			//LOGD(buffer);

			if (j >= VerticalInValidRow[i] && VerticalInValidRow[i] != -1) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
				//LOGD(buffer);
				if (nLen > 0) {
					if (pOrientationInfo[j][i].pValleys != NULL) {
						free(pOrientationInfo[j][i].pValleys);
						pOrientationInfo[j][i].pValleys = NULL;
					}
					pOrientationInfo[j][i].pValleys = (struct OrientationPOINT *) malloc(sizeof(struct OrientationPOINT) * nLen);
					memset(pOrientationInfo[j][i].pValleys, 0, nLen);

					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								pOrientationInfo[j][i].pValleys[nVIDx].x = i * OneBlockSize + ii;
								pOrientationInfo[j][i].pValleys[nVIDx].y = j * OneBlockSize + jj;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nVIDx++;
							}
						}
					}
					nVIDx = 0;
				}
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
				//LOGD(buffer);
				if (nLen > 0) {
					if (pOrientationInfo[j][i].pValleys != NULL) {
						free(pOrientationInfo[j][i].pValleys);
						pOrientationInfo[j][i].pValleys = NULL;
					}
					pOrientationInfo[j][i].pValleys = (struct OrientationPOINT *) malloc(sizeof(struct OrientationPOINT) * nLen);
					memset(pOrientationInfo[j][i].pValleys, 0, nLen);

					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								pOrientationInfo[j][i].pValleys[nVIDx].x = i * OneBlockSize + ii;
								pOrientationInfo[j][i].pValleys[nVIDx].y = j * OneBlockSize + jj;
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nVIDx++;
							}
						}
					}
					nVIDx = 0;
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d nTotal = %d\r\n", __FUNCTION__, __LINE__, nTotal);
	//LOGD(buffer);
}

void SearchValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int nStartCols, int nWidth, int nHeight)
{
	if (pOrientationInfo == NULL || pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	char buffer[1000] = { 0 };
	int i, j, ii, jj, nStartX, nStartY;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nV1 = 245;

	if (nStartCols == -1)
		nStartCols = (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) / 2;
	sprintf(buffer, "[FPTexture.c] %s() #Line %d nStartCols = %d\r\n", __FUNCTION__, __LINE__, nStartCols);
	LOGD(buffer);

	//Block向右尋找
	for (i = nStartCols; i < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); i++) {
		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			if (j >= VerticalInValidRow[i] && VerticalInValidRow[i] != -1) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToRight Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (j == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								CheckBlockBadValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							}
						}
					}
				}
				else {
					for (ii = 0; ii < OneBlockSize; ii++) {
						if (pMergeImage[j * OneBlockSize][i * OneBlockSize + ii] == nV1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize);
							//LOGD(buffer);
							nStartX = i * OneBlockSize + ii;
							nStartY = j * OneBlockSize;
							CheckBlockBadValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
						}
					}
				}
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToRight Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (i == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								CheckBlockGoodValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							}
						}
					}
				}
				else {
					for (jj = 0; jj < OneBlockSize; jj++) {
						if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize] == nV1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize + jj);
							//LOGD(buffer);
							nStartX = i * OneBlockSize;
							nStartY = j * OneBlockSize + jj;
							CheckBlockGoodValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
						}
					}
				}
			}
		}
	}

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d \r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d *********************************************************************************\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	//Block向左尋找
	for (i = nStartCols - 1; i >= 0; i--) {
		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			if (j >= VerticalInValidRow[i] && VerticalInValidRow[i] != -1) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToLeft Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (j == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								CheckBlockBadValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							}
						}
					}
				}
				else {
					for (ii = 0; ii < OneBlockSize; ii++) {
						if (pMergeImage[j * OneBlockSize][i * OneBlockSize + ii] == nV1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize);
							//LOGD(buffer);
							nStartX = i * OneBlockSize + ii;
							nStartY = j * OneBlockSize;
							CheckBlockBadValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
						}
					}
				}
			}
			else {	
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToLeft Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (i == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nV1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								CheckBlockGoodValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							}
						}
					}
				}
				else {
					for (jj = 0; jj < OneBlockSize; jj++) {
						if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize] == nV1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize + jj);
							//LOGD(buffer);
							nStartX = i * OneBlockSize;
							nStartY = j * OneBlockSize + jj;
							CheckBlockGoodValleyStart(pOrientationInfo, pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
						}
					}
				}	
			}
		}
	}

	//Re-Check and Update Valid Valleys Data
	ReCheckValleysEnding(pMergeImage, VerticalInValidRow, nWidth, nHeight);
}

void CheckBlockGoodValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pOrientationInfo == NULL || pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;   //分岔標記號
	unsigned char V4 = 252, D4 = 82;   //第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	int nCurrent_X_Valleys = 0, nCurrent_Y_Valleys = 0, nFromBlock_X = -1, nFromBlock_Y = -1;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值245 -> 244 代表已經處理過 */
	pMergeImage[nSy][nSx] = nV2;
	nCurrent_Y_Valleys = nSy;//保存 nV1 標記所在的 Y 的位置
	nFromBlock_X = nSx / OneBlockSize;//保存 nV1 標記所在的 Block X 資訊
	nFromBlock_Y = nSy / OneBlockSize;//保存 nV1 標記所在的 Block Y 資訊
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Right pMergeImage[%d][%d] = %d / Block (%d, %d) / Block(%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize, nFromBlock_X, nFromBlock_Y);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向右尋找下一點 */
		for (i = nSx; i < nWidth - 1; ) {
			i = i + 1;	//將 X 座標右移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				//if (nSy + SearchRange[nIDx] > nHeight - 1 || nSy + SearchRange[nIDx] < 0) {
				//	bIsEdge = true;//超過 Y 方向的上下邊界限制，可不繼續做下去
				//	break;
				//}
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}
					
				//符合Valleys的標記255
				if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == V1) {
						pMergeImage[j][i] = V2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else  {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nV1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, j - nCurrent_Y_Valleys);
					//LOGD(buffer);
					pMergeImage[j][i] = nV2;//更改標記為244代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
					//LOGD(buffer);

					nCurrent_Y_Valleys = j;//更新符 nV1 條件所在的 Y 位置
					nFromBlock_X = i / OneBlockSize;//更新符 nV1 條件所在的 Block X 區域
					nFromBlock_Y = j / OneBlockSize;//更新符 nV1 條件所在的 Block Y 區域
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Update] Block (%d, %d) / Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
					//LOGD(buffer);

					//已經發現，若前面有先後順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == D2 || pMergeImage[j][i] == (V2 - 100) || pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == (V2 - 100)) {
							if (nDoubleFirst == 1) {
								if(bIsEdge == true)
									pMergeImage[j][i] = V2;
								else
									pMergeImage[j][i] = V4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = V2;
								else
									pMergeImage[j][i] = V3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nV2 || pMergeImage[j][i] == (nV2 - 100)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, j - nCurrent_Y_Valleys);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / [Last] Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
						//LOGD(buffer);

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nValleysEnding;//標記為 Valleys Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}
			
			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V2 || 
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V3 || 
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V4 || 
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nV2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 100;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i < nWidth - 1; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/* 初始化變數 */
	 bNotFind = false;
	 bPriority = false;
	 bBxPriority = false;
	 bBxFinish = false;
	 bBlockLimit = false;
	 bIsEdge = false;
	 nBxPriorityIDx = -1;
	 nCurrentIDx = 0;
	 nDoubleFirst = 0;
	 nPriorityIDx = -1;
	 nContinueNotFindIDxCnt = 0;
	 nCurrent_X_Valleys = 0;
	 nCurrent_Y_Valleys = 0;
	 nFromBlock_X = -1;
	 nFromBlock_Y = -1;
	 nLastNodeX = -1;
	 nLastNodeY = -1;

	 nSx = *nStartX;
	 nSy = *nStartY;

	 /* 定位 Point 並修改目前標記值245 -> 244 代表已經處理過 */
	 pMergeImage[nSy][nSx] = nV2;
	 nCurrent_Y_Valleys = nSy;//保存 nV1 標記所在的 Y 的位置
	 nFromBlock_X = nSx / OneBlockSize;//保存 nV1 標記所在的 Block X 資訊
	 nFromBlock_Y = nSy / OneBlockSize;//保存 nV1 標記所在的 Block Y 資訊
	 //sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Left pMergeImage[%d][%d] = %d / Block (%d, %d) / Block(%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize, nFromBlock_X, nFromBlock_Y);
	 //LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向左尋找下一點 */
		for (i = nSx; i >= 0; ) {
			i = i - 1;	//將 X 座標左移
			if (i < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				//if (nSy + SearchRange[nIDx] > nHeight - 1 || nSy + SearchRange[nIDx] < 0) {
				//	bIsEdge = true;//超過 Y 方向的上下邊界限制，可不繼續做下去
				//	break;
				//}
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記255
				if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == V1) {
						pMergeImage[j][i] = V2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nV1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, (j - nCurrent_Y_Valleys) * (-1));
					//LOGD(buffer);
					pMergeImage[j][i] = nV2;//更改標記為244代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
					//LOGD(buffer);

					nCurrent_Y_Valleys = j;//更新符 nV1 條件所在的 Y 位置
					nFromBlock_X = i / OneBlockSize;//更新符 nV1 條件所在的 Block X 區域
					nFromBlock_Y = j / OneBlockSize;//更新符 nV1 條件所在的 Block Y 區域
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Update] Block (%d, %d) / Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
					//LOGD(buffer);

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == D2 || pMergeImage[j][i] == (V2 - 100) || pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == (V2 - 100)) {
							if (nDoubleFirst == 1) {
								if(bIsEdge == true)
									pMergeImage[j][i] = V2;
								else
									pMergeImage[j][i] = V4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = V2;
								else
									pMergeImage[j][i] = V3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nV2 || pMergeImage[j][i] == (nV2 - 100)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, (j - nCurrent_Y_Valleys) * (-1));
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / [Last] Y Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_Y_Valleys);
						//LOGD(buffer);

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find *** (nLastNodeX = %d / nLastNodeY = %d) \r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}
				
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nValleysEnding;//標記為 Valleys Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V2 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V3 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == V4 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nV2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 100;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i >= 0; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)
}

void CheckBlockBadValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pOrientationInfo == NULL || pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;   //分岔標記號
	unsigned char V4 = 252, D4 = 82;   //第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	int nCurrent_X_Valleys = 0, nCurrent_Y_Valleys = 0, nFromBlock_X = -1, nFromBlock_Y = -1;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值245 -> 244 代表已經處理過 */
	pMergeImage[nSy][nSx] = nV2;
	nCurrent_X_Valleys = nSx;//保存 nV1 標記所在的 X 的位置
	nFromBlock_X = nSx / OneBlockSize;//保存 nV1 標記所在的 Block X 資訊
	nFromBlock_Y = nSy / OneBlockSize;//保存 nV1 標記所在的 Block Y 資訊
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Down pMergeImage[%d][%d] = %d / Block (%d, %d) / Block(%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize, nFromBlock_X, nFromBlock_Y);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向下尋找下一點 */
		for (j = nSy; j < nHeight - 1; ) {
			j = j + 1;	//將 Y 座標下移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				//if (nSx + SearchRange[nIDx] > nWidth - 1)
				//	i = nWidth - 1;
				//else if (nSx + SearchRange[nIDx] < 0)
				//	i = 0;
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記255
				if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == V1) {
						pMergeImage[j][i] = V2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nV1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, i - nCurrent_X_Valleys);
					//LOGD(buffer);
					pMergeImage[j][i] = nV2;//更改標記為244代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
					//LOGD(buffer);

					nCurrent_X_Valleys = i;//更新符 nV1 條件所在的 X 位置
					nFromBlock_X = i / OneBlockSize;//更新符 nV1 條件所在的 Block X 區域
					nFromBlock_Y = j / OneBlockSize;//更新符 nV1 條件所在的 Block Y 區域
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Update] Block (%d, %d) / X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
					//LOGD(buffer);

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == D2 || pMergeImage[j][i] == (V2 - 100) || pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == (V2 - 100)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = V4;
							}
							else {
								pMergeImage[j][i] = V3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D4;
							}
							else {
								pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nV2 || pMergeImage[j][i] == (nV2 - 100)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, i - nCurrent_X_Valleys);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / [Last] X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
						//LOGD(buffer);

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d /nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nValleysEnding;//標記為 Valleys Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nV2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 100;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j < nHeight - 1; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);
	
	/* 初始化變數 */
	bNotFind = false;
	bPriority = false;
	bBxPriority = false;
	bBxFinish = false;
	bBlockLimit = false;
	bIsEdge = false;
	nBxPriorityIDx = -1;
	nCurrentIDx = 0;
	nDoubleFirst = 0;
	nPriorityIDx = -1;
	nContinueNotFindIDxCnt = 0;
	nCurrent_X_Valleys = 0;
	nCurrent_Y_Valleys = 0;
	nFromBlock_X = -1;
	nFromBlock_Y = -1;
	nLastNodeX = -1;
	nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值245 -> 244 代表已經處理過 */
	pMergeImage[nSy][nSx] = nV2;
	nCurrent_X_Valleys = nSx;//保存 nV1 標記所在的 X 的位置
	nFromBlock_X = nSx / OneBlockSize;//保存 nV1 標記所在的 Block X 資訊
	nFromBlock_Y = nSy / OneBlockSize;//保存 nV1 標記所在的 Block Y 資訊
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Up pMergeImage[%d][%d] = %d / Block (%d, %d) / Block(%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize, nFromBlock_X, nFromBlock_Y);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向上尋找下一點 */
		for (j = nSy; j >= 0; ) {
			j = j - 1;	//將 Y 座標上移
			if (j < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				//if (nSx + SearchRange[nIDx] > nWidth - 1)
				//	i = nWidth - 1;
				//else if (nSx + SearchRange[nIDx] < 0)
				//	i = 0;
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記255
				if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == V1) {
						pMergeImage[j][i] = V2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nV1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, (i - nCurrent_X_Valleys) * (-1));
					//LOGD(buffer);
					pMergeImage[j][i] = nV2;//更改標記為244代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
					//LOGD(buffer);

					nCurrent_X_Valleys = i;//更新符 nV1 條件所在的 X 位置
					nFromBlock_X = i / OneBlockSize;//更新符 nV1 條件所在的 Block X 區域
					nFromBlock_Y = j / OneBlockSize;//更新符 nV1 條件所在的 Block Y 區域
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Update] Block (%d, %d) / X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
					//LOGD(buffer);

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == D2 || pMergeImage[j][i] == (V2 - 100) || pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == V2 || pMergeImage[j][i] == (V2 - 100)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = V4;
							}
							else {
								pMergeImage[j][i] = V3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D4;
							}
							else {
								pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nV2 || pMergeImage[j][i] == (nV2 - 100)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) / [%d] *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize, (i - nCurrent_X_Valleys) * (-1));
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** [Last] Block (%d, %d) / [Last] X Value [%d] *** \r\n", __FUNCTION__, __LINE__, nFromBlock_X, nFromBlock_Y, nCurrent_X_Valleys);
						//LOGD(buffer);

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nValleysEnding;//標記為 Valleys Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == V4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nV2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 100;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j >= 0; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)
}

void ReCheckValleysEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;
	
	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy, nBlockType = -1;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nSx = i;
					nSy = j;
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d) / Block(%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSx / OneBlockSize, nSy / OneBlockSize, nBlockType);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD)
						ReCheckGoodValleysEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_GOOD);
					else
						ReCheckBadValleysEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_BAD);

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
					//LOGD(buffer);

				}//if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2))
			}//if (pMergeImage[j][i] == nValleysEnding)
		}//for (i = 0; i < nWidth; i++)
	}//for (j = 0; j < nHeight; j++)
}

void ReCheckGoodValleysEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nNotFindCnt = 0, nLenCnt = 0, nIDx = 0, nCurrentIDx = 0, nPriorityIDx = -1, nBlockType = -1, nLenPart1 = 0;
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;   //分岔標記號
	unsigned char V4 = 252, D4 = 82;   //第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bPriority = false, bLength = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向右尋找下一點 */
	for (ii = nSx; ii < nWidth; ) {
		ii = ii + 1; //將 X 座標右移
		if (ii == nWidth)
			break;

		for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
			if (nSy + SearchRange[nIDx] > nHeight - 1) {
				jj = nHeight - 1;
				bIsEdge = true;//超過 Y 方向的下邊界限制
			}
			else if (nSy + SearchRange[nIDx] < 0) {
				jj = 0;
				bIsEdge = true;//超過 Y 方向的上邊界限制
			}
			else
				jj = nSy + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_GOOD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Good限制線就停止不做
					if (jj >= nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}//if (nBlockType == EMC_BLOCK_GOOD)

			if (pMergeImage[jj][ii] == V1 ||
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100)) {

				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100)) {
					bPriority = true;
					nPriorityIDx = nIDx;//優先
				}
				if (bFind == false)
					bFind = true;
				nCurrentIDx = nIDx;//保存最後符合條件的Index
			}
			else if (pMergeImage[jj][ii] == R1 ||
					 pMergeImage[jj][ii] == E0) {
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
				//LOGD(buffer);
				continue;
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
				//LOGD(buffer);
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
				//LOGD(buffer);
			}
		}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

		if (nNotFindCnt == SEARCH_SIZE) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		//超過 Y 的上下邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
		}

		/* 更新最後找到且符合條件的IDx 並清除 R1/E0 旗標計數器 */
		nNotFindCnt = 0;
		if (bPriority == true) {
			bPriority = false;
			nCurrentIDx = nPriorityIDx;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [Priority First] nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
			//LOGD(buffer);
			nPriorityIDx = -1;
		}

		if (bFind == true) {
			bFind = false;
			nLenCnt++;         //長度計數器累加
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
			//LOGD(buffer);

			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
			//LOGD(buffer);

			//更新下次的 Y 座標
			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
			//LOGD(buffer);
		}//if (bFind == true)

	}//for (ii = nSx; ii < nWidth; )

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nLenCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nLenCnt, i, j);
	//LOGD(buffer);

	if (nLenCnt > nLenTh) {
		bLength = true;
	}

	if (bLength == false) {
		//保存該階段的長度數據
		nLenPart1 = nLenCnt;

		//清除長度計數器
		nLenCnt = 0;
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);

		//為滿足長度門檻，需要繼續往另一方向計數長度
		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bPriority = false;
		nCurrentIDx = 0;
		nPriorityIDx = -1;
		nNotFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向左尋找下一點 */
		for (ii = nSx; ii >= 0; ) {
			ii = ii - 1;	//將 X 座標左移
			if (ii < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100)) {

					if (pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V2 ||
						pMergeImage[jj][ii] == V3 ||
						pMergeImage[jj][ii] == V4 ||
						pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV1 ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100)) {
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存最後符合條件的Index
				}
				else if (pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
					//LOGD(buffer);
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			/* 更新最後找到且符合條件的IDx 並清除 R1/E0 旗標計數器 */
			nNotFindCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [Priority First] nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;
			}

			if (bFind == true) {
				bFind = false;
				nLenCnt++;         //長度計數器累加
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
				//LOGD(buffer);

				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				//LOGD(buffer);

				//更新下次的 Y 座標
				nSy = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

		}//for (ii = nSx; ii >= 0; )

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX nLenCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nLenCnt, i, j);
		//LOGD(buffer);

		if (nLenCnt + nLenPart1 > nLenTh) {
			bLength = true;
		}

		if (bLength == false) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			//LOGD(buffer);

			//將長度門檻未達標準的變更旗標為255
			pMergeImage[j][i] = V1;
		}//if (bLength == false)

	}//if (bLength == false)

	 //清除該階段的長度數據
	nLenPart1 = 0;

	//清除長度計數器
	nLenCnt = 0;

	if(bLength == true)
		bLength = false;
}

void ReCheckBadValleysEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nNotFindCnt = 0, nLenCnt = 0, nIDx = 0, nCurrentIDx = 0, nPriorityIDx = -1, nBlockType = -1, nLenPart1 = 0;
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;   //分岔標記號
	unsigned char V4 = 252, D4 = 82;   //第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bPriority = false, bLength = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向下尋找下一點 */
	for (jj = nSy; jj < nHeight; ) {
		jj = jj + 1;	//將 Y 座標下移
		if (jj == nHeight)
			break;

		for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
			if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
				bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
				break;
			}
			else
				ii = nSx + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_BAD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Bad限制線就停止不做
					if (jj < nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}

			if (pMergeImage[jj][ii] == V1 ||
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100)) {

				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100)) {
					bPriority = true;
					nPriorityIDx = nIDx;//優先
				}
				if (bFind == false)
					bFind = true;
				nCurrentIDx = nIDx;//保存最後符合條件的Index
			}
			else if (pMergeImage[jj][ii] == R1 ||
					 pMergeImage[jj][ii] == E0) {
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
				//LOGD(buffer);
				continue;
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
				//LOGD(buffer);
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
				//LOGD(buffer);
			}
		}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

		if (nNotFindCnt == SEARCH_SIZE) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		//超過 X 的左右邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
			break;
		}

		/* 更新最後找到且符合條件的IDx 並清除 R1/E0 旗標計數器 */
		nNotFindCnt = 0;
		if (bPriority == true) {
			bPriority = false;
			nCurrentIDx = nPriorityIDx;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [Priority First] nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
			//LOGD(buffer);
			nPriorityIDx = -1;
		}

		if (bFind == true) {
			bFind = false;
			nLenCnt++;         //長度計數器累加
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
			//LOGD(buffer);

			int nUpdateIDx = 0;
			if (nSx + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSx + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSx + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			//更新下次的 X 座標
			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
			//LOGD(buffer);
		}//if (bFind == true)

	}//for (jj = nSy; jj < nHeight; )

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nLenCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nLenCnt, i, j);
	//LOGD(buffer);

	if (nLenCnt > nLenTh) {
		bLength = true;
	}

	if (bLength == false) {
		//保存該階段的長度數據
		nLenPart1 = nLenCnt;

		//清除長度計數器
		nLenCnt = 0;
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);

		//為滿足長度門檻，需要繼續往另一方向計數長度
		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bPriority = false;
		nCurrentIDx = 0;
		nPriorityIDx = -1;
		nNotFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向上尋找下一點 */
		for (jj = nSy; jj >= 0; ) {
			jj = jj - 1;	//將 Y 座標上移
			if (jj < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100)) {

					if (pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V2 ||
						pMergeImage[jj][ii] == V3 ||
						pMergeImage[jj][ii] == V4 ||
						pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV1 ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100)) {
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存最後符合條件的Index
				}
				else if (pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
					//LOGD(buffer);
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除 R1/E0 旗標計數器 */
			nNotFindCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [Priority First] nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;
			}

			if (bFind == true) {
				bFind = false;
				nLenCnt++;         //長度計數器累加
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
				//LOGD(buffer);

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//更新下次的 X 座標
				nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

		}//for (jj = nSy; jj >= 0; )

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX nLenCnt = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nLenCnt, i, j);
		//LOGD(buffer);

		if (nLenCnt + nLenPart1 > nLenTh) {
			bLength = true;
		}

		if (bLength == false) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] XXXX (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			//LOGD(buffer);

			//將長度門檻未達標準的變更旗標為255
			pMergeImage[j][i] = V1;
		}//if (bLength == false)

	}//if (bLength == false)

	 //清除該階段的長度數據
	nLenPart1 = 0;

	//清除長度計數器
	nLenCnt = 0;

	if (bLength == true)
		bLength = false;
}

int GetValleysEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTextureTemplate == NULL)
		return -1;

	char buffer[1000] = { 0 };
	int i, j, nValidValleysCnt = 0, nIDx = 0;
	unsigned char nValleysEnding = 101;//Valleys Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nValidValleysCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValidValleysCnt = %d\r\n", __FUNCTION__, __LINE__, nValidValleysCnt);
	//LOGD(buffer);

	if (pTextureTemplate->pValleysEndingPoint == NULL && pTextureTemplate->nTotalValleysEnding == 0) {
		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}
		
		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				if (pMergeImage[j][i] == nValleysEnding) {
					if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
						pTextureTemplate->pValleysEndingPoint[nIDx].x = i;
						pTextureTemplate->pValleysEndingPoint[nIDx].y = j;
						nIDx++;
					}
				}
			}
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pValleysEndingPoint);
		pTextureTemplate->pValleysEndingPoint = NULL;
		pTextureTemplate->nTotalValleysEnding = 0;

		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				if (pMergeImage[j][i] == nValleysEnding) {
					if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
						pTextureTemplate->pValleysEndingPoint[nIDx].x = i;
						pTextureTemplate->pValleysEndingPoint[nIDx].y = j;
						nIDx++;
					}
				}
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nIDx = %d\r\n", __FUNCTION__, __LINE__, nIDx);
	LOGD(buffer);

	for (nIDx = 0; nIDx < pTextureTemplate->nTotalValleysEnding; nIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pValleysEndingPoint[nIDx].x, pTextureTemplate->pValleysEndingPoint[nIDx].y);
		LOGD(buffer);
	}
	return nValidValleysCnt;
}

int ComputeDirection(int x1, int y1, int x2, int y2, int nType)
{
	char buffer[1000] = { 0 };
	double nTheta = 0.0f, nDeltaTheta = 0.0f;

	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, x1, y1, x2, y2);
	LOGD(buffer);
	if (nType == EMC_BLOCK_GOOD) {
		nDeltaTheta = (double) (y2 - y1) / (x2 - x1);
		nTheta = (double) atan(nDeltaTheta) * 180 / PI;
		//nTheta = (double) atan2((y2 - y1), (x2 - x1)) * 180 / PI;
		if (nTheta < 0)
			nTheta = 360 + nTheta;
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nDeltaTheta = %f / nTheta = %f / nTheta = %d\r\n", __FUNCTION__, __LINE__, nDeltaTheta, nTheta, (int)(nTheta + 0.5));
		LOGD(buffer);
	}
	else {
		nDeltaTheta = (double) (y2 - y1) / (x2 - x1);
		nTheta = (double) atan(nDeltaTheta) * 180 / PI;
		//nTheta = (double) atan2((y2 - y1), (x2 - x1)) * 180 / PI;
		if (nTheta < 0)
			nTheta = 180 + nTheta;
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nDeltaTheta = %f / nTheta = %f / nTheta = %d\r\n", __FUNCTION__, __LINE__, nDeltaTheta, nTheta, (int)(nTheta + 0.5));
		LOGD(buffer);
	}
	return (int) (nTheta + 0.5);
}

int ValleysEndingDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return -400;

//	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nIDx, nCurrentIDx, nBlockType = -1, nNotFindCnt = 0, nLenCnt = 0, nDir = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char R1 = 0, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	if (nBlockType == EMC_BLOCK_GOOD) {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d nBlockType = %d\r\n", __FUNCTION__, __LINE__, nBlockType);
//		LOGD(buffer);

		/* 向右搜尋 */
		for (ii = nSx; ii < nWidth; ) {
			ii = ii + 1; //將 X 座標右移
			if (ii == nWidth)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);
			
				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == (D1 - 10) ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			if (bFind == true) {
				bFind = false; 
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				//LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 Y 座標
				if(bLength == false)
					nSy = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
//				LOGD(buffer);
				//nDir = ComputeDirection(i, j, ii, nSy + SearchRange[nCurrentIDx], nBlockType);
				nDir = xVFComputeLineDirection(i, j, ii, nSy + SearchRange[nCurrentIDx]);
				break;
			}
		}//for (ii = nSx; ii < nWidth; )

		if (bNotFind == true) {
			/* 向右搜尋沒有找到時，需要繼續向左搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向左尋找下一點 */
			for (ii = nSx; ii >= 0; ) {
				ii = ii - 1;	//將 X 座標左移
				if (ii < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSy + SearchRange[nIDx] > nHeight - 1) {
						jj = nHeight - 1;
						bIsEdge = true;//超過 Y 方向的下邊界限制
					}
					else if (nSy + SearchRange[nIDx] < 0) {
						jj = 0;
						bIsEdge = true;//超過 Y 方向的上邊界限制
					}
					else
						jj = nSy + SearchRange[nIDx];

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Good限制線就停止不做
							if (jj >= nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_GOOD)

					if (pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100) ||
						pMergeImage[jj][ii] == (D1 - 10) ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
							 pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == D1 ||
							 pMergeImage[jj][ii] == D2 ||
							 pMergeImage[jj][ii] == D3 ||
							 pMergeImage[jj][ii] == D4 ||
							 pMergeImage[jj][ii] == R1 ||
							 pMergeImage[jj][ii] == E0) {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						//LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						//LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					//LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					break;
				}

				//超過 Y 的上下邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSy + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
					//LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 Y 座標
					if (bLength == false)
						nSy = nSy + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
					//LOGD(buffer);
				}//if (bFind == true)

				if (bLength == true) {
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
//					LOGD(buffer);
					//nDir = ComputeDirection(ii, nSy + SearchRange[nCurrentIDx], i, j, nBlockType);
					nDir = xVFComputeLineDirection(ii, nSy + SearchRange[nCurrentIDx], i, j);
					break;
				}
			}//for (ii = nSx; ii >= 0; )
		}//if (bNotFind == true)
	}
	else {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d nBlockType = %d\r\n", __FUNCTION__, __LINE__, nBlockType);
//		LOGD(buffer);

		/* 向下尋找下一點 */
		for (jj = nSy; jj < nHeight; ) {
			jj = jj + 1;	//將 Y 座標下移
			if (jj == nHeight)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_BAD)

				if (pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == (D1 - 10) ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 X 座標
				if (bLength == false)
					nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
//				LOGD(buffer);
				//nDir = ComputeDirection(i, j, nSx + SearchRange[nCurrentIDx], jj, nBlockType);
				nDir = xVFComputeLineDirection(i, j, nSx + SearchRange[nCurrentIDx], jj);
				break;
			}
		}//for (jj = nSy; jj < nHeight; )

		if (bNotFind == true) {
			/* 向下搜沒有找到時，需要繼續向上搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向上尋找下一點 */
			for (jj = nSy; jj >= 0; ) {
				jj = jj - 1;	//將 Y 座標上移
				if (jj < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
						bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
						break;
					}
					else
						ii = nSx + SearchRange[nIDx];

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_BAD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Bad限制線就停止不做
							if (jj < nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_BAD)

					if (pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100) ||
						pMergeImage[jj][ii] == (D1 - 10) ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
							 pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == D1 ||
							 pMergeImage[jj][ii] == D2 ||
							 pMergeImage[jj][ii] == D3 ||
							 pMergeImage[jj][ii] == D4 ||
							 pMergeImage[jj][ii] == R1 ||
							 pMergeImage[jj][ii] == E0) {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						//LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						//LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					//LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					break;
				}

				//超過 X 的左右邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
					break;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSx + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
					//LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 X 座標
					if (bLength == false)
						nSx = nSx + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
					//LOGD(buffer);
				}//if (bFind == true)

				if (bLength == true) {
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
//					LOGD(buffer);
					//nDir = ComputeDirection(nSx + SearchRange[nCurrentIDx], jj, i, j, nBlockType);
					nDir = xVFComputeLineDirection(nSx + SearchRange[nCurrentIDx], jj, i, j);
					break;
				}
			}//for (jj = nSy; jj >= 0; )
		}//if (bNotFind == true)
	}
	return nDir;
}

int GetValleysDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return -1;

//	char buffer[1000] = { 0 };
	int i, j, nIDx = 0, nBlockType = -1, nValleysCnt = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nValleysEnding = 101;//Valleys Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nValleysCnt++;
				}
			}
		}
	}
//	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValleysCnt = %d\r\n", __FUNCTION__, __LINE__, nValleysCnt);
//	LOGD(buffer);

	if (pValleysDirection != NULL) {
		free(pValleysDirection);
		pValleysDirection = NULL;
		nValleysDirectionCnt = -1;
	}
	
	nValleysDirectionCnt = nValleysCnt;
	if (nValleysCnt > 0) {
		pValleysDirection = (short *)malloc(sizeof(short) * nValleysCnt);
		memset(pValleysDirection, 0, sizeof(short) * nValleysCnt);
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;

//					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, i, j, nBlockType);
//					LOGD(buffer);
					pValleysDirection[nIDx] = ValleysEndingDirection(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, nBlockType);
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pValleysDirection[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, pValleysDirection[nIDx]);
//					LOGD(buffer);
					nIDx++;
				}
			}
		}
	}
//	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]XXXXXXXXXXXXXXXXXXXXXXX\r\n", __FUNCTION__, __LINE__);
//	LOGD(buffer);
	return 0;
}

int RidgesEndingDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return -400;

//	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nIDx, nCurrentIDx, nBlockType = -1, nNotFindCnt = 0, nLenCnt = 0, nDir = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1, D5 = 86;
	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號

	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	if (nBlockType == EMC_BLOCK_GOOD) {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d nBlockType = %d\r\n", __FUNCTION__, __LINE__, nBlockType);
//		LOGD(buffer);

		/* 向右搜尋 */
		for (ii = nSx; ii < nWidth; ) {
			ii = ii + 1; //將 X 座標右移
			if (ii == nWidth)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == (V2 - 100) ||
						 pMergeImage[jj][ii] == (V3 - 100) ||
						 pMergeImage[jj][ii] == (V4 - 100) ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == nV2 ||
						 pMergeImage[jj][ii] == (nV2 - 100) ||
						 pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == R2 ||
						 pMergeImage[jj][ii] == R3 ||
						 pMergeImage[jj][ii] == R4 ||
						 pMergeImage[jj][ii] == nR1 ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == (D2 - 10) ||
						 pMergeImage[jj][ii] == (D3 - 10) ||
					 	 pMergeImage[jj][ii] == (D4 - 10) ||
						 pMergeImage[jj][ii] == D5 ||
						 pMergeImage[jj][ii] == D6 ||
						 pMergeImage[jj][ii] == D7 ||
						 pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			if (bFind == true) {
				bFind = false;
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				//LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 Y 座標
				if (bLength == false)
					nSy = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
//				LOGD(buffer);
				//nDir = ComputeDirection(i, j, ii, nSy + SearchRange[nCurrentIDx], nBlockType);
				nDir = xVFComputeLineDirection(i, j, ii, nSy + SearchRange[nCurrentIDx]);
				break;
			}
		}//for (ii = nSx; ii < nWidth; )

		if (bNotFind == true) {
			/* 向右搜尋沒有找到時，需要繼續向左搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向左尋找下一點 */
			for (ii = nSx; ii >= 0; ) {
				ii = ii - 1;	//將 X 座標左移
				if (ii < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSy + SearchRange[nIDx] > nHeight - 1) {
						jj = nHeight - 1;
						bIsEdge = true;//超過 Y 方向的下邊界限制
					}
					else if (nSy + SearchRange[nIDx] < 0) {
						jj = 0;
						bIsEdge = true;//超過 Y 方向的上邊界限制
					}
					else
						jj = nSy + SearchRange[nIDx];

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Good限制線就停止不做
							if (jj >= nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_GOOD)

					if (pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V2 ||
						pMergeImage[jj][ii] == V3 ||
						pMergeImage[jj][ii] == V4 ||
						pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV1 ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100) ||
						pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == E0) {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						//LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						//LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					//LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					break;
				}

				//超過 Y 的上下邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSy + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
					//LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 Y 座標
					if (bLength == false)
						nSy = nSy + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
					//LOGD(buffer);
				}//if (bFind == true)

				if (bLength == true) {
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
//					LOGD(buffer);
					//nDir = ComputeDirection(ii, nSy + SearchRange[nCurrentIDx], i, j, nBlockType);
					nDir = xVFComputeLineDirection(ii, nSy + SearchRange[nCurrentIDx], i, j);
					break;
				}
			}//for (ii = nSx; ii >= 0; )
		}//if (bNotFind == true)
	}
	else {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d nBlockType = %d\r\n", __FUNCTION__, __LINE__, nBlockType);
//		LOGD(buffer);

		/* 向下尋找下一點 */
		for (jj = nSy; jj < nHeight; ) {
			jj = jj + 1;	//將 Y 座標下移
			if (jj == nHeight)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_BAD)

				if (pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == E0) {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					//LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 X 座標
				if (bLength == false)
					nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
//				LOGD(buffer);
				//nDir = ComputeDirection(i, j, nSx + SearchRange[nCurrentIDx], jj, nBlockType);
				nDir = xVFComputeLineDirection(i, j, nSx + SearchRange[nCurrentIDx], jj);
				break;
			}
		}//for (jj = nSy; jj < nHeight; )

		if (bNotFind == true) {
			/* 向下搜沒有找到時，需要繼續向上搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向上尋找下一點 */
			for (jj = nSy; jj >= 0; ) {
				jj = jj - 1;	//將 Y 座標上移
				if (jj < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
						bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
						break;
					}
					else
						ii = nSx + SearchRange[nIDx];

					//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_BAD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Bad限制線就停止不做
							if (jj < nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_BAD)

					if (pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V2 ||
						pMergeImage[jj][ii] == V3 ||
						pMergeImage[jj][ii] == V4 ||
						pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV1 ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100) ||
						pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == E0) {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						//LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						//LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					//LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					break;
				}

				//超過 X 的左右邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
					break;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
//					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSx + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
					//LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 X 座標
					if (bLength == false)
						nSx = nSx + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
					//LOGD(buffer);
				}//if (bFind == true)

				if (bLength == true) {
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
//					LOGD(buffer);
					//nDir = ComputeDirection(nSx + SearchRange[nCurrentIDx], jj, i, j, nBlockType);
					nDir = xVFComputeLineDirection(nSx + SearchRange[nCurrentIDx], jj, i, j);
					break;
				}
			}//for (jj = nSy; jj >= 0; )
		}//if (bNotFind == true)
	}
	return nDir;
}

int GetRidgesDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return -1;

//	char buffer[1000] = { 0 };
	int i, j, nIDx = 0, nBlockType = -1, nRidgesCnt = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nRidgesEnding = 111;//Ridges Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nRidgesCnt++;
				}
			}
		}
	}
//	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgesCnt = %d\r\n", __FUNCTION__, __LINE__, nRidgesCnt);
//	LOGD(buffer);

	if (pRidgesDirection != NULL) {
		free(pRidgesDirection);
		pRidgesDirection = NULL;
		nRidgesDirectionCnt = -1;
	}

	nRidgesDirectionCnt = nRidgesCnt;
	if (nRidgesCnt > 0) {
		pRidgesDirection = (short *)malloc(sizeof(short) * nRidgesCnt);
		memset(pRidgesDirection, 0, sizeof(short) * nRidgesCnt);
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;

//					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, i, j, nBlockType);
//					LOGD(buffer);
					pRidgesDirection[nIDx] = RidgesEndingDirection(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, nBlockType);
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pRidgesDirection[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, pRidgesDirection[nIDx]);
					//LOGD(buffer);
					nIDx++;
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]XXXXXXXXXXXXXXXXXXXXXXX\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);
	return 0;
}

void RecoverData(unsigned char **pMergeImage_In, int nWidth, int nHeight)
{
	if (pMergeImage_In == NULL)
		return;

	int i, j;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84;
	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage_In[j][i] == V2 || 
				pMergeImage_In[j][i] == V3 || 
				pMergeImage_In[j][i] == V4 ||
				pMergeImage_In[j][i] == (V2 - 100) ||
				pMergeImage_In[j][i] == (V3 - 100) ||
				pMergeImage_In[j][i] == (V4 - 100)) {
				pMergeImage_In[j][i] = V1;
			}

			if (pMergeImage_In[j][i] == D2 || 
				pMergeImage_In[j][i] == D3 || 
				pMergeImage_In[j][i] == D4 ||
				pMergeImage_In[j][i] == (D2 - 10) ||
				pMergeImage_In[j][i] == (D3 - 10) ||
				pMergeImage_In[j][i] == (D4 - 10)) {
				pMergeImage_In[j][i] = D1;
			}

			if (pMergeImage_In[j][i] == nV2 || pMergeImage_In[j][i] == (nV2 - 100)) {
				pMergeImage_In[j][i] = nV1;
			}
		}
	}
}

void GetValidRidgeData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTexture == NULL || pHorizontalBlockInfoData == NULL || pVerticalBlockInfoData == NULL)
		return;

	int i, /*j, jj, */ n, x, y;
	unsigned char nR1 = 10;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	//signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) };	 //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };	 //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	//unsigned char HorizontalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };//保存水平方向 Row 的無效區域總數
	//unsigned char HorizontalInValidCol[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };//保存水平方向 Col 的無效區域總數

#if 0
	/*尋找水平方向的分界線位置*/
	/*1.紀錄Row方向無效區數量*/
	int nBadRowCnt = 0, nBadColCnt = 0;
	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
		for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
			if (pHorizontalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD)
				nBadRowCnt++;
		}
		HorizontalInValidRow[j] = nBadRowCnt;//紀錄Row方向的無效區域總數
		nBadRowCnt = 0;
	}

	/*2.紀錄Col方向無效區數量*/
	for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
		if (HorizontalInValidRow[j] == 1 || HorizontalInValidRow[j] == 2) {
			for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
				if (pHorizontalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
					for (jj = 0; jj < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; jj++) {
						if (pHorizontalBlockInfoData->EdgeStatus[jj][i] == EMC_BLOCK_BAD) {
							nBadColCnt++;
						}
						if (HorizontalInValidRow[jj] == 0)//代表這一列沒有Bad Block，用此行來過濾與判斷
							break;
					}
					if (j <= jj) {
						HorizontalInValidCol[i] = nBadColCnt;//紀錄Col方向的無效區域總數
						nBadColCnt = 0;
					}
				}
			}
		}
	}

	int nMax = 0, nColIDx = 0;
	nMax = HorizontalInValidCol[0];
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		if (HorizontalInValidCol[i] >= nMax) {
			nMax = HorizontalInValidCol[i];
			nColIDx = i;
		}
	}

	/*尋找垂直方向的分界線位置*/
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		for (j = 0; j < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; j++) {
			if (pVerticalBlockInfoData->EdgeStatus[j][i] == EMC_BLOCK_BAD) {
				VerticalInValidRow[i] = j;//紀錄無效區域的Y位置
				break;
			}
		}
	}
#endif

	/*尋找垂直方向的分界線位置*/
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		VerticalInValidRow[i] = g_nRow_Y;//紀錄無效區域的Y位置
	}

	for (n = 0; n < pTexture->nTotalBoundRidge; n++) {
		if (pTexture->pBoundRidge[n].valid == true) {
			x = pTexture->pBoundRidge[n].x;
			y = pTexture->pBoundRidge[n].y;
			pMergeImage[y][x] = nR1;
		}
	}
	//RecoverData(pMergeImage, nWidth, nHeight);
	//SaveTXT("xxMergerImage.txt", pMergeImage, nWidth, nHeight);

#if __USE_RIDGE_SEARCH__
	SearchRidgeStart(pMergeImage, VerticalInValidRow, g_nCol_X /*nColIDx*/, nWidth, nHeight);
#else
	CheckValleysBranchSearchRidges(pMergeImage, VerticalInValidRow, nWidth, nHeight);
#endif // __USE_RIDGE_SEARCH__
	//SaveTXT("xxxMergerImage.txt", pMergeImage, nWidth, nHeight);

	GetRidgesDirection(pMergeImage, VerticalInValidRow, nWidth, nHeight);//Get Valid Ridges Direction
}

void SearchRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nStartCols, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nStartX, nStartY;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nR1 = 10;
	
	if (nStartCols == -1)
		nStartCols = (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) / 2;

	//Block向右尋找
	for (i = nStartCols; i < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); i++) {
		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			if (j >= VerticalInValidRow[i] && VerticalInValidRow[i] != -1) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToRight Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (j == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nR1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								//_CheckBlockBadRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
								CheckBlockBadRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							}
						}
					}
				}
				else {
					for (ii = 0; ii < OneBlockSize; ii++) {
						if (pMergeImage[j * OneBlockSize][i * OneBlockSize + ii] == nR1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize);
							//LOGD(buffer);
							nStartX = i * OneBlockSize + ii;
							nStartY = j * OneBlockSize;
							//_CheckBlockBadRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							CheckBlockBadRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
						}
					}
				}
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToRight Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (i == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nR1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								//_CheckBlockGoodRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
								CheckBlockGoodRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							}
						}
					}
				}
				else {
					for (jj = 0; jj < OneBlockSize; jj++) {
						if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize] == nR1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize + jj);
							//LOGD(buffer);
							nStartX = i * OneBlockSize;
							nStartY = j * OneBlockSize + jj;
							//_CheckBlockGoodRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							CheckBlockGoodRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
						}
					}
				}
			}
		}
	}

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d \r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d *********************************************************************************\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	//Block向左尋找
	for (i = nStartCols - 1; i >= 0; i--) {
		for (j = 0; j < (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE); j++) {
			if (j >= VerticalInValidRow[i] && VerticalInValidRow[i] != -1) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToLeft Block Bad (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (j == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (jj = 0; jj < OneBlockSize; jj++) {
						for (ii = 0; ii < OneBlockSize; ii++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nR1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								//_CheckBlockBadRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
								CheckBlockBadRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							}
						}
					}
				}
				else {
					for (ii = 0; ii < OneBlockSize; ii++) {
						if (pMergeImage[j * OneBlockSize][i * OneBlockSize + ii] == nR1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize);
							//LOGD(buffer);
							nStartX = i * OneBlockSize + ii;
							nStartY = j * OneBlockSize;
							//_CheckBlockBadRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
							CheckBlockBadRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_BAD);
						}
					}
				}
			}
			else {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d ToLeft Block Good (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize);
				//LOGD(buffer);
				if (i == (EMC_SENSOR_SIZE / EMC_BLOCK_SIZE) - 1) {
					for (ii = 0; ii < OneBlockSize; ii++) {
						for (jj = 0; jj < OneBlockSize; jj++) {
							if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize + ii] == nR1) {
								//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize + ii, j * OneBlockSize + jj);
								//LOGD(buffer);
								nStartX = i * OneBlockSize + ii;
								nStartY = j * OneBlockSize + jj;
								//_CheckBlockGoodRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
								CheckBlockGoodRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							}
						}
					}
				}
				else {
					for (jj = 0; jj < OneBlockSize; jj++) {
						if (pMergeImage[j * OneBlockSize + jj][i * OneBlockSize] == nR1) {
							//sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, i * OneBlockSize, j * OneBlockSize + jj);
							//LOGD(buffer);
							nStartX = i * OneBlockSize;
							nStartY = j * OneBlockSize + jj;
							//_CheckBlockGoodRidgeStart_(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
							CheckBlockGoodRidgeStart(pMergeImage, VerticalInValidRow, &nStartX, &nStartY, nWidth, nHeight, EMC_BLOCK_GOOD);
						}
					}
				}	
			}
		}
	}

	//Re-Check and Update Valid Valleys Data
	ReCheckRidgesEnding(pMergeImage, VerticalInValidRow, nWidth, nHeight);
}

void _CheckBlockGoodRidgeStart_(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1;

	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	//unsigned char nValleysEnding = 101;//Valleys Ending標記號

	unsigned char R3 = 2;				//Ridges分岔標記號
	unsigned char R4 = 3;				//Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Right pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向右尋找下一點 */
		for (i = nSx; i < nWidth - 1; ) {
			i = i + 1;	//將 X 座標右移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合 Ridges 的標記 0
				if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有先後順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == E0 || pMergeImage[j][i] == nV1) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 || 
						pMergeImage[j][i] == D2 || 
						pMergeImage[j][i] == (R2 + 50) || 
						pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R2 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R3 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R4 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nR2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] + 50;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i < nWidth - 1; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/* 初始化變數 */
	bNotFind = false;
	bPriority = false;
	bBxPriority = false;
	bBxFinish = false;
	bBlockLimit = false;
	bIsEdge = false;
	nBxPriorityIDx = -1;
	nCurrentIDx = 0;
	nDoubleFirst = 0;
	nPriorityIDx = -1;
	nContinueNotFindIDxCnt = 0;
	nLastNodeX = -1;
	nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Left pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向左尋找下一點 */
		for (i = nSx; i >= 0; ) {
			i = i - 1;	//將 X 座標左移
			if (i < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合 Ridges 的標記 0
				if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == E0 || pMergeImage[j][i] == nV1) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 || 
						pMergeImage[j][i] == D2 || 
						pMergeImage[j][i] == (R2 + 50) || 
						pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D2;
								else
									pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find *** (nLastNodeX = %d / nLastNodeY = %d) \r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R2 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R3 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R4 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nR2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] + 50;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i >= 0; )
	}//if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)
}

void _CheckBlockBadRidgeStart_(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1;

	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	//unsigned char nValleysEnding = 101;//Valleys Ending標記號

	unsigned char R3 = 2;			   //Ridges分岔標記號
	unsigned char R4 = 3;			   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Down pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向下尋找下一點 */
		for (j = nSy; j < nHeight - 1; ) {
			j = j + 1;	//將 Y 座標下移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合 Ridges 的標記 1
				if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == E0 || pMergeImage[j][i] == nV1) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 || 
						pMergeImage[j][i] == D2 || 
						pMergeImage[j][i] == (R2 + 50) || 
						pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = R4;
							}
							else {
								pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D4;
							}
							else {
								pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d /nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nR2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] + 50;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j < nHeight - 1; )
	}//if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/* 初始化變數 */
	bNotFind = false;
	bPriority = false;
	bBxPriority = false;
	bBxFinish = false;
	bBlockLimit = false;
	bIsEdge = false;
	nBxPriorityIDx = -1;
	nCurrentIDx = 0;
	nDoubleFirst = 0;
	nPriorityIDx = -1;
	nContinueNotFindIDxCnt = 0;
	nLastNodeX = -1;
	nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Up pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向上尋找下一點 */
		for (j = nSy; j >= 0; ) {
			j = j - 1;	//將 Y 座標上移
			if (j < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記 0
				if (pMergeImage[j][i] == R1 || pMergeImage[j][i] == D1) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D2;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || pMergeImage[j][i] == E0 || pMergeImage[j][i] == nV1) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 || 
						pMergeImage[j][i] == D2 || 
						pMergeImage[j][i] == (R2 + 50) || 
						pMergeImage[j][i] == (D2 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = R4;
							}
							else {
								pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D4;
							}
							else {
								pMergeImage[j][i] = D3;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nR2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] + 50;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j >= 0; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)
}

void CheckBlockGoodRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1, D5 = 86;

	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	//unsigned char nValleysEnding = 101;//Valleys Ending標記號
	
	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Right pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)) {
		/* 向右尋找下一點 */
		for (i = nSx; i < nWidth - 1; ) {
			i = i + 1;	//將 X 座標右移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合 Ridges 的標記 0
				if (pMergeImage[j][i] == R1 ||
					pMergeImage[j][i] == D1 ||
					pMergeImage[j][i] == D2 ||
					pMergeImage[j][i] == D3 ||
					pMergeImage[j][i] == D4 ||
					pMergeImage[j][i] == (D2 - 10) ||
					pMergeImage[j][i] == (D3 - 10) ||
					pMergeImage[j][i] == (D4 - 10)) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D5;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) ***\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					// 保存該點為這輪最後的Node點
					//nLastNodeX = i;
					//nLastNodeY = nSy + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
					//LOGD(buffer);

					//已經發現，若前面有先後順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || 
					     pMergeImage[j][i] == V2 || 
						 pMergeImage[j][i] == V3 || 
					     pMergeImage[j][i] == V4 || 
						 pMergeImage[j][i] == nV1 ||
						 pMergeImage[j][i] == nV2 ||
						 pMergeImage[j][i] == (V2 - 100) ||
						 pMergeImage[j][i] == (V3 - 100) ||
						 pMergeImage[j][i] == (V4 - 100) ||
						 pMergeImage[j][i] == (nV2 - 100) ||
					     pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 ||
						pMergeImage[j][i] == D5 ||
						pMergeImage[j][i] == (R2 + 50) ||
						pMergeImage[j][i] == (D5 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D5;
								else
									pMergeImage[j][i] = D7;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D5;
								else
									pMergeImage[j][i] = D6;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R2 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R3 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R4 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nR2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] + 50;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i < nWidth - 1; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/* 初始化變數 */
	bNotFind = false;
	bPriority = false;
	bBxPriority = false;
	bBxFinish = false;
	bBlockLimit = false;
	bIsEdge = false;
	nBxPriorityIDx = -1;
	nCurrentIDx = 0;
	nDoubleFirst = 0;
	nPriorityIDx = -1;
	nContinueNotFindIDxCnt = 0;
	nLastNodeX = -1;
	nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Left pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向左尋找下一點 */
		for (i = nSx; i >= 0; ) {
			i = i - 1;	//將 X 座標左移
			if (i < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					j = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					j = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					j = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (j >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合 Ridges 的標記 0
				if (pMergeImage[j][i] == R1 ||
					pMergeImage[j][i] == D1 ||
					pMergeImage[j][i] == D2 ||
					pMergeImage[j][i] == D3 ||
					pMergeImage[j][i] == D4 ||
					pMergeImage[j][i] == (D2 - 10) ||
					pMergeImage[j][i] == (D3 - 10) ||
					pMergeImage[j][i] == (D4 - 10)) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D5;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					// 保存該點為這輪最後的Node點
					//nLastNodeX = i;
					//nLastNodeY = nSy + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
					//LOGD(buffer);

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1  || 
						 pMergeImage[j][i] == V2  || 
						 pMergeImage[j][i] == V3  || 
						 pMergeImage[j][i] == V4  ||
						 pMergeImage[j][i] == nV1 ||
						 pMergeImage[j][i] == nV2 ||
						 pMergeImage[j][i] == (V2 - 100) ||
						 pMergeImage[j][i] == (V3 - 100) ||
					 	 pMergeImage[j][i] == (V4 - 100) ||
						 pMergeImage[j][i] == (nV2 - 100) ||
						 pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 ||
						pMergeImage[j][i] == D5 ||
						pMergeImage[j][i] == (R2 + 50) ||
						pMergeImage[j][i] == (D5 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R4;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = R2;
								else
									pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								if (bIsEdge == true)
									pMergeImage[j][i] = D5;
								else
									pMergeImage[j][i] = D7;
							}
							else {
								if (bIsEdge == true)
									pMergeImage[j][i] = D5;
								else
									pMergeImage[j][i] = D6;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find *** (nLastNodeX = %d / nLastNodeY = %d) \r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				//break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = i;
				nLastNodeY = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//Check and Update nCurrentIDx (如果是下上邊界的話需再更新)
			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < SEARCH_SIZE; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);
			if (pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R2 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R3 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == R4 ||
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] == nR2) {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] + 50;
			}
			else {
				pMergeImage[nSy + SearchRange[nCurrentIDx]][i] = pMergeImage[nSy + SearchRange[nCurrentIDx]][i] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, i, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][i]);
			//LOGD(buffer);

			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSy = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (i = nSx; i >= 0; )
	}//if ((nSx >= 0 && nSx < nWidth - 1) && (nSy >= 0 && nSy < nHeight - 1)
}

void CheckBlockBadRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1, D5 = 86;

	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	//unsigned char nValleysEnding = 101;//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	int nIDx, nCurrentIDx = 0, nPriorityIDx = -1, nContinueNotFindIDxCnt = 0;
	bool bNotFind = false, bPriority = false, bIsEdge = false;
	bool bBxPriority = false, bBxFinish = false, bBlockLimit = false;//分岔點處理變數
	int nBxPriorityIDx = -1;//分岔點處理變數
	int nLimitLine = 0, nDoubleFirst = 0, nLastNodeX = -1, nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Down pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向下尋找下一點 */
		for (j = nSy; j < nHeight - 1; ) {
			j = j + 1;	//將 Y 座標下移

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記 0
				if (pMergeImage[j][i] == R1 ||
					pMergeImage[j][i] == D1 ||
					pMergeImage[j][i] == D2 ||
					pMergeImage[j][i] == D3 ||
					pMergeImage[j][i] == D4 ||
					pMergeImage[j][i] == (D2 - 10) ||
					pMergeImage[j][i] == (D3 - 10) ||
					pMergeImage[j][i] == (D4 - 10)) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D5;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 || 
					     pMergeImage[j][i] == V2 ||
					     pMergeImage[j][i] == V3 ||
					     pMergeImage[j][i] == V4 ||
						 pMergeImage[j][i] == nV1 ||
						 pMergeImage[j][i] == nV2 ||
						 pMergeImage[j][i] == (V2 - 100) ||
						 pMergeImage[j][i] == (V3 - 100) ||
						 pMergeImage[j][i] == (V4 - 100) ||
						 pMergeImage[j][i] == (nV2 - 100) ||
						 pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 ||
						pMergeImage[j][i] == D5 ||
						pMergeImage[j][i] == (R2 + 50) ||
						pMergeImage[j][i] == (D5 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = R4;
							}
							else {
								pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D7;
							}
							else {
								pMergeImage[j][i] = D6;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d /nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nR2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] + 50;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j < nHeight - 1; )
	}//if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1))

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d\r\n", __FUNCTION__, __LINE__);
	//LOGD(buffer);

	/* 初始化變數 */
	bNotFind = false;
	bPriority = false;
	bBxPriority = false;
	bBxFinish = false;
	bBlockLimit = false;
	bIsEdge = false;
	nBxPriorityIDx = -1;
	nCurrentIDx = 0;
	nDoubleFirst = 0;
	nPriorityIDx = -1;
	nContinueNotFindIDxCnt = 0;
	nLastNodeX = -1;
	nLastNodeY = -1;

	nSx = *nStartX;
	nSy = *nStartY;

	/* 定位 Point 並修改目前標記值 10 -> 9 代表已經處理過 */
	pMergeImage[nSy][nSx] = nR2;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d Start Up pMergeImage[%d][%d] = %d / Block (%d, %d)\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx], nSx / OneBlockSize, nSy / OneBlockSize);
	//LOGD(buffer);

	if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)) {
		/* 向上尋找下一點 */
		for (j = nSy; j >= 0; ) {
			j = j - 1;	//將 Y 座標上移
			if (j < 0)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					i = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, i, j, i / OneBlockSize, j / OneBlockSize, VerticalInValidRow[i / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[i / OneBlockSize] != -1) {
						nLimitLine = (VerticalInValidRow[i / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (j < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				//符合Valleys的標記 0
				if (pMergeImage[j][i] == R1 ||
					pMergeImage[j][i] == D1 ||
					pMergeImage[j][i] == D2 ||
					pMergeImage[j][i] == D3 ||
					pMergeImage[j][i] == D4 ||
					pMergeImage[j][i] == (D2 - 10) ||
					pMergeImage[j][i] == (D3 - 10) ||
					pMergeImage[j][i] == (D4 - 10)) {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** pMergeImage[%d][%d] = %d *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					if (pMergeImage[j][i] == R1) {
						pMergeImage[j][i] = R2;
						bPriority = true;
						nPriorityIDx = nIDx;//優先
					}
					else {
						pMergeImage[j][i] = D5;
					}
					nCurrentIDx = nIDx; //保存最後符合條件的Index
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx);
					//LOGD(buffer);
				}
				else if (pMergeImage[j][i] == nR1) {
					//紀錄方向角度並改標
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
					//LOGD(buffer);
					pMergeImage[j][i] = nR2;//更改標記為 9 代表已處理過
					nCurrentIDx = nIDx; //保存最後符合條件的Index

					//已經發現，若前面有有先順序的旗標，需要一併清除旗標，直接使用找到目標的Index
					if (bPriority == true) {
						bPriority = false;
						nPriorityIDx = -1;
					}
					break;
				}
				else if (pMergeImage[j][i] == V1 ||
						 pMergeImage[j][i] == V2 ||
						 pMergeImage[j][i] == V3 ||
						 pMergeImage[j][i] == V4 ||
						 pMergeImage[j][i] == nV1 ||
						 pMergeImage[j][i] == nV2 ||
						 pMergeImage[j][i] == (V2 - 100) ||
						 pMergeImage[j][i] == (V3 - 100) ||
						 pMergeImage[j][i] == (V4 - 100) ||
						 pMergeImage[j][i] == (nV2 - 100) ||
						 pMergeImage[j][i] == E0) {
					nContinueNotFindIDxCnt++;
					continue;
				}
				else {
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Find pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i]);
					//LOGD(buffer);
					nDoubleFirst++;

					//分叉點路徑
					if (pMergeImage[j][i] == R2 ||
						pMergeImage[j][i] == D5 ||
						pMergeImage[j][i] == (R2 + 50) ||
						pMergeImage[j][i] == (D5 - 10)) {
						if (pMergeImage[j][i] == R2 || pMergeImage[j][i] == (R2 + 50)) {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = R4;
							}
							else {
								pMergeImage[j][i] = R3;
							}
							bBxPriority = true;
							nBxPriorityIDx = nIDx;//分岔優先
						}
						else {
							if (nDoubleFirst == 1) {
								pMergeImage[j][i] = D7;
							}
							else {
								pMergeImage[j][i] = D6;
							}
						}
						nCurrentIDx = nIDx; //保存最後符合條件的Index
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d Double Bx nCurrentIDx = %d / nIDx = %d / nDoubleFirst = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nIDx, nDoubleFirst);
						//LOGD(buffer);
					}
					else if (pMergeImage[j][i] == nR2 || pMergeImage[j][i] == (nR2 + 50)) {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Find pMergeImage[%d][%d] = %d / Block (%d, %d) *** \r\n", __FUNCTION__, __LINE__, j, i, pMergeImage[j][i], i / OneBlockSize, j / OneBlockSize);
						//LOGD(buffer);
						nCurrentIDx = nIDx; //保存最後符合條件的Index

						if (bBxPriority == true) {
							bBxPriority = false;
							nBxPriorityIDx = -1;
						}
						nDoubleFirst = 0;
						bBxFinish = true;
						break;
					}
					else {
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find (nLastNodeX = %d / nLastNodeY = %d) ***\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
						//LOGD(buffer);
						bNotFind = true;
						nCurrentIDx = 0;
						nDoubleFirst = 0;
						break;
					}
				}

			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nContinueNotFindIDxCnt == SEARCH_SIZE) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nContinueNotFindIDxCnt = %d / nLastNodeX = %d / nLastNodeY = %d] ***\r\n", __FUNCTION__, __LINE__, nContinueNotFindIDxCnt, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
				if (nLastNodeX != -1 && nLastNodeY != -1)
					pMergeImage[nLastNodeY][nLastNodeX] = nRidgesEnding;//標記為 Ridges Ending
				bNotFind = true;
				nCurrentIDx = 0;
				nDoubleFirst = 0;
				nContinueNotFindIDxCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			//分叉點處理完成
			if (bBxFinish == true) {
				bBxFinish = false;
				break;
			}

			/* 更新最後找到且符合條件的IDx 並清除R1/E0旗標計數器 */
			nContinueNotFindIDxCnt = 0;
			if (bPriority == true) {
				bPriority = false;
				nCurrentIDx = nPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nPriorityIDx);
				//LOGD(buffer);
				nPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);

				// 有機會是已經走過的點，所以須清除
				if (bBxPriority == true)
					bBxPriority = false;
			}

			if (bBxPriority == true) {
				bBxPriority = false;
				nCurrentIDx = nBxPriorityIDx;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx] = %d / nBxPriorityIDx = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nBxPriorityIDx);
				//LOGD(buffer);
				nBxPriorityIDx = -1;

				// 保存該點為這輪最後的Node點
				nLastNodeX = nSx + SearchRange[nCurrentIDx];
				nLastNodeY = j;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d nLastNodeX = %d / nLastNodeY = %d\r\n", __FUNCTION__, __LINE__, nLastNodeX, nLastNodeY);
				//LOGD(buffer);
			}

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);
			if (pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R2 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R3 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == R4 ||
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] == nR2) {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] + 50;
			}
			else {
				pMergeImage[j][nSx + SearchRange[nCurrentIDx]] = pMergeImage[j][nSx + SearchRange[nCurrentIDx]] - 10;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current Path (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], j, pMergeImage[j][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update [nSx = %d] / nCurrentIDx = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx, nIDx);
			//LOGD(buffer);
		}//for (j = nSy; j >= 0; )
	}//if ((nSx >= 0 && nSx <= nWidth - 1) && (nSy >= 0 && nSy <= nHeight - 1)
}

void ReCheckRidgesEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

//	char buffer[1000] = { 0 };
	int i, j, nSx, nSy, nBlockType = -1;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nSx = i;
					nSy = j;
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d) / Block(%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSx / OneBlockSize, nSy / OneBlockSize, nBlockType);
//					LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						ReCheckGoodRidgesEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_GOOD);
					}
					else {
						ReCheckBadRidgesEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_BAD);
					}	
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
					//LOGD(buffer);
				}//if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2))
			}//if (pMergeImage[j][i] == nRidgesEnding)
		}//for (i = 0; i < nWidth; i++)
	}//for (j = 0; j < nHeight; j++)
}

void ReCheckGoodRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

//	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nRidgeFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nBlockType = -1, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindRidge = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向右尋找下一點 */
	for (ii = nSx; ii < nWidth; ) {
		ii = ii + 1; //將 X 座標右移
		if (ii == nWidth)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSy + SearchRange[nIDx] > nHeight - 1) {
				jj = nHeight - 1;
				bIsEdge = true;//超過 Y 方向的下邊界限制
			}
			else if (nSy + SearchRange[nIDx] < 0) {
				jj = 0;
				bIsEdge = true;//超過 Y 方向的上邊界限制
			}
			else
				jj = nSy + SearchRange[nIDx];

//			sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
//			LOGD(buffer);

			if (nBlockType == EMC_BLOCK_GOOD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Good限制線就停止不做
					if (jj >= nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}//if (nBlockType == EMC_BLOCK_GOOD)

			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR1 ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50))
				{
					if (bFindRidge == false)
						bFindRidge = true;
					//nRidgeFindCnt++;
					nCurrentIDx = nIDx;
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (/*pMergeImage[jj][ii] == V1 ||*/
					 pMergeImage[jj][ii] == V2 ||
					 pMergeImage[jj][ii] == V3 ||
					 pMergeImage[jj][ii] == V4 ||
					 pMergeImage[jj][ii] == (V2 - 100) ||
					 pMergeImage[jj][ii] == (V3 - 100) ||
					 pMergeImage[jj][ii] == (V4 - 100) ||
					 pMergeImage[jj][ii] == nV1 ||
					 pMergeImage[jj][ii] == nV2 ||
					 pMergeImage[jj][ii] == (nV2 - 100))
			{
				if (bFind == false)
					bFind = true;
				break;
			}
			else {
				nNotFindCnt++;
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//				LOGD(buffer);
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
//			LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			//nRidgeFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		 //超過 Y 的上下邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
		}

		if (bFindRidge == true) {
			bFindRidge = false;
			nRidgeFindCnt++;

			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
			//LOGD(buffer);

			//更新下次的 Y 座標
			nSy = nSy + SearchRange[nCurrentIDx];
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
//			LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindRidge == true)
	}//for (ii = nSx; ii < nWidth; )

//	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
//	LOGD(buffer);

	if (bValid == true) {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);
	}
	else {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);

		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bValid = false;
		bFindRidge = false;
		nCurrentIDx = 0;
		nNotFindCnt = 0;
		nRidgeFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向左尋找下一點 */
		for (ii = nSx; ii >= 0; ) {
			ii = ii - 1;	//將 X 座標左移
			if (ii < 0)
				break;

			for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

//				sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
//				LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10) ||
					pMergeImage[jj][ii] == E0)
				{
					if (pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50))
					{
						if (bFindRidge == false)
							bFindRidge = true;
						//nRidgeFindCnt++;
						nCurrentIDx = nIDx;
					}
					else {
						nNotFindCnt++;
						continue;
					}
				}
				else if (/*pMergeImage[jj][ii] == V1 ||*/
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100))
				{
					if (bFind == false)
						bFind = true;
					break;
				}
				else
				{
					nNotFindCnt++;
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//					LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			if (nNotFindCnt == _SEARCH_SIZE_) {
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
//				LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
				//nRidgeFindCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				bValid = true;
				break;
			}//if (bFind == true)

			 //超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			if (bFindRidge == true) {
				bFindRidge = false;
				nRidgeFindCnt++;
			
				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				//LOGD(buffer);

				//更新下次的 Y 座標
				nSy = nSy + SearchRange[nCurrentIDx];
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
//				LOGD(buffer);

				nNotFindCnt = 0;
			}//if (bFindRidge == true)
		}//for (ii = nSx; ii >= 0; )

//		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
//		LOGD(buffer);

		if (bValid == true) {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);
		}
		else {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);

			//將長度門檻未達標準的變更旗標為0
			pMergeImage[j][i] = R1;
		}
	}
}

void ReCheckBadRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nRidgeFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nBlockType = -1, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindRidge = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向下尋找下一點 */
	for (jj = nSy; jj < nHeight; ) {
		jj = jj + 1;	//將 Y 座標下移
		if (jj == nHeight)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
				bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
				break;
			}
			else
				ii = nSx + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_BAD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Bad限制線就停止不做
					if (jj < nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}

			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR1 ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50)) 
				{
					if (bFindRidge == false)
						bFindRidge = true;
					//nRidgeFindCnt++;
					nCurrentIDx = nIDx;
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (/*pMergeImage[jj][ii] == V1 ||*/
					 pMergeImage[jj][ii] == V2 ||
					 pMergeImage[jj][ii] == V3 ||
					 pMergeImage[jj][ii] == V4 ||
					 pMergeImage[jj][ii] == (V2 - 100) ||
					 pMergeImage[jj][ii] == (V3 - 100) ||
					 pMergeImage[jj][ii] == (V4 - 100) ||
					 pMergeImage[jj][ii] == nV1 ||
					 pMergeImage[jj][ii] == nV2 ||
					 pMergeImage[jj][ii] == (nV2 - 100)) 
			{
				if (bFind == false)
					bFind = true;
				break;
			}
			else {
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
				//LOGD(buffer);
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			//nRidgeFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		//超過 X 的左右邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
			break;
		}

		if (bFindRidge == true) {
			bFindRidge = false;
			nRidgeFindCnt++;

			int nUpdateIDx = 0;
			if (nSx + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			//更新下次的 X 座標
			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
			//LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindRidge == true)
	}//for (jj = nSy; jj < nHeight; )

	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
	//LOGD(buffer);

	if (bValid == true) {
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);
	}
	else {

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);

		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bValid = false;
		bFindRidge = false;
		nCurrentIDx = 0;
		nNotFindCnt = 0;
		nRidgeFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向上尋找下一點 */
		for (jj = nSy; jj >= 0; ) {
			jj = jj - 1;	//將 Y 座標上移
			if (jj < 0)
				break;

			for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10) ||
					pMergeImage[jj][ii] == E0) 
				{

					if (pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50))
					{
						if (bFindRidge == false)
							bFindRidge = true;
						//nRidgeFindCnt++;
						nCurrentIDx = nIDx;
					}
					else {
						nNotFindCnt++;
						continue;
					}
				}
				else if (/*pMergeImage[jj][ii] == V1 ||*/
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == (V2 - 100) ||
						 pMergeImage[jj][ii] == (V3 - 100) ||
						 pMergeImage[jj][ii] == (V4 - 100) ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == nV2 ||
						 pMergeImage[jj][ii] == (nV2 - 100)) 
				{
					if (bFind == false)
						bFind = true;
					break;
				}
				else {
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
					//LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			if (nNotFindCnt == _SEARCH_SIZE_) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
				//nRidgeFindCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}
			
			if (bFind == true) {
				bFind = false;
				bValid = true;
				break;
			}//if (bFind == true)

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFindRidge == true) {
				bFindRidge = false;
				nRidgeFindCnt++;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//更新下次的 X 座標
				nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);

				nNotFindCnt = 0;
			}//if (bFindRidge == true)
		}//for (jj = nSy; jj >= 0; )

		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
		//LOGD(buffer);

		if (bValid == true) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			//LOGD(buffer);
		}
		else {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			//LOGD(buffer);

			//將長度門檻未達標準的變更旗標為0
			pMergeImage[j][i] = R1;
		}
	}
}

int GetRidgesEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTextureTemplate == NULL)
		return -1;

	char buffer[1000] = { 0 };
	int i, j, nValidRidgesCnt = 0, nIDx = 0;
	unsigned char nRidgesEnding = 111;//Ridges Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nValidRidgesCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValidRidgesCnt = %d\r\n", __FUNCTION__, __LINE__, nValidRidgesCnt);
	//LOGD(buffer);

	if (pTextureTemplate->pRidgesEndingPoint == NULL && pTextureTemplate->nTotalRidgesEnding == 0) {
		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				if (pMergeImage[j][i] == nRidgesEnding) {
					if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
						pTextureTemplate->pRidgesEndingPoint[nIDx].x = i;
						pTextureTemplate->pRidgesEndingPoint[nIDx].y = j;
						nIDx++;
					}
				}
			}
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pRidgesEndingPoint);
		pTextureTemplate->pRidgesEndingPoint = NULL;
		pTextureTemplate->nTotalRidgesEnding = 0;

		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}

		for (j = 0; j < nHeight; j++) {
			for (i = 0; i < nWidth; i++) {
				if (pMergeImage[j][i] == nRidgesEnding) {
					if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
						pTextureTemplate->pRidgesEndingPoint[nIDx].x = i;
						pTextureTemplate->pRidgesEndingPoint[nIDx].y = j;
						nIDx++;
					}
				}
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nIDx = %d\r\n", __FUNCTION__, __LINE__, nIDx);
	LOGD(buffer);

	for (nIDx = 0; nIDx < pTextureTemplate->nTotalRidgesEnding; nIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pRidgesEndingPoint[nIDx].x, pTextureTemplate->pRidgesEndingPoint[nIDx].y);
		LOGD(buffer);
	}
	return nValidRidgesCnt;
}

int GetEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTextureTemplate == NULL)
		return -1;

	char buffer[1000] = { 0 };
	int i, j, nValidValleysCnt = 0, nValidRidgesCnt = 0, nVIDx = 0, nRIDx = 0;
	unsigned char nValleysEnding = 101;//Valleys Ending標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {

			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nValidValleysCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);
				}
			}

			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nValidRidgesCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
					//LOGD(buffer);
				}
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValidValleysCnt = %d / nValidRidgesCnt = %d\r\n", __FUNCTION__, __LINE__, nValidValleysCnt, nValidRidgesCnt);
	//LOGD(buffer);

	if (pTextureTemplate->pValleysEndingPoint == NULL && pTextureTemplate->nTotalValleysEnding == 0) {
		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pValleysEndingPoint);
		pTextureTemplate->pValleysEndingPoint = NULL;
		pTextureTemplate->nTotalValleysEnding = 0;

		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}
	}

	if (pTextureTemplate->pRidgesEndingPoint == NULL && pTextureTemplate->nTotalRidgesEnding == 0) {
		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pRidgesEndingPoint);
		pTextureTemplate->pRidgesEndingPoint = NULL;
		pTextureTemplate->nTotalRidgesEnding = 0;

		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					pTextureTemplate->pValleysEndingPoint[nVIDx].x = i;
					pTextureTemplate->pValleysEndingPoint[nVIDx].y = j;
					nVIDx++;
				}
			}

			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					pTextureTemplate->pRidgesEndingPoint[nRIDx].x = i;
					pTextureTemplate->pRidgesEndingPoint[nRIDx].y = j;
					nRIDx++;
				}
			}			
		}
	}

	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nVIDx = %d / nRIDx = %d\r\n", __FUNCTION__, __LINE__, nVIDx, nRIDx);
	LOGD(buffer);

	for (nVIDx = 0; nVIDx < pTextureTemplate->nTotalValleysEnding; nVIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pValleysEndingPoint[nVIDx].x, pTextureTemplate->pValleysEndingPoint[nVIDx].y);
		LOGD(buffer);
	}

	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
	LOGD(buffer);

	for (nRIDx = 0; nRIDx < pTextureTemplate->nTotalRidgesEnding; nRIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pRidgesEndingPoint[nRIDx].x, pTextureTemplate->pRidgesEndingPoint[nRIDx].y);
		LOGD(buffer);
	}
	return 0;
}

int GetEndingTemplateWithDirection(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTextureTemplate == NULL)
		return -1;

	char buffer[1000] = { 0 };
	int i, j, nValidValleysCnt = 0, nValidRidgesCnt = 0, nVIDx = 0, nRIDx = 0;
	unsigned char nValleysEnding = 101;//Valleys Ending標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	nValidValleysCnt = nValleysDirectionCnt;
	nValidRidgesCnt = nRidgesDirectionCnt;
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValidValleysCnt = %d / nValidRidgesCnt = %d\r\n", __FUNCTION__, __LINE__, nValidValleysCnt, nValidRidgesCnt);
	//LOGD(buffer);

	if (pTextureTemplate->pValleysEndingPoint == NULL && pTextureTemplate->nTotalValleysEnding == 0) {
		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pValleysEndingPoint);
		pTextureTemplate->pValleysEndingPoint = NULL;
		pTextureTemplate->nTotalValleysEnding = 0;

		pTextureTemplate->nTotalValleysEnding = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pValleysEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pValleysEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidValleysCnt);
		}
	}

	if (pTextureTemplate->pRidgesEndingPoint == NULL && pTextureTemplate->nTotalRidgesEnding == 0) {
		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pRidgesEndingPoint);
		pTextureTemplate->pRidgesEndingPoint = NULL;
		pTextureTemplate->nTotalRidgesEnding = 0;

		pTextureTemplate->nTotalRidgesEnding = nValidRidgesCnt;
		if (nValidRidgesCnt > 0) {
			pTextureTemplate->pRidgesEndingPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
			memset(pTextureTemplate->pRidgesEndingPoint, 0, sizeof(struct TextureTemplatePOINT) * nValidRidgesCnt);
		}
	}
	
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nValleysEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					pTextureTemplate->pValleysEndingPoint[nVIDx].x = i;
					pTextureTemplate->pValleysEndingPoint[nVIDx].y = j;
					pTextureTemplate->pValleysEndingPoint[nVIDx].theta = pValleysDirection[nVIDx];
					nVIDx++;
				}
			}

			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					pTextureTemplate->pRidgesEndingPoint[nRIDx].x = i;
					pTextureTemplate->pRidgesEndingPoint[nRIDx].y = j;
					pTextureTemplate->pRidgesEndingPoint[nRIDx].theta = pRidgesDirection[nRIDx];
					nRIDx++;
				}
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nVIDx = %d / nRIDx = %d\r\n", __FUNCTION__, __LINE__, nVIDx, nRIDx);
	LOGD(buffer);

	for (nVIDx = 0; nVIDx < pTextureTemplate->nTotalValleysEnding; nVIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Ending (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pValleysEndingPoint[nVIDx].x, pTextureTemplate->pValleysEndingPoint[nVIDx].y, pTextureTemplate->pValleysEndingPoint[nVIDx].theta);
		LOGD(buffer);
	}

	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
	LOGD(buffer);

	for (nRIDx = 0; nRIDx < pTextureTemplate->nTotalRidgesEnding; nRIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pRidgesEndingPoint[nRIDx].x, pTextureTemplate->pRidgesEndingPoint[nRIDx].y, pTextureTemplate->pRidgesEndingPoint[nRIDx].theta);
		LOGD(buffer);
	}

	if (pValleysDirection != NULL) {
		free(pValleysDirection);
		pValleysDirection = NULL;
		nValleysDirectionCnt = -1;
	}
	
	if (pRidgesDirection != NULL) {
		free(pRidgesDirection);
		pRidgesDirection = NULL;
		nRidgesDirectionCnt = -1;
	}
	return 0;
}

int GetBoundingValleysTemplate(struct TextureTemplate *pTextureTemplate, struct Texture *pTexture)
{
	if (pTextureTemplate == NULL || pTexture == NULL)
		return -1;

	int i, nValidValleysCnt = 0, nIDx = 0;
	char buffer[1000] = { 0 };

	for (i = 0; i < pTexture->nTotalBoundValley; i++) {
		if (pTexture->pBoundValley[i].valid == true) {
			nValidValleysCnt++;
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Bounding nValidValleysCnt = %d\r\n", __FUNCTION__, __LINE__, nValidValleysCnt);
	LOGD(buffer);

	if (pTextureTemplate->pBoundValleys == NULL && pTextureTemplate->pBoundValleys == 0) {
		pTextureTemplate->nTotalBoundValleys = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pBoundValleys = (struct TexturePOINT *) malloc(sizeof(struct TexturePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pBoundValleys, 0, sizeof(struct TexturePOINT) * nValidValleysCnt);
		}
	}
	else {
		free(pTextureTemplate->pBoundValleys);
		pTextureTemplate->pBoundValleys = NULL;
		pTextureTemplate->nTotalBoundValleys = 0;

		pTextureTemplate->nTotalBoundValleys = nValidValleysCnt;
		if (nValidValleysCnt > 0) {
			pTextureTemplate->pBoundValleys = (struct TexturePOINT *) malloc(sizeof(struct TexturePOINT) * nValidValleysCnt);
			memset(pTextureTemplate->pBoundValleys, 0, sizeof(struct TexturePOINT) * nValidValleysCnt);
		}
	}

	for (i = 0; i < pTexture->nTotalBoundValley; i++) {
		if (pTexture->pBoundValley[i].valid == true) {
			pTextureTemplate->pBoundValleys[nIDx].x = pTexture->pBoundValley[i].x;
			pTextureTemplate->pBoundValleys[nIDx].y = pTexture->pBoundValley[i].y;
			pTextureTemplate->pBoundValleys[nIDx].type = pTexture->pBoundValley[i].type;
			pTextureTemplate->pBoundValleys[nIDx].valid = pTexture->pBoundValley[i].valid;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pBoundValleys[nIDx].x, pTextureTemplate->pBoundValleys[nIDx].y);
			//LOGD(buffer);
			nIDx++;
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nIDx = %d\r\n", __FUNCTION__, __LINE__, nIDx);
	//LOGD(buffer);
	return 0;
}

void CheckValleysBranchSearchRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	char buffer[1000] = { 0 };
	int i, j, nSx, nSy, nBlockType = -1;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (/*pMergeImage[j][i] == V3 || pMergeImage[j][i] == (V3 - 100) ||*/ pMergeImage[j][i] == V4 || (pMergeImage[j][i] == V4 - 100) || pMergeImage[j][i] == D4 || (pMergeImage[j][i] == D4 - 10)) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nSx = i;
					nSy = j;
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Valleys Search Ridges Ending (%d, %d) / Block(%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSx / OneBlockSize, nSy / OneBlockSize, nBlockType);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						ValleysBranchSearchGoodRidges(pMergeImage, VerticalInValidRow, &i, &j, nWidth, nHeight, EMC_BLOCK_GOOD);
					}
					else {
						ValleysBranchSearchBadRidges(pMergeImage, VerticalInValidRow, &i, &j, nWidth, nHeight, EMC_BLOCK_BAD);
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
					//LOGD(buffer);
				}//if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2))
			}//if (pMergeImage[j][i] == nRidgesEnding)
		}//for (i = 0; i < nWidth; i++)
	}//for (j = 0; j < nHeight; j++)

	//Re-Check and Update Valid Valleys Data
#if __USE_RIDGE_SEARCH__
	ReCheckRidgesEnding(pMergeImage, VerticalInValidRow, nWidth, nHeight);
#else
	ReCheckValleysBranchSearchRidgesEnding(pMergeImage, VerticalInValidRow, nWidth, nHeight);
#endif // __USE_RIDGE_SEARCH__
}

void ValleysBranchSearchGoodRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nValleyFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindValley = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;

	/* 向右尋找下一點 */
	for (ii = nSx; ii < nWidth; ) {
		ii = ii + 1; //將 X 座標右移
		if (ii == nWidth)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSy + SearchRange[nIDx] > nHeight - 1) {
				jj = nHeight - 1;
				bIsEdge = true;//超過 Y 方向的下邊界限制
			}
			else if (nSy + SearchRange[nIDx] < 0) {
				jj = 0;
				bIsEdge = true;//超過 Y 方向的上邊界限制
			}
			else
				jj = nSy + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_GOOD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Good限制線就停止不做
					if (jj >= nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}//if (nBlockType == EMC_BLOCK_GOOD)

			if (pMergeImage[jj][ii] == V1 ||
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{
				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFindValley == false)
						bFindValley = true;
					nCurrentIDx = nIDx;	
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (pMergeImage[jj][ii] == R1 ||
					 pMergeImage[jj][ii] == R2 ||
					 pMergeImage[jj][ii] == R3 ||
					 pMergeImage[jj][ii] == R4 ||
					 pMergeImage[jj][ii] == (R2 + 50) ||
					 pMergeImage[jj][ii] == (R3 + 50) ||
					 pMergeImage[jj][ii] == (R4 + 50) ||
					 pMergeImage[jj][ii] == nR1 ||
					 pMergeImage[jj][ii] == nR2 ||
					 pMergeImage[jj][ii] == (nR2 + 50))
			{

				if (bFind == false)
					bFind = true;
				break;
			}
			else
			{
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
				//LOGD(buffer);
				if (pMergeImage[jj][ii] == nRidgesEnding) {
					bFind = true;
					break;
				}
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		 //超過 Y 的上下邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
		}

		if (bFindValley == true) {
			bFindValley = false;
			nValleyFindCnt++;

			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
			//LOGD(buffer);

			//更新下次的 Y 座標
			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
			//LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindValley == true)
	}//for (ii = nSx; ii < nWidth; )

	 //sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValleyFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nValleyFindCnt, i, j, ii, jj);
	 //LOGD(buffer);

	 if (bValid == true) {
		 //sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		 //LOGD(buffer);

		 //標記為Ridges Ending
		 pMergeImage[jj][ii] = nRidgesEnding;
	 }
	 else {
		 //sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		 //LOGD(buffer);

		 /* 初始化變數 */
		 bIsEdge = false;
		 bBlockLimit = false;
		 bNotFind = false;
		 bFind = false;
		 bValid = false;
		 bFindValley = false;
		 nCurrentIDx = 0;
		 nNotFindCnt = 0;
		 nValleyFindCnt = 0;

		 /* 載入未滿長度條件的座標 */
		 nSx = *nStartX;
		 nSy = *nStartY;

		 /* 向左尋找下一點 */
		 for (ii = nSx; ii >= 0; ) {
			 ii = ii - 1;	//將 X 座標左移
			 if (ii < 0)
				 break;

			 for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				 if (nSy + SearchRange[nIDx] > nHeight - 1) {
					 jj = nHeight - 1;
					 bIsEdge = true;//超過 Y 方向的下邊界限制
				 }
				 else if (nSy + SearchRange[nIDx] < 0) {
					 jj = 0;
					 bIsEdge = true;//超過 Y 方向的上邊界限制
				 }
				 else
					 jj = nSy + SearchRange[nIDx];

				 //sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				 //LOGD(buffer);

				 if (nBlockType == EMC_BLOCK_GOOD) {
					 if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						 int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						 //超過區域Good限制線就停止不做
						 if (jj >= nLimitLine) {
							 bBlockLimit = true;
							 break;
						 }
					 }
				 }//if (nBlockType == EMC_BLOCK_GOOD)

				 if (pMergeImage[jj][ii] == V1 ||
					 pMergeImage[jj][ii] == V2 ||
					 pMergeImage[jj][ii] == V3 ||
					 pMergeImage[jj][ii] == V4 ||
					 pMergeImage[jj][ii] == (V2 - 100) ||
					 pMergeImage[jj][ii] == (V3 - 100) ||
					 pMergeImage[jj][ii] == (V4 - 100) ||
					 pMergeImage[jj][ii] == nV1 ||
					 pMergeImage[jj][ii] == nV2 ||
					 pMergeImage[jj][ii] == (nV2 - 100) ||
					 pMergeImage[jj][ii] == D1 ||
					 pMergeImage[jj][ii] == D2 ||
					 pMergeImage[jj][ii] == D3 ||
					 pMergeImage[jj][ii] == D4 ||
					 pMergeImage[jj][ii] == D5 ||
					 pMergeImage[jj][ii] == D6 ||
					 pMergeImage[jj][ii] == D7 ||
					 pMergeImage[jj][ii] == (D2 - 10) ||
					 pMergeImage[jj][ii] == (D3 - 10) ||
					 pMergeImage[jj][ii] == (D4 - 10) ||
					 pMergeImage[jj][ii] == (D5 - 10) ||
					 pMergeImage[jj][ii] == (D6 - 10) ||
					 pMergeImage[jj][ii] == (D7 - 10) ||
					 pMergeImage[jj][ii] == E0)
				 {
					 if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == (V2 - 100) ||
						 pMergeImage[jj][ii] == (V3 - 100) ||
						 pMergeImage[jj][ii] == (V4 - 100) ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == nV2 ||
						 pMergeImage[jj][ii] == (nV2 - 100) ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == D5 ||
						 pMergeImage[jj][ii] == D6 ||
						 pMergeImage[jj][ii] == D7 ||
						 pMergeImage[jj][ii] == (D2 - 10) ||
						 pMergeImage[jj][ii] == (D3 - 10) ||
						 pMergeImage[jj][ii] == (D4 - 10) ||
						 pMergeImage[jj][ii] == (D5 - 10) ||
						 pMergeImage[jj][ii] == (D6 - 10) ||
						 pMergeImage[jj][ii] == (D7 - 10))
					 {
						 if (bFindValley == false)
							 bFindValley = true;
						 nCurrentIDx = nIDx;
					 }
					 else {
						 nNotFindCnt++;
						 continue;
					 }
				 }
				 else if (pMergeImage[jj][ii] == R1 ||
					 pMergeImage[jj][ii] == R2 ||
					 pMergeImage[jj][ii] == R3 ||
					 pMergeImage[jj][ii] == R4 ||
					 pMergeImage[jj][ii] == (R2 + 50) ||
					 pMergeImage[jj][ii] == (R3 + 50) ||
					 pMergeImage[jj][ii] == (R4 + 50) ||
					 pMergeImage[jj][ii] == nR1 ||
					 pMergeImage[jj][ii] == nR2 ||
					 pMergeImage[jj][ii] == (nR2 + 50))
				 {

					 if (bFind == false)
						 bFind = true;
					 break;
				 }
				 else
				 {
					 nNotFindCnt++;
					 //sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
					 //LOGD(buffer);
					 if (pMergeImage[jj][ii] == nRidgesEnding) {
						 bFind = true;
						 break;
					 }
				 }
			 }// for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			 if (nNotFindCnt == _SEARCH_SIZE_) {
				 //sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				 //LOGD(buffer);
				 bNotFind = true;
				 nCurrentIDx = 0;
				 nNotFindCnt = 0;
			 }

			 if (bNotFind == true) {
				 bNotFind = false;
				 break;
			 }

			 if (bBlockLimit == true) {
				 bBlockLimit = false;
				 break;
			 }

			 if (bFind == true) {
				 bFind = false;
				 bValid = true;
				 break;
			 }//if (bFind == true)

			  //超過 Y 的上下邊界限制
			 if (bIsEdge == true) {
				 bIsEdge = false;
			 }

			 if (bFindValley == true) {
				 bFindValley = false;
				 nValleyFindCnt++;

				 int nUpdateIDx = 0;
				 if (nSy + SearchRange[nCurrentIDx] < 0) {
					 for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						 if (nSy + SearchRange[nn] == 0) {
							 nUpdateIDx = nn;
							 break;
						 }
					 }
					 nCurrentIDx = nUpdateIDx;
				 }
				 else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					 for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						 if (nSy + SearchRange[nn] == nHeight - 1) {
							 nUpdateIDx = nn;
							 break;
						 }
					 }
					 nCurrentIDx = nUpdateIDx;
				 }
				 //sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				 //LOGD(buffer);

				 //更新下次的 Y 座標
				 nSy = nSy + SearchRange[nCurrentIDx];
				 //sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				 //LOGD(buffer);

				 nNotFindCnt = 0;
			 }//if (bFindRidge == true)
		 }//for (ii = nSx; ii >= 0; )

		  //sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValleyFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nValleyFindCnt, i, j, ii, jj);
		  //LOGD(buffer);

		  if (bValid == true) {
			  //sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			  //LOGD(buffer);

			  //標記為Ridges Ending
			  pMergeImage[jj][ii] = nRidgesEnding;
		  }
		  else {
			  //sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			  //LOGD(buffer);
		  }
	 }
}

void ValleysBranchSearchBadRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nValleyFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindValley = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;

	/* 向下尋找下一點 */
	for (jj = nSy; jj < nHeight; ) {
		jj = jj + 1;	//將 Y 座標下移
		if (jj == nHeight)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
				bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
				break;
			}
			else
				ii = nSx + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_BAD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Bad限制線就停止不做
					if (jj < nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}

			if (pMergeImage[jj][ii] == V1 ||
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{
				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFindValley == false)
						bFindValley = true;
					nCurrentIDx = nIDx;
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (pMergeImage[jj][ii] == R1 ||
					 pMergeImage[jj][ii] == R2 ||
					 pMergeImage[jj][ii] == R3 ||
					 pMergeImage[jj][ii] == R4 ||
					 pMergeImage[jj][ii] == (R2 + 50) ||
					 pMergeImage[jj][ii] == (R3 + 50) ||
					 pMergeImage[jj][ii] == (R4 + 50) ||
					 pMergeImage[jj][ii] == nR1 ||
					 pMergeImage[jj][ii] == nR2 ||
					 pMergeImage[jj][ii] == (nR2 + 50))
			{

				if (bFind == false)
					bFind = true;
				break;
			}
			else
			{
				nNotFindCnt++;
				//sprintf(buffer, "[FPTexturepMergeImage[jj][ii].c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
				//LOGD(buffer);
				if (pMergeImage[jj][ii] == nRidgesEnding) {
					bFind = true;
					break;
				}
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		 //超過 X 的左右邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
			break;
		}

		if (bFindValley == true) {
			bFindValley = false;
			nValleyFindCnt++;

			int nUpdateIDx = 0;
			if (nSx + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			//更新下次的 X 座標
			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
			//LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindRidge == true)
	}//for (jj = nSy; jj < nHeight; )

	 //sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValleyFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nValleyFindCnt, i, j, ii, jj);
	 //LOGD(buffer);

	if (bValid == true) {
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);

		//標記為Ridges Ending
		pMergeImage[jj][ii] = nRidgesEnding;
	}
	else {
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
		//LOGD(buffer);

		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bValid = false;
		bFindValley = false;
		nCurrentIDx = 0;
		nNotFindCnt = 0;
		nValleyFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向上尋找下一點 */
		for (jj = nSy; jj >= 0; ) {
			jj = jj - 1;	//將 Y 座標上移
			if (jj < 0)
				break;

			for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				if (pMergeImage[jj][ii] == V1 ||
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10) ||
					pMergeImage[jj][ii] == E0)
				{
					if (pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V2 ||
						pMergeImage[jj][ii] == V3 ||
						pMergeImage[jj][ii] == V4 ||
						pMergeImage[jj][ii] == (V2 - 100) ||
						pMergeImage[jj][ii] == (V3 - 100) ||
						pMergeImage[jj][ii] == (V4 - 100) ||
						pMergeImage[jj][ii] == nV1 ||
						pMergeImage[jj][ii] == nV2 ||
						pMergeImage[jj][ii] == (nV2 - 100) ||
						pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10))
					{
						if (bFindValley == false)
							bFindValley = true;
						nCurrentIDx = nIDx;
					}
					else {
						nNotFindCnt++;
						continue;
					}
				}
				else if (pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == R2 ||
						 pMergeImage[jj][ii] == R3 ||
						 pMergeImage[jj][ii] == R4 ||
						 pMergeImage[jj][ii] == (R2 + 50) ||
						 pMergeImage[jj][ii] == (R3 + 50) ||
						 pMergeImage[jj][ii] == (R4 + 50) ||
						 pMergeImage[jj][ii] == nR1 ||
						 pMergeImage[jj][ii] == nR2 ||
						 pMergeImage[jj][ii] == (nR2 + 50))
				{

					if (bFind == false)
						bFind = true;
					break;
				}
				else
				{
					nNotFindCnt++;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
					//LOGD(buffer);
					if (pMergeImage[jj][ii] == nRidgesEnding) {
						bFind = true;
						break;
					}
				}
			}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			if (nNotFindCnt == _SEARCH_SIZE_) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				bValid = true;
				break;
			}//if (bFind == true)

			 //超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFindValley == true) {
				bFindValley = false;
				nValleyFindCnt++;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//更新下次的 X 座標
				nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);

				nNotFindCnt = 0;
			}//if (bFindRidge == true)
		}//for (jj = nSy; jj >= 0; )

		 //sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nValleyFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nValleyFindCnt, i, j, ii, jj);
		 //LOGD(buffer);

		 if (bValid == true) {
			 //sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			 //LOGD(buffer);

			 //標記為Ridges Ending
			 pMergeImage[jj][ii] = nRidgesEnding;
		 }
		 else {
			 //sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
			 //LOGD(buffer);
		 }
	}
}

void ReCheckValleysBranchSearchRidgesEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	//char buffer[1000] = { 0 };
	int i, j, nSx, nSy, nBlockType = -1;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nRidgesEnding) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					nSx = i;
					nSy = j;
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Ridges Ending (%d, %d) / Block(%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSx / OneBlockSize, nSy / OneBlockSize, nBlockType);
					//LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						ReCheckValleysBranchSearchGoodRidgesEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_GOOD);
					}
					else {
						ReCheckValleysBranchSearchBadRidgesEnding(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, EMC_BLOCK_BAD);
					}
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
					//LOGD(buffer);
				}//if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2))
			}//if (pMergeImage[j][i] == nRidgesEnding)
		}//for (i = 0; i < nWidth; i++)
	}//for (j = 0; j < nHeight; j++)
}

void ReCheckValleysBranchSearchGoodRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

//	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nRidgeFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nBlockType = -1, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH - 2;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindRidge = false, bFindPore = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向右尋找下一點 */
	for (ii = nSx; ii < nWidth; ) {
		ii = ii + 1; //將 X 座標右移
		if (ii == nWidth)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSy + SearchRange[nIDx] > nHeight - 1) {
				jj = nHeight - 1;
				bIsEdge = true;//超過 Y 方向的下邊界限制
			}
			else if (nSy + SearchRange[nIDx] < 0) {
				jj = 0;
				bIsEdge = true;//超過 Y 方向的上邊界限制
			}
			else
				jj = nSy + SearchRange[nIDx];

//			sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
//			LOGD(buffer);

			if (nBlockType == EMC_BLOCK_GOOD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Good限制線就停止不做
					if (jj >= nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}//if (nBlockType == EMC_BLOCK_GOOD)

			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR1 ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50))
				{
					if (bFindRidge == false)
						bFindRidge = true;
					nCurrentIDx = nIDx;
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (/*pMergeImage[jj][ii] == V1 ||*/
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100))
			{
				if (bFind == false)
					bFind = true;
				break;
			}
			else {
				nNotFindCnt++;
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//				LOGD(buffer);

				if (pMergeImage[jj][ii] == V1) {
					bFindPore = true;
					break;
				}

				if (pMergeImage[jj][ii] == nRidgesEnding) {
					bFind = true;
					break;
				}
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bFindPore == true) {
			bFindPore = false;
			break;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		 //超過 Y 的上下邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
		}

		if (bFindRidge == true) {
			bFindRidge = false;
			nRidgeFindCnt++;

			int nUpdateIDx = 0;
			if (nSy + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSy + SearchRange[nn] == nHeight - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
			//LOGD(buffer);

			//更新下次的 Y 座標
			nSy = nSy + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
			//LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindRidge == true)
	}//for (ii = nSx; ii < nWidth; )

//	 sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d) / [%d]\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj, pMergeImage[jj][ii]);
//	 LOGD(buffer);

	if (bValid == true) {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);
	}
	else {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);

		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bValid = false;
		bFindRidge = false;
		nCurrentIDx = 0;
		nNotFindCnt = 0;
		nRidgeFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向左尋找下一點 */
		for (ii = nSx; ii >= 0; ) {
			ii = ii - 1;	//將 X 座標左移
			if (ii < 0)
				break;

			for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10) ||
					pMergeImage[jj][ii] == E0)
				{
					if (pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50))
					{
						if (bFindRidge == false)
							bFindRidge = true;
						nCurrentIDx = nIDx;
					}
					else {
						nNotFindCnt++;
						continue;
					}
				}
				else if (/*pMergeImage[jj][ii] == V1 ||*/
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100))
				{
					if (bFind == false)
						bFind = true;
					break;
				}
				else
				{
					nNotFindCnt++;
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//					LOGD(buffer);

					if (pMergeImage[jj][ii] == V1) {
						bFindPore = true;
						break;
					}

					if (pMergeImage[jj][ii] == nRidgesEnding) {
						bFind = true;
						break;
					}
				}
			}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			if (nNotFindCnt == _SEARCH_SIZE_) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bFindPore == true) {
				bFindPore = false;
				break;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				bValid = true;
				break;
			}//if (bFind == true)

			 //超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			if (bFindRidge == true) {
				bFindRidge = false;
				nRidgeFindCnt++;

				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				//LOGD(buffer);

				//更新下次的 Y 座標
				nSy = nSy + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				//LOGD(buffer);

				nNotFindCnt = 0;
			}//if (bFindRidge == true)
		}//for (ii = nSx; ii >= 0; )

//		 sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
//		 LOGD(buffer);

		if (bValid == true) {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);
		}
		else {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);

			//將長度門檻未達標準的變更旗標為0
			pMergeImage[j][i] = R1;
		}
	}
}

void ReCheckValleysBranchSearchBadRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

//	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nRidgeFindCnt = 0, nIDx = 0, nCurrentIDx = 0, nBlockType = -1, nNotFindCnt = 0;
	signed char SearchRange[_SEARCH_SIZE_] = { 0, 1, -1 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, D1 = 85, D2 = 84, V1 = 255, V2 = 254;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindRidge = false, bFindPore = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	/* 向下尋找下一點 */
	for (jj = nSy; jj < nHeight; ) {
		jj = jj + 1;	//將 Y 座標下移
		if (jj == nHeight)
			break;

		for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
			if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
				bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
				break;
			}
			else
				ii = nSx + SearchRange[nIDx];

			//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
			//LOGD(buffer);

			if (nBlockType == EMC_BLOCK_BAD) {
				if (VerticalInValidRow[ii / OneBlockSize] != -1) {
					int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
					//超過區域Bad限制線就停止不做
					if (jj < nLimitLine) {
						bBlockLimit = true;
						break;
					}
				}
			}

			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR1 ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50) ||
				pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == E0)
			{

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50))
				{
					if (bFindRidge == false)
						bFindRidge = true;
					nCurrentIDx = nIDx;
				}
				else {
					nNotFindCnt++;
					continue;
				}
			}
			else if (/*pMergeImage[jj][ii] == V1 ||*/
				pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == nV1 ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100))
			{
				if (bFind == false)
					bFind = true;
				break;
			}
			else {
				nNotFindCnt++;
//				sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//				LOGD(buffer);

				if (pMergeImage[jj][ii] == V1) {
					bFindPore = true;
					break;
				}

				if (pMergeImage[jj][ii] == nRidgesEnding) {
					bFind = true;
					break;
				}
			}
		}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

		if (nNotFindCnt == _SEARCH_SIZE_) {
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
			//LOGD(buffer);
			bNotFind = true;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
		}

		if (bFindPore == true) {
			bFindPore = false;
			break;
		}

		if (bNotFind == true) {
			bNotFind = false;
			break;
		}

		if (bBlockLimit == true) {
			bBlockLimit = false;
			break;
		}

		if (bFind == true) {
			bFind = false;
			bValid = true;
			break;
		}//if (bFind == true)

		 //超過 X 的左右邊界限制
		if (bIsEdge == true) {
			bIsEdge = false;
			break;
		}

		if (bFindRidge == true) {
			bFindRidge = false;
			nRidgeFindCnt++;

			int nUpdateIDx = 0;
			if (nSx + SearchRange[nCurrentIDx] < 0) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == 0) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			else if (nSx + SearchRange[nCurrentIDx] > nWidth - 1) {
				for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
					if (nSx + SearchRange[nn] == nWidth - 1) {
						nUpdateIDx = nn;
						break;
					}
				}
				nCurrentIDx = nUpdateIDx;
			}
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
			//LOGD(buffer);

			//更新下次的 X 座標
			nSx = nSx + SearchRange[nCurrentIDx];
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
			//LOGD(buffer);

			nNotFindCnt = 0;
		}//if (bFindRidge == true)
	}//for (jj = nSy; jj < nHeight; )

//	 sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
//	 LOGD(buffer);

	if (bValid == true) {
//		sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);
	}
	else {

//		sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//		LOGD(buffer);

		/* 初始化變數 */
		bIsEdge = false;
		bBlockLimit = false;
		bNotFind = false;
		bFind = false;
		bValid = false;
		bFindRidge = false;
		nCurrentIDx = 0;
		nNotFindCnt = 0;
		nRidgeFindCnt = 0;

		/* 載入未滿長度條件的座標 */
		nSx = *nStartX;
		nSy = *nStartY;

		/* 向上尋找下一點 */
		for (jj = nSy; jj >= 0; ) {
			jj = jj - 1;	//將 Y 座標上移
			if (jj < 0)
				break;

			for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				//sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d) / Block (%d, %d) / Limit %d\r\n", __FUNCTION__, __LINE__, ii, jj, ii / OneBlockSize, jj / OneBlockSize, VerticalInValidRow[ii / OneBlockSize]);
				//LOGD(buffer);

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == R2 ||
					pMergeImage[jj][ii] == R3 ||
					pMergeImage[jj][ii] == R4 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR1 ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == D1 ||
					pMergeImage[jj][ii] == D2 ||
					pMergeImage[jj][ii] == D3 ||
					pMergeImage[jj][ii] == D4 ||
					pMergeImage[jj][ii] == D5 ||
					pMergeImage[jj][ii] == D6 ||
					pMergeImage[jj][ii] == D7 ||
					pMergeImage[jj][ii] == (D2 - 10) ||
					pMergeImage[jj][ii] == (D3 - 10) ||
					pMergeImage[jj][ii] == (D4 - 10) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10) ||
					pMergeImage[jj][ii] == E0)
				{

					if (pMergeImage[jj][ii] == R1 ||
						pMergeImage[jj][ii] == R2 ||
						pMergeImage[jj][ii] == R3 ||
						pMergeImage[jj][ii] == R4 ||
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR1 ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50))
					{
						if (bFindRidge == false)
							bFindRidge = true;
						nCurrentIDx = nIDx;
					}
					else {
						nNotFindCnt++;
						continue;
					}
				}
				else if (/*pMergeImage[jj][ii] == V1 ||*/
					pMergeImage[jj][ii] == V2 ||
					pMergeImage[jj][ii] == V3 ||
					pMergeImage[jj][ii] == V4 ||
					pMergeImage[jj][ii] == (V2 - 100) ||
					pMergeImage[jj][ii] == (V3 - 100) ||
					pMergeImage[jj][ii] == (V4 - 100) ||
					pMergeImage[jj][ii] == nV1 ||
					pMergeImage[jj][ii] == nV2 ||
					pMergeImage[jj][ii] == (nV2 - 100))
				{
					if (bFind == false)
						bFind = true;
					break;
				}
				else {
					nNotFindCnt++;
//					sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii]);
//					LOGD(buffer);

					if (pMergeImage[jj][ii] == V1) {
						bFindPore = true;
						break;

					}
					if (pMergeImage[jj][ii] == nRidgesEnding) {
						bFind = true;
						break;
					}
				}
			}//for (nIDx = 0; nIDx < _SEARCH_SIZE_; nIDx++)

			if (nNotFindCnt == _SEARCH_SIZE_) {
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				//LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bFindPore == true) {
				bFindPore = false;
				break;
			}

			if (bNotFind == true) {
				bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				bValid = true;
				break;
			}//if (bFind == true)

			 //超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFindRidge == true) {
				bFindRidge = false;
				nRidgeFindCnt++;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nWidth - 1) {
					for (int nn = 0; nn < _SEARCH_SIZE_; nn++) {
						if (nSx + SearchRange[nn] == nWidth - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				//LOGD(buffer);

				//更新下次的 X 座標
				nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);

				nNotFindCnt = 0;
			}//if (bFindRidge == true)
		}//for (jj = nSy; jj >= 0; )

//		 sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nRidgeFindCnt = %d / (%d, %d) / (%d, %d)\r\n", __FUNCTION__, __LINE__, nRidgeFindCnt, i, j, ii, jj);
//		 LOGD(buffer);

		if (bValid == true) {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d OOO OOO [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);
		}
		else {
//			sprintf(buffer, "[FPTexture.c] %s() #Line %d XXX XXX [KennyKang] (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
//			LOGD(buffer);

			//將長度門檻未達標準的變更旗標為0
			pMergeImage[j][i] = R1;
		}
	}
}

/*Save Text File Utility*/
void SaveTXT(char *filename, unsigned char**pData, int nWidth, int nHeight)
{
	if (filename == NULL || pData == NULL || nWidth == 0 || nHeight == 0)
		return;

	int i, j;
	char str[128] = { 0 };
	char pStrPath[256] = { 0 };
	sprintf(pStrPath, "%s%s", strGlobalPath, filename);
	FILE *file = fopen(pStrPath, "w");
	if (!file) {
		return;
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == nWidth - 1) {
				sprintf(str, "%4d,\n", pData[j][i]);
			}
			else {
				sprintf(str, "%4d,", pData[j][i]);
			}
			fwrite(str, 1, strlen(str), file);
		}
	}
	fclose(file);
}

void Save2ByteTXT(char *filename, short**pData, int nWidth, int nHeight)
{
	if (filename == NULL || pData == NULL || nWidth == 0 || nHeight == 0)
		return;

	int i, j;
	char str[128] = { 0 };
	char pStrPath[256] = { 0 };
	sprintf(pStrPath, "%s%s", strGlobalPath, filename);
	//char buffer[1000] = { 0 };
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d pStrPath = %s\r\n", __FUNCTION__, __LINE__, pStrPath);
	//LOGD(buffer);

	FILE *file = fopen(pStrPath, "w");
	if (!file) {
		return;
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == nWidth - 1) {
				sprintf(str, "%4d,\n", pData[j][i]);
			}
			else {
				sprintf(str, "%4d,", pData[j][i]);
			}
			fwrite(str, 1, strlen(str), file);
		}
	}
	fclose(file);
}

/*Get Org Texture Differential Data*/
void GetTextureHorizontalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight)
{
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2dInData[j][i] - 128;
			p2dOutData[j][i] = sbuf;
		}
	}
}

void GetTextureVerticalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight)
{
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	int i, j;
	short sbuf;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2dInData[j][i] - 128;
			p2dOutData[j][i] = sbuf;
		}
	}
}

void GetTextureVerticalReduceDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight)
{
	if (p2dInData == NULL || p2dOutData == NULL)
		return;

	int i, j;
	short sbuf;
	//將 Reduce Differential 資料的上下邊緣補上 第 (1) 條 與 第 (nHeight-1) 條 數據值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0)			//第 (0)  列資料拿 第 (1) 列資料回補
				p2dInData[j][i] = p2dInData[j + 1][i];
			if(j == nHeight - 1)//第 (nHeight-1)  列資料拿 (nHeight-2) 列資料回補
				p2dInData[j][i] = p2dInData[j - 1][i];
		}
	}

	SaveTXT("GetTextureVerticalReduceDifferentialData.csv", p2dInData, nWidth, nHeight);
	
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2dInData[j][i] - 128;
			p2dOutData[j][i] = sbuf;
		}
	}
}

int xVFComputeLineDirection(int x1, int y1, int x2, int y2)
{
	int nDir;
	unsigned char dir;
	char buffer[1000] = { 0 };

	int dx = x2 - x1;
	int dy = y2 - y1;
	int dxAbs = xVFAbs(dx);
	int dyAbs = xVFAbs(dy);
	int maxAbs = xVFMax(dxAbs, dyAbs);
	int maxAbsHalf = maxAbs >> 1;
	if (maxAbs >= xVF_ATANS_MAX)
	{
		dxAbs = (dxAbs * (xVF_ATANS_MAX - 1) + maxAbsHalf) / maxAbs;
		dyAbs = (dyAbs * (xVF_ATANS_MAX - 1) + maxAbsHalf) / maxAbs;
	}
	dir = _vfAtans_[dyAbs][dxAbs];
	if (dx > 0)
	{
		if (dy < 0) dir = xVFDIR_360 - dir;
	}
	else
		if (dy > 0) dir = xVFDIR_180 - dir;
		else dir += xVFDIR_180;
		if (dir == xVFDIR_360) dir = xVFDIR_0;

		nDir = (int) (dir * 1.5);
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d dir = %d / nDir = %d\r\n", __FUNCTION__, __LINE__, dir, nDir);
		//LOGD(buffer);
		return nDir;
}

/* Pore Function */
void CheckingDisquisePore(unsigned char **pMergeImage, int nWidth, int nHeight)
{
	if (pMergeImage == NULL)
		return;

	char buffer[1000] = { 0 };
	int i, j, nSx, nSy, nBlockType = -1;
	//signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1) };	 //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	signed char VerticalInValidRow[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE] = { 0 };	 //保存垂直方向 Row 的 Bad Block 位置 /* KennyKang20190619 Modify */
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1, D5 = 86;
	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	unsigned char nDisPore = 251;		//偽汗孔標記號
	unsigned char nPore = 121;			//汗孔標記號
	unsigned char nMergePore = 122;		//汗孔標記號合併
	unsigned char nProcessPore = 123;	//汗孔標記號處理過
	unsigned char nFinalPore = 124;		//汗孔標記號確認
	unsigned char nMultiPore = 125;		//汗孔標記號多個

	/*尋找垂直方向的分界線位置*/
	for (i = 0; i < EMC_SENSOR_SIZE / EMC_BLOCK_SIZE; i++) {
		VerticalInValidRow[i] = g_nRow_Y;//紀錄無效區域的Y位置
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == V1) {
				if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1) {
					nBlockType = EMC_BLOCK_BAD;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
					//LOGD(buffer);

					nSy = j;

					//往右確認
					nSx = i + 1;
					if (nSx > nWidth - 1)
						nSx = nWidth - 1;
					if (pMergeImage[nSy][nSx] == nV2 ||
						pMergeImage[nSy][nSx] == (nV2 - 100))
					{
						pMergeImage[j][i] = nDisPore;
					}

					//往左確認
					nSx = i - 1;
					if (nSx < 0)
						nSx = 0;
					if (pMergeImage[nSy][nSx] == nV2 ||
						pMergeImage[nSy][nSx] == (nV2 - 100))
					{
						pMergeImage[j][i] = nDisPore;
					}
				}
				else {
					nBlockType = EMC_BLOCK_GOOD;
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
					//LOGD(buffer);

					nSx = i;

					//往下確認
					nSy = j + 1;
					if (nSy > nHeight - 1)
						nSy = nHeight - 1;
					if (pMergeImage[nSy][nSx] == nV2 || 
						pMergeImage[nSy][nSx] == (nV2 - 100))
					{
						pMergeImage[j][i] = nDisPore;
					}

					//往上確認
					nSy = j - 1;
					if (nSy < 0)
						nSy = 0;
					if (pMergeImage[nSy][nSx] == nV2 ||
						pMergeImage[nSy][nSx] == (nV2 - 100))
					{
						pMergeImage[j][i] = nDisPore;
					}
				}	
			}//if (pMergeImage[j][i] == V1) {
		}//for (i = 0; i < nWidth; i++) {
	}//for (j = 0; j < nHeight; j++) {

	int nCnt = 0;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == V1) {
				nCnt++;
				SearchPore(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight);
			}
		}
	}
	//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nCnt = %d\r\n", __FUNCTION__, __LINE__, nCnt);
	//LOGD(buffer);

	int nPoreCnt = 0;
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nPore) {
				nPoreCnt++;
				SelectPore(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight);
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nPoreCnt = %d\r\n", __FUNCTION__, __LINE__, nPoreCnt);
	LOGD(buffer);

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nMergePore) {
				SelectMergePore(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight);
			}
		}
	}

	GetPoreDirection(pMergeImage, VerticalInValidRow, nWidth, nHeight);
}

void SearchPore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;

	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nIDx, nCurrentIDx = 0, nNotFindCnt = 0, nLenCnt = 0, nCheckcnt = 0, nBlockType = -1, nPriorityIDx = -1;
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	signed char PoreSearchRange[PORE_RANGE_SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;
	unsigned char R1 = 0, R2 = 1, D5 = 86, E0 = 170, V1 = 255, V2 = 254, D1 = 85, D2 = 84;
	unsigned char V3 = 253, D3 = 83;    //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;    //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244; //Bounding Valleys標記號
	unsigned char nValleysEnding = 101;	//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	unsigned char nDisPore = 251;		//偽汗孔標記號
	unsigned char V5 = 250;				//處理過汗孔的標記號
	unsigned char V6 = 249;				//符合第一階段汗孔的標記號
	unsigned char nPore = 121;			//汗孔標記號
	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false, bValid = false, bFindRidge = false, bFindValley = false, bPore = false, bValleyBreak = false, bBlockBreak = false, bPriority = false;
	bool bUpPore = false, bDownPore = false, bLeftPore = false, bRightPore = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;

	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Start ... (%d, %d)\r\n", __FUNCTION__, __LINE__, i, j);
	LOGD(buffer);

	/* 定位與並標記為處理過的汗孔標記號 */
	pMergeImage[j][i] = V5;

	if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1) {
		nBlockType = EMC_BLOCK_BAD;
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
		//LOGD(buffer);

		jj = nSy;
checkRight:
		ii = nSx + 1;
		if (ii > nWidth - 1)
			goto exit;
		if (pMergeImage[jj][ii] == R1 ||
			pMergeImage[jj][ii] == R2 ||
			pMergeImage[jj][ii] == R3 ||
			pMergeImage[jj][ii] == R4 ||
			pMergeImage[jj][ii] == (R2 + 50) ||
			pMergeImage[jj][ii] == (R3 + 50) ||
			pMergeImage[jj][ii] == (R4 + 50) ||
			pMergeImage[jj][ii] == nR2 ||
			pMergeImage[jj][ii] == (nR2 + 50))
		{
			bRightPore = true;
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Right] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
		}
		else if (pMergeImage[jj][ii] == D1 ||
				 pMergeImage[jj][ii] == D2 ||
				 pMergeImage[jj][ii] == D3 ||
				 pMergeImage[jj][ii] == D4 ||
				 pMergeImage[jj][ii] == D5 ||
				 pMergeImage[jj][ii] == D6 ||
				 pMergeImage[jj][ii] == D7 ||
				 pMergeImage[jj][ii] == (D2 - 10) ||
				 pMergeImage[jj][ii] == (D3 - 10) ||
				 pMergeImage[jj][ii] == (D4 - 10) ||
				 pMergeImage[jj][ii] == (D5 - 10) ||
				 pMergeImage[jj][ii] == (D6 - 10) ||
				 pMergeImage[jj][ii] == (D7 - 10) ||
				 pMergeImage[jj][ii] == V1 ||
				 pMergeImage[jj][ii] == V5 ||
				 pMergeImage[jj][ii] == V6 ||
				 pMergeImage[jj][ii] == nPore)
		{
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Right] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
			if (nCheckcnt == nLenTh)
				goto exit;
			nSx = ii;
			goto checkRight;
		}
		else if (pMergeImage[jj][ii] == V2 ||
				 pMergeImage[jj][ii] == V3 ||
				 pMergeImage[jj][ii] == V4 ||
				 pMergeImage[jj][ii] == (V2 - 100) ||
				 pMergeImage[jj][ii] == (V3 - 100) ||
				 pMergeImage[jj][ii] == (V4 - 100) ||
				 pMergeImage[jj][ii] == nV2 ||
				 pMergeImage[jj][ii] == (nV2 - 100) ||
			 	 pMergeImage[jj][ii] == nValleysEnding ||
				 pMergeImage[jj][ii] == nDisPore)
		{
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Right] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
			goto exit;
		}
		else {
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Right] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
		}

		if (bRightPore == true) {
			bRightPore = false;
			nCheckcnt = 0;
			nSx = *nStartX;

checkLeft:
			ii = nSx - 1;
			if (ii < 0)
				goto exit;
			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50))
			{
				bLeftPore = true;
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Left] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
			}
			else if (pMergeImage[jj][ii] == D1 ||
					 pMergeImage[jj][ii] == D2 ||
					 pMergeImage[jj][ii] == D3 ||
					 pMergeImage[jj][ii] == D4 ||
					 pMergeImage[jj][ii] == D5 ||
					 pMergeImage[jj][ii] == D6 ||
					 pMergeImage[jj][ii] == D7 ||
					 pMergeImage[jj][ii] == (D2 - 10) ||
					 pMergeImage[jj][ii] == (D3 - 10) ||
					 pMergeImage[jj][ii] == (D4 - 10) ||
					 pMergeImage[jj][ii] == (D5 - 10) ||
					 pMergeImage[jj][ii] == (D6 - 10) ||
					 pMergeImage[jj][ii] == (D7 - 10) ||
					 pMergeImage[jj][ii] == V1 ||
					 pMergeImage[jj][ii] == V5 ||
					 pMergeImage[jj][ii] == V6 ||
					 pMergeImage[jj][ii] == nPore)
			{
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Left] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
				if (nCheckcnt == nLenTh)
					goto exit;
				nSx = ii;
				goto checkLeft;
			}
			else if (pMergeImage[jj][ii] == V2 ||
					 pMergeImage[jj][ii] == V3 ||
					 pMergeImage[jj][ii] == V4 ||
					 pMergeImage[jj][ii] == (V2 - 100) ||
					 pMergeImage[jj][ii] == (V3 - 100) ||
					 pMergeImage[jj][ii] == (V4 - 100) ||
					 pMergeImage[jj][ii] == nV2 ||
					 pMergeImage[jj][ii] == (nV2 - 100) ||
					 pMergeImage[jj][ii] == nValleysEnding ||
					 pMergeImage[jj][ii] == nDisPore)
			{
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Left] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
				goto exit;
			}
			else {
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Left] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
			}
		}//if (bRightPore == true) {

		if (bLeftPore == true) {
			bLeftPore = false;
			//*************************
			nCheckcnt = 0;
			pMergeImage[j][i] = V6;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Left] nBlockType = %d / (%d, %d) / [IsPore] ***\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
			LOGD(buffer);

			/* 重新載入 nSx / nSy 數值 */
			nSx = *nStartX;
			nSy = *nStartY;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** (%d, %d) / pMergeImage[%d][%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSx, nSy, j, i, pMergeImage[j][i]);
			LOGD(buffer);

			/* 向下尋找下一點 */
			for (jj = nSy; jj < nHeight; ) {
				jj = jj + 1;	//將 Y 座標下移
				if (jj == nHeight)
					break;

				for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {
					if (nSx + PoreSearchRange[nIDx] < 0) {
						ii = 0;
						bIsEdge = true;//超過 X 方向的左邊界限制，可不繼續做下去
					}
					else if (nSx + PoreSearchRange[nIDx] > nWidth - 1) {
						ii = nWidth - 1;
						bIsEdge = true;//超過 X 方向的右邊界限制，可不繼續做下去
					}
					else
						ii = nSx + PoreSearchRange[nIDx];

					sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, jj);
					LOGD(buffer);

					if (nBlockType == EMC_BLOCK_BAD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Bad限制線就停止不做
							if (jj < nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}

					if (pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10) ||
						pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V5 ||
						pMergeImage[jj][ii] == V6 ||
						pMergeImage[jj][ii] == nPore)
					{
						if (bFind == false)
							bFind = true;
						if (pMergeImage[jj][ii] == (D2 - 10) ||
							pMergeImage[jj][ii] == (D3 - 10) ||
							pMergeImage[jj][ii] == (D4 - 10) ||
							pMergeImage[jj][ii] == (D5 - 10) ||
							pMergeImage[jj][ii] == (D6 - 10) ||
							pMergeImage[jj][ii] == (D7 - 10))
						{
							bPriority = true;
							nPriorityIDx = nIDx;//優先
						}
						else {
							nCurrentIDx = nIDx;//保存最後符合條件的Index
						}
						sprintf(buffer, "[FPTexture.c] %s() #Line %d pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
					else if (pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
							 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == nValleysEnding ||
							 pMergeImage[jj][ii] == nDisPore)
					{
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [Valleys] pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
						bFindValley = true;
						break;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {

				if (nNotFindCnt == PORE_RANGE_SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bFindValley == true) {
					bFindValley = false;
					bValleyBreak = true;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bBlockBreak = true;
					break;
				}

				//超過 X 的左右邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
					//break;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
					
					if (bPriority == true) {
						nCurrentIDx = nPriorityIDx;
						bPriority = false;
						nPriorityIDx = -1;
					}

					int nUpdateIDx = 0;
					if (nSx + PoreSearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSx + PoreSearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSx + PoreSearchRange[nCurrentIDx] > nWidth - 1) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSx + PoreSearchRange[nn] == nWidth - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);

					//更新下次的 X 座標
					nSx = nSx + PoreSearchRange[nCurrentIDx];
				}//if (bFind == true)

			}//for (jj = nSy; jj < nHeight; ) {

			if (bValleyBreak == true) {
				bValleyBreak = false;
				goto exit;
			}

			if (bBlockBreak == true) {
				bBlockBreak = false;
				goto exit;
			}

			 /* 初始化變數 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			bFindValley = false;
			bValleyBreak = false;
			bBlockBreak = false;
			bPriority = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nPriorityIDx = -1;

			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Down-Left_Right] / (%d, %d) / nLenCnt = %d ***\r\n", __FUNCTION__, __LINE__, i, j, nLenCnt);
			LOGD(buffer);

			/* 重新載入 nSx / nSy 數值 */
			nSx = *nStartX;
			nSy = *nStartY;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** (%d, %d) / pMergeImage[%d][%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSx, nSy, j, i, pMergeImage[j][i]);
			LOGD(buffer);

			/* 向上尋找下一點 */
			for (jj = nSy; jj >= 0; ) {
				jj = jj - 1;	//將 Y 座標上移
				if (jj < 0)
					break;

				for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {
					if (nSx + PoreSearchRange[nIDx] < 0) {
						ii = 0;
						bIsEdge = true;//超過 X 方向的左邊界限制，可不繼續做下去
					}
					else if (nSx + PoreSearchRange[nIDx] > nWidth - 1) {
						ii = nWidth - 1;
						bIsEdge = true;//超過 X 方向的右邊界限制，可不繼續做下去
					}
					else
						ii = nSx + PoreSearchRange[nIDx];

					sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, jj);
					LOGD(buffer);

					if (nBlockType == EMC_BLOCK_BAD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Bad限制線就停止不做
							if (jj < nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}

					if (pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10) ||
						pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V5 ||
						pMergeImage[jj][ii] == V6 ||
						pMergeImage[jj][ii] == nPore)
					{
						if (bFind == false)
							bFind = true;
						if (pMergeImage[jj][ii] == (D2 - 10) ||
							pMergeImage[jj][ii] == (D3 - 10) ||
							pMergeImage[jj][ii] == (D4 - 10) ||
							pMergeImage[jj][ii] == (D5 - 10) ||
							pMergeImage[jj][ii] == (D6 - 10) ||
							pMergeImage[jj][ii] == (D7 - 10)) 
						{
							bPriority = true;
							nPriorityIDx = nIDx;//優先
						}
						else {
							nCurrentIDx = nIDx;//保存最後符合條件的Index
						}
						sprintf(buffer, "[FPTexture.c] %s() #Line %d pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
					else if (pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
						 	 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == nValleysEnding ||
							 pMergeImage[jj][ii] == nDisPore)
					{
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [Valleys] pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
						bFindValley = true;
						break;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {

				if (nNotFindCnt == PORE_RANGE_SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;	
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bFindValley == true) {
					bFindValley = false;
					bValleyBreak = true;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bBlockBreak = true;
					break;
				}

				//超過 X 的左右邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
					//break;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
				
					if (bPriority == true) {
						nCurrentIDx = nPriorityIDx;
						bPriority = false;
						nPriorityIDx = -1;
					}

					int nUpdateIDx = 0;
					if (nSx + PoreSearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSx + PoreSearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSx + PoreSearchRange[nCurrentIDx] > nWidth - 1) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSx + PoreSearchRange[nn] == nWidth - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);

					//更新下次的 X 座標
					nSx = nSx + PoreSearchRange[nCurrentIDx];
				}//if (bFind == true)

			}//for (jj = nSy; jj >= 0; ) {

			if (bValleyBreak == true) {
				bValleyBreak = false;
				goto exit;
			}

			if (bBlockBreak == true) {
				bBlockBreak = false;
				goto exit;
			}

			nLenCnt = nLenCnt + 1;//自己也要算一步，最後補回去
			sprintf(buffer, "[FPTexture.c] %s() #Line %d nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nLenCnt);
			LOGD(buffer);
			if (nLenCnt <= nLenTh)
				bLength = true;
			else
				bLength = false;

			if (bLength == true) {
				bLength = false;
				bPore = true;
			}

			if (bPore == true) {
				bPore = false;
				//標記為汗孔
				pMergeImage[j][i] = nPore;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Up-Left_Right] / (%d, %d) / [IsPore] ***\r\n", __FUNCTION__, __LINE__, i, j);
				LOGD(buffer);
			}//if (bPore == true) {
		}//if (bLeftPore == true) {
	}
	else {
		nBlockType = EMC_BLOCK_GOOD;
		//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d)\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
		//LOGD(buffer);

		ii = nSx;
checkDown:
		jj = nSy + 1;
		if (jj > nHeight - 1)
			goto exit;
		if (pMergeImage[jj][ii] == R1 ||
			pMergeImage[jj][ii] == R2 ||
			pMergeImage[jj][ii] == R3 ||
			pMergeImage[jj][ii] == R4 ||
			pMergeImage[jj][ii] == (R2 + 50) ||
			pMergeImage[jj][ii] == (R3 + 50) ||
			pMergeImage[jj][ii] == (R4 + 50) ||
			pMergeImage[jj][ii] == nR2 ||
			pMergeImage[jj][ii] == (nR2 + 50))
		{
			bDownPore = true;
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Down] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
		}
		else if (pMergeImage[jj][ii] == D1 ||
			pMergeImage[jj][ii] == D2 ||
			pMergeImage[jj][ii] == D3 ||
			pMergeImage[jj][ii] == D4 ||
			pMergeImage[jj][ii] == D5 ||
			pMergeImage[jj][ii] == D6 ||
			pMergeImage[jj][ii] == D7 ||
			pMergeImage[jj][ii] == (D2 - 10) ||
			pMergeImage[jj][ii] == (D3 - 10) ||
			pMergeImage[jj][ii] == (D4 - 10) ||
			pMergeImage[jj][ii] == (D5 - 10) ||
			pMergeImage[jj][ii] == (D6 - 10) ||
			pMergeImage[jj][ii] == (D7 - 10) ||
			pMergeImage[jj][ii] == V1 ||
			pMergeImage[jj][ii] == V5 ||
			pMergeImage[jj][ii] == V6 ||
			pMergeImage[jj][ii] == nPore)
		{
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Down] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
			if (nCheckcnt == nLenTh)
				goto exit;
			nSy = jj;
			goto checkDown;
		}
		else if (pMergeImage[jj][ii] == V2 ||
			pMergeImage[jj][ii] == V3 ||
			pMergeImage[jj][ii] == V4 ||
			pMergeImage[jj][ii] == (V2 - 100) ||
			pMergeImage[jj][ii] == (V3 - 100) ||
			pMergeImage[jj][ii] == (V4 - 100) ||
			pMergeImage[jj][ii] == nV2 ||
			pMergeImage[jj][ii] == (nV2 - 100) ||
			pMergeImage[jj][ii] == nValleysEnding ||
			pMergeImage[jj][ii] == nDisPore)
		{
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Down] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
			goto exit;
		}
		else {
			nCheckcnt++;
			//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Down] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
			//LOGD(buffer);
		}

		if (bDownPore == true) {
			bDownPore = false;
			nCheckcnt = 0;
			nSy = *nStartY;
checkUp:
			jj = nSy - 1;
			if (jj < 0)
				goto exit;
			if (pMergeImage[jj][ii] == R1 ||
				pMergeImage[jj][ii] == R2 ||
				pMergeImage[jj][ii] == R3 ||
				pMergeImage[jj][ii] == R4 ||
				pMergeImage[jj][ii] == (R2 + 50) ||
				pMergeImage[jj][ii] == (R3 + 50) ||
				pMergeImage[jj][ii] == (R4 + 50) ||
				pMergeImage[jj][ii] == nR2 ||
				pMergeImage[jj][ii] == (nR2 + 50))
			{
				bUpPore = true;
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Up] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
			}
			else if (pMergeImage[jj][ii] == D1 ||
				pMergeImage[jj][ii] == D2 ||
				pMergeImage[jj][ii] == D3 ||
				pMergeImage[jj][ii] == D4 ||
				pMergeImage[jj][ii] == D5 ||
				pMergeImage[jj][ii] == D6 ||
				pMergeImage[jj][ii] == D7 ||
				pMergeImage[jj][ii] == (D2 - 10) ||
				pMergeImage[jj][ii] == (D3 - 10) ||
				pMergeImage[jj][ii] == (D4 - 10) ||
				pMergeImage[jj][ii] == (D5 - 10) ||
				pMergeImage[jj][ii] == (D6 - 10) ||
				pMergeImage[jj][ii] == (D7 - 10) ||
				pMergeImage[jj][ii] == V1 ||
				pMergeImage[jj][ii] == V5 ||
				pMergeImage[jj][ii] == V6 ||
				pMergeImage[jj][ii] == nPore)
			{
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Up] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
				if (nCheckcnt == nLenTh)
					goto exit;
				nSy = jj;
				goto checkUp;
			}
			else if (pMergeImage[jj][ii] == V2 ||
				pMergeImage[jj][ii] == V3 ||
				pMergeImage[jj][ii] == V4 ||
				pMergeImage[jj][ii] == (V2 - 100) ||
				pMergeImage[jj][ii] == (V3 - 100) ||
				pMergeImage[jj][ii] == (V4 - 100) ||
				pMergeImage[jj][ii] == nV2 ||
				pMergeImage[jj][ii] == (nV2 - 100) ||
				pMergeImage[jj][ii] == nValleysEnding ||
				pMergeImage[jj][ii] == nDisPore)
			{
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Up] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
				goto exit;
			}
			else {
				nCheckcnt++;
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] [Up] nBlockType = %d /  pMergeImage[%d][%d] = %d / nCheckcnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, jj, ii, pMergeImage[jj][ii], nCheckcnt);
				//LOGD(buffer);
			}
		}//if (bDownPore == true)

		if (bUpPore == true) {
			bUpPore = false;
			//*************************
			nCheckcnt = 0;
			pMergeImage[j][i] = V6;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Up] nBlockType = %d / (%d, %d) / [IsPore] ***\r\n", __FUNCTION__, __LINE__, nBlockType, i, j);
			LOGD(buffer);

			/* 重新載入 nSx / nSy 數值 */
			nSx = *nStartX;
			nSy = *nStartY;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** (%d, %d) / pMergeImage[%d][%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSx, nSy, j, i, pMergeImage[j][i]);
			LOGD(buffer);
#if 1
			/* 向右尋找下一點 */
			for (ii = nSx; ii < nWidth; ) {
				ii = ii + 1; //將 X 座標右移
				if (ii == nWidth)
					break;

				for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {
					if (nSy + PoreSearchRange[nIDx] < 0) {
						jj = 0;
						bIsEdge = true;//超過 Y 方向的上邊界限制
					}
					else if(nSy + PoreSearchRange[nIDx] > nHeight - 1) {
						jj = nHeight - 1;
						bIsEdge = true;//超過 Y 方向的下邊界限制
					}
					else
						jj = nSy + PoreSearchRange[nIDx];

					sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, jj);
					LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Good限制線就停止不做
							if (jj >= nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_GOOD)

					if (pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10) ||
						pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V5 ||
						pMergeImage[jj][ii] == V6 ||
						pMergeImage[jj][ii] == nPore)
					{
						if (bFind == false)
							bFind = true;
						if (pMergeImage[jj][ii] == (D2 - 10) ||
							pMergeImage[jj][ii] == (D3 - 10) ||
							pMergeImage[jj][ii] == (D4 - 10) ||
							pMergeImage[jj][ii] == (D5 - 10) ||
							pMergeImage[jj][ii] == (D6 - 10) ||
							pMergeImage[jj][ii] == (D7 - 10)) 
						{
							bPriority = true;
							nPriorityIDx = nIDx;//優先
						}
						else {
							nCurrentIDx = nIDx;//保存最後符合條件的Index
						}
						sprintf(buffer, "[FPTexture.c] %s() #Line %d pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
					else if (pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
							 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == nValleysEnding ||
							 pMergeImage[jj][ii] == nDisPore)
					{
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [Valleys] pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
						bFindValley = true;
						break;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {

				if (nNotFindCnt == PORE_RANGE_SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}
	
				if (bFindValley == true) {
					bFindValley = false;
					bValleyBreak = true;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bBlockBreak = true;
					break;
				}

				//超過 Y 的下邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加

					if (bPriority == true) {
						nCurrentIDx = nPriorityIDx;
						bPriority = false;
						nPriorityIDx = -1;
					}
					
					int nUpdateIDx = 0;
					if (nSy + PoreSearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSy + PoreSearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSy + PoreSearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSy + PoreSearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);

					//更新下次的 Y 座標
					nSy = nSy + PoreSearchRange[nCurrentIDx];
				}//if (bFind == true)

			}//for (ii = nSx; ii < nWidth; )

			if (bValleyBreak == true) {
				bValleyBreak = false;
				goto exit;
			}

			if (bBlockBreak == true) {
				bBlockBreak = false;
				goto exit;
			}

			/* 初始化變數 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			bFindValley = false;
			bValleyBreak = false;
			bBlockBreak = false;
			bPriority = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nPriorityIDx = -1;

			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Right-Down_Up] / (%d, %d) / nLenCnt = %d ***\r\n", __FUNCTION__, __LINE__, i, j, nLenCnt);
			LOGD(buffer);

			/* 重新載入 nSx / nSy 數值 */
			nSx = *nStartX;
			nSy = *nStartY;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** (%d, %d) / pMergeImage[%d][%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSx, nSy, j, i, pMergeImage[j][i]);
			LOGD(buffer);

			/* 向左尋找下一點 */
			for (ii = nSx; ii >= 0; ) {
				ii = ii - 1;	//將 X 座標左移
				if (ii < 0)
					break;

				for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {
					if (nSy + PoreSearchRange[nIDx] < 0) {
						jj = 0;
						bIsEdge = true;//超過 Y 方向的上邊界限制
					}
					else if (nSy + PoreSearchRange[nIDx] > nHeight - 1) {
						jj = nHeight - 1;
						bIsEdge = true;//超過 Y 方向的下邊界限制
					}
					else
						jj = nSy + PoreSearchRange[nIDx];

					sprintf(buffer, "[FPTexture.c] %s() #Line %d --> (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, jj);
					LOGD(buffer);

					if (nBlockType == EMC_BLOCK_GOOD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Good限制線就停止不做
							if (jj >= nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_GOOD)

					if (pMergeImage[jj][ii] == D1 ||
						pMergeImage[jj][ii] == D2 ||
						pMergeImage[jj][ii] == D3 ||
						pMergeImage[jj][ii] == D4 ||
						pMergeImage[jj][ii] == D5 ||
						pMergeImage[jj][ii] == D6 ||
						pMergeImage[jj][ii] == D7 ||
						pMergeImage[jj][ii] == (D2 - 10) ||
						pMergeImage[jj][ii] == (D3 - 10) ||
						pMergeImage[jj][ii] == (D4 - 10) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10) ||
						pMergeImage[jj][ii] == V1 ||
						pMergeImage[jj][ii] == V5 ||
						pMergeImage[jj][ii] == V6)
					{

						if (bFind == false)
							bFind = true;
						if (pMergeImage[jj][ii] == (D2 - 10) ||
							pMergeImage[jj][ii] == (D3 - 10) ||
							pMergeImage[jj][ii] == (D4 - 10) ||
							pMergeImage[jj][ii] == (D5 - 10) ||
							pMergeImage[jj][ii] == (D6 - 10) ||
							pMergeImage[jj][ii] == (D7 - 10)) 
						{
							bPriority = true;
							nPriorityIDx = nIDx;//優先
						}
						else {
							nCurrentIDx = nIDx;//保存最後符合條件的Index
						}
						sprintf(buffer, "[FPTexture.c] %s() #Line %d pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
					else if (pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
							 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == nValleysEnding ||
							 pMergeImage[jj][ii] == nDisPore)
					{
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [Valleys] pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
						bFindValley = true;
						break;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < PORE_RANGE_SEARCH_SIZE; nIDx++) {

				if (nNotFindCnt == PORE_RANGE_SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bFindValley == true) {
					bFindValley = false;
					bValleyBreak = true;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bBlockBreak = true;
					break;
				}

				//超過 Y 的下邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
				}

				if (bFind == true) {
					bFind = false;

					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加

					if (bPriority == true) {
						nCurrentIDx = nPriorityIDx;
						bPriority = false;
						nPriorityIDx = -1;
					}
					
					int nUpdateIDx = 0;
					if (nSy + PoreSearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSy + PoreSearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSy + PoreSearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < PORE_RANGE_SEARCH_SIZE; nn++) {
							if (nSy + PoreSearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);

					//更新下次的 Y 座標
					nSy = nSy + PoreSearchRange[nCurrentIDx];
				}//if (bFind == true)

			}//for (ii = nSx; ii >= 0; ) {

			if (bValleyBreak == true) {
				bValleyBreak = false;
				goto exit;
			}
			
			if (bBlockBreak == true) {
				bBlockBreak = false;
				goto exit;
			}

			nLenCnt = nLenCnt + 1;//自己也要算一步，最後補回去
			sprintf(buffer, "[FPTexture.c] %s() #Line %d nLenCnt = %d\r\n", __FUNCTION__, __LINE__,nLenCnt);
			LOGD(buffer);
			if (nLenCnt <= nLenTh)
				bLength = true;
			else
				bLength = false;

			if (bLength == true) {
				bLength = false;
				bPore = true;
			}

			if (bPore == true) {
				bPore = false;
				//標記為汗孔
				pMergeImage[j][i] = nPore;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** [Left-Down_Up] / (%d, %d) / [IsPore] ***\r\n", __FUNCTION__, __LINE__, i, j);
				LOGD(buffer);
			}//if (bPore == true) {
#endif
		}//if (bUpPore == true) {
	}
exit:
	nLenCnt = 0;
}

void SelectPore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL)
		return;

	char buffer[1000] = { 0 };
	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, 3, 4 };
	bool bCheckBox[SEARCH_SIZE] = { false, false , false, false , false };
	int i, j, ii, jj, nSx, nSy, nBlockType = -1, nCheckCnt = 0, nIDx = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nPore = 121;			//汗孔標記號
	unsigned char nMergePore = 122;		//汗孔標記號合併
	unsigned char nProcessPore = 123;	//汗孔標記號處理過

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;

	if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1) {
		nBlockType = EMC_BLOCK_BAD;
		//先處理左右
		for (int ii = 0; ii < SEARCH_SIZE; ii++) {
			if (nSx + ii > nWidth - 1)
				goto exit;
			if (pMergeImage[nSy][nSx] == pMergeImage[nSy][nSx + ii]) {
				bCheckBox[ii] = true;
				nCheckCnt++;
			}
		}
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d) / nCheckCnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, i, j, nCheckCnt);
		LOGD(buffer);

		if (nCheckCnt >= 1) {
			nIDx = nCheckCnt / 2;
			for (ii = 0; ii < SEARCH_SIZE; ii++) {
				if (bCheckBox[ii] == true) {
					if (ii == nIDx) {
						pMergeImage[nSy][nSx + ii] = nMergePore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSy, nSx + ii, pMergeImage[nSy][nSx + ii]);
						LOGD(buffer);
					}
					else {
						pMergeImage[nSy][nSx + ii] = nProcessPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSy, nSx + ii, pMergeImage[nSy][nSx + ii]);
						LOGD(buffer);
					}
				}
			
			}
		
		}
		nCheckCnt = 0;
		nIDx = 0;
	}
	else {
		nBlockType = EMC_BLOCK_GOOD;
		//先處理上下
		for (jj = 0; jj < SEARCH_SIZE; jj++) {
			if (nSy + jj > nHeight - 1)
				goto exit;
			if (pMergeImage[nSy][nSx] == pMergeImage[nSy + jj][nSx]) {
				bCheckBox[jj] = true;
				nCheckCnt++;
			}
		}
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nBlockType = %d / (%d, %d) / nCheckCnt = %d\r\n", __FUNCTION__, __LINE__, nBlockType, i, j, nCheckCnt);
		LOGD(buffer);

		if (nCheckCnt >= 1) {
			nIDx = nCheckCnt / 2;
			for (jj = 0; jj < SEARCH_SIZE; jj++) {
				if (bCheckBox[jj] == true) {
					if (jj == nIDx) {
						pMergeImage[nSy + jj][nSx] = nMergePore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSy + jj, nSx, pMergeImage[nSy + jj][nSx]);
						LOGD(buffer);
					}
					else {
						pMergeImage[nSy + jj][nSx] = nProcessPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSy + jj, nSx, pMergeImage[nSy + jj][nSx]);
						LOGD(buffer);
					}
				}
			}
		}
		nCheckCnt = 0;
		nIDx = 0;
	}
exit:
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
	LOGD(buffer);
}

void SelectMergePore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL)
		return;

	char buffer[1000] = { 0 };
	signed char sRange[RANGE_SEARCH_SIZE] = { 0, 1, 2, 3, 4, -1, -2, -3, -4 };
	//signed char sMergeRange[MERGR_RANGE_SEARCH_SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, -1, -2, -3, -4, -5, -6, -7 };
	signed char bCheckBox[EMC_BLOCK_SIZE] = { -1, -1 , -1, -1 , -1, -1, -1, -1 };
	int i, j, ii, jj, nSx, nSy, nSi, nSj, nBlockType = -1, nCheckCnt = 0, nIDx = 0, nIDxCnt = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nPore = 121;			//汗孔標記號
	unsigned char nMergePore = 122;		//汗孔標記號合併
	unsigned char nProcessPore = 123;	//汗孔標記號處理過
	unsigned char nFinalPore = 124;		//汗孔標記號確認
	unsigned char nMultiPore = 125;		//汗孔標記號多個

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSy, nSx, pMergeImage[nSy][nSx]);
	LOGD(buffer);

	if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1) {
		nBlockType = EMC_BLOCK_BAD;
		//向下處理
		for (jj = 0; jj < OneBlockSize; jj++) {
			for (ii = 0; ii < RANGE_SEARCH_SIZE; ii++) {
				nSi = nSx + sRange[ii];
				//nSi = nSx + sMergeRange;
				nSj = nSy + jj;
				if (nSi > nWidth - 1 || nSi < 0 || nSj > nHeight - 1 || nSj < 0)
					goto exit;

				if (pMergeImage[nSj][nSi] == nMergePore || pMergeImage[nSj][nSi] == nFinalPore || pMergeImage[nSj][nSi] == nMultiPore) {
					nCheckCnt++;
					bCheckBox[jj] = ii;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** pMergeImage[%d][%d] = %d / bCheckBox[%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSj, nSi, pMergeImage[nSj][nSi], jj, bCheckBox[jj]);
					LOGD(buffer);
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, ii, jj, nSj, nSi, pMergeImage[nSj][nSi);
				//LOGD(buffer);
			}//for (ii = 0; ii < RANGE_SEARCH_SIZE; ii++) {
		}//for (jj = 0; jj < OneBlockSize; jj++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nCheckCnt = %d\r\n", __FUNCTION__, __LINE__, nCheckCnt);
		LOGD(buffer);
		if (nCheckCnt > 1) {
			nIDx = (nCheckCnt - 1) / 2;//取中心點
			for(jj = 0; jj < OneBlockSize; jj++) {
				if (bCheckBox[jj] != -1) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] bCheckBox[%d] = %d\r\n", __FUNCTION__, __LINE__, jj, bCheckBox[jj]);
					LOGD(buffer);
					if (nIDxCnt == nIDx) {
						pMergeImage[nSj][nSx + sRange[bCheckBox[jj]]] = nFinalPore;
						//pMergeImage[nSj][nSx + sMergeRange[bCheckBox[jj]]] = nFinalPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] @@@ (%d, %d) / nIDx = %d / nIDxCnt = %d pMergeImage[%d][%d] = %d @@@\r\n", __FUNCTION__, __LINE__, nSx + sRange[bCheckBox[jj]], nSj, nIDx, nIDxCnt, nSj, nSx + sRange[bCheckBox[jj]], pMergeImage[nSj][nSx + sRange[bCheckBox[jj]]]);
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] @@@ (%d, %d) / nIDx = %d / nIDxCnt = %d pMergeImage[%d][%d] = %d @@@\r\n", __FUNCTION__, __LINE__, nSx + sMergeRange[bCheckBox[jj]], nSj, nIDx, nIDxCnt, nSj, nSx + sMergeRange[bCheckBox[jj]], pMergeImage[nSj][nSx + sMergeRange[bCheckBox[jj]]]);
						LOGD(buffer);
					}
					else {
						pMergeImage[nSj][nSx + sRange[bCheckBox[jj]]] = nMultiPore;
						//pMergeImage[nSj][nSx + sMergeRange[bCheckBox[jj]]] = nMultiPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / nIDx = %d / nIDxCnt = %d / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx + sRange[bCheckBox[jj]], nSj, nIDx, nIDxCnt, nSj, nSx + sRange[bCheckBox[jj]], pMergeImage[nSj][nSx + sRange[bCheckBox[jj]]]);
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / nIDx = %d / nIDxCnt = %d / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx + sMergeRange[bCheckBox[jj]], nSj, nIDx, nIDxCnt, nSj, nSx + sMergeRange[bCheckBox[jj]], pMergeImage[nSj][nSx + sMergeRange[bCheckBox[jj]]]);
						LOGD(buffer);
					}
					nIDxCnt++;
				}
			}
		}
		else {
			pMergeImage[nSy][nSx] = nFinalPore;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSy, nSx, pMergeImage[nSy][nSx]);
			LOGD(buffer);
		}
	}
	else {
		nBlockType = EMC_BLOCK_GOOD;
		//向右處理
		for (ii = 0; ii < OneBlockSize; ii++) {
			for (jj = 0; jj < RANGE_SEARCH_SIZE; jj++) {
				nSi = nSx + ii;
				nSj = nSy + sRange[jj];
				//nSj = nSy + sMergeRange[jj];
				if (nSi > nWidth - 1 || nSi < 0 || nSj > nHeight - 1 || nSj < 0)
					goto exit;

				if (pMergeImage[nSj][nSi] == nMergePore || pMergeImage[nSj][nSi] == nFinalPore || pMergeImage[nSj][nSi] == nMultiPore) {
					nCheckCnt++;
					bCheckBox[ii] = jj;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] *** pMergeImage[%d][%d] = %d bCheckBox[%d] = %d ***\r\n", __FUNCTION__, __LINE__, nSj, nSi, pMergeImage[nSj][nSi], ii, bCheckBox[ii]);
					LOGD(buffer);
				}
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, ii, jj, nSj, nSi, pMergeImage[nSj][nSi]);
				//LOGD(buffer);
			}//for (jj = 0; jj < RANGE_SEARCH_SIZE; jj++) {
		}//for (ii = 0; ii < OneBlockSize; ii++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nCheckCnt = %d\r\n", __FUNCTION__, __LINE__, nCheckCnt);
		LOGD(buffer);
		if (nCheckCnt > 1) {
			nIDx = (nCheckCnt - 1) / 2;//取中心點
			for (ii = 0; ii < OneBlockSize; ii++) {
				if (bCheckBox[ii] != -1) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] bCheckBox[%d] = %d\r\n", __FUNCTION__, __LINE__, ii, bCheckBox[ii]);
					LOGD(buffer);
					if (nIDxCnt == nIDx) {
						pMergeImage[nSy + sRange[bCheckBox[ii]]][nSx + ii] = nFinalPore;
						//pMergeImage[nSy + sMergeRange[bCheckBox[ii]]][nSx + ii] = nFinalPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] @@@ (%d, %d) / nIDx = %d / nIDxCnt = %d /pMergeImage[%d][%d] = %d @@@\r\n", __FUNCTION__, __LINE__, nSx + ii, nSy + sRange[bCheckBox[ii]], nIDx, nIDxCnt, nSy + sRange[bCheckBox[ii]], nSx + ii, pMergeImage[nSy + sRange[bCheckBox[ii]]][nSx + ii]);
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] @@@ (%d, %d) / nIDx = %d / nIDxCnt = %d /pMergeImage[%d][%d] = %d @@@\r\n", __FUNCTION__, __LINE__, nSx + ii, nSy + sMergeRange[bCheckBox[ii]], nIDx, nIDxCnt, nSy + sMergeRange[bCheckBox[ii]], nSx + ii, pMergeImage[nSy + sMergeRange[bCheckBox[ii]]][nSx + ii]);
						LOGD(buffer);
					}
					else {
						pMergeImage[nSy + sRange[bCheckBox[ii]]][nSx + ii] = nMultiPore;
						//pMergeImage[nSy + sMergeRange[bCheckBox[ii]]][nSx + ii] = nMultiPore;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / nIDx = %d / nIDxCnt = %d /  pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx + ii, nSy + sRange[bCheckBox[ii]], nIDx, nIDxCnt, nSy + sRange[bCheckBox[ii]], nSx + ii, pMergeImage[nSy + sRange[bCheckBox[ii]]][nSx + ii]);
						//sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / nIDx = %d / nIDxCnt = %d /  pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx + ii, nSy + sMergeRange[bCheckBox[ii]], nIDx, nIDxCnt, nSy + sMergeRange[bCheckBox[ii]], nSx + ii, pMergeImage[nSy + sMergeRange[bCheckBox[ii]]][nSx + ii]);
						LOGD(buffer);
					}
					nIDxCnt++;
				}
			}
		}
		else {
			pMergeImage[nSy][nSx] = nFinalPore;
			sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] (%d, %d) / pMergeImage[%d][%d] = %d\r\n", __FUNCTION__, __LINE__, nSx, nSy, nSy, nSx, pMergeImage[nSy][nSx]);
			LOGD(buffer);
		}
	}
exit:
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang]\r\n", __FUNCTION__, __LINE__);
	LOGD(buffer);
	nIDxCnt = 0;
}

void GetPoreDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return;
	
	char buffer[1000] = { 0 };
	int i, j, nIDx = 0, nBlockType = -1, nPoreCnt = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE;
	unsigned char nPore = 121;			//汗孔標記號
	unsigned char nMergePore = 122;		//汗孔標記號合併
	unsigned char nProcessPore = 123;	//汗孔標記號處理過
	unsigned char nFinalPore = 124;		//汗孔標記號確認
	unsigned char nMultiPore = 125;		//汗孔標記號多個

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nFinalPore) {
				nPoreCnt++;
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nPoreCnt = %d\r\n", __FUNCTION__, __LINE__, nPoreCnt);
	LOGD(buffer);

	if (pPoreDirection != NULL) {
		free(pPoreDirection);
		pPoreDirection = NULL;
		nPoreDirectionCnt = -1;
	}

	nPoreDirectionCnt = nPoreCnt;
	if (nPoreCnt > 0) {
		pPoreDirection = (short *)malloc(sizeof(short) * nPoreCnt);
		memset(pPoreDirection, 0, sizeof(short) * nPoreCnt);
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nFinalPore) {
				if ((i > 0 && i < nWidth - 1) && (j > 1 && j < nHeight - 2)) {
					if (j >= VerticalInValidRow[i / OneBlockSize] * OneBlockSize && VerticalInValidRow[i / OneBlockSize] != -1)
						nBlockType = EMC_BLOCK_BAD;
					else
						nBlockType = EMC_BLOCK_GOOD;

					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Pore (%d, %d) / nBlockType = %d\r\n", __FUNCTION__, __LINE__, i, j, nBlockType);
					LOGD(buffer);
					pPoreDirection[nIDx] = PoreDirection(pMergeImage, &i, &j, VerticalInValidRow, nWidth, nHeight, nBlockType);
					sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] pPoreDirection[%d] = %d\r\n", __FUNCTION__, __LINE__, nIDx, pPoreDirection[nIDx]);
					LOGD(buffer);
					nIDx++;
				}
			}
		}
	}
}

int PoreDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType)
{
	if (pMergeImage == NULL || VerticalInValidRow == NULL)
		return -400;

	char buffer[1000] = { 0 };
	int i, j, ii, jj, nSx, nSy, nIDx, nCurrentIDx, nBlockType = -1, nNotFindCnt = 0, nLenCnt = 0, nDir = 0;
	unsigned char OneBlockSize = EMC_BLOCK_SIZE, nLenTh = VALLEY_LENGTH_TH;

	signed char SearchRange[SEARCH_SIZE] = { 0, 1, 2, -1, -2 };
	unsigned char R1 = 0, E0 = 170, V1 = 255, D1 = 85, V2 = 254, D2 = 84, R2 = 1, D5 = 86;
	unsigned char V3 = 253, D3 = 83;   //Valleys分岔標記號
	unsigned char V4 = 252, D4 = 82;   //Valleys第一點分岔標記號
	unsigned char nV1 = 245, nV2 = 244;//Bounding Valleys標記號
	unsigned char nValleysEnding = 101;//Valleys Ending標記號

	unsigned char R3 = 2, D6 = 87;	   //Ridges分岔標記號
	unsigned char R4 = 3, D7 = 88;	   //Ridges第一點分岔標記號
	unsigned char nR1 = 10, nR2 = 9;   //Bounding Ridges標記號
	unsigned char nRidgesEnding = 111; //Ridges Ending標記號

	unsigned char V5 = 250;				//處理過汗孔的標記號
	unsigned char V6 = 249;				//符合第一階段汗孔的標記號
	unsigned char nDisPore = 251;		//偽汗孔標記號
	unsigned char nPore = 121;			//汗孔標記號
	unsigned char nMergePore = 122;		//汗孔標記號合併
	unsigned char nProcessPore = 123;	//汗孔標記號處理過
	unsigned char nFinalPore = 124;		//汗孔標記號確認
	unsigned char nMultiPore = 125;		//汗孔標記號多個

	bool bIsEdge = false, bBlockLimit = false, bNotFind = false, bFind = false, bLength = false;

	i = *nStartX;
	j = *nStartY;
	nSx = *nStartX;
	nSy = *nStartY;
	nBlockType = nType;

	if (nBlockType == EMC_BLOCK_GOOD) {
		/* 向右搜尋 */
		for (ii = nSx; ii < nWidth; ) {
			ii = ii + 1; //將 X 座標右移
			if (ii == nWidth)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSy + SearchRange[nIDx] > nHeight - 1) {
					jj = nHeight - 1;
					bIsEdge = true;//超過 Y 方向的下邊界限制
				}
				else if (nSy + SearchRange[nIDx] < 0) {
					jj = 0;
					bIsEdge = true;//超過 Y 方向的上邊界限制
				}
				else
					jj = nSy + SearchRange[nIDx];

				if (nBlockType == EMC_BLOCK_GOOD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Good限制線就停止不做
						if (jj >= nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_GOOD)

				if (pMergeImage[jj][ii] == R1 ||
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == (V2 - 100) ||
						 pMergeImage[jj][ii] == (V3 - 100) ||
						 pMergeImage[jj][ii] == (V4 - 100) ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == nV2 ||
						 pMergeImage[jj][ii] == (nV2 - 100) ||
						 pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == R2 ||
						 pMergeImage[jj][ii] == R3 ||
						 pMergeImage[jj][ii] == R4 ||
						 pMergeImage[jj][ii] == nR1 ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == (D2 - 10) ||
						 pMergeImage[jj][ii] == (D3 - 10) ||
						 pMergeImage[jj][ii] == (D4 - 10) ||
						 pMergeImage[jj][ii] == D5 ||
						 pMergeImage[jj][ii] == D6 ||
						 pMergeImage[jj][ii] == D7 ||
						 pMergeImage[jj][ii] == E0 || 
						 pMergeImage[jj][ii] == V5 ||
						 pMergeImage[jj][ii] == V6 ||
						 pMergeImage[jj][ii] == nDisPore ||
						 pMergeImage[jj][ii] == nPore ||
						 pMergeImage[jj][ii] == nMergePore ||
						 pMergeImage[jj][ii] == nProcessPore ||
						 pMergeImage[jj][ii] == nMultiPore) 
				{
					nNotFindCnt++;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {

			if (nNotFindCnt == SEARCH_SIZE) {
				sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}
			
			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				bNotFind = true;
				break;
			}

			//超過 Y 的上下邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
			}

			if (bFind == true) {
				bFind = false;
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSy + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSy + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
				LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 Y 座標
				if (bLength == false)
					nSy = nSy + SearchRange[nCurrentIDx];
				sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSy = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSy, nCurrentIDx);
				LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
				LOGD(buffer);
				//nDir = ComputeDirection(i, j, ii, nSy + SearchRange[nCurrentIDx], nBlockType);
				nDir = xVFComputeLineDirection(i, j, ii, nSy + SearchRange[nCurrentIDx]);
				break;
			}
		}//for (ii = nSx; ii < nWidth; ) {

		if (bNotFind == true) {
			/* 向右搜尋沒有找到時，需要繼續向左搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向左尋找下一點 */
			for (ii = nSx; ii >= 0; ) {
				ii = ii - 1;	//將 X 座標左移
				if (ii < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSy + SearchRange[nIDx] > nHeight - 1) {
						jj = nHeight - 1;
						bIsEdge = true;//超過 Y 方向的下邊界限制
					}
					else if (nSy + SearchRange[nIDx] < 0) {
						jj = 0;
						bIsEdge = true;//超過 Y 方向的上邊界限制
					}
					else
						jj = nSy + SearchRange[nIDx];

					if (nBlockType == EMC_BLOCK_GOOD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Good限制線就停止不做
							if (jj >= nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_GOOD)

					if (pMergeImage[jj][ii] == R1 || 
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
							 pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
							 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == R1 ||
							 pMergeImage[jj][ii] == R2 ||
							 pMergeImage[jj][ii] == R3 ||
							 pMergeImage[jj][ii] == R4 ||
							 pMergeImage[jj][ii] == nR1 ||
							 pMergeImage[jj][ii] == D1 ||
							 pMergeImage[jj][ii] == D2 ||
						 	 pMergeImage[jj][ii] == D3 ||
							 pMergeImage[jj][ii] == D4 ||
							 pMergeImage[jj][ii] == (D2 - 10) ||
							 pMergeImage[jj][ii] == (D3 - 10) ||
							 pMergeImage[jj][ii] == (D4 - 10) ||
							 pMergeImage[jj][ii] == D5 ||
							 pMergeImage[jj][ii] == D6 ||
							 pMergeImage[jj][ii] == D7 ||
							 pMergeImage[jj][ii] == E0 ||
							 pMergeImage[jj][ii] == V5 ||
							 pMergeImage[jj][ii] == V6 ||
							 pMergeImage[jj][ii] == nDisPore ||
							 pMergeImage[jj][ii] == nPore ||
							 pMergeImage[jj][ii] == nMergePore ||
							 pMergeImage[jj][ii] == nProcessPore ||
							 pMergeImage[jj][ii] == nMultiPore) {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bNotFind = true;
					break;
				}

				//超過 Y 的上下邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSy + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSy + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSy + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx], pMergeImage[nSy + SearchRange[nCurrentIDx]][ii]);
					LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 Y 座標
					if (bLength == false)
						nSy = nSy + SearchRange[nCurrentIDx];
				}//if (bFind == true)

				if (bLength == true) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, ii, nSy + SearchRange[nCurrentIDx]);
					LOGD(buffer);
					//nDir = ComputeDirection(ii, nSy + SearchRange[nCurrentIDx], i, j, nBlockType);
					nDir = xVFComputeLineDirection(ii, nSy + SearchRange[nCurrentIDx], i, j);
					break;
				}
			}//for (ii = nSx; ii >= 0; )
		}//if (bNotFind == true)
	}
	else {
		/* 向下尋找下一點 */
		for (jj = nSy; jj < nHeight; ) {
			jj = jj + 1;	//將 Y 座標下移
			if (jj == nHeight)
				break;

			for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
				if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
					bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
					break;
				}
				else
					ii = nSx + SearchRange[nIDx];

				if (nBlockType == EMC_BLOCK_BAD) {
					if (VerticalInValidRow[ii / OneBlockSize] != -1) {
						int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
						//超過區域Bad限制線就停止不做
						if (jj < nLimitLine) {
							bBlockLimit = true;
							break;
						}
					}
				}//if (nBlockType == EMC_BLOCK_BAD)

				if (pMergeImage[jj][ii] == R1 || 
					pMergeImage[jj][ii] == (R2 + 50) ||
					pMergeImage[jj][ii] == (R3 + 50) ||
					pMergeImage[jj][ii] == (R4 + 50) ||
					pMergeImage[jj][ii] == nR2 ||
					pMergeImage[jj][ii] == (nR2 + 50) ||
					pMergeImage[jj][ii] == (D5 - 10) ||
					pMergeImage[jj][ii] == (D6 - 10) ||
					pMergeImage[jj][ii] == (D7 - 10))
				{
					if (bFind == false)
						bFind = true;
					nCurrentIDx = nIDx;//保存符合條件的Index
				}
				else if (pMergeImage[jj][ii] == V1 ||
						 pMergeImage[jj][ii] == V2 ||
						 pMergeImage[jj][ii] == V3 ||
						 pMergeImage[jj][ii] == V4 ||
						 pMergeImage[jj][ii] == (V2 - 100) ||
						 pMergeImage[jj][ii] == (V3 - 100) ||
						 pMergeImage[jj][ii] == (V4 - 100) ||
						 pMergeImage[jj][ii] == nV1 ||
						 pMergeImage[jj][ii] == nV2 ||
						 pMergeImage[jj][ii] == (nV2 - 100) ||
						 pMergeImage[jj][ii] == R1 ||
						 pMergeImage[jj][ii] == R2 ||
						 pMergeImage[jj][ii] == R3 ||
						 pMergeImage[jj][ii] == R4 ||
						 pMergeImage[jj][ii] == nR1 ||
						 pMergeImage[jj][ii] == D1 ||
						 pMergeImage[jj][ii] == D2 ||
						 pMergeImage[jj][ii] == D3 ||
						 pMergeImage[jj][ii] == D4 ||
						 pMergeImage[jj][ii] == (D2 - 10) ||
						 pMergeImage[jj][ii] == (D3 - 10) ||
						 pMergeImage[jj][ii] == (D4 - 10) ||
						 pMergeImage[jj][ii] == D5 ||
						 pMergeImage[jj][ii] == D6 ||
						 pMergeImage[jj][ii] == D7 ||
						 pMergeImage[jj][ii] == E0 ||
						 pMergeImage[jj][ii] == V5 ||
						 pMergeImage[jj][ii] == V6 ||
						 pMergeImage[jj][ii] == nDisPore ||
						 pMergeImage[jj][ii] == nPore ||
						 pMergeImage[jj][ii] == nMergePore ||
						 pMergeImage[jj][ii] == nProcessPore ||
						 pMergeImage[jj][ii] == nMultiPore) {
					nNotFindCnt++;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
					LOGD(buffer);
					continue;
				}
				else {
					nNotFindCnt++;
					sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
					LOGD(buffer);
				}
			}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

			if (nNotFindCnt == SEARCH_SIZE) {
				sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
				LOGD(buffer);
				bNotFind = true;
				nCurrentIDx = 0;
				nNotFindCnt = 0;
			}

			if (bNotFind == true) {
				//bNotFind = false;
				break;
			}

			if (bBlockLimit == true) {
				bBlockLimit = false;
				bNotFind = true;
				break;
			}

			//超過 X 的左右邊界限制
			if (bIsEdge == true) {
				bIsEdge = false;
				break;
			}

			if (bFind == true) {
				bFind = false;
				nNotFindCnt = 0;
				nLenCnt++;         //長度計數器累加
				sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
				LOGD(buffer);
				if (nLenCnt == nLenTh)
					bLength = true;

				int nUpdateIDx = 0;
				if (nSx + SearchRange[nCurrentIDx] < 0) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == 0) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
					for (int nn = 0; nn < SEARCH_SIZE; nn++) {
						if (nSx + SearchRange[nn] == nHeight - 1) {
							nUpdateIDx = nn;
							break;
						}
					}
					nCurrentIDx = nUpdateIDx;
				}
				sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
				LOGD(buffer);

				//長度條件為滿足，則繼續更新下次的 X 座標
				if (bLength == false)
					nSx = nSx + SearchRange[nCurrentIDx];
				//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
				//LOGD(buffer);
			}//if (bFind == true)

			if (bLength == true) {
				sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
				LOGD(buffer);
				//nDir = ComputeDirection(i, j, nSx + SearchRange[nCurrentIDx], jj, nBlockType);
				nDir = xVFComputeLineDirection(i, j, nSx + SearchRange[nCurrentIDx], jj);
				break;
			}
		}//for (jj = nSy; jj < nHeight; )

		if (bNotFind == true) {
			/* 向下搜沒有找到時，需要繼續向上搜尋 */
			bIsEdge = false;
			bBlockLimit = false;
			bNotFind = false;
			bFind = false;
			nCurrentIDx = 0;
			nNotFindCnt = 0;
			nLenCnt = 0;

			/* 載入未滿長度條件的座標 */
			nSx = *nStartX;
			nSy = *nStartY;

			/* 向上尋找下一點 */
			for (jj = nSy; jj >= 0; ) {
				jj = jj - 1;	//將 Y 座標上移
				if (jj < 0)
					break;

				for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++) {
					if (nSx + SearchRange[nIDx] > nWidth - 1 || nSx + SearchRange[nIDx] < 0) {
						bIsEdge = true;//超過 X 方向的左右邊界限制，可不繼續做下去
						break;
					}
					else
						ii = nSx + SearchRange[nIDx];

					if (nBlockType == EMC_BLOCK_BAD) {
						if (VerticalInValidRow[ii / OneBlockSize] != -1) {
							int nLimitLine = (VerticalInValidRow[ii / OneBlockSize] * OneBlockSize);
							//超過區域Bad限制線就停止不做
							if (jj < nLimitLine) {
								bBlockLimit = true;
								break;
							}
						}
					}//if (nBlockType == EMC_BLOCK_BAD)

					if (pMergeImage[jj][ii] == R1 || 
						pMergeImage[jj][ii] == (R2 + 50) ||
						pMergeImage[jj][ii] == (R3 + 50) ||
						pMergeImage[jj][ii] == (R4 + 50) ||
						pMergeImage[jj][ii] == nR2 ||
						pMergeImage[jj][ii] == (nR2 + 50) ||
						pMergeImage[jj][ii] == (D5 - 10) ||
						pMergeImage[jj][ii] == (D6 - 10) ||
						pMergeImage[jj][ii] == (D7 - 10))
					{
						if (bFind == false)
							bFind = true;
						nCurrentIDx = nIDx;//保存符合條件的Index
					}
					else if (pMergeImage[jj][ii] == V1 ||
							 pMergeImage[jj][ii] == V2 ||
							 pMergeImage[jj][ii] == V3 ||
							 pMergeImage[jj][ii] == V4 ||
							 pMergeImage[jj][ii] == (V2 - 100) ||
							 pMergeImage[jj][ii] == (V3 - 100) ||
							 pMergeImage[jj][ii] == (V4 - 100) ||
							 pMergeImage[jj][ii] == nV1 ||
							 pMergeImage[jj][ii] == nV2 ||
							 pMergeImage[jj][ii] == (nV2 - 100) ||
							 pMergeImage[jj][ii] == R1 ||
							 pMergeImage[jj][ii] == R2 ||
							 pMergeImage[jj][ii] == R3 ||
							 pMergeImage[jj][ii] == R4 ||
							 pMergeImage[jj][ii] == nR1 ||
							 pMergeImage[jj][ii] == D1 ||
							 pMergeImage[jj][ii] == D2 ||
							 pMergeImage[jj][ii] == D3 ||
							 pMergeImage[jj][ii] == D4 ||
							 pMergeImage[jj][ii] == (D2 - 10) ||
							 pMergeImage[jj][ii] == (D3 - 10) ||
							 pMergeImage[jj][ii] == (D4 - 10) ||
							 pMergeImage[jj][ii] == D5 ||
							 pMergeImage[jj][ii] == D6 ||
							 pMergeImage[jj][ii] == D7 ||
							 pMergeImage[jj][ii] == E0 ||
							 pMergeImage[jj][ii] == V5 ||
							 pMergeImage[jj][ii] == V6 ||
							 pMergeImage[jj][ii] == nDisPore ||
							 pMergeImage[jj][ii] == nPore ||
							 pMergeImage[jj][ii] == nMergePore ||
							 pMergeImage[jj][ii] == nProcessPore ||
							 pMergeImage[jj][ii] == nMultiPore) {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, nNotFindCnt, nIDx);
						LOGD(buffer);
						continue;
					}
					else {
						nNotFindCnt++;
						sprintf(buffer, "[FPTexture.c] %s() #Line %d Others ... pMergeImage[%d][%d] = %d / nNotFindCnt = %d / nIDx = %d\r\n", __FUNCTION__, __LINE__, jj, ii, pMergeImage[jj][ii], nNotFindCnt, nIDx);
						LOGD(buffer);
					}
				}//for (nIDx = 0; nIDx < SEARCH_SIZE; nIDx++)

				if (nNotFindCnt == SEARCH_SIZE) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d *** Not Find [nNotFindCnt = %d] ***\r\n", __FUNCTION__, __LINE__, nNotFindCnt);
					LOGD(buffer);
					bNotFind = true;
					nCurrentIDx = 0;
					nNotFindCnt = 0;
				}

				if (bNotFind == true) {
					bNotFind = false;
					break;
				}

				if (bBlockLimit == true) {
					bBlockLimit = false;
					bNotFind = true;
					break;
				}

				//超過 X 的左右邊界限制
				if (bIsEdge == true) {
					bIsEdge = false;
					break;
				}

				if (bFind == true) {
					bFind = false;
					nNotFindCnt = 0;
					nLenCnt++;         //長度計數器累加
					sprintf(buffer, "[FPTexture.c] %s() #Line %d nCurrentIDx = %d / nLenCnt = %d\r\n", __FUNCTION__, __LINE__, nCurrentIDx, nLenCnt);
					LOGD(buffer);
					if (nLenCnt == nLenTh)
						bLength = true;

					int nUpdateIDx = 0;
					if (nSx + SearchRange[nCurrentIDx] < 0) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == 0) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					else if (nSx + SearchRange[nCurrentIDx] > nHeight - 1) {
						for (int nn = 0; nn < SEARCH_SIZE; nn++) {
							if (nSx + SearchRange[nn] == nHeight - 1) {
								nUpdateIDx = nn;
								break;
							}
						}
						nCurrentIDx = nUpdateIDx;
					}
					sprintf(buffer, "[FPTexture.c] %s() #Line %d Current (%d, %d) / %d\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj, pMergeImage[jj][nSx + SearchRange[nCurrentIDx]]);
					LOGD(buffer);

					//長度條件為滿足，則繼續更新下次的 X 座標
					if (bLength == false)
						nSx = nSx + SearchRange[nCurrentIDx];
					//sprintf(buffer, "[FPTexture.c] %s() #Line %d Update Next [nSx = %d / nCurrentIDx = %d]\r\n", __FUNCTION__, __LINE__, nSx, nCurrentIDx);
					//LOGD(buffer);
				}//if (bFind == true)

				if (bLength == true) {
					sprintf(buffer, "[FPTexture.c] %s() #Line %d (%d, %d)\r\n", __FUNCTION__, __LINE__, nSx + SearchRange[nCurrentIDx], jj);
					LOGD(buffer);
					//nDir = ComputeDirection(nSx + SearchRange[nCurrentIDx], jj, i, j, nBlockType);
					nDir = xVFComputeLineDirection(nSx + SearchRange[nCurrentIDx], jj, i, j);
					break;
				}
			}//for (jj = nSy; jj >= 0; )
		}//if (bNotFind == true)
	}
	return nDir;
}

int GetPoreTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight)
{
	if (pMergeImage == NULL || pTextureTemplate == NULL)
		return -1;

	char buffer[1000] = { 0 };
	int i, j, nPoreCnt = 0, nPoreIDx = 0;
	unsigned char nFinalPore = 124;		//汗孔標記號確認

	nPoreCnt = nPoreDirectionCnt;
	if (pTextureTemplate->pPoresPoint == NULL && pTextureTemplate->nTotalPores == 0) {
		pTextureTemplate->nTotalPores = nPoreCnt;
		if (nPoreCnt > 0) {
			pTextureTemplate->pPoresPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nPoreCnt);
			memset(pTextureTemplate->pPoresPoint, 0, sizeof(struct TextureTemplatePOINT) * nPoreCnt);
		}
	}
	else {
		//free memory
		free(pTextureTemplate->pPoresPoint);
		pTextureTemplate->pPoresPoint = NULL;
		pTextureTemplate->nTotalPores = 0;

		pTextureTemplate->nTotalPores = nPoreCnt;
		if (nPoreCnt > 0) {
			pTextureTemplate->pPoresPoint = (struct TextureTemplatePOINT *) malloc(sizeof(struct TextureTemplatePOINT) * nPoreCnt);
			memset(pTextureTemplate->pPoresPoint, 0, sizeof(struct TextureTemplatePOINT) * nPoreCnt);
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (pMergeImage[j][i] == nFinalPore) {
				pTextureTemplate->pPoresPoint[nPoreIDx].x = i;
				pTextureTemplate->pPoresPoint[nPoreIDx].y = j;
				pTextureTemplate->pPoresPoint[nPoreIDx].theta = pPoreDirection[nPoreIDx];
				nPoreIDx++;
			}
		}
	}
	sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] nPoreIDx = %d\r\n", __FUNCTION__, __LINE__, nPoreIDx);
	LOGD(buffer);

	for (nPoreIDx = 0; nPoreIDx < pTextureTemplate->nTotalPores; nPoreIDx++) {
		sprintf(buffer, "[FPTexture.c] %s() #Line %d [KennyKang] Pore (%d, %d, %d)\r\n", __FUNCTION__, __LINE__, pTextureTemplate->pPoresPoint[nPoreIDx].x, pTextureTemplate->pPoresPoint[nPoreIDx].y, pTextureTemplate->pPoresPoint[nPoreIDx].theta);
		LOGD(buffer);
	}

	if (pPoreDirection != NULL) {
		free(pPoreDirection);
		pPoreDirection = NULL;
		nPoreDirectionCnt = -1;
	}
	return 0;
}

//**********************************************************************************************************
/*Texture HVPN-Smoothing*/ 
void Texture_HSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	/* Output Size (W * H) : 78 * 80 */
	for (j = 0; j < nHeight; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j][i - 1] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j][i + 1];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	//做左右邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補
		}
	}

	SaveTXT("Texture_HSmooth.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_VSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	/* Output Size (W * H) : 80 * 78 */
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 0; i < nWidth; i++) {
			buf = (short)pImage_In[j - 1][i] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j + 1][i];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	//做上下邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補
		}
	}

	SaveTXT("Texture_VSmooth.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_PSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	/* Output Size (W * H) : 78 * 78 */
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j - 1][i + 1] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j + 1][i - 1];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_PSmooth.csv", image_out, nWidth, nHeight);

	//做左右上下四方向邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補(左)
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補(右)
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補(上)
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補(下)

			if (i == 0 && j == 0)           //第1行 第1列
				image_out[j][i] = image_out[j + 1][i + 1];//例外處理1
			if (i == nWidth - 1 && j == 0) //第79行 第1列
				image_out[j][i] = image_out[j + 1][i - 1];//例外處理2
		}
	}

	SaveTXT("Texture_PSmooth.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_NSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	/* Output Size (W * H) : 78 * 78 */
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j - 1][i - 1] +
				(short)pImage_In[j][i] + (short)pImage_In[j][i] +
				(short)pImage_In[j + 1][i + 1];
			pImage_Out[j][i] = (buf + 2) / 4;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_NSmooth.csv", image_out, nWidth, nHeight);

	//做左右上下四方向邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補(左)
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補(右)
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補(上)
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補(下)

			if (i == 0 && j == 0)           //第1行 第1列
				image_out[j][i] = image_out[j + 1][i + 1];//例外處理1
			if (i == nWidth - 1 && j == 0) //第79行 第1列
				image_out[j][i] = image_out[j + 1][i - 1];//例外處理2
		}
	}

	SaveTXT("Texture_NSmooth.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

/*Texture HVPN-Differential*/
void Texture_HDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	//直接複製數值
	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	//進行H差分運算
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 0; i < nWidth; i++) {
			buf = (short)pImage_In[j - 1][i] * (-1) +
				  (short)pImage_In[j][i] * (0) +
				  (short)pImage_In[j + 1][i] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}
	
	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_HDifferential.csv", image_out, nWidth, nHeight);

	//做上下邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補
		}
	}

	SaveTXT("Texture_HDifferential.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_VDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	//直接複製數值
	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	//進行V差分運算
	for (j = 0; j < nHeight; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j][i - 1] * (-1) +
				  (short)pImage_In[j][i] * (0) +
				  (short)pImage_In[j][i + 1] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_VDifferential.csv", image_out, nWidth, nHeight);

	//做左右邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補
		}
	}

	SaveTXT("Texture_VDifferential.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_PDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	//直接複製數值
	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	//進行P差分運算
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j - 1][i - 1] * (-1) +
				(short)pImage_In[j][i] * (0) +
				(short)pImage_In[j + 1][i + 1] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_PDifferential.csv", image_out, nWidth, nHeight);

	//做左右上下四方向邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補(左)
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補(右)
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補(上)
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補(下)

			if (i == 0 && j == 0)           //第1行 第1列
				image_out[j][i] = image_out[j + 1][i + 1];//例外處理1
			if (i == nWidth - 1 && j == 0) //第79行 第1列
				image_out[j][i] = image_out[j + 1][i - 1];//例外處理2
		}
	}

	SaveTXT("Texture_PDifferential.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

void Texture_NDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight)
{
	if (image_in == NULL || image_out == NULL)
		return;

	short i, j, buf;
	unsigned char **pImage_In = NULL, **pImage_Out = NULL;
	pImage_In = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	pImage_Out = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pImage_In[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_In[j], 0, nWidth);
		pImage_Out[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pImage_Out[j], 0, nWidth);
	}

	//直接複製數值
	for (j = 0; j < nHeight; j++)
		memcpy(pImage_In[j], image_in[j], nWidth);

	//進行N差分運算
	for (j = 1; j < nHeight - 1; j++) {
		for (i = 1; i < nWidth - 1; i++) {
			buf = (short)pImage_In[j - 1][i + 1] * (-1) +
				(short)pImage_In[j][i] * (0) +
				(short)pImage_In[j + 1][i - 1] * (1);
			pImage_Out[j][i] = buf + 128;
		}
	}

	for (j = 0; j < nHeight; j++)
		memcpy(image_out[j], pImage_Out[j], nWidth);

	SaveTXT("Be_Texture_NDifferential.csv", image_out, nWidth, nHeight);

	//做左右上下四方向邊緣補值
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			if (i == 0)       //第1行
				image_out[j][i] = image_out[j][i + 1];//第1行拿第2行的數值去補(左)
			if (i == nWidth - 1)//第79行
				image_out[j][i] = image_out[j][i - 1];//第79行拿第78行的數值去補(右)
			if (j == 0)         //第1列
				image_out[j][i] = image_out[j + 1][i];//第1列拿第2列的數值去補(上)
			if (j == nHeight - 1)//第79列
				image_out[j][i] = image_out[j - 1][i];//第79列拿第78列的數值去補(下)

			if (i == 0 && j == 0)           //第1行 第1列
				image_out[j][i] = image_out[j + 1][i + 1];//例外處理1
			if (i == nWidth - 1 && j == 0) //第79行 第1列
				image_out[j][i] = image_out[j + 1][i - 1];//例外處理2
		}
	}

	SaveTXT("Texture_NDifferential.csv", image_out, nWidth, nHeight);

	if (pImage_Out != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_Out[j]);
		}
		free(pImage_Out);
		pImage_Out = NULL;

	}

	if (pImage_In != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pImage_In[j]);
		}
		free(pImage_In);
		pImage_In = NULL;
	}
}

/*Texture HVPN-Labeling*/
void Texture_HLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j, nExpandTopLine = 2;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	short **pIn_Image = NULL;
	unsigned char **pOut_Image = NULL;

	pIn_Image = (short **)malloc(sizeof(short *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pIn_Image[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			pIn_Image[j][i] = 0;
		}
	}

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {	//0 ~ 81
		for (i = 0; i < nWidth; i++) {
			if (j == 0)						//第0條
				pIn_Image[j][i] = pImage_in[j][i];
			else if (j == 1)				//第1條
				pIn_Image[j][i] = pImage_in[j - 1][i];
			else
				pIn_Image[j][i] = pImage_in[j - 2][i];
		}
	}

	pOut_Image = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pOut_Image[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(pOut_Image[j], BG, nWidth);
	}

	for (i = 0; i < nWidth; i++) {
		for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++) {
			sA = pIn_Image[j - 2][i];	/*A*/
			sB = pIn_Image[j - 1][i];	/*B*/
			sC = pIn_Image[j][i];		/*C*/
			sD = pIn_Image[j + 1][i];	/*D*/
			sE = pIn_Image[j + 2][i];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j + 1][i] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = R0;
				}
				else {
					pOut_Image[j + 1][i] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = R0;
						}
						else {
							pOut_Image[j + 1][i] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = V0;
						}
						else {
							pOut_Image[j + 1][i] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j + 1][i] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = V0;
				}
				else {
					pOut_Image[j + 1][i] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++)
	} //for (i = 0; i < nWidth; i++)

	for (i = 0; i < nWidth; i++) {
		for (j = 0; j < nHeight; j++) {
			//抓取 (nWidth * nHeight) 的數據 (80 * 80)
			pImage_out[j][i] = pOut_Image[j + nExpandTopLine][i];

			//將X軸的第1條與最後1條補成BG定義的數值
			if (i == 0)
				pImage_out[j][i] = BG;
			if (i == nWidth - 1)
				pImage_out[j][i] = BG;
		}
	}

	SaveTXT("Texture_HLabeling.csv", pImage_out, nWidth, nHeight);

	if (pOut_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pOut_Image[j]);
		}
		free(pOut_Image);
		pOut_Image = NULL;
	}

	if (pIn_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pIn_Image[j]);
		}
		free(pIn_Image);
		pIn_Image = NULL;
	}
}

void Texture_VLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j, nExpandLeftLine = 2;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	short **pIn_Image = NULL;
	unsigned char **pOut_Image = NULL;

	pIn_Image = (short **)malloc(sizeof(short *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pIn_Image[j] = (short *)malloc(sizeof(short) * (nWidth + nExpandLeftLine));
		for (i = 0; i < (nWidth + nExpandLeftLine); i++) {
			pIn_Image[j][i] = 0;
		}
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < (nWidth + nExpandLeftLine); i++) { //0 ~ 81
			if (i == 0)						//第0列
				pIn_Image[j][i] = pImage_in[j][i];
			else if (i == 1)				//第1列
				pIn_Image[j][i] = pImage_in[j][i - 1];
			else
				pIn_Image[j][i] = pImage_in[j][i - 2];
		}
	}

	pOut_Image = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		pOut_Image[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + nExpandLeftLine));
		memset(pOut_Image[j], BG, (nWidth + nExpandLeftLine));
	}

	for (j = 0; j < nHeight; j++) {
		for (i = 2; i < (nWidth + nExpandLeftLine) - 2; i++) {
			sA = pIn_Image[j][i - 2];	/*A*/
			sB = pIn_Image[j][i - 1];	/*B*/
			sC = pIn_Image[j][i];		/*C*/
			sD = pIn_Image[j][i + 1];	/*D*/
			sE = pIn_Image[j][i + 2];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j][i + 1] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = R0;
				}
				else {
					pOut_Image[j][i + 1] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = R0;
						}
						else {
							pOut_Image[j][i + 1] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = V0;
						}
						else {
							pOut_Image[j][i + 1] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pOut_Image[j][i + 1] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = V0;
				}
				else {
					pOut_Image[j][i + 1] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < (nWidth + nExpandLeftLine) - 2; i++)
	} //for (j = 0; j < nHeight; j++)

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			//抓取 (nWidth * nHeight) 的數據 (80 * 80)
			pImage_out[j][i] = pOut_Image[j][i + nExpandLeftLine];

			//將Y軸的第1列與最後1列補成BG定義的數值
			if (j == 0)
				pImage_out[j][i] = BG;
			if (j == nHeight - 1)
				pImage_out[j][i] = BG;
		}
	}

	SaveTXT("Texture_VLabeling.csv", pImage_out, nWidth, nHeight);

	if (pOut_Image != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pOut_Image[j]);
		}
		free(pOut_Image);
		pOut_Image = NULL;
	}

	if (pIn_Image != 0) {
		for (j = 0; j < nHeight; j++) {
			free(pIn_Image[j]);
		}
		free(pIn_Image);
		pIn_Image = NULL;
	}
}

void Texture_PLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j, nExpandTopLine = 2, nExpandLeftLine = 2;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	short **pIn_Image = NULL;
	unsigned char **pOut_Image = NULL;

	pIn_Image = (short **)malloc(sizeof(short *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pIn_Image[j] = (short *)malloc(sizeof(short) * (nWidth + nExpandLeftLine));
		for (i = 0; i < (nWidth + nExpandLeftLine); i++) {
			pIn_Image[j][i] = 0;
		}
	}

	for (j = 0; j < nHeight; j++) {    //0 ~ 79
		for (i = 0; i < nWidth; i++) { //0 ~ 79
			pIn_Image[(j + nExpandTopLine)][(i + nExpandLeftLine)] = pImage_in[j][i];
		}
	}

	//Save2ByteTXT("Expand_ShortType_PDifferential_1.csv", pIn_Image, (nHeight + nExpandTopLine), (nHeight + nExpandTopLine));

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {	   //0 ~ 81
		for (i = 0; i < (nWidth + nExpandLeftLine); i++) { //0 ~ 81
			if (i < nExpandLeftLine && j < nExpandTopLine)
				pIn_Image[j][i] = pIn_Image[nExpandTopLine][nExpandLeftLine];
			else if (i < nExpandLeftLine)
				pIn_Image[j][i] = pIn_Image[j][nExpandLeftLine];
			else if (j < nExpandTopLine)
				pIn_Image[j][i] = pIn_Image[nExpandTopLine][i];
		}
	}

	Save2ByteTXT("Expand_ShortType_PDifferential.csv", pIn_Image, (nHeight + nExpandTopLine), (nHeight + nExpandTopLine));

	pOut_Image = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pOut_Image[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + nExpandLeftLine));
		memset(pOut_Image[j], BG, (nWidth + nExpandLeftLine));
	}

	for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++) {
		for (i = 2; i < (nWidth + nExpandLeftLine) - 2; i++) {
			sA = pIn_Image[j - 2][i - 2];	/*A*/
			sB = pIn_Image[j - 1][i - 1];	/*B*/
			sC = pIn_Image[j][i];			/*C*/
			sD = pIn_Image[j + 1][i + 1];	/*D*/
			sE = pIn_Image[j + 2][i + 2];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j + 1][i + 1] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = R0;
				}
				else {
					pOut_Image[j + 1][i + 1] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = R0;
						}
						else {
							pOut_Image[j + 1][i + 1] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = V0;
						}
						else {
							pOut_Image[j + 1][i + 1] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pOut_Image[j + 1][i + 1] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = V0;
				}
				else {
					pOut_Image[j + 1][i + 1] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < (nWidth + nExpandLeftLine) - 2; i++)
	} //for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++)

	
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			//抓取 (nWidth * nHeight) 的數據 (80 * 80)
			pImage_out[j][i] = pOut_Image[j + nExpandTopLine][i + nExpandLeftLine];

			//將X軸的第1條與最後1條補成BG定義的數值
			if (i == 0)
				pImage_out[j][i] = BG;
			if (i == nWidth - 1)
				pImage_out[j][i] = BG;

			//將Y軸的第1列與最後1列補成BG定義的數值
			if (j == 0)
				pImage_out[j][i] = BG;
			if (j == nHeight - 1)
				pImage_out[j][i] = BG;
		}
	}

	SaveTXT("Texture_PLabeling.csv", pImage_out, nWidth, nHeight);

	if (pOut_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pOut_Image[j]);
		}
		free(pOut_Image);
		pOut_Image = NULL;
	}

	if (pIn_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pIn_Image[j]);
		}
		free(pIn_Image);
		pIn_Image = NULL;
	}
}

void Texture_NLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
	if (pImage_in == NULL || pImage_out == NULL)
		return;

	int i, j, nExpandTopLine = 2, nExpandRightLine = 2;
	unsigned char R0 = 85, DL = 0, V0 = 255, BG = 170;
	short sA = 255, sB = 255, sC = 255, sD = 255, sE = 255;
	short **pIn_Image = NULL;
	unsigned char **pOut_Image = NULL;

	pIn_Image = (short **)malloc(sizeof(short *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pIn_Image[j] = (short *)malloc(sizeof(short) * (nWidth + nExpandRightLine));
		for (i = 0; i < (nWidth + nExpandRightLine); i++) {
			pIn_Image[j][i] = 0;
		}
	}

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {	//0 ~ 81
		for (i = 0; i < nWidth; i++) {
			if (j == 0)						//第0條
				pIn_Image[j][i] = pImage_in[j][i];
			else if (j == 1)				//第1條
				pIn_Image[j][i] = pImage_in[j - 1][i];
			else
				pIn_Image[j][i] = pImage_in[j - 2][i];
		}
	}

	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		for (i = 0; i < (nWidth + nExpandRightLine); i++) { //0 ~ 81
			if (i == (nWidth + nExpandRightLine) - 2)		//第80列
				pIn_Image[j][i] = pIn_Image[j][nWidth + nExpandRightLine - 3];
			else if (i == (nWidth + nExpandRightLine) - 1)	//第81列
				pIn_Image[j][i] = pIn_Image[j][nWidth + nExpandRightLine - 3];
		}
	}

	Save2ByteTXT("Expand_ShortType_NDifferential.csv", pIn_Image, (nHeight + nExpandTopLine), (nHeight + nExpandTopLine));
	
	pOut_Image = (unsigned char **)malloc(sizeof(unsigned char *) * (nHeight + nExpandTopLine));
	for (j = 0; j < (nHeight + nExpandTopLine); j++) {
		pOut_Image[j] = (unsigned char *)malloc(sizeof(unsigned char) * (nWidth + nExpandRightLine));
		memset(pOut_Image[j], BG, (nWidth + nExpandRightLine));
	}

	for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++) {
		for (i = 2; i < (nWidth + nExpandRightLine) - 2; i++) {
			sA = pIn_Image[j - 2][i + 2];	/*A*/
			sB = pIn_Image[j - 1][i + 1];	/*B*/
			sC = pIn_Image[j][i];			/*C*/
			sD = pIn_Image[j + 1][i - 1];	/*D*/
			sE = pIn_Image[j + 2][i - 2];	/*E*/

			if (sC < 0 && sD > 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (abs(sC) > abs(sD)) {
					pOut_Image[j + 1][i - 1] = R0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = R0;
				}
				else {
					pOut_Image[j + 1][i - 1] = R0;
				}
			}
			else if (sC == 0) {
				if (sB < 0 && sD > 0) {
					if (pOut_Image[j][i] == V0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = R0;
				}
				else if (sB > 0 && sD < 0) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (sD == 0) {
					if (sB < 0 && sE > 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = R0;
						}
						else {
							pOut_Image[j + 1][i - 1] = R0;
						}
					}
					else if (sB > 0 && sE < 0) {
						if (abs(sB) < abs(sE)) {
							pOut_Image[j][i] = V0;
						}
						else {
							pOut_Image[j + 1][i - 1] = V0;
						}
					}
				}
			}
			else if (sC > 0 && sD < 0) {
				if (abs(sC) < abs(sD)) {
					if (pOut_Image[j][i] == R0)
						pOut_Image[j][i] = DL;
					else
						pOut_Image[j][i] = V0;
				}
				else if (abs(sC) > abs(sD)) {

					pOut_Image[j + 1][i - 1] = V0;
				}
				else if (abs(sB) < abs(sE)) {
					pOut_Image[j][i] = V0;
				}
				else {
					pOut_Image[j + 1][i - 1] = V0;
				}
			}
			sA = 255;
			sB = 255;
			sC = 255;
			sD = 255;
			sE = 255;
		} //for (i = 2; i < (nWidth + nExpandRightLine) - 2; i++)
	} //for (j = 2; j < (nHeight + nExpandTopLine) - 2; j++)

	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			//抓取 (nWidth * nHeight) 的數據 (80 * 80)
			pImage_out[j][i] = pOut_Image[j + nExpandTopLine][i];

			//將X軸的第1條與最後1條補成BG定義的數值
			if (i == 0)
				pImage_out[j][i] = BG;
			if (i == nWidth - 1)
				pImage_out[j][i] = BG;

			//將Y軸的第1列與最後1列補成BG定義的數值
			if (j == 0)
				pImage_out[j][i] = BG;
			if (j == nHeight - 1)
				pImage_out[j][i] = BG;
		}
	}

	SaveTXT("Texture_NLabeling.csv", pImage_out, nWidth, nHeight);

	if (pOut_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pOut_Image[j]);
		}
		free(pOut_Image);
		pOut_Image = NULL;
	}

	if (pIn_Image != 0) {
		for (j = 0; j < (nHeight + nExpandTopLine); j++) {
			free(pIn_Image[j]);
		}
		free(pIn_Image);
		pIn_Image = NULL;
	}
}

/*Texture HVPN-Smoothing + HVPN-Differential + HVPN-Labeling*/
int Texture_HImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int i, j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;
	////
	short **p2dDibArray = NULL;
	short sbuf;
	unsigned char BG = 170;
	////

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//HSmooth
	Texture_HSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);

	//copy output smooth image data
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//HDifferential
	Texture_HDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//copy output differential image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	////
	//Get Short Type Differential Data
	p2dDibArray = (short **)malloc(sizeof(short *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2dDibArray[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			p2dDibArray[j][i] = 255;
		}
	}
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2DImageTemp[j][i] - 128;
			p2dDibArray[j][i] = sbuf;
		}
	}
	Save2ByteTXT("ShortType_HDifferential.csv", p2dDibArray, nWidth, nHeight);
	
	for (j = 0; j < nHeight; j++) 
		memset(p2DImageTemp[j], BG, nWidth);

	//HLabeling
	Texture_HLabeling(p2dDibArray, p2DImageTemp, nWidth, nHeight);

	//Record HLabeling
	Texture_Record_HLabeling(p2DImageTemp, nWidth, nHeight);

	//copy output labeling image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	if (p2dDibArray != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(p2dDibArray[j]);
		}
		free(p2dDibArray);
		p2dDibArray = NULL;
	}
	////

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	p2DImageTemp = NULL;
	return 0;
}

int Texture_VImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int i, j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;
	////
	short **p2dDibArray = NULL;
	short sbuf;
	unsigned char BG = 170;
	////

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//VSmooth
	Texture_VSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);
	
	//copy output smooth image data
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//VDifferential
	Texture_VDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//copy output differential image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	////
	//Get Short Type Differential Data
	p2dDibArray = (short **)malloc(sizeof(short *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2dDibArray[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			p2dDibArray[j][i] = 255;
		}
	}
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2DImageTemp[j][i] - 128;
			p2dDibArray[j][i] = sbuf;
		}
	}
	Save2ByteTXT("ShortType_VDifferential.csv", p2dDibArray, nWidth, nHeight);

	for (j = 0; j < nHeight; j++)
		memset(p2DImageTemp[j], BG, nWidth);

	//VLabeling
	Texture_VLabeling(p2dDibArray, p2DImageTemp, nWidth, nHeight);

	//Record VLabeling
	//Texture_Record_VLabeling(p2DImageTemp, nWidth, nHeight);

	//copy output labeling image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	if (p2dDibArray != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(p2dDibArray[j]);
		}
		free(p2dDibArray);
		p2dDibArray = NULL;
	}
	////

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

int Texture_PImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int i, j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;
	////
	short **p2dDibArray = NULL;
	short sbuf;
	unsigned char BG = 170;
	////

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//PSmooth
	Texture_PSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);

	//copy output smooth image data
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//PDifferential
	Texture_PDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//copy output differential image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	////
	//Get Short Type Differential Data
	p2dDibArray = (short **)malloc(sizeof(short *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2dDibArray[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			p2dDibArray[j][i] = 255;
		}
	}
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2DImageTemp[j][i] - 128;
			p2dDibArray[j][i] = sbuf;
		}
	}
	Save2ByteTXT("ShortType_PDifferential.csv", p2dDibArray, nWidth, nHeight);

	for (j = 0; j < nHeight; j++)
		memset(p2DImageTemp[j], BG, nWidth);

	//PLabeling
	Texture_PLabeling(p2dDibArray, p2DImageTemp, nWidth, nHeight);

	//copy output labeling image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	if (p2dDibArray != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(p2dDibArray[j]);
		}
		free(p2dDibArray);
		p2dDibArray = NULL;
	}
	////

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

int Texture_NImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight)
{
	if (pInData == NULL || pOutData == NULL)
		return -1;

	int i, j;
	unsigned char **p2DImageIn = NULL, **p2DImageOut = NULL, **p2DImageTemp = NULL;
	////
	short **p2dDibArray = NULL;
	short sbuf;
	unsigned char BG = 170;
	////

	p2DImageIn = pInData;
	p2DImageOut = pOutData;
	p2DImageTemp = (unsigned char **)malloc(sizeof(unsigned char *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2DImageTemp[j] = (unsigned char *)malloc(sizeof(unsigned char) * nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//NSmooth
	Texture_NSmooth(p2DImageIn, p2DImageTemp, nWidth, nHeight);

	//copy output smooth image data
	for (j = 0; j < nHeight; j++) {
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);
		memset(p2DImageTemp[j], 0, nWidth);
	}

	//NDifferential
	Texture_NDifferential(p2DImageOut, p2DImageTemp, nWidth, nHeight);

	//copy output differential image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	////
	//Get Short Type Differential Data
	p2dDibArray = (short **)malloc(sizeof(short *) * nHeight);
	for (j = 0; j < nHeight; j++) {
		p2dDibArray[j] = (short *)malloc(sizeof(short) * nWidth);
		for (i = 0; i < nWidth; i++) {
			p2dDibArray[j][i] = 255;
		}
	}
	for (j = 0; j < nHeight; j++) {
		for (i = 0; i < nWidth; i++) {
			sbuf = p2DImageTemp[j][i] - 128;
			p2dDibArray[j][i] = sbuf;
		}
	}
	Save2ByteTXT("ShortType_NDifferential.csv", p2dDibArray, nWidth, nHeight);

	for (j = 0; j < nHeight; j++)
		memset(p2DImageTemp[j], BG, nWidth);

	//NLabeling
	Texture_NLabeling(p2dDibArray, p2DImageTemp, nWidth, nHeight);

	//copy output labeling image data
	for (j = 0; j < nHeight; j++)
		memcpy(p2DImageOut[j], p2DImageTemp[j], nWidth);

	if (p2dDibArray != NULL) {
		for (j = 0; j < nHeight; j++) {
			free(p2dDibArray[j]);
		}
		free(p2dDibArray);
		p2dDibArray = NULL;
	}
	////

	//free memory
	for (j = 0; j < nHeight; j++) {
		free(p2DImageTemp[j]);
	}
	free(p2DImageTemp);
	return 0;
}

/*Texture Record HVPN-Labeling*/
void Texture_Record_HLabeling(unsigned char **pImage_in, int nWidth, int nHeight)
{
	if (pImage_in == NULL)
		return;

	char buffer[1000] = { 0 };
	signed char HE[80][30] = { 0 };
	char ctr, ctrRV, ctrD, last;
	unsigned char R = 85, V = 255, D = 0, E = 170;
	int x, y, i, sum;

	ctr = ctrRV = ctrD = last = 0;
	for (x = 0; x < nWidth; x++)  {
		ctr = ctrRV = ctrD = 0;
		for (y = 0; y < nHeight; y++) {
			if (pImage_in[y][x] == E)     //170
			{
				ctr++;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else if (pImage_in[y][x] == V)//255
			{
				HE[x][ctrRV++] = ctr;
				last = 1;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else if (pImage_in[y][x] == R)//85
			{
				HE[x][ctrRV++] = 0 - ctr;
				last = -1;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else                         //0
			{
				HE[x][ctrRV++] = 0;
				HE[x][ctrRV++] = 0;
				ctrD++;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
		}
		if (last == -1)
			HE[x][ctrRV] = ctr;
		else if (last == 1)
			HE[x][ctrRV] = 0 - ctr;
		else
			HE[x][ctrRV] = ctr;

		sprintf(buffer, "[FPTexture.c] %s() #Line %d ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, ctr, ctrRV, ctrD, last);
		LOGD(buffer);

		sum = ctrRV - ctrD;
		for (i = 0; i <= ctrRV; i++) {
			sum += abs(HE[x][i]);
			sprintf(buffer, "[FPTexture.c] %s() #Line %d sum = %d / HE[%2d][%2d] = %2d\r\n", __FUNCTION__, __LINE__, sum, x, i, HE[x][i]);
			LOGD(buffer);
		}
		if (sum == nHeight) {
			sprintf(buffer, "[FPTexture.c] %s() #Line %d RIGHT!!\n", __FUNCTION__, __LINE__);
			LOGD(buffer);
		}
		else {
			sprintf(buffer, "[FPTexture.c] %s() #Line %d ERROR!!\n", __FUNCTION__, __LINE__);
			LOGD(buffer);
		}
	}
}

void Texture_Record_VLabeling(unsigned char **pImage_in, int nWidth, int nHeight)
{
	if (pImage_in == NULL)
		return;

	char buffer[1000] = { 0 };
	signed char VE[80][30] = { 0 };
	char ctr, ctrRV, ctrD, last;
	unsigned char R = 85, V = 255, D = 0, E = 170;
	int x, y, i, sum;

	ctr = ctrRV = ctrD = last = 0;
	for (y = 0; y < nHeight; y++) {
		ctr = ctrRV = ctrD = 0;
		for (x = 0; x < nWidth; x++) {
			if (pImage_in[y][x] == E)     //170 / E
			{
				ctr++;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else if (pImage_in[y][x] == V)//255 / V
			{
				VE[y][ctrRV++] = ctr;
				last = 1;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else if (pImage_in[y][x] == R)//85 / R
			{
				VE[y][ctrRV++] = 0 - ctr;
				last = -1;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
			else                         //0 / D
			{
				VE[y][ctrRV++] = 0;
				VE[y][ctrRV++] = 0;
				ctrD++;
				ctr = 0;
				sprintf(buffer, "[FPTexture.c] %s() #Line %d pImage_in[%3d][%3d] = %3d / ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, y, x, pImage_in[y][x], ctr, ctrRV, ctrD, last);
				LOGD(buffer);
			}
		}
		if (last == -1)
			VE[y][ctrRV] = ctr;
		else if (last == 1)
			VE[y][ctrRV] = 0 - ctr;
		else
			VE[y][ctrRV] = ctr;

		sprintf(buffer, "[FPTexture.c] %s() #Line %d ctr = %3d / ctrRV = %3d / ctrD = %3d / last = %3d\r\n", __FUNCTION__, __LINE__, ctr, ctrRV, ctrD, last);
		LOGD(buffer);

		sum = ctrRV - ctrD;
		for (i = 0; i <= ctrRV; i++) {
			sum += abs(VE[y][i]);
			sprintf(buffer, "[FPTexture.c] %s() #Line %d sum = %d / VE[%2d][%2d] = %2d\r\n", __FUNCTION__, __LINE__, sum, y, i, VE[y][i]);
			LOGD(buffer);
		}
		if (sum == nWidth) {
			sprintf(buffer, "[FPTexture.c] %s() #Line %d RIGHT!!\n", __FUNCTION__, __LINE__);
			LOGD(buffer);
		}
		else {
			sprintf(buffer, "[FPTexture.c] %s() #Line %d ERROR!!\n", __FUNCTION__, __LINE__);
			LOGD(buffer);
		}
	}
}
//**********************************************************************************************************
