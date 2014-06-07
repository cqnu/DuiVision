#include "StdAfx.h"
#include "MenuItem.h"

CMenuItem::CMenuItem(HWND hWnd, CDuiObject* pDuiObject)
						   : CControlBaseFont(hWnd, pDuiObject)
{
	m_enButtonState = enBSNormal;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_bSelect = FALSE;
	m_bIsSeparator = FALSE;
	m_bIsPopup = FALSE;
	m_pPopupMenu = NULL;
	m_strMenuXml = _T("");

	m_nLeft = 30;
}

CMenuItem::CMenuItem(HWND hWnd, CDuiObject* pDuiObject, UINT uControlID, CRect rc, CString strTitle/*= TEXT("")*/, int nLeft/* = 30*/, BOOL bSelect/* = false*/,
					 BOOL bIsVisible/* = TRUE*/,  BOOL bIsDisable/* = FALSE*/ ,BOOL bIsPressDown/* = FALSE*/)
						   : CControlBaseFont(hWnd, pDuiObject, uControlID, rc, strTitle, bIsVisible, bIsDisable)
{
	m_enButtonState = enBSNormal;
	m_bDown = false;
	m_bMouseDown = false;
	m_uVAlignment = DT_VCENTER;
	m_bSelect = bSelect;
	m_bIsPopup = FALSE;
	m_pPopupMenu = NULL;
	m_strMenuXml = _T("");

	m_nLeft = nLeft;
}

CMenuItem::~CMenuItem(void)
{
	if((m_pPopupMenu != NULL) && IsWindow(m_pPopupMenu->GetSafeHwnd()))
	{
		m_pPopupMenu->CloseWindow();
	}
}

// ���ؼ���XML�ڵ㺯��,�ж��Ƿ����Ӳ˵�
BOOL CMenuItem::Load(TiXmlElement* pXmlElem, BOOL bLoadSubControl)
{
	BOOL bRet = __super::Load(pXmlElem);

	// �ж��Ƿ��ж����Ӳ˵�
	if(pXmlElem && (pXmlElem->FirstChildElement() != NULL))
	{
		m_bIsPopup = TRUE;
	}

	// �����Ƕ�ײ˵�(��menu����),��ͨ�����ø��˵���Load������Ƕ�ײ˵���Ϊƽ���˵�����
	CMenuEx* pParentMenu = GetParentMenu();
	if(pParentMenu && !m_strMenuXml.IsEmpty())
	{
		pParentMenu->LoadXmlFile(m_strMenuXml);
		// �����Ƕ�ײ˵�,�򷵻�FALSE,�����Ͳ��ᴴ���˲˵���,ֻ�ᴴ��Ƕ�ײ˵��ж���Ĳ˵���
		return FALSE;
	}

	return bRet;
}

// ��ȡ���˵�����
CMenuEx* CMenuItem::GetParentMenu()
{
	CDuiObject* pParentObj = GetParent();
	while((pParentObj != NULL) && (!pParentObj->IsClass("menu")))
	{
		if(pParentObj->IsClass("popup"))
		{
			pParentObj = ((CDlgPopup*)pParentObj)->GetParent();
		}else
		if(pParentObj->IsClass("dlg"))
		{
			pParentObj = ((CDlgBase*)pParentObj)->GetParent();
		}else
		{
			pParentObj = ((CControlBase*)pParentObj)->GetParent();
		}
	}
	if((pParentObj != NULL) && pParentObj->IsClass("menu"))
	{
		return (CMenuEx*)pParentObj;
	}

	return NULL;
}

// ��ʾ�����˵�
void CMenuItem::ShowPopupMenu()
{
	// ����Ѿ���ʾ���Ӳ˵�,��ֱ���˳�
	if(m_pPopupMenu)
	{
		return;
	}

	if(m_bIsPopup)
	{
		m_pPopupMenu = new CMenuEx( DuiSystem::GetDefaultFont(), 12);
		m_pPopupMenu->SetParent(this);
		CPoint point;
		CRect rc = GetRect();
		point.SetPoint(rc.left + rc.Width(), rc.top);
		int nMenuWidth = rc.Width();

		CDlgBase* pParentDlg = GetParentDialog();
		// ����˵������XML�ļ�,��ʹ�ô˲˵�����XML�ļ������Ӳ˵�
		// ������Ҹ��˵�����,�ҵ���Ӧ��XML�ļ���,ʹ�ô�XML�ļ��������Ӳ˵�
		CString strXmlFile = _T("");		
		CMenuEx* pParentMenu = GetParentMenu();
		if(pParentMenu)
		{
			if(!m_strMenuXml.IsEmpty())
			{
				// ʹ�ò˵�����XML�ļ�
				strXmlFile = m_strMenuXml;
			}else
			{
				// XML�ļ�����Ϊ���˵������XML�ļ�
				strXmlFile = pParentMenu->GetXmlFile();
			}
			// ���뽫���˵�������Զ��رչص�,�����Ӳ˵���ʾʱ��,���˵�ʧȥ����,���Զ�����
			pParentMenu->SetAutoClose(FALSE);
			// ת���Ӳ˵�������
			::ClientToScreen(pParentMenu->GetSafeHwnd(), &point);

			// �����˵���Ԥ��ֵ�˵����б����ӵ��Ӳ˵���
			for (size_t i = 0; i < pParentMenu->m_vecMenuItemValue.size(); i++)
			{
				MenuItemValue& itemValue = pParentMenu->m_vecMenuItemValue.at(i);
				m_pPopupMenu->m_vecMenuItemValue.push_back(itemValue);
			}
		}

		if(!strXmlFile.IsEmpty())
		{
			BOOL bSucc = m_pPopupMenu->LoadXmlFile(strXmlFile, pParentDlg, point, WM_DUI_MENU, GetName());
			if(bSucc)	// ���ص����˵��ɹ�����ʾ�����˵�
			{
				// ����˵���λ�ò���ʾ
				CRect rc;
				m_pPopupMenu->GetWindowRect(&rc);
				rc.OffsetRect(rc.Width()/2, 0);
				// ���������Ļ�Ҳ෶Χ,��˵����������ƶ�һЩ,�ƶ�����ǰ�˵������
				int nScreenWidth= GetSystemMetrics(SM_CXFULLSCREEN);
				if(rc.right > nScreenWidth)
				{
					//rc.OffsetRect(nScreenWidth - rc.right -10, 0);	// �ƶ�����Ļ���Ҳ�
					rc.OffsetRect(-(nMenuWidth + rc.Width()), 0);	// �ƶ�����ǰ�˵����
				}
				m_pPopupMenu->MoveWindow(rc);
				m_pPopupMenu->ShowWindow(SW_SHOW);
			}else
			{
				// �����˵�����ʧ��,ɾ���˵�����
				delete m_pPopupMenu;
				m_pPopupMenu = NULL;
			}
		}
	}
}

BOOL CMenuItem::GetCheck()
{
	return m_bDown;
}

BOOL CMenuItem::SetCheck(BOOL bCheck)
{
	if(!m_bSelect) return false;

	if (m_bIsDisable)
	{
		return m_bDown;
	}

	BOOL bDown = m_bDown;
	m_bDown = bCheck;
	if(m_bDown)
	{
		m_enButtonState = enBSDown;
	}
	else
	{
		 m_enButtonState = enBSNormal;
	}
	if(m_bDown != bDown)
	{
		UpdateControl();
	}
	return m_bDown;
}

// ��XML���ü�������
HRESULT CMenuItem::OnAttributeCheck(const CStringA& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty()) return E_FAIL;

	SetCheck(strValue == "true");

	return bLoading?S_FALSE:S_OK;
}

BOOL CMenuItem::OnControlMouseMove(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable && !m_bMouseDown)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
			{
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;
				}
				else
				{
					m_enButtonState = enBSHover;
				}
			}
			else
			{
				m_enButtonState = enBSHover;
				// ������Ӳ˵�,����ص����˵�
				if(m_bIsPopup)
				{
					ShowPopupMenu();
				}
			}
		}
		else
		{
			// ������ڵ����˵�,������겻�ڵ����˵���Χ��,��رյ����˵�
			if(m_bIsPopup && (m_pPopupMenu != NULL))
			{
				// ��鸸�˵��ĸ����Ӳ˵�,����굱ǰ�Ƿ��������˵�������
				CMenuEx* pParentMenu = GetParentMenu();
				CMenuItem* pHoverItem = pParentMenu->GetHoverMenuItem();
				if((pHoverItem != NULL) && (pHoverItem != this))
				{
					// ������������ƽ���Ĳ˵���,��ر��Ӳ˵�
					if(IsWindow(m_pPopupMenu->GetSafeHwnd()))
					{
						m_pPopupMenu->CloseWindow();
					}
					m_pPopupMenu = NULL;
				
					// ���˵��������û��Զ��ر�
					CMenuEx* pParentMenu = GetParentMenu();
					if(pParentMenu)
					{
						pParentMenu->SetAutoClose(TRUE);
						pParentMenu->SetForegroundWindow();
					}
				}
			}

			if(m_bSelect)
			{
				if(m_bDown)
				{
					m_enButtonState = enBSDown;
				}
				else
				{
					m_enButtonState = enBSNormal;
				}
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();
		return true;
	}
	return false;
}

BOOL CMenuItem::OnControlLButtonDown(UINT nFlags, CPoint point)
{
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			m_bMouseDown = true;
			if(m_bSelect)
			{
				bool bDown = m_bDown;
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;					
				}
				else
				{
					m_enButtonState = enBSHover;
				}				
				SendMessage(BUTTOM_CHECK, bDown, true);
			}
			else
			{
				m_enButtonState = enBSDown;
				SendMessage(BUTTOM_DOWN, false, false);
			}
		}
	}
	
	if(buttonState != m_enButtonState)
	{
		//UpdateControl();
		return true;
	}
	return false;
}

BOOL CMenuItem::OnControlLButtonUp(UINT nFlags, CPoint point)
{
	bool bSend = false;
	bool bbDown = false;
	bool bSelect = false;
	enumButtonState buttonState = m_enButtonState;
	if (!m_bIsDisable)
	{
		if(m_rc.PtInRect(point))
		{
			if(m_bSelect)
			{
				if(m_bDown)
				{
					m_enButtonState = enBSHoverDown;
				}
				else
				{
					m_enButtonState = enBSHover;
				}
				if(m_bMouseDown)
				{
					m_bDown = !m_bDown;
					bSend = false;
					bbDown = m_bDown;
					bSelect = true;
				}				
			}
			else
			{
				m_enButtonState = enBSHover;
				bSend = true;
				
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}			
		}
	}
	m_bMouseDown = false;
	
	if(buttonState != m_enButtonState)
	{
		UpdateControl();

		if(bSend)
		{
			SendMessage(BUTTOM_UP, bbDown, bSelect);
		}
		return true;
	}
	return false;
}

void  CMenuItem::SetControlDisable(BOOL bIsDisable)
{
	if(m_bIsDisable != bIsDisable)
	{
		m_bIsDisable = bIsDisable;
		if(bIsDisable)
		{
			if(m_bDown && m_bSelect)
			{
				m_enButtonState = enBSDisableDown;
			}
			else
			{
				m_enButtonState = enBSDisable;
			}
		}
		else
		{
			if(m_bDown)
			{
				m_enButtonState = enBSDown;
			}
			else
			{
				m_enButtonState = enBSNormal;
			}
		}
		::InvalidateRect(m_hWnd, &m_rc, true);
	}
}

void CMenuItem::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		int nImageCount = m_bSelect ? 6 : 4;
		if(m_nImagePicCount != 4)
		{
			// ����޸Ĺ�img-count����,���ô��������õ�ͼƬ����
			nImageCount = m_nImagePicCount;
		}
		if(m_bIsSeparator)
		{
			nImageCount = 1;
		}
		UpdateMemDC(dc, nWidth * nImageCount, nHeight);

		// ˢ��ͼƬ�Ĵ�С(��Ϊm_bSelectѡ���п�����SetBitmap֮���б仯)
		if(m_pImage != NULL)
		{
			if(m_bIsPopup)
			{
				m_sizeImage.SetSize(m_pImage->GetWidth() / 2, m_pImage->GetHeight());
			}else
			{
				m_sizeImage.SetSize(m_pImage->GetWidth() / nImageCount, m_pImage->GetHeight());
			}
		}

		Graphics graphics(m_memDC);
		CRect  rcTemp(0, 0, nWidth, nHeight);

		for(int i = 0; i < nImageCount; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			if(enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i)
			{
				SolidBrush brush(Color(254, 71, 156, 235));
				graphics.FillRectangle(&brush, i * nWidth, 0, nWidth, nHeight);
			}

			if(m_pImage != NULL)
			{
				if(m_bIsSeparator)
				{
					// ����Ƿָ���,�����ƽ�̷�ʽ��ͼ
					TextureBrush tileBrush(m_pImage, WrapModeTile);
					graphics.FillRectangle(&tileBrush, RectF(rcTemp.left, rcTemp.top + (nHeight - m_sizeImage.cy) / 2, nWidth, m_sizeImage.cy));
				}else
				if(m_bIsPopup)
				{
					// ����ǵ����˵�,���Ҳ�ļ�ͷͼƬ
					graphics.DrawImage(m_pImage, Rect(rcTemp.right - m_sizeImage.cx - 6, rcTemp.top + (nHeight - m_sizeImage.cy) / 2, m_sizeImage.cx, m_sizeImage.cy),
						(i % 2) * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
				}else
				{
					// ��ͨ�˵����ͼƬ
					graphics.DrawImage(m_pImage, Rect(rcTemp.left + (m_nLeft - m_sizeImage.cx) / 2, rcTemp.top + (nHeight - m_sizeImage.cy) / 2, m_sizeImage.cx, m_sizeImage.cy),
						i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);
				}
			}

			rcTemp.OffsetRect(nWidth, 0);
		}
		
		if(!m_strTitle.IsEmpty())
		{
			m_memDC.SetBkMode(TRANSPARENT);

			rcTemp.SetRect(0, 0, nWidth, nHeight);

			FontFamily fontFamily(m_strFont.AllocSysString());
			Font font(&fontFamily, (REAL)m_nFontWidth, m_fontStyle, UnitPixel);
			graphics.SetTextRenderingHint( TextRenderingHintClearTypeGridFit );

			StringFormat strFormat;
			strFormat.SetAlignment(StringAlignmentNear);
			strFormat.SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsMeasureTrailingSpaces);
			Size size = GetTextBounds(font, strFormat, m_strTitle);
			CPoint point = GetOriginPoint(nWidth - m_nLeft, nHeight, size.Width, size.Height, m_uAlignment, m_uVAlignment);

			for(int i = 0; i < nImageCount; i++)
			{
				SolidBrush solidBrush(enBSDisable == i ? Color(254, 128, 128, 128) : (enBSHover == i || (enBSDown == i && !m_bSelect) || enBSHoverDown == i ? Color(254, 255, 255, 255) : Color(254, 56, 56, 56)));

				RectF rect(m_nLeft + point.x + i * nWidth, point.y, nWidth - (m_nLeft + point.x), size.Height);
				graphics.DrawString(m_strTitle.AllocSysString(), (INT)wcslen(m_strTitle.AllocSysString()), &font, 
					rect, &strFormat, &solidBrush);				
			}
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_enButtonState * nWidth, 0, SRCCOPY);
}