#pragma once
#ifndef __FPIMAGEUTIL_INCLUDED__
#define __FPIMAGEUTIL_INCLUDED__
//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include <stdbool.h >
/*
Fingerprint Sensor Size Type
Square_L    --> 160 * 160
Square_M    --> 120 * 120
Square_XXS  --> 96 * 96
Square_XXXS --> 80 * 80
*/
typedef enum {
	elSquare_L = 0,
	elSquare_M = 1,
	elSquare_XXS = 2,
	elSquare_XXXS = 3,
	elSquare_TOTAL
} SensorType;

struct SensorTypeSize {
	unsigned char width;
	unsigned char height;
};

#define EMC_SQUARE_L_SIZE		160
#define EMC_SQUARE_M_SIZE		120
#define EMC_SQUARE_XXS_SIZE		96
#define EMC_SQUARE_XXSS_SIZE	80

#define EMC_SensorTYPE(sensor_type)				((sensor_type == elSquare_L) ? EMC_SQUARE_L_SIZE : (sensor_type == elSquare_M) ? EMC_SQUARE_M_SIZE : (sensor_type == elSquare_XXS) ? EMC_SQUARE_XXS_SIZE : (sensor_type == elSquare_XXXS) ? EMC_SQUARE_XXSS_SIZE : EMC_SQUARE_L_SIZE)
#define EMC_IMAGE_WIDTH(sensor_type)			EMC_SensorTYPE(sensor_type)
#define EMC_IMAGE_HEIGHT(sensor_type)			EMC_SensorTYPE(sensor_type)
#define EMC_MaxRecordItemLength(sensor_type)	(EMC_IMAGE_WIDTH(sensor_type) * 2)

#define EMC_Embedded
#ifdef EMC_Embedded
#define IMAGE_WIDTH				120
#define IMAGE_HEIGHT			120
#define MaxRecordItemLength		(IMAGE_WIDTH - (IMAGE_WIDTH / 6))
#else
#define IMAGE_WIDTH				160
#define IMAGE_HEIGHT			160
#define MaxRecordItemLength		(IMAGE_WIDTH * 2)
#endif

#if 0
#define Square_L	1
#define Square_M	0
#define Square_XXS	0

#if Square_L
#define IMAGE_WIDTH			160
#define IMAGE_HEIGHT		160
#define MaxRecordItemLength	(IMAGE_WIDTH * 2)
#elif  Square_M
#define IMAGE_WIDTH			120
#define IMAGE_HEIGHT		120
#define MaxRecordItemLength	(IMAGE_WIDTH * 2)
#else 
#define IMAGE_WIDTH			96
#define IMAGE_HEIGHT		96
#define MaxRecordItemLength	(IMAGE_WIDTH * 2)
#endif
#endif

#define MaxC1Len			11
#define QualityTh			0
#define JumpingTh			4

extern int nConnectingTh;
extern bool bLengthAndQuality;
extern bool bZeroDifferential;
extern int nSensorTypeL, nSensorTypeM, nSensorTypeXXS;
extern int nSensorWidth, nSensorHeight;

extern struct VerticalTextureInfo *pVerticalTextureInfo;
extern struct HorizontalTextureInfo *pHorizontalTextureInfo;

typedef enum {
	emhvNone = 0,
	emhvHorizontal = 1,
	emhvVertical = 2
} HorizontalVerticalType;

typedef enum  {
	emEnding = 0,
	emBifurcation = 1,
	emCrossing = 2,
	emJumping = 3,
	emUnknown = 4
} MinutiaeType;

struct BifurcationInfo {
	unsigned char xX;	 //紀錄分岔點 X 的座標
	unsigned char xY;	 //紀錄分岔點 Y 的座標
	unsigned char xL;	 //紀錄分岔點的線段
	unsigned char BxL;	 //紀錄被分岔點岔到的線段
};

struct MinutiaePOINT {
	MinutiaeType T;							//Minutiae 的分類
	unsigned char X;						//Minutiae X 的座標
	unsigned char Y;						//Minutiae Y 的座標
	int Sita1;								//Minutiae (X,Y) 的角度θ1
	int Sita2;								//Minutiae (X,Y) 的角度θ2
};

struct txPOINT {
	unsigned char x;						//X 的座標
	unsigned char y;						//Y 的座標
	unsigned char Uw;						//X 的上寬
	unsigned char Dw;						//X 的下寬
	unsigned char Lw;						//Y 的左寬
	unsigned char Rw;						//Y 的右寬
	char Weight;							//X or Y 的權重
};

struct HorizontalData {
	struct txPOINT *pHorizontal;			//水平的 X/Y 座標
	unsigned char Head;						//線段頭 Y 的座標
	unsigned char Tail;						//線段尾 Y 的座標
	unsigned short Length;					//線段 Y 的總長度
	short WeightSum;						//該線段上所有點集合的 Weight 加總
	bool bHeadQuality;						//線段頭 Y 的品質狀況
	bool bTailQuality;						//線段尾 Y 的品質狀況

	struct MinutiaePOINT *pEndingMinutiae;	   //紀錄頭、尾終點資料
	struct MinutiaePOINT *pBifurcationMinutiae;//紀錄分岔點資料
	struct MinutiaePOINT *pCrossingMinutiae;   //紀錄相交點資料
	struct MinutiaePOINT *pJumpingMinutiae;	   //記錄跳躍點資料
	short nEndingLength;		//紀錄頭、尾終點資料數量
	short nBifurcationLength;	//紀錄分岔點資料數量
	short nCrossingLength;		//紀錄相交點資料數量
	short nJumpingLength;		//紀錄跳躍點點資料數量

	struct BifurcationInfo *pBifurcationInfo;	//紀錄分岔點資訊
};

struct HorizontalTextureInfo {
	struct HorizontalData *HorizontalItem;	//水平線段資料
	int nLength;							//紀錄水平的線段數
};

struct VerticalData {
	struct txPOINT *pVertical;				//垂直的 X/Y 座標
	unsigned char Head;						//線段頭 X 的座標
	unsigned char Tail;						//線段尾 X 的座標
	unsigned short Length;					//線段 X 的總長度
	short WeightSum;						//該線段上所有點集合的 Weight 加總
	bool bHeadQuality;						//線段頭 X 的品質狀況
	bool bTailQuality;						//線段尾 X 的品質狀況

	struct MinutiaePOINT *pEndingMinutiae;	   //紀錄頭、尾終點資料
	struct MinutiaePOINT *pBifurcationMinutiae;//紀錄分岔點資料
	struct MinutiaePOINT *pCrossingMinutiae;   //紀錄相交點資料
	struct MinutiaePOINT *pJumpingMinutiae;	   //記錄跳躍點資料
	short nEndingLength;		//紀錄頭、尾終點資料數量
	short nBifurcationLength;	//紀錄分岔點資料數量
	short nCrossingLength;		//紀錄相交點資料數量
	short nJumpingLength;		//紀錄跳躍點點資料數量

	struct BifurcationInfo *pBifurcationInfo;	//紀錄分岔點資訊
};

struct VerticalTextureInfo {
	struct VerticalData *VerticalItem;		//垂直線段資料
	int nLength;							//紀錄垂直的線段數
};

void LOGD(char *pStr);
void SetSensorType(int nSensorType);
int GetSensorType(void);
void SensorSizeSetup(int nSensorType);

//void Smooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
/*Smooth and Differential*/
void HorizontalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void VerticalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void HorizontalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void VerticalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void Cover1DFormat(unsigned char **pImage_in, unsigned char *pImage_out, int nWidth, int nHeight, int bExchangeBits);
/*Labeling*/
void HorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void VerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
#if 0
void xxHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void xxVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void xHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void xVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
#endif
/*Connecting*/
void HorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void VerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void xHorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void xVerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void CheckHorizontalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void CheckVerticalConnecting(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void CheckHorizontalConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void CheckVerticalConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
#ifdef EMC_Embedded
void emCheckHorizontalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void emCheckVerticalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
#else
void CheckHorizontalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
void CheckVerticalZeroConnectingWithLengthAndQuality(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight);
#endif

int CheckConnectLength(unsigned char *pArray, int nWidth, int nHeight, int nLengthTh, int *pMaximum, int *pMinimum, HorizontalVerticalType nHorizontalVerticalType);
int CheckConnectLengthAndQuality(unsigned char *pArray, int nWidth, int nHeight, int nLengthTh, int nWeightSum, int *pMaximum, int *pMinimum, HorizontalVerticalType nHorizontalVerticalType);

void GetHorizontalAndVerticalConnectingData(short **pDifferentialData_in, unsigned char **pLabelingData_in, unsigned char **pLabelingData_out, int nWidth, int nHeight, HorizontalVerticalType nHorizontalVerticalType);

void InitVerticalAlgoMemory(int nLength);
void FreeVerticalAlgoMemory(void);
void SetVerticalConnectingData(struct txPOINT **pPoint, unsigned char *Hc, unsigned char Hhead, unsigned char Htail, unsigned char Hlenght, unsigned char *Uw, unsigned char *Dw, char *Hw, int nIDx);
struct VerticalTextureInfo *GetVerticalConnectingData(void);
void SetVerticalLength(int Length);
int GetVerticalLength(void);
void UpdateVerticalLength(int nNewLength);
int GetUpdateVerticalLength(void);

void InitHorizontalAlgoMemory(int nLength);
void FreeHorizontalAlgoMemory(void);
void SetHorizontalConnectingData(struct txPOINT **pPoint, unsigned char *Vc, unsigned char Vhead, unsigned char Vtail, unsigned char Vlenght, unsigned char *Lw, unsigned char *Rw, char *Vw, int nIDx);
struct HorizontalTextureInfo *GetHorizontalConnectingData(void);
void SetHorizontalLength(int Length);
int GetHorizontalLength(void);
void UpdateHorizontalLength(int nNewLength);
int GetUpdateHorizontalLength(void);

void GetHorizontalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight);
void GetVerticalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight);

int GetVerticalConnectingUpDownWidth(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight, unsigned char nStartX, unsigned char nStartY, unsigned char *Uw, unsigned char *Dw, int nIDx);
int GetHorizontalConnectingLeftRightWidth(short **pDifferentialData_in, unsigned char **pLabelingData_in, int nWidth, int nHeight, unsigned char nStartX, unsigned char nStartY, unsigned char *Lw, unsigned char *Rw, int nIDx);

int CheckVerticalConnectingWidthQuality(short **pDifferentialData_in, unsigned char *pUpWidth, unsigned char *pDownWidth, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx);
int CheckHorizontalConnectingWidthQuality(short **pDifferentialData_in, unsigned char *pLeftWidth, unsigned char *pRightWidth, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx);

int CheckHeadTailQuality(bool *bHeadStatus, bool *bTailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh);

typedef enum {
	emhtNone = 0,
	emhtHeadTail = 1,
	emhtHead = 2,
	emhtTail = 3
} HeadTailType;
int UpdateHeadTailConnecting(unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh, HeadTailType nHeadTailType, unsigned char *NewHead, unsigned char *NewTail);

int CheckV0ConnectingQuality(unsigned char **pLabelingData_in, char *pWeight, unsigned char nStartX, unsigned char nStartY, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType);
int CheckV0ConnectingLinePointQuality(unsigned char **pLabelingData_in, char *pWeight, unsigned char *pConnectingArray, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType);

int CheckConnectingToEndingMinutiae(bool *HeadStatus, bool *TailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, char *pWeight, int nIDx, int nQualityTh, HorizontalVerticalType nHorizontalVerticalType, unsigned char *pNewHead, unsigned char *pNewTail);
int SetEndingMinutiaeData(struct MinutiaePOINT **pEndingMinutiae, int nEndingLength, unsigned char *pConnectingArray, unsigned char NewHead, unsigned char NewTail, HorizontalVerticalType nHorizontalVerticalType);

int CheckConnectingToJumpingMinutiae(unsigned char *pConnectingArray, int nThJumpingLength, unsigned char *pJumpingArray, HorizontalVerticalType nHorizontalVerticalType);
int SetJumpingMinutiaeData(struct MinutiaePOINT **pJumpingMinutiae, int nJumpingLength, unsigned char *pJumpingArray, HorizontalVerticalType nHorizontalVerticalType);

int CheckConnectingToBifurcationMinutiae(unsigned char *pConnectingArray, unsigned char(*pBifurcationArray)[IMAGE_HEIGHT]/*unsigned char pBifurcationArray[MaxRecordItemLength][IMAGE_HEIGHT]*/, int nMaxRecordNum, int nIDx, struct BifurcationInfo *pBifurcationInfo, int *pCurrentBifurcationArrayIDx, HorizontalVerticalType nHorizontalVerticalType);
int SetBifurcationMinutiaeData(struct MinutiaePOINT **pBifurcationMinutiae, int nBifurcationLength, unsigned char *pConnectingArray, unsigned char(*pBifurcationArray)[IMAGE_HEIGHT]/*unsigned char pBifurcationArray[MaxRecordItemLength][IMAGE_HEIGHT]*/, int nMaxRecordNum, HorizontalVerticalType nHorizontalVerticalType);
void SetBifurcationInfo(struct BifurcationInfo **pBifurcationInfo_Out, struct BifurcationInfo *pBifurcationInfo, int nBifurcationInfoLength, int *pCurrentBifurcationArrayIDx);

void CheckBifurcation(unsigned char *pConnectingArray, unsigned char *pBifurcationArray, int nIDx, HorizontalVerticalType nHorizontalVerticalType);
void UpdateConnectingQualityReferenceBifurcation(bool *HeadStatus, bool *TailStatus, unsigned char Head, unsigned char Tail, unsigned char Lenght, int nIDx, int *pBifurcationFindLineArrayIndex, HorizontalVerticalType nHorizontalVerticalType);

int CheckImageConnectingToBifurcationMinutiae(unsigned char *pConnectingArray, unsigned char **pBifurcationArray, int nMaxRecordNum, int nIDx, struct BifurcationInfo *pBifurcationInfo, int *pCurrentBifurcationArrayIDx, HorizontalVerticalType nHorizontalVerticalType);
int SetImageBifurcationMinutiaeData(struct MinutiaePOINT **pBifurcationMinutiae, int nBifurcationLength, unsigned char *pConnectingArray, unsigned char **pBifurcationArray, int nMaxRecordNum, HorizontalVerticalType nHorizontalVerticalType);

//#ifdef __cplusplus
//}
//#endif
#endif