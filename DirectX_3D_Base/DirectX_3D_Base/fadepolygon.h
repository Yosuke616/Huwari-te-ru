/**
 * @file	fadepolygon.h
 * @brief	�t�F�[�h�C��/�A�E�g�p�|���S������
 * @author	�u���Č�
 * @date	2021/12/23
 */
#pragma once

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFadePolygon(ID3D11Device* pDevice);
void UninitFadePolygon(void);
void UpdateFadePolygon(float);
void DrawFadePolygon(ID3D11DeviceContext* pDeviceContext);
void SetFadePolygonTexture(ID3D11ShaderResourceView* pTexture);
void SetFadePolygonPos(float fX, float fY);
void SetFadePolygonSize(float fScaleX, float fScaleY);
void SetFadePolygonAngle(float fAngle);
void SetFadePolygonUV(float fU, float fV);
void SetFadePolygonFrameSize(float fWidth, float fHeight);
void SetFadePolygonColor(float fRed, float fGreen, float fBlue);
void SetFadePolygonAlpha(float fAlpha);
