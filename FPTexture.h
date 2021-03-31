#pragma once
#ifndef __FPTEXTURE_INCLUDED__
#define __FPTEXTURE_INCLUDED__

extern char strGlobalPath[256];
extern int g_nCol_X, g_nRow_Y;

#define VALLEY_LENGTH_TH	8

#define EMC_SENSOR_SIZE		80//160//120 /* KennyKang20190619 Modify */
#define EMC_BLOCK_SIZE		8
#define EMC_BLOCK_GOOD		1
#define EMC_BLOCK_BAD		0

#pragma pack( push, 1 ) // avoid structure padding... I am using Win32 here
typedef enum {
	txUnknown = 0,
	txBound = 1,
	txEnd = 2,
	txPore = 3,
	txCross = 4,
	txTotal
} TextureType;

struct BlockInfo {
	signed char EdgeStatus[EMC_SENSOR_SIZE / EMC_BLOCK_SIZE][EMC_SENSOR_SIZE / EMC_BLOCK_SIZE];
};

struct OrientationPOINT {
	unsigned char x;
	unsigned char y;
};

struct OrientationInfo {
	struct OrientationPOINT *pValleys;
	unsigned char ValleyTotal;
	signed char theta1;
	signed char theta2;
};

struct TexturePOINT {
	unsigned char x;
	unsigned char y;
	unsigned char type;
	bool valid;
};

struct Texture {
	struct TexturePOINT *pBoundRidge;
	struct TexturePOINT *pBoundValley;
	short nTotalBoundRidge;
	short nTotalBoundValley;
};

struct TextureTemplatePOINT {
	unsigned char x;
	unsigned char y;
	short theta;
};

struct TextureTemplate {
	struct TextureTemplatePOINT *pValleysEndingPoint;
	struct TextureTemplatePOINT *pRidgesEndingPoint;
	struct TextureTemplatePOINT *pPoresPoint;
	struct TexturePOINT *pBoundValleys;
	short nTotalValleysEnding;
	short nTotalRidgesEnding;
	short nTotalPores;
	short nTotalBoundValleys;
};

typedef enum {
	txHorizontal = 0,
	txVertical = 1
} TextureSmoothType;

#pragma pack(pop) // avoid structure padding... I am using Win32 here

/*Smooth and Differential*/
int HorizontalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int VerticalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);

/*Texture Smooth and Differential*/
int TextureHorizontalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int TextureVerticalImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int TextureVerticalReduceImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);

/*Texture Smoothing*/
void TextureHorizontalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void TextureVerticalSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void TextureExpandSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType);
void TextureReduceSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType);
void Smooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight, TextureSmoothType nHVType);

/*Texture Differential*/
void TextureHorizontalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void TextureVerticalDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void TextureVerticalExpandDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void TextureVerticalReduceDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);

/*Texture Labeling*/
void TextureHorizontalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void TextureVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);

/*Texture New Labeling*/
void TextureNewVerticalLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void TextureVerticalReduceLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void TextureVerticalExpandTopAndReduceLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);

/*Texture Block Labeling*/
void TextureHorizontalBlockLabeling(short **pImage_in, unsigned char **pImage_out, struct BlockInfo *pBlockInfoData, int nWidth, int nHeight);
void TextureVerticalBlockLabeling(short **pImage_in, unsigned char **pImage_out, struct BlockInfo *pBlockInfoData, int nWidth, int nHeight);

/*Texture Merge Block Labeling*/
void TextureMergeBlockLabeling(unsigned char **pHorizontalImage_in, unsigned char **pVerticalImage_in, unsigned char **pMergeImage_out, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, struct Texture *pTexture, int nWidth, int nHeight);
void CheckAvgBoundValidData(struct Texture *pTexture, int nWidth, int nHeight);
void CheckMinDiffBoundValidData(struct Texture *pTexture, unsigned char **OgrImage_in, struct BlockInfo *pVerticalBlockInfoData, short **pMergeData, int nWidth, int nHeight);
void GetMinimumValue(short *pData, int nMaxCount, int *pIndex);
void DoubleCheckMinDiffBoundValidData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pVerticalBlockInfoData, short **pMergeData, int nWidth, int nHeight);
void GetValidValleyData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, int nWidth, int nHeight);
void GetBlockValleyDataStatus(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
void SearchValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int nStartCols, int nWidth, int nHeight);
void CheckBlockGoodValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void CheckBlockBadValleyStart(struct OrientationInfo **pOrientationInfo, unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void ReCheckValleysEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
void ReCheckGoodValleysEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
void ReCheckBadValleysEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
int GetValleysEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight);

/* Get Valleys / Ridges Ending Direction */
int ComputeDirection(int x1, int y1, int x2, int y2, int nType);
int ValleysEndingDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
int GetValleysDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
int RidgesEndingDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
int GetRidgesDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);

void RecoverData(unsigned char **pMergeImage_In, int nWidth, int nHeight);
void GetValidRidgeData(unsigned char **pMergeImage, struct Texture *pTexture, struct BlockInfo *pHorizontalBlockInfoData, struct BlockInfo *pVerticalBlockInfoData, int nWidth, int nHeight);
void SearchRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nStartCols, int nWidth, int nHeight);
void _CheckBlockGoodRidgeStart_(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void _CheckBlockBadRidgeStart_(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void CheckBlockGoodRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void CheckBlockBadRidgeStart(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void ReCheckRidgesEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
void ReCheckGoodRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
void ReCheckBadRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
int GetRidgesEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight);

int GetEndingTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight);
int GetEndingTemplateWithDirection(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight);
int GetBoundingValleysTemplate(struct TextureTemplate *pTextureTemplate, struct Texture *pTexture);

void CheckValleysBranchSearchRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
void ValleysBranchSearchGoodRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void ValleysBranchSearchBadRidges(unsigned char **pMergeImage, signed char *VerticalInValidRow, int *nStartX, int *nStartY, int nWidth, int nHeight, int nBlockType);
void ReCheckValleysBranchSearchRidgesEnding(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
void ReCheckValleysBranchSearchGoodRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
void ReCheckValleysBranchSearchBadRidgesEnding(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);

/*Save Text File Utility*/
void SaveTXT(char *filename, unsigned char**pData, int nWidth, int nHeight);
void Save2ByteTXT(char *filename, short**pData, int nWidth, int nHeight);

/*Get Org Texture Differential Data*/
void GetTextureHorizontalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight);
void GetTextureVerticalDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight);
void GetTextureVerticalReduceDifferentialData(unsigned char **p2dInData, short **p2dOutData, int nWidth, int nHeight);

/* Minutiae Get Direction Function */
int xVFComputeLineDirection(int x1, int y1, int x2, int y2);

/* Pore Function */
void CheckingDisquisePore(unsigned char **pMergeImage, int nWidth, int nHeight);
void SearchPore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight);
void SelectPore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight);
void SelectMergePore(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight);
void GetPoreDirection(unsigned char **pMergeImage, signed char *VerticalInValidRow, int nWidth, int nHeight);
int PoreDirection(unsigned char **pMergeImage, int *nStartX, int *nStartY, signed char *VerticalInValidRow, int nWidth, int nHeight, int nType);
int GetPoreTemplate(unsigned char **pMergeImage, struct TextureTemplate *pTextureTemplate, int nWidth, int nHeight);

//**********************************************************************************************************
/*Texture HVPN-Smoothing*/
void Texture_HSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_VSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_PSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_NSmooth(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);

/*Texture HVPN-Differential*/
void Texture_HDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_VDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_PDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);
void Texture_NDifferential(unsigned char **image_in, unsigned char **image_out, int nWidth, int nHeight);

/*Texture HVPN-Labeling*/
void Texture_HLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void Texture_VLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void Texture_PLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
void Texture_NLabeling(short **pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);

/*Texture HVPN-Smoothing +  HVPN-Differential*/
int Texture_HImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int Texture_VImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int Texture_PImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);
int Texture_NImageProcess(unsigned char **pInData, unsigned char **pOutData, int nWidth, int nHeight);

/*Texture Record HVPN-Labeling*/
void Texture_Record_HLabeling(unsigned char **pImage_in, int nWidth, int nHeight);
void Texture_Record_VLabeling(unsigned char **pImage_in, int nWidth, int nHeight);
//**********************************************************************************************************
#endif
