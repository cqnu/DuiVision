// DuiVision id define
//

#define					WM_SKIN								(WM_USER + 1)	// Ƥ��ѡ����Ϣ
#define					WM_MIAN_MENU						(WM_USER + 2)	// ���˵���Ϣ
#define					WM_UI_TASK							(WM_USER + 3)	// תUI�̵߳���������Ϣ
#define					WM_SYSTEM_TRAYICON					(WM_USER + 4)	// Trayͼ����Ϣ
#define					WM_USER_CLOSEWND					(WM_USER + 5)	// �Զ���Ĵ��ڹر���Ϣ

//��ʱ��
#define					IDI_TIME_TEST						1

// ϵͳ�ؼ�
#define					APP_IPC								100	// ���ڽ��̼�֪ͨ��IPC
#define					TRAY_ICON							800	// ����ͼ��
#define					BT_MIN								900	// ��С����ť
#define					BT_MAX								901	// ��󻯰�ť
#define					BT_CLOSE							902	// �رհ�ť
#define					FRAME_MAINWND						903	// �����ڵ�͸���Ƚ�����ɰ�ͼƬ
#define					BT_SKIN								904	// ������ť
#define					WND_SKIN							905	// Ƥ������
#define					BT_OK								910	// ȷ����ť
#define					BT_CANCEL							911	// ȡ����ť
#define					BT_YES								912	// �ǰ�ť
#define					BT_NO								913	// ��ť

// ϵͳ�ؼ�����
#define					NAME_TRAY_ICON						_T("tray.icon")			// ����ͼ��

#define					NAME_BT_MIN							_T("button.min")		// ��С����ť
#define					NAME_BT_MAX							_T("button.max")		// ��󻯰�ť
#define					NAME_BT_CLOSE						_T("button.close")		// �رհ�ť
#define					NAME_BT_SKIN						_T("button.skin")		// ������ť
#define					NAME_BT_SETUP						_T("button.setup")		// ���ð�ť
#define					NAME_FRAME_MAINWND					_T("frame.mainwnd")		// �����ڵ�͸���Ƚ�����ɰ�ͼƬ

#define					NAME_BT_OK							_T("button.ok")			// ȷ����ť
#define					NAME_BT_CANCEL						_T("button.cancel")		// ȡ����ť
#define					NAME_BT_YES							_T("button.yes")		// �ǰ�ť
#define					NAME_BT_NO							_T("button.no")			// ��ť

#define					NAME_SKIN_WND						_T("skin.wnd")			// Skin����
#define					NAME_BT_SKIN_CUSTOMIZE				_T("skin.button.customize")
#define					NAME_BT_SKIN_IMAGE					_T("skin.button.image")
#define					NAME_BT_SKIN_COLOR					_T("skin.button.color")
#define					NAME_SKIN_PIC_SELECT				_T("skin.pic.select")
#define					NAME_SKIN_BOX_COLOR					_T("skin.box.color")
#define					NAME_SKIN_BOX_IMAGE					_T("skin.box.image")

// ϵͳ��������
#define					ACTION_CLOSE_WINDOW					_T("close-window:")		// �رմ���
#define					ACTION_HIDE_WINDOW					_T("hide-window")		// ���ش���
#define					ACTION_SHOW_WINDOW					_T("show-window:")		// ��ʾ����

// ϵͳ��Ϣ����
#define					MSG_TRAY_DBCLICK					1						// ����˫����Ϣ


// ���̼���Ϣ���ݵĽṹ����
struct DUI_INTERPROCESS_MSG
{
	UINT	uControlID;			// �ؼ�ID
	WCHAR	wControlName[64];	// �ؼ�����
	UINT	uMsg;				// ��Ϣ
	WPARAM	wParam;				// ����1
	LPARAM	lParam;				// ����2
	WCHAR	wInfo[10240];		// ��Ϣ�ַ���
	UINT	uDataLen;			// ��չ���ֳ���
};